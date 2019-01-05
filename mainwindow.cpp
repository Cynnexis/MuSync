#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	qRegisterMetaType<Track>("Track");
	qRegisterMetaType<Album>("Album");
	qRegisterMetaType<Artist>("Artist");
	qRegisterMetaType<Lyrics>("Lyrics");
	qRegisterMetaType<QArtistList>("QArtistList");
	
	changeTitle();
	
	pref = Preferences::getInstance(this);
	
	// Change the the name of "File" menu to the application name
	ui->menuFile->setTitle(qApp->applicationName());
	
	// Add some icons
	ui->actionRefresh->setIcon(R::getRefresh());
	ui->actionExit->setIcon(R::getPower());
	ui->actionOpenTrackOnSpotifyWeb->setIcon(R::getBrowser(R::getSpotifyColor()));
	ui->actionOpenAlbumOnSpotifyWeb->setIcon(R::getBrowser(R::getSpotifyColor()));
	
	ui->lb_title->addActions({ui->actionOpenTrackOnSpotifyApp, ui->actionOpenTrackOnSpotifyWeb});
	ui->lb_album->addActions({ui->actionOpenAlbumOnSpotifyApp, ui->actionOpenAlbumOnSpotifyWeb});
	
	//api = new WebAPI();
	threadAPIs = new QThread(this);
	refreshAPIs = new AutoRefreshAPI();
	
	refreshAPIs->moveToThread(threadAPIs);
	connect(threadAPIs, &QThread::started, refreshAPIs, &AutoRefreshAPI::refresh);
	connect(threadAPIs, &QThread::finished, [=]() {refreshAPIs->stop(); refreshAPIs->deleteLater();});
	
	connect(refreshAPIs, SIGNAL(spotifyPlayingTrackFetched(Track)), this, SLOT(getTrack(Track)));
	connect(refreshAPIs, SIGNAL(geniusLyricsFetched(Lyrics)), this, SLOT(getLyrics(Lyrics)));
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
}

MainWindow::~MainWindow() {
	// Stop background thread
	if (refreshAPIs != nullptr)
		refreshAPIs->stop();
	
	if (threadAPIs != nullptr && threadAPIs->isRunning())
		threadAPIs->exit();
	
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

void MainWindow::getTrack(Track track) {
	if (track.getName() != "" && currentTrack != track) {
		currentTrack = track;
		currentTrack.downloadThumbnail();
	}
}

void MainWindow::getLyrics(Lyrics lyrics) {
	ui->actionRefresh->setEnabled(true);
	if (lyrics.getLyrics() != "" && currentLyrics != lyrics)
		currentLyrics = lyrics;
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

void MainWindow::onAPIsConnected() {
#ifdef QT_DEBUG
	cout << "MainWindow> APIs connected" << endl;
#endif
	ui->actionRefresh->setEnabled(true);
}

void MainWindow::onAboutToRefresh() {
#ifdef QT_DEBUG
	cout << "MainWindow> About to refresh" << endl;
#endif
	ui->actionRefresh->setEnabled(false);
}

void MainWindow::on_actionRefresh_triggered() {
    //refreshAPIs->refresh();
	ui->statusBar->showMessage("Err... not now, it's not a good moment...", 5000);
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
	cout << "MainWindow> Genius URL: " << currentLyrics.getGeniusUrl().toStdString() << endl;
	QDesktopServices::openUrl(QUrl(currentLyrics.getGeniusUrl()));
}

void MainWindow::on_actionAboutMuSync_triggered() {
    QMessageBox::about(this, "About MuSync...", "MuSync is a Qt base application that fetch the lyrics of your current song on Spotify. This application use Material Design Icons from Google.");
}

void MainWindow::on_actionAboutQt_triggered() {
    QMessageBox::aboutQt(this, "About Qt...");
}
