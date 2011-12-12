#pragma once

#include <QtGui/QLabel>

class QLabelButton : public QLabel
{
	Q_OBJECT
public:
	QLabelButton(QWidget* parent = 0);
	~QLabelButton(){}

signals:
	void clicked();
	void mouseLeave();
	void mouseEnter();

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void leaveEvent(QEvent * event);
	virtual void enterEvent(QEvent * event);

};
