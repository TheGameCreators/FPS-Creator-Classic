// CartMsgDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CartMsgDialog.h"
#include "Editor.h"


// CartMsgDialog dialog

IMPLEMENT_DYNAMIC(CartMsgDialog, CDialog)

CartMsgDialog::CartMsgDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CartMsgDialog::IDD, pParent)
{
	m_sMessage = "";
	m_sTitle = "";
	m_sRegID = "";
}

CartMsgDialog::~CartMsgDialog()
{
}

void CartMsgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkNotShowAgain);
	DDX_Control(pDX, IDC_MSG, m_txtMessage);
}

void CartMsgDialog::SetValues( const char *szMsg, const char *szTitle, const char *szRegID )
{
	if ( !szMsg ) return;
	if ( !szRegID ) return;

	m_sMessage = szMsg;
	m_sRegID = szRegID;
	if ( szTitle ) m_sTitle = szTitle;
	else m_sTitle = "";
}

BOOL CartMsgDialog::OnInitDialog()
{
	if ( !CDialog::OnInitDialog() ) return FALSE;

	m_chkNotShowAgain.SetWindowText( GetLanguageData( _T("TGC Store"), _T("DontShow") ) );

	m_txtMessage.SetWindowText( m_sMessage );
	SetWindowText( m_sTitle );

	return TRUE;
}


BEGIN_MESSAGE_MAP(CartMsgDialog, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CartMsgDialog::OnDontShowAgainClicked)
	ON_BN_CLICKED(IDOK, &CartMsgDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CartMsgDialog message handlers

void CartMsgDialog::OnDontShowAgainClicked()
{
	
}

void CartMsgDialog::OnBnClickedOk()
{
	DWORD dwCheck = m_chkNotShowAgain.GetCheck();
	DWORD dwValue = dwCheck > 0 ? 0 : 1;

	CString sRegKey = "display_";
	sRegKey += m_sRegID;
	sRegKey += "_message";

	theApp.SetRegistryValue( "Software\\FPSCreator\\Editor\\Settings", sRegKey, dwValue );
	
	OnOK();
}
