#include "nwpwin.h"
#include "gdi.h"
#include "resource.h"

#include <time.h>
#include <vector>

// left click adds point
// right click clears all points
// keydown is added to string
// ESC clears string

namespace {

COLORREF get_color(HWND parent, COLORREF cur) {
	COLORREF custCols[16] = { 0 };
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof cc);
	cc.lStructSize = sizeof cc;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.hwndOwner = parent;
	cc.lpCustColors = custCols;
	cc.rgbResult = cur;
	if (ChooseColor(&cc))
		cur = cc.rgbResult;
	return cur;
}

void get_font(HWND parent, LOGFONT& lf)
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

} // namespace

class main_window : public vsite::nwp::window
{
	COLORREF fore{ RGB(0,0,0) };
	COLORREF back{ RGB(255,255,255) };
	std::vector<POINT> v;
	tstring s;
	LOGFONT lf;
	int curid {0};
	bool region{ false }, aniso{ false }, pie{ false }, winding{false};
public:
	main_window() 
	{
		::ZeroMemory(&lf, sizeof(lf));
		_tcscpy_s(lf.lfFaceName, _T("Arial"));
	}
protected:

	void on_command(int id) override
	{
		switch(id){
			case ID_EXIT:
				DestroyWindow(*this);
				return;
			case ID_OPTIONS_FOREGROUND: 
				fore = get_color(*this, fore); 
				break;
			case ID_OPTIONS_BACKGROUND: 
				back = get_color(*this, back);
				break;
			case ID_OPTIONS_FONT: 
				get_font(*this, lf); 
				break;
			case ID_OPTIONS_REGION:
				check_item(id, region);
				break;
			case ID_OPTIONS_ANISOTROPIC:
				check_item(id, aniso);
				break;
			case ID_OPTIONS_PIE:
				check_item(id, pie);
				break;
			case ID_OPTIONS_WINDING:
				check_item(id, winding);
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

	void check_item(int id , bool& b)
	{
		::CheckMenuItem(GetSubMenu(GetMenu(*this), 1), id, (b = !b) ? MF_CHECKED : MF_UNCHECKED);
	}

	void on_left_button_down(POINT p) override  
	{ 
		v.push_back(p);
		::InvalidateRect(*this, 0, true);
	}

	void on_right_button_down(POINT) override 
	{ 
		v.clear();
		::InvalidateRect(*this, 0, true);
	}

	void on_char(TCHAR c) override  
	{ 
		if(c == VK_ESCAPE) s = _T("");
		else s += c;
		::InvalidateRect(*this, 0, true);
	}

	void on_paint(HDC hdc) override  
	{ 
		RECT rc; GetClientRect(*this, &rc);

		if(region && v.size() > 2){
			vsite::nwp::gdi::region rgn(&*v.begin(), v.size());
			::FillRgn(hdc, rgn, vsite::nwp::gdi::brush(back));
			::SelectClipRgn(hdc, rgn);
		}
		switch(curid)
		{
			case ID_VIEW_PIXEL:
				draw_pixel(hdc, rc);
				break;
			case ID_VIEW_LINE:
				draw_lines(hdc, rc);
				break;
			case ID_VIEW_POLYLINE:
				draw_polyline(hdc, rc);
				break;
			case ID_VIEW_RECT:
				draw_rect(hdc, rc);
				break;
			case ID_VIEW_POLYGON:
				draw_polygon(hdc, rc);
				break;
			case ID_VIEW_TEXT:
				draw_text(hdc, rc);
				break;
			case ID_VIEW_DRAWMODE:
				draw_mode(hdc, rc);
				break;
			case ID_VIEW_MAPMODE:
				map_mode(hdc, rc);
				break;
		}
	}

	void on_destroy() override
	{
		::PostQuitMessage(0);
	}

	void draw_pixel(HDC hdc, RECT rc);
	void draw_lines(HDC hdc, RECT rc);
	void draw_polyline(HDC hdc, RECT rc);
	void draw_rect(HDC hdc, RECT rc);
	void draw_polygon(HDC hdc, RECT rc);
	void draw_text(HDC hdc, RECT rc);

	void map_mode(HDC hdc, RECT rc);
	void draw_mode(HDC hdc, RECT rc);
};

void main_window::draw_pixel(HDC hdc, RECT rc)
{
	srand(static_cast<unsigned int>(time(0)));
	for(int y=rc.top; y<rc.bottom; ++y)
		for(int x=rc.left; x<rc.right; ++x)
		{
			int r = rand()%256;
			int g = rand()%256;
			int b = rand()%256;
			::SetPixel(hdc, x, y, RGB(r, g, b));
		}
}

void main_window::draw_lines(HDC hdc, RECT rc)
{
	// PS_SOLID =0, ..., PS_DASHDOTDOT = 4
	int dy = rc.bottom/6;
	for(int i=0; i<5; ++i)
	{
		{
			vsite::nwp::gdi::pen p(fore, 1, i);
			vsite::nwp::gdi::sel_obj sel(hdc, p);
			::MoveToEx(hdc, 20, (i+1)*dy, nullptr); ::LineTo(hdc, rc.right/2, (i+1)*dy);
		}
		{
			vsite::nwp::gdi::pen p(fore, i*10, i);
			vsite::nwp::gdi::sel_obj sel(hdc, p);
			::MoveToEx(hdc, rc.right/2, (i+1)*dy, nullptr); ::LineTo(hdc, rc.right-20, (i+1)*dy);
		}
	}
}

void main_window::draw_polyline(HDC hdc, RECT rc)
{
	::Polyline(hdc, &*v.begin(), v.size());
	vsite::nwp::gdi::pen p(fore, 1, PS_SOLID);
	vsite::nwp::gdi::sel_obj sel(hdc, p);
	
	::PolyBezier(hdc, &*v.begin(), v.size());
}


void main_window::draw_polygon(HDC hdc, RECT rc)
{
	using namespace vsite::nwp::gdi;
	pen p(fore, 1, PS_SOLID);
	sel_obj sp(hdc, p);
	brush b(back);
	sel_obj sb(hdc, b);
	::SetPolyFillMode(hdc, winding ?  WINDING : ALTERNATE);
	if(v.size() > 2)
		::Polygon(hdc, &*v.begin(), v.size());
}

void main_window::draw_text(HDC hdc, RECT rc)
{
	::SetTextColor(hdc, fore);
	::SetBkColor(hdc, back);
	vsite::nwp::gdi::font f(lf);
	vsite::nwp::gdi::sel_obj sf(hdc, f);
	if(v.size()) // display text at point
		::TextOut(hdc, v[0].x, v[0].y, s.c_str(), s.size());
	else // display text in center of rectangle
		::DrawText(hdc, s.c_str(), -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
}


void main_window::draw_rect(HDC hdc, RECT rc)
{
	using namespace vsite::nwp::gdi;
	pen p(fore, 1, PS_SOLID);
	sel_obj sp(hdc, p);
	brush b(back);
	sel_obj sb(hdc, b);
	if(v.size() >= 4){
		{	// mark lines used for creating Chord (grey)
			pen p(RGB(192,192,192), 1, PS_SOLID);
			sel_obj sp(hdc, p);
			::MoveToEx(hdc, (v[0].x+v[1].x)/2, (v[0].y+v[1].y)/2, nullptr); ::LineTo(hdc, v[2].x, v[2].y);
			::MoveToEx(hdc, (v[0].x+v[1].x)/2, (v[0].y+v[1].y)/2, nullptr); ::LineTo(hdc, v[3].x, v[3].y);
		}
		if(pie)
			Pie(hdc, v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, v[3].x, v[3].y);
		else
			Chord(hdc, v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, v[3].x, v[3].y);
	}
	else if(v.size() >= 2)
		Ellipse(hdc, v[0].x, v[0].y, v[1].x, v[1].y);
}

void main_window::draw_mode(HDC hdc, RECT rc)
{
	using vsite::nwp::gdi::sel_obj;
	sel_obj sp(hdc, GetStockObject(NULL_PEN));
	sel_obj sb(hdc, GetStockObject(BLACK_BRUSH));
	::Rectangle(hdc, 0, 0, rc.right, rc.bottom/4);
	::Rectangle(hdc, 0, rc.bottom/2, rc.right, rc.bottom*3/4);
	for(int i=0; i<16; ++i){
		::SetROP2(hdc, i+1);
		sel_obj sb(hdc, GetStockObject(BLACK_BRUSH));
		::Rectangle(hdc, i*rc.right/16, 0, (i+1)*rc.right/16+1, rc.bottom/2);
		sel_obj sw(hdc, GetStockObject(WHITE_BRUSH));
		::Rectangle(hdc, i*rc.right/16, rc.bottom/2, (i+1)*rc.right/16+1, rc.bottom);
	}

}

void main_window::map_mode(HDC hdc, RECT rc)
{
	::SetMapMode(hdc, aniso ? MM_ANISOTROPIC : MM_ISOTROPIC);
	::SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
	::SetWindowExtEx(hdc, 10, 10, NULL);

	::Ellipse(hdc, 0, 0, 10, 10);
	::SetBkMode(hdc, TRANSPARENT); // do not fill text background
	for(int i=0; i<10; ++i){
		tstringstream ss; ss << i; // instead of _stprintf
		tstring s = ss.str();
		// horizontal line
		::MoveToEx(hdc, 0, i, nullptr); ::LineTo(hdc, 10, i);
		::SetTextAlign(hdc, TA_BOTTOM|TA_LEFT);
		::TextOut(hdc, 0, i, s.c_str(), s.size());
		// vertical line
		::MoveToEx(hdc, i, 0, nullptr);	::LineTo(hdc, i, 10);
		::SetTextAlign(hdc, TA_BOTTOM|TA_RIGHT);
		::TextOut(hdc, i, 10, s.c_str(), s.size());
	}
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	main_window w;
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 6"), (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_P6)));
	vsite::nwp::set_icons(instance, w, IDI_P6);
	vsite::nwp::application app;
	return app.run();
}
