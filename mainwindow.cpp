#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	qRegisterMetaType<Track>("Track");
	qRegisterMetaType<SpotifyTrack>("SpotifyTrack");
	qRegisterMetaType<GeniusTrack>("GeniusTrack");
	qRegisterMetaType<Album>("Album");
	qRegisterMetaType<Artist>("Artist");
	qRegisterMetaType<QArtistList>("QArtistList");
	qRegisterMetaType<QList<GeniusTrack>>("QList<GeniusTrack>");
	
	changeTitle();
	
	pref = Preferences::getInstance(this);
	
	connect(pref, SIGNAL(startupBehaviourChanged(int)), this, SLOT(onStartupBehaviourChanged(int)));
	connect(pref, SIGNAL(styleChanged(int)), this, SLOT(onStyleChanged(int)));
	
	cb_geniusResults = new QComboBox(this);
	cb_geniusResults->setEditable(false);
	cb_geniusResults->setMinimumWidth(150);
	
	connect(cb_geniusResults, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cb_geniusResults_currentIndexChanged(int)));
	
	ui->mainToolBar->addSeparator();
	ui->mainToolBar->addWidget(cb_geniusResults);
	
	// Disable the menu until the loading screen is gone
	ui->menuBar->setEnabled(false);
	
	// Configure Tray System
	traySystem = new QSystemTrayIcon(QIcon(R::getMuSyncIcon()), this);
	connect(traySystem, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayClicked(QSystemTrayIcon::ActivationReason)));
	connect(traySystem, SIGNAL(messageClicked()), this, SLOT(onTrayMessageClicked()));
	
	trayMenu = new QMenu(qApp->applicationName(), this);
	trayMenu->addAction(ui->actionRefresh);
	trayMenu->addAction(ui->actionResumePause);
	trayMenu->addSeparator();
	trayMenu->addAction(ui->actionExit);
	
	traySystem->setContextMenu(trayMenu);
	traySystem->show();
	
	// Change the the name of "File" menu to the application name
	ui->menuFile->setTitle(qApp->applicationName());
	
	// Add some icons
	ui->menuTrack->setIcon(R::getMusic());
	ui->menuArtists->setIcon(R::getActors());
	ui->menuAlbum->setIcon(R::getAlbum());
	ui->actionRefresh->setIcon(R::getRefresh());
	ui->actionResumePause->setIcon(R::getPause());
	ui->actionSettings->setIcon(R::getSettings());
	ui->actionExit->setIcon(R::getPower());
	ui->actionOpenTrackOnSpotifyWeb->setIcon(R::getBrowser(R::getSpotifyColor()));
	ui->actionOpenAlbumOnSpotifyWeb->setIcon(R::getBrowser(R::getSpotifyColor()));
	
	ui->lb_title->addActions({ui->actionOpenTrackOnSpotifyApp, ui->actionOpenTrackOnSpotifyWeb});
	ui->lb_album->addActions({ui->actionOpenAlbumOnSpotifyApp, ui->actionOpenAlbumOnSpotifyWeb});
	
	//api = new WebAPI();
	threadAPIs = new QThread(this);
	refreshAPIs = new AutoRefreshAPI();
	
	connect(this, SIGNAL(windowAboutToBeClosed()), refreshAPIs, SLOT(stop()));
	connect(this, SIGNAL(pauseRequest()), refreshAPIs, SLOT(pause()));
	connect(this, SIGNAL(resumeRequest()), refreshAPIs, SLOT(resume()));
	
	refreshAPIs->moveToThread(threadAPIs);
	connect(threadAPIs, &QThread::started, refreshAPIs, &AutoRefreshAPI::refresh);
	connect(threadAPIs, &QThread::finished, [=]() {
		refreshAPIs->stop();
		delete refreshAPIs;
		refreshAPIs = nullptr;
	});
	
	connect(ui->actionRefresh, SIGNAL(triggered()), refreshAPIs, SLOT(refresh()));
	
	connect(refreshAPIs, SIGNAL(spotifyPlayingTrackFetched(SpotifyTrack)), this, SLOT(getTrack(SpotifyTrack)));
	connect(refreshAPIs, SIGNAL(geniusLyricsListFetched(QList<GeniusTrack>)), this, SLOT(getLyricsList(QList<GeniusTrack>)));
	connect(refreshAPIs, SIGNAL(spotifyOpenBrowser(QUrl)), this, SLOT(requestOpenBrowser(QUrl)));
	connect(refreshAPIs, SIGNAL(spotifyCloseBrowser()), this, SLOT(requestCloseBrowser()));
	connect(refreshAPIs, SIGNAL(spotifyLinkingFailed()), this, SLOT(requestCloseBrowser()));
	connect(refreshAPIs, SIGNAL(spotifyLinkingSucceeded()), this, SLOT(requestCloseBrowser()));
	connect(refreshAPIs, SIGNAL(geniusOpenBrowser(QUrl)), this, SLOT(requestOpenBrowser(QUrl)));
	connect(refreshAPIs, SIGNAL(geniusCloseBrowser()), this, SLOT(requestCloseBrowser()));
	connect(refreshAPIs, SIGNAL(geniusLinkingSucceeded()), this, SLOT(requestCloseBrowser()));
	connect(refreshAPIs, SIGNAL(geniusLinkingFailed()), this, SLOT(requestCloseBrowser()));
	
	connect(refreshAPIs, SIGNAL(APIsConnected()), this, SLOT(onAPIsConnected()));
	connect(refreshAPIs, SIGNAL(aboutToRefresh()), this, SLOT(onAboutToRefresh()));
	
	// Connect `currentTrack` to slots
	connect(&currentTrack, SIGNAL(nameChanged(QString)), this, SLOT(onTrackNameChanged(QString)));
	connect(&currentTrack, SIGNAL(artistsChanged(QArtistList)), this, SLOT(onTrackArtistsChanged(QArtistList)));
	connect(&currentTrack, SIGNAL(albumChanged(Album)), this, SLOT(onTrackAlbumChanged(Album)));
	connect(&currentTrack, SIGNAL(thumbnailChanged(QPixmap)), this, SLOT(onTrackThumbnailChanged(QPixmap)));
	
	// Connect `currentLyrics` to slots
	connect(&currentLyrics, SIGNAL(lyricsChanged(QString)), this, SLOT(onLyricsLyricsChanged(QString)));
	
	// Configure Loading Overlay
	loadingOverlay = new LoadingOverlay(this);
}

MainWindow::~MainWindow() {
	// Stop background thread
	stopThreads();
	
	// Delete graphic elements
	delete ui;
	
	if (webdialog != nullptr)
		delete webdialog;
}

void MainWindow::changeTitle(QString title) {
	if (title == "")
		this->setWindowTitle(qApp->applicationName());
	else
		this->setWindowTitle(qApp->applicationName() + " - " + title);
}

void MainWindow::changeTitle(Track track) {
	if (track.getName() == "")
		changeTitle();
	else
		changeTitle(track.toString());
}

void MainWindow::stopThreads() {
	if (refreshAPIs != nullptr && refreshAPIs->isRunning()) {
		emit windowAboutToBeClosed();
		QThread::sleep(1);
	}
	
	if (threadAPIs != nullptr && threadAPIs->isRunning()) {
		threadAPIs->exit();
		threadAPIs->wait();
	}
}

void MainWindow::closeEvent(QCloseEvent* event) {
	if (pref->getCloseButtonMinimized()) {
		hide();
		event->ignore();
	}
	else {
		stopThreads();
		event->accept();
	}
}

void MainWindow::getTrack(SpotifyTrack track) {
	// Hide the loading screen & re-enable the menu bar
	if (!loadingOverlay->isHidden()) {
		loadingOverlay->hide();
		ui->menuBar->setEnabled(true);
	}
	
	if (track.getName() != "" && currentTrack != track) {
		currentTrack = track;
		currentTrack.downloadThumbnail();
	}
}

void MainWindow::getLyricsList(QList<GeniusTrack> lyricsList) {
	ui->mainToolBar->setEnabled(true);
	ui->actionRefresh->setEnabled(true);
	ui->actionResumePause->setEnabled(true);
	if (!lyricsList.isEmpty()) {
		currentLyricsList.clear();
		currentLyricsList = lyricsList;
		
		cb_geniusResults->clear();
		for (int i = 0, max = currentLyricsList.size(); i < max; i++)
			cb_geniusResults->addItem(currentLyricsList[i].getName());
		
		// Automatically call `on_cb_geniusResults_currentIndexChanged()` slot
		cb_geniusResults->setCurrentIndex(0);
	}
}

void MainWindow::onTrackNameChanged(QString name) {
	ui->lb_title->setText(name);
	changeTitle(currentTrack);
	
	ui->menuTrack->setEnabled(true);
}

void MainWindow::onTrackArtistsChanged(QArtistList artists) {
	ui->lb_artists->setText(artists.join());
	changeTitle(currentTrack);
	
	// Update the links
	ui->menuArtists->setEnabled(true);
	ui->menuArtists->clear();
	
	for (Artist artist : artists) {
		QMenu* m = ui->menuArtists->addMenu(artist.getName());
		QAction* aApp = m->addAction(QIcon(R::getSpotify()), "Open on Spotify App");
		QAction* aWeb = m->addAction(QIcon(R::getBrowser(R::getSpotifyColor())), "Open on Spotify Web");
		// Lambda functions: https://medium.com/genymobile/how-c-lambda-expressions-can-improve-your-qt-code-8cd524f4ed9f
		connect(aApp, &QAction::triggered, [=]() {
			QDesktopServices::openUrl(QUrl(artist.getSpotifyUri()));
		});
		connect(aWeb, &QAction::triggered, [=]() {
			QDesktopServices::openUrl(QUrl(artist.getSpotifyWebUrl()));
		});
	}
}

void MainWindow::onTrackAlbumChanged(Album album) {
	ui->lb_album->setText(album.getName());
	changeTitle(currentTrack);
	
	ui->menuAlbum->setEnabled(true);
}

void MainWindow::onTrackThumbnailChanged(QPixmap thumbnail) {
	ui->lb_thumbnail->setPixmap(thumbnail
								.scaled(ui->lb_thumbnail->width(),
										ui->lb_thumbnail->height(),
										Qt::KeepAspectRatio));
}

void MainWindow::onLyricsLyricsChanged(QString lyrics) {
	if (ui->te_lyrics->toPlainText() != lyrics) {
		ui->te_lyrics->setText(lyrics);
		ui->menuGenius->setEnabled(true);
	}
}

void MainWindow::requestOpenBrowser(const QUrl& url) {	
	if (webdialog == nullptr)
		webdialog = new OAuthDialog(this);
	webdialog->load(url);
	webdialog->show();
}

void MainWindow::requestCloseBrowser() {
	if (webdialog != nullptr) {
		webdialog->close();
		webdialog->hide();
	}
}

void MainWindow::showEvent(QShowEvent* event) {
	QMainWindow::showEvent(event);
	
	threadAPIs->start();
}

void MainWindow::onTrayClicked(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::DoubleClick:
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::MiddleClick:
			show();
			break;
		default:
			break;
	}
}

void MainWindow::onTrayMessageClicked() {
	// Do something if messages are displayed
}

void MainWindow::onAPIsConnected() {
	ui->mainToolBar->setEnabled(true);
	ui->actionRefresh->setEnabled(true);
	ui->actionResumePause->setEnabled(true);
}

void MainWindow::onAboutToRefresh() {
	ui->actionRefresh->setEnabled(false);
	ui->actionResumePause->setEnabled(false);
}

void MainWindow::onStartupBehaviourChanged(int startupBehaviour) {
#ifdef QT_DEBUG
	cout << "MainWindow> Startup behaviour changed: " << startupBehaviour << endl;
#endif
	pref->runAppAtStartup(startupBehaviour >= 1);
}

void MainWindow::onStyleChanged(int style) {
	// Style is changed (from Preferences)
}

void MainWindow::on_cb_geniusResults_currentIndexChanged(int index) {
	if (0 <= index && index < currentLyricsList.size())
		currentLyrics = currentLyricsList[index];
	else
		cb_geniusResults->clear();
}

void MainWindow::on_actionRefresh_triggered() {
    // Refresh function already connecting. UI can be updated from here
}

void MainWindow::on_actionResumePause_triggered() {
	if (refreshStatus == RESUMED) {
		ui->actionResumePause->setText(tr("Resume Sync", "Resume/Pause button"));
		ui->actionResumePause->setToolTip(ui->actionResumePause->text());
		ui->actionResumePause->setIcon(R::getPlay());
		refreshStatus = PAUSED;
		emit pauseRequest();
	}
	else {
		ui->actionResumePause->setText(tr("Pause Sync", "Resume/Pause button"));
		ui->actionResumePause->setToolTip(ui->actionResumePause->text());
		ui->actionResumePause->setIcon(R::getPause());
		refreshStatus = RESUMED;
		emit resumeRequest();
	}
}

void MainWindow::on_actionSettings_triggered() {
	if (dsettings == nullptr)
		dsettings = new DSettings(this);
	
	dsettings->show();
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit(0);
}

void MainWindow::on_actionOpenTrackOnSpotifyApp_triggered() {
    QDesktopServices::openUrl(QUrl(currentTrack.getSpotifyUri()));
}

void MainWindow::on_actionOpenTrackOnSpotifyWeb_triggered() {
	QDesktopServices::openUrl(QUrl(currentTrack.getSpotifyWebUrl()));
}

void MainWindow::on_actionOpenAlbumOnSpotifyApp_triggered() {
	QDesktopServices::openUrl(QUrl(currentTrack.getAlbum().getSpotifyUri()));
}

void MainWindow::on_actionOpenAlbumOnSpotifyWeb_triggered() {
	QDesktopServices::openUrl(QUrl(currentTrack.getAlbum().getSpotifyWebUrl()));
}

void MainWindow::on_actionOpenLyricsOnGenius_triggered() {
	cout << "MainWindow> Genius URL: " << currentLyrics.getLyricsUrl().toStdString() << endl;
	QDesktopServices::openUrl(QUrl(currentLyrics.getLyricsUrl()));
}

void MainWindow::on_actionAboutMuSync_triggered() {
    QMessageBox::about(this, "About MuSync...", "MuSync is a Qt base application that fetch the lyrics of your current song on Spotify. This application use Material Design Icons from Google.");
}

void MainWindow::on_actionAboutQt_triggered() {
    QMessageBox::aboutQt(this, "About Qt...");
}
