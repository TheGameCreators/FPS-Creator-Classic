
#include "stdafx.h"
#include "editor.h"
#include "Browser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBrowser::CBrowser ( CWnd* pParent ) : CTabPageSSL ( CBrowser::IDD, pParent )
{
	bGo				= true;
	m_pTest			= ( CBrowserNew* ) pParent;
	m_iParentWidth  = 0;
	m_iParentHeight = 0;
	m_iCount		= 0;
	m_iArrayCount	= 0;

	memset ( m_iArray, 0, sizeof ( m_iArray ) );
}

void CBrowser::DoDataExchange ( CDataExchange* pDX )
{
	CDialog::DoDataExchange ( pDX );
	//{{AFX_DATA_MAP(CBrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP ( CBrowser, CDialog )
	//{{AFX_MSG_MAP(CBrowser)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY( NM_CLICK, IDC_PREVIEW_LIST, OnClickList )
	ON_NOTIFY( NM_DBLCLK, IDC_PREVIEW_LIST, OnClickListDbl )
	ON_NOTIFY( NM_KEYDOWN, IDC_PREVIEW_LIST, OnKeyDown )
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

void CBrowser::OnKeyDown ( NMHDR* pNMHDR, LRESULT* pResult )
{
	theApp.SetFileMapData ( 528, 1 );
}

void CBrowser::OnClickListDbl ( NMHDR* pNMHDR, LRESULT* pResult )
{
	theApp.SetFileMapData ( 528, 1 );
}

void CBrowser::OnClickList ( NMHDR* pNMHDR, LRESULT* pResult )
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

	for ( int i = 0; i < m_iArrayCount; i++ )
	{
		if ( hitTest.iItem == m_iArray [ i ] )
		{
			theApp.SetFileMapData ( 524, i );
		}
	}

	for ( i = 0; i < m_iArrayCount; i++ )
	{
		ListView_SetItemState ( pList->GetSafeHwnd ( ), i, 1, LVIS_SELECTED );
	}

	theApp.SetFileMapData ( 516, 1 );
}

BOOL CBrowser::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


int CBrowser::OnCreate ( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CDialog::OnCreate ( lpCreateStruct ) == -1 )
		return -1;

	m_iCount = 0;

	// 270105
	UINT iTimer = SetTimer ( 1, 1000, NULL );

	return 0;
}

void CBrowser::SetParentSize ( int iWidth, int iHeight )
{

}

void CBrowser::OnLButtonUp ( UINT nFlags, CPoint point )
{
	CListCtrl*	pList = NULL;

	if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
			return;

	int iIndex = pList->GetHotItem ( );
	
	CDialog::OnLButtonUp ( nFlags, point );
}

void CBrowser::OnMouseMove ( UINT nFlags, CPoint point )
{
	
}

void CBrowser::AddBitmap ( LPTSTR szBitmap, LPTSTR szName )
{
	CListCtrl*	pList = NULL;

	if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
		return;

	/*
	// 170105
	// CONVERT WIDE TO ANSI
	{
		char szDestination [ MAX_PATH ];
		int  iLen     = wcslen ( szBitmap );
		int  nResult = WideCharToMultiByte ( CP_ACP, 0, szBitmap, -1, szDestination, iLen * sizeof ( CHAR ), NULL, NULL );
		szDestination [ iLen ] = 0;
		int c = 0;
	}
	*/

	EnterCriticalSection( &theApp.m_csDirectoryChanges );
	
	SetCurrentDirectory ( theApp.m_szDirectory );

	CBitmap	bitmap;
	bitmap.Attach ( ::LoadImage ( NULL, szBitmap, IMAGE_BITMAP, 64, 64, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION ) );
	
	LeaveCriticalSection( &theApp.m_csDirectoryChanges );

	m_imageList.Add ( &bitmap, RGB ( 0, 0, 0 ) );
	bitmap.DeleteObject();
	pList->InsertItem ( 0, szName, m_iCount++ );

	m_iArray [ m_iArrayCount++ ] = 3;

	for ( int i = 0; i < m_iArrayCount - 1; i++ )
	{
		m_iArray [ i ]++;
	}

	m_iArray [ m_iArrayCount - 1 ] = 0;

	CSize size;

	size.cy = 1000;

	//pList->Scroll ( size );
	pList->Scroll ( 0 );
}

void CBrowser::RemoveAllBitmaps ( void )
{
	CListCtrl*	pList = NULL;

	if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
		return;

	m_iArrayCount = 0;

	pList->DeleteAllItems ( );
}

void CBrowser::RemoveBitmap ( int iIndex )
{
	CListCtrl*	pList = NULL;

	if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
			return;

	pList->DeleteItem ( iIndex );
}

void CBrowser::OnShowWindow ( BOOL bShow, UINT nStatus )
{
	if ( bGo )
	{
		// create the image list using images at a size of 64 * 64,
		// we also specify that we use an independent colour depth
		m_imageList.Create     ( 64, 64, ILC_COLORDDB, 1, 16 );

		m_imageList.SetBkColor ( RGB ( 255, 255, 255 ) );

		CListCtrl*	pList = NULL;

		if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
			return;

		pList->DeleteAllItems ( );

		pList->SetImageList ( &m_imageList, LVSIL_NORMAL );
		pList->SetItemCount ( 1000 );

		bGo = false;
	}
}

void CBrowser::OnSize ( UINT nType, int cx, int cy )
{
	CListCtrl*	pList = NULL;

	if ( ( pList = ( CListCtrl* ) GetDlgItem ( IDC_PREVIEW_LIST ) ) == NULL )
		return;

	// 270105 - force redraw
	//theApp.m_bBuildGameDisplay = true;

	pList->SetWindowPos ( NULL, -1, -1, cx, cy, SWP_SHOWWINDOW );
}

void CBrowser::OnTimer ( UINT nIDEvent )
{
	// 270105 - force redraw
	//theApp.m_bBuildGameDisplay = false;

	// 310105
	theApp.m_bBrowserDisplay = false;
}