#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QResizeEvent>

class OverlayWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit OverlayWidget(QWidget *parent = nullptr);
	
protected:
	bool eventFilter(QObject* obj, QEvent* ev) override;
	bool event(QEvent* ev) override;
	
private:
	void configureParent();
};

#endif // OVERLAYWIDGET_H
