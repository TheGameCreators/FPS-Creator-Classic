
#if !defined(AFX_CNTRITEM_H__803A36AF_D62E_4771_83D8_0C467F28B2C7__INCLUDED_)
#define AFX_CNTRITEM_H__803A36AF_D62E_4771_83D8_0C467F28B2C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CEditorDoc;
class CEditorView;

class CEditorCntrItem : public COleClientItem
{
	DECLARE_SERIAL ( CEditorCntrItem )

	public:
		CEditorCntrItem ( CEditorDoc* pContainer = NULL );
		
	public:
		CEditorDoc*  GetDocument   ( ) { return ( CEditorDoc*  ) COleClientItem::GetDocument   ( ); }
		CEditorView* GetActiveView ( ) { return ( CEditorView* ) COleClientItem::GetActiveView ( ); }

	//{{AFX_VIRTUAL(CEditorCntrItem)
	public:
		virtual void OnChange ( OLE_NOTIFICATION wNotification, DWORD dwParam );
		virtual void OnActivate ( );

	protected:
		virtual void OnGetItemPosition    ( CRect& rPosition );
		virtual void OnDeactivateUI       ( BOOL bUndoable );
		virtual BOOL OnChangeItemPosition ( const CRect& rectPos );
	//}}AFX_VIRTUAL

	public:
		~CEditorCntrItem ( );

	#ifdef _DEBUG

		virtual void AssertValid ( ) const;
		virtual void Dump        ( CDumpContext& dc ) const;

	#endif

	virtual void Serialize ( CArchive& ar );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__803A36AF_D62E_4771_83D8_0C467F28B2C7__INCLUDED_)
