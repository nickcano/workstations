#include "QLabelButton.h"

QLabelButton::QLabelButton(QWidget * parent) : QLabel(parent)
{
	this->setMouseTracking(true);
}

void QLabelButton::mousePressEvent(QMouseEvent * event)
{
	QLabel::mousePressEvent(event);
	emit clicked();
}

void QLabelButton::leaveEvent(QEvent * event) 
{
	QLabel::leaveEvent(event);
	emit mouseLeave();
}
void QLabelButton::enterEvent(QEvent * event) 
{
	QLabel::enterEvent(event);
	emit mouseEnter();
}

