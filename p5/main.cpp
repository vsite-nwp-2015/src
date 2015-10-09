#include "nwpwin.h"
#include "resource.h"

#include <list>
#include <algorithm>

// base class Window is extended with OnPaint virtual function


class SelObj {
	HDC hdc;
	HGDIOBJ hOld;
public:
	SelObj(HDC hdc, HGDIOBJ hObj) : hdc(hdc), hOld(::SelectObject(hdc, hObj)) { }
	~SelObj() { ::SelectObject(hdc, hOld);  }
};

class draw
{
	HDC hdc;
public:
	draw(HDC hdc) : hdc(hdc) {}
	void operator()(POINT p) { Ellipse(hdc, p.x-10, p.y-10, p.x+10, p.y+10); }
};

class MyWindow : public Window
{
	bool usePaint;
	std::list<POINT> points;
public:
	MyWindow() : usePaint(false) {}
protected:

	int OnCreate(CREATESTRUCT* pcs)
	{
		return 0;		
	}
	void OnCommand(int id)
	{
		switch(id){
			case ID_FILE_PAINT:
				usePaint = !usePaint;
				InvalidateRect(*this, 0, TRUE);
				points.clear();
				CheckMenuItem(GetSubMenu(GetMenu(*this), 0), ID_FILE_PAINT, usePaint ? MF_CHECKED : MF_UNCHECKED);
				break;
			case ID_EXIT:
				DestroyWindow(*this);
				break;
		}
	}

	void OnLButtonDown(POINT p)  
	{ 
		if(usePaint)
		{
			points.push_back(p);
			RECT rc = {p.x-10, p.y-10, p.x+10, p.y+10}; 
			InvalidateRect(*this, &rc, TRUE);
		}
		else
		{
			HDC hdc = GetDC(*this);
			draw d(hdc);
			d(p);
			ReleaseDC(*this, hdc);
		}
	}

	void OnPaint(HDC hdc)  
	{ 
		::LineTo(hdc, 100, 100);
		{
			SelObj selpen(hdc, ::GetStockObject(WHITE_PEN));
			SelObj selbrush(hdc, ::GetStockObject(HOLLOW_BRUSH));
			std::for_each(points.begin(), points.end(), draw(hdc));
		}
		::LineTo(hdc, 0, 200);
	}

	void OnDestroy()
	{
		::PostQuitMessage(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MyWindow wnd;	
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_P5));
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 5"), (int)hMenu);
	// set icons
	HICON hib = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_P5), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hib));
	HICON his = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_P5), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(his));

	return app.Run();
}
