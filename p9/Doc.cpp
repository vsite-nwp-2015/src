// Doc.cpp : implementation of the Doc class
//

#include "stdafx.h"
#include "p9.h"

#include "Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Doc

IMPLEMENT_DYNCREATE(Doc, CDocument)

BEGIN_MESSAGE_MAP(Doc, CDocument)
	//{{AFX_MSG_MAP(Doc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Doc construction/destruction

Doc::Doc()
{
}

Doc::~Doc()
{
}

BOOL Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// Doc serialization

void Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << c.size();
		for(iter i = c.begin(); i != c.end(); ++i)
			ar << i->x << i->y;
	}
	else
	{
		int n; ar >> n;
		while(n--){
			POINT p; ar >> p.x >> p.y;
			c.push_back(p);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Doc diagnostics

#ifdef _DEBUG
void Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Doc commands

void Doc::DeleteContents() 
{
	c.clear();
	CDocument::DeleteContents();
}
