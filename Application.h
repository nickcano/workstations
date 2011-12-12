#pragma once

#include <QtGui>
#include <QtGui/QApplication>
#include <QtGui/QPlastiqueStyle>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMenu>

#include "mainwindow.h"
#include "resource.h"
#include "QPixmapIcon.h"

class Application : QApplication
{
	Q_OBJECT
public:
	Application(int &argc, char **argv) : QApplication(argc, argv) {};

	static int runInstance(int argc, char *argv[]);


private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	int runApplication();

	MainWindow* mainWindow;
	QSystemTrayIcon* trayIcon;
	QMenu* trayIconMenu;
	void createMainWindow();
	void createTrayIcon();
	void displayTrayIcon();

};
