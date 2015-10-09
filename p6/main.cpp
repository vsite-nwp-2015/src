#include "nwpwin.h"
#include "gdi.h"
#include "resource.h"

#include <time.h>
#include <vector>

// left click adds point
// right click clears all points
// keydown is added to string
// ESC clears string

COLORREF GetColor(HWND parent, COLORREF cur){
	COLORREF custCols[16] = {0};
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof cc);
	cc.lStructSize = sizeof cc;
	cc.Flags = CC_FULLOPEN|CC_RGBINIT;
	cc.hwndOwner = parent;
	cc.lpCustColors = custCols;
	cc.rgbResult = cur;
	if(ChooseColor(&cc))
		cur = cc.rgbResult;
	return cur;
}

void GetFont(HWND parent, LOGFONT &lf)
{
	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof cf);
	cf.lStructSize = sizeof cf;
	cf.Flags = CF_INITTOLOGFONTSTRUCT 
		| CF_SCREENFONTS | CF_EFFECTS;
	cf.hwndOwner = parent;
	cf.lpLogFont = &lf;
	ChooseFont(&cf);
}

class MyWindow : public Window
{
	COLORREF fore, back;
	std::vector<POINT> v;
	tstring s;
	LOGFONT lf;
	int curid;
	bool region, aniso, pie, winding;
public:
	MyWindow() : fore(RGB(0,0,0)), back(RGB(255,255,255)), 
		curid(0), region(false), aniso(false), pie(false), winding(false)
	{
		::ZeroMemory(&lf, sizeof(lf));
		_tcscpy(lf.lfFaceName, _T("Arial"));
	}
protected:

	void OnCommand(int id)
	{
		switch(id){
			case ID_EXIT:
				DestroyWindow(*this);
				return;
			case ID_OPTIONS_FOREGROUND: 
				fore = GetColor(*this, fore); 
				break;
			case ID_OPTIONS_BACKGROUND: 
				back = GetColor(*this, back); 
				break;
			case ID_OPTIONS_FONT: 
				GetFont(*this, lf); 
				break;
			case ID_OPTIONS_REGION:
				CheckItem(id, region);
				break;
			case ID_OPTIONS_ANISOTROPIC:
				CheckItem(id, aniso);
				break;
			case ID_OPTIONS_PIE:
				CheckItem(id, pie);
				break;
			case ID_OPTIONS_WINDING:
				CheckItem(id, winding);
				break;
			case ID_HELP_INSTRUCTIONS:
				TCHAR s[1024]; LoadString(0, IDS_INSTR, s, sizeof(s));
				MessageBox(*this, s, _T("NWP 6"), MB_OK);
				break;
			default:
				curid = id;
				break;
		}
		InvalidateRect(*this, 0, true);
	}

	void CheckItem(int id , bool& b)
	{
		CheckMenuItem(GetSubMenu(GetMenu(*this), 1), id, (b = !b) ? MF_CHECKED : MF_UNCHECKED);
	}
	void OnLButtonDown(POINT p)  
	{ 
		v.push_back(p);
		InvalidateRect(*this, 0, true);
	}

	void OnRButtonDown(POINT)  
	{ 
		v.clear();
		InvalidateRect(*this, 0, true);
	}

	void OnChar(TCHAR c)  
	{ 
		if(c == VK_ESCAPE) s = _T("");
		else s += c;
		InvalidateRect(*this, 0, true);
	}

	void OnPaint(HDC hdc)  
	{ 
		RECT rc; GetClientRect(*this, &rc);

		if(region && v.size() > 2){
			Region rgn(&*v.begin(), v.size());
			FillRgn(hdc, rgn, Brush(back));
			SelectClipRgn(hdc, rgn);
		}
		switch(curid)
		{
			case ID_VIEW_PIXEL:
				DrawPixel(hdc, rc);
				break;
			case ID_VIEW_LINE:
				DrawLines(hdc, rc);
				break;
			case ID_VIEW_POLYLINE:
				DrawPolyline(hdc, rc);
				break;
			case ID_VIEW_RECT:
				DrawRect(hdc, rc);
				break;
			case ID_VIEW_POLYGON:
				DrawPolygon(hdc, rc);
				break;
			case ID_VIEW_TEXT:
				DrawText(hdc, rc);
				break;
			case ID_VIEW_DRAWMODE:
				DrawMode(hdc, rc);
				break;
			case ID_VIEW_MAPMODE:
				MapMode(hdc, rc);
				break;
		}
	}

	void OnDestroy()
	{
		::PostQuitMessage(0);
	}

	void DrawPixel(HDC hdc, RECT rc);
	void DrawLines(HDC hdc, RECT rc);
	void DrawPolyline(HDC hdc, RECT rc);
	void DrawRect(HDC hdc, RECT rc);
	void DrawPolygon(HDC hdc, RECT rc);
	void DrawText(HDC hdc, RECT rc);

	void MapMode(HDC hdc, RECT rc);
	void DrawMode(HDC hdc, RECT rc);
};

void MyWindow::DrawPixel(HDC hdc, RECT rc)
{
	srand(time(0));
	for(int y=rc.top; y<rc.bottom; ++y)
		for(int x=rc.left; x<rc.right; ++x)
		{
			int r = rand()%256;
			int g = rand()%256;
			int b = rand()%256;
			SetPixel(hdc, x, y, RGB(r, g, b));
		}
}

void MyWindow::DrawLines(HDC hdc, RECT rc)
{
	// PS_SOLID =0, ..., PS_DASHDOTDOT = 4
	int dy = rc.bottom/6;
	for(int i=0; i<5; ++i)
	{
		{
			Pen p(fore, 1, i);
			DCSelObj sel(hdc, p);
			MoveToEx(hdc, 20, (i+1)*dy, NULL); LineTo(hdc, rc.right/2, (i+1)*dy);
		}
		{
			Pen p(fore, i*10, i);
			DCSelObj sel(hdc, p);
			MoveToEx(hdc, rc.right/2, (i+1)*dy, NULL); LineTo(hdc, rc.right-20, (i+1)*dy);
		}
	}

}


void MyWindow::DrawPolyline(HDC hdc, RECT rc)
{
	Polyline(hdc, &*v.begin(), v.size());
	Pen p(fore, 1, PS_SOLID);
	DCSelObj sel(hdc, p);
	
	PolyBezier(hdc, &*v.begin(), v.size());
}


void MyWindow::DrawPolygon(HDC hdc, RECT rc)
{

	Pen p(fore, 1, PS_SOLID);
	DCSelObj sp(hdc, p);
	Brush b(back);
	DCSelObj sb(hdc, b);
	SetPolyFillMode(hdc, winding ?  WINDING : ALTERNATE);
	if(v.size() > 2)
		Polygon(hdc, &*v.begin(), v.size());
}

void MyWindow::DrawText(HDC hdc, RECT rc)
{
	::SetTextColor(hdc, fore);
	::SetBkColor(hdc, back);
	Font f(lf);
	DCSelObj sf(hdc, f);
	if(v.size()) // display text at point
		::TextOut(hdc, v[0].x, v[0].y, s.c_str(), s.size());
	else // display text in center of rectangle
		::DrawText(hdc, s.c_str(), -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}


void MyWindow::DrawRect(HDC hdc, RECT rc)
{
	Pen p(fore, 1, PS_SOLID);
	DCSelObj sp(hdc, p);
	Brush b(back);
	DCSelObj sb(hdc, b);
	if(v.size() >= 4){
		{	// mark lines used for creating Chord (grey)
			Pen p(RGB(192,192,192), 1, PS_SOLID);
			DCSelObj sp(hdc, p);
			MoveToEx(hdc, (v[0].x+v[1].x)/2, (v[0].y+v[1].y)/2, NULL); LineTo(hdc, v[2].x, v[2].y);
			MoveToEx(hdc, (v[0].x+v[1].x)/2, (v[0].y+v[1].y)/2, NULL); LineTo(hdc, v[3].x, v[3].y);
		}
		if(pie)
			Pie(hdc, v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, v[3].x, v[3].y);
		else
			Chord(hdc, v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, v[3].x, v[3].y);
	}
	else if(v.size() >= 2)
		Ellipse(hdc, v[0].x, v[0].y, v[1].x, v[1].y);
}

void MyWindow::DrawMode(HDC hdc, RECT rc)
{
	DCSelObj sp(hdc, GetStockObject(NULL_PEN));
	DCSelObj sb(hdc, GetStockObject(BLACK_BRUSH));
	Rectangle(hdc, 0, 0, rc.right, rc.bottom/4);
	Rectangle(hdc, 0, rc.bottom/2, rc.right, rc.bottom*3/4);
	for(int i=0; i<16; ++i){
		SetROP2(hdc, i+1);
		DCSelObj sb(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, i*rc.right/16, 0, (i+1)*rc.right/16+1, rc.bottom/2);
		DCSelObj sw(hdc, GetStockObject(WHITE_BRUSH));
		Rectangle(hdc, i*rc.right/16, rc.bottom/2, (i+1)*rc.right/16+1, rc.bottom);
	}

}

void MyWindow::MapMode(HDC hdc, RECT rc)
{
	SetMapMode(hdc, aniso ? MM_ANISOTROPIC : MM_ISOTROPIC);
	SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
	SetWindowExtEx(hdc, 10, 10, NULL);

	Ellipse(hdc, 0, 0, 10, 10);
	SetBkMode(hdc, TRANSPARENT); // do not fill text background
	for(int i=0; i<10; ++i){
		tsstream ss; ss << i; // instead of _stprintf
		tstring s = ss.str();
		// horizontal line
		MoveToEx(hdc, 0, i, 0); LineTo(hdc, 10, i); 
		SetTextAlign(hdc, TA_BOTTOM|TA_LEFT);
		TextOut(hdc, 0, i, s.c_str(), s.size());
		// vertical line
		MoveToEx(hdc, i, 0, 0);	LineTo(hdc, i, 10); 
		SetTextAlign(hdc, TA_BOTTOM|TA_RIGHT);
		TextOut(hdc, i, 10, s.c_str(), s.size());
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MyWindow wnd;	
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_P6));
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 6"), (int)hMenu);
	// set icons
	HICON hib = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_P6), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hib));
	HICON his = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_P6), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(his));

	return app.Run();
}
