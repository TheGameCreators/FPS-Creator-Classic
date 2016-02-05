// cLibraryTree.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cLibraryTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char		m_szLastPath [ 255 ];

char		m_szLastPathName [ 255 ];
char		m_szLastPathSegments [ 255 ];
char		m_szLastPathEntities [ 255 ];
char		m_szLastPathPrefabs [ 255 ];

struct sTreeData
{
	HTREEITEM	item;
	CString		displayName;
	CString		realName;
};

vector < sTreeData > g_TreeData;

cLibraryTree::cLibraryTree ( )
{
	m_hCheck = 0;
}

cLibraryTree::~cLibraryTree ( )
{
}


BEGIN_MESSAGE_MAP(cLibraryTree, CTreeCtrl)
	//{{AFX_MSG_MAP(cLibraryTree)
	//ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int cLibraryTree::Start ( cLibraryImage* pImage, int iScroll )
{
	g_TreeData.clear ( );

	m_pImage = pImage;

	CString string = "FPSEXCHANGE";
	char szTest [ MAX_PATH ];
	memset ( szTest, 0, sizeof ( szTest ) );

	m_bCreating = true;

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
					char szA [ 255 ];
					//memcpy ( &szA [ 0 ], &theApp.m_pData [ 1000 ], sizeof ( char ) * 255 );

					char szB [ 255 ];
					//memcpy ( &szB [ 0 ], &theApp.m_pData [ 1256 ], sizeof ( char ) * 255 );

					//memcpy ( &szTest [ 0 ], &theApp.m_pData [ 2536 ], sizeof ( char ) * 255 );

					cIPC* pIPC	= theApp.m_Message.GetIPC ( );
					
					pIPC->ReceiveBuffer ( &szA    [ 0 ], 1000, sizeof ( szA    ) );
					pIPC->ReceiveBuffer ( &szB    [ 0 ], 1256, sizeof ( szB    ) );
					pIPC->ReceiveBuffer ( &szTest [ 0 ], 2536, sizeof ( szTest ) );

					

					m_Path = szA;
					m_Name = szB;

					m_sPath = m_Path;

					strcpy ( m_szLastPathName, szB );
				}
			}

			//UnmapViewOfFile ( m_lpVoid );
			//CloseHandle     ( m_hFileMap );
		}
	}

	m_ImageListTree.Create( 1, 17, 1, RGB(255,255,255) );

	this->SetImageList( &m_ImageListTree, TVSIL_NORMAL );

	//CBitmap icon;
	//icon.LoadBitmap( IDB_STORE_ICON );
	//m_ImageListTree.Add( &icon, (CBitmap*) NULL );

	m_hRoot = InsertItemEx( TVI_ROOT, NULL, m_sPath, 1 );

	this->SetItemText ( m_hRoot, m_Name );

	char szTokens [ 10 ] [ 255 ];
	int  iToken = 0;
	
	if ( strlen ( szTest ) > 0 )
	{
		char  seps[ ] = "\\";
		char* token;

		memset ( szTokens, 0, sizeof ( szTokens ) );

		token = strtok ( szTest, seps );

		while ( token != NULL )
		{
			strcpy ( szTokens [ iToken++ ], token );
			token = strtok ( NULL, seps );
		}
	}

	int iIndex = 0;
	int nLevel = 0;
	CString sDirName;
	HTREEITEM hCurrent = TVI_ROOT;

	HTREEITEM test;

	while ( ParsePath( m_sPath, nLevel, &sDirName ) )
	{
		HTREEITEM hItem = this->GetChildItem( hCurrent );

		if ( nLevel == 0 )
			test = hItem;

		nLevel++;
	}

	SetRedraw(FALSE);
	
	Expand( m_hRoot, 2 );
	ExpandItem();
	
	SetRedraw(TRUE);

	this->Select( m_hRoot, TVGN_CARET );

	if (  m_hCheck )
		this->Select( m_hCheck, TVGN_CARET );

	m_bCreating = false;

	// 250105
	m_pImage->LoadImages ( m_sSelectionPath );

	return 0;
}

void cLibraryTree::SaveTreeState( )
{
	//theApp.m_TreeGroups.SetTreeSize( 100 );
	
	Initialize( );
	HTREEITEM hItem = GetNext();
	while ( hItem )
	{
		CString sPath = GetItemPath( hItem );
		theApp.m_TreeGroups.SetExpanded( sPath, (GetItemState( hItem, TVIS_EXPANDED ) & TVIS_EXPANDED) > 1 ? 1 : 0 );
		hItem = GetNext();
	}
}

bool cLibraryTree::ParsePath ( CString sPath, int nLevel, CString* const psDirName )
{
	ASSERT ( psDirName );
	
	int nStart = 0;

	while ( nLevel )
	{
		nStart = sPath.Find ( _T ( '\\' ), nStart );

		if ( nStart < 0 )
			return false;

		nLevel--;
		nStart++;
	}

	int nEnd = sPath.Find ( _T ( '\\' ), nStart );

	if ( nEnd < 0 )
		*psDirName = sPath.Mid ( nStart );
	else
		*psDirName = sPath.Mid ( nStart, nEnd - nStart );

	return true;
}

HTREEITEM cLibraryTree::InsertItemEx ( HTREEITEM hParent, NETRESOURCE* const pNetResource, CString sText, int iImage, int iImageSelected, HTREEITEM hInsertAfter )
{
	TVINSERTSTRUCT InsertStruct;

	InsertStruct.hParent			= hParent;	
	InsertStruct.hInsertAfter		= hInsertAfter;
	InsertStruct.itemex.mask		= TVIF_IMAGE | TVIF_TEXT | TVIF_CHILDREN | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertStruct.itemex.pszText		= sText.GetBuffer ( sText.GetLength ( ) );
	InsertStruct.itemex.iImage		= iImage;
	InsertStruct.itemex.cChildren	= 1;
	InsertStruct.itemex.lParam		= ( LPARAM ) pNetResource;

	if ( iImageSelected == -1 )
		InsertStruct.itemex.iSelectedImage = iImage;
	else
		InsertStruct.itemex.iSelectedImage = iImageSelected;

	sText.ReleaseBuffer ( );

	return this->InsertItem ( &InsertStruct );
}

bool cLibraryTree::PopulateTree ( CString sPath, HTREEITEM hParent ) 
{
	bool bGotChildren = false;

	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile ( sPath+_T ( "*.*" ) );

		while ( bWorking )
		{
			bWorking = finder.FindNextFile ( );

			if ( finder.IsDots ( ) )
				continue;


			CString test = finder.GetFileTitle( );

			if ( test.GetLength ( ) == 0 )
				continue;

			if ( test [ 0 ] == _T( '_' ) )
				continue;

			if ( finder.IsDirectory ( ) )
			{
				HTREEITEM test = NULL;

				DWORD dwLength = finder.GetFileName().GetLength();
				char *szFilename = new char [ dwLength + 1 ];
				strcpy_s( szFilename, dwLength+1, finder.GetFileName() );
				strlower( szFilename );

				if ( strcmp( szFilename, "purchased" ) == 0 )
				{
					test = InsertItemEx ( hParent, NULL, szFilename, DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN, TVI_FIRST );
				}
				else
				{
					test = InsertItemEx ( hParent, NULL, finder.GetFileName ( ), DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN );
				}

				delete [] szFilename;
				szFilename = NULL;

				// 220205
				{
					
					sTreeData data;

					data.item        = test;
					data.displayName = "large";
					data.realName    = finder.GetFileName ( );

					

					char szLOCPath [ MAX_PATH ] = "";

					{
						char szItem [ 255 ] [ MAX_PATH ];
						char  seps[ ] = "\\/";
						char* token;
						int iToken = 0;
						int iFileIndex = 0;

						char szPath [ MAX_PATH ] = "";
						strcpy ( szPath, finder.GetRoot ( ) );

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

						strcpy ( szLOCPath, theApp.m_szLanguagePath );
						strcat ( szLOCPath, "/textfiles/library/" );

						for ( int i = iFileIndex + 1; i < iToken; i++ )
						{
							strcat ( szLOCPath, szItem [ i ] );
							strcat ( szLOCPath, "/" );
						}
					}

					//strcpy ( szLOCPath, finder.GetRoot ( ) );
					//strcat ( szLOCPath, "\\" );
					strcat ( szLOCPath, data.realName );
					strcat ( szLOCPath, ".loc" );
					
					// now open this file and extract the text
					FILE* fp = fopen ( szLOCPath, "rt" );

					if ( fp )
					{
						char szData [ MAX_PATH ] = "";
						int iOffset = 0;
						char c;

						while ( !feof ( fp ) )
						{
							
							fread ( &c, sizeof ( char ), 1, fp );

							if ( c == '\n' )
								break;

							szData [ iOffset++ ] = c;
						}

						// handle if CR in LOC file or not
						if ( c == '\n' )
							szData [ iOffset ] = 0;
						else
							szData [ iOffset - 1 ] = 0;

						fclose ( fp );

						data.displayName = szData;

						g_TreeData.push_back ( data );
					}
					else
					{
						data.displayName = data.realName;
						g_TreeData.push_back ( data );
					}
				}

				{
					CString test1 = finder.GetFileName ( );
					CString check = sPath + test1;
					CString name = test1.SpanIncluding( check );

					{
						char szPath [ 255 ] = "";
						ConvertWideToANSI ( NULL, &check, szPath );

						if ( strnicmp ( m_szLastPathName, "Segment Library", strlen ( m_szLastPathName ) ) == 0 )
						{
							if ( strnicmp ( szPath, m_szLastPathSegments, strlen ( szPath ) ) == 0 )
								m_hCheck = test;
						}

						if ( strnicmp ( m_szLastPathName, "Entity Library", strlen ( m_szLastPathName ) ) == 0 )
						{
							if ( strnicmp ( szPath, m_szLastPathEntities, strlen ( szPath ) ) == 0 )
								m_hCheck = test;
						}

						if ( strnicmp ( m_szLastPathName, "Prefab Library", strlen ( m_szLastPathName ) ) == 0 )
						{
							if ( strnicmp ( szPath, m_szLastPathPrefabs, strlen ( szPath ) ) == 0 )
								m_hCheck = test;
						}
					}
				}

				bGotChildren = true;
			}
		}
	}

	if ( !bGotChildren )
	{
		TVITEM item		= { 0 };
		item.mask		= TVIF_HANDLE | TVIF_CHILDREN;
		item.hItem		= hParent;
		item.cChildren	= 0;

		this->SetItem ( &item );
	}

	return bGotChildren;
}

CString cLibraryTree::GetItemPath ( HTREEITEM hItem ) 
{
	CString sRet;

	do
	{
		NETRESOURCE* const pNetResource = ( NETRESOURCE* ) ( this->GetItemData ( hItem ) );
		
		if ( pNetResource )
		{
			sRet = CString ( pNetResource->lpRemoteName ) + _T( '\\' ) + sRet; 		
			break;
		}
		
		sRet  = this->GetItemText   ( hItem ) + _T ( '\\' ) + sRet; 
		hItem = this->GetParentItem ( hItem );
	}
	while ( hItem );

	return sRet;
}

void cLibraryTree::OnSelchangedTree ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = ( NM_TREEVIEW* ) pNMHDR;

	ASSERT ( pNMTreeView );
	ASSERT ( pResult );

	SetRedraw( 0 );
	
	CString sPath;
	
	this->SetItemText ( m_hRoot, m_Path );

	
	{
		// 220205
		for ( int i = 0; i < (int)g_TreeData.size ( ); i++ )
			this->SetItemText ( g_TreeData [ i ].item, g_TreeData [ i ].realName );

		/*
		for ( i = 0; i < g_TreeData.size ( ); i++ )
		{
			if ( g_TreeData [ i ].item == pNMTreeView->itemNew.hItem )
			{
				sPath = GetItemPath ( pNMTreeView->itemNew.hItem );
				break;
			}
		}	
		*/
	}
	

	sPath = GetItemPath ( pNMTreeView->itemNew.hItem );
	
	m_LastPath = sPath;

	m_sSelectionPath = sPath;

	if ( m_bCreating == false )
	{
		char szPath [ 255 ] = "";
		ConvertWideToANSI ( NULL, &sPath, szPath );

		//strcpy ( m_szLastPath, szPath );

		if ( strnicmp ( m_szLastPathName, "Segment Library", strlen ( m_szLastPathName ) ) == 0 )
			strcpy ( m_szLastPathSegments, szPath );
	
		if ( strnicmp ( m_szLastPathName, "Entity Library", strlen ( m_szLastPathName ) ) == 0 )
			strcpy ( m_szLastPathEntities, szPath );
		
		if ( strnicmp ( m_szLastPathName, "Prefab Library", strlen ( m_szLastPathName ) ) == 0 )
			strcpy ( m_szLastPathPrefabs, szPath );	

		// 250105
		m_pImage->LoadImages ( szPath );
	}

	this->SetItemText ( m_hRoot, m_Name );

	{
		// 220205
		for ( int i = 0; i < (int)g_TreeData.size ( ); i++ )
			this->SetItemText ( g_TreeData [ i ].item, g_TreeData [ i ].displayName );
	}
	
	SetRedraw( 1 );

	*pResult = 0;
}

void cLibraryTree::OnItemexpandingTree ( NMHDR* pNMHDR, LRESULT* pResult )
{
	CWaitCursor CursorWaiting;
	NM_TREEVIEW* pNMTreeView = ( NM_TREEVIEW* ) pNMHDR;
	
	ASSERT ( pNMTreeView );
	ASSERT ( pResult );

	SetRedraw( 0 );

	{
		// 220205
		for ( int i = 0; i < (int)g_TreeData.size ( ); i++ )
			this->SetItemText ( g_TreeData [ i ].item, g_TreeData [ i ].realName );
	}
	
	this->SetItemText ( m_hRoot, m_Path );

	if ( pNMTreeView->action == 2 )
	{
		CString sPath = GetItemPath ( pNMTreeView->itemNew.hItem );

		if ( !this->GetChildItem ( pNMTreeView->itemNew.hItem ) )
		{
			PopulateTree ( sPath, pNMTreeView->itemNew.hItem );

			if ( this->GetSelectedItem ( ) != pNMTreeView->itemNew.hItem )
				this->SelectItem ( pNMTreeView->itemNew.hItem );
		}
	}

	this->SetItemText ( m_hRoot, m_Name );

	{
		// 220205
		for ( int i = 0; i < (int)g_TreeData.size ( ); i++ )
			this->SetItemText ( g_TreeData [ i ].item, g_TreeData [ i ].displayName );
	}

	SetRedraw( 1 );
	

	*pResult = 0;
}

CString cLibraryTree::GetSelectionPath ( void )
{
	return m_sSelectionPath;
}