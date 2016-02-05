#if !defined(AFX_CCONTROLS_H__C0F83DAC_908F_440A_B385_9D7DEE2D0C2A__INCLUDED_)
#define AFX_CCONTROLS_H__C0F83DAC_908F_440A_B385_9D7DEE2D0C2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class cControls
{
	public:
		cControls  ( );
		~cControls ( );

		void AddList		( CListBox*		pList,      int iOffset );
		void AddEdit		( CEdit*		pEdit,      int iOffset );
		void AddButton		( CButton*		pButton,    int iOffset );
		void AddRadioButton	( CButton*		pButton,    int iOffset );
		void AddCombo		( CComboBoxEx*	pCombo,     int iOffset );
		void AddProgressBar ( CProgressCtrl* pProgress, int iOffset );

		void RemoveCombo	( int iOffset );

		void SetButton      ( int iOffset );
		void SetRadio       ( int iOffset, bool bState );
		//void Update         ( BYTE* pData );
		void Update         ( cIPC* pIPC );

		static int CALLBACK BrowseCallbackProc ( HWND hwnd, UINT uMsg, LPARAM , LPARAM lpData );

	public:
		CListBox*		m_pList      [ 100 ];
		CEdit*			m_pEdit      [ 100 ];
		CButton*		m_pButton    [ 100 ];
		CButton*		m_pRadio     [ 100 ];
		CComboBoxEx*	m_pCombo     [ 100 ];
		CProgressCtrl*	m_pProgress  [ 100 ];
		
		bool			m_bButtonClicked [ 100 ];
		bool			m_bRadioClicked  [ 100 ];
		int				m_iComboSize     [ 100 ];
		int				m_iListCount;
		int				m_iEditCount;
		int				m_iButtonCount;
		int				m_iComboCount;

		/*
		void UpdateEditBoxes    ( BYTE* pData );
		void UpdateButtons      ( BYTE* pData );
		void UpdateBrowse	    ( BYTE* pData );
		void UpdateFolderSelect ( BYTE* pData );
		void UpdateListControls ( BYTE* pData );
		void UpdateComboBoxes   ( BYTE* pData );
		void UpdateRadioButtons ( BYTE* pData );
		void UpdateProgress     ( BYTE* pData );
		*/

		void UpdateEditBoxes    ( cIPC* pIPC );
		void UpdateButtons      ( cIPC* pIPC );
		void UpdateBrowse	    ( cIPC* pIPC );
		void UpdateFolderSelect ( cIPC* pIPC );
		void UpdateListControls ( cIPC* pIPC );
		void UpdateComboBoxes   ( cIPC* pIPC );
		void UpdateRadioButtons ( cIPC* pIPC );
		void UpdateProgress     ( cIPC* pIPC );
};


#endif // !defined(AFX_CCONTROLS_H__C0F83DAC_908F_440A_B385_9D7DEE2D0C2A__INCLUDED_)