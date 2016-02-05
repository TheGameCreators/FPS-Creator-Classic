// cMessages.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cMessages::cMessages ( )
{
	// constructor for the class

	// set all member variables to default values
	m_bEnd           = false;
	m_szName         = _T ( "" );
	m_dwElapse       = 0;
	m_pBase          = NULL;
	m_bValid		 = false;
	m_bMessageUsed	 = false;
}

cMessages::~cMessages ( )
{
	// destructor for the class
	if ( m_bMessageUsed ) End ( );
}

BEGIN_MESSAGE_MAP(cMessages, CWnd)
	//{{AFX_MSG_MAP(cMessages)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void cMessages::Start ( CString name, DWORD dwSize, DWORD dwElapse, cBaseMessage* pBase )
{

	// this function starts off the whole process, the parameters are as follows -
	//
	//		* name     - name of the file map to create
	//		* dwSize   - size of memory to allocate
	//		* dwElapse - time to wait between calling update
	//		* pBase    - the class that is using these functions
	//				   - it must implement the "HandleMessages" function

	// we must create a dummy window because we have inherited from the CWnd class, we need
	// to do this so we can implement timers within the message map
	if ( !CreateEx ( 0, AfxRegisterWndClass ( 0, 0 ), NULL, 0, 0, 0, 0, 0, NULL, NULL ) )
	{
		// failed
	}

	// check the pointer to the base class first
	if ( !pBase )
	{
		// failed
	}

	// store the parameters into our member variables
	m_szName         = name;
	m_dwElapse		 = dwElapse;
	m_pBase          = pBase;

	strcpy ( m_szNameChar, name );

	///////////////////

	TCHAR szName [ 256 ] = "";

	strcpy ( szName, m_szName );

	m_pIPC = new cIPC ( ( char* ) szName, dwSize );

	UINT iTimer = SetTimer ( 1, dwElapse, NULL );

	m_dwSize = dwSize;

	m_bMessageUsed = true;
}

void cMessages::End ( void )
{
	// this function can be called to finish off and kill
	// the timer to stop processing messages

	// set end flag to true
	m_bEnd = true;

	// kill the timer
	KillTimer ( 1 );
	
	// set all member variables to default values
	m_bEnd           = false;
	m_szName         = _T ( "" );
	m_dwElapse       = 0;
	m_pBase          = NULL;
	m_bValid		 = false;
}

BYTE* cMessages::GetData ( void )
{
	// return a pointer to the data contained within
	// the file map
	// if not return null
	return NULL;
}

cIPC* cMessages::GetIPC  ( void )
{
	return m_pIPC;
}

void cMessages::OnTimer ( UINT nIDEvent )
{
	// timer event, called automatically by windows when a timer
	// event is set by the call to "SetTimer"

	// see if we need to stop
	if ( m_bEnd )
	{
		End ( );	// call the end function
		return;		// and return from the function
	}

	// call the virtual function
	m_pBase->HandleMessages ( );
	
	// default timer implementation
	CWnd::OnTimer ( nIDEvent );
}

void cMessages::SetSpeed ( DWORD dwSpeed )
{
	// set the speed of the timer

	// store the speed in the member variable
	m_dwElapse = dwSpeed;

	// kill the current timer
	KillTimer ( 1 );

	// set the new timer
	UINT iTimer = SetTimer ( 1, m_dwElapse, NULL );

	// check timer is valid
	if ( !iTimer )
	{
		// failed
	}
}

void cMessages::Stop ( void )
{
	KillTimer ( 1 );
}

void cMessages::Restart ( void )
{
	if ( !m_bEnd )
		UINT iTimer = SetTimer ( 1, m_dwElapse, NULL );
}