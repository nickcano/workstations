#include "ScreenshotUtility.h"

DrawingSize ScreenshotUtility::getHDCSize(HDC from)
{
	DrawingSize ret;
	ret.width =  GetDeviceCaps(from, HORZRES);
	ret.height = GetDeviceCaps(from, VERTRES);
	return ret;
}

HBITMAP ScreenshotUtility::fromHDC(HDC from)
{
	HDC copyDC;
	HBITMAP bitmap;
	DrawingSize size = getHDCSize(from);

	copyDC = CreateCompatibleDC(from);

	bitmap = CreateCompatibleBitmap(from, size.width, size.height);

	SelectObject(copyDC, bitmap);      
	BitBlt(copyDC, 0,0, size.width, size.height, from, 0, 0, SRCCOPY);
	            
	DeleteDC(copyDC);

	return bitmap;
}
