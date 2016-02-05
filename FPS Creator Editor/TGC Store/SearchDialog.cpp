// SearchDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SearchDialog.h"
#include "..\Editor.h"


// CSearchDialog dialog

IMPLEMENT_DYNAMIC(CSearchDialog, CDialog)

CSearchDialog::CSearchDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDialog::IDD, pParent)
{
	m_bAdmin = false;
}

CSearchDialog::~CSearchDialog()
{
}

void CSearchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editKeyword);
	DDX_Control(pDX, IDC_SEARCH_NAME, m_txtKeyword);
	DDX_Control(pDX, IDOK, m_btnSearch);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CHECK1, m_chkExact);
	DDX_Control(pDX, IDC_APPROVAL_ITEMS, m_chkApprovalItems);
}

BOOL CSearchDialog::OnInitDialog( )
{
	if ( !CDialog::OnInitDialog( ) ) return FALSE;

	m_editKeyword.SetWindowText( m_sKeyword );
	m_txtKeyword.SetWindowText( GetLanguageData( _T("TGC Store"), _T("SearchLabel") ) );

	m_chkExact.SetCheck( m_bExactMatch ? 1 : 0 );
	m_chkApprovalItems.SetCheck( m_bApprovalItems ? 1 : 0 );
	if ( m_bAdmin ) m_chkApprovalItems.ShowWindow( SW_SHOW );
	else m_chkApprovalItems.ShowWindow( SW_HIDE );

	if ( m_bApprovalItems && m_bAdmin )
	{
		m_editKeyword.EnableWindow( FALSE );
		m_chkExact.EnableWindow( FALSE );
	}
	else
	{
		m_editKeyword.EnableWindow( TRUE );
		m_chkExact.EnableWindow( TRUE );
	}

	/*
	m_editCategory.SetWindowText( m_sCategory );
	m_editCostLow.SetWindowText( m_sCostLow );
	m_editCostHigh.SetWindowText( m_sCostHigh );
	m_editAuthor.SetWindowText( m_sAuthor );
	m_editRatingLow.SetWindowText( m_sRatingLow );
	m_editRatingHigh.SetWindowText( m_sRatingHigh );
	m_editKeyword.SetWindowText( m_sKeyword );

	CString sSearchStatement = GetLanguageData( _T("TGC Store"), _T("SearchBox1") );
	sSearchStatement += "\r\n\r\n";
	sSearchStatement += GetLanguageData( _T("TGC Store"), _T("SearchBox2") );

	m_txtStatement.SetWindowText( sSearchStatement );
	m_txtCategory.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Category") ) );
	m_txtCost1.SetWindowText( GetLanguageData( _T("TGC Store"), _T("CostBetween") ) );
	m_txtCost2.SetWindowText( GetLanguageData( _T("TGC Store"), _T("And") ) );
	m_txtAuthor.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Author") ) );
	m_txtRating1.SetWindowText( GetLanguageData( _T("TGC Store"), _T("RatingBetween") ) );
	m_txtRating2.SetWindowText( GetLanguageData( _T("TGC Store"), _T("And") ) );
	m_txtKeyword.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Keyword") ) );
	*/

	m_btnSearch.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Search") ) );
	m_btnCancel.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Cancel") ) );
	m_chkExact.SetWindowText( GetLanguageData( _T("TGC Store"), _T("Exact") ) );

	m_editKeyword.SetFocus( );

	return FALSE;
}


BEGIN_MESSAGE_MAP(CSearchDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSearchDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPROVAL_ITEMS, &CSearchDialog::OnBnClickedApprovalItems)
END_MESSAGE_MAP()


// CSearchDialog message handlers

bool CheckStringAlNum( CString sField, CString sString )
{
	for ( int i = 0; i < sString.GetLength(); i++ )
	{
		if ( !isalnum(sString.GetAt( i )) )
		{
			CString sMsg;
			
			sMsg.Format( "\"%s\" %s", sField.GetString( ), GetLanguageData( _T("TGC Store"), _T("InputError1") ) );
			MessageBox( NULL, sMsg, theApp.m_szErrorString, 0 );
			return false;
		}
	}

	return true;
}

bool CheckStringAlNum2( CString sField, CString sString )
{
	if ( sString.FindOneOf( "|\\/?\"*><" ) > -1 )
	{
		CString sMsg;
		sMsg.Format( "\"%s\" %s:  | \\ / ? \" * > <", sField.GetString( ), GetLanguageData( _T("TGC Store"), _T("InputError1") ) );
		MessageBox( NULL, sMsg, theApp.m_szErrorString, 0 );
		return false;
	}

	return true;
}

bool CheckStringDigit( CString sField, CString sString )
{
	for ( int i = 0; i < sString.GetLength(); i++ )
	{
		if ( !isdigit(sString.GetAt( i )) )
		{
			CString sMsg;
			sMsg.Format( "\"%s\" %s", sField.GetString( ), GetLanguageData( _T("TGC Store"), _T("InputError2") ) );
			MessageBox( NULL, sMsg, theApp.m_szErrorString, 0 );
			return false;
		}
	}

	return true;
}

void CSearchDialog::OnBnClickedOk()
{
	//m_bCompleteSearch = false;

	m_editKeyword.GetWindowText( m_sKeyword );
	m_bExactMatch = m_chkExact.GetCheck() > 0;
	m_bApprovalItems = m_chkApprovalItems.GetCheck() > 0;

	m_sKeyword.Trim();

	if ( m_sKeyword.GetLength() < 1 && !m_bApprovalItems )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("EmptySearch") ), theApp.m_szErrorString );
		return;
	}
	
	/*
	m_editCategory.GetWindowText( m_sCategory );
	m_editCostLow.GetWindowText( m_sCostLow );
	m_editCostHigh.GetWindowText( m_sCostHigh );
	m_editAuthor.GetWindowText( m_sAuthor );
	m_editRatingLow.GetWindowText( m_sRatingLow );
	m_editRatingHigh.GetWindowText( m_sRatingHigh );
	m_editKeyword.GetWindowText( m_sKeyword );

	if ( strlen(m_sName) < 1 && strlen(m_sCategory) < 1 && strlen(m_sCostLow) < 1 && strlen(m_sCostHigh) < 1 && 
		 strlen(m_sAuthor) < 1 && strlen(m_sRatingLow) < 1 && strlen(m_sRatingHigh) < 1 && strlen(m_sKeyword) < 1 )
	{
		m_bCompleteSearch = true;
		OnOK();
		return;
		//MessageBox( "At least one field must contain data to do a search", "Input Error" );
		//return;
	}
	*/

	//if ( !CheckStringAlNum2( GetLanguageData( _T("TGC Store"), _T("Name") ), m_sName ) ) return;
	//if ( !CheckStringAlNum2( GetLanguageData( _T("TGC Store"), _T("Category") ), m_sCategory ) ) return;
	//if ( !CheckStringDigit( GetLanguageData( _T("TGC Store"), _T("CostBetween") ), m_sCostLow ) ) return;
	//if ( !CheckStringDigit( GetLanguageData( _T("TGC Store"), _T("Cost") ), m_sCostHigh ) ) return;
	//if ( !CheckStringAlNum2( GetLanguageData( _T("TGC Store"), _T("Author") ), m_sAuthor ) ) return;
	//if ( !CheckStringDigit( GetLanguageData( _T("TGC Store"), _T("RatingBetween") ), m_sRatingLow ) ) return;
	//if ( !CheckStringDigit( GetLanguageData( _T("TGC Store"), _T("Rating") ), m_sRatingHigh ) ) return;
	//if ( !CheckStringAlNum( GetLanguageData( _T("TGC Store"), _T("Keyword") ), m_sKeyword ) ) return;

	/*
	if ( (strlen(m_sCostLow) > 0 && strlen(m_sCostHigh) < 1) || (strlen(m_sCostLow) < 1 && strlen(m_sCostHigh) > 0) )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("InputError3") ), theApp.m_szErrorString );
		return;
	}

	if ( (strlen(m_sRatingLow) > 0 && strlen(m_sRatingHigh) < 1) || (strlen(m_sRatingLow) < 1 && strlen(m_sRatingHigh) > 0) )
	{
		MessageBox( GetLanguageData( _T("TGC Store"), _T("InputError4") ), theApp.m_szErrorString );
		return;
	}
	*/

	OnOK();
}

void CSearchDialog::OnBnClickedApprovalItems()
{
	int check = m_chkApprovalItems.GetCheck();

	if ( check > 0 )
	{
		m_editKeyword.EnableWindow( FALSE );
		m_chkExact.EnableWindow( FALSE );
	}
	else
	{
		m_editKeyword.EnableWindow( TRUE );
		m_chkExact.EnableWindow( TRUE );
	}
}
