#ifndef DSETTINGS_H
#define DSETTINGS_H

#include <iostream>
#include <QDialog>
#include <QMessageBox>

#include "preferences.h"

using namespace std;

namespace Ui {
	class DSettings;
}

class DSettings : public QDialog
{
	Q_OBJECT
	
public:
	explicit DSettings(QWidget *parent = nullptr);
	~DSettings();
	
private slots:
	void on_cb_startupBehavior_currentIndexChanged(const QString &arg1);
	void on_cb_closeMinimizesApp_stateChanged(int arg1);
	void on_cb_style_currentIndexChanged(const QString &arg1);
	void on_sb_refreshTimeout_valueChanged(int arg1);
	void on_pb_resetSettings_clicked();
	
private:
	Ui::DSettings *ui;
	Preferences* pref;
};

#endif // DSETTINGS_H
