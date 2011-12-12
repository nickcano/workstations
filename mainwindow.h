#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QCloseEvent>
#include "ui_mainwindow.h"

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <list>
#include <sstream>
#include <math.h>

#include "Desktop.h"
#include "QLabelButton.h"
#include "ScreenshotUtility.h"

using namespace std;

#define WORKSTATION_BASENAME "workstation"


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = (Qt::WFlags)(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
	Ui::MainWindowClass ui;

	void showForm();


public slots:
	void onAddClick();
	
	void onSwitchClick();
	void onSwitchEnter();
	void onSwitchLeave();

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void focusOutEvent(QFocusEvent * event);

private:
	list<QLabelButton*> thumbnails;
	bool updateThumbs;
	void positionByTray();
	void createSelfThumbnail();
	void updateSelfThumbnail();
	QPixmap loadDesktopThumbnail(std::string name);
	void updateThumbnails();


	int countDesktops();

	struct MARGINS
	{
		DWORD l, r, t, b;
	};
	typedef void (*DwmExtendFrameIntoClientAreaPTR)(HWND handle, MARGINS* m);  
	typedef void (*DwmIsCompositionEnabledPTR)(BOOL* enabled);  
	typedef void (*DwmEnableCompositionPTR)(UINT uCompositionAction);



};

#endif // MAINWINDOW_H
