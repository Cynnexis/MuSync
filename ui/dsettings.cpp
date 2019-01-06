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
    //
}

void DSettings::on_cb_closeMinimizesApp_stateChanged(int check) {
	switch (check) {
		case Qt::Checked:
			//
			break;
		case Qt::Unchecked:
			//
			break;
		default: // Qt::PartiallyChecked
			break;
	}
}

void DSettings::on_cb_style_currentIndexChanged(int index) {
    //
}

void DSettings::on_sb_refreshTimeout_valueChanged(int timeout_s) {
    //
}

void DSettings::on_pb_resetSettings_clicked() {
    //
}
