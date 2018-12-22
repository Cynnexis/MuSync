#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	
	api = new WebAPI(this);
	
	connect(api, SIGNAL(spotifyPlayingTrackFetched(Track)), this, SLOT(getTrack(Track)));
	connect(api, SIGNAL(geniusLyricsFetched(QString)), this, SLOT(getLyrics(QString)));
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::getTrack(Track track) {
	ui->lb_title->setText(track.getName());
	ui->lb_artists->setText(track.getArtists().join(", "));
	ui->lb_album->setText(track.getAlbumName());
}

void MainWindow::getLyrics(QString lyrics) {
	ui->te_lyrics->setText(lyrics);
}
