// EditorView.cpp : implementation of the CEditorView class
//

#include "stdafx.h"
#include "Editor.h"

#include "EditorDoc.h"
#include "CntrItem.h"
#include "EditorView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	//{{AFX_MSG_MAP(CEditorView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeaveSystem)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHoverSystem)

	ON_MESSAGE(WM_NCACTIVATE, OnNcActivate)
	ON_WM_MDIACTIVATE()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorView construction/destruction

CEditorView::CEditorView ( )
{
	m_pSelection = NULL;

	m_bMouseInside = FALSE;
    m_bMouseHover  = FALSE;

	m_lastPoint.x = 0;
	m_lastPoint.y = 0;

	
}

CEditorView::~CEditorView ( )
{
}

BOOL CEditorView::PreCreateWindow ( CREATESTRUCT& cs )
{
	theApp.m_pEditorView = this;

	return CView::PreCreateWindow ( cs );
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView drawing

void CEditorView::OnDraw(CDC* pDC)
{
	//CEditorDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	// TODO: also draw all OLE items in the document

	
}

void CEditorView::OnInitialUpdate ( )
{
	CView::OnInitialUpdate ( );

	SetTimer( 0, 1, NULL );

	m_pSelection = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView printing

void CEditorView::OnFilePrintPreview() 
{
	BCGPrintPreview (this);
}

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEditorView::OnDestroy()
{

	

	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CEditorView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CEditorCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

void CEditorView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CEditorCntrItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CEditorCntrItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document.
		CEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CEditorCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data.
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);

		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted.

		// TODO: reimplement selection as appropriate for your application

		m_pSelection = pItem;   // set selection to last inserted item
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CEditorView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}



void CEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	theApp.SetFileMapData ( 700, 1 );
	theApp.SetFileMapData ( 704, cx );
	theApp.SetFileMapData ( 708, cy );

	//SetCursorPos ( 0, 0 );

	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView diagnostics

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorView message handlers

void CEditorView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	theApp.SetFileMapData ( 36, 1 );

	theApp.OnViewDoubleClick (this, IDR_EDITORTYPE);
}

void CEditorView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CEditorView::OnMouseEnter ( UINT nFlags, CPoint point ) 
{
	//theApp.m_bEntityFocus = false;
}

void CEditorView::OnMouseHover ( UINT nFlags, CPoint point ) 
{

}

void CEditorView::OnMouseLeave ( ) 
{
	//theApp.m_bEntityFocus = true;

    //theApp.SetFileMapData ( 0, -1 );
	//theApp.SetFileMapData ( 4, -1 );


	// 100105
	//theApp.SetFileMapData ( 20, 0 );
	//theApp.SetFileMapData ( 24, 0 );
	//theApp.SetFileMapData ( 28, 0 );
	//theApp.SetFileMapData ( 32, 0 );
}

void CEditorView::MouseMoveProcessEHL ( UINT nFlags, CPoint point ) 
{
	m_bMouseHover = false;

    if ( ! m_bMouseInside )
	{
		// use _TrackMouseEvent ( ) to watch for a WM_MOUSELEAVE event
		TRACKMOUSEEVENT tme;

        tme.cbSize    = sizeof ( TRACKMOUSEEVENT );
        tme.dwFlags   = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
		
        _TrackMouseEvent ( &tme );

        // use _TrackMouseEvent ( ) to watch for a WM_MOUSEHOVER event
        tme.cbSize      = sizeof ( TRACKMOUSEEVENT );
        tme.dwFlags     = TME_HOVER;
        tme.hwndTrack   = m_hWnd;
        tme.dwHoverTime = HOVER_DEFAULT;
		
        _TrackMouseEvent ( &tme );

        // generate a mouse enter event
        OnMouseEnter ( nFlags, point );

        m_bMouseInside = true;
    }
}

LRESULT CEditorView::OnMouseHoverSystem ( WPARAM wParam, LPARAM lParam )
{
    // schedule another tracking for a WM_MOUSEHOVER event

    TRACKMOUSEEVENT tme;

    tme.cbSize      = sizeof ( TRACKMOUSEEVENT );
    tme.dwFlags     = TME_HOVER;
    tme.hwndTrack   = m_hWnd;
    tme.dwHoverTime = HOVER_DEFAULT;
		
    _TrackMouseEvent ( &tme );

    // set up values for nFlags and point before calling OnMouseHover ( )

    UINT nFlags = wParam;
    CPoint point ( ( DWORD ) lParam );

    // call on mouse hover
	if ( ! m_bMouseHover )
	{
        OnMouseHover ( nFlags, point );
        m_bMouseHover = true;
    }

    return TRUE;
}

LRESULT CEditorView::OnMouseLeaveSystem ( WPARAM wParam, LPARAM lParam )
{
	OnMouseLeave ( );

    m_bMouseInside = false;
    m_bMouseHover  = false;

    return TRUE;
}

BOOL CALLBACK EnumWindowsProc1(HWND hwnd,LPARAM lParam );

void CEditorView::OnMouseMove ( UINT nFlags, CPoint point )
{
	MouseMoveProcessEHL ( nFlags, point );

	if ( theApp.GetFileMapData ( 48 ) != 1 )
	{
		RECT rc;
		GetClientRect ( &rc );

		float xRatio = ( float ) 800 / ( float ) rc.right;
		float yRatio = ( float ) 600 / ( float ) rc.bottom;
		
		point.x = ( int ) ( point.x * xRatio);
		point.y = ( int ) ( point.y * yRatio);

		if ( point.x != m_lastPoint.x || point.y != m_lastPoint.y )
		{
			POINT newPoint;

			newPoint.x = point.x - m_lastPoint.x;
			newPoint.y = point.y - m_lastPoint.y;

			m_lastPoint = point;

			// V110 BETA3 - 060608 - make mousemove cumilative (to be wiped by DBP program)
			LONG lCurrentX = (LONG)theApp.GetFileMapData ( 8 );
			LONG lCurrentY = (LONG)theApp.GetFileMapData ( 12 );

			// mouse x and y delta positions
			theApp.SetFileMapData (  8, lCurrentX + newPoint.x );
			theApp.SetFileMapData ( 12, lCurrentY + newPoint.y );
		}
	}

	// mouse X and mouse Y positions
	theApp.SetFileMapData ( 0, point.x );
	theApp.SetFileMapData ( 4, point.y );

	theApp.pMainFrame->OnMouseMoveXX ( nFlags, point );

	/* 220508 - no apparent functionality here
	// 100105
	if ( theApp.GetFileMapData ( 908 ) == 0 );
		EnumWindows ( EnumWindowsProc1, 0 );
	// 210405 - mike - removed end semi colon for some reason
	//if ( theApp.GetFileMapData ( 908 ) == 0 )
	//	EnumWindows ( EnumWindowsProc1, 0 );
	*/

	CView::OnMouseMove ( nFlags, point );
}

void CEditorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	theApp.SetFileMapData ( 20, 1 );

	CView::OnLButtonDown(nFlags, point);
}

void CEditorView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	theApp.SetFileMapData ( 20, 0 );
	theApp.SetFileMapData ( 24, 1 );

	CView::OnLButtonUp(nFlags, point);
	
}

void CEditorView::OnPaint() 
{
	if ( theApp.m_bSplashScreen )
	{
		//SetRedraw ( FALSE );
		//CPaintDC dc(this); // device context for painting
		//SetRedraw ( TRUE );

		CPaintDC dc(this); // device context for painting

		CRect rcClient;
		GetClientRect ( &rcClient );

		int cx = rcClient.Width  ( ) / 2;
		int cy = rcClient.Height ( ) / 2;

		//CRect rcShape ( cx - 45, cy - 45, cx + 45, cy + 45 );
		CRect rcShape ( 0, 0, rcClient.Width ( ), rcClient.Height ( ) );

		CBrush brush ( RGB (   0,   0, 0) );

		CBrush* pOldBrush = dc.SelectObject ( &brush );

		dc.Rectangle ( rcClient );

		dc.SelectObject ( pOldBrush );

	}
	else
	{
		if ( theApp.m_bAppRunning )
			SetRedraw ( FALSE );

		CPaintDC dc(this); // device context for painting


		// 221204
		theApp.SetFileMapData ( 60, 1 );

		if ( !theApp.m_bAppRunning )
		{
			CRect rcClient;
			GetClientRect ( &rcClient );

			int cx = rcClient.Width  ( ) / 2;
			int cy = rcClient.Height ( ) / 2;

			//CRect rcShape ( cx - 45, cy - 45, cx + 45, cy + 45 );
			CRect rcShape ( 0, 0, rcClient.Width ( ), rcClient.Height ( ) );

			CBrush brush ( RGB (   0,   0, 0) );

			CBrush* pOldBrush = dc.SelectObject ( &brush );

			dc.Rectangle ( rcClient );

			dc.SelectObject ( pOldBrush );
		}

		SetRedraw ( TRUE );

		if ( theApp.m_bInvalidate )
		{
			Invalidate ( );
			theApp.m_bInvalidate = false;
		}
	}
}

void CEditorView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnRButtonUp(nFlags, point);

	theApp.SetFileMapData ( 28, 0 );
	theApp.SetFileMapData ( 32, 1 );
	
}

void CEditorView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnRButtonDown(nFlags, point);

	theApp.SetFileMapData ( 28, 1 );
}

void CEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

	if ( KEY_UP(VK_RETURN) )
		theApp.SetFileMapData ( 108, 0 );

	if ( KEY_UP (VK_SHIFT) )
		theApp.SetFileMapData ( 112, 0 );

	if ( KEY_UP (VK_CONTROL) )
		theApp.SetFileMapData ( 116, 0 );

	if ( KEY_UP (VK_UP) )
		theApp.SetFileMapData ( 120, 0 );

	if ( KEY_UP (VK_DOWN) )
		theApp.SetFileMapData ( 124, 0 );

	if ( KEY_UP (VK_LEFT) )
		theApp.SetFileMapData ( 128, 0 );

	if ( KEY_UP (VK_RIGHT) )
		theApp.SetFileMapData ( 132, 0 );

	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 100, 0 );
	theApp.SetFileMapData ( 104, nChar );

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//theApp.m_Debug.Write ( "OnKeyDown" );

	theApp.SetFileMapData ( 100, nChar );
	
	if ( ::GetAsyncKeyState (VK_RETURN) & 0x8000 )
		theApp.SetFileMapData ( 108, 1 );

	if ( ::GetAsyncKeyState (VK_SHIFT) & 0x8000 )
		theApp.SetFileMapData ( 112, 1 );

	if ( ::GetAsyncKeyState (VK_CONTROL) & 0x8000 )
		theApp.SetFileMapData ( 116, 1 );

	if ( ::GetAsyncKeyState (VK_UP) & 0x8000 )
		theApp.SetFileMapData ( 120, 1 );

	if ( ::GetAsyncKeyState (VK_DOWN) & 0x8000 )
		theApp.SetFileMapData ( 124, 1 );

	if ( ::GetAsyncKeyState (VK_LEFT) & 0x8000 )
		theApp.SetFileMapData ( 128, 1 );

	if ( ::GetAsyncKeyState (VK_RIGHT) & 0x8000 )
		theApp.SetFileMapData ( 132, 1 );

	theApp.SetFileMapData ( 60, 1 );

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	return;
}

void CEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	theApp.SetFileMapData ( 60, 1 );
	
	CView::OnChar(nChar, nRepCnt, nFlags);

}

void CEditorView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CString string = "FPSEXCHANGE";

	DWORD dwOpen = 0;
	DWORD dwSave = 0;
	
	{
		{
			//HANDLE m_hFileMap;
			//LPVOID m_lpVoid;
			//BYTE*  m_pData;

			//m_hFileMap = OpenFileMapping ( FILE_MAP_ALL_ACCESS, TRUE, string );
			//m_lpVoid   = MapViewOfFile ( m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0 );

			
			//m_pData    = ( BYTE* ) m_lpVoid;

			//if ( m_lpVoid )
			{
				//if ( m_pData )
				{
					cIPC* pIPC	  = theApp.m_Message.GetIPC ( );

					pIPC->ReceiveBuffer ( &dwOpen, 424, sizeof ( dwOpen ) );
					pIPC->ReceiveBuffer ( &dwSave, 428, sizeof ( dwSave ) );

					//memcpy ( &dwOpen, &m_pData [ 424 ], sizeof ( DWORD ) );
					//memcpy ( &dwSave, &m_pData [ 428 ], sizeof ( DWORD ) );
				}
			}

			//UnmapViewOfFile ( m_lpVoid );
			//CloseHandle     ( m_hFileMap );
		}
	}

	if ( dwOpen == 0 && dwSave == 0 && theApp.m_bFocus == false )
	{
		if ( theApp.m_bEntityFocus == false )
			SetFocus ( );
	}

	CView::OnTimer(nIDEvent);
}

BOOL CEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	// mouse wheel
	theApp.SetFileMapData ( 16, zDelta );
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEditorView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	theApp.SetFileMapData ( 40, 1 );

	CView::OnRButtonDblClk(nFlags, point);
}

void CEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	if ( bActivate == FALSE )
	{
		//theApp.m_Debug.Write ( "OnActivateView" );
	}
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

WPARAM  wParam2 = 0;

BOOL CALLBACK EnumWindowsProc2(HWND hwnd,LPARAM lParam )
{
	TCHAR szBuffer  [ 256 ];
	TCHAR szDestroy [ 256 ];

	SetCurrentDirectory ( theApp.m_szDirectory );

	//GetPrivateProfileString ( _T ( "General" ), _T ( "Destroy Window" ), _T ( "" ), szDestroy, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );
	GetPrivateProfileString ( _T ( "General" ), _T ( "Destroy Window" ), _T ( "" ), szDestroy, MAX_PATH, theApp.m_szLanguage );

	GetWindowText ( hwnd, szBuffer, 256 );

	// 170105
	//if ( wcscmp ( szBuffer, szDestroy ) == 0 )
	if ( strcmp ( szBuffer, szDestroy ) == 0 )
	{
		LPARAM  lParam = 0;
		lParam = ( LPARAM ) hwnd;
		//SendMessage ( hwnd, WM_ACTIVATE, wParam2, lParam );
	}

	return TRUE;
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CEditorView::OnSetFocus(CWnd* pOldWnd)
{
	/*
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	//theApp.m_bEntityFocus = false;
		*/
	//wParam2 = WA_ACTIVE;
	//EnumWindows ( EnumWindowsProc2, 0 );
	

	CView::OnSetFocus(pOldWnd);
}

void CEditorView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

		//wParam2 = WA_INACTIVE;
		//EnumWindows ( EnumWindowsProc2, 0 );
}

LRESULT CEditorView::OnNcActivate( WPARAM w, LPARAM l )
{
	return (LRESULT)CWnd::OnNcActivate ( (BOOL)w );
}

void CEditorView::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	int c = 0;
}

void CEditorView::OnEnterIdle( UINT, CWnd* )
{
	int lee=42;
}