// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__49BF3799_3334_425E_BE41_095AEDFB84CD__INCLUDED_)
#define AFX_MAINFRM_H__49BF3799_3334_425E_BE41_095AEDFB84CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WorkspaceBar.h"
#include "OutputBar.h"
#include "cLevelResize.h"
#include "cPreferences.h"

#define CMDIFrameWnd CBCGMDIFrameWnd



class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

	
	void OnMouseMoveXX(UINT nFlags, CPoint point);
	void SetWorkspaceVisible ( bool bVisible );
	void SetEntityVisible ( bool bVisible );
	void AddBitmapToWorkspace ( int iTab, LPTSTR szBitmap, LPTSTR szName );
	void RemoveBitmapFromWorkspace ( int iTab, int iIndex );

	void SetToolbarButtonState ( int iToolbar, int iButton, int iState );

	void SetStatusBarText ( int iIndex, LPTSTR pszText );

	void ShowEntityWindow ( bool bVisible );

	TCHAR	m_szStatus [ 5 ] [ 255 ];

	COutputBar		m_wndOutput;
	CBCGMenuBar		m_wndMenuBar;

	void Quit ( void );
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual void OnEnterIdle( UINT, CWnd* );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	//CBCGMenuBar		m_wndMenuBar;
	CBCGStatusBar	m_wndStatusBar;

	CBCGToolBar		m_wndToolBar;
	CBCGToolBar		m_wndToolBarView;
	CBCGToolBar		m_wndToolBarMode;
	CBCGToolBar		m_wndToolBarProperty;
	CBCGToolBar		m_wndToolBarDraw;
	CBCGToolBar		m_wndToolBarSelection;
	CBCGToolBar		m_wndToolBarSegment;
	CBCGToolBar		m_wndToolBarEntity;
	CBCGToolBar		m_wndToolBarWaypoint;
	CBCGToolBar		m_wndToolBarGame;

	CBCGToolBar*	m_pToolBarList [ 10 ];

	
	CWorkspaceBar	m_wndWorkSpace;
	//COutputBar		m_wndOutput;

	CReBar			m_wndReBar;
	CDialogBar      m_wndDlgBar;

	CCmdUI*			m_pTest;
	BOOL			m_bTest [ 11 ] [ 11 ];

	CBCGToolBarImages	m_UserImages;

	cLevelResize	m_LevelResize;
	cPreferences	m_Preferences;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void LaunchGameCreatorStore( bool bGotoAddons = false );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewIncreaseShroud();
	afx_msg void OnViewDecreaseShroud();
	afx_msg void OnViewLayers();
	afx_msg void OnViewMoveUpLayer();
	afx_msg void OnViewMoveDownLayer();
	afx_msg void OnViewOverview();
	afx_msg void OnViewCloseup();
	afx_msg void OnModePaint();
	afx_msg void OnModeSelect();
	afx_msg void OnModeArt();
	afx_msg void OnModeEntity();
	afx_msg void OnModeWaypoint();
	afx_msg void OnPropertyRotate();
	afx_msg void OnPropertyMirror();
	afx_msg void OnPropertyFlip();
	afx_msg void OnDrawA();
	afx_msg void OnDrawB();
	afx_msg void OnDrawC();
	afx_msg void OnDrawD();
	afx_msg void OnDrawStateNone();
	afx_msg void OnDrawPick();
	afx_msg void OnSelectionCut();
	afx_msg void OnSelectionCopy();
	afx_msg void OnSelectionPaste();
	afx_msg void OnSelectionClear();
	afx_msg void OnSelectionClearNopaste();
	afx_msg void OnSelectionReplace();
	afx_msg void OnSegmentLine();
	afx_msg void OnSegmentBox();
	afx_msg void OnSegmentCircle();
	afx_msg void OnSegmentDecrease();
	afx_msg void OnSegmentIncrease();
	afx_msg void OnSegmentSpray();
	afx_msg void OnEntityDelete();
	afx_msg void OnEntityRotateX();
	afx_msg void OnEntityRotateY();
	afx_msg void OnEntityRotateZ();
	afx_msg void OnWaypointCreate();
	afx_msg void OnViewRotate();
	afx_msg void OnViewPrefab();
	afx_msg void OnViewSelection();
	afx_msg void OnViewDraw();
	afx_msg void OnViewRotateEntity();
	afx_msg void OnViewWaypoint();
	afx_msg void OnViewTestGame();
	afx_msg void OnTestGame();
	afx_msg void OnUpdateTestMap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTestGame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewIncreaseShroud(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDecreaseShroud(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLayers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMoveupLayer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMovedownLayer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOverview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewCloseUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModePaint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModeSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModeArt(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModeEntity(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModeWaypoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModeWaypointSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertyRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertyMirror(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertyFlip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawA(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawB(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawC(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawD(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawPick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawStateNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionClearNoPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionReplace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentBox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentDecrease(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentIncrease(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSegmentSpray(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityRotateX(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityRotateY(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityRotateZ(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWaypointCreate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLevelResizelevel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePreferences(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewRotateEntity(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCopy1();
	afx_msg void OnEditCut1();
	afx_msg void OnEditPaste1();
	afx_msg void OnEditRedo1();
	afx_msg void OnEditUndo1();
	afx_msg void OnHelpIndex();
	afx_msg void OnWaypointSelect();
	afx_msg void OnTestMap();
	afx_msg void OnFileBuildgame();
	afx_msg void OnFilePreferences();
	afx_msg void OnLevelResizelevel();
	afx_msg void OnClose();
	afx_msg void OnSegmentsExternaldrawmode();
	afx_msg void OnSegmentsInternaldrawmode();
	afx_msg void OnSegmentsPaintallofsegments();
	afx_msg void OnSegmentsPaintonlyfloors();
	afx_msg void OnSegmentsPaintonlywalls();
	afx_msg void OnSegmentsPicksegmentfromgrid();
	afx_msg void OnEditingClipboardedit();
	afx_msg void OnEditingEntitymode();
	afx_msg void OnEditingMovedownalayer();
	afx_msg void OnEditingMoveupalayer();
	afx_msg void OnEditingSegmentmode();
	afx_msg void OnEditingViewentirelevel();
	afx_msg void OnEditingZoomin();
	afx_msg void OnEditingZoomout();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEntityView();
	afx_msg void OnSegmentView();
	afx_msg void OnUpdateSegmentView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntityView(CCmdUI* pCmdUI);
	afx_msg void OnAppCredits();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewWorkspace();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnViewView();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPrefab(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSelection(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDraw(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewEntity(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewWaypoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTestGame(CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg LRESULT OnThreadFinishedTest (WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnUpdateEntityWindow (WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCloseLevelWindow (WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CBCGPopupMenu* pMenuPopup);

	

public:
	afx_msg void OnHelpwizard();	//HELPW
	afx_msg void OnHelpCheckForUpdates();
	afx_msg void OnHelpReadManual();

	afx_msg void OnTimer( UINT_PTR nIDEvent );	//AutoUpdate
public:
	afx_msg void OnTgcStoreHelpClicked();
public:
	afx_msg void OnHelpResetChecks();
public:
	afx_msg void OnHelpPlayintroagain();
public:
	afx_msg void OnHelpViewitemlicenses();
public:
	afx_msg void OnHelpWatchstorevideo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__49BF3799_3334_425E_BE41_095AEDFB84CD__INCLUDED_)
