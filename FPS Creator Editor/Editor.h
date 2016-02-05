// Editor.h : main header file for the EDITOR application
//

#if !defined(AFX_EDITOR_H__9200D31C_755C_4645_B4AB_2AB08F00AB12__INCLUDED_)
#define AFX_EDITOR_H__9200D31C_755C_4645_B4AB_2AB08F00AB12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "MainFrm.h"
#include "ChildFrm.h"
#include "EditorDoc.h"
#include "EditorView.h"

#include "Splash.h"
#include "resource.h"       // main symbols
#include "about.h"
#include <wininet.h>
#include "HashTree.h"

// External to global to control interface use
extern bool		g_bStopInterfaceOperations;


/////////////////////////////////////////////////////////////////////////////
// CEditorApp:
// See Editor.cpp for the implementation of this class
//

//#define WM_USER_THREAD_FINISHED_TEST WM_USER+0x100

//#define WM_MIKE WM_USER+0x101

#define WM_MYMESSAGE (WM_USER + 1)
#define WM_ENTITY_WINDOW (WM_USER + 2)
#define WM_CLOSE_LEVEL_WINDOW (WM_USER + 3)

#include "cDebug.h"

#include "cMessages.h"

#include "IPC.h"

class CHelpWizard;	//HELPW - for pointer def
class UpdateCheckThread;	//AutoUpdate

typedef void	( *CreateFileMapPFN		) ( char*, DWORD );
typedef void	( *DestroyFileMapPFN	) ( char* );
typedef BYTE*	( *GetFileMapDataPFN	) ( char* );

class CEditorApp : public CWinApp, public CBCGWorkspace, cBaseMessage
{
	public:

		CHelpWizard* m_pHelpWizardDlg;	//HELPW - the help wizard dialog
		bool		 m_bHelpWizardVisible;

		HashTree m_TreeGroups;

		UpdateCheckThread *m_pUpdateThread;

		HANDLE m_hFileMap;
		LPVOID m_lpVoid;
		BYTE*  m_pData;
		HANDLE m_hOnlyOneEditorMutex;

		CEditorDoc* m_pDocumentA;
		bool		m_bFocus;

		bool		m_bEntityFocus;

		bool		m_bRestart;

		bool m_bAppActive;

		CEditorView* m_pEditorView;

		CString	m_szProjectName;

		HINSTANCE m_hInstance;

		cDebug		m_Debug;

		POINT m_lastPoint;

		bool			m_bPopupDisplay;
		bool			m_bBuildGameDisplay;
		bool			m_bBrowserDisplay;	//	310105	http://www.fpscreator.com/bugtraq/index.php?do=details&id=310

		bool				m_bSplashScreen;

		int					m_iRecover;
		DWORD				m_dwRecoverTimer;
		bool				m_bGameOrBuildInProgress;
		DWORD				m_dwCheckProcessesTimer;
		bool				m_bAppHasStartedRunning;
		bool				m_bDoNotNeedToCheckForCrashesAnymore;

		bool				m_DBProApplicationLaunched;
		SHELLEXECUTEINFO	m_DBProApplication;

		// 020205 - add for process
		PROCESS_INFORMATION	m_DBProProcess;
		
		// 110105
		bool				m_bTestGameCancel;

		//HINSTANCE			m_hEnhancementsDLL;
		//CreateFileMapPFN	m_pfnCreateFileMap;
		//DestroyFileMapPFN	m_pfnDestroyFileMap;
		//GetFileMapDataPFN	m_pfnGetFileMapData;
		
		//CDBProMessages m_DBPro;

		//cIPC*				m_pIPC;

//#ifdef TGC_STORE
		bool m_bSavePassword;
		bool m_bSaveUsername;

		CString m_sUsername;
		CString m_sPassword;
		char *m_szSESID;
		char *m_szCredits;
		char m_szOrderBy [ 32 ];
		char m_szAuthorName [ 30 ];
		char m_szVersion [ 10 ];
		bool m_bAdmin;
		bool m_bUploader;
		bool m_bAgreedTerms;
		bool m_bGotoAddons;
		HINTERNET m_hInet;

		int m_iAppX;
		int m_iAppY;
		int m_iAppWidth;
		int m_iAppHeight;

		bool m_bFirstRun;
		CRITICAL_SECTION m_csDirectoryChanges;
//#endif

		void SetRegistryValue( const char *szLocation, const char *szKey, const char *szValue );
		void SetRegistryValue( const char *szLocation, const char *szKey, DWORD dwValue );
		bool GetRegistryValue( const char *szLocation, const char *szKey, char *szValue, DWORD dwSize, HKEY hRoot = HKEY_CURRENT_USER );
		bool GetRegistryValue( const char *szLocation, const char *szKey, DWORD *dwValue );
		bool DeleteRegistryValue( const char *szLocation, const char *szKey );

		char* Encrypt( const char *szString );
		char* Decrypt( const char *szString );

		bool GetDefaultBrowserPath( char *szValue, DWORD dwSize );

		void UpdateMouse           ( void );
		void UpdateToolBar         ( void );
		void UpdateStatusBar       ( void );
		void UpdateLibraryWindow   ( void );
		void UpdateBrowserWindow   ( void );
		void UpdateFileOpenAndSave ( void );
		void UpdateMessageBoxes    ( void );
		void UpdatePopup           ( void );
		void UpdateTermination     ( void );
		void UpdateRestart         ( void );
		void UpdateEntityWindow    ( void );
		void UpdateBuildGame       ( void );
		void UpdatePreferences     ( void );
		void UpdateTestGame        ( void );
		void UpdateFPIWizard       ( void );
		void UpdateProject         ( void );
		void UpdateQuit			   ( void );

		void CheckForDBProApplications ( void );

		bool IsProcessRunning ( char* szProcess );

		void LoadLanguage ( void );
		void LoadSettings ( void );

		bool CheckForMultipleInstances ( void );

		void	HandleMessages ( void );
		cMessages			m_Message;

		// V110 - 110608 - added serial code validation check to m_App (so store & cFront can access it)
		bool SerialCodeCheck ( LPSTR pSerialCodeStringToCheck );
		bool IsSerialCodeValid ( LPSTR pTryCodeOrNULL );
		void CheckSerialCodeValidity ( void );
		DWORD GetTodaysDays ( void );

	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		CEditorApp();
		~CEditorApp();

		void ClearTGCStoreCache( );

		// Override from CBCGWorkspace
		virtual void PreLoadState ();

		void OnFileOpen();
		void OnFileNewType( int );
		void OnFileNew();
		void OnFileNewArena();

		CMainFrame* pMainFrame;
		
		bool	m_bAppRunning;
		bool	m_bSetupFrame;
		bool	m_bInvalidate;

		bool	m_bDebugTrace;
		bool	m_bDebugProcess;
		bool	m_bDebugActivation;

		BOOL		m_bEnd;
		BOOL	m_bDisplayMouse;
		BOOL	m_bDisplayMouseInQuickPreview;

		bool	m_bDisable;

		BOOL			m_bTest [ 10 ];

		TCHAR m_szLanguage [ MAX_PATH ];
		TCHAR m_szLanguageName [ 40 ];
		TCHAR m_szLanguagePath [ MAX_PATH ];
		TCHAR m_szDirectory [ MAX_PATH ];
		TCHAR m_szErrorString [ 64 ];

		HANDLE m_hThread;

		bool m_bFileNewValid;
		void ClearFileMap ( void );
		void SetFileMapData ( int iOffset, DWORD dwData );
		void SetFileMapDataString ( int iOffset, LPTSTR pString );
		DWORD GetFileMapData ( int iOffset );

		static UINT CheckMemoryStatus ( LPVOID pParameter );
		void CheckMemoryStatus ( void );

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEditorApp)
	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();
		virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	// Implementation
		//{{AFX_MSG(CEditorApp)
		afx_msg void OnAppAbout();
	afx_msg void OnAppExit1();
	afx_msg BOOL OnOpenRecentFile(UINT nID);
	afx_msg void OnEditCopy1();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditCut1();
	afx_msg void OnEditPaste();
	afx_msg void OnEditPaste1();
	afx_msg void OnEditRedo();
	afx_msg void OnEditRedo1();
	afx_msg void OnEditUndo();
	afx_msg void OnEditUndo1();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildGame(CCmdUI* pCmdUI);
	afx_msg void OnHelpWebsite();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNewArena(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAsEx(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePreferences(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile7(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile8(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile9(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile11(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile12(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile13(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile14(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMruFile15(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPrefab(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDraw(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewWaypoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTestGame(CCmdUI* pCmdUI);

	afx_msg void OnTimer( UINT_PTR nIDEvent );	//AutoUpdate
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

			
};


extern CEditorApp theApp;

void ConvertWideToANSI ( BYTE* pData, CString* pUNI, char* pANSI );
void ConvertWideToANSI ( BYTE* pData, CString* pUNI );

void ConvertANSIToWide ( BYTE* pData, char* pANSI, CString* pUNI );

void ConvertANSIToWide ( char* pANSI, CString* pUNI );
CString GetLanguageData ( CString group, CString key );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITOR_H__9200D31C_755C_4645_B4AB_2AB08F00AB12__INCLUDED_)
