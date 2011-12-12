#include "Application.h"

int Application::runInstance(int argc, char *argv[])
{
	Application* App = new Application(argc, argv);
	return App->runApplication();
}

int Application::runApplication()
{
	this->setStyle(new QPlastiqueStyle);

	this->createMainWindow();
	this->createTrayIcon();
	this->displayTrayIcon();

	return this->exec();
}

void Application::createMainWindow()
{
	this->mainWindow = new MainWindow();
	HICON applicationIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
	this->mainWindow->setWindowIcon(QPixmapIcon::fromHICON(applicationIcon));
}

void Application::createTrayIcon()
{
	QAction* settingAction = new QAction(tr("&Settings"), this->mainWindow);
	settingAction->setCheckable(false);

	QAction* quitAction = new QAction(tr("&Quit"), this->mainWindow);
	quitAction->setCheckable(false); //future refernce: connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	trayIconMenu = new QMenu(this->mainWindow);
	trayIconMenu->addAction(settingAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this->mainWindow);
	trayIcon->setContextMenu(trayIconMenu);

	trayIcon->setIcon(this->mainWindow->windowIcon());
	trayIcon->setToolTip("Workstations");

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void Application::displayTrayIcon()
{
	trayIcon->show();
	trayIcon->showMessage("Workstations", "Click this icon to add workstations and navigate through them.", QSystemTrayIcon::MessageIcon(0), 2000);
	this->mainWindow->setFocusPolicy(Qt::StrongFocus);
}

void Application::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
		{
			this->mainWindow->showForm();
			break;
		}
		case QSystemTrayIcon::MiddleClick:
			break;
		default:
			break;
     }
 }
