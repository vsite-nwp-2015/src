// Frame.h : interface of the Frame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__32637523_DBFA_424E_A404_7AC64913A202__INCLUDED_)
#define AFX_FRAME_H__32637523_DBFA_424E_A404_7AC64913A202__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Frame : public CFrameWnd
{
	
protected: // create from serialization only
	Frame();
	DECLARE_DYNCREATE(Frame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Frame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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

// Generated message map functions
protected:
	//{{AFX_MSG(Frame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAME_H__32637523_DBFA_424E_A404_7AC64913A202__INCLUDED_)
