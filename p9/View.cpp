// View.cpp : implementation of the View class
//

#include "stdafx.h"
#include "p9.h"

#include "Doc.h"
#include "View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// View

IMPLEMENT_DYNCREATE(View, CScrollView)

BEGIN_MESSAGE_MAP(View, CScrollView)
	//{{AFX_MSG_MAP(View)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// View construction/destruction

View::View()
{
}

View::~View()
{
}

BOOL View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// View drawing

void View::OnDraw(CDC* pDC)
{
	Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	for(iter i = pDoc->c.begin(); i != pDoc->c.end(); ++i)
		pDC->Ellipse(i->x-10, i->y-10, i->x+10, i->y+10);
}

/////////////////////////////////////////////////////////////////////////////
// View diagnostics

#ifdef _DEBUG
void View::AssertValid() const
{
	CScrollView::AssertValid();
}

void View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

Doc* View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Doc)));
	return (Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// View message handlers

void View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	point += GetScrollPosition();
	GetDocument()->c.push_back(point);
	GetDocument()->SetModifiedFlag();
	Invalidate();
}

void View::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(1000, 800));
}	

