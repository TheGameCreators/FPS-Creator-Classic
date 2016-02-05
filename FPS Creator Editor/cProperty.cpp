// cProperty.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cProperty

#define ENTITY_SETUP							 0
//#define ENTITY_WINDOW_TITLE						 4

#define ENTITY_ADD_GROUP						 8
#define	ENTITY_DELETE_GROUP						12
#define	ENTITY_SET_GROUP_INDEX					16
#define ENTITY_SET_CONTROL_INDEX				20
#define ENTITY_DELETE_CONTROL					24

#define ENTITY_ADD_EDIT_BOX						28
#define ENTITY_ADD_LIST_BOX						32
#define ENTITY_ADD_COLOR_PICKER					36
#define ENTITY_ADD_FILE_PICKER					40
#define ENTITY_ADD_FONT_PICKER					44

#define ENTITY_SET_CONTROL_NAME					48
#define ENTITY_SET_CONTROL_CONTENTS				52
#define ENTITY_SET_CONTROL_DESCRIPTION			56
#define ENTITY_SET_CONTROL_STATE				60
#define ENTITY_GET_CONTROL_NAME					64
#define ENTITY_GET_CONTROL_CONTENTS				68
#define ENTITY_GET_CONTROL_DESCRIPTION			72

#define ENTITY_ADD_ITEM_TO_LIST_BOX				76
#define ENTITY_CLEAR_LIST_BOX					80

#define ENTITY_CONTROL_MODIFIED					92
#define ENTITY_CONTROL_MODIFIED_GROUP			96
#define ENTITY_CONTROL_MODIFIED_INDEX			100
#define ENTITY_CONTROL_MODIFIED_RESET			104

#define ENTITY_BUILD							108

#define	STRING_A								1000
#define	STRING_B								1256
#define	STRING_C								1512
#define	STRING_D								1768

#define	STRING_E								2024
#define	STRING_F								2280
#define	STRING_G								2536

/////////////////
struct sPropertyMessage
{
	int		iMessage;
	int		iGroup;
	int		iControl;

	CString	A;
	CString	B;
	CString	C;
	CString	D;
	CString	E;
	CString	F;
	CString	G;
};

vector < sPropertyMessage > g_MessageList;
/////////////////

cProperty::cProperty(CObjectProperty* pProperty)
{
	m_pPropertyWindow = pProperty;

	if ( m_pPropertyWindow )
	{
		m_pPropertyWindow->m_bApplyButton = false;
		m_pPropertyWindow->m_bCancelButton = false;
	}

	m_dwGroupIndex = 0;
	m_dwControlIndex = 0;
}

cProperty::~cProperty()
{
}


BEGIN_MESSAGE_MAP(cProperty, CWnd)
	//{{AFX_MSG_MAP(cProperty)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cProperty message handlers

void cProperty::ResetAll ( void )
{
	// so buttons reset
	if ( m_pIPC )
	{
		DWORD dwNReady	= 0;
		m_pIPC->SendBuffer ( &dwNReady, 112, sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwNReady, 116, sizeof ( DWORD ) );
	}

	// restart property box
	m_pPropertyWindow->Clear();
}

void cProperty::HandleMessages ( cIPC* pIPC, cMessages* pMessage )
{
	// check if ptrs valid
	if ( !pIPC ) return;

	// skip if mapeditor not waiting for interface
	if ( !pIPC->SkipIfEventNotWaiting () )
		return;

	m_pIPC = pIPC;
	m_pMessages = pMessage;

	//
	m_pMessages->Stop ( );

	DWORD dwReady	= 1;
	m_pIPC->SendBuffer ( &dwReady, ENTITY_SETUP, sizeof ( DWORD ) );

	// Update other systems	
	UpdateGroups		( );
	UpdateControls		( );
	UpdateWindow		( );
	CheckForNewControls ( );
	UpdateListBoxes		( );

	// if property window active
	if ( m_pPropertyWindow )
	{
		if ( m_pPropertyWindow->m_bApplyButton )
		{
			m_pPropertyWindow->m_bApplyButton = false;
			m_pIPC->SendBuffer ( &dwReady, 112, sizeof ( DWORD ) );
		}
		if ( m_pPropertyWindow->m_bCancelButton )
		{
			m_pPropertyWindow->m_bCancelButton = false;
			m_pIPC->SendBuffer ( &dwReady, 116, sizeof ( DWORD ) );
		}
	}
	
	// only when update completely done, signal that event complete
	m_pMessages->Restart ( );
	pIPC->AllowSyncEventNow();
}


void cProperty::UpdateWindow ( void )
{
	
}

void cProperty::UpdateSelection ( void )
{
	m_pIPC->ReceiveBuffer ( &m_dwGroupIndex,   ENTITY_SET_GROUP_INDEX,   sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &m_dwControlIndex, ENTITY_SET_CONTROL_INDEX, sizeof ( DWORD ) );
}

void cProperty::UpdateControls ( void )
{
	DWORD		dwDeleteControl				= 999;
	DWORD		dwSetControlName			= 0;
	DWORD		dwSetControlContents		= 0;
	DWORD		dwSetControlDescription		= 0;
	DWORD		dwGetControlName			= 0;
	DWORD		dwGetControlContents		= 0;
	DWORD		dwGetControlDescription		= 0;
	DWORD		dwSetControlState			= 999;

	char		szControlNameANSI			[ 255 ] = "";
	char		szControlContentsANSI		[ 255 ] = "";
	char		szControlDescriptionANSI	[ 255 ] = "";
	char		szControlStringANSI			[ 255 ] = "";
	char		szTemporaryANSI				[ 255 ] = "";
	
	CString		szControlNameUNI;
	CString		szControlContentsUNI;
	CString		szControlDescriptionUNI;
	CString		szControlStringUNI;

	m_pIPC->ReceiveBuffer ( &dwDeleteControl,			ENTITY_DELETE_CONTROL,				sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwSetControlName,			ENTITY_SET_CONTROL_NAME,			sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwSetControlContents,		ENTITY_SET_CONTROL_CONTENTS,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwSetControlDescription,   ENTITY_SET_CONTROL_DESCRIPTION,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwGetControlName,			ENTITY_GET_CONTROL_NAME,			sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwGetControlContents,		ENTITY_GET_CONTROL_CONTENTS,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwGetControlDescription,   ENTITY_GET_CONTROL_DESCRIPTION,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwSetControlState,			ENTITY_SET_CONTROL_STATE,			sizeof ( DWORD ) );

	/*
	memcpy ( &dwDeleteControl,			&m_pMessageData [ ENTITY_DELETE_CONTROL           ], sizeof ( DWORD ) );
	memcpy ( &dwSetControlName,			&m_pMessageData [ ENTITY_SET_CONTROL_NAME         ], sizeof ( DWORD ) );
	memcpy ( &dwSetControlContents,		&m_pMessageData [ ENTITY_SET_CONTROL_CONTENTS     ], sizeof ( DWORD ) );
	memcpy ( &dwSetControlDescription,	&m_pMessageData [ ENTITY_SET_CONTROL_DESCRIPTION  ], sizeof ( DWORD ) );
	memcpy ( &dwGetControlName,			&m_pMessageData [ ENTITY_GET_CONTROL_NAME         ], sizeof ( DWORD ) );
	memcpy ( &dwGetControlContents,		&m_pMessageData [ ENTITY_GET_CONTROL_CONTENTS     ], sizeof ( DWORD ) );
	memcpy ( &dwGetControlDescription,	&m_pMessageData [ ENTITY_GET_CONTROL_DESCRIPTION  ], sizeof ( DWORD ) );
	memcpy ( &dwSetControlState,	    &m_pMessageData [ ENTITY_SET_CONTROL_STATE        ], sizeof ( DWORD ) );
	*/

	UpdateSelection ( );

	if ( dwDeleteControl == 1 )
	{
		//m_pMessages->Stop ( );

		m_pPropertyWindow->DeleteControl ( m_dwGroupIndex, m_dwControlIndex );

		/////////////
		DWORD dwValue = 0;

		m_pIPC->SendBuffer ( &dwValue, ENTITY_DELETE_CONTROL, sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_DELETE_CONTROL ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pMessages->Restart ( );
	}

	if ( dwSetControlName || dwSetControlContents || dwSetControlDescription )
	{
		//m_pMessages->Stop ( );

		m_pIPC->ReceiveBuffer ( &szControlStringANSI [ 0 ], STRING_A, sizeof ( char ) * 255 );
		szControlStringUNI = szControlStringANSI;

		int iValue = 0;
		
		if ( dwSetControlName )
			iValue = 0;

		if ( dwSetControlContents )
			iValue = 1;

		if ( dwSetControlDescription )
			iValue = 2;

		m_pPropertyWindow->SetControl ( m_dwGroupIndex, m_dwControlIndex, iValue, szControlStringUNI );

		/////////////
		DWORD dwValue = 0;

		m_pIPC->SendBuffer ( &dwValue, ENTITY_SET_CONTROL_NAME,			sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_SET_CONTROL_CONTENTS,		sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_SET_CONTROL_DESCRIPTION,	sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_SET_CONTROL_NAME ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_SET_CONTROL_CONTENTS ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_SET_CONTROL_DESCRIPTION ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pMessages->Restart ( );
	}

	if ( dwGetControlName || dwGetControlContents || dwGetControlDescription )
	{
		//m_pMessages->Stop ( );

		int iValue = 0;
		
		if ( dwGetControlName )
			iValue = 0;

		if ( dwGetControlContents )
			iValue = 1;

		if ( dwGetControlDescription )
			iValue = 2;

		CString string = m_pPropertyWindow->GetControl ( m_dwGroupIndex, m_dwControlIndex, iValue );

		/*
		// 170105
		{
			int  iLen     = wcslen ( string );
			int  nResult = WideCharToMultiByte ( CP_ACP, 0, string, -1, szTemporaryANSI, iLen * sizeof ( CHAR ), NULL, NULL );
			szTemporaryANSI [ iLen ] = 0;
		}
		*/
		strcpy ( szTemporaryANSI, string );

		if ( iValue == 0 )
		{
			m_pIPC->SendBuffer ( &szTemporaryANSI [ 0 ], STRING_A, sizeof ( char ) * 255 );

			//memcpy ( &m_pMessageData [ STRING_A ], &szTemporaryANSI [ 0 ], sizeof ( char ) * 255 );
		}

		if ( iValue == 1 )
		{
			m_pIPC->SendBuffer ( &szTemporaryANSI [ 0 ], STRING_B, sizeof ( char ) * 255 );

			//memcpy ( &m_pMessageData [ STRING_B ], &szTemporaryANSI [ 0 ], sizeof ( char ) * 255 );
		}

		if ( iValue == 2 )
		{
			m_pIPC->SendBuffer ( &szTemporaryANSI [ 0 ], STRING_C, sizeof ( char ) * 255 );

			//memcpy ( &m_pMessageData [ STRING_C ], &szTemporaryANSI [ 0 ], sizeof ( char ) * 255 );
		}

		/////////////
		DWORD dwValue = 0;

		m_pIPC->SendBuffer ( &dwValue, ENTITY_GET_CONTROL_NAME,			sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_GET_CONTROL_CONTENTS,		sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_GET_CONTROL_DESCRIPTION,	sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_GET_CONTROL_NAME ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_GET_CONTROL_CONTENTS ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_GET_CONTROL_DESCRIPTION ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pMessages->Restart ( );
	}

	if ( dwSetControlState != 0 )
	{
		//m_pMessages->Stop ( );

		int iValue = 0;

		if ( dwSetControlState == 1 )
			iValue = 1;

		if ( dwSetControlState == 2 )
			iValue = 0;

		m_pPropertyWindow->SetControlState ( m_dwGroupIndex, m_dwControlIndex, iValue );

		/////////////
		DWORD dwValue = 0;

		m_pIPC->SendBuffer ( &dwValue, ENTITY_SET_CONTROL_STATE, sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_SET_CONTROL_STATE ], &dwValue, sizeof ( DWORD ) );
		/////////////

		//m_pMessages->Restart ( );
	}
}

void cProperty::UpdateGroups ( void )
{
	DWORD		dwAddGroup			    = 999;
	DWORD		dwDeleteGroup		    = 0;
	char		szGroupNameANSI	[ 255 ] = "";
	CString		szGroupNameUNI;
	
	m_pIPC->ReceiveBuffer ( &dwAddGroup,	ENTITY_ADD_GROUP,	 sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwDeleteGroup,	ENTITY_DELETE_GROUP, sizeof ( DWORD ) );

	//memcpy ( &dwAddGroup,	 &m_pMessageData [ ENTITY_ADD_GROUP    ], sizeof ( DWORD ) );
	//memcpy ( &dwDeleteGroup, &m_pMessageData [ ENTITY_DELETE_GROUP ], sizeof ( DWORD ) );

	if ( dwAddGroup == 1 )
	{
		m_pIPC->ReceiveBuffer ( &szGroupNameANSI [ 0 ], STRING_A, sizeof ( char ) * 255 );

		//memcpy ( &szGroupNameANSI [ 0 ], &m_pMessageData [ STRING_A ], sizeof ( char ) * 255 );
		szGroupNameUNI = szGroupNameANSI;

		/*
		/////////////////////
		{
			sPropertyMessage message;

			message.iMessage = ENTITY_ADD_GROUP;
			message.A        = szGroupNameUNI;

			g_MessageList.push_back ( message );
		}
		/////////////////////
		*/

		m_pPropertyWindow->AddGroup ( szGroupNameUNI );

		DWORD dwValue = 0;

		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_GROUP, sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_ADD_GROUP ], &dwValue, sizeof ( DWORD ) );
	}

	if ( dwDeleteGroup > 0 )
	{
		m_pPropertyWindow->DeleteGroup ( dwDeleteGroup );

		DWORD dwValue = 0;

		m_pIPC->SendBuffer ( &dwValue, ENTITY_DELETE_GROUP, sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_DELETE_GROUP ], &dwValue, sizeof ( DWORD ) );
	}

	/*
	/////////////////////
	
	DWORD dwBuild = 0;
	memcpy ( &dwBuild, &m_pMessageData [ ENTITY_BUILD ], sizeof ( DWORD ) );

	if ( dwBuild )
	{
		for ( int i = 0; i < g_MessageList.size ( ); i++ )
		{
			switch ( g_MessageList [ i ].iMessage )
			{
				case ENTITY_ADD_GROUP:
				{
					m_pPropertyWindow->AddGroup ( g_MessageList [ i ].A );
				}
				break;

				default:
				break;
			}
		}

		dwBuild = 0;
		memcpy ( &m_pMessageData [ ENTITY_BUILD ], &dwBuild, sizeof ( DWORD ) );
	}
	/////////////////////
	*/
}

void cProperty::CheckForNewControls ( void )
{
	DWORD		dwAddEditBox						= 999;
	DWORD		dwAddListBox						= 999;
	DWORD		dwAddColorPicker					= 999;
	DWORD		dwAddFilePicker						= 999;
	DWORD		dwAddFontPicker						= 999;

	int			iControl							= -1;

	char		szControlNameANSI			[ 255 ] = "";
	char		szControlContentsANSI		[ 255 ] = "";
	char		szControlDescriptionANSI	[ 255 ] = "";
	char		szControlExtraANSI			[ 255 ] = "";

	char		szDir			[ 255 ] = "";
	char		szFilter		[ 255 ] = "";
	char		szTitle			[ 255 ] = "";

	CString		szControlNameUNI;
	CString		szControlContentsUNI;
	CString		szControlDescriptionUNI;
	CString		szControlExtraUNI;

	CString		dir;
	CString		filter;
	CString		title;

	m_pIPC->ReceiveBuffer ( &dwAddEditBox,		ENTITY_ADD_EDIT_BOX,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwAddListBox,		ENTITY_ADD_LIST_BOX,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwAddColorPicker,	ENTITY_ADD_COLOR_PICKER,	sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwAddFilePicker,	ENTITY_ADD_FILE_PICKER,		sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwAddFontPicker,	ENTITY_ADD_FONT_PICKER,		sizeof ( DWORD ) );

	//memcpy ( &dwAddEditBox,		&m_pMessageData [ ENTITY_ADD_EDIT_BOX     ], sizeof ( DWORD ) );
	//memcpy ( &dwAddListBox,		&m_pMessageData [ ENTITY_ADD_LIST_BOX     ], sizeof ( DWORD ) );
	//memcpy ( &dwAddColorPicker,	&m_pMessageData [ ENTITY_ADD_COLOR_PICKER ], sizeof ( DWORD ) );
	//memcpy ( &dwAddFilePicker,	&m_pMessageData [ ENTITY_ADD_FILE_PICKER  ], sizeof ( DWORD ) );
	//memcpy ( &dwAddFontPicker,	&m_pMessageData [ ENTITY_ADD_FONT_PICKER  ], sizeof ( DWORD ) );

	m_pIPC->ReceiveBuffer ( &szControlNameANSI		  [ 0 ], STRING_A, sizeof ( char ) * 255 );
	m_pIPC->ReceiveBuffer ( &szControlContentsANSI	  [ 0 ], STRING_B, sizeof ( char ) * 255 );
	m_pIPC->ReceiveBuffer ( &szControlDescriptionANSI [ 0 ], STRING_C, sizeof ( char ) * 255 );
	m_pIPC->ReceiveBuffer ( &szControlExtraANSI		  [ 0 ], STRING_D, sizeof ( char ) * 255 );

	//memcpy ( &szControlNameANSI        [ 0 ], &m_pMessageData [ STRING_A ], sizeof ( char ) * 255 );
	//memcpy ( &szControlContentsANSI    [ 0 ], &m_pMessageData [ STRING_B ], sizeof ( char ) * 255 );
	//memcpy ( &szControlDescriptionANSI [ 0 ], &m_pMessageData [ STRING_C ], sizeof ( char ) * 255 );
	//memcpy ( &szControlExtraANSI       [ 0 ], &m_pMessageData [ STRING_D ], sizeof ( char ) * 255 );

	m_pIPC->ReceiveBuffer ( &szDir		[ 0 ], STRING_E, sizeof ( char ) * 255 );
	m_pIPC->ReceiveBuffer ( &szFilter	[ 0 ], STRING_F, sizeof ( char ) * 255 );
	m_pIPC->ReceiveBuffer ( &szTitle	[ 0 ], STRING_G, sizeof ( char ) * 255 );

	//memcpy ( &szDir    [ 0 ], &m_pMessageData [ STRING_E ], sizeof ( char ) * 255 );
	//memcpy ( &szFilter [ 0 ], &m_pMessageData [ STRING_F ], sizeof ( char ) * 255 );
	//memcpy ( &szTitle  [ 0 ], &m_pMessageData [ STRING_G ], sizeof ( char ) * 255 );

	dir    = szDir;
	filter = szFilter;
	title  = szTitle;

	szControlNameUNI        = szControlNameANSI;
	szControlContentsUNI    = szControlContentsANSI;
	szControlDescriptionUNI = szControlDescriptionANSI;
	szControlExtraUNI		= szControlExtraANSI;

	UpdateSelection ( );

	if ( dwAddEditBox == 1 )
	{
		//theApp.m_Debug.Write ( "cProperty::CheckForNewControls::dwAddEditBox" );
		iControl = 0;
	}

	if ( dwAddListBox == 1 )
	{
		//theApp.m_Debug.Write ( "cProperty::CheckForNewControls::dwAddListBox" );
		iControl = 1;
	}

	if ( dwAddColorPicker == 1 )
	{
		//theApp.m_Debug.Write ( "cProperty::CheckForNewControls::dwAddColorPicker" );
		iControl = 2;
	}

	if ( dwAddFilePicker == 1 )
	{
		//theApp.m_Debug.Write ( "cProperty::CheckForNewControls::dwAddFilePicker" );
		iControl = 3;
	}

	if ( dwAddFontPicker == 1 )
	{
		//theApp.m_Debug.Write ( "cProperty::CheckForNewControls::dwAddFontPicker" );
		iControl = 4;
	}

	if ( iControl >= 0 )
	{

		char szDebug [ 255 ] = "";
		

		//m_pMessages->Stop ( );

		CScrollBar* pScroll = m_pPropertyWindow->GetScrollBarCtrl ( SB_VERT );
		
		m_pPropertyWindow->AddControl ( 
											iControl,
											m_dwGroupIndex,
											m_dwControlIndex,
											szControlNameUNI,
											szControlContentsUNI,
											szControlDescriptionUNI,
											szControlExtraUNI,
											dir,
											filter,
											title
										);

		

		pScroll->SetScrollPos ( 0, TRUE );

		UpdateWindow ( );

		m_pPropertyWindow->SendMessage ( WM_PAINT, 0, 0 );

		//m_pMessages->Restart ( );

		/////////////
		DWORD dwValue = 0;
		//memcpy ( &m_pMessageData [ ENTITY_SET_GROUP_INDEX ], &dwValue, sizeof ( DWORD ) );


		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_EDIT_BOX,		sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_LIST_BOX,		sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_COLOR_PICKER, sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_FILE_PICKER,	sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_FONT_PICKER,	sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_ADD_EDIT_BOX ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_ADD_LIST_BOX ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_ADD_COLOR_PICKER ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_ADD_FILE_PICKER ], &dwValue, sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_ADD_FONT_PICKER ], &dwValue, sizeof ( DWORD ) );
		/////////////
	}

	

	{
		DWORD dwClear = 0;
		DWORD dwEnd = 0;

		m_pIPC->ReceiveBuffer ( &dwClear, ENTITY_CONTROL_MODIFIED_RESET, sizeof ( DWORD ) );

		//memcpy ( &dwClear, &m_pMessageData [ ENTITY_CONTROL_MODIFIED_RESET ], sizeof ( DWORD ) );

		if ( dwClear )
		{
			m_pPropertyWindow->m_bModified = false;

			m_pIPC->SendBuffer ( &dwEnd, ENTITY_CONTROL_MODIFIED,		sizeof ( DWORD ) );
			m_pIPC->SendBuffer ( &dwEnd, ENTITY_CONTROL_MODIFIED_RESET,	sizeof ( DWORD ) );

			//memcpy ( &m_pMessageData [ ENTITY_CONTROL_MODIFIED ], &dwEnd, sizeof ( DWORD ) );
			//memcpy ( &m_pMessageData [ ENTITY_CONTROL_MODIFIED_RESET ], &dwEnd, sizeof ( DWORD ) );
		}
	}

	if ( m_pPropertyWindow->m_bModified )
	{
		DWORD dwModified = 1;
		DWORD dwGroup    = m_pPropertyWindow->m_iGroup;
		DWORD dwIndex    = m_pPropertyWindow->m_iIndex;

		m_pIPC->SendBuffer ( &dwModified,	ENTITY_CONTROL_MODIFIED,		sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwGroup,		ENTITY_CONTROL_MODIFIED_GROUP,	sizeof ( DWORD ) );
		m_pIPC->SendBuffer ( &dwIndex,		ENTITY_CONTROL_MODIFIED_INDEX,	sizeof ( DWORD ) );

		//memcpy ( &m_pMessageData [ ENTITY_CONTROL_MODIFIED ], &dwModified,	sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_CONTROL_MODIFIED_GROUP ], &dwGroup,		sizeof ( DWORD ) );
		//memcpy ( &m_pMessageData [ ENTITY_CONTROL_MODIFIED_INDEX ], &dwIndex,		sizeof ( DWORD ) );
	}
	
}


void cProperty::UpdateListBoxes ( void )
{
	DWORD		dwAddItemToListBox				= 999;
	DWORD		dwClearListBox					= 999;
	char		szListBoxItemANSI	[ 255 ]		= "";
	CString		szListBoxItemUNI;
	
	m_pIPC->ReceiveBuffer ( &dwAddItemToListBox,	ENTITY_ADD_ITEM_TO_LIST_BOX,	sizeof ( DWORD ) );
	m_pIPC->ReceiveBuffer ( &dwClearListBox,		ENTITY_CLEAR_LIST_BOX,			sizeof ( DWORD ) );

	UpdateSelection ( );

	if ( dwAddItemToListBox == 1 )
	{
		m_pMessages->Stop ( );

		m_pIPC->ReceiveBuffer ( &szListBoxItemANSI [ 0 ], STRING_A, sizeof ( char ) * 255 );
		szListBoxItemUNI = szListBoxItemANSI;
		m_pPropertyWindow->AddItemToListBox ( m_dwGroupIndex, m_dwControlIndex, szListBoxItemUNI );

		DWORD dwValue = 0;
		m_pIPC->SendBuffer ( &dwValue, ENTITY_ADD_ITEM_TO_LIST_BOX,	sizeof ( DWORD ) );

		m_pMessages->Restart ( );
	}
	
	if ( dwClearListBox == 1 )
	{
		m_pMessages->Stop ( );

		m_dwGroupIndex = 0;
		m_dwControlIndex = 0;

		m_pPropertyWindow->ClearListBox ( m_dwGroupIndex, m_dwControlIndex );

		DWORD dwValue = 0;
		m_pIPC->SendBuffer ( &dwValue, ENTITY_CLEAR_LIST_BOX,	sizeof ( DWORD ) );

		m_pMessages->Restart ( );
	}
}