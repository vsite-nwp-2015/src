#include "nwpwin.h"
#include "resource.h"

// there are 4 project configurations: Debug, Release, Debug Unicode, Release Unicode
// base class Window is extended with TCHAR support, keyboard, mouse, timer virtual functions

class MyWindow : public Window
{
	bool clip;
	int timer;
	Edit edit;
	static WNDPROC origEditWndProc;

public:
	MyWindow() : timer(0), clip(false) { }

protected:

	int OnCreate(CREATESTRUCT* pcs)
	{
		edit.Create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, _T("Write here!"), IDC_EDIT1, 10, 10, 200, 50);
		return 0;		
	}
	void OnCommand(int id)
	{
		switch(id){
			case ID_FILE_SUBCLASS:
				if(origEditWndProc){
					SetWindowLong(edit, GWL_WNDPROC, (LONG)origEditWndProc); // return original
					origEditWndProc = 0;
				}else{
					origEditWndProc = (WNDPROC)SetWindowLong(edit, GWL_WNDPROC, (LONG)EditWndProc); // subclass
				}
				CheckMenuItem(GetSubMenu(GetMenu(*this), 0), ID_FILE_SUBCLASS, origEditWndProc ? MF_CHECKED : MF_UNCHECKED);
				break;

			case ID_FILE_TIMER:
				// start or stop timer - see OnTimer
				// interval is 1 second = 1000 miliseconds
				if(timer > 0){
					KillTimer(*this, 1); 
					timer = 0;
					SetWindowText(*this, _T("NWP 4"));
				}else{
					SetTimer(*this, 1, 1000, 0);
					timer = 1;
				}
				CheckMenuItem(GetSubMenu(GetMenu(*this), 0), ID_FILE_TIMER, timer ? MF_CHECKED : MF_UNCHECKED);
				break;

			case ID_FILE_CLIP:
				// this will limit cursor moving only inside this window
				RECT rc; GetWindowRect(*this, &rc);
				ClipCursor((clip = !clip) ? &rc : NULL);
				CheckMenuItem(GetSubMenu(GetMenu(*this), 0), ID_FILE_CLIP, clip ? MF_CHECKED : MF_UNCHECKED);
				break;

			case ID_EXIT:
				DestroyWindow(*this);
				break;
		}
	}
	void OnTimer(int id)
	{
		tstringstream ss;
		// change main window caption
		ss << _T("NWP 4 - ") << timer++;
		SetWindowText(*this, ss.str().c_str());
	}

	void OnLButtonDblClk(POINT p)  
	{ 
		// display kordinates on double click
		// do not forget CS_DBLCLKS in WNDCLASS
		tstringstream ss;
		ss << _T("double click\n(") << p.x << _T(", ") << p.y << _T(")");
		MessageBox(*this, ss.str().c_str(), _T("NWP 4"), MB_OK|MB_ICONEXCLAMATION);
	}

	void OnDestroy()
	{
		::PostQuitMessage(0);
	}

	static LRESULT CALLBACK EditWndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
	{
		if(msg == WM_CHAR)
			return 0;
		return CallWindowProc(origEditWndProc, hw, msg, wp, lp) ;
	}
};

WNDPROC MyWindow::origEditWndProc;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	Application app;
	MyWindow wnd;	
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_P4));
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 4"), (int)hMenu);
	// set icons
	HICON hib = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_P4), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hib));
	HICON his = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_P4), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	PostMessage(wnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(his));

	return app.Run();
}
