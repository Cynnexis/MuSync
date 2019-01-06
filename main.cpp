#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

#include "DarkStyle.h"
#include "framelesswindow/framelesswindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QCoreApplication::setApplicationName("MuSync");
	QCoreApplication::setOrganizationName("MuSync");
	
	a.setStyle(new DarkStyle());
	
	FramelessWindow framelessWindow;
	MainWindow w;
	
	framelessWindow.setContent(&w);
	framelessWindow.setWindowTitle(w.windowTitle());
	framelessWindow.setWindowIcon(w.windowIcon());
	framelessWindow.show();
	
	return a.exec();
}
