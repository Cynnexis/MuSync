#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	pref = Preferences::getInstance(this);
	
	// Change the the name of "File" menu to the application name
	ui->menuFile->setTitle(qApp->applicationName());
	
	api = new WebAPI(this);
	
	timer = new QTimer(this);
	timer->setSingleShot(true);
	connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
	timer->start(pref->getRefreshTimeout() + 1000);
	
	connect(api, SIGNAL(spotifyPlayingTrackFetched(Track)), this, SLOT(getTrack(Track)));
	connect(api, SIGNAL(geniusLyricsFetched(QString)), this, SLOT(getLyrics(QString)));
	
	connect(&currentTrack, SIGNAL(nameChanged(QString)), this, SLOT(onTrackNameChanged(QString)));
	connect(&currentTrack, SIGNAL(artistsChanged(QStringList)), this, SLOT(onTrackArtistsChanged(QStringList)));
	connect(&currentTrack, SIGNAL(albumNameChanged(QString)), this, SLOT(onTrackAlbumName(QString)));
	connect(&currentTrack, SIGNAL(thumbnailChanged(QPixmap)), this, SLOT(onTrackThumbnailChanged(QPixmap)));
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::getTrack(Track track) {
	if (track.getName() != "" && currentTrack != track) {
		currentTrack = track;
		currentTrack.downloadThumbnail();
	}
}

void MainWindow::getLyrics(QString lyrics) {
	if (ui->te_lyrics->toPlainText() != lyrics)
		ui->te_lyrics->setText(lyrics);
}

void MainWindow::onTrackNameChanged(QString name) {
	ui->lb_title->setText(name);
}

void MainWindow::onTrackArtistsChanged(QStringList artists) {
	ui->lb_artists->setText(artists.join(", "));
}

void MainWindow::onTrackAlbumName(QString albumName) {
	ui->lb_album->setText(albumName);
}

void MainWindow::onTrackThumbnailChanged(QPixmap thumbnail) {
	ui->lb_thumbnail->setPixmap(thumbnail
								.scaled(ui->lb_thumbnail->width(),
										ui->lb_thumbnail->height(),
										Qt::KeepAspectRatio));
}

void MainWindow::refresh() {
	cout << "MainWindow> Refreshing..." << endl;
	timer->stop();
	api->getLyrics();
	timer->start(pref->getRefreshTimeout());
}

void MainWindow::on_actionRefresh_triggered() {
    refresh();
}

void MainWindow::on_actionExit_triggered() {
    qApp->exit(0);
}
