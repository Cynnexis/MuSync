#ifndef LOADINGOVERLAY_H
#define LOADINGOVERLAY_H

#include <QWidget>
#include <QPainter>

#include "ui/overlaywidget.h"
#include "r.h"

class LoadingOverlay : public OverlayWidget
{
	Q_OBJECT

public:
	explicit LoadingOverlay(QWidget* parent = nullptr);
	
protected:
	void paintEvent(QPaintEvent* event) override;
};

#endif // LOADINGOVERLAY_H
