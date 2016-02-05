#include "stdafx.h"
#include "TabPageSSL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction

CTabPageSSL::CTabPageSSL () {
#ifndef _AFX_NO_OCC_SUPPORT
	AfxEnableControlContainer ();
#endif // !_AFX_NO_OCC_SUPPORT
	m_bRouteCommand = true;
	m_bRouteCmdMsg = true;
	m_bRouteNotify = true;
}

CTabPageSSL::CTabPageSSL (UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent) {
#ifndef _AFX_NO_OCC_SUPPORT
	AfxEnableControlContainer ();
#endif // !_AFX_NO_OCC_SUPPORT
	//m_bRouteCommand = false;
	//m_bRouteCmdMsg = false;
	//m_bRouteNotify = false;

	m_bRouteCommand = true;
	m_bRouteCmdMsg = true;
	m_bRouteNotify = true;
}

/////////////////////////////////////////////////////////////////////////////
// Destruction

CTabPageSSL::~CTabPageSSL () {
}

/////////////////////////////////////////////////////////////////////////////
// Message Handlers

void CTabPageSSL::OnOK (void) {
	//
	// Prevent CDialog::OnOK from calling EndDialog.
	//
}

void CTabPageSSL::OnCancel (void) {
	//
	// Prevent CDialog::OnCancel from calling EndDialog.
	//
}

BOOL CTabPageSSL::OnCommand (WPARAM wParam, LPARAM lParam) {
	// Call base class OnCommand to allow message map processing
	BOOL bReturn = CDialog::OnCommand (wParam, lParam);

	if (true == m_bRouteCommand)
	{
		//
		// Forward WM_COMMAND messages to the dialog's parent.
		//
		return GetParent ()->SendMessage (WM_COMMAND, wParam, lParam);
	}

	return bReturn;
}

BOOL CTabPageSSL::OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult)
 {
	BOOL bReturn = CDialog::OnNotify(wParam, lParam, pResult);

	if (true == m_bRouteNotify)
	{
		//
		// Forward WM_NOTIFY messages to the dialog's parent.
		//

		return GetParent ()->SendMessage (WM_NOTIFY, wParam, lParam);
	}

	return bReturn;
}

BOOL CTabPageSSL::OnCmdMsg (UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo) {
	BOOL bReturn = CDialog::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);

#ifndef _AFX_NO_OCC_SUPPORT
	if (true == m_bRouteCmdMsg)
	{
		//
		// Forward ActiveX control events to the dialog's parent.
		//
		if (nCode == CN_EVENT)
			return GetParent ()->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
	}
#endif // !_AFX_NO_OCC_SUPPORT

	return bReturn;
}

BOOL CTabPageSSL::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	// mouse wheel
	//theApp.SetFileMapData ( 16, zDelta );
	
	//return CView::OnMouseWheel(nFlags, zDelta, pt);

	return TRUE;
}