#pragma once
#include <windows.h>

struct DrawingSize
{
	int width, height;
};
class ScreenshotUtility
{
public:
	static HBITMAP fromHDC(HDC from);
	static DrawingSize getHDCSize(HDC from);
};
