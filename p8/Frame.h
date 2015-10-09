// Frame.h : interface of the Frame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__59691A8D_9618_4D6D_9FE3_245998CB3A19__INCLUDED_)
#define AFX_FRAME_H__59691A8D_9618_4D6D_9FE3_245998CB3A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "View.h"

class Frame : public CFrameWnd
{
	
public:
	Frame();
protected: 
	DECLARE_DYNAMIC(Frame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Frame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Frame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	View    m_wndView;

// Generated message map functions
protected:
	//{{AFX_MSG(Frame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAME_H__59691A8D_9618_4D6D_9FE3_245998CB3A19__INCLUDED_)
