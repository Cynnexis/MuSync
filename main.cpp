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
	
	Preferences* pref = Preferences::getInstance();
	
	if (pref->getStyle() == Preferences::STYLE_DARK)
		a.setStyle(new DarkStyle());
	
	MainWindow w;
	FramelessWindow *framelessWindow;
	
	if (pref->getStyle() == Preferences::STYLE_DARK) {
		framelessWindow = new FramelessWindow();
		framelessWindow->setContent(&w);
		framelessWindow->setWindowTitle(w.windowTitle());
		framelessWindow->setWindowIcon(w.windowIcon());
		framelessWindow->resize(w.size().width() + 100, w.size().height() + 100);
		framelessWindow->show();
	}
	else
		w.show();
	
	return a.exec();
}
