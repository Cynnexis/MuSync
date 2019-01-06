#include "dsettings.h"
#include "ui_dsettings.h"

DSettings::DSettings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DSettings) {
	ui->setupUi(this);
	
	pref = Preferences::getInstance(this);
}

DSettings::~DSettings() {
	delete ui;
}

void DSettings::on_cb_startupBehaviour_currentIndexChanged(int index) {
    pref->setStartupBehaviour(index);
}

void DSettings::on_cb_closeMinimizesApp_stateChanged(int check) {
	switch (check) {
		case Qt::Checked:
			pref->setCloseButtonMinimized(true);
			break;
		case Qt::Unchecked:
			pref->setCloseButtonMinimized(false);
			break;
		default: // Qt::PartiallyChecked
			ui->cb_closeMinimizesApp->setCheckState(Qt::Unchecked);
			break;
	}
}

void DSettings::on_cb_style_currentIndexChanged(int index) {
    pref->setStyle(index);
}

void DSettings::on_sb_refreshTimeout_valueChanged(int timeout_s) {
    pref->setRefreshTimeout(timeout_s * 1000);
}

void DSettings::on_pb_resetSettings_clicked() {
    int answer = QMessageBox::question(this, "Clear all settings", "Are you sure you want to clear all the settings of the application? This include the credentials for the APIs Spotify and Genius.");
	
	if (answer == QMessageBox::Yes) {
#ifdef QT_DEBUG
		cout << "DSettings> Clearing all settings" << endl;
#endif
		pref->clear();
	}
}
