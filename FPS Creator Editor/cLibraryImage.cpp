// cLibraryImage.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cLibraryImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cLibraryImage::cLibraryImage ( )
{
}

cLibraryImage::~cLibraryImage ( )
{
}


BEGIN_MESSAGE_MAP ( cLibraryImage, CListCtrl )
	//{{AFX_MSG_MAP(cLibraryImage)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLLCLIPBOARD()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ( )

int cLibraryImage::Start ( void )
{
	m_Images.Create     ( 64, 64, ILC_COLORDDB, 1, 16 );
	m_Images.SetBkColor ( RGB ( 255, 255, 255 ) );
		
	this->DeleteAllItems ( );
	this->SetImageList   ( &m_Images, LVSIL_NORMAL );
	this->SetItemCount   ( 64 );

	return 1;
}

#include "cDialogEnd.h"
extern int g_iBoysLifeQuit;

void cLibraryImage::OnClickList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	// retrieve message info.
	LPNMITEMACTIVATE pItemAct = ( LPNMITEMACTIVATE ) pNMHDR;

	// determine which item receives the click
	LVHITTESTINFO  hitTest;

	// determine item clicked on
	memset ( &hitTest, '\0', sizeof ( LVHITTESTINFO ) );
	hitTest.pt = pItemAct->ptAction;
	this->SendMessage ( LVM_SUBITEMHITTEST, 0, ( LPARAM ) &hitTest );

	// reset strings
	theApp.SetFileMapDataString ( 1512, "" );
	theApp.SetFileMapDataString ( 1256, "" );

	// fill strings if item valid
	if ( hitTest.iItem > -1 )
	{
		// get file only
		bool bItemInvalid=false;
		int iLength = strlen( m_szImageFileNames [ hitTest.iItem ] );
		for ( int n=iLength; n>0; n-- )
		{
			if ( m_szImageFileNames [ hitTest.iItem ][n]=='\\' || m_szImageFileNames [ hitTest.iItem ][n]=='/' )
			{
				// set invalid if underscore here
				if ( m_szImageFileNames [ hitTest.iItem ][n+1]=='_' ) bItemInvalid=true;
				break;
			}
		}

		
		#ifdef FPSC_DEMO
		if ( bItemInvalid )
		{
				cDialogEnd end;

				g_iBoysLifeQuit = 1;
				end.DoModal ( );
				g_iBoysLifeQuit = 0;
		}
				#endif

		// if string starts with _ underscore, it is a NULL item and should not fill strings
		if ( bItemInvalid==false )
		{
			
			
			theApp.SetFileMapDataString ( 1512, m_szImageFileNames [ hitTest.iItem ] );
			theApp.SetFileMapDataString ( 1256, m_szImageExtra [ hitTest.iItem ] );

		}
	}
}

extern int m_iScroll;

struct sFileList
{
	char	szFullPath	  [ MAX_PATH ];
	char	szDisplayName [ MAX_PATH ];
	char	szFileName    [ MAX_PATH ];
	char	szImageName   [ MAX_PATH ];
	char	szExtension   [ 4 ];
};


void cLibraryImage::LoadImages ( CString sPath )
{
	theApp.m_Debug.Write ( "\ncLibraryImage::LoadImages - Start" );

	m_iImageCount = 0;

	memset ( m_szImageFileNames, 0, sizeof ( m_szImageFileNames ) );

	TCHAR szDirectory [ MAX_PATH ];
	GetCurrentDirectory ( MAX_PATH, szDirectory );

	char szFullPath [ MAX_PATH ];

	int iLen = strlen ( sPath );
	strcpy ( szFullPath, sPath );

	if ( szFullPath [ iLen - 1 ] == '\\' && szFullPath [ iLen - 2 ] == '\\' )
		szFullPath [ iLen - 1 ] = 0;

	SetCurrentDirectory ( sPath );

	char szOut [ 256 ] = "";
	sprintf ( szOut, "cLibraryImage::LoadImages - directory - %s", sPath );
	theApp.m_Debug.Write ( szOut );

	CString string = "FPSEXCHANGE";
	
	char szBlankImage [ 255 ];
	char szTokens [ 10 ] [ 255 ];
	int  iToken = 0;
	bool bAddAllFiles = false;

	{
		{
			/*
			HANDLE m_hFileMap;
			LPVOID m_lpVoid;
			BYTE*  m_pData;

			m_hFileMap = OpenFileMapping ( FILE_MAP_ALL_ACCESS, TRUE, string );
			m_lpVoid   = MapViewOfFile ( m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0 );

			m_pData    = ( BYTE* ) m_lpVoid;
			*/

			//if ( m_lpVoid )
			{
				//if ( m_pData )
				{
					cIPC* pIPC			= theApp.m_Message.GetIPC ( );
	
					

					char szFilter [ 255 ];
					
					memset ( szFilter,     0, sizeof ( szFilter     ) );
					memset ( szTokens,     0, sizeof ( szTokens     ) );
					memset ( szBlankImage, 0, sizeof ( szBlankImage ) );

					//memcpy ( &szFilter [ 0 ], &theApp.m_pData [ 1768 ], sizeof ( char ) * 255 );
					//memcpy ( &szBlankImage [ 0 ], &theApp.m_pData [ 2024 ], sizeof ( char ) * 255 );

					pIPC->ReceiveBuffer ( &szFilter     [ 0 ], 1768, sizeof ( char ) * 255 );
					pIPC->ReceiveBuffer ( &szBlankImage [ 0 ], 2024, sizeof ( char ) * 255 );

					

					if ( strcmp ( szFilter, "*.*" ) == 0 )
						bAddAllFiles = true;

					if ( !bAddAllFiles )
					{
						char  seps[ ] = ",";
						char* token;

						token = strtok ( szFilter, seps );

						while ( token != NULL )
						{
							strcpy ( szTokens [ iToken++ ], token );

							{
								char szOut [ 256 ] = "";
								sprintf ( szOut, "cLibraryImage::LoadImages - token - %s", token );
								theApp.m_Debug.Write ( szOut );
							}

							token = strtok ( NULL, seps );
						}
					}
				}
			}

			//UnmapViewOfFile ( m_lpVoid );
			//CloseHandle     ( m_hFileMap );
		}
	}

	
	//sFileList	FileList [ 100 ];
	//memset ( FileList, 0, sizeof ( FileList ) );
	//int			iFileListCount = 0;

	vector < sFileList > FileList;

	

	{
		CString	strPath, strPattern;
		CFile	ImgFile;	
		BYTE	data[8] = {'\0'};	
		BOOL	bRC = TRUE;
		
		HANDLE					hFind = NULL;
		WIN32_FIND_DATA			FindFileData;
		CString					FileNames [ 256 ];
		int iIndex = 0;
		
		strPattern = sPath + "\\*.*";
		hFind = ::FindFirstFile(strPattern, &FindFileData);

		while(bRC)
		{
			bRC = ::FindNextFile(hFind, &FindFileData);
			if(bRC)
			{
				////////
				sFileList file;
				////////

				// filter off the system files and directories
				if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  &&
				   !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)     &&
				   !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)     &&
				   !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY))
				{
					// open the file
					strPath = sPath + "\\" + FindFileData.cFileName;

					/*
					// 170105
					char szFile [ MAX_PATH ];
					int  iLen     = wcslen ( FindFileData.cFileName );
					int  nResult = WideCharToMultiByte ( CP_ACP, 0, FindFileData.cFileName, -1, szFile, iLen * sizeof ( CHAR ), NULL, NULL );
					szFile [ iLen ] = 0;
					char szExt [ 4 ];
					*/

					char szFile [ MAX_PATH ];
					char szExt [ 4 ];
					strcpy ( szFile, FindFileData.cFileName );
					memset ( szExt, 0, sizeof ( szExt ) );

					bool bAdd = false;
					if ( strlen(szFile) > 4 )
					{
						szExt [ 0 ] = szFile [ strlen ( szFile ) - 3 ];
						szExt [ 1 ] = szFile [ strlen ( szFile ) - 2 ];
						szExt [ 2 ] = szFile [ strlen ( szFile ) - 1 ];
					}

					////////
					memset ( file.szDisplayName, 0, sizeof ( file.szDisplayName ) );
					if ( strlen(szFile) > 4 )
						memcpy ( file.szDisplayName, szFile, sizeof ( char ) * ( strlen ( szFile ) - 4 ) );
					else
						strcpy ( file.szDisplayName, szFile );

					strcpy ( file.szFullPath, szFullPath );
					////////

					if ( bAddAllFiles )
					{
						bAdd = true;
					}
					else
					{
						for ( int i = 0; i < iToken; i++ )
						{
							if ( strcmp ( szExt, szTokens [ i ] ) == 0 )
							{
								bAdd = true;
							}
						}
					}

					if ( bAdd )
					{
						//////////
						strcpy ( file.szFileName,  szFile );
						strcpy ( file.szExtension, szExt );
						//////////

						if ( strcmp ( szExt, "bmp" ) == 0 )
						{
							//strcpy ( FileList [ iFileListCount ].szImageName, szFile );

							////////
							strcpy ( file.szImageName, szFile );
							////////
						}
						else
						{
							////////
							strcpy ( file.szImageName, szBlankImage );
							strcpy ( file.szImageName,file.szDisplayName );
							strcat ( file.szImageName, ".bmp" );
							////////

							FILE* fp = NULL;

							//fp = fopen ( FileList [ iFileListCount ].szImageName, "rb" );

							///////
							fp = fopen ( file.szImageName, "rb" );
							///////

							if ( !fp )
							{
								//strcpy ( FileList [ iFileListCount ].szImageName, szBlankImage );

								////////
								strcpy ( file.szImageName, szBlankImage );
								////////
							}

							if ( fp )
								fclose ( fp );
						}

						//strcat ( FileList [ iFileListCount ].szFullPath, FileList [ iFileListCount ].szFileName );

						///////
						strcat ( file.szFullPath, file.szFileName );

						
						// 210205 - loc file change
						{
							char szOriginalPath [ MAX_PATH ] = "";

							GetCurrentDirectory ( MAX_PATH, szOriginalPath );
							SetCurrentDirectory ( theApp.m_szDirectory );

							// get the corresponding loc file name
							char szLOCFile [ MAX_PATH ] = "";
							char szLOCFull [ MAX_PATH ] = "";

							for ( int i = 0; i < (int)strlen ( file.szFileName ); i++ )
							{
								if ( file.szFileName [ i ] == '.' )
									break;
								
								szLOCFile [ i ] = file.szFileName [ i ];
							}

							
							strcat ( szLOCFile, ".loc" );

							// get the corresponding loc file path
							char szItem [ 255 ] [ MAX_PATH ];
							char  seps[ ] = "\\/";
							char* token;
							int iToken = 0;
							int iFileIndex = 0;

							char szPath [ MAX_PATH ] = "";
							strcpy ( szPath, file.szFullPath );

							//token = strtok ( file.szFullPath, seps );
							token = strtok ( szPath, seps );

							
							while ( token != NULL )
							{
								_strlwr ( token );

								if ( strcmp ( token, "files" ) == 0 )
									iFileIndex = iToken;

								strcpy ( &szItem [ iToken++ ] [ 0 ], token );
								token = strtok ( NULL, seps );
							}

							strcpy ( szLOCFull, theApp.m_szLanguagePath );
							strcat ( szLOCFull, "/textfiles/library/" );

							for ( i = iFileIndex + 1; i < iToken - 1; i++ )
							{
								strcat ( szLOCFull, szItem [ i ] );
								strcat ( szLOCFull, "/" );
							}

							strcat ( szLOCFull, szLOCFile );

							// now open this file and extract the text
							FILE* fp = fopen ( szLOCFull, "rt" );

							if ( fp )
							{
								char szData [ MAX_PATH ] = "";
								int iOffset = 0;
								char c;

								while ( !feof ( fp ) )
								{
									
									fread ( &c, sizeof ( char ), 1, fp );
									szData [ iOffset++ ] = c;
								}

								szData [ strlen ( szData ) - 1 ] = 0;
								szData [ strlen ( szData ) - 1 ] = 0;

								fclose ( fp );

								strcpy ( file.szDisplayName, szData );
							}
							

							SetCurrentDirectory ( szOriginalPath );
							
						}
						

						{
							char szOut [ 256 ] = "";
							sprintf ( szOut, "cLibraryImage::LoadImages - szImageName - %s", file.szImageName );
							theApp.m_Debug.Write ( szOut );

							sprintf ( szOut, "cLibraryImage::LoadImages - szFullPath - %s", file.szFullPath );
							theApp.m_Debug.Write ( szOut );
						}

						FileList.push_back ( file );
						///////

						//iFileListCount++;
					}
				}
			}  
			else
			{
				DWORD err = ::GetLastError ( );
			}
		}

		::FindClose ( hFind );
	}

	m_Images.DeleteImageList ( );
	m_Images.Create     ( 64, 64, ILC_COLORDDB, 1, 16 );
	m_Images.SetBkColor ( RGB ( 255, 255, 255 ) );
	
		
	this->DeleteAllItems ( );
	this->SetImageList ( &m_Images, LVSIL_NORMAL );
	//this->SetItemCount ( 64 );

	int iA = 0;
	int iB = 0;

	{
		char szOut [ 256 ] = "";
		sprintf ( szOut, "cLibraryImage::LoadImages - FileList.size ( ) - %i", FileList.size ( ) );
		theApp.m_Debug.Write ( szOut );
	}

	//for ( int i = 0; i < iFileListCount; i++ )
	for ( int i = 0; i < (int)FileList.size ( ); i++ )
	{
		CString bitmapLoad = FileList [ i ].szImageName;
		CString name       = FileList [ i ].szDisplayName;

		CString filename = FileList [ i ].szFullPath;

		// 170105
		//wcscpy ( m_szImageFileNames [ m_iImageCount++ ], filename );

		

			strcpy ( m_szImageExtra [ m_iImageCount ], name );

		strcpy ( m_szImageFileNames [ m_iImageCount++ ], filename );

		CBitmap	bitmap;

		HANDLE hImage = 0;

		hImage = ::LoadImage ( NULL, bitmapLoad, IMAGE_BITMAP, 64, 64, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION );
		//hImage = ::LoadImage ( NULL, bitmapLoad, IMAGE_BITMAP, 64, 64, LR_MONOCHROME | LR_LOADFROMFILE | LR_CREATEDIBSECTION );

		// 250105
		if ( !hImage )
		{
			char szOut [ 256 ] = "";

			sprintf ( szOut, "cLibraryImage::LoadImages - failed to load image - %s", bitmapLoad );

			theApp.m_Debug.Write ( szOut );
		}
		
		if ( !bitmap.Attach ( hImage ) )
		{
			theApp.m_Debug.Write ( "cLibraryImage::LoadImages - failed to attach image" );
		}

		m_Images.Add ( &bitmap, RGB ( 0, 0, 0 ) );
		bitmap.DeleteObject();

		//m_Images.SetBkColor ( RGB ( 255, 0, 0 ) );

		this->InsertItem ( iA++, name, iB++ );
	}

	//m_Images.SetOverlayImage ( 0, 1 );

	{
		theApp.m_Debug.Write ( "cLibraryImage::LoadImages - End\n" );
	}
}


void cLibraryImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnHScroll(nSBCode, nPos, pScrollBar);
}

void cLibraryImage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL cLibraryImage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CListCtrl ::OnMouseWheel(nFlags, zDelta, pt);
}

void cLibraryImage::OnVScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnVScrollClipboard(pClipAppWnd, nSBCode, nPos);
}

void cLibraryImage::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl ::OnMouseMove(nFlags, point);
}
