#include "oauthdialog.h"

OAuthDialog::OAuthDialog(QWidget *parent) : QWidget(parent) {
	layout.addWidget(webView);
	this->setLayout(layout);
}
