#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QCoreApplication::setApplicationName("MuSync");
	QCoreApplication::setOrganizationName("MuSync");
	
	MainWindow w;
	w.show();
	
	return a.exec();
}
