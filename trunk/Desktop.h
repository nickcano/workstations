#pragma once
#include <windows.h>
#include <string>
#include <list>

using namespace std;

class Desktop
{
public:
	HDESK instance;
	std::string name;

	static Desktop* getByName(const char* name);
	static Desktop* getCurrentDesktop();
	static Desktop* createDesktop(const char* name);
	~Desktop(void);

	static int getDesktopCount();
	static void openDesktop(const char* name);
	static void createProccess(char* path, const char* desktopName);

	void openDesktop();
	void createProccess(char* path);

protected:
	static list<string> desktops;
	Desktop(HDESK desktop, std::string title);


private:

	static BOOL CALLBACK EnumDesktopCallback(LPSTR lpszDesktop, LPARAM lParam);
};
