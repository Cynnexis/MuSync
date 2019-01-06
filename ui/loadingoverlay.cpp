#include "loadingoverlay.h"

LoadingOverlay::LoadingOverlay(QWidget* parent) : OverlayWidget (parent) {
	setAttribute(Qt::WA_TranslucentBackground);
}

void LoadingOverlay::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.fillRect(rect(), {100, 100, 100, 128});
	painter.setPen(R::getPrimaryColor());
	painter.setFont({"Arial,Helvetica", 48});
	painter.drawText(rect(), "Loading...", Qt::AlignHCenter | Qt::AlignVCenter);
}
