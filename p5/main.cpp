#include "nwpwin.h"
#include "resource.h"

#include <list>
#include <algorithm>

// base class window is extended with on_paint virtual function
namespace {
class sel_obj {
	HDC hdc;
	HGDIOBJ hOld;
public:
	sel_obj(HDC hdc, HGDIOBJ hObj) : hdc(hdc), hOld(::SelectObject(hdc, hObj)) { }
	~sel_obj() { ::SelectObject(hdc, hOld); }
};

class draw
{
	HDC hdc;
public:
	draw(HDC hdc) : hdc(hdc) {}
	void operator()(POINT p) { ::Ellipse(hdc, p.x - 10, p.y - 10, p.x + 10, p.y + 10); }
};
} // namespace

class main_window : public vsite::nwp::window
{
	bool use_paint{ false };
	std::list<POINT> points;
protected:

	void on_command(int id) override
	{
		switch(id){
			case ID_FILE_PAINT:
				use_paint = !use_paint;
				::InvalidateRect(*this, 0, TRUE);
				points.clear();
				::CheckMenuItem(GetSubMenu(GetMenu(*this), 0), ID_FILE_PAINT, use_paint ? MF_CHECKED : MF_UNCHECKED);
				break;
			case ID_EXIT:
				::DestroyWindow(*this);
				break;
		}
	}

	void on_left_button_down(POINT p) override
	{ 
		if(use_paint)
		{
			points.push_back(p);
			RECT rc = {p.x-10, p.y-10, p.x+10, p.y+10}; 
			::InvalidateRect(*this, &rc, TRUE);
		}
		else
		{
			HDC hdc = ::GetDC(*this);
			draw d(hdc);
			d(p);
			::ReleaseDC(*this, hdc);
		}
	}

	void on_paint(HDC hdc) override  
	{ 
		::LineTo(hdc, 100, 100);
		{
			sel_obj sel_pen(hdc, ::GetStockObject(WHITE_PEN));
			sel_obj sel_brush(hdc, ::GetStockObject(HOLLOW_BRUSH));
			std::for_each(points.begin(), points.end(), draw(hdc));
		}
		::LineTo(hdc, 0, 200);
	}

	void on_destroy() override
	{
		::PostQuitMessage(0);
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	main_window w;
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 5"), (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_P5)));
	vsite::nwp::set_icons(instance, w, IDI_P5);
	vsite::nwp::application app;
	return app.run();
}
