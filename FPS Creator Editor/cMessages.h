
#if !defined(AFX_CMESSAGES_H__AE1780FB_543E_4622_969B_152D53AA9160__INCLUDED_)
#define AFX_CMESSAGES_H__AE1780FB_543E_4622_969B_152D53AA9160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif


#include "IPC.h"

class cBaseMessage
{
	// this is a base class, any classes that want to use the messaging
	// system must inherit from this and then override the "HandleMessages"
	// function with their own implementation

	public:
		virtual	void HandleMessages ( void ) = 0;
};

class cMessages : public CWnd
{
	// the implementation of messages, the class that requires access to
	// messages must declare a member variable of this type, then call
	// the "Start" function to set things up, then using "HandleMessages"
	// it can access the data required from the DB Pro application

	public:
		 cMessages ( );
		~cMessages ( );

	public:
		//HANDLE				m_hFileMapCreate;
		bool				m_bEnd;

		//HANDLE				m_hFileMap;
		//LPVOID				m_lpVoid;
		//BYTE*				m_pMessageData;

		CString				m_szName;
		char				m_szNameChar [ 256 ];

		DWORD				m_dwElapse;
		cBaseMessage*		m_pBase;
		DWORD				m_dwSize;

		bool				m_bValid;

		bool				m_bMessageUsed;

	private:
		cIPC*				m_pIPC;

	public:
		void  Start    ( CString name, DWORD dwSize, DWORD dwElapse, cBaseMessage* pBase );
		void  End      ( void );
		BYTE* GetData  ( void );

		cIPC* GetIPC  ( void );
		

		void  SetSpeed ( DWORD dwSpeed );

		void  Stop     ( void );
		void  Restart ( void );

	protected:
		afx_msg void OnTimer ( UINT nIDEvent );
		DECLARE_MESSAGE_MAP ( )
};

#endif // !defined(AFX_CMESSAGES_H__AE1780FB_543E_4622_969B_152D53AA9160__INCLUDED_)
