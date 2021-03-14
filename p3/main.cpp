#include <windows.h>
#include <string>
#include "resource.h"

//////////////////////////////////////////////////////////////////////////////////////////
// modeless dialog functions                                                               //
//////////////////////////////////////////////////////////////////////////////////////////

HWND hDlgModeless;

BOOL CALLBACK ModelessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
			return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// modal dialog functions                                                               //
//////////////////////////////////////////////////////////////////////////////////////////

void OnInitDialog(HWND hDlg)
{
	// edit
	SetDlgItemText(hDlg, IDC_EDIT1, "abcd");
	SetDlgItemInt(hDlg, IDC_EDIT2, 123, true);
	// check box
	CheckDlgButton(hDlg, IDC_CHECK1, BST_CHECKED);
	// radio buttons
	CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
	// combo box
	for(int i=0; i<10; ++i){
		std::string s { "item " + std::to_string(i) };
		SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)s.c_str());
	}
	SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 1, 0);
	// list box
	for(int i=0; i<10; ++i){
		std::string s { "item " + std::to_string(i) };
		SendDlgItemMessage(hDlg, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)s.c_str());
	}
	SendDlgItemMessage(hDlg, IDC_LIST1, LB_SETCURSEL, 2, 0);
}


void OnOK(HWND hDlg)
{
	char s[64]; 
	GetDlgItemText(hDlg, IDC_EDIT1, s, sizeof s);
	
	BOOL suc; 
	int n = GetDlgItemInt(hDlg, IDC_EDIT2, &suc, true);

	int comboboxsel = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0);
	
	int listboxsel = SendDlgItemMessage(hDlg, IDC_LIST1, LB_GETCURSEL, 0, 0);
}

BOOL CALLBACK ModalDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
			OnInitDialog(hDlg);
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_CHECK1:
					EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED);
					EnableWindow(GetDlgItem(hDlg, IDC_EDIT2), IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED);
					return TRUE;
				case IDOK:
					OnOK(hDlg);
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
			}
			break;	
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// common dialogs                                                                       //
//////////////////////////////////////////////////////////////////////////////////////////

void FileOpen(HWND parent)
{
	char path[MAX_PATH]; *path = 0;

	char filter[] = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof ofn);
	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = parent;
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter;

	ofn.Flags = OFN_HIDEREADONLY;
	if(GetOpenFileName(&ofn))
		MessageBox(0, ofn.lpstrFile, "NWP", MB_OK);
}

void FileSave(HWND parent)
{
	char path[MAX_PATH]; *path = 0;

	char filter[] = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof ofn);
	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = parent;
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter;

	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(GetSaveFileName(&ofn))
		MessageBox(0, ofn.lpstrFile, "NWP", MB_OK);
}


void Color(HWND parent)
{
	static COLORREF myCol = RGB(255,128,0);

	COLORREF custCols[16] = {0};
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof cc);
	cc.lStructSize = sizeof cc;
	cc.Flags = CC_FULLOPEN|CC_RGBINIT;
	cc.hwndOwner = parent;
	cc.lpCustColors = custCols;
	cc.rgbResult = myCol;
	if(ChooseColor(&cc))
		myCol = cc.rgbResult;
}

void Font(HWND parent)
{
	static LOGFONT lf = {0}; 
	if(0 == *lf.lfFaceName) strcpy_s(lf.lfFaceName, "Arial");

	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof cf);
	cf.lStructSize = sizeof cf;
	cf.Flags = CF_INITTOLOGFONTSTRUCT 
		| CF_SCREENFONTS | CF_EFFECTS;
	cf.hwndOwner = parent;
	cf.lpLogFont = &lf;
	ChooseFont(&cf);
}

//////////////////////////////////////////////////////////////////////////////////////////
// main window functions                                                                //
//////////////////////////////////////////////////////////////////////////////////////////

void OnCommand(HWND hw, int id, int code){
	switch(id){
		case ID_DIALOG_COMMON_COLOR:
			Color(hw);
			break;
		case ID_DIALOG_COMMON_FONT:
			Font(hw);
			break;
		case ID_DIALOG_COMMON_OPEN:
			FileOpen(hw);
			break;
		case ID_DIALOG_COMMON_SAVE:
			FileSave(hw);
			break;
		case ID_DIALOG_MODAL:
			if(IDOK == DialogBox(0, MAKEINTRESOURCE(IDD_DIALOG1), hw, ModalDlgProc))
				MessageBox(hw, "OK!", "NWP", MB_OK);
			break;
		case ID_DIALOG_MODELESS:
			{
			if(0 == hDlgModeless)
				hDlgModeless = CreateDialog(0, MAKEINTRESOURCE(IDD_DIALOG2), hw, ModelessDlgProc);
			bool vis = IsWindowVisible(hDlgModeless) != 0;
			ShowWindow(hDlgModeless, vis ? SW_HIDE : SW_SHOW);
			CheckMenuItem(GetSubMenu(GetMenu(hw), 0), ID_DIALOG_MODELESS, !vis ? MF_CHECKED : MF_UNCHECKED);
			}
			break;
		case ID_EXIT:
			DestroyWindow(hw);
			break;
	}
}



LRESULT CALLBACK WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_COMMAND:
			OnCommand(hw, LOWORD(wp), HIWORD(wp));
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hw, msg, wp, lp);
}

//////////////////////////////////////////////////////////////////////////////////////////
// basic functions                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

int RegisterMyClass(HINSTANCE hInstance, char* className)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof wc);

	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P3));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	return RegisterClass(&wc);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hp, LPSTR cmdLine, int nShow)
{
	char clsName[] = "NWPClass";

	if(!RegisterMyClass(hInstance, clsName))
		return 0;

	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_P3));
	HWND hwnd = CreateWindow(clsName, "NWP 3",  WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, hMenu, hInstance, NULL); 

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if (hDlgModeless == 0 || !IsDialogMessage (hDlgModeless, &msg))
		{
			TranslateMessage (&msg) ;
			DispatchMessage  (&msg) ;
		}
	}
	return msg.wParam;
}

// enable themes support ("visual styles")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
