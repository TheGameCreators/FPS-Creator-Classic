// EditorView.h : interface of the CEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITORVIEW_H__6593D728_E901_4D20_9E3B_F25CC21CCB64__INCLUDED_)
#define AFX_EDITORVIEW_H__6593D728_E901_4D20_9E3B_F25CC21CCB64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditorCntrItem;

class CEditorView : public CView
{
	protected: // create from serialization only
		CEditorView();
		DECLARE_DYNCREATE(CEditorView)

	// Attributes
	public:
		CEditorDoc* GetDocument();
		// m_pSelection holds the selection to the current CEditorCntrItem.
		// For many applications, such a member variable isn't adequate to
		//  represent a selection, such as a multiple selection or a selection
		//  of objects that are not CEditorCntrItem objects.  This selection
		//  mechanism is provided just to help you get started.

		// TODO: replace this selection mechanism with one appropriate to your app.
		CEditorCntrItem* m_pSelection;

		POINT m_lastPoint;

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEditorView)
	public:
		virtual void OnDraw(CDC* pDC);  // overridden to draw this view
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual void OnEnterIdle( UINT, CWnd* );
	protected:
		virtual void OnInitialUpdate(); // called first time after construct
		virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
		virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CEditorView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	protected:

	// Generated message map functions
	protected:
		//{{AFX_MSG(CEditorView)
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnDestroy();
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnInsertObject();
		afx_msg void OnCancelEditCntr();
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnPaint();
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnNcActivate( WPARAM w, LPARAM l );//BOOL bActive );
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
		afx_msg void OnContextMenu(CWnd*, CPoint point);
		afx_msg void OnFilePrintPreview();

		void OnMouseEnter ( UINT nFlags, CPoint point );
		void OnMouseHover ( UINT nFlags, CPoint point );
		void OnMouseLeave ( );

		void MouseMoveProcessEHL ( UINT nFlags, CPoint point );
		BOOL m_bMouseInside;
		BOOL m_bMouseHover;


		 afx_msg LRESULT OnMouseLeaveSystem ( WPARAM wParam, LPARAM lParam ) ;
		 afx_msg LRESULT OnMouseHoverSystem ( WPARAM wParam, LPARAM lParam ) ;

		 
		
		DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EditorView.cpp
inline CEditorDoc* CEditorView::GetDocument()
   { return (CEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORVIEW_H__6593D728_E901_4D20_9E3B_F25CC21CCB64__INCLUDED_)
