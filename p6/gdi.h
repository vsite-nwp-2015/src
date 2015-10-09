class Font {
	HFONT h;
public:
	Font(const LOGFONT& lf) {
		h = ::CreateFontIndirect(&lf);
	}
	Font(const TCHAR* name, int height, bool bold=false, bool italic=false, int angle=0, bool underline=false) : h(NULL) {
		h = ::CreateFont(height, 0, angle*10, 0, bold ? FW_BOLD : FW_NORMAL, italic, underline,	0, 0, 0, 0, 0, 0, name);
	}
	~Font(){ if(h) ::DeleteObject(h); }
	operator HFONT() { return h; }
};

class Pen {
	HPEN h;
public:
	Pen(COLORREF color, int width=1, int style=PS_SOLID) :
	  h(::CreatePen(style, width, color)) {}
	~Pen(){ ::DeleteObject(h); }
	operator HPEN() { return h; }
};

class Brush {
	HBRUSH h;
public:
	Brush(COLORREF color, int hatch=-1) : 
	  h(hatch >= 0 ? CreateHatchBrush(hatch, color) : ::CreateSolidBrush(color)) {} 
	~Brush(){ ::DeleteObject(h); }
	operator HBRUSH() { return h; }
};

class Bitmap {
	HBITMAP h;
public:
	Bitmap(int w, int h, int bc, int pl=1, const void* p=0) : 
		h(::CreateBitmap(w, h, pl, bc, p)) {} 
	Bitmap(HDC hdc, int w, int h) : 
		h(::CreateCompatibleBitmap(hdc, w, h)) {} 
	~Bitmap(){ ::DeleteObject(h); }
	operator HBITMAP() { return h; }
};

class DC {
	HDC h;
protected:
	DC(HDC h) : h(h) {}
public:
	operator HDC() const { return h; }
};

class PaintDC : public DC {
	HWND hw;
	PAINTSTRUCT ps;
public:
	PaintDC(HWND hw) : hw(hw), DC(::BeginPaint(hw, &ps)) {}
	~PaintDC() { ::EndPaint(hw, &ps); }
};

class ClientDC : public DC {
	HWND hw;
public:
	ClientDC(HWND hw) : hw(hw), DC(::GetDC(hw)) {}
	~ClientDC() { ::ReleaseDC(hw, *this); }
};

class MemDC : public DC {
public:
	MemDC(HDC hdc) : DC(::CreateCompatibleDC(hdc)) {}
	~MemDC() { ::DeleteDC(*this); }
};

class Region {
	HRGN h;
public:
#ifdef UNDER_CE
	Region(const RECT& rc) : h(::CreateRectRgnIndirect(&rc)){}
#else
	Region(const RECT& rc, bool ellip=false) :
		h(ellip ? ::CreateEllipticRgnIndirect(&rc) : ::CreateRectRgnIndirect(&rc)){}
	Region(const POINT* pts, int n) :
		h(::CreatePolygonRgn(pts, n, WINDING)) {}
#endif
	~Region() { ::DeleteObject(h); }
	operator HRGN() { return h; }
};

class DCSelObj {
	HDC hdc;
	HGDIOBJ hOld;
public:
	DCSelObj(HDC hdc, HGDIOBJ hObj) : 
		hdc(hdc), hOld(::SelectObject(hdc, hObj)) { }
	~DCSelObj() { ::SelectObject(hdc, hOld);  }
};

