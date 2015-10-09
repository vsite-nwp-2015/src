// p10.h : main header file for the P10 application
//

#if !defined(AFX_P10_H__B72F159D_E197_44D1_83E0_C92DF89A5060__INCLUDED_)
#define AFX_P10_H__B72F159D_E197_44D1_83E0_C92DF89A5060__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// App:
// See p10.cpp for the implementation of this class
//

class App : public CWinApp
{
public:
	App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(App)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P10_H__B72F159D_E197_44D1_83E0_C92DF89A5060__INCLUDED_)
