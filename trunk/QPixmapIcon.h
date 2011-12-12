#pragma once
#include <QtGui/QPixMap>
#include <windows.h>

class QPixmapIcon
{
public:
	static QPixmap fromHICON(HICON icon);
private:
	//These are private non-member functions inside the QPixmap class
	//I'm simply exporting the code to my program so I can use them too
	static QPixmap convertHIconToPixmap(HICON icon);
	static QImage qt_fromWinHBITMAP(HDC hdc, HBITMAP bitmap, int w, int h);
};
