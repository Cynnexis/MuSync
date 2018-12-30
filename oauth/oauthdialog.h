#ifndef OAUTHDIALOG_H
#define OAUTHDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QIcon>
#include <QPixmap>

// See http://doc.qt.io/qt-5/qtwidgets-dialogs-standarddialogs-dialog-h.html
//	   http://doc.qt.io/qt-5/qtwidgets-dialogs-standarddialogs-dialog-cpp.html
// and https://gist.github.com/penk/dea91ab88e3e8c838662

namespace Ui {
	class OAuthDialog;
}

class OAuthDialog : public QDialog
{
	Q_OBJECT
public:
	explicit OAuthDialog(QWidget *parent = nullptr);
	explicit OAuthDialog(QUrl url, QWidget *parent = nullptr);
	~OAuthDialog();
	
private:
	void init();
	
public:	
	void load(const QUrl& url);
	
private slots:
	void onLoadStarted();
	void onLoadProgress(int progress);
	void onLoadFinished(bool);
	void onUrlChanged(QUrl& url);
	void onIconChanged(QIcon& icon);
	void onTitleChanged(QString& title);
	
private:
	Ui::OAuthDialog *ui;
};

#endif // OAUTHDIALOG_H
