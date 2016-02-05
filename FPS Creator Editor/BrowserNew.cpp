// BrowserNew.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "BrowserNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CBrowserNew::CBrowserNew(CWnd* pParent ) : CDialog(CBrowserNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowserNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBrowserNew::DoDataExchange ( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowserNew)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON1, m_btnStore);
}


BEGIN_MESSAGE_MAP ( CBrowserNew, CDialog )
	//{{AFX_MSG_MAP(CBrowserNew)
	ON_WM_PAINT     ( )
	ON_WM_SIZE      ( )
	ON_WM_LBUTTONUP ( )
	ON_NOTIFY       ( NM_CLICK, IDC_PREVIEW_LIST, OnClickList )
	ON_WM_MOUSEMOVE ( )
	ON_WM_KEYDOWN   ( )
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBrowserNew::Create ( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
	return CDialog::Create ( IDD, pParentWnd );
}

void CBrowserNew::OnPaint ( )
{
	CPaintDC dc ( this );

	// 310105
//	theApp.m_bBuildGameDisplay = true;

	if ( !theApp.m_bBuildGameDisplay )
		theApp.m_bBrowserDisplay = true;
}

void CBrowserNew::OnClickList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	CListCtrl*	pList = NULL;

	if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
			return;

	// retrieve message info.
	LPNMITEMACTIVATE pItemAct = ( LPNMITEMACTIVATE ) pNMHDR;

	// determine which item receives the click
	LVHITTESTINFO  hitTest;
	
	memset ( &hitTest, '\0', sizeof ( LVHITTESTINFO ) );

	hitTest.pt = pItemAct->ptAction;
	
	pList->SendMessage ( LVM_SUBITEMHITTEST, 0, ( LPARAM ) &hitTest );

	theApp.SetFileMapData ( 516, 1 );
	theApp.SetFileMapData ( 524, hitTest.iItem );
}

void CBrowserNew::AddBitmap ( int iTab, LPTSTR szBitmap, LPTSTR szName )
{
	m_tabCtrl.ActivateSSLPage ( iTab );

	if ( iTab == 0 )
	{
		m_BrowserPrefabs.AddBitmap ( szBitmap, szName );
	}
	else if ( iTab == 1 )
	{
		m_BrowserSegments.AddBitmap ( szBitmap, szName );
	}
	else if ( iTab == 2 )
	{
		m_BrowserEntities.AddBitmap ( szBitmap, szName );
	}
	else if ( iTab == 3 )
	{
		m_BrowserMarkers.AddBitmap ( szBitmap, szName );
	}
}

void CBrowserNew::RemoveAllBitmaps ( int iTab )
{
	if ( iTab == 0 )
	{
		m_BrowserPrefabs.RemoveAllBitmaps ( );
	}
	else if ( iTab == 1 )
	{
		m_BrowserSegments.RemoveAllBitmaps ( );
	}
	else if ( iTab == 2 )
	{
		m_BrowserEntities.RemoveAllBitmaps ( );
	}
	else if ( iTab == 3 )
	{
		m_BrowserMarkers.RemoveAllBitmaps ( );
	}
}

void CBrowserNew::RemoveBitmap ( int iTab, int iIndex )
{
	m_tabCtrl.ActivateSSLPage ( iTab );

	if ( iTab == 0 )
	{
		m_BrowserPrefabs.RemoveBitmap ( iIndex );
	}
	else if ( iTab == 1 )
	{
		m_BrowserSegments.RemoveBitmap ( iIndex );
	}
	else if ( iTab == 2 )
	{
		m_BrowserEntities.RemoveBitmap ( iIndex );
	}

	else if ( iTab == 3 )
	{
		m_BrowserMarkers.RemoveBitmap ( iIndex );
	}
}

BOOL CBrowserNew::OnInitDialog ( ) 
{
	CDialog::OnInitDialog ( );

	m_BrowserSegments.Create ( IDD_BROWSER, this );
	m_BrowserPrefabs.Create  ( IDD_BROWSER, this );
	m_BrowserEntities.Create ( IDD_BROWSER, this );
	m_BrowserMarkers.Create  ( IDD_BROWSER, this );

	TCHAR szString [ 4 ] [ MAX_PATH ];

	GetPrivateProfileString ( _T ( "Library" ), _T ( "Tab1" ), _T ( "" ), szString [ 0 ], MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Library" ), _T ( "Tab2" ), _T ( "" ), szString [ 1 ], MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Library" ), _T ( "Tab3" ), _T ( "" ), szString [ 2 ], MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Library" ), _T ( "Tab4" ), _T ( "" ), szString [ 3 ], MAX_PATH, theApp.m_szLanguage );

	m_tabCtrl.AddSSLPage ( szString [ 0 ], 0, &m_BrowserPrefabs  );
	m_tabCtrl.AddSSLPage ( szString [ 1 ], 0, &m_BrowserSegments );
	m_tabCtrl.AddSSLPage ( szString [ 2 ], 0, &m_BrowserEntities );
	m_tabCtrl.AddSSLPage ( szString [ 3 ], 0, &m_BrowserMarkers  );

	CBitmap *bmpStoreButton = new CBitmap;
	bmpStoreButton->LoadBitmap( IDB_STORE_BUTTON );
	m_btnStore.SetBitmap( *bmpStoreButton );
	m_btnStore.Invalidate();
	
	return TRUE;
}

void CBrowserNew::OnSize ( UINT nType, int cx, int cy )
{
	CDialog::OnSize ( nType, cx, cy );
	
	if ( m_tabCtrl )
	{
		m_tabCtrl.SetWindowPos ( NULL, -1, -1, cx, cy-30, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );

		int iTab = m_tabCtrl.GetSSLActivePage ( );

		m_tabCtrl.ActivateSSLPage ( 0 );
		m_tabCtrl.ResizeDialog ( 0, cx, cy-30 );
		m_tabCtrl.ActivateSSLPage ( iTab );
	}

	if ( m_btnStore ) m_btnStore.SetWindowPos( NULL, 0,cy-30, cx,30, 0 );
}

void CBrowserNew::OnLButtonUp ( UINT nFlags, CPoint point )
{
	CDialog::OnLButtonUp ( nFlags, point );

	theApp.SetFileMapData ( 520, m_tabCtrl.GetSSLActivePage ( ) );
}

void CBrowserNew::OnMouseMove ( UINT nFlags, CPoint point )
{
	theApp.SetFileMapData ( 520, m_tabCtrl.GetSSLActivePage ( ) );
	
	CDialog::OnMouseMove ( nFlags, point );
}

void CBrowserNew::OnKeyDown ( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CDialog::OnKeyDown ( nChar, nRepCnt, nFlags );
}

BOOL CBrowserNew::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog ::OnMouseWheel(nFlags, zDelta, pt);
}

