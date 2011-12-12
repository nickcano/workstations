#include "Desktop.h"
list<string> Desktop::desktops;

Desktop::Desktop(HDESK desktop, std::string title)
{
	this->instance = desktop;
	this->name = title;
}

Desktop::~Desktop(void)
{
	CloseDesktop(this->instance);
}

BOOL CALLBACK Desktop::EnumDesktopCallback(LPSTR lpszDesktop, LPARAM lParam)
{
	desktops.push_back(string(lpszDesktop));
	return true;
}
///////////////////////////////////////////////////////////
//            Publicly Initializing class
///////////////////////////////////////////////////////////
Desktop* Desktop::getByName(const char* name)
{
	desktops.clear();
	EnumDesktopsA(GetProcessWindowStation(), Desktop::EnumDesktopCallback, NULL);

	list<string>::iterator it;
	for (it = desktops.begin(); it != desktops.end(); it++)
	{
		if (strcmp(it->c_str(), name) == 0)
			return new Desktop(OpenDesktopA(name, 0, FALSE, GENERIC_ALL), name);
	}
}

Desktop* Desktop::getCurrentDesktop()
{
	HDESK currentDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
	
	DWORD nameLength;
	char name[256];
	GetUserObjectInformationA(currentDesktop, UOI_NAME, &name, 256, &nameLength);

	return new Desktop(currentDesktop, string(name));

}

Desktop* Desktop::createDesktop(const char* name)
{
	HDESK currentDesktop = CreateDesktopA(name, NULL, NULL, 0, GENERIC_ALL, NULL);
	return new Desktop(currentDesktop, string(name));
}

///////////////////////////////////////////////////////////
//                   Static functions
///////////////////////////////////////////////////////////
int Desktop::getDesktopCount()
{
	desktops.clear();
	EnumDesktopsA(GetProcessWindowStation(), Desktop::EnumDesktopCallback, NULL);
	return desktops.size();
}
void Desktop::openDesktop(const char* name)
{
	Desktop* desktop = getByName(name);
	if (desktop)
	{
		desktop->openDesktop();
		delete desktop;
	}
}
void Desktop::createProccess(char* path, const char* desktopName)
{
	Desktop* desktop = getByName(desktopName);
	if (desktop)
	{
		desktop->createProccess(path);
		delete desktop;
	}
}

///////////////////////////////////////////////////////////
//                   Member functions
///////////////////////////////////////////////////////////
void Desktop::openDesktop()
{
	SwitchDesktop(this->instance);
}
void Desktop::createProccess(char* path)
{
	this->openDesktop();
	STARTUPINFOA si = {sizeof(si)};
	si.cb = sizeof(STARTUPINFOA);
	si.lpDesktop = (LPSTR)this->name.c_str();
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	PROCESS_INFORMATION pi;
	CreateProcessA(NULL, path, NULL, NULL, true, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}
