// View.cpp : implementation of the View class
//

#include "stdafx.h"
#include "p8.h"
#include "View.h"
#include "Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// View

View::View() : perc(100)
{
}

View::~View()
{
}


BEGIN_MESSAGE_MAP(View,CWnd )
	//{{AFX_MSG_MAP(View)
	ON_WM_PAINT()
	ON_COMMAND(ID_VIEW_DIALOG, OnViewDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// View message handlers

BOOL View::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void View::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc; GetClientRect(&rc);
	rc.DeflateRect(rc.Width()*(100-perc)/200, rc.Height()*(100-perc)/200); 
	dc.Ellipse(rc);
	
}


void View::OnViewDialog() 
{
	Dlg d;
	d.p = perc;
	if(d.DoModal() == IDOK){
		perc = d.p;
		Invalidate();
	}
}
