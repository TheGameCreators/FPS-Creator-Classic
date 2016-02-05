#if !defined(AFX_TABPAGESSL_H__619331B3_7DE7_4DB1_A039_2103E87E8E71__INCLUDED_)
#define AFX_TABPAGESSL_H__619331B3_7DE7_4DB1_A039_2103E87E8E71__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CTabPageSSL declaration

class CTabPageSSL : public CDialog
{
public:
// Construction
	CTabPageSSL ();	// Default Constructor
	CTabPageSSL (UINT nIDTemplate, CWnd* pParent = NULL);	// Standard Constructor
// Destruction
	~CTabPageSSL ();

	// Enable/disable command routing to the parent.
	void EnableRouteCommand(bool bRoute = true) { m_bRouteCommand = bRoute; };
	bool IsRouteCommand() { return m_bRouteCommand; };
	// Enable CmdMsg routing to the parent.
	void EnableRouteCmdMsg(bool bRoute = true) { m_bRouteCmdMsg = bRoute; };
	bool IsRouteCmdMsg() { return m_bRouteCmdMsg; };
	// Enable/Disable Notify routing to the parent.
	void EnableRouteNotify(bool bRoute = true) { m_bRouteNotify = bRoute; };
	bool IsRouteNotify() { return m_bRouteNotify; };

protected:
// Message Handlers

	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnOK (void);
	virtual void OnCancel (void);
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

// Routing flags
	bool m_bRouteCommand;
	bool m_bRouteCmdMsg;
	bool m_bRouteNotify;
};

#endif // !defined(AFX_TABPAGE_H__619331B3_7DE7_4DB1_A039_2103E87E8E71__INCLUDED_)
