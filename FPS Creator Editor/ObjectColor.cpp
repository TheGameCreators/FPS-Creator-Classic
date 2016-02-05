// ObjectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "Editor.h"
#include "ObjectColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty



CObjectColor::~CObjectColor()
{
	int c = 0;
}


/*
BEGIN_MESSAGE_MAP(CObjectColor, CObjectColor)
	//{{AFX_MSG_MAP(CBCGColorProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

void CObjectColor::OnDrawValue (CDC* pDC, CRect rect)
{
	MessageBox ( NULL, _T("go"), _T("here"), MB_OK );
}

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty message handlers
