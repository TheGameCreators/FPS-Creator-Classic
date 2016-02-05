// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Editor.h"
#include "MainFrm.h"
#include "Splash.h"
#include "cTestGame.h"
#include "cBuildGame.h"
#include "cPreferences.h"
#include "cFPIWizard.h"
#include "cCredits.h"
#include "HelpWizard.h"
#include "AutoUpdate.h"	//AutoUpdate
#include "UpdateCheckThread.h"
#include "TGC Store\MarketWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Externals
extern bool g_bFreeVersion;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_TIMER()	//AutoUpdate
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_INCREASE_SHROUD, OnViewIncreaseShroud)
	ON_COMMAND(ID_VIEW_DECREASE_SHROUD, OnViewDecreaseShroud)
	ON_COMMAND(ID_VIEW_LAYERS, OnViewLayers)
	ON_COMMAND(ID_VIEW_MOVEUP_LAYER, OnViewMoveUpLayer)
	ON_COMMAND(ID_VIEW_MOVEDOWN_LAYER, OnViewMoveDownLayer)
	ON_COMMAND(ID_VIEW_OVERVIEW, OnViewOverview)
	ON_COMMAND(ID_VIEW_CLOSEUP, OnViewCloseup)
	ON_COMMAND(ID_MODE_PAINT, OnModePaint)
	ON_COMMAND(ID_MODE_SELECT, OnModeSelect)
	ON_COMMAND(ID_MODE_ART, OnModeArt)
	ON_COMMAND(ID_MODE_ENTITY, OnModeEntity)
	ON_COMMAND(ID_MODE_WAYPOINT, OnModeWaypoint)
	ON_COMMAND(ID_PROPERTY_ROTATE, OnPropertyRotate)
	ON_COMMAND(ID_PROPERTY_MIRROR, OnPropertyMirror)
	ON_COMMAND(ID_PROPERTY_FLIP, OnPropertyFlip)
	ON_COMMAND(ID_DRAW_A, OnDrawA)
	ON_COMMAND(ID_DRAW_B, OnDrawB)
	ON_COMMAND(ID_DRAW_C, OnDrawC)
	ON_COMMAND(ID_DRAW_D, OnDrawD)
	ON_COMMAND(ID_DRAW_STATE_NONE, OnDrawStateNone)
	ON_COMMAND(ID_DRAW_PICK, OnDrawPick)
	ON_COMMAND(ID_SELECTION_CUT, OnSelectionCut)
	ON_COMMAND(ID_SELECTION_COPY, OnSelectionCopy)
	ON_COMMAND(ID_SELECTION_PASTE, OnSelectionPaste)
	ON_COMMAND(ID_SELECTION_CLEAR, OnSelectionClear)
	ON_COMMAND(ID_SELECTION_CLEAR_NOPASTE, OnSelectionClearNopaste)
	ON_COMMAND(ID_SELECTION_REPLACE, OnSelectionReplace)
	ON_COMMAND(ID_SEGMENT_LINE, OnSegmentLine)
	ON_COMMAND(ID_SEGMENT_BOX, OnSegmentBox)
	ON_COMMAND(ID_SEGMENT_CIRCLE, OnSegmentCircle)
	ON_COMMAND(ID_SEGMENT_DECREASE, OnSegmentDecrease)
	ON_COMMAND(ID_SEGMENT_INCREASE, OnSegmentIncrease)
	ON_COMMAND(ID_SEGMENT_SPRAY, OnSegmentSpray)
	ON_COMMAND(ID_ENTITY_DELETE, OnEntityDelete)
	ON_COMMAND(ID_ENTITY_ROTATE_X, OnEntityRotateX)
	ON_COMMAND(ID_ENTITY_ROTATE_Y, OnEntityRotateY)
	ON_COMMAND(ID_ENTITY_ROTATE_Z, OnEntityRotateZ)
	ON_COMMAND(ID_WAYPOINT_CREATE, OnWaypointCreate)
	ON_COMMAND(ID_VIEW_ROTATE, OnViewRotate)
	ON_COMMAND(ID_VIEW_PREFAB, OnViewPrefab)
	ON_COMMAND(ID_VIEW_SELECTION, OnViewSelection)
	ON_COMMAND(ID_VIEW_DRAW, OnViewDraw)
	ON_COMMAND(ID_VIEW_ROTATE_ENTITY, OnViewRotateEntity)
	ON_COMMAND(ID_VIEW_WAYPOINT, OnViewWaypoint)
	ON_COMMAND(ID_VIEW_TEST_GAME, OnViewTestGame)
	ON_COMMAND(ID_TEST_GAME, OnTestGame)
	ON_UPDATE_COMMAND_UI(ID_TEST_MAP, OnUpdateTestMap)
	ON_UPDATE_COMMAND_UI(ID_TEST_GAME, OnUpdateTestGame)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INCREASE_SHROUD, OnUpdateViewIncreaseShroud)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DECREASE_SHROUD, OnUpdateViewDecreaseShroud)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LAYERS, OnUpdateViewLayers)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVEUP_LAYER, OnUpdateViewMoveupLayer)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVEDOWN_LAYER, OnUpdateViewMovedownLayer)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OVERVIEW, OnUpdateViewOverview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLOSEUP, OnUpdateViewCloseUp)
	ON_UPDATE_COMMAND_UI(ID_MODE_PAINT, OnUpdateModePaint)
	ON_UPDATE_COMMAND_UI(ID_MODE_SELECT, OnUpdateModeSelect)
	ON_UPDATE_COMMAND_UI(ID_MODE_ART, OnUpdateModeArt)
	ON_UPDATE_COMMAND_UI(ID_MODE_ENTITY, OnUpdateModeEntity)
	ON_UPDATE_COMMAND_UI(ID_MODE_WAYPOINT, OnUpdateModeWaypoint)
	ON_UPDATE_COMMAND_UI(ID_WAYPOINT_SELECT, OnUpdateModeWaypointSelect)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_ROTATE, OnUpdatePropertyRotate)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_MIRROR, OnUpdatePropertyMirror)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_FLIP, OnUpdatePropertyFlip)
	ON_UPDATE_COMMAND_UI(ID_DRAW_A, OnUpdateDrawA)
	ON_UPDATE_COMMAND_UI(ID_DRAW_B, OnUpdateDrawB)
	ON_UPDATE_COMMAND_UI(ID_DRAW_C, OnUpdateDrawC)
	ON_UPDATE_COMMAND_UI(ID_DRAW_D, OnUpdateDrawD)
	ON_UPDATE_COMMAND_UI(ID_DRAW_PICK, OnUpdateDrawPick)
	ON_UPDATE_COMMAND_UI(ID_DRAW_STATE_NONE, OnUpdateDrawStateNone)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_CUT, OnUpdateSelectionCut)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_COPY, OnUpdateSelectionCopy)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_PASTE, OnUpdateSelectionPaste)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_CLEAR, OnUpdateSelectionClear)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_CLEAR_NOPASTE, OnUpdateSelectionClearNoPaste)
	ON_UPDATE_COMMAND_UI(ID_SELECTION_REPLACE, OnUpdateSelectionReplace)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_LINE, OnUpdateSegmentLine)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BOX, OnUpdateSegmentBox)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_CIRCLE, OnUpdateSegmentCircle)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_DECREASE, OnUpdateSegmentDecrease)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_INCREASE, OnUpdateSegmentIncrease)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_SPRAY, OnUpdateSegmentSpray)
	ON_UPDATE_COMMAND_UI(ID_ENTITY_DELETE, OnUpdateEntityDelete)
	ON_UPDATE_COMMAND_UI(ID_ENTITY_ROTATE_X, OnUpdateEntityRotateX)
	ON_UPDATE_COMMAND_UI(ID_ENTITY_ROTATE_Y, OnUpdateEntityRotateY)
	ON_UPDATE_COMMAND_UI(ID_ENTITY_ROTATE_Z, OnUpdateEntityRotateZ)
	ON_UPDATE_COMMAND_UI(ID_WAYPOINT_CREATE, OnUpdateWaypointCreate)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_RESIZELEVEL, OnUpdateLevelResizelevel)
	ON_UPDATE_COMMAND_UI(ID_FILE_PREFERENCES, OnUpdateFilePreferences)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE_ENTITY, OnUpdateViewRotateEntity)
	ON_COMMAND(ID_FILE_SAVE_EX, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS_EX, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_COPY_1, OnEditCopy1)
	ON_COMMAND(ID_EDIT_CUT_1, OnEditCut1)
	ON_COMMAND(ID_EDIT_PASTE_1, OnEditPaste1)
	ON_COMMAND(ID_EDIT_REDO_1, OnEditRedo1)
	ON_COMMAND(ID_EDIT_UNDO_1, OnEditUndo1)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_WAYPOINT_SELECT, OnWaypointSelect)
	ON_COMMAND(ID_TEST_MAP, OnTestMap)
	ON_COMMAND(ID_FILE_BUILDGAME, OnFileBuildgame)
	ON_COMMAND(ID_FILE_PREFERENCES, OnFilePreferences)
	ON_COMMAND(ID_LEVEL_RESIZELEVEL, OnLevelResizelevel)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SEGMENTS_EXTERNALDRAWMODE, OnSegmentsExternaldrawmode)
	ON_COMMAND(ID_SEGMENTS_INTERNALDRAWMODE, OnSegmentsInternaldrawmode)
	ON_COMMAND(ID_SEGMENTS_PAINTALLOFSEGMENTS, OnSegmentsPaintallofsegments)
	ON_COMMAND(ID_SEGMENTS_PAINTONLYFLOORS, OnSegmentsPaintonlyfloors)
	ON_COMMAND(ID_SEGMENTS_PAINTONLYWALLS, OnSegmentsPaintonlywalls)
	ON_COMMAND(ID_SEGMENTS_PICKSEGMENTFROMGRID, OnSegmentsPicksegmentfromgrid)
	ON_COMMAND(ID_EDITING_CLIPBOARDEDIT, OnEditingClipboardedit)
	ON_COMMAND(ID_EDITING_ENTITYMODE, OnEditingEntitymode)
	ON_COMMAND(ID_EDITING_MOVEDOWNALAYER, OnEditingMovedownalayer)
	ON_COMMAND(ID_EDITING_MOVEUPALAYER, OnEditingMoveupalayer)
	ON_COMMAND(ID_EDITING_SEGMENTMODE, OnEditingSegmentmode)
	ON_COMMAND(ID_EDITING_VIEWENTIRELEVEL, OnEditingViewentirelevel)
	ON_COMMAND(ID_EDITING_ZOOMIN, OnEditingZoomin)
	ON_COMMAND(ID_EDITING_ZOOMOUT, OnEditingZoomout)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_ENTITY_VIEW, OnEntityView)
	ON_COMMAND(ID_SEGMENT_VIEW, OnSegmentView)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_VIEW, OnUpdateSegmentView)
	ON_UPDATE_COMMAND_UI(ID_ENTITY_VIEW, OnUpdateEntityView)
	ON_COMMAND(ID_APP_CREDITS, OnAppCredits)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATEAPP()
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, OnUpdateViewRotate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREFAB, OnUpdateViewPrefab)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTION, OnUpdateViewSelection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW, OnUpdateViewDraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WAYPOINT, OnUpdateViewWaypoint)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_COMMAND(ID_VIEW_VIEW, OnViewView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEW, OnUpdateViewView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, OnUpdateViewRotate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREFAB, OnUpdateViewPrefab)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTION, OnUpdateViewSelection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW, OnUpdateViewDraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE_ENTITY, OnUpdateViewEntity)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WAYPOINT, OnUpdateViewWaypoint)

	ON_UPDATE_COMMAND_UI(ID_VIEW_TEST_GAME, OnUpdateViewTestGame)

	ON_MESSAGE( WM_MYMESSAGE, OnThreadFinishedTest )
	ON_MESSAGE( WM_ENTITY_WINDOW, OnUpdateEntityWindow )

	ON_MESSAGE( WM_CLOSE_LEVEL_WINDOW, OnCloseLevelWindow )
	ON_COMMAND(ID_HELP_HELPWIZARD, &CMainFrame::OnHelpwizard)
	ON_COMMAND(ID_HELP_CHECKFORUPDATES, &CMainFrame::OnHelpCheckForUpdates)
	ON_COMMAND(ID_HELP_READMANUAL, &CMainFrame::OnHelpReadManual)
	ON_COMMAND(ID_HELP_TGCSTORE, &CMainFrame::OnTgcStoreHelpClicked)
	ON_COMMAND(ID_HELP_RESET_CHECKS, &CMainFrame::OnHelpResetChecks)
	ON_COMMAND(ID_HELP_PLAYINTROAGAIN, &CMainFrame::OnHelpPlayintroagain)
	ON_COMMAND(ID_HELP_VIEWITEMLICENSES, &CMainFrame::OnHelpViewitemlicenses)
	ON_COMMAND(ID_HELP_WATCHSTOREVIDEO, &CMainFrame::OnHelpWatchstorevideo)
	END_MESSAGE_MAP()

static UINT indicators [ ] =
{
	ID_SEPARATOR,           // status line indicator
		//ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

void CMainFrame::OnSize( UINT nType, int cx, int cy )
{
	if ( !m_wndToolBar.m_hWnd ) return;

	CRect rectA;
	CSize size = m_wndToolBar.CalcFixedLayout( FALSE, TRUE );

	m_wndToolBar.GetWindowRect(&rectA);
	rectA.right = size.cx; rectA.left = 0;
	
	DockControlBar ( &m_wndToolBar, AFX_IDW_DOCKBAR_TOP, &rectA );
	rectA.OffsetRect( m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cx,0 );
	
	DockControlBar ( &m_wndToolBarView, AFX_IDW_DOCKBAR_TOP, &rectA );
	rectA.OffsetRect( m_wndToolBarView.CalcFixedLayout( FALSE, TRUE ).cx,0 );

	DockControlBar ( &m_wndToolBarSegment, AFX_IDW_DOCKBAR_TOP,  &rectA );
	rectA.OffsetRect( m_wndToolBarSegment.CalcFixedLayout( FALSE, TRUE ).cx,0 );

	DockControlBar ( &m_wndToolBarDraw, AFX_IDW_DOCKBAR_TOP,  &rectA );
	rectA.OffsetRect( m_wndToolBarDraw.CalcFixedLayout( FALSE, TRUE ).cx,0 );

	DockControlBar (&m_wndToolBarWaypoint,AFX_IDW_DOCKBAR_TOP,&rectA);
	rectA.OffsetRect( m_wndToolBarWaypoint.CalcFixedLayout( FALSE, TRUE ).cx,0 );

	DockControlBar (&m_wndToolBarGame,AFX_IDW_DOCKBAR_TOP,&rectA );

	CMDIFrameWnd::OnSize( nType, cx,cy );
}

LRESULT CMainFrame::OnCloseLevelWindow (WPARAM wParam, LPARAM lParam )
{
	theApp.m_pDocumentA->OnCloseDocument ( );
	theApp.OnFileNew ( );
	return 0;
}

LRESULT CMainFrame::OnUpdateEntityWindow (WPARAM wParam, LPARAM lParam )
{
	if ( wParam == 1 )
	{
		ShowEntityWindow ( true );
	}

	if ( wParam == 2 )
	{
		ShowEntityWindow ( false );
	}
	return 0;
}

LRESULT CMainFrame::OnThreadFinishedTest (WPARAM wParam, LPARAM lParam )
{
	POSITION pos = theApp.GetFirstDocTemplatePosition ( );
	CDocTemplate* pTemplate = theApp.GetNextDocTemplate ( pos );
	POSITION posA = pTemplate->GetFirstDocPosition ( );
	CDocument* pDoc = pTemplate->GetNextDoc ( posA );
	pDoc->SetTitle ( theApp.m_szProjectName );
	int c = 0;
	return 0;
}

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	memset ( m_bTest, 1, sizeof ( m_bTest ) );
	memset ( m_bTest, 0, sizeof ( m_bTest ) );

	strcpy ( &m_szStatus [ 0 ] [ 0 ], _T ( "" ) );
	strcpy ( &m_szStatus [ 1 ] [ 0 ], _T ( "" ) );
	strcpy ( &m_szStatus [ 2 ] [ 0 ], _T ( "" ) );
	strcpy ( &m_szStatus [ 3 ] [ 0 ], _T ( "" ) );
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam )
{
	return FALSE;

	TCHAR szBuffer  [ 256 ];
	TCHAR szDestroy [ 256 ];

	//GetPrivateProfileString ( _T ( "General" ), _T ( "Destroy Window" ), _T ( "" ), szDestroy, MAX_PATH, _T ( "./files/editors/host/editor.ini" ) );
	GetPrivateProfileString ( _T ( "General" ), _T ( "Destroy Window" ), _T ( "" ), szDestroy, MAX_PATH, theApp.m_szLanguage );

	GetWindowText ( hwnd, szBuffer, 256 );

	// 170105
	//if ( wcscmp ( szBuffer, szDestroy ) == 0 )
	if ( strcmp ( szBuffer, szDestroy ) == 0 )
	{
		// 211204
		//SendMessage ( hwnd, WM_DESTROY, 0, 0 );

		// 211204
		if ( theApp.m_bAppRunning == false )
			return TRUE;

		SendMessage ( hwnd, WM_DESTROY, 0, 0 );
	}

	return TRUE;
}

CMainFrame::~CMainFrame()
{
	// 211204
	//EnumWindows ( EnumWindowsProc, 0 );
}

void CMainFrame::AddBitmapToWorkspace ( int iTab, LPTSTR szBitmap, LPTSTR szName )
{
	m_wndOutput.AddBitmap ( iTab, szBitmap, szName );
}

void CMainFrame::RemoveBitmapFromWorkspace ( int iTab, int iIndex )
{
	m_wndOutput.RemoveBitmap ( iTab, iIndex );
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// enable Office XP look:
	CBCGVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGVisualManagerXP));

	// Load toolbar user images:
	//if (!m_UserImages.Load (_T(".files\\UserImages.bmp")))
	{
	//	CBCGToolBar::SetUserImages (&m_UserImages);
	}

	//CBCGToolBar::EnableQuickCustomization ();

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	// Remove menubar gripper and borders:
	//m_wndMenuBar.SetBarStyle (m_wndMenuBar.GetBarStyle() &
	//	~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	

	//m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_TOOLTIPS );

	m_wndMenuBar.EnableMenuShadows ( FALSE );
	m_wndMenuBar.SetDefaultMenuResId( IDR_MAINFRAME );
	

	

	CBCGToolbarButton* pButton = m_wndMenuBar.GetMenuItem ( 0 );
	//pButton->m_strText = "mike";

	//CMenu* pMen
	//m_wndMenuBar.ModifyMenu ( );
	
	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	//UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR256 : 0;
	//UINT uiToolbarColdID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBARCOLD256 : 0;
	UINT uiToolbarColdID = bIsHighColor ? IDB_TOOLBAR256 : 0;

	//UINT uiToolbarHotID = bIsHighColor ? IDB_ADD_MAIN_FRAME : 0;
	//UINT uiToolbarColdID = bIsHighColor ? IDB_ADD_MAIN_FRAME_DIS : 0;

	// m_wndToolBars

	

	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		//| CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, IDB_TOOLBARCOLD256, 0, FALSE, 0, 0, uiToolbarHotID))

		//!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, IDB_ADD_MAIN_FRAME, 0, FALSE, 0, IDB_ADD_MAIN_FRAME_DIS, IDB_ADD_MAIN_FRAME_DIS))
		//!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, IDB_ADD_MAIN_FRAME, 0, FALSE, 0, 0, 0))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//m_wndToolBar.SetToolBarBtnText ( 0, "LARGE\nLARGE", FALSE, TRUE );

	/* V110 BETA3 - 060608 - Tooltips text A-H do not exist!! Canont run in DEBUG
	m_wndToolBar.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "A" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "B" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 2,  GetLanguageData ( "Tooltips", "C" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 3,  GetLanguageData ( "Tooltips", "D" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 4,  GetLanguageData ( "Tooltips", "E" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 5,  GetLanguageData ( "Tooltips", "F" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 6,  GetLanguageData ( "Tooltips", "G" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 7,  GetLanguageData ( "Tooltips", "H" ) );
	*/

	/*
	void SetToolBarBtnText (UINT nBtnIndex,
							LPCTSTR szText = NULL,
							BOOL bShowText = TRUE,
							BOOL bShowImage = TRUE);
							*/

	if (!m_wndToolBarView.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_VIEW) ||
		!m_wndToolBarView.LoadToolBar(IDR_VIEW, IDB_BITMAP1, 0, FALSE, 0, 0, IDB_BITMAP1))
		//!m_wndToolBarView.LoadToolBar(IDR_VIEW, IDB_BITMAP1, 0, FALSE, 0, IDB_BITMAP14))
		//!m_wndToolBarView.LoadToolBar(IDR_VIEW, IDB_BITMAP1, 0, IDB_BITMAP14, 0, 0))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	//m_wndToolBarView.SetHotBorder ( FALSE );

	//m_wndToolBarView.SetGrayDisabledButtons(FALSE);

	
	if (!m_wndToolBarDraw.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_VIEW) ||
		!m_wndToolBarDraw.LoadToolBar(IDR_DRAW, IDB_BITMAP3, 0, FALSE, 0, 0, IDB_BITMAP3))
		
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}



	if (!m_wndToolBarSegment.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_VIEW) ||
		!m_wndToolBarSegment.LoadToolBar(IDR_SEGMENT, IDB_BITMAP5, 0, FALSE, 0, 0, IDB_BITMAP5))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}



	if (!m_wndToolBarWaypoint.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_VIEW) ||
		!m_wndToolBarWaypoint.LoadToolBar(IDR_WAYPOINT, IDB_BITMAP7, 0, FALSE, 0, 0, IDB_BITMAP7))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	if (!m_wndToolBarGame.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_VIEW) ||
		//!m_wndToolBarWaypoint.LoadToolBar (IDR_WAYPOINT))
		!m_wndToolBarGame.LoadToolBar(IDR_GAME, IDB_BITMAP8, 0, FALSE, 0, 0, IDB_BITMAP8))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	// Each rebar pane will ocupy its own row:
	DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_BREAK;

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//m_wndStatusBar.SetPaneWidth(1,65);
	
	m_wndStatusBar.SetPaneWidth(0,40);
	m_wndStatusBar.SetPaneWidth(1,80);
	m_wndStatusBar.SetPaneWidth(2,65);
	m_wndStatusBar.SetPaneWidth(3,70);


	// mike - 070205 - disable movement of entity properties window
	//if (!m_wndWorkSpace.Create (_T("Workspace"), this, CSize (250, 200), TRUE, ID_VIEW_WORKSPACE, WS_CHILD | CBRS_LEFT ))
	if (!m_wndWorkSpace.Create (_T("Workspace"), this, CSize (250, 200), FALSE, ID_VIEW_WORKSPACE, WS_CHILD | CBRS_LEFT ))
	{
		TRACE0("Failed to create workspace bar\n");
		return -1;      // fail to create
	}

	//if (!m_wndOutput.Create (_T("Library"), this, CSize (145, 500),
	//if (!m_wndOutput.Create (_T("Library"), this, CSize (150, 500),
	if (!m_wndOutput.Create (GetLanguageData ( "Library", "Name" ) , this, CSize (150, 500),
		
		TRUE , ID_VIEW_OUTPUT,
		//WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
		WS_CHILD | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}

	
	EnableDocking                      ( CBRS_ALIGN_ANY );

	m_wndMenuBar.EnableDocking         ( CBRS_ALIGN_ANY );
	m_wndToolBar.EnableDocking         ( CBRS_ALIGN_TOP );
	m_wndToolBarView.EnableDocking     ( CBRS_ALIGN_TOP );
	m_wndToolBarDraw.EnableDocking     ( CBRS_ALIGN_TOP );
	m_wndToolBarSegment.EnableDocking     ( CBRS_ALIGN_TOP );
	m_wndToolBarWaypoint.EnableDocking     ( CBRS_ALIGN_TOP );
	m_wndToolBarGame.EnableDocking     ( CBRS_ALIGN_TOP );
	
	
	DockControlBar ( &m_wndMenuBar );
	DockControlBar ( &m_wndToolBar );
	
	m_wndWorkSpace.EnableDocking   ( CBRS_ALIGN_ANY );
	
	
	m_wndOutput.EnableDocking      ( CBRS_ALIGN_ANY );
	
	CRect rectA;
	CRect rectB;
	CRect rectC;
	CRect rectD;
	CRect rectE;
	CRect rectF;
	CRect rectG;
	CRect rectH;


	RecalcLayout(TRUE);
	

	
	m_wndToolBar.GetWindowRect(&rectA);
	rectA.OffsetRect(1,0);
	
	DockControlBar (&m_wndToolBarGame,AFX_IDW_DOCKBAR_TOP,&rectA);
	DockControlBar (&m_wndToolBarWaypoint,AFX_IDW_DOCKBAR_TOP,&rectA);
	DockControlBar ( &m_wndToolBarSegment, AFX_IDW_DOCKBAR_TOP,  &rectA );
	DockControlBar ( &m_wndToolBarDraw, AFX_IDW_DOCKBAR_TOP,  &rectA );
	DockControlBar ( &m_wndToolBarView,     AFX_IDW_DOCKBAR_TOP, &rectA );
	
	DockControlBar ( &m_wndWorkSpace );

	DockControlBar ( &m_wndOutput,     AFX_IDW_DOCKBAR_LEFT, &rectA );

	
	
	CString strMainToolbarTitle;

	//m_wndToolBar.SetWindowText (strMainToolbarTitle);
	m_wndToolBarView.SetWindowText (_T("View"));
	

	//RecalcLayout(TRUE);
	

	{
	
		// FREE VERSION - serial code read from USERDETAILS.INI (else free version)
		theApp.CheckSerialCodeValidity();

		TCHAR szStringA [ MAX_PATH ];
		GetPrivateProfileString ( _T("Application"), _T("Name"), _T(""), szStringA, MAX_PATH, theApp.m_szLanguage );
		// V110 - 110608 - add FREE if the free version
		if ( g_bFreeVersion ) strcat ( szStringA, " Free" );
		SetTitle ( szStringA );
		SetWindowText ( szStringA );

		/*
		// 170105
		// CONVERT WIDE TO ANSI
		{
			char szDestination [ MAX_PATH ];
			int  iLen     = wcslen ( theApp.m_szLanguage );
			int  nResult = WideCharToMultiByte ( CP_ACP, 0, theApp.m_szLanguage, -1, szDestination, iLen * sizeof ( CHAR ), NULL, NULL );
			szDestination [ iLen ] = 0;
			int c = 0;
		}
		*/

		//SetTitle ( _T("Bob") );
		//SetWindowText ( _T("Bob") );
	}


	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	m_pToolBarList [ 0 ] = &m_wndToolBar;
	m_pToolBarList [ 1 ] = &m_wndToolBarView;
	//m_pToolBarList [ 2 ] = &m_wndToolBarProperty;
	m_pToolBarList [ 3 ] = &m_wndToolBarDraw;
	//m_pToolBarList [ 4 ] = &m_wndToolBarSelection;
	m_pToolBarList [ 5 ] = &m_wndToolBarSegment;
	//m_pToolBarList [ 6 ] = &m_wndToolBarEntity;
	m_pToolBarList [ 7 ] = &m_wndToolBarWaypoint;
	m_pToolBarList [ 8 ] = &m_wndToolBarGame;
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGToolbarCustomize* pDlgCust = new CBCGToolbarCustomize (this,
		TRUE /* Automatic menus scaning */
		, (	BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | 
			BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS |
			BCGCUSTOMIZE_SELECT_SKINS)
		);

	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	// 210205 - don't show right mouse button popup menu
	return 0;

	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CBCGPopupMenu* pPopupMenu = new CBCGPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CBCGPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
    CMDIFrameWnd::OnShowPopupMenu (pMenuPopup);

	if ( pMenuPopup )
	{
		// get the parent button of the popup menu
		CBCGToolbarMenuButton* pParent = pMenuPopup->GetParentButton ( );

		if ( pParent )
		{
			// get number of items in menu
			int iCount = pMenuPopup->GetMenuItemCount ( );

			// array to store strings for menu items
			TCHAR szMenu  [ 10 ] [ MAX_PATH ];
			TCHAR szPopup [ 10 ] [ MAX_PATH ];

			// array of buttons
			CBCGToolbarMenuButton* pButtons [ 20 ];

			// get menu list from language file
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "A" ), _T ( "" ), szMenu [ 0 ], MAX_PATH, theApp.m_szLanguage );	// file
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "B" ), _T ( "" ), szMenu [ 1 ], MAX_PATH, theApp.m_szLanguage );	// edit
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "C" ), _T ( "" ), szMenu [ 2 ], MAX_PATH, theApp.m_szLanguage );	// view
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "D" ), _T ( "" ), szMenu [ 3 ], MAX_PATH, theApp.m_szLanguage );	// tools
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "E" ), _T ( "" ), szMenu [ 4 ], MAX_PATH, theApp.m_szLanguage );	// help
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "F" ), _T ( "" ), szMenu [ 5 ], MAX_PATH, theApp.m_szLanguage );	// help
			GetPrivateProfileString ( _T ( "Menu" ), _T ( "G" ), _T ( "" ), szMenu [ 6 ], MAX_PATH, theApp.m_szLanguage );	// toolbars

			// file menu update
			if ( strcmp ( pParent->m_strText, szMenu [ 0 ] ) == 0 )
			{
				// get strings for file menu from language file
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "C" ), _T ( "" ), szPopup [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "D" ), _T ( "" ), szPopup [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "E" ), _T ( "" ), szPopup [ 4 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "F" ), _T ( "" ), szPopup [ 5 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "G" ), _T ( "" ), szPopup [ 6 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "H" ), _T ( "" ), szPopup [ 7 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "File Menu" ), _T ( "I" ), _T ( "" ), szPopup [ 8 ], MAX_PATH, theApp.m_szLanguage );

				// assign buttons
				for ( int i = 0; i < iCount; i++ )
					pButtons [ i ] = pMenuPopup->GetMenuItem ( i );

				// FILE MENU layout
				pMenuPopup->GetMenuItem ( 0 )->m_strText = szPopup [ 0 ];
				pMenuPopup->GetMenuItem ( 1 )->m_strText = szPopup [ 7 ];
				pMenuPopup->GetMenuItem ( 2 )->m_strText = szPopup [ 1 ];
				pMenuPopup->GetMenuItem ( 3 )->m_strText = szPopup [ 2 ];
				pMenuPopup->GetMenuItem ( 4 )->m_strText = szPopup [ 3 ];

				// FREE VERSION
				if ( g_bFreeVersion )
					pMenuPopup->GetMenuItem ( 6 )->m_strText = szPopup [ 8 ];
				else
					pMenuPopup->GetMenuItem ( 6 )->m_strText = szPopup [ 4 ];

				pMenuPopup->GetMenuItem ( 8 )->m_strText = szPopup [ 5 ];
				pMenuPopup->GetMenuItem ( iCount - 1 )->m_strText = szPopup [ 6 ];					
			}

			// edit menu update
			if ( strcmp ( pParent->m_strText, szMenu [ 1 ] ) == 0 )
			{
				// get strings for edit menu from language file
				GetPrivateProfileString ( _T ( "Edit Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Edit Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Edit Menu" ), _T ( "C" ), _T ( "" ), szPopup [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Edit Menu" ), _T ( "D" ), _T ( "" ), szPopup [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Edit Menu" ), _T ( "E" ), _T ( "" ), szPopup [ 4 ], MAX_PATH, theApp.m_szLanguage );

				pButtons [ 0 ]            = pMenuPopup->GetMenuItem ( 0 );			// undo
				pButtons [ 1 ]            = pMenuPopup->GetMenuItem ( 1 );			// redo
				pButtons [ 2 ]            = pMenuPopup->GetMenuItem ( 3 );			// cut
				pButtons [ 3 ]            = pMenuPopup->GetMenuItem ( 4 );			// copy
				pButtons [ 4 ]            = pMenuPopup->GetMenuItem ( 5 );			// paste

				pButtons [ 0 ]->m_strText = szPopup [ 0 ];							// undo
				pButtons [ 1 ]->m_strText = szPopup [ 1 ];							// redo
				pButtons [ 2 ]->m_strText = szPopup [ 2 ];							// cut
				pButtons [ 3 ]->m_strText = szPopup [ 3 ];							// copy
				pButtons [ 4 ]->m_strText = szPopup [ 4 ];							// paste
			}

			// segments menu
			if ( strcmp ( pParent->m_strText, szMenu [ 2 ] ) == 0 )
			{
				// get strings for edit menu from language file
				GetPrivateProfileString ( _T ( "Segment Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Segment Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Segment Menu" ), _T ( "C" ), _T ( "" ), szPopup [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Segment Menu" ), _T ( "D" ), _T ( "" ), szPopup [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Segment Menu" ), _T ( "E" ), _T ( "" ), szPopup [ 4 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Segment Menu" ), _T ( "F" ), _T ( "" ), szPopup [ 5 ], MAX_PATH, theApp.m_szLanguage );

				pButtons [ 0 ]            = pMenuPopup->GetMenuItem ( 0 );			// undo
				pButtons [ 1 ]            = pMenuPopup->GetMenuItem ( 1 );			// redo
				pButtons [ 2 ]            = pMenuPopup->GetMenuItem ( 2 );			// cut
				pButtons [ 3 ]            = pMenuPopup->GetMenuItem ( 3 );			// copy
				pButtons [ 4 ]            = pMenuPopup->GetMenuItem ( 4 );			// paste
				pButtons [ 5 ]            = pMenuPopup->GetMenuItem ( 5 );			// paste

				pButtons [ 0 ]->m_strText = szPopup [ 0 ];							// undo
				pButtons [ 1 ]->m_strText = szPopup [ 1 ];							// redo
				pButtons [ 2 ]->m_strText = szPopup [ 2 ];							// cut
				pButtons [ 3 ]->m_strText = szPopup [ 3 ];							// copy
				pButtons [ 4 ]->m_strText = szPopup [ 4 ];							// paste
				pButtons [ 5 ]->m_strText = szPopup [ 5 ];							// paste
			}

			// segments menu
			if ( strcmp ( pParent->m_strText, szMenu [ 3 ] ) == 0 )
			{
				// get strings for edit menu from language file
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "C" ), _T ( "" ), szPopup [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "D" ), _T ( "" ), szPopup [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "E" ), _T ( "" ), szPopup [ 4 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "F" ), _T ( "" ), szPopup [ 5 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "G" ), _T ( "" ), szPopup [ 6 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Editing Menu" ), _T ( "H" ), _T ( "" ), szPopup [ 7 ], MAX_PATH, theApp.m_szLanguage );

				pButtons [ 0 ]            = pMenuPopup->GetMenuItem ( 0 );			// undo
				pButtons [ 1 ]            = pMenuPopup->GetMenuItem ( 1 );			// redo
				pButtons [ 2 ]            = pMenuPopup->GetMenuItem ( 2 );			// cut
				pButtons [ 3 ]            = pMenuPopup->GetMenuItem ( 3 );			// copy
				pButtons [ 4 ]            = pMenuPopup->GetMenuItem ( 4 );			// paste
				pButtons [ 5 ]            = pMenuPopup->GetMenuItem ( 5 );			// paste
				pButtons [ 6 ]            = pMenuPopup->GetMenuItem ( 6 );			// paste
				pButtons [ 7 ]            = pMenuPopup->GetMenuItem ( 7 );			// paste

				pButtons [ 0 ]->m_strText = szPopup [ 0 ];							// undo
				pButtons [ 1 ]->m_strText = szPopup [ 1 ];							// redo
				pButtons [ 2 ]->m_strText = szPopup [ 2 ];							// cut
				pButtons [ 3 ]->m_strText = szPopup [ 3 ];							// copy
				pButtons [ 4 ]->m_strText = szPopup [ 4 ];							// paste
				pButtons [ 5 ]->m_strText = szPopup [ 5 ];							// paste
				pButtons [ 6 ]->m_strText = szPopup [ 6 ];							// paste
				pButtons [ 7 ]->m_strText = szPopup [ 7 ];							// paste
				
			}

			// view menu update
			if ( strcmp ( pParent->m_strText, szMenu [ 4 ] ) == 0 )
			{
				// get strings for edit menu from language file
				GetPrivateProfileString ( _T ( "View Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "View Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );

				pButtons [ 0 ]            = pMenuPopup->GetMenuItem ( 0 );	// toolbars
				pButtons [ 1 ]            = pMenuPopup->GetMenuItem ( 1 );	// status bar

				pButtons [ 0 ]->m_strText = szPopup [ 0 ];					// toolbars
				pButtons [ 1 ]->m_strText = szPopup [ 1 ];					// status bar
			}

			// help menu update
			if ( strcmp ( pParent->m_strText, szMenu [ 5 ] ) == 0 )
			{
				// get strings for edit menu from language file
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "C" ), _T ( "" ), szPopup [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "D" ), _T ( "" ), szPopup [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "E" ), _T ( "" ), szPopup [ 4 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "F" ), _T ( "" ), szPopup [ 5 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "G" ), _T ( "" ), szPopup [ 6 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "H" ), _T ( "" ), szPopup [ 7 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Help Menu" ), _T ( "I" ), _T ( "" ), szPopup [ 8 ], MAX_PATH, theApp.m_szLanguage );
				
				pButtons [ 0 ]            = pMenuPopup->GetMenuItem ( 0 );				// new window
				pButtons [ 1 ]            = pMenuPopup->GetMenuItem ( 1 );				// cascade
				pButtons [ 2 ]            = pMenuPopup->GetMenuItem ( 2 );
				pButtons [ 3 ]            = pMenuPopup->GetMenuItem ( 3 );
				pButtons [ 4 ]            = pMenuPopup->GetMenuItem ( 5 );
				pButtons [ 5 ]            = pMenuPopup->GetMenuItem ( 6 );
				pButtons [ 6 ]            = pMenuPopup->GetMenuItem ( 7 );
				
				// 110805 - remove dwebsitr due to strange slow response..
				pButtons [ 0 ]->m_strText = szPopup [ 1 ];								// new window
				pButtons [ 1 ]->m_strText = szPopup [ 2 ];								// cascade
				pButtons [ 2 ]->m_strText = szPopup [ 5 ];
				pButtons [ 3 ]->m_strText = szPopup [ 4 ];
				pButtons [ 4 ]->m_strText = szPopup [ 6 ];
				pButtons [ 5 ]->m_strText = szPopup [ 7 ];
				pButtons [ 6 ]->m_strText = szPopup [ 8 ];
			}
		}
	}
	

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);

		{
			// get number of items in menu
				int iCount = pMenuPopup->GetMenuItemCount ( );

				// array to store strings for menu items
//				TCHAR szMenu  [ 10 ] [ MAX_PATH ];
				TCHAR szPopup [ 10  ] [ MAX_PATH ];

				// array of buttons
				CBCGToolbarMenuButton* pButtons [ 20 ];

				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "A" ), _T ( "" ), szPopup [ 0 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "B" ), _T ( "" ), szPopup [ 1 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "C" ), _T ( "" ), szPopup [ 2 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "D" ), _T ( "" ), szPopup [ 3 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "E" ), _T ( "" ), szPopup [ 4 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "F" ), _T ( "" ), szPopup [ 5 ], MAX_PATH, theApp.m_szLanguage );
				GetPrivateProfileString ( _T ( "Toolbar Menu" ), _T ( "G" ), _T ( "" ), szPopup [ 6 ], MAX_PATH, theApp.m_szLanguage );

				pButtons [ 0 ]            = pMenuPopup->GetMenuItem ( 0 );				// new window
				pButtons [ 1 ]            = pMenuPopup->GetMenuItem ( 1 );				// cascade
				pButtons [ 2 ]            = pMenuPopup->GetMenuItem ( 2 );				// tile
				pButtons [ 3 ]            = pMenuPopup->GetMenuItem ( 3 );				// new window
				pButtons [ 4 ]            = pMenuPopup->GetMenuItem ( 4 );				// cascade
				pButtons [ 5 ]            = pMenuPopup->GetMenuItem ( 5 );				// tile
				pButtons [ 6 ]            = pMenuPopup->GetMenuItem ( 6 );				// tile

				pButtons [ 0 ]->m_strText = szPopup [ 0 ];
				
				pButtons [ 1 ]->m_strText = szPopup [ 1 ];
				pButtons [ 2 ]->m_strText = szPopup [ 2 ];
				pButtons [ 3 ]->m_strText = szPopup [ 3 ];
				pButtons [ 4 ]->m_strText = szPopup [ 4 ];
				pButtons [ 5 ]->m_strText = szPopup [ 5 ];
				pButtons [ 6 ]->m_strText = szPopup [ 6 ];
		}
    }

	return TRUE;
}

void CMainFrame::SetWorkspaceVisible ( bool bVisible )
{
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::ShowEntityWindow ( bool bVisible )
{
	if ( bVisible )
	{
		//ShowControlBar (&m_wndWorkSpace,TRUE,FALSE);

		m_wndWorkSpace.SetVisible ( TRUE );

		m_wndOutput.ShowWindow ( 0 );
	}
	else
	{
		m_wndWorkSpace.SetVisible ( FALSE );
		//ShowControlBar (&m_wndWorkSpace,FALSE,FALSE);

		m_wndOutput.ShowWindow ( 1 );
	}

	RecalcLayout ();
}

void CMainFrame::SetEntityVisible ( bool bVisible )
{
	//ShowControlBar (&m_wndOutput, !(m_wndOutput.GetStyle () & WS_VISIBLE), FALSE);

	ShowControlBar (&m_wndOutput, WS_VISIBLE, FALSE);
	

	RecalcLayout ();
	
}

void CMainFrame::OnViewWorkspace() 
{
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndWorkSpace.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnViewView() 
{
	ShowControlBar (&m_wndToolBarView,
					!(m_wndToolBarView.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndOutput.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnUpdateViewView(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndToolBarView.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CMDIFrameWnd::OnPaint() for painting messages

	m_wndToolBar.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "1" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "2" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 2,  GetLanguageData ( "Tooltips", "3" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 3,  GetLanguageData ( "Tooltips", "4" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 4,  GetLanguageData ( "Tooltips", "5" ) );
	m_wndToolBar.SetToolBarBtnTooltip ( 5,  GetLanguageData ( "Tooltips", "6" ) );
	// V110 BETA3 - 060608 - removed UNDO/REDO buttons
	// m_wndToolBar.SetToolBarBtnTooltip ( 6,  GetLanguageData ( "Tooltips", "7" ) );
	// m_wndToolBar.SetToolBarBtnTooltip ( 7,  GetLanguageData ( "Tooltips", "8" ) );

	m_wndToolBarView.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "9" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "10" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 2,  GetLanguageData ( "Tooltips", "11" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 3,  GetLanguageData ( "Tooltips", "12" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 4,  GetLanguageData ( "Tooltips", "13" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 5,  GetLanguageData ( "Tooltips", "14" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 6,  GetLanguageData ( "Tooltips", "15" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 7,  GetLanguageData ( "Tooltips", "16" ) );
	m_wndToolBarView.SetToolBarBtnTooltip ( 8,  GetLanguageData ( "Tooltips", "17" ) );

	m_wndToolBarDraw.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "18" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "19" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 2,  GetLanguageData ( "Tooltips", "20" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 3,  GetLanguageData ( "Tooltips", "21" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 4,  GetLanguageData ( "Tooltips", "22" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 5,  GetLanguageData ( "Tooltips", "23" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 6,  GetLanguageData ( "Tooltips", "24" ) );
	m_wndToolBarDraw.SetToolBarBtnTooltip ( 7,  GetLanguageData ( "Tooltips", "25" ) );

	m_wndToolBarSegment.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "26" ) );
	m_wndToolBarSegment.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "27" ) );
	m_wndToolBarSegment.SetToolBarBtnTooltip ( 2,  GetLanguageData ( "Tooltips", "28" ) );
	m_wndToolBarSegment.SetToolBarBtnTooltip ( 3,  GetLanguageData ( "Tooltips", "29" ) );
	m_wndToolBarSegment.SetToolBarBtnTooltip ( 4,  GetLanguageData ( "Tooltips", "30" ) );
	m_wndToolBarSegment.SetToolBarBtnTooltip ( 5,  GetLanguageData ( "Tooltips", "31" ) );

	m_wndToolBarWaypoint.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "32" ) );
	m_wndToolBarWaypoint.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "33" ) );

	m_wndToolBarGame.SetToolBarBtnTooltip ( 0,  GetLanguageData ( "Tooltips", "34" ) );
	m_wndToolBarGame.SetToolBarBtnTooltip ( 1,  GetLanguageData ( "Tooltips", "35" ) );
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	

	CMDIFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIFrameWnd::OnLButtonUp(nFlags, point);
}

void CMainFrame::SetStatusBarText ( int iIndex, LPTSTR pszText )
{
	// 170105
	//wcscpy ( &m_szStatus [ iIndex ] [ 0 ], _T ( "" ) );
	//wcscpy ( &m_szStatus [ iIndex ] [ 0 ], pszText );

	strcpy ( &m_szStatus [ iIndex ] [ 0 ], _T ( "" ) );
	strcpy ( &m_szStatus [ iIndex ] [ 0 ], pszText );

	
}

void CMainFrame::OnMouseMoveXX(UINT nFlags, CPoint point)
{
	
	TCHAR szBufferX [ 12 ];
	TCHAR szBufferY [ 12 ];

	TCHAR szFinal [ 12 ];

	// 170105
	//_itow ( point.x, szBufferX, 10 );
	//_itow ( point.y, szBufferY, 10 );
	_itoa ( point.x, szBufferX, 10 );
	_itoa ( point.y, szBufferY, 10 );

	TCHAR szString [ 2 ] [ MAX_PATH ];
	GetPrivateProfileString ( _T ( "Status" ), _T ( "C" ), _T ( "" ), szString [ 0 ], MAX_PATH, theApp.m_szLanguage );
	GetPrivateProfileString ( _T ( "Status" ), _T ( "D" ), _T ( "" ), szString [ 1 ], MAX_PATH, theApp.m_szLanguage );

	// 170105
	//swprintf ( szFinal, _T ( "%s %i %s %i" ), szString [ 0 ], point.x, szString [ 1 ], point.y );
	sprintf ( szFinal, _T ( "%s %i %s %i" ), szString [ 0 ], point.x, szString [ 1 ], point.y );
	


	{
		char szA [ 255 ];
		char szB [ 255 ];
		char szC [ 255 ];
		char szD [ 255 ];

		cIPC* pIPC			= theApp.m_Message.GetIPC ( );
	
		pIPC->ReceiveBuffer ( &szA, 4000, sizeof ( char ) * 255 );
		pIPC->ReceiveBuffer ( &szB, 4256, sizeof ( char ) * 255 );
		pIPC->ReceiveBuffer ( &szC, 4512, sizeof ( char ) * 255 );
		pIPC->ReceiveBuffer ( &szD, 4768, sizeof ( char ) * 255 );
		

		//memcpy ( &szA [ 0 ], &theApp.m_pData [ 4000 ], sizeof ( char ) * 255 );
		//memcpy ( &szB [ 0 ], &theApp.m_pData [ 4256 ], sizeof ( char ) * 255 );
		//memcpy ( &szC [ 0 ], &theApp.m_pData [ 4512 ], sizeof ( char ) * 255 );
		//memcpy ( &szD [ 0 ], &theApp.m_pData [ 4768 ], sizeof ( char ) * 255 );

		CString a = szA;
		CString b = szB;
		CString c = szC;
		CString d = szD;

		if ( m_wndStatusBar )
		{
			m_wndStatusBar.SetPaneText ( 0, a, TRUE );
			m_wndStatusBar.SetPaneText ( 1, b, TRUE );
			m_wndStatusBar.SetPaneText ( 2, c, TRUE );
			m_wndStatusBar.SetPaneText ( 3, d, TRUE );
		}
	}

}

void CMainFrame::OnViewZoomIn() 
{
	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnViewZoomOut() 
{
	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 2 );
}

void CMainFrame::OnViewIncreaseShroud() 
{
	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 3 );
}

void CMainFrame::OnViewDecreaseShroud() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 4 );
}

void CMainFrame::OnViewLayers() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 5 );
}

void CMainFrame::OnViewMoveUpLayer() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 6 );
}

void CMainFrame::OnViewMoveDownLayer() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 7 );
}

void CMainFrame::OnViewOverview() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 8 );
}

void CMainFrame::OnViewCloseup() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 2 );
	theApp.SetFileMapData ( 204, 9 );
}

void CMainFrame::OnModePaint() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnModeSelect() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnModeArt() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnModeEntity() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnModeWaypoint() 
{
	// TODO: Add your command handler code here
	
}

/*
void CMainFrame::OnMenuViewMode() 
{
	// TODO: Add your command handler code here

	ShowControlBar (&m_wndToolBarMode,
					!(m_wndToolBarMode.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
	
}
*/

void CMainFrame::OnUpdateViewMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndToolBarMode.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnUpdateViewRotate(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck (m_wndToolBarProperty.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnPropertyRotate() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 3 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnPropertyMirror() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 3 );
	theApp.SetFileMapData ( 204, 3 );
}

void CMainFrame::OnPropertyFlip() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 3 );
	theApp.SetFileMapData ( 204, 2 );
}

void CMainFrame::OnDrawA() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnDrawB() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 2 );
}

void CMainFrame::OnDrawC() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 3 );
}

void CMainFrame::OnDrawD() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 4 );
}

void CMainFrame::OnDrawStateNone() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 5 );
}

void CMainFrame::OnDrawPick() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 6 );
}

void CMainFrame::OnSelectionCut() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 5 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnSelectionCopy() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 5 );
	theApp.SetFileMapData ( 204, 2 );
}

void CMainFrame::OnSelectionPaste() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 5 );
	theApp.SetFileMapData ( 204, 3 );
}

void CMainFrame::OnSelectionClear() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 5 );
	theApp.SetFileMapData ( 204, 5 );
}

void CMainFrame::OnSelectionClearNopaste() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 5 );
	theApp.SetFileMapData ( 204, 6 );
}

void CMainFrame::OnSelectionReplace() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 5 );
	theApp.SetFileMapData ( 204, 4 );
}

void CMainFrame::OnSegmentLine() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 6 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnSegmentBox() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 6 );
	theApp.SetFileMapData ( 204, 2 );
}

void CMainFrame::OnSegmentCircle() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 6 );
	theApp.SetFileMapData ( 204, 3 );
}

void CMainFrame::OnSegmentDecrease() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 6 );
	theApp.SetFileMapData ( 204, 6 );
}

void CMainFrame::OnSegmentIncrease() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 6 );
	theApp.SetFileMapData ( 204, 5 );
}

void CMainFrame::OnSegmentSpray() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 6 );
	theApp.SetFileMapData ( 204, 4 );
}

void CMainFrame::OnEntityDelete() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 7 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnEntityRotateX() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 7 );
	theApp.SetFileMapData ( 204, 2 );
}

void CMainFrame::OnEntityRotateY() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 7 );
	theApp.SetFileMapData ( 204, 3 );
}

void CMainFrame::OnEntityRotateZ() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 7 );
	theApp.SetFileMapData ( 204, 4 );
}

void CMainFrame::OnWaypointCreate() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 8 );
	theApp.SetFileMapData ( 204, 1 );
}

void CMainFrame::OnViewRotate() 
{
}

void CMainFrame::OnViewPrefab() 
{
	// TODO: Add your command handler code here
		ShowControlBar (&m_wndToolBarDraw,
					!(m_wndToolBarDraw.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewPrefab(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndToolBarDraw.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewSelection() 
{
	// TODO: Add your command handler code here
		//ShowControlBar (&m_wndToolBarSelection,
		//			!(m_wndToolBarSelection.GetStyle () & WS_VISIBLE),
		//			FALSE);
	//RecalcLayout ();
}

void CMainFrame::OnUpdateViewSelection(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck (m_wndToolBarSelection.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewDraw() 
{
	// TODO: Add your command handler code here
			ShowControlBar (&m_wndToolBarSegment,
					!(m_wndToolBarSegment.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewDraw(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndToolBarSegment.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewRotateEntity() 
{
	// TODO: Add your command handler code here
	//ShowControlBar (&m_wndToolBarEntity,
	//	!(m_wndToolBarEntity.GetStyle () & WS_VISIBLE),
	//	FALSE);
	//RecalcLayout ();
}

void CMainFrame::OnUpdateViewEntity(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck (m_wndToolBarEntity.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewWaypoint() 
{

	

	ShowControlBar (&m_wndToolBarWaypoint,
		!(m_wndToolBarWaypoint.GetStyle () & WS_VISIBLE),
		FALSE);
	RecalcLayout ();
}

void CMainFrame::OnViewTestGame() 
{
	// TODO: Add your command handler code here
	ShowControlBar (&m_wndToolBarGame,
		!(m_wndToolBarGame.GetStyle () & WS_VISIBLE),
		FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewTestGame(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ( theApp.m_bDisable );
	pCmdUI->SetCheck (m_wndToolBarGame.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnUpdateViewWaypoint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ( theApp.m_bDisable );

	pCmdUI->SetCheck (m_wndToolBarWaypoint.GetStyle () & WS_VISIBLE);

	//pCmdUI->SetCheck (!WS_VISIBLE);
}

BOOL CALLBACK EnumWindowsProcEx(HWND hwnd,LPARAM lParam );

void CMainFrame:: Quit ( void )
{
	PostQuitMessage ( 0 );
}

void CMainFrame::OnTestGame() 
{
	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 200, 9 );
	theApp.SetFileMapData ( 204, 1 );

	theApp.SetFileMapData ( 916, 1 );
}

void CMainFrame::OnUpdateTestGame ( CCmdUI* pCmdUI )
{
	//SetToolbarButtonState ( 9, 1, 1 );

	//pCmdUI->Enable ( m_bTest [ 9 ] [ 0 ] );
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateTestMap ( CCmdUI* pCmdUI )
{
	//SetToolbarButtonState ( 9, 1, 1 );

	//pCmdUI->Enable ( m_bTest [ 9 ] [ 0 ] );
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::SetToolbarButtonState ( int iToolbar, int iButton, int iState )
{
	m_bTest [ iToolbar ] [ iButton ] = iState;
}

void CMainFrame::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewIncreaseShroud(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewDecreaseShroud(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewLayers(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewMoveupLayer(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewMovedownLayer(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewOverview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateViewCloseUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateModePaint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnUpdateModeSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnUpdateModeArt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnUpdateModeEntity(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnUpdateModeWaypoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnUpdatePropertyRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdatePropertyFlip(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdatePropertyMirror(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateDrawA(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateDrawB(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateDrawC(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateDrawD(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateDrawStateNone(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateDrawPick(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSelectionCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSelectionCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSelectionPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSelectionClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSelectionClearNoPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSelectionReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSegmentLine(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSegmentBox(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );	
}

void CMainFrame::OnUpdateSegmentCircle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSegmentDecrease(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );	
}

void CMainFrame::OnUpdateSegmentIncrease(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateSegmentSpray(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateEntityDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateEntityRotateX(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateEntityRotateY(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );	
}

void CMainFrame::OnUpdateEntityRotateZ(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateWaypointCreate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateModeWaypointSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ( theApp.m_bDisable );
}


void CMainFrame::OnUpdateLevelResizelevel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnUpdateFilePreferences(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

/*
void CMainFrame::OnUpdateViewRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
*/

/*
void CMainFrame::OnUpdateViewPrefab(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
*/

/*
void CMainFrame::OnUpdateViewSelection(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
*/

/*
void CMainFrame::OnUpdateViewDraw(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
*/

void CMainFrame::OnUpdateViewRotateEntity(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

/*
void CMainFrame::OnUpdateViewWaypoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
*/
void CMainFrame::OnFileSave() 
{
	theApp.SetFileMapData       ( 404, 1 );

	//OnFileSaveAs ( );
}

void CMainFrame::OnFileSaveAs() 
{
	/*
	// display a save dialog box
	OPENFILENAME	ofn;				// common dialog box structure
	TCHAR			szFile [ 260 ];		// buffer for filename
	HANDLE			hf;					// file handle
	int				iLen;				// length of file
	
	memset ( szFile,  0, sizeof ( szFile ) );
	memset ( &ofn,    0, sizeof ( ofn    ) );

	ofn.lStructSize     = sizeof ( ofn );
	ofn.hwndOwner       = NULL;
	ofn.lpstrFile       = szFile;
	ofn.nMaxFile        = sizeof ( szFile );
	ofn.lpstrFilter     = _T ( "Maps\0*.fpm" );
	ofn.nFilterIndex    = 1;
	ofn.lpstrFileTitle  = NULL;
	ofn.nMaxFileTitle   = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	SetCurrentDirectory ( theApp.m_szDirectory );

	SetCurrentDirectory ( _T ( "files\\mapbank" ) );
	
	if ( GetSaveFileName ( &ofn ) == TRUE )
		wcscpy ( szFile, ofn.lpstrFile );

	SetCurrentDirectory ( theApp.m_szDirectory );
	*/

	theApp.SetFileMapData       ( 434, 1 );
	//theApp.SetFileMapDataString ( 1000, szFile );
}

/*
void CMainFrame::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIFrameWnd::OnLButtonDblClk(nFlags, point);
}
*/

void CMainFrame::OnEditCopy1() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 962, 1 );
}

void CMainFrame::OnEditCut1() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 958, 1 );
}

void CMainFrame::OnEditPaste1() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 966, 1 );
}

void CMainFrame::OnEditRedo1() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 954, 1 );
}

void CMainFrame::OnEditUndo1() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 950, 1 );
}

void CMainFrame::OnHelpIndex() 
{
	// TODO: Add your command handler code here

	TCHAR szLink [ MAX_PATH ];

	GetPrivateProfileString ( _T ( "Help" ), _T ( "Index" ), _T ( "" ), szLink, MAX_PATH, theApp.m_szLanguage );
	
	HINSTANCE hInstance = ShellExecute ( NULL, _T ( "open" ), szLink, _T ( "" ), NULL, SW_SHOWNORMAL );
}

void CMainFrame::OnWaypointSelect() 
{
	// TODO: Add your command handler code here

	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 200, 8 );
	theApp.SetFileMapData ( 204, 2 );
	
}

void CMainFrame::OnTestMap() 
{
	// TODO: Add your command handler code here

	theApp.SetFileMapData ( 60, 1 );

	theApp.SetFileMapData ( 200, 9 );
	theApp.SetFileMapData ( 204, 2 );
	
}


void CMainFrame::LaunchGameCreatorStore( bool bGotoAddons )
{
	DWORD dwState = 0;
	BOOL bOnline = InternetGetConnectedState( &dwState, 0 );
	if ( !bOnline )
	{
		MessageBox( "Could not detect an internet connection", theApp.m_szErrorString );
	}
	else
	{
		char str [ 256 ];
		theApp.GetMainWnd()->GetWindowText( str, 256 );

		theApp.m_iAppX = 0;
		theApp.m_iAppY = 0;
		theApp.m_iAppWidth = 0;
		theApp.m_iAppHeight = 0;

		theApp.m_bGotoAddons = bGotoAddons;

		theApp.GetMainWnd()->SetWindowText( "FPS Creator - Game Creator Store" );
		CMarketWindow TGCStore;
		TGCStore.DoModal();

		theApp.m_bGotoAddons = false;

		theApp.GetMainWnd()->SetWindowPos( NULL, theApp.m_iAppX,theApp.m_iAppY, theApp.m_iAppWidth,theApp.m_iAppHeight, 0 );
		//theApp.GetMainWnd()->ShowWindow( SW_SHOW );
		
		//theApp.GetMainWnd()->SetWindowText( str );
		theApp.GetMainWnd()->SetWindowText( "FPS Creator" );

		// V110 - 110608 - add FREE if the free version
		if ( g_bFreeVersion ) theApp.GetMainWnd()->SetWindowText( "FPS Creator Free" );
	}
}

#include "cDialogEnd.h"
extern int g_iBoysLifeQuit;

/* replaced with newer function from the combined-interface below (101109)
void CMainFrame::OnFileBuildgame() 
{
	// FREE VERSION
	if ( g_bFreeVersion )
	{
		// V110 FREE - 120608 - display message and allow to cancel jump to upgrade store
		char pMessageText[] = "\
You can upgrade FPS Creator with the ability to create standalone executables\n \
of your games, which include multi-level and multiplayer arena games. The upgrade\n \
also grants a royalty free license to sell what you make.\n \
\n \
To upgrade:\n \
\n \
* Click on the “Game Creator Store” button\n \
* Create a store account or log-in if you already have one\n \
* Buy the “Build Game Upgrade” from the TGC/Add-ons section\n \
\n \
Once purchased you will see a new Build Game menu item in the File menu.\n \
Would you like to go to the Game Creator Store now?";

		if ( MessageBox ( pMessageText, GetLanguageData( "File Menu", "I" ), MB_YESNO )==IDYES )
		{
			// V110 BETA4 - 070608 - launch Game Creator Store module
			LaunchGameCreatorStore( true );
		}
	}
	else
	{
		// flag BUILDGAME has selected
		theApp.SetFileMapData ( 758, 1 );
	}
}
*/

void CMainFrame::OnFileBuildgame() 
{
	// FREE VERSION
	if ( g_bFreeVersion )
	{
		// V110 FREE - 120608 - display message and allow to cancel jump to upgrade store
		char pMessageText[] = "\
You can upgrade FPS Creator with the ability to create standalone executables\n \
of your games, which include multi-level and multiplayer arena games. The upgrade \
also grants a royalty free license to sell what you make.\n \
\n \
To upgrade:\n \
\n \
* Click on the “Game Creator Store” button\n \
* Create a store account or log-in if you already have one\n \
* Buy the “Build Game Upgrade” from the TGC/Add-ons section\n \
\n \
Once purchased you will see a new Build Game menu item in the File menu.\n \
Would you like to go to the Game Creator Store now?";

		// display message box, jump to store if click YES
		if ( MessageBox ( pMessageText, GetLanguageData( "File Menu", "I" ), MB_YESNO )==IDYES )
		{
			// V110 BETA4 - 070608 - launch Game Creator Store module
			LaunchGameCreatorStore( true );
		}
		else
		{
			// FPGC - 230909 - Second Bite At The Cherry (offer it for free)
		char pTrialPayText[] = "\
Instead of buying the upgrade, how about getting it absolutely FREE?\n \
\n\
TrialPay are offering you the incredible opportunity to obtain FPS Creator fully featured \
and includes additional game building media plus of course the ability to save standalone games \
and multiplayer games.\n \
\n\
Would you like to learn more about this offer?";

			// display message box, jump to store if click YES
			if ( MessageBox ( pTrialPayText, GetLanguageData( "File Menu", "I" ), MB_YESNO )==IDYES )
			{
				// FPGC - 230909 - launch trial pay website
				char szBrowser [ MAX_PATH ];
				theApp.GetDefaultBrowserPath( szBrowser, MAX_PATH );
				ShellExecute ( NULL, "open", szBrowser, "http://www.trialpay.com/productpage/?c=014cd22&tid=9ahOL99", "", SW_SHOW );
			}
		}
	}
	else
	{
		// flag BUILDGAME has selected
		theApp.SetFileMapData ( 758, 1 );
	}
}

void CMainFrame::OnFilePreferences() 
{
	// flag PREFS DIALOG
	theApp.SetFileMapData ( 754, 1 );
}

void CMainFrame::OnLevelResizelevel() 
{
	// TODO: Add your command handler code here
	m_LevelResize.DoModal ( );	
}

void CMainFrame::OnClose() 
{

	// TODO: Add your message handler code here and/or call default

	theApp.SetFileMapData ( 908, 1 );
	theApp.m_Debug.Write ( "CMainFrame::OnClose - 908 = 1" );
	
	//CMDIFrameWnd::OnClose();
}




void CMainFrame::OnEditingSegmentmode() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 1 );
}


void CMainFrame::OnEditingEntitymode() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 2 );
}

void CMainFrame::OnEditingClipboardedit() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 3 );
}

void CMainFrame::OnEditingZoomin() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 4 );
}

void CMainFrame::OnEditingZoomout() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 5 );
}


void CMainFrame::OnEditingMoveupalayer() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 6 );
}

void CMainFrame::OnEditingMovedownalayer() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 7 );
}

void CMainFrame::OnEditingViewentirelevel() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 1 );
	theApp.SetFileMapData ( 716, 8 );
}

void CMainFrame::OnSegmentsPaintallofsegments() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 2 );
	theApp.SetFileMapData ( 716, 1 );
}




void CMainFrame::OnSegmentsExternaldrawmode() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 2 );
	theApp.SetFileMapData ( 716, 5 );
}

void CMainFrame::OnSegmentsInternaldrawmode() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 2 );
	theApp.SetFileMapData ( 716, 6 );
}


void CMainFrame::OnSegmentsPaintonlyfloors() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 2 );
	theApp.SetFileMapData ( 716, 3 );
}

void CMainFrame::OnSegmentsPaintonlywalls() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 2 );
	theApp.SetFileMapData ( 716, 2 );
}

void CMainFrame::OnSegmentsPicksegmentfromgrid() 
{
	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 712, 2 );
	theApp.SetFileMapData ( 716, 4 );
}


WPARAM  wParam1 = 0;


BOOL CALLBACK EnumWindowsProc1(HWND hwnd,LPARAM lParam )
{
	
	TCHAR szBuffer  [ 256 ];
	
	GetWindowText ( hwnd, szBuffer, 256 );

	// 100105
	if ( theApp.GetFileMapData ( 908 ) == 1 )
		return FALSE;

	_strlwr ( szBuffer );
	if ( strcmp ( szBuffer, _T ( "fpsc map editor" ) ) == 0 )
	{
		LPARAM  lParam = 0;
		lParam = ( LPARAM ) hwnd;
//		SendMessage ( hwnd, WM_ACTIVATE, wParam1, lParam ); - avoid as mapeditor could be WAITING (sync system)
	}

	return TRUE;
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	// 080205 - winme comment out test
	//return;

	//if ( theApp.m_bDebugActivation )
	//	return;

	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if ( theApp.GetFileMapData ( 424 ) == 1 )
		return;
	
	if ( theApp.GetFileMapData ( 750 ) == 1 )
		return;

	if ( theApp.m_bPopupDisplay )
		return;

	
	
	if ( theApp.m_bBrowserDisplay )
	{
		theApp.m_bAppActive = true;

		//theApp.m_Debug.Write ( "OnActivate - active" );
		wParam1 = WA_ACTIVE;

		if ( theApp.m_pEditorView )
			theApp.m_pEditorView->SetTimer( 0, 1, NULL );

		EnumWindows ( EnumWindowsProc1, 0 );

		return;
	}

	// 110105
	if ( theApp.m_bBuildGameDisplay )
	{
		return;

		theApp.m_bAppActive = true;

		//theApp.m_Debug.Write ( "OnActivate - active" );
		wParam1 = WA_ACTIVE;

		if ( theApp.m_pEditorView )
			theApp.m_pEditorView->SetTimer( 0, 1, NULL );

		EnumWindows ( EnumWindowsProc1, 0 );

		return;
	}

	if ( theApp.m_bBuildGameDisplay )
	{
		return;

		theApp.m_bAppActive = true;

		//theApp.m_Debug.Write ( "OnActivate - active" );
		wParam1 = WA_ACTIVE;

		if ( theApp.m_pEditorView )
			theApp.m_pEditorView->SetTimer( 0, 1, NULL );

		EnumWindows ( EnumWindowsProc1, 0 );

		return;
	}
	

	if ( pWndOther )
	{
		char szPath [ 255 ] = "";
		char szFinal [ 255 ] = "";

		CString szTitle;
		pWndOther->GetWindowText ( szTitle );

		ConvertWideToANSI ( NULL, &szTitle, szPath );

		sprintf ( szFinal, "WINDOW = %s", szPath );

		//theApp.m_Debug.Write ( szFinal );
	}

	if ( nState == WA_INACTIVE )
	{
		if ( theApp.m_bDebugActivation )
			return;

		theApp.m_bAppActive = false;

		//theApp.m_Debug.Write ( "OnActivate - deactive" );
		wParam1 = WA_INACTIVE;

		if ( theApp.m_pEditorView )
			theApp.m_pEditorView->KillTimer ( 0 );

		EnumWindows ( EnumWindowsProc1, 0 );
	}
	else
	{
		theApp.m_bAppActive = true;

		//theApp.m_Debug.Write ( "OnActivate - active" );
		wParam1 = WA_ACTIVE;

		if ( theApp.m_pEditorView )
			theApp.m_pEditorView->SetTimer( 0, 1, NULL );

		EnumWindows ( EnumWindowsProc1, 0 );
	}
}

void CMainFrame::OnEntityView() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 7 );
}

void CMainFrame::OnSegmentView() 
{
	theApp.SetFileMapData ( 60, 1 );

	// TODO: Add your command handler code here
	theApp.SetFileMapData ( 200, 4 );
	theApp.SetFileMapData ( 204, 8 );
}

void CMainFrame::OnUpdateSegmentView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnUpdateEntityView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( theApp.m_bDisable );
}

void CMainFrame::OnAppCredits() 
{
	// 270105 - force redraw
	theApp.m_bBuildGameDisplay = true;
	cCredits credits;
	credits.DoModal ( );
	theApp.m_bBuildGameDisplay = false;
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIFrameWnd::OnSetFocus(pOldWnd);

}

void CMainFrame::OnKillFocus(CWnd* pNewWnd) 
{
	CMDIFrameWnd::OnKillFocus(pNewWnd);
}

/*
void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	if ( bActive == FALSE )
	{
		theApp.m_Debug.Write ( "OnActivateApp - deactive" );

		// 221204
		wParam1 = WA_INACTIVE;
		EnumWindows ( EnumWindowsProc1, 0 );

	}
	else
	{
		theApp.m_Debug.Write ( "OnActivateApp - active" );

		// 221204
		 //wParam1 = WA_ACTIVE;
		//EnumWindows ( EnumWindowsProc1, 0 );

	}

	CMDIFrameWnd::OnActivateApp(bActive, hTask);	
}
*/

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class

	//theApp.m_Debug.Write ( "ActivateFrame" );

	//wParam1 = WA_ACTIVE;
	//	EnumWindows ( EnumWindowsProc1, 0 );

	//theApp.m_Debug.Write ( "ActivateFrame" );
	
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	
}

void CMainFrame::OnEnterIdle( UINT, CWnd* )
{
	int lee=42;
}


//HELPW - create a new help menu option and point it to this function
void CMainFrame::OnHelpwizard()
{
	if ( theApp.m_pHelpWizardDlg )
	{
		if ( theApp.m_bHelpWizardVisible ) 
		{
			//dialog created an open
			theApp.m_bHelpWizardVisible = true;
			theApp.m_pHelpWizardDlg->ShowWindow( SW_RESTORE );
		}
		else 
		{
			//dialog created but closed, restart thread
			theApp.m_bHelpWizardVisible = true;
			theApp.m_pHelpWizardDlg->Start( );
		}
	}
	else
	{
		//dialog not yet created (true if not opened on start-up)
		theApp.m_bHelpWizardVisible = true;
		theApp.m_pHelpWizardDlg = new CHelpWizard();
		theApp.m_pHelpWizardDlg->Start( );
	}
}
//HELPW - end

//AutoUpdate - create a new help menu option and point it to this function
void CMainFrame::OnHelpCheckForUpdates()
{
	CAutoUpdate cAutoUpdateDlg;
	cAutoUpdateDlg.DoModal( );
}

void CMainFrame::OnHelpReadManual()
{
	CString sFullPath = theApp.m_szDirectory;

	// V110 - 110608 - add FREE if the free version
	if ( g_bFreeVersion )
		sFullPath += "\\Docs\\FPS Creator Manual (Free).pdf";
	else
		sFullPath += "\\Docs\\FPS Creator Manual.pdf";

	ShellExecute( NULL, "open", sFullPath.GetString(), NULL, NULL, SW_SHOW );
}

//AutoUpdate
void CMainFrame::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		if ( !theApp.m_pUpdateThread ) KillTimer( 1 );

		if ( theApp.m_pUpdateThread->CheckFinished( ) )
		{
			KillTimer( 1 );

			//current version
			char szCurrentVersion [ 64 ];
			strcpy_s( szCurrentVersion, 64, "n/a" );

			FILE *pFile = NULL;
			int error = fopen_s( &pFile, "version.ini", "rb" );
			if ( !error )
			{
				fgets( szCurrentVersion, 64, pFile );
				fclose( pFile );
				pFile = NULL;
			}

			const char *szLatestVersion = theApp.m_pUpdateThread->GetLatestVersion( );

			if ( strcmp( szLatestVersion, "n/a" ) != 0 && strcmp( szCurrentVersion, szLatestVersion ) < 0 )
			{
				int result = MessageBox( "A newer version of FPS Creator is available to download, open the update window?", "Update", MB_YESNO );
				if ( result == IDYES )
				{
					CAutoUpdate cAutoUpdateDlg;
					cAutoUpdateDlg.DoModal( );
				}
			}

			delete theApp.m_pUpdateThread;
			theApp.m_pUpdateThread = NULL;
		}
	}
}

void CMainFrame::OnTgcStoreHelpClicked()
{
	// V110 BETA4 - 070608 - launch Game Creator Store module
	LaunchGameCreatorStore();
}

void CMainFrame::OnHelpResetChecks()
{
	theApp.DeleteRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "first_run" );
	theApp.DeleteRegistryValue( "Software\\FPSCreator\\Editor\\Settings", "check_x9_updates_on_startup" );
	theApp.m_bFirstRun = true;
}

void CMainFrame::OnHelpPlayintroagain()
{
	if ( theApp.m_pHelpWizardDlg )
	{
		//m_pHelpWizardDlg->Terminate();
		if ( theApp.m_bHelpWizardVisible ) return;

		delete theApp.m_pHelpWizardDlg;
		theApp.m_pHelpWizardDlg = NULL;
	}

	theApp.m_bHelpWizardVisible = true;
	theApp.m_pHelpWizardDlg = new CHelpWizard();
	theApp.m_pHelpWizardDlg->Start( );
}

void CMainFrame::OnHelpViewitemlicenses()
{
	CString sPath = theApp.m_szDirectory;
	sPath += "\\Files\\TGCStore\\licenses";

	if ( !SetCurrentDirectory( sPath ) )
	{
		// leefix - 210508 - missed out space : MessageBox( GetLanguageData( "TGCStore", "NoLicense" ) );
		MessageBox( GetLanguageData( "TGC Store", "NoLicense" ) );
	}
	else 
	{
		ShellExecute( NULL, "open", "explorer.exe", sPath, NULL, SW_SHOWNORMAL );
	}
}

void CMainFrame::OnHelpWatchstorevideo()
{
	char szBrowser [ MAX_PATH ];
	theApp.GetDefaultBrowserPath( szBrowser, MAX_PATH );
	ShellExecute ( NULL, "open", szBrowser, "http://www.youtube.com/watch?v=PgWOlP52c-E", "", SW_SHOW );
}
