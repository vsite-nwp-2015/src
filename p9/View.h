// View.h : interface of the View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_H__369DE263_04ED_4F03_9044_E3005FF8D21B__INCLUDED_)
#define AFX_VIEW_H__369DE263_04ED_4F03_9044_E3005FF8D21B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class View : public CScrollView
{
protected: // create from serialization only
	View();
	DECLARE_DYNCREATE(View)

// Attributes
public:
	Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(View)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in View.cpp
inline Doc* View::GetDocument()
   { return (Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW_H__369DE263_04ED_4F03_9044_E3005FF8D21B__INCLUDED_)
