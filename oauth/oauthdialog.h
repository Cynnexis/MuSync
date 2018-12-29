#ifndef OAUTHDIALOG_H
#define OAUTHDIALOG_H

#include <QObject>
#include <QWidget>
#include <QWebEngineView>
#include <QVBoxLayout>

// See http://doc.qt.io/qt-5/qtwidgets-dialogs-standarddialogs-dialog-h.html
//	   http://doc.qt.io/qt-5/qtwidgets-dialogs-standarddialogs-dialog-cpp.html
// and https://gist.github.com/penk/dea91ab88e3e8c838662

class OAuthDialog : public QWidget
{
	Q_OBJECT
public:
	explicit OAuthDialog(QWidget *parent = nullptr);
	
private:
	QWebEngineView webView;
	QVBoxLayout layout;
};

#endif // OAUTHDIALOG_H
