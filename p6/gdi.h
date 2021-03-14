#pragma once

namespace vsite::nwp::gdi {

class font {
	HFONT h;
public:
	font(const LOGFONT& lf) {
		h = ::CreateFontIndirect(&lf);
	}
	font(const TCHAR* name, int height, bool bold = false, bool italic = false, int angle = 0, bool underline = false) : h(NULL) {
		h = ::CreateFont(height, 0, angle * 10, 0, bold ? FW_BOLD : FW_NORMAL, italic, underline, 0, 0, 0, 0, 0, 0, name);
	}
	~font() { if (h) ::DeleteObject(h); }
	operator HFONT() { return h; }
};

class pen {
	HPEN h;
public:
	pen(COLORREF color, int width = 1, int style = PS_SOLID) :
		h(::CreatePen(style, width, color)) {}
	~pen() { ::DeleteObject(h); }
	operator HPEN() { return h; }
};

class brush {
	HBRUSH h;
public:
	brush(COLORREF color, int hatch = -1) :
		h(hatch >= 0 ? CreateHatchBrush(hatch, color) : ::CreateSolidBrush(color)) {}
	~brush() { ::DeleteObject(h); }
	operator HBRUSH() { return h; }
};

class bitmap {
	HBITMAP h;
public:
	bitmap(int w, int h, int bc, int pl = 1, const void* p = 0) :
		h(::CreateBitmap(w, h, pl, bc, p)) {}
	bitmap(HDC hdc, int w, int h) :
		h(::CreateCompatibleBitmap(hdc, w, h)) {}
	~bitmap() { ::DeleteObject(h); }
	operator HBITMAP() { return h; }
};

class dc {
	HDC h;
protected:
	dc(HDC h) : h(h) {}
public:
	operator HDC() const { return h; }
};

class paint_dc : public dc {
	HWND hw;
	PAINTSTRUCT ps;
public:
	paint_dc(HWND hw) : hw(hw), dc(::BeginPaint(hw, &ps)) {}
	~paint_dc() { ::EndPaint(hw, &ps); }
};

class client_dc : public dc {
	HWND hw;
public:
	client_dc(HWND hw) : hw(hw), dc(::GetDC(hw)) {}
	~client_dc() { ::ReleaseDC(hw, *this); }
};

class mem_dc : public dc {
public:
	mem_dc(HDC hdc) : dc(::CreateCompatibleDC(hdc)) {}
	~mem_dc() { ::DeleteDC(*this); }
};

class region {
	HRGN h;
public:
	region(const RECT& rc, bool ellip = false) :
		h(ellip ? ::CreateEllipticRgnIndirect(&rc) : ::CreateRectRgnIndirect(&rc)) {}
	region(const POINT* pts, int n) :
		h(::CreatePolygonRgn(pts, n, WINDING)) {}
	~region() { ::DeleteObject(h); }
	operator HRGN() { return h; }
};

class sel_obj {
	HDC hdc;
	HGDIOBJ hOld;
public:
	sel_obj(HDC hdc, HGDIOBJ hObj) :
		hdc(hdc), hOld(::SelectObject(hdc, hObj)) { }
	~sel_obj() { ::SelectObject(hdc, hOld); }
};

} // namespace