// cLibraryWindowNew.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cLibraryWindowNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cLibraryWindowNew dialog



cLibraryWindowNew::cLibraryWindowNew() : CPropertyPage(cLibraryWindowNew::IDD)
{
	//{{AFX_DATA_INIT(cLibraryWindowNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cLibraryWindowNew::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cLibraryWindowNew)
		// NOTE: the ClassWizard will add DDX and DDV calls here
  DDX_Control(pDX, IDC_SPLIT_MID, m_spliter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cLibraryWindowNew, CPropertyPage)
	//{{AFX_MSG_MAP(cLibraryWindowNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg3 message handlers

BOOL cLibraryWindowNew::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
  //m_spliter.addWindow(IDC_LIST1,sp_StyleRight|sp_StyleBottom);
  //m_spliter.addWindow(IDC_LIST9,sp_StyleRight|sp_StyleTop);
  //m_spliter.addWindow(IDC_LIST6,sp_StyleLeft|sp_StyleTop);
  //m_spliter.addWindow(IDC_LIST5,sp_StyleLeft|sp_StyleBottom);

	m_spliter.addWindow(IDC_TREE1,sp_StyleRight|sp_StyleBottom);
	m_spliter.addWindow(IDC_LIST1,sp_StyleLeft|sp_StyleBottom);
	m_spliter.setStyle(sp_StyleRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// cLibraryWindowNew message handlers
