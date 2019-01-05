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

void DSettings::on_cb_startupBehavior_currentIndexChanged(const QString &arg1) {
    //
}

void DSettings::on_cb_closeMinimizesApp_stateChanged(int arg1) {
	switch (arg1) {
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

void DSettings::on_cb_style_currentIndexChanged(const QString &arg1) {
    //
}

void DSettings::on_sb_refreshTimeout_valueChanged(int arg1) {
    //
}

void DSettings::on_pb_resetSettings_clicked() {
    //
}
