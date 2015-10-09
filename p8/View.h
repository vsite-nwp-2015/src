// View.h : interface of the View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_H__7EBC3284_F277_4B52_9F17_45F9C36CBF40__INCLUDED_)
#define AFX_VIEW_H__7EBC3284_F277_4B52_9F17_45F9C36CBF40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// View window

class View : public CWnd
{
	int perc;
// Construction
public:
	View();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(View)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~View();

	// Generated message map functions
protected:
	//{{AFX_MSG(View)
	afx_msg void OnPaint();
	afx_msg void OnViewDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW_H__7EBC3284_F277_4B52_9F17_45F9C36CBF40__INCLUDED_)
