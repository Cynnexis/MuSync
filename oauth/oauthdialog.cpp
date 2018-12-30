#include "oauthdialog.h"
#include "ui_oauthdialog.h"

OAuthDialog::OAuthDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OAuthDialog) {
	this->setWindowFlags(Qt::Dialog | Qt::Desktop);
	init();
}

OAuthDialog::OAuthDialog(QUrl url, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::OAuthDialog) {
	init();
	load(url);
}

OAuthDialog::~OAuthDialog() {
	delete ui;
}

void OAuthDialog::init()
{
	ui->setupUi(this);
	
	this->setWindowTitle("Loading...");
	
	connect(ui->webView, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
	connect(ui->webView, SIGNAL(loadProgress(int)), this, SLOT(onLoadProgress(int)));
	connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
	connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
	connect(ui->webView, SIGNAL(iconChanged(QIcon)), this, SLOT(onIconChanged(QIcon)));
	connect(ui->webView, SIGNAL(titleChanged(QString)), this, SLOT(onTitleChanged(QString)));
}

void OAuthDialog::load(const QUrl& url) {
	ui->webView->load(url);
}

void OAuthDialog::onLoadStarted() {
	ui->pb_loading->setValue(0);
}

void OAuthDialog::onLoadProgress(int progress) {
	ui->pb_loading->setValue(progress);
}

void OAuthDialog::onLoadFinished(bool) {
	ui->pb_loading->setValue(ui->pb_loading->maximum());
}

void OAuthDialog::onUrlChanged(QUrl url) {
	ui->le_url->setText(url.toString());
	
	if (this->windowTitle() == "")
		this->setWindowTitle(url.toString());
}

void OAuthDialog::onIconChanged(QIcon icon) {
	ui->lb_icon->setPixmap(icon.pixmap(ui->lb_icon->size()));
}

void OAuthDialog::onTitleChanged(QString title) {
	this->setWindowTitle(title);
}
