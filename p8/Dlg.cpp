// Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "p8.h"
#include "Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dlg dialog


Dlg::Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dlg)
	p = 0;
	//}}AFX_DATA_INIT
}


void Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dlg)
	DDX_Text(pDX, IDC_EDIT1, p);
	DDV_MinMaxInt(pDX, p, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dlg, CDialog)
	//{{AFX_MSG_MAP(Dlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dlg message handlers
