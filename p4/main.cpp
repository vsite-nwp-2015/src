#include "nwpwin.h"
#include "resource.h"

// there are 4 project configurations: Debug, Release, Debug Unicode, Release Unicode
// base class window is extended with TCHAR support, keyboard, mouse, timer virtual functions

class main_window : public vsite::nwp::window
{
	bool clip { false };
	int timer { 0 };
	vsite::nwp::edit edit;
	static WNDPROC orig_edit_wnd_proc;

protected:

	int on_create(CREATESTRUCT* pcs) override
	{
		edit.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, _T("Write here!"), IDC_EDIT1, 10, 10, 200, 50);
		return 0;		
	}
	void on_command(int id) override
	{
		switch(id){
			case ID_FILE_SUBCLASS:
				if(orig_edit_wnd_proc){
					::SetWindowLong(edit, GWL_WNDPROC, (LONG)orig_edit_wnd_proc); // return original
					orig_edit_wnd_proc = 0;
				}else{
					orig_edit_wnd_proc = (WNDPROC)::SetWindowLong(edit, GWL_WNDPROC, (LONG)edit_wnd_proc); // subclass
				}
				::CheckMenuItem(::GetSubMenu(::GetMenu(*this), 0), ID_FILE_SUBCLASS, orig_edit_wnd_proc ? MF_CHECKED : MF_UNCHECKED);
				break;

			case ID_FILE_TIMER:
				// start or stop timer - see OnTimer
				// interval is 1 second = 1000 miliseconds
				if(timer > 0){
					::KillTimer(*this, 1); 
					timer = 0;
					::SetWindowText(*this, _T("NWP 4"));
				}else{
					::SetTimer(*this, 1, 1000, 0);
					timer = 1;
				}
				::CheckMenuItem(::GetSubMenu(::GetMenu(*this), 0), ID_FILE_TIMER, timer ? MF_CHECKED : MF_UNCHECKED);
				break;

			case ID_FILE_CLIP:
				// this will limit cursor moving only inside this window
				RECT rc; ::GetWindowRect(*this, &rc);
				::ClipCursor((clip = !clip) ? &rc : NULL);
				::CheckMenuItem(::GetSubMenu(::GetMenu(*this), 0), ID_FILE_CLIP, clip ? MF_CHECKED : MF_UNCHECKED);
				break;

			case ID_EXIT:
				::DestroyWindow(*this);
				break;
		}
	}
	void on_timer(int id) override
	{
		tstringstream ss;
		// change main window caption
		ss << _T("NWP 4 - ") << timer++;
		SetWindowText(*this, ss.str().c_str());
	}

	void on_left_button_double_click(POINT p)
	{ 
		// display kordinates on double click
		// do not forget CS_DBLCLKS in WNDCLASS
		tstringstream ss;
		ss << _T("double click\n(") << p.x << _T(", ") << p.y << _T(")");
		MessageBox(*this, ss.str().c_str(), _T("NWP 4"), MB_OK | MB_ICONEXCLAMATION);
	}

	void on_destroy() override
	{
		::PostQuitMessage(0);
	}

	static LRESULT CALLBACK edit_wnd_proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
	{
		if(msg == WM_CHAR)
			return 0;
		return ::CallWindowProc(orig_edit_wnd_proc, hw, msg, wp, lp) ;
	}
};

WNDPROC main_window::orig_edit_wnd_proc;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	main_window w;	
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 4"), (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_P4)));
	vsite::nwp::set_icons(instance, w, IDI_P4);
	vsite::nwp::application app;
	return app.run();
}
