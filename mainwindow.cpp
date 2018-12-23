#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	api = new WebAPI(this);
	
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
	currentTrack = track;
	currentTrack.downloadThumbnail();
}

void MainWindow::getLyrics(QString lyrics) {
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
