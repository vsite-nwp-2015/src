#include <windows.h>
#include <windowsx.h>

#include "res.h"

void OnCreate(HWND hw){
	char s[128];
	LoadString(0, IDS_B1, s, sizeof s);
	CreateWindow("BUTTON", s,  WS_CHILD | WS_VISIBLE, 50, 50, 80, 24,	hw, (HMENU)IDC_BUTTON1, 0, 0); 
	LoadString(0, IDS_B2, s, sizeof s);
	CreateWindow("BUTTON", s,  WS_CHILD | WS_VISIBLE, 50, 80, 80, 24,	hw, (HMENU)IDC_BUTTON2, 0, 0); 
}

void OnCommand(HWND hw, int id, int code){
	switch(id){
		case IDC_BUTTON1:
			{
			HRSRC hRc = FindResource(0, MAKEINTRESOURCE(ID_BIN), "MYTYPE");
			HGLOBAL h = LoadResource(0, hRc);
			void* pData = LockResource(h);
			char* pText = reinterpret_cast<char*>(pData);
			MessageBox(hw, pText, "NWP 2", MB_OK|MB_ICONWARNING);
			FreeResource(h);
			}
			break;
		case IDC_BUTTON2:
			MoveWindow(GetDlgItem(hw, IDC_BUTTON2), 150, 80, 80, 24, TRUE);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hw);
			break;
		case ID_HELP_ABOUT:
			MessageBox(hw, "About 1", "NWP 2", MB_OK|MB_ICONWARNING);
			break;
	}
}

void OnContextMenu(HWND hw){
	HMENU hPopup = CreatePopupMenu();
	AppendMenu(hPopup, MF_STRING, ID_HELP_ABOUT, "&About");
	AppendMenu(hPopup, MF_SEPARATOR, 0, NULL);
	AppendMenu(hPopup, MF_STRING, ID_FILE_EXIT, "E&xit");
	DWORD pos = GetMessagePos();
	int x = GET_X_LPARAM(pos), y = GET_Y_LPARAM(pos);

	// TrackPopupMenu(hPopup, 0, x, y, 0, hwnd, 0); 	// verzija 1: salje WM_COMMAND prozoru

	UINT id = TrackPopupMenu(hPopup, TPM_RETURNCMD, x, y, 0, hw, 0); 	// verzija 2: vraca odabrani id (TPM_RETURNCMD)
	if(id == ID_HELP_ABOUT)
		MessageBox(hw, "About 2", "NWP 2", MB_OK|MB_ICONWARNING);
}

LRESULT CALLBACK WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
			OnCreate(hw);
			return 0;
		case WM_COMMAND:
			OnCommand(hw, LOWORD(wp), HIWORD(wp));
			return 0;
		case WM_CONTEXTMENU:
			OnContextMenu(hw);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hw, msg, wp, lp);
}


int RegisterMyClass(HINSTANCE hInstance, char* className)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof wc);

	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P2));
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_P2));
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	return RegisterClass(&wc);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	char clsName[] = "NWPClass";

	if(!RegisterMyClass(hInstance, clsName))
		return 0;

	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_P2));
	HWND hwnd = CreateWindow(clsName, "NWP 2",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, hMenu, hInstance, NULL); 

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage (&msg) ; 
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

