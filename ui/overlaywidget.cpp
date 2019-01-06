#include "overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent) : QWidget(parent) {
	this->setAttribute(Qt::WA_NoSystemBackground);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);
	configureParent();
}

bool OverlayWidget::eventFilter(QObject* obj, QEvent* ev) {
	if (obj == parent()) {
		if (ev->type() == QEvent::Resize) {
			QResizeEvent* rev = static_cast<QResizeEvent*>(ev);
			
			if (rev != nullptr)
				resize(rev->size());
		}
		else if (ev->type() == QEvent::ChildAdded)
			raise();
	}
	
	return QWidget::eventFilter(obj, ev);
}

bool OverlayWidget::event(QEvent* ev) {
	if (ev->type() == QEvent::ParentAboutToChange) {
		if (parent() != nullptr)
			parent()->removeEventFilter(this);
	}
	else if (ev->type() == QEvent::ParentChange)
		configureParent();
	
	return QWidget::event(ev);
}

void OverlayWidget::configureParent() {
	if (parent() != nullptr) {
		parent()->installEventFilter(this);
		raise();
	}
}
