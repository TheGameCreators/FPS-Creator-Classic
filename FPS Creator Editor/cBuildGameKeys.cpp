// cBuildGameKeys.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "cBuildGameKeys.h"
#include "cBuildGameProject.h"

#include <dinput.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cBuildGameKeys::cBuildGameKeys ( cBuildGameProject* pParent ) : CDialog ( cBuildGameKeys::IDD, pParent )
{
	//{{AFX_DATA_INIT(cBuildGameKeys)
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	
}


void cBuildGameKeys::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cBuildGameKeys)
		//DDX_Control(pDX, IDC_EDIT_KEYS, m_Key);

		DDX_Control(pDX, IDC_OK, m_Button);
		DDX_Control(pDX, IDC_STATIC_KEY_SELECT, m_Static);
	//}}AFX_DATA_MAP

		m_Button.ShowWindow ( FALSE );

	SetWindowText ( GetLanguageData ( "Key", "Title" ) );
	m_Static.SetWindowText ( GetLanguageData ( "Key", "Text" ) );
}


BEGIN_MESSAGE_MAP(cBuildGameKeys, CDialog)
	//{{AFX_MSG_MAP(cBuildGameKeys)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cBuildGameKeys message handlers


void cBuildGameKeys::OnOk() 
{
	//EndDialog ( 1 );
}

void cBuildGameKeys::GetKey ( void )
{
	for ( int iKey = 0; iKey < 255; iKey++ )
	{
		if ( ::GetAsyncKeyState ( iKey ) & 0x8000 )
		{
			uiVirtKey = iKey;
		}

	}

	#define BUFFER_LEN 50
	TCHAR szBuffer [BUFFER_LEN + 1];

	ZeroMemory (szBuffer, sizeof (szBuffer));
	
	UINT nScanCode = ::MapVirtualKeyEx (uiVirtKey, 0,  ::GetKeyboardLayout (0)) <<16 | 0x1;
	
	if ( nScanCode > 1 )
	{
		nScanCode=nScanCode;
	}

	if (uiVirtKey >= VK_PRIOR && uiVirtKey <= VK_HELP)
	{
		nScanCode |= 0x01000000;
	}
	
	::GetKeyNameText (nScanCode, szBuffer, BUFFER_LEN);
	
	CString strKey(szBuffer);
	strKey.MakeLower();

	CString str = _T("");

		for (int nCount = 0; nCount < strKey.GetLength(); nCount++)
	{
		TCHAR c = strKey[nCount];
		if (IsCharLower (c))
		{
			c = (TCHAR) toupper (c);
			strKey.SetAt (nCount, c);
			break;
		}
	}

	str += strKey;
	
	if ( !str.IsEmpty ( ) )
	{
		UINT iKey = 0;

		DWORD dwValues [ ] = 

		{
			 65, DIK_A ,
			 66, DIK_B ,
			 67, DIK_C ,
			 68, DIK_D ,
			 69, DIK_E ,
			 70, DIK_F ,
			 71, DIK_G ,
			 72, DIK_H ,
			 73, DIK_I ,
			 74, DIK_J ,
			 75, DIK_K ,
			 76, DIK_L ,
			 77, DIK_M ,
			 78, DIK_N ,
			 79, DIK_O ,
			 80, DIK_P ,
			 81, DIK_Q ,
			 82, DIK_R ,
			 83, DIK_S ,
			 84, DIK_T ,
			 85, DIK_U ,
			 86, DIK_V ,
			 87, DIK_W ,
			 88, DIK_X ,
			 89, DIK_Y ,
			 90, DIK_Z ,
			 32, DIK_SPACE ,
			 28, DIK_RETURN,

			 38, DIK_UP,
			 40, DIK_DOWN,
			 39, DIK_RIGHT,
			 37, DIK_LEFT,

			 160, DIK_LSHIFT,
			 161, DIK_RSHIFT,

			 162, DIK_LCONTROL,
			 163, DIK_RCONTROL,
		};

		int iPos = 1;

		for ( int i = 0; i < 76; i += 2 )
		{
			if ( uiVirtKey == dwValues [ i ] )
			{
				uiVirtKey = dwValues [ iPos ];
				break;
			}

			iPos += 2;
		}

		/*
		// 170105
		TCHAR szTest [ MAX_PATH ] = _T ( "" );

		swprintf ( szTest, _T ( "%i" ), uiVirtKey );
		*/

		CHAR szTest [ MAX_PATH ] = _T ( "" );
		sprintf ( szTest, _T ( "%i" ), uiVirtKey );
		///////////

		m_Key = str;
		
		//if ( !m_Key.IsEmpty ( ) )
		EndDialog ( 1 );
	}
	else
	{
		// 010205
		m_Key = "Enter";
		uiVirtKey = 28;
	}
}

void cBuildGameKeys::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);


	GetKey ( );

	//m_Key.SetWindowText ( str );
}

int cBuildGameKeys::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Key = _T( "" );

	uiVirtKey = 0;
	//uiVirtKey = 28;

	UINT iTimer = SetTimer ( 1, 1, NULL );
	
	// TODO: Add your specialized creation code here

	
	
	return 0;
}

void cBuildGameKeys::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);

	uiVirtKey = nChar;
}

void cBuildGameKeys::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void cBuildGameKeys::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	
	CDialog::OnClose();
}

void cBuildGameKeys::OnDestroy() 
{
	
	if ( m_Key.IsEmpty ( ) )
	{
		m_Key = _T("Enter");
	}

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
