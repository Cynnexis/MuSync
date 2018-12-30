#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	changeTitle();
	
	pref = Preferences::getInstance(this);
	
	// Change the the name of "File" menu to the application name
	ui->menuFile->setTitle(qApp->applicationName());
	
	api = new WebAPI();
	
	connect(api, SIGNAL(spotifyPlayingTrackFetched(Track)), this, SLOT(getTrack(Track)));
	connect(api, SIGNAL(geniusLyricsFetched(QString)), this, SLOT(getLyrics(QString)));
	connect(api, SIGNAL(spotifyOpenBrowser(QUrl)), this, SLOT(requestOpenBrowser(QUrl)));
	connect(api, SIGNAL(spotifyCloseBrowser()), this, SLOT(requestCloseBrowser()));
	connect(api, SIGNAL(spotifyLinkingFailed()), this, SLOT(requestCloseBrowser()));
	connect(api, SIGNAL(spotifyLinkingSucceeded()), this, SLOT(requestCloseBrowser()));
	connect(api, SIGNAL(geniusOpenBrowser(QUrl)), this, SLOT(requestOpenBrowser(QUrl)));
	connect(api, SIGNAL(geniusCloseBrowser()), this, SLOT(requestCloseBrowser()));
	connect(api, SIGNAL(geniusLinkingSucceeded()), this, SLOT(requestCloseBrowser()));
	connect(api, SIGNAL(geniusLinkingFailed()), this, SLOT(requestCloseBrowser()));
	
	// Connect `currentTrack` to slots
	connect(&currentTrack, SIGNAL(nameChanged(QString)), this, SLOT(onTrackNameChanged(QString)));
	connect(&currentTrack, SIGNAL(artistsChanged(QArtistList)), this, SLOT(onTrackArtistsChanged(QArtistList)));
	connect(&currentTrack, SIGNAL(albumNameChanged(QString)), this, SLOT(onTrackAlbumName(QString)));
	connect(&currentTrack, SIGNAL(thumbnailChanged(QPixmap)), this, SLOT(onTrackThumbnailChanged(QPixmap)));
}

MainWindow::~MainWindow() {
	// Stop thread
	if (threadAPIs != nullptr && threadAPIs->isRunning()) {
		timerRefresh->stop();
		threadAPIs->exit();
	}
	
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
	else {
		QString artists = track.getArtists().join();
		
		if (artists != "")
			artists = " by " + artists;
		
		if (track.getAlbumName() != "")
			artists += " [" + track.getAlbumName() + "]";
		
		changeTitle(track.getName() + artists);
	}
}

void MainWindow::getTrack(Track track) {
	if (track.getName() != "" && currentTrack != track) {
		currentTrack = track;
		currentTrack.downloadThumbnail();
		changeTitle(currentTrack);
	}
}

void MainWindow::getLyrics(QString lyrics) {
	ui->actionRefresh->setEnabled(true);
	if (ui->te_lyrics->toPlainText() != lyrics)
		ui->te_lyrics->setText(lyrics);
}

void MainWindow::onTrackNameChanged(QString name) {
	ui->lb_title->setText(name);
	changeTitle(currentTrack);
}

void MainWindow::onTrackArtistsChanged(QArtistList artists) {
	ui->lb_artists->setText(artists.join());
	changeTitle(currentTrack);
}

void MainWindow::onTrackAlbumName(QString albumName) {
	ui->lb_album->setText(albumName);
	changeTitle(currentTrack);
}

void MainWindow::onTrackThumbnailChanged(QPixmap thumbnail) {
	ui->lb_thumbnail->setPixmap(thumbnail
								.scaled(ui->lb_thumbnail->width(),
										ui->lb_thumbnail->height(),
										Qt::KeepAspectRatio));
}

void MainWindow::connectAPIs() {
#ifdef QT_DEBUG
	cout << "MainWindow> Connecting to APIs..." << endl;
#endif
	
	api->connectToSpotify();
	api->connectToGenius();
	
#ifdef QT_DEBUG
	cout << "MainWindow> APIs connected." << endl;
#endif
	
	timerRefresh->setSingleShot(true);
	connect(timerRefresh, SIGNAL(timeout()), this, SLOT(refresh()));
	timerRefresh->start(1000);
	
	ui->actionRefresh->setEnabled(true);
}

void MainWindow::refresh() {
	if (timerRefresh != nullptr) {
		cout << "MainWindow> Refreshing..." << endl;
		timerRefresh->stop();
		ui->actionRefresh->setEnabled(false);
		api->getLyrics();
		timerRefresh->start(pref->getRefreshTimeout());
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
	
	// Connect the API in another thread
	timerRefresh = new QTimer(nullptr);
	timerRefresh->setSingleShot(true);
	threadAPIs = new QThread(this);
	api->moveToThread(threadAPIs);
	connect(this, SIGNAL(destroyed()), threadAPIs, SLOT(deleteLater()));
	threadAPIs->start();
	connectAPIs();
}

void MainWindow::on_actionRefresh_triggered() {
    refresh();
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit(0);
}
