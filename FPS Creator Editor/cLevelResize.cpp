// cLevelResize.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cLevelResize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cLevelResize dialog


cLevelResize::cLevelResize(CWnd* pParent /*=NULL*/)
	: CDialog(cLevelResize::IDD, pParent)
{
	//{{AFX_DATA_INIT(cLevelResize)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



void cLevelResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cLevelResize)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cLevelResize, CDialog)
	//{{AFX_MSG_MAP(cLevelResize)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cLevelResize message handlers
