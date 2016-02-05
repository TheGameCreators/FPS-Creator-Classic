
#include "stdafx.h"
#include "Editor.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL ( CEditorCntrItem, COleClientItem, 0 )

CEditorCntrItem::CEditorCntrItem ( CEditorDoc* pContainer ) : COleClientItem ( pContainer )
{
	
}

CEditorCntrItem::~CEditorCntrItem ( )
{
	
}

void CEditorCntrItem::OnChange ( OLE_NOTIFICATION nCode, DWORD dwParam )
{
	ASSERT_VALID ( this );

	COleClientItem::OnChange ( nCode, dwParam );

	GetDocument()->UpdateAllViews ( NULL );
}

BOOL CEditorCntrItem::OnChangeItemPosition ( const CRect& rectPos )
{
	ASSERT_VALID ( this );

	if ( !COleClientItem::OnChangeItemPosition ( rectPos ) )
		return FALSE;

	return TRUE;
}

void CEditorCntrItem::OnGetItemPosition ( CRect& rPosition )
{
	ASSERT_VALID ( this );
}

void CEditorCntrItem::OnActivate ( )
{
    CEditorView* pView = GetActiveView ( );

    ASSERT_VALID ( pView );

    COleClientItem* pItem = GetDocument ( )->GetInPlaceActiveItem ( pView );

    if ( pItem != NULL && pItem != this )
        pItem->Close ( );

	theApp.m_pEditorView = pView;
    
    COleClientItem::OnActivate ( );
}

void CEditorCntrItem::OnDeactivateUI ( BOOL bUndoable )
{
	COleClientItem::OnDeactivateUI ( bUndoable );

    DWORD dwMisc = 0;

    m_lpObject->GetMiscStatus ( GetDrawAspect ( ), &dwMisc );

    if ( dwMisc & OLEMISC_INSIDEOUT )
        DoVerb ( OLEIVERB_HIDE, NULL );
}

void CEditorCntrItem::Serialize ( CArchive& ar )
{
	ASSERT_VALID ( this );

	COleClientItem::Serialize ( ar );

	if ( ar.IsStoring ( ) )
	{
		
	}
	else
	{
		
	}
}

#ifdef _DEBUG

	void CEditorCntrItem::AssertValid ( ) const
	{
		COleClientItem::AssertValid ( );
	}

	void CEditorCntrItem::Dump ( CDumpContext& dc ) const
	{
		COleClientItem::Dump ( dc );
	}

#endif

/////////////////////////////////////////////////////////////////////////////
