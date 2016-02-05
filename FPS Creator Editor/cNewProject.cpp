// cNewProject.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cNewProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cNewProject dialog


cNewProject::cNewProject(CWnd* pParent /*=NULL*/)
	: CDialog(cNewProject::IDD, pParent)
{
	//{{AFX_DATA_INIT(cNewProject)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cNewProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cNewProject)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	
		DDX_Control(pDX, IDC_STATIC_NEW_PROJECT_1, m_Line1);
		DDX_Control(pDX, IDOK, m_OK);
		DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP

	DDX_Control ( pDX, IDC_EDIT_NEW, m_Edit );

	SetWindowText ( GetLanguageData ( "New Project", "Title" ) );
	m_Line1.SetWindowText ( GetLanguageData ( "New Project", "Text1" ) );
	m_OK.SetWindowText ( GetLanguageData ( "New Project", "Button1" ) );
	m_Cancel.SetWindowText ( GetLanguageData ( "New Project", "Button2" ) );
}


BEGIN_MESSAGE_MAP(cNewProject, CDialog)
	//{{AFX_MSG_MAP(cNewProject)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cNewProject message handlers

int cNewProject::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;


	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void cNewProject::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
		CEdit*	pEdit = NULL;

	if ( ( pEdit = ( CEdit* ) GetDlgItem ( IDC_EDIT_NEW ) ) == NULL )
		return;

	pEdit->SetFocus();
}

void cNewProject::OnOK() 
{
	m_Edit.GetWindowText ( m_UNI );
	
	CDialog::OnOK();
}
