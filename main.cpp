#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
//#include <QtWebEngine/qtwebengineglobal.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QCoreApplication::setApplicationName("MuSync");
	QCoreApplication::setOrganizationName("MuSync");
	
	// Initialize the web engine
	//QtWebEngine::initialize();
	
	/*MainWindow w;
	w.show();*/
	WebAPI api;
	api.connectToSpotify();
	Track track = api.getPlayingTrack();
	api.connectToGenius();
	api.getLyrics(track);
	
	return a.exec();
}
