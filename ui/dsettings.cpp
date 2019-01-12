#include "dsettings.h"
#include "ui_dsettings.h"

DSettings::DSettings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DSettings) {
	ui->setupUi(this);
	
	pref = Preferences::getInstance(this);
	
	onStartupBehaviourChanged(pref->getStartupBehavior());
	onCloseButtonMinimizedChanged(pref->getCloseButtonMinimized());
	onStyleChanged(pref->getStyle());
	onRefreshTimeoutChanged(pref->getRefreshTimeout());
	
	isInitialization = false;
}

DSettings::~DSettings() {
	delete ui;
}

void DSettings::onStartupBehaviourChanged(int startupBehaviour) {
	if (pref->isStartupBehaviourValid(startupBehaviour)) {
		disconnect(pref, SIGNAL(startupBehaviourChanged(int)), this, SLOT(onStartupBehaviourChanged(int)));
		ui->cb_startupBehaviour->setCurrentIndex(startupBehaviour);
		connect(pref, SIGNAL(startupBehaviourChanged(int)), this, SLOT(onStartupBehaviourChanged(int)));
	}
}

void DSettings::onCloseButtonMinimizedChanged(bool closeButtonMinimized) {
	disconnect(pref, SIGNAL(closeButtonMinimizedChanged(bool)), this, SLOT(onCloseButtonMinimizedChanged(bool)));
	ui->cb_closeMinimizesApp->setCheckState(closeButtonMinimized ? Qt::Checked : Qt::Unchecked);
	connect(pref, SIGNAL(closeButtonMinimizedChanged(bool)), this, SLOT(onCloseButtonMinimizedChanged(bool)));
}

void DSettings::onStyleChanged(int style) {
	if (pref->isStyleValid(style)) {
#ifdef QT_DEBUG
		cout << "DSettings> Style changed: " << style << " (" << ui->cb_style->itemText(style).toStdString() << ")" << endl;
#endif
		disconnect(pref, SIGNAL(styleChanged(int)), this, SLOT(onStyleChanged(int)));
		ui->cb_style->setCurrentIndex(style);
		connect(pref, SIGNAL(styleChanged(int)), this, SLOT(onStyleChanged(int)));
		
		if (!isInitialization) {
			QMessageBox message(this);
			message.setWindowTitle("Restart required");
			message.setWindowIcon(R::getMuSyncIcon());
			message.setText("Please restart the application to apply the style.");
			message.exec();
		}
	}
}

void DSettings::onRefreshTimeoutChanged(int refreshTimeout) {
	if (pref->isRefreshTimeoutValid(refreshTimeout)) {
		disconnect(pref, SIGNAL(refreshTimeoutChanged(int)), this, SLOT(onRefreshTimeoutChanged(int)));
		ui->sb_refreshTimeout->setValue(refreshTimeout/1000);
		connect(pref, SIGNAL(refreshTimeoutChanged(int)), this, SLOT(onRefreshTimeoutChanged(int)));
	}
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
#ifdef QT_DEBUG
	cout << "DSettings> Style has been set to " << index << " (" << ui->cb_style->itemText(index).toStdString() << ")" << endl;
#endif
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
