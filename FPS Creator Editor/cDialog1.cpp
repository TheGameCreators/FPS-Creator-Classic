// cDialog1.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cDialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cDialog1 dialog


cDialog1::cDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(cDialog1::IDD, pParent)
{
	//{{AFX_DATA_INIT(cDialog1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDialog1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cDialog1, CDialog)
	//{{AFX_MSG_MAP(cDialog1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDialog1 message handlers
