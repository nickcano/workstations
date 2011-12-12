#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	this->updateThumbs = true;

	//Aero glass
	HINSTANCE DWMAPI = LoadLibraryA("dwmapi.dll");
	if (DWMAPI)
	{
		DwmExtendFrameIntoClientAreaPTR DwmExtendFrameIntoClientArea = (DwmExtendFrameIntoClientAreaPTR)GetProcAddress(DWMAPI,"DwmExtendFrameIntoClientArea");
		DwmIsCompositionEnabledPTR DwmIsCompositionEnabled = (DwmIsCompositionEnabledPTR)GetProcAddress(DWMAPI,"DwmIsCompositionEnabled");

		BOOL enabled;
		DwmIsCompositionEnabled(&enabled);
		if (enabled)
		{
			this->setStyleSheet("QMainWindow {background-color: transparent;}");
			MARGINS mar = {0}; mar.l = -1; mar.r = -1; mar.t = -1; mar.b = -1;
			DwmExtendFrameIntoClientArea ((HWND)this->winId(), &mar);
		}
	}
}

int MainWindow::countDesktops()
{
	int findCount = 1;
	while (true)
	{
		stringstream NewName;
		NewName << WORKSTATION_BASENAME << findCount;
		Desktop* d = Desktop::getByName(NewName.str().c_str());
		if (d)
		{
			findCount++;
			delete d;
		}
		else
		{
			return findCount-1;
		}
	}
}

void MainWindow::showForm()
{
	if (!this->isVisible())
	{
		this->createSelfThumbnail();

		if (this->updateThumbs)
			this->updateThumbnails();
		else
			this->updateSelfThumbnail();

		this->positionByTray();
		this->show();
		this->setFocus(Qt::MouseFocusReason);
	}
	else
		this->hide();
}

void MainWindow::positionByTray()
{
	APPBARDATA taskBarInfo;
	taskBarInfo.cbSize = sizeof(taskBarInfo);

	SHAppBarMessage(ABM_GETTASKBARPOS, &taskBarInfo);
	
	switch(taskBarInfo.uEdge)
	{
		case ABE_LEFT:
		{
			int left = taskBarInfo.rc.right + (this->frameSize().width() - this->width());
			int top = (int)taskBarInfo.rc.bottom - this->frameSize().height();
			this->setGeometry(left, top, this->width(), this->height());
			break;
		}
		case ABE_RIGHT:
		{
			int left = taskBarInfo.rc.left - this->frameSize().width();
			int top = (int)taskBarInfo.rc.bottom - this->frameSize().height();
			this->setGeometry(left, top, this->width(), this->height());
			break;
		}
		case ABE_TOP:
		{
			int left = taskBarInfo.rc.right - this->frameSize().width();
			int top = (int)taskBarInfo.rc.bottom + (this->frameSize().height() - this->height());
			this->setGeometry(left, top, this->width(), this->height());
			break;
		}
		case ABE_BOTTOM:
		{
			int left = taskBarInfo.rc.right - this->frameSize().width();
			int top = (int)taskBarInfo.rc.top - this->frameSize().height();
			this->setGeometry(left, top, this->width(), this->height());
			break;
		}
	}
}

void MainWindow::onAddClick()
{
	stringstream NewName;
	NewName << WORKSTATION_BASENAME << countDesktops() + 1;
	
	Desktop* NewDesktop = Desktop::createDesktop(NewName.str().c_str());
	if (NewDesktop)
	{
		char exeName[MAX_PATH] = "";
		GetModuleFileNameA(NULL, exeName, MAX_PATH);
		NewDesktop->createProccess("C:\\Windows\\explorer.exe");
		NewDesktop->createProccess(exeName);

		NewDesktop->openDesktop();
		this->updateThumbs = true;
		this->hide();
		delete NewDesktop;
	}
}

void MainWindow::onSwitchClick()
{
	this->createSelfThumbnail();
	QLabelButton* Sender = (QLabelButton*)QObject::sender();
	Desktop::openDesktop(Sender->objectName().toStdString().c_str());
	this->updateThumbs = true;
	this->hide();
}

void MainWindow::onSwitchEnter()
{
	QLabelButton* Sender = (QLabelButton*)QObject::sender();
	Sender->setFrameShadow(QFrame::Raised);
}

void MainWindow::onSwitchLeave()
{
	QLabelButton* Sender = (QLabelButton*)QObject::sender();
	Sender->setFrameShadow(QFrame::Sunken);
}

void MainWindow::createSelfThumbnail()
{
	Desktop* Desktop = Desktop::getCurrentDesktop();
	if (Desktop)
	{
		HDC WinDC = GetDC(GetDesktopWindow());
		HBITMAP thumbnail = ScreenshotUtility::fromHDC(WinDC);

		stringstream name; name << Desktop->name << ".png";

		QPixmap pixmap = QPixmap::fromWinHBITMAP(thumbnail);
		pixmap = pixmap.scaled(pixmap.width() / 4, pixmap.height() / 4);
		pixmap.save(QString::fromStdString(name.str()));
		
		DeleteObject(thumbnail);
		ReleaseDC(GetDesktopWindow(), WinDC);

		delete Desktop;
	}
}

void MainWindow::updateSelfThumbnail()
{
	Desktop* Desktop = Desktop::getCurrentDesktop();
	if (Desktop)
	{
		std::list<QLabelButton*>::iterator it;
		for (it = this->thumbnails.begin(); it != this->thumbnails.end(); it++)
		{
			if ((*it)->objectName() == QString::fromStdString(Desktop->name))
			{
				(*it)->setPixmap(loadDesktopThumbnail(Desktop->name));
				break;
			}
		}

		delete Desktop;
	}

}

QPixmap MainWindow::loadDesktopThumbnail(std::string name)
{
	stringstream loadname; loadname << name << ".png";
	QPixmap pixmap;
	pixmap.load(QString::fromStdString(loadname.str()));
	return pixmap;
}

void MainWindow::updateThumbnails()
{
	this->updateThumbs = false;

	std::list<QLabelButton*>::iterator it;
	for (it = this->thumbnails.begin(); it != this->thumbnails.end(); it++)
		delete *it;

	this->thumbnails.clear();

	int maximumCount = 8;
	int count = countDesktops()+2;
	bool noNew = false;
	if (count == 9)
	{
		count = 8;
		noNew = true;
	}


	HDC WinDC = GetDC(GetDesktopWindow());
	DrawingSize size = ScreenshotUtility::getHDCSize(WinDC);
	ReleaseDC(GetDesktopWindow(), WinDC);

	int halfCount = ceil(((float)count) / 2.0f);
	int index = 0;
	int width = size.width / 8;
	int height = size.height / 8;
	
	for (int i = 0; i < 2; i++)
	{
		for (int b = 0; b < halfCount && index < count; b++, index++)
		{
				QLabelButton* ScreenView = new QLabelButton(ui.centralWidget);
				stringstream NewName;
				if (index == count-1 && !noNew) //new button
				{
					NewName << "NewButton";
					QObject::connect(ScreenView, SIGNAL(clicked()), this, SLOT(onAddClick()));
					QObject::connect(ScreenView, SIGNAL(mouseLeave()), this, SLOT(onSwitchEnter()));
					QObject::connect(ScreenView, SIGNAL(mouseEnter()), this, SLOT(onSwitchLeave()));
					ScreenView->setText("New Workstation");
					ScreenView->setFrameShadow(QFrame::Raised);
				}
				else
				{
					if (index == 0) //default desktop
						NewName << "Default";
					else			//one of our desktops
						NewName << WORKSTATION_BASENAME << index;

					ScreenView->setText(QString::fromStdString(NewName.str()));
					QObject::connect(ScreenView, SIGNAL(clicked()), this, SLOT(onSwitchClick()));
					QObject::connect(ScreenView, SIGNAL(mouseLeave()), this, SLOT(onSwitchLeave()));
					QObject::connect(ScreenView, SIGNAL(mouseEnter()), this, SLOT(onSwitchEnter()));
					ScreenView->setPixmap(loadDesktopThumbnail(NewName.str()));
					ScreenView->setFrameShadow(QFrame::Sunken);
				}

				ScreenView->setAlignment(Qt::AlignCenter);
				
				ScreenView->setFrameShape(QFrame::WinPanel);
				ScreenView->setScaledContents(true);
				ScreenView->setObjectName(QString::fromStdString(NewName.str()));
				ScreenView->setGeometry(QRect(	10 + b * (width + 10),
												10 + i * (height + 10),
												width, height));
				ScreenView->show();

				this->thumbnails.push_back(ScreenView);
		}
	}

	int thiswidth = 10 + halfCount * (width + 10);
	int thisheight = 10 + 2 * (height + 10);
	this->setFixedWidth(thiswidth);
	this->setFixedHeight(thisheight);
	this->repaint();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	this->hide();
	event->ignore();
}
void MainWindow::focusOutEvent(QFocusEvent * event)
{
	this->hide();
}