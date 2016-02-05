// ObjectProperty.cpp : implementation file
//

#include "stdafx.h"
#include "Editor.h"
#include "ObjectProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty

#define WM_ENTITY_ADDGROUP   (WM_USER + 1)
#define WM_ENTITY_DELETEGROUP   (WM_USER + 5)
#define WM_ENTITY_DELETECONTROL (WM_USER + 6)

#define WM_ENTITY_ADDEDITBOX (WM_USER + 2)
#define WM_ENTITY_ADDLISTBOX (WM_USER + 3)
#define WM_ENTITY_ADDCOLORPICKER (WM_USER + 10)
#define WM_ENTITY_ADDFILEPICKER (WM_USER + 11)
#define WM_ENTITY_ADDFONTPICKER (WM_USER + 12)

#define WM_ENTITY_SETCONTROL      (WM_USER + 7)
#define WM_ENTITY_GETCONTROL      (WM_USER + 8)
#define WM_ENTITY_SETCONTROLSTATE (WM_USER + 9)


#define WM_ENTITY_ADDITEMTOLISTBOX (WM_USER + 4)
#define WM_ENTITY_CLEARLISTBOX (WM_USER + 13)



CObjectProperty::~CObjectProperty()
{
}


BEGIN_MESSAGE_MAP(CObjectProperty, CBCGPropList)
	//{{AFX_MSG_MAP(CObjectProperty)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_MESSAGE( WM_ENTITY_ADDGROUP,         OnAddGroup   )
	ON_MESSAGE( WM_ENTITY_DELETEGROUP,         OnDeleteGroup   )
	ON_MESSAGE( WM_ENTITY_ADDEDITBOX,       OnAddEditBox )
	ON_MESSAGE( WM_ENTITY_DELETECONTROL,       OnDeleteControl )
	ON_MESSAGE( WM_ENTITY_ADDLISTBOX,       OnAddListBox )
	ON_MESSAGE( WM_ENTITY_ADDCOLORPICKER,       OnAddColorPicker )
	ON_MESSAGE( WM_ENTITY_ADDFILEPICKER,       OnAddFilePicker )
	ON_MESSAGE( WM_ENTITY_ADDFONTPICKER,       OnAddFontPicker )
	ON_MESSAGE( WM_ENTITY_ADDITEMTOLISTBOX, OnAddItemToListBox )
	ON_MESSAGE( WM_ENTITY_CLEARLISTBOX, OnClearListBox )
	ON_MESSAGE( WM_ENTITY_SETCONTROL, OnSetControl )
	ON_MESSAGE( WM_ENTITY_GETCONTROL, OnGetControl )
	ON_MESSAGE( WM_ENTITY_SETCONTROLSTATE, OnSetControlState )
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED ( IDOK,	 OnButtonApply )
	ON_BN_CLICKED ( IDCANCEL,	 OnButtonCancel )
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty message handlers


LRESULT CObjectProperty::OnAddGroup ( WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "\n" );
	//theApp.m_Debug.Write ( "OnAddGroup - Begin" );

	CString* pString = ( CString* ) lParam;

	CBCGProp* pProp = new CBCGProp ( *pString );

	sPropertyItem add;

	add.iType     = 0;
	add.pProperty = pProp;

	m_pGroups.push_back ( add );

	AddProperty ( pProp );

	//theApp.m_Debug.Write ( "AddProperty->pProp" );

	EnsureVisible ( pProp );

	return 0;
}

void CObjectProperty::TestUpdate ( )
{
	AdjustLayout ( );
}

LRESULT CObjectProperty::OnDeleteGroup ( WPARAM wParam, LPARAM lParam )
{
	int iIndex = ( int ) lParam;

	iIndex--;

	if ( iIndex >= (int)m_pGroups.size ( ) )
		return 0;

	if ( iIndex == 0 )
	{
		m_lstProps.RemoveHead();
		m_ToolTip.DelTool (this, 0);
		m_nTooltipsCount--;
	}
	else if ( iIndex == m_lstProps.GetCount ( ) - 1)
	{
		m_lstProps.RemoveTail();
		m_ToolTip.DelTool (this, m_lstProps.GetCount ( )-1);
		m_nTooltipsCount--;
	}
	else
	{
		POSITION pos = m_lstProps.FindIndex ( iIndex );
		m_lstProps.RemoveAt( pos );
		m_ToolTip.DelTool (this, iIndex);
		m_nTooltipsCount--;
	}
	return 0;
}

void CObjectProperty::ShowApply ( bool bState )
{
	if ( !bState )
	{
		m_ApplyButton.ShowWindow ( FALSE );
		m_CancelButton.ShowWindow ( FALSE );
	}
}

void CObjectProperty::OnButtonApply ( )
{
	m_bApplyButton = true;
}

void CObjectProperty::OnButtonCancel ( )
{
	m_bCancelButton = true;
}

void CObjectProperty::Clear ( void )
{
	if ( m_pEditItem )
	{
		m_pEditItem->OnEndEdit();
		m_pEditItem->Enable ( FALSE );
	}
	SetCurSel (  NULL,TRUE);
	
	int iGroupSize = m_pGroups.size ( );
	for ( int iGroup = iGroupSize; iGroup > 0; iGroup-- )
	{
		int iSubSize = m_pGroups [ iGroup - 1 ].pProperty->GetSubItemsCount ( );
		for ( int iControl = iSubSize; iControl > 0; iControl-- )
		{
			DeleteControl ( iGroup - 1, iControl - 1 );
		}
		DeleteGroup ( iGroup );
	}
	
	AdjustLayout ( );

	m_iGroupCount = 0;

	m_bModified = false;
	m_iGroup	= 0;
	m_iIndex	= 0;

	m_pGroups.clear ( );
}

void CObjectProperty::SetControlState ( int iGroup, int iControl, int iState )
{
	sItem item;

	item.iGroup      = iGroup;
	item.iControl    = iControl;

	
	SendMessage ( WM_ENTITY_SETCONTROLSTATE, iState, ( LPARAM ) &item );
}

LRESULT CObjectProperty::OnSetControlState (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "\n" );
	//theApp.m_Debug.Write ( "OnSetControlState - Begin" );

	sItem*    pItem    = ( sItem* ) lParam;

	//sprintf ( szDebug, "pItem->iGroup - %i", pItem->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	CBCGProp* pProp    = m_pGroups [ pItem->iGroup ].pProperty;

	//sprintf ( szDebug, "pItem->iControl - %i", pItem->iControl );
	//theApp.m_Debug.Write ( szDebug );

	CBCGProp* pControl = pProp->GetSubItem ( pItem->iControl );

	if ( wParam )
	{
		//theApp.m_Debug.Write ( "pControl->Enable" );
		pControl->Enable ( TRUE );
	}
	else
	{
		//theApp.m_Debug.Write ( "pControl->Disable" );
		pControl->Enable ( FALSE );
	}

	return 0;
}

void CObjectProperty::SetControl ( int iGroup, int iControl, int iType, CString string )
{
	sItem edit;

	edit.iGroup      = iGroup;
	edit.iControl    = iControl;
	edit.name        = string;
	edit.contents    = string;
	edit.description = string;

	SendMessage ( WM_ENTITY_SETCONTROL, iType, ( LPARAM ) &edit );
}

LRESULT CObjectProperty::OnSetControl (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "\n" );
	//theApp.m_Debug.Write ( "OnSetControl - Begin" );

	sItem* pEdit = ( sItem* ) lParam;

	//sprintf ( szDebug, "pEdit->iGroup - %i", pEdit->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pEdit->iGroup >= (int)m_pGroups.size ( ) )
	{
		//theApp.m_Debug.Write ( "INVALID GROUP" );
		return 0;
	}

	CBCGProp*  pProp = m_pGroups [ pEdit->iGroup ].pProperty;

	if ( pEdit->iControl >= pProp->GetSubItemsCount ( ) )
		return 0;

	//sprintf ( szDebug, "pEdit->iControl - %i", pEdit->iControl );
	//theApp.m_Debug.Write ( szDebug );

	CBCGProp* pControl = pProp->GetSubItem ( pEdit->iControl );

	if ( wParam == 0 )
	{
		// name
		pControl->m_strName = pEdit->name;

		//sprintf ( szDebug, "pEdit->name - %s", pEdit->name );
		//theApp.m_Debug.Write ( szDebug );
	}
	else if ( wParam == 1 )
	{
		// contents

		pControl->SetValue ( (_variant_t) pEdit->contents );

		//sprintf ( szDebug, "pEdit->contents - %s", pEdit->contents );
		//theApp.m_Debug.Write ( szDebug );
	}
	else if ( wParam == 2 )
	{
		// description
		pControl->SetDesciption ( pEdit->description );

		//sprintf ( szDebug, "pEdit->description - %s", pEdit->description );
		//theApp.m_Debug.Write ( szDebug );
	}
	else
	{
		// failure
		//theApp.m_Debug.Write ( "FAILED TO SET CONTROL" );
	}

	AdjustLayout ( );

	//theApp.m_Debug.Write ( "OnSetControl - End\n" );
	return 0;
}

CString CObjectProperty::GetControl ( int iGroup, int iControl, int iType )
{
	sItem edit;

	edit.iGroup      = iGroup;
	edit.iControl    = iControl;
	edit.name        = _T ( "" );
	edit.contents    = _T ( "" );
	edit.description = _T ( "" );

	
	SendMessage ( WM_ENTITY_GETCONTROL, iType, ( LPARAM ) &edit );

	if ( iType == 0 )
		return edit.name;
	
	if ( iType == 1 )
		return edit.contents;
	
	if ( iType == 2 )
		return edit.description;

	return _T ( "" );
}

LRESULT CObjectProperty::OnGetControl (WPARAM wParam, LPARAM lParam )
{
	

	sItem* pEdit = ( sItem* ) lParam;

	if ( pEdit->iGroup >= (int)m_pGroups.size ( ) )
		return 0;

	CBCGProp*  pProp = m_pGroups [ pEdit->iGroup ].pProperty;

	if ( !pProp )
		return 0;

	if ( pEdit->iControl >= pProp->GetSubItemsCount() )
		return 0;

	CBCGProp* pControl = pProp->GetSubItem ( pEdit->iControl );

	if ( !pControl )
		return 0;

	if ( wParam == 0 )
	{
		// name
		pEdit->name = pControl->m_strName;
	}
	else if ( wParam == 1 )
	{
		CString string = ( LPCTSTR ) ( _bstr_t ) pControl->m_varValue;

		// contents
		pEdit->contents = string;

		if ( pControl->IsKindOf ( RUNTIME_CLASS ( CBCGColorProp ) ) )
		{
			CBCGColorProp* pColor = ( CBCGColorProp* ) pControl;

			COLORREF color = pColor->GetColor ( );

			TCHAR test [ 256 ] = _T("");

			int iRed   = GetRValue ( color );
			int iGreen = GetGValue ( color );
			int iBlue  = GetBValue ( color );

			wsprintf ( test, _T ( "%d %d %d" ),iRed, iGreen, iBlue );

			pEdit->contents = test;
		}
		
		else if ( pControl->IsKindOf ( RUNTIME_CLASS ( CBCGFileProp ) ) )
		{

			CBCGFileProp* pFileEx = ( CBCGFileProp* ) pControl;

			char szContentsANSI [ 255 ]	= "";
			char szDirANSI      [ 255 ]	= "";
			char szExcludeANSI  [ 255 ]	= "";

			CString a = string.SpanIncluding( pControl->m_FPSCDir );

			if ( a.GetLength ( ) >= pControl->m_FPSCDir.GetLength ( ) )
			{


				int a =  pControl->m_FPSCDir.GetLength ( );
				int b =  string.GetLength ( );

				CString strBang = string.Right( b - a );


				ConvertWideToANSI ( NULL, &string,              szContentsANSI );
				ConvertWideToANSI ( NULL, &pControl->m_FPSCDir, szDirANSI );
				ConvertWideToANSI ( NULL, &strBang, szExcludeANSI );

				int c = 0;

				//if ( !strBang.IsEmpty ( ) )
				{
					pControl->SetValue((_variant_t)strBang);
				}


				pEdit->contents = strBang;
				
			}
		}

	}
	else if ( wParam == 2 )
	{
		// description
		pEdit->description = pControl->m_strDescr;
	}
	else
	{
		// failure
	}
	return 0;
}

char* GetANSIFromWide ( CString wide )
{
	static char szTextANSI [ 255 ]	= "";
	ConvertWideToANSI ( NULL, &wide, szTextANSI );

	return szTextANSI;
}		

LRESULT CObjectProperty::OnAddEditBox ( WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnAddEditBox - Begin" );

	sItem* pEdit = ( sItem* ) lParam;

	//sprintf ( szDebug, "pEdit->iGroup - %i", pEdit->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pEdit->iGroup > (int)m_pGroups.size ( ) )
	{
		//theApp.m_Debug.Write ( "INVALID GROUP" );
		return 0;
	}

	CBCGProp* pProp = m_pGroups [ pEdit->iGroup ].pProperty;

	m_pGroups [ pEdit->iGroup ].iType = 0;

	//sprintf ( szDebug, "pEdit->name - %s", GetANSIFromWide ( pEdit->name ) );
	//theApp.m_Debug.Write ( szDebug );
	//sprintf ( szDebug, "pEdit->contents - %s", GetANSIFromWide ( pEdit->contents ) );
	//theApp.m_Debug.Write ( szDebug );
	//sprintf ( szDebug, "pEdit->description - %s", GetANSIFromWide ( pEdit->description ) );
	//theApp.m_Debug.Write ( szDebug );

	pProp->AddSubItem ( new CBCGProp ( pEdit->name, ( _variant_t ) pEdit->contents, pEdit->description ) );

	EnsureVisible ( pProp );

	//theApp.m_Debug.Write ( "OnAddEditBox - End\n" );
	return 0;
}

LRESULT CObjectProperty::OnAddColorPicker (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnAddColorPicker - Begin" );

	sItem* pColor = ( sItem* ) lParam;

	//sprintf ( szDebug, "pColor->iGroup - %i", pColor->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pColor->iGroup > (int)m_pGroups.size ( ) )
	{
		//theApp.m_Debug.Write ( "INVALID GROUP" );
		return 0;
	}

	CBCGProp* pProp = m_pGroups [ pColor->iGroup ].pProperty;

	int iColor [ 3 ];

	{
		char		szTextANSI [ 255 ]	= "";
		ConvertWideToANSI ( NULL, &pColor->contents, szTextANSI );

		char  seps[ ] = " ";
		char* token;

		token = strtok ( szTextANSI, seps );

		int iToken = 0;

		while ( token != NULL )
		{
			iColor [ iToken++ ] = atoi ( token );
			token = strtok ( NULL, seps );
		}
	}
	

	CBCGColorProp* pColorProp = new CBCGColorProp ( pColor->name, RGB ( iColor [ 0 ], iColor [ 1 ], iColor [ 2 ] ), NULL, pColor->description );

	//sprintf ( szDebug, "pColor->description - %s", GetANSIFromWide ( pColor->description ) );
	//theApp.m_Debug.Write ( szDebug );

	m_pGroups [ pColor->iGroup ].iType = 2;

	pProp->AddSubItem ( pColorProp );

	// v105 - 190107 - fix by reverting to standard colour dialog
//	pColorProp->EnableOtherButton ( _T ( "Other..." ) );
	pColorProp->EnableOtherButton ( _T ( "Other..." ), FALSE, TRUE );
	pColorProp->EnableAutomaticButton (_T ( "Default" ), ::GetSysColor ( COLOR_3DFACE ) );

	

	EnsureVisible ( pProp );

	//theApp.m_Debug.Write ( "OnAddColorPicker - End\n" );
	return 0;
}

LRESULT CObjectProperty::OnAddFilePicker (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnAddFilePicker - Begin" );

	sItem* pFile = ( sItem* ) lParam;

	//sprintf ( szDebug, "pFile->iGroup - %i", pFile->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pFile->iGroup > (int)m_pGroups.size ( ) )
	{
		//theApp.m_Debug.Write ( "INVALID GROUP" );
		return 0;
	}

	CBCGProp* pProp = m_pGroups [ pFile->iGroup ].pProperty;

	TCHAR szFilter[] = _T("All Files (*.*)|*.*||");

	CBCGFileProp* pFileEx = new CBCGFileProp (pFile->name, TRUE, pFile->contents, _T("*.*"), 0, szFilter, pFile->description);

	pFileEx->m_Mike_szDirectory = pFile->dir;
	pFileEx->m_Mike_szFilter    = pFile->filter;
	pFileEx->m_Mike_szTitle     = pFile->title;
	pFileEx->m_Mike_Type        = true;
	pFileEx->m_FPSCDir			= pFile->extra;
	
	pProp->AddSubItem ( pFileEx );

	m_pGroups [ pFile->iGroup ].iType = 3;
	EnsureVisible ( pProp );
	
	//theApp.m_Debug.Write ( "OnAddFilePicker - End\n" );
	return 0;
}

LRESULT CObjectProperty::OnAddFontPicker (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnAddFontPicker - Begin" );

	sItem* pFile = ( sItem* ) lParam;

	//sprintf ( szDebug, "pFont->iGroup - %i", pFile->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pFile->iGroup > (int)m_pGroups.size ( ) )
	{
		//theApp.m_Debug.Write ( "INVALID GROUP" );
		return 0;
	}

	CBCGProp* pProp = m_pGroups [ pFile->iGroup ].pProperty;

	LOGFONT lf;
	CFont* font = CFont::FromHandle ((HFONT) GetStockObject (DEFAULT_GUI_FONT));
	font->GetLogFont (&lf);

	//sprintf ( szDebug, "pFont->name - %s", GetANSIFromWide ( pFile->name ) );
	//theApp.m_Debug.Write ( szDebug );
	//sprintf ( szDebug, "pFont->description - %s", GetANSIFromWide ( pFile->description ) );
	//theApp.m_Debug.Write ( szDebug );

	pProp->AddSubItem (new CBCGFontProp ( pFile->name, lf, CF_EFFECTS | CF_SCREENFONTS, pFile->description, 0 ) );

	m_pGroups [ pFile->iGroup ].iType = 4;

	EnsureVisible ( pProp );

	//theApp.m_Debug.Write ( "OnAddFontPicker - End\n" );
	return 0;
}

LRESULT CObjectProperty::OnDeleteControl (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnDeleteControl - Begin" );

	sItem* pItem = ( sItem* ) lParam;

	//sprintf ( szDebug, "pItem->iGroup - %i", pItem->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pItem->iGroup >= (int)m_pGroups.size ( ) )
		return 0;

	CBCGProp* pProp = m_pGroups [ pItem->iGroup ].pProperty;

	if ( pItem->iControl >= pProp->m_lstSubItems.GetCount ( ) )
		return 0;

	int iIndex = pItem->iControl;

	if ( iIndex == 0 )
	{
		pProp->m_lstSubItems.RemoveHead();
	}
	else if ( iIndex == pProp->m_lstSubItems.GetCount ( ) - 1)
	{
		pProp->m_lstSubItems.RemoveTail();
	}
	else
	{
		POSITION pos = pProp->m_lstSubItems.FindIndex ( iIndex );

		pProp->m_lstSubItems.RemoveAt( pos );
	}

	return 0;
}

void CObjectProperty::DeleteControl ( int iGroup, int iControl )
{
	sItem	item;

	item.iGroup    = iGroup;
	item.iControl  = iControl;

	
	SendMessage ( WM_ENTITY_DELETECONTROL, 0, ( LPARAM ) &item );
}

LRESULT CObjectProperty::OnAddListBox (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnAddListBox - Begin" );

	sItem* pEdit = ( sItem* ) lParam;

	//sprintf ( szDebug, "pList->iGroup - %i", pEdit->iGroup );
	//theApp.m_Debug.Write ( szDebug );

	if ( pEdit->iGroup > (int)m_pGroups.size ( ) )
	{
		//theApp.m_Debug.Write ( "INVALID GROUP" );
		return 0;
	}

	CBCGProp* pProp = m_pGroups [ pEdit->iGroup ].pProperty;

	/*
	sprintf ( szDebug, "pFont->name - %s", GetANSIFromWide ( pEdit->name ) );
	theApp.m_Debug.Write ( szDebug );
	sprintf ( szDebug, "pFont->contents - %s", GetANSIFromWide ( pEdit->contents ) );
	theApp.m_Debug.Write ( szDebug );
	sprintf ( szDebug, "pFont->description - %s", GetANSIFromWide ( pEdit->description ) );
	theApp.m_Debug.Write ( szDebug );
	*/

	CBCGProp* pList = new CBCGProp ( pEdit->name, (_variant_t)pEdit->contents, pEdit->description);

	m_pGroups [ pEdit->iGroup ].iType = 1;
	
	pList->AllowEdit (FALSE);

	pProp->AddSubItem (pList);

	EnsureVisible ( pProp );

	//theApp.m_Debug.Write ( "OnAddListBox - End\n" );
	return 0;
}

LRESULT CObjectProperty::OnAddItemToListBox (WPARAM wParam, LPARAM lParam )
{
	//char szDebug [ 255 ] = "";
	//theApp.m_Debug.Write ( "OnAddItemToListBox - Begin" );

	sItem* pList = ( sItem* ) lParam;

	//sprintf ( szDebug, "pList->iGroup - %i", pList->iGroup );
	//theApp.m_Debug.Write ( szDebug );
	
	CBCGProp*  pProp = m_pGroups [ pList->iGroup ].pProperty;

	if ( pList->iControl > pProp->GetSubItemsCount ( ) )
		return 0;

	//sprintf ( szDebug, "pList->iControl - %i", pList->iControl );
	//theApp.m_Debug.Write ( szDebug );

	CBCGProp* pListBox = pProp->GetSubItem ( pList->iControl );

	if ( pListBox )
	{
		//sprintf ( szDebug, "pList->item - %s", GetANSIFromWide ( pList->item ) );
		//theApp.m_Debug.Write ( szDebug );

		pListBox->AddOption (pList->item);
	}
	else
	{
		//theApp.m_Debug.Write ( "INVALID LIST BOX" );
	}

	EnsureVisible ( pProp );

	return 0;
}

void CObjectProperty::AddItemToListBox ( int iGroup, int iControl, CString item )
{
	sItem	list;

	list.iGroup    = iGroup;
	list.iControl  = iControl;
	list.item      = item;

	SendMessage ( WM_ENTITY_ADDITEMTOLISTBOX, 0, ( LPARAM ) &list );
}

LRESULT CObjectProperty::OnClearListBox (WPARAM wParam, LPARAM lParam )
{
	sItem* pList = ( sItem* ) lParam;
	CBCGProp*  pProp = m_pGroups [ pList->iGroup ].pProperty;

	CBCGProp* pListBox = pProp->GetSubItem ( pList->iControl );

	pListBox->RemoveAllOptions ( );

	EnsureVisible ( pProp );
	return 0;
}

void CObjectProperty::ClearListBox ( int iGroup, int iControl )
{
	sItem	list;

	list.iGroup    = iGroup;
	list.iControl  = iControl;

	
	
	SendMessage ( WM_ENTITY_CLEARLISTBOX, 0, ( LPARAM ) &list );
}

void CObjectProperty::AddControl ( int iType, int iGroup, int iControl, CString name, CString contents, CString description, CString extra, CString dir, CString title, CString filter )
{
	m_bRedraw = FALSE;
	m_wndScrollVert.ShowScrollBar( FALSE );
	SetTimer( 1, 1500, NULL );

	sItem control;

	control.iGroup      = iGroup;
	control.iControl    = iControl;
	control.name        = name;
	control.contents    = contents;
	control.description = description;
	control.extra       = extra;

	control.dir = dir;
	control.title = title;
	control.filter = filter;

	//control.iGroup = 0;

	switch ( iType )
	{
		case 0:
			SendMessage ( WM_ENTITY_ADDEDITBOX, 0, ( LPARAM ) &control );
		break;

		case 1:
			SendMessage ( WM_ENTITY_ADDLISTBOX, 0, ( LPARAM ) &control );
		break;

		case 2:
			SendMessage ( WM_ENTITY_ADDCOLORPICKER, 0, ( LPARAM ) &control );
		break;

		case 3:
			SendMessage ( WM_ENTITY_ADDFILEPICKER, 0, ( LPARAM ) &control );
		break;

		case 4:
			SendMessage ( WM_ENTITY_ADDFONTPICKER, 0, ( LPARAM ) &control );
		break;
	}

	int cx = 120;
	int cy = 250;

	m_nDescrHeight = 100;
	
	AdjustLayout ();

	m_nVertScrollOffset = 0;

	ReposProperties ();
	//RedrawWindow ();

	if ( m_iY > 0 )
	{
		
		m_ApplyButton.SetWindowPos (NULL, m_iX / 2 - 90, m_iY - 32, 110, 25, SWP_NOACTIVATE | SWP_NOZORDER);
		m_CancelButton.SetWindowPos (NULL, ( m_iX / 2 - 90 ) + 115, m_iY - 32, 50, 25, SWP_NOACTIVATE | SWP_NOZORDER);

		m_iY = 0;
	}
}

void CObjectProperty::AddGroup ( CString name )
{
	SendMessage ( WM_ENTITY_ADDGROUP, 0, ( LPARAM ) &name );
}

void CObjectProperty::DeleteGroup ( int iIndex )
{
	SendMessage ( WM_ENTITY_DELETEGROUP, 0, ( LPARAM ) iIndex );
}

void CObjectProperty::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CBCGPropList::OnLButtonUp(nFlags, point);
}

void CObjectProperty::OnDrawDesciption (CDC* pDC, CRect rect)
{
	if ( m_iY > 0 )
	{
		
	}

	CBCGPropList::OnDrawDesciption ( pDC, rect );
}


int CObjectProperty::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPropList::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Start ( _T ( "FPSENTITY" ), 1024 * 2, 10 );

	//SetVSDotNetLook ();

	m_pEditItem = NULL;

	strcpy ( szLocation, "" );

	EnableHeaderCtrl     ( FALSE );
	EnableDesciptionArea ( );

	m_bModified = false;
	m_iGroup	= 0;
	m_iIndex	= 0;


	RECT rect;
	rect.top = 400;
	rect.bottom = 450;
	rect.left = 10;
	rect.right = 125;


	m_ApplyButton.Create ( GetLanguageData ( "Property", "Button1" ),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , rect, this, IDOK );
	m_CancelButton.Create ( GetLanguageData ( "Property", "Button2" ),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , rect, this, IDCANCEL );

	//m_ApplyButton.Create ( _T("Apply Changes"),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , rect, this, IDOK );
	//m_CancelButton.Create ( _T("Cancel"),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , rect, this, IDCANCEL );

	//void SetFont( CFont* pFont, BOOL bRedraw = TRUE );

	CFont* pFont = GetFont ( );

	m_ApplyButton.SetFont ( pFont );
	m_CancelButton.SetFont ( pFont );


	m_bApplyButton = false;
	m_bCancelButton = false;

	m_iGroupCount = 0;

	//EnableDesciptionArea ( 0 );

	return 0;
}

void CObjectProperty::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == 1 )
	{
		KillTimer( 1 );
		m_bRedraw = TRUE;
		m_wndScrollVert.ShowScrollBar( TRUE );
		m_wndScrollVert.EnableWindow( TRUE );
		Invalidate( );
		//this->ScrollWindow(0,0);
		//m_wndScrollVert.ShowScrollBar( TRUE );
	}
}

void CObjectProperty::OnDrawList (CDC* pDC)
{
	if ( !m_bRedraw ) return;

	CBCGPropList::OnDrawList( pDC );
}

BOOL CObjectProperty::EndEditItem (BOOL bUpdateData)
{
	CBCGPropList::EndEditItem(bUpdateData);

	
	if ( m_pEditItem )
	{
		bool bFound = false;

		m_bModified = false;
		m_iGroup	= 0;
		m_iIndex	= 0;
		
		for ( m_iGroup = 0; m_iGroup < (int)m_pGroups.size ( ); m_iGroup++ )
		{
			for ( m_iIndex = 0; m_iIndex < m_pGroups [ m_iGroup ].pProperty->GetSubItemsCount ( ); m_iIndex++ )
			{
				if ( m_pEditItem == m_pGroups [ m_iGroup ].pProperty->GetSubItem ( m_iIndex ) )
				{
					bFound = true;
					goto exit;
				}
			}
		}

		exit:
		if ( bFound )
		{
			//m_iGroup--;
			m_bModified = true;
		}
	}
	

	return TRUE;
}

BOOL CObjectProperty::EditItem (CBCGProp* pProp, LPPOINT lptClick)
{
	CBCGPropList::EditItem ( pProp, lptClick );

	m_pEditItem = pProp;

	return TRUE;
}


void CObjectProperty::OnSetFocus(CWnd* pOldWnd) 
{
	CBCGPropList::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	theApp.m_bEntityFocus = true;

	
}

void CObjectProperty::OnKillFocus(CWnd* pNewWnd) 
{
	CBCGPropList::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	//theApp.m_bEntityFocus = false;

}

void CObjectProperty::OnDestroy() 
{
	CBCGPropList::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CObjectProperty::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBCGPropList::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}

void CObjectProperty::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CBCGPropList::OnClose();

	//m_bClose = true;
}

void CObjectProperty::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPropList::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_iX = cx;
	m_iY = cy;
	
	//SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	// 110105
	if ( m_iY > 0 )
	{
		
		//m_ApplyButton.SetWindowPos  (NULL, m_iX / 2 - 90, m_iY - 32, 110, 25, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_CancelButton.SetWindowPos (NULL, ( m_iX / 2 - 90 ) + 115, m_iY - 32, 50, 25, SWP_NOACTIVATE | SWP_NOZORDER);

		m_ApplyButton.SetWindowPos  (NULL, m_iX / 2 - 110, m_iY - 32, 110, 25, SWP_NOACTIVATE | SWP_NOZORDER);
		m_CancelButton.SetWindowPos (NULL, ( m_iX / 2 - 107 ) + 115, m_iY - 32, 100, 25, SWP_NOACTIVATE | SWP_NOZORDER);

		//RedrawWindow ();

		m_iY = 0;
	}
	// 110105 - end

	//m_ApplyButton.SetWindowPos (NULL, m_iX / 2 - 90, m_iY - 32, 110, 25, SWP_NOACTIVATE | SWP_NOZORDER);
	//	m_CancelButton.SetWindowPos (NULL, ( m_iX / 2 - 90 ) + 115, m_iY - 32, 50, 25, SWP_NOACTIVATE | SWP_NOZORDER);
}
