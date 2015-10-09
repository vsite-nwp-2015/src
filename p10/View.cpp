// View.cpp : implementation of the View class
//

#include "stdafx.h"
#include "p10.h"

#include "Doc.h"
#include "View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// View

IMPLEMENT_DYNCREATE(View, CView)

BEGIN_MESSAGE_MAP(View, CView)
	//{{AFX_MSG_MAP(View)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// View construction/destruction

View::View()
{
	CString section(_T("Print"));
	numPages = AfxGetApp()->GetProfileInt(section, _T("NumPages"), 4);
	fontSize = AfxGetApp()->GetProfileInt(section, _T("FontSize"), 36);
	fontName = AfxGetApp()->GetProfileString(section, _T("FontName"), _T("Times New Roman"));
}

View::~View()
{
}

BOOL View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// View drawing

void View::OnDraw(CDC* pDC)
{
	Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rc; GetClientRect(rc);
	pDC->Ellipse(rc);
}

/////////////////////////////////////////////////////////////////////////////
// View printing

BOOL View::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(numPages);
	return DoPreparePrinting(pInfo);
}

void View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// View diagnostics

#ifdef _DEBUG
void View::AssertValid() const
{
	CView::AssertValid();
}

void View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Doc* View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Doc)));
	return (Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// View message handlers

void View::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CFont f; f.CreateFont(-fontSize*pDC->GetDeviceCaps(LOGPIXELSY)/72,
		0,0,0,0,0,0,0,0,0,0,0,0,fontName);
	pDC->SelectObject(&f);
	CString s; s.Format(_T("Page %d of %d"), pInfo->m_nCurPage, numPages);
	pDC->DrawText(s, pInfo->m_rectDraw, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}
