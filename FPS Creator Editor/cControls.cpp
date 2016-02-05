
#include "stdafx.h"
#include "editor.h"
#include "cControls.h"
#include "FileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SET_EDIT_ITEM				40
#define SET_EDIT_TEXT				44
#define GET_EDIT_TEXT				48

#define BUTTON_CLICKED				52

#define BROWSE_DISPLAY				56
#define BROWSE_FILE_SELECTED		60

#define	SET_LIST_ITEM				64
#define ADD_LIST_ITEM				68
#define DELETE_LIST_ITEM			116
#define GET_LIST_ITEM				72
#define SET_LIST_INDEX				76
#define GET_LIST_SELECTION			120
#define LIST_SELECTION				124
#define LIST_INSERT_ITEM			128
#define LIST_INSERT_POSITION		132
#define LIST_CLEAR					152
#define LIST_SELECT_ITEM			156
#define LIST_SELECT_ITEM_INDEX		160

#define	SET_COMBO_ITEM				80
#define ADD_COMBO_ITEM				84
#define GET_COMBO_ITEM				88
#define SET_COMBO_INDEX				92

#define SET_RADIO_ITEM				96
#define GET_RADIO_ITEM				100
#define SET_RADIO_STATE				148

#define SET_PROGRESS_ITEM			108
#define SET_PROGRESS_POSITION		112

#define	STRING_A					1000
#define	STRING_B					1256
#define	STRING_C					1512
#define	STRING_D					1768

cControls::cControls ( )
{
	memset ( m_pList,			0, sizeof ( m_pList			 ) );
	memset ( m_pEdit,			0, sizeof ( m_pEdit			 ) );
	memset ( m_pButton,			0, sizeof ( m_pButton		 ) );
	memset ( m_bButtonClicked,	0, sizeof ( m_bButtonClicked ) );
	memset ( m_bRadioClicked,	0, sizeof ( m_bRadioClicked  ) );
	memset ( m_pCombo,			0, sizeof ( m_pCombo		 ) );
	memset ( m_pRadio,			0, sizeof ( m_pRadio		 ) );
	memset ( m_iComboSize,      0, sizeof ( m_iComboSize     ) );
	memset ( m_pProgress,       0, sizeof ( m_pProgress      ) );

	m_iListCount   = 0;
	m_iEditCount   = 0;
	m_iButtonCount = 0;
	m_iComboCount  = 0;
}

cControls::~cControls ( )
{

}

void cControls::AddList ( CListBox* pList, int iOffset )
{
	m_pList [ iOffset ] = pList;
}

void cControls::AddEdit ( CEdit* pEdit, int iOffset )
{
	m_pEdit [ iOffset ] = pEdit;
}

void cControls::AddButton ( CButton* pButton, int iOffset )
{
	m_pButton [ iOffset ] = pButton;
}

void cControls::AddCombo ( CComboBoxEx*	pCombo, int iOffset )
{
	m_pCombo [ iOffset ] = pCombo;
}

void cControls::RemoveCombo	( int iOffset )
{
	m_pCombo [ iOffset ] = NULL;

	m_iComboSize [ iOffset ] = 0;
}

void cControls::AddRadioButton ( CButton* pButton, int iOffset )
{
	m_pRadio [ iOffset ] = pButton;
}

void cControls::AddProgressBar ( CProgressCtrl* pProgress, int iOffset )
{
	m_pProgress [ iOffset ] = pProgress;
}

void cControls::SetButton ( int iOffset )
{
	m_bButtonClicked [ iOffset ] = true;
}

void cControls::SetRadio ( int iOffset, bool bState )
{
	m_bRadioClicked [ iOffset ] = bState;
}

//void cControls::Update ( BYTE* pData )
void cControls::Update ( cIPC* pIPC )
{
	UpdateEditBoxes    ( pIPC );
	UpdateButtons      ( pIPC );
	UpdateBrowse       ( pIPC );
	UpdateFolderSelect ( pIPC );
	UpdateListControls ( pIPC );
	UpdateComboBoxes   ( pIPC );
	UpdateRadioButtons ( pIPC );
	UpdateProgress	   ( pIPC );
}

void cControls::UpdateEditBoxes ( cIPC* pIPC )
{
	DWORD		dwSetEditItem			= 0;
	DWORD		dwSetEditText			= 0;
	DWORD		dwGetEditText			= 0;
	char		szTextANSI [ 255 ]		= "";
	CString		szTextUNI				= "";

	// flags
	pIPC->ReceiveBuffer ( &dwSetEditText, SET_EDIT_TEXT, sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwGetEditText, GET_EDIT_TEXT, sizeof ( DWORD ) );

	// support data
	pIPC->ReceiveBuffer ( &dwSetEditItem, SET_EDIT_ITEM, sizeof ( DWORD ) );

	if ( dwSetEditItem && dwSetEditText )
	{
		char szString [ MAX_PATH ];
		pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
		ConvertANSIToWide ( ( BYTE* ) &szString [ 0 ], szTextANSI, &szTextUNI );
		m_pEdit [ dwSetEditItem ]->SetWindowText ( szTextUNI );

		// reset states when action completed
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, SET_EDIT_ITEM, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, SET_EDIT_TEXT, sizeof ( DWORD ) );
	}

	if ( dwSetEditItem && dwGetEditText )
	{
		m_pEdit [ dwSetEditItem ]->GetWindowText ( szTextUNI );
		char szString [ MAX_PATH ];
		ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &szTextUNI, szTextANSI );
		pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
	
		// reset states when action completed
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, SET_EDIT_ITEM, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, GET_EDIT_TEXT, sizeof ( DWORD ) );
	}
}

void cControls::UpdateButtons ( cIPC* pIPC )
{
	for ( int iButton = 1; iButton < 100; iButton++ )
	{
		if ( m_bButtonClicked [ iButton ] )
		{
			pIPC->SendBuffer ( &iButton, BUTTON_CLICKED, sizeof ( DWORD ) );
			m_bButtonClicked [ iButton ] = false;
		}
	}
}

void cControls::UpdateBrowse ( cIPC* pIPC )
{
	DWORD			dwDisplayBrowse			= 0;
	DWORD			dwBrowseFileSelected	= 1;
	char			szTitleANSI     [ 255 ]	= "";
	char			szDirectoryANSI [ 255 ]	= "";
	char			szFilterANSI    [ 255 ]	= "";
	char			szFileANSI      [ 255 ]	= "";
	CString			szTitleUNI				= "";
	CString			szDirectoryUNI			= "";
	CString			szFilterUNI				= "";
	CString			szFileUNI				= "";
//	TCHAR			szFile [ 260 ];
//	OPENFILENAME	ofn;

	pIPC->ReceiveBuffer ( &dwDisplayBrowse, BROWSE_DISPLAY, sizeof ( DWORD ) );
	if ( dwDisplayBrowse )
	{
		dwDisplayBrowse = 0;
		pIPC->SendBuffer ( &dwDisplayBrowse, BROWSE_DISPLAY, sizeof ( DWORD ) );

		//pData [ BROWSE_DISPLAY ] = 0;

		char szStringA [ MAX_PATH ];
		char szStringB [ MAX_PATH ];
		char szStringC [ MAX_PATH ];

		pIPC->ReceiveBuffer ( &szStringA [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
		pIPC->ReceiveBuffer ( &szStringB [ 0 ], STRING_B, MAX_PATH * sizeof ( char ) );
		pIPC->ReceiveBuffer ( &szStringC [ 0 ], STRING_C, MAX_PATH * sizeof ( char ) );

		ConvertANSIToWide ( ( BYTE* ) &szStringA [ 0 ], szTitleANSI,     &szTitleUNI     );
		ConvertANSIToWide ( ( BYTE* ) &szStringB [ 0 ], szDirectoryANSI, &szDirectoryUNI );
		ConvertANSIToWide ( ( BYTE* ) &szStringC [ 0 ], szFilterANSI,    &szFilterUNI    );

		cNewCustomFileDialog dialog ( true, szDirectoryUNI, szTitleUNI, szFilterUNI );

		if (dialog.DoModal() == IDOK)
		{
			szFileUNI = dialog.GetPathName();
		}
		
		char szString [ MAX_PATH ];
		

		ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &szFileUNI, szFileANSI );

		pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

		pIPC->SendBuffer ( &dwBrowseFileSelected, BROWSE_FILE_SELECTED, sizeof ( DWORD ) );
	}
}

void cControls::UpdateListControls ( cIPC* pIPC )
{
	DWORD		dwSetItem			= 0;
	DWORD		dwAddItem			= 0;
	DWORD		dwGetItem			= 0;
	DWORD		dwSetIndex			= 0;
	DWORD		dwDeleteItem		= 0;

	DWORD		dwGetListSelection		= 0;
	DWORD		dwListSelection			= 0;

	DWORD		dwInsertItem			= 0;
	DWORD		dwInsertPosition		= 0;

	DWORD		dwSelectItem		= 0;
	DWORD		dwSelectItemIndex	= 0;

	DWORD		dwListClear = 0;

	char		szTextANSI [ 255 ]	= "";
	CString		szTextUNI			= "";

	// trigger flags (first so if they ARE set, we then get the CRITICAL data and do not miss it from parallel process change of filemap)
	pIPC->ReceiveBuffer ( &dwSelectItem,		LIST_SELECT_ITEM,		sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwAddItem,			ADD_LIST_ITEM,			sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwListClear,			LIST_CLEAR,				sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwGetItem,			GET_LIST_ITEM,			sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwDeleteItem,		DELETE_LIST_ITEM,		sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwGetListSelection,	GET_LIST_SELECTION,		sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwInsertItem,		LIST_INSERT_ITEM,		sizeof ( DWORD ) );

	// these values support above flags and are gathered only AFTER the flags are determined this call
	pIPC->ReceiveBuffer ( &dwSetItem,			SET_LIST_ITEM,			sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwSetIndex,			SET_LIST_INDEX,			sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwSelectItemIndex,	LIST_SELECT_ITEM_INDEX, sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwInsertPosition,	LIST_INSERT_POSITION,	sizeof ( DWORD ) );

	if ( dwSelectItem )
	{
		m_pList [ dwSetItem ]->SetCurSel ( dwSelectItemIndex );

		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, LIST_SELECT_ITEM,		 sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, LIST_SELECT_ITEM_INDEX, sizeof ( DWORD ) );
	}

	if ( dwSetItem && dwAddItem )
	{
		char szString [ MAX_PATH ];
		pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

		ConvertANSIToWide ( ( BYTE* ) &szString [ 0 ], szTextANSI, &szTextUNI );

		//ConvertANSIToWide ( &pData [ STRING_A ], szTextANSI, &szTextUNI );

		m_pList [ dwSetItem ]->AddString ( szTextUNI );


		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, SET_LIST_ITEM,	sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, ADD_LIST_ITEM, sizeof ( DWORD ) );

		//memcpy ( &pData [ SET_LIST_ITEM ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &pData [ ADD_LIST_ITEM ], &dwValue, sizeof ( DWORD ) );
		/////////////


		//pData [ SET_LIST_ITEM ] = 0;
		//pData [ ADD_LIST_ITEM ] = 0;
	}

	if ( dwListClear )
	{
		if ( m_pList [ dwSetItem ] != NULL )
		{

			int iCount = m_pList [ dwSetItem ]->GetCount ( );

			for ( int i = 0; i < iCount; i++ )
				m_pList [ dwSetItem ]->DeleteString ( 0 );

			/////////////
			DWORD dwValue = 0;

			pIPC->SendBuffer ( &dwValue, SET_LIST_ITEM,	sizeof ( DWORD ) );
			pIPC->SendBuffer ( &dwValue, LIST_CLEAR,	sizeof ( DWORD ) );

			//memcpy ( &pData [ SET_LIST_ITEM ], &dwValue, sizeof ( DWORD ) );
			//memcpy ( &pData [ LIST_CLEAR ], &dwValue, sizeof ( DWORD ) );
			/////////////

			//pData [ SET_LIST_ITEM ] = 0;
			//pData [ LIST_CLEAR ] = 0;
		}
	}

	// Getting an item from a list
	if ( dwSetItem && dwGetItem )
	{
		// temp string
		CString strItem;

		// How many in list
		int iCount = m_pList [ dwSetItem ]->GetCount ( );

		// Ensure index is within list range
		if ( dwSetIndex < 9999 )
		{
			// v105 - 200107 - larger means -1 passed in (no level selected)
			if ( (int)dwSetIndex < iCount )
			{
				// get list string into temp
				CString temp;
				m_pList [ dwSetItem ]->GetText ( dwSetIndex, strItem );
				temp = strItem;

				// if gamebuild keyinput list
				if ( dwSetItem == 2 )
				{
					// get actual key value from list (real-DX-scancode)
					DWORD dwValue = m_pList [ dwSetItem ]->GetItemData ( dwSetIndex );

					// wipe out temp char and fill with string equiviant of keyscancode
					TCHAR szChar [ 10 ] = _T( "" );
					_itoa ( dwValue, szChar, 10 );

					// return this string version of the keyscancode
					strItem = szChar;

					// if it was a RETURNKEY with no value, artificially add one
					if ( temp == _T("Enter")) strItem = _T("28");
					
					// add actual name to end of the value
					strItem += _T(" ") + temp;
				}
			}
		}

		// copy string to filemap
		char szString [ MAX_PATH ];
		ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &strItem, szTextANSI );
		pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

		// resets
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, GET_LIST_ITEM,	 sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, SET_LIST_INDEX, sizeof ( DWORD ) );
	}

	if ( dwSetItem && dwDeleteItem )
	{
		m_pList [ dwSetItem ]->DeleteString ( dwSetIndex );

		//pData [ DELETE_LIST_ITEM ] = 0;

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, DELETE_LIST_ITEM,	 sizeof ( DWORD ) );

		//memcpy ( &pData [ DELETE_LIST_ITEM ], &dwValue, sizeof ( DWORD ) );
		/////////////
	}

	if ( dwGetListSelection )
	{
		DWORD dwAnyIndex = m_pList [ dwSetItem ]->SendMessage ( LB_GETCURSEL, 0, 0 );
		if ( dwAnyIndex!=LB_ERR )
			dwListSelection = dwAnyIndex;
		else
			dwListSelection = -1;

		pIPC->SendBuffer ( &dwListSelection, LIST_SELECTION,	 sizeof ( DWORD ) );

		//memcpy ( &pData [ LIST_SELECTION ], &dwListSelection, sizeof ( DWORD ) );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, GET_LIST_SELECTION, sizeof ( DWORD ) );

		//memcpy ( &pData [ GET_LIST_SELECTION ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//pData [ GET_LIST_SELECTION ] = 0;
	}

	if ( dwInsertItem )
	{
		char szString [ MAX_PATH ];
		pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

		ConvertANSIToWide ( ( BYTE* ) &szString [ 0 ], szTextANSI, &szTextUNI );

		//ConvertANSIToWide ( &pData [ STRING_A ], szTextANSI, &szTextUNI );

		m_pList [ dwSetItem ]->InsertString ( dwInsertPosition, szTextUNI );

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, LIST_INSERT_ITEM, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, ADD_LIST_ITEM,	   sizeof ( DWORD ) );

		//memcpy ( &pData [ LIST_INSERT_ITEM ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &pData [ ADD_LIST_ITEM ], &dwValue, sizeof ( DWORD ) );
		/////////////

		

		//pData [ LIST_INSERT_ITEM ] = 0;
		//pData [ ADD_LIST_ITEM ] = 0;
	}

	
}

void cControls::UpdateComboBoxes ( cIPC* pIPC )
{
	DWORD		dwSetItem			= 0;
	DWORD		dwAddItem			= 0;
	DWORD		dwGetItem			= 0;
	DWORD		dwSetIndex			= 0;
	char		szTextANSI [ 255 ]	= "";
	CString		szTextUNI			= "";

	// flags
	pIPC->ReceiveBuffer ( &dwAddItem,  ADD_COMBO_ITEM,			sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwGetItem,  GET_COMBO_ITEM,			sizeof ( DWORD ) );

	// support data
	pIPC->ReceiveBuffer ( &dwSetItem,  SET_COMBO_ITEM,			sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwSetIndex, SET_COMBO_INDEX,			sizeof ( DWORD ) );

	if ( dwSetItem && dwAddItem && m_pCombo [ dwSetItem ] )
	{
		char szString [ MAX_PATH ];
		pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
		ConvertANSIToWide ( ( BYTE* ) &szString [ 0 ], szTextANSI, &szTextUNI );

		// 100105 - check string is long enough
		if ( strlen ( szTextANSI ) > 0 )
		{
			COMBOBOXEXITEM	cbi = { 0 };

			cbi.mask    = CBEIF_TEXT;
			cbi.iItem   = m_iComboSize [ dwSetItem ]++;

			cbi.pszText = new TCHAR [ szTextUNI.GetLength ( ) + 1 ];

			// 170105
			//wcscpy ( cbi.pszText, szTextUNI );
			strcpy ( cbi.pszText, szTextUNI );

			m_pCombo [ dwSetItem ]->InsertItem  ( &cbi );

			m_pCombo [ dwSetItem ]->SetCurSel  ( 0 );
		}
		
		/////////////
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, SET_COMBO_ITEM, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, ADD_COMBO_ITEM, sizeof ( DWORD ) );
	}

	if ( dwSetItem && dwGetItem )
	{
		CString strItem;

		char szString [ MAX_PATH ];
		ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &strItem, szTextANSI );
		pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

		/////////////
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, SET_COMBO_ITEM, sizeof ( DWORD ) );
		pIPC->SendBuffer ( &dwValue, GET_COMBO_ITEM, sizeof ( DWORD ) );
	}
}

void cControls::UpdateRadioButtons ( cIPC* pIPC )
{
	DWORD	dwSetItem	= 0;
	DWORD	dwGetItem	= 0;

	DWORD	dwState		= 0;

	pIPC->ReceiveBuffer ( &dwSetItem,  SET_RADIO_ITEM,	sizeof ( DWORD ) );
	pIPC->ReceiveBuffer ( &dwState,    SET_RADIO_STATE,	sizeof ( DWORD ) );

	if ( dwSetItem )
	{
		if ( m_bRadioClicked [ dwSetItem ] )
		{
			dwGetItem = 1;

			pIPC->SendBuffer ( &dwGetItem, GET_RADIO_ITEM, sizeof ( DWORD ) );

			//memcpy ( &pData [ GET_RADIO_ITEM ], &dwGetItem, sizeof ( DWORD ) );
		}
		else
		{
			dwGetItem = 0;

			pIPC->SendBuffer ( &dwGetItem, GET_RADIO_ITEM, sizeof ( DWORD ) );

			//memcpy ( &pData [ GET_RADIO_ITEM ], &dwGetItem, sizeof ( DWORD ) );
		}

		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, SET_RADIO_ITEM, sizeof ( DWORD ) );

		//memcpy ( &pData [ SET_RADIO_ITEM ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//pData [ SET_RADIO_ITEM ] = 0;
	}

	if ( dwSetItem && dwState )
	{
		if ( dwState == 1 )
		{
			if ( m_pRadio [ dwSetItem ] )
			{
				m_pRadio [ dwSetItem ]->SetCheck ( TRUE );
				m_bRadioClicked [ dwSetItem ] = true;
			}
		}
		else
		{
			if ( m_pRadio [ dwSetItem ] )
			{
				m_pRadio [ dwSetItem ]->SetCheck ( FALSE );
				m_bRadioClicked [ dwSetItem ] = false;
			}
		}

		dwGetItem = 0;
		pIPC->SendBuffer ( &dwGetItem, SET_RADIO_STATE, sizeof ( DWORD ) );
		//memcpy ( &pData [ SET_RADIO_STATE ], &dwGetItem, sizeof ( DWORD ) );
	}
}

void cControls::UpdateProgress ( cIPC* pIPC )
{
	DWORD	dwSetItem	= 0;
	DWORD	dwProgress	= 0;

	pIPC->ReceiveBuffer ( &dwSetItem, SET_PROGRESS_ITEM, sizeof ( DWORD ) );
	if ( dwSetItem )
	{
		pIPC->ReceiveBuffer ( &dwProgress, SET_PROGRESS_POSITION, sizeof ( DWORD ) );

		//memcpy ( &dwProgress, &pData [ SET_PROGRESS_POSITION ], sizeof ( DWORD ) );

		m_pProgress [ dwSetItem ]->SetPos ( dwProgress );

		/////////////
		DWORD dwValue = 0;
		pIPC->SendBuffer ( &dwValue, SET_PROGRESS_ITEM, sizeof ( DWORD ) );
		//memcpy ( &pData [ SET_PROGRESS_ITEM ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//pData [ SET_PROGRESS_ITEM ] = 0;
	}
}

CString			m_Path;

int CALLBACK cControls::BrowseCallbackProc ( HWND hwnd, UINT uMsg, LPARAM , LPARAM lpData )
{
	if (uMsg == BFFM_INITIALIZED)
	{
		//170105
		//TCHAR szFile [ MAX_PATH ];
		CHAR szFile [ MAX_PATH ];

		//170105
		//wcscpy ( szFile, m_Path );

		strcpy ( szFile, m_Path );

		SendMessage (hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)szFile);
	}

	return 0;
}

void cControls::UpdateFolderSelect ( cIPC* pIPC )
{
	// folder display - 
	// folder selected - 60
	// folder start dir - string a

	char			szTitleANSI     [ 255 ]	= "";
	char			szDirectoryANSI [ 255 ]	= "";
	char			szFilterANSI    [ 255 ]	= "";
	char			szFileANSI      [ 255 ]	= "";
	CString			szTitleUNI				= "";
	CString			szDirectoryUNI			= "";
	CString			szFilterUNI				= "";
	CString			szFileUNI				= "";

	DWORD dwBrowse = 0;
	DWORD dwSelect = 1;

	pIPC->ReceiveBuffer ( &dwBrowse, 200, sizeof ( DWORD ) );
	if ( dwBrowse )
	{
		/////////////
		DWORD dwValue = 0;

		pIPC->SendBuffer ( &dwValue, 200, sizeof ( DWORD ) );

		//memcpy ( &pData [ 200 ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//pData [ 200 ] = 0;

		char szString [ MAX_PATH ];
		pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
		ConvertANSIToWide ( ( BYTE* ) &szString [ 0 ], szDirectoryANSI, &szDirectoryUNI );

		//ConvertANSIToWide ( &pData [ STRING_A ], szDirectoryANSI,     &szDirectoryUNI     );

		//ConvertANSIToWide ( &pData [ STRING_B ], szDirectoryANSI, &szDirectoryUNI );
		//ConvertANSIToWide ( &pData [ STRING_C ], szFilterANSI,    &szFilterUNI    );

		TCHAR szDisplayName [MAX_PATH];

		BROWSEINFO bi;
		ZeroMemory (&bi, sizeof (bi));

		bi.lpszTitle = _T("");
		bi.pszDisplayName = szDisplayName;
		bi.hwndOwner = NULL;
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = (LPARAM) this;
		bi.pidlRoot = NULL;
		bi.ulFlags = 0;
		bi.iImage = -1;

		m_Path = szDirectoryUNI;

		//m_lpszInitialPath = lplszInitialFolder;

		BOOL bRes = FALSE;

		LPITEMIDLIST pidlRes = SHBrowseForFolder (&bi);
		if (pidlRes != NULL)
		{
			TCHAR szPath [MAX_PATH];
			if (SHGetPathFromIDList (pidlRes, szPath))
			{
				
				CString final = szPath;
				//strFolder = szPath;

				char szString [ MAX_PATH ];
//				pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
				ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &final, szFileANSI );
				pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

				//ConvertWideToANSI ( &pData [ STRING_A ], &final, szFileANSI );

				pIPC->SendBuffer ( &dwSelect, 204, sizeof ( DWORD ) );

				//memcpy ( &pData [ 204 ], &dwSelect, sizeof ( DWORD ) );
			}
			else
			{
			
			}
		}
		else
		{
	CString final = _T("");
				//strFolder = szPath;

				char szString [ MAX_PATH ];
//				pIPC->ReceiveBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );
				ConvertWideToANSI ( ( BYTE* ) &szString [ 0 ], &final, szFileANSI );
				pIPC->SendBuffer ( &szString [ 0 ], STRING_A, MAX_PATH * sizeof ( char ) );

				//ConvertWideToANSI ( &pData [ STRING_A ], &final, szFileANSI );

				pIPC->SendBuffer ( &dwSelect, 204, sizeof ( DWORD ) );

				//memcpy ( &pData [ 204 ], &dwSelect, sizeof ( DWORD ) );
		}
		
	}
}

		
