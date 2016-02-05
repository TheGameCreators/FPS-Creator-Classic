
#include "stdafx.h"

#include "ZSplitter.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZSplitter class
       
IMPLEMENT_DYNAMIC(ZSplitter,CWnd)
BEGIN_MESSAGE_MAP(ZSplitter,CWnd)
  ON_WM_SETCURSOR()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONDOWN()
  ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

ZSplitter::ZSplitter() : m_bInDragTestNow(false), ownStyle(sp_StyleNone)
{
}
ZSplitter::~ZSplitter()
{
}

void ZSplitter::addWindow(int idx,int style)
{
  idxs.push_back(idx);
  styles.push_back(style);
}

void ZSplitter::OnLButtonDown( UINT nFlags, CPoint point )
{
  // run default processing
  CWnd::OnLButtonDown( nFlags, point );

  // initiate drag check
  m_pointDragStart = point;
  m_bInDragTestNow = true;
  SetCapture();
}

void ZSplitter::OnMouseMove( UINT nFlags, CPoint point )
{
  if ( m_bInDragTestNow )         
  {
    move(point,point - m_pointDragStart);
  }
  else CWnd::OnMouseMove( nFlags, point );
}

void ZSplitter::move(CPoint point,CPoint dif)
{
  if(!(ownStyle&sp_StyleRight) ) dif.x = min(dif.x,0);
  if(!(ownStyle&sp_StyleLeft) )  dif.x = max(dif.x,0);
  if(!(ownStyle&sp_StyleBottom) )dif.y = max(dif.y,0);
  if(!(ownStyle&sp_StyleTop) )   dif.y = min(dif.y,0);

  CRect rc,parent;
  GetWindowRect(&rc);
  GetParent()->GetClientRect(&parent);
  GetParent()->ScreenToClient(&rc);
  int left = rc.left+dif.x;
  int top = rc.top+dif.y;
  bool canAll = true;  CRect rect;
  for(int i=0; i<(int)idxs.size();i++)
    if (!countRect(rect,i,dif))
      canAll = false;
  if(canAll && left>0 && left<parent.right && top>0 && top<parent.bottom )
  {
    for(int i=0; i<(int)idxs.size();i++)
    {
      countRect(rect,i,dif);
      CWnd *wnd = GetParent()->GetDlgItem(idxs[i]);
      wnd->MoveWindow(&rect);
      GetParent()->PostMessage(WM_MOVE_ZSPLITTER,0,(long)wnd->m_hWnd);
      wnd->UpdateWindow();
    }
    SetWindowPos(0,left,top,0,0,SWP_NOSIZE|SWP_NOZORDER);
    m_pointDragStart = point-dif;
  }
}

bool ZSplitter::countRect( CRect& rc, int i,CPoint dif)
{
  CWnd *wnd = GetParent()->GetDlgItem(idxs[i]);
  wnd->GetWindowRect(&rc);
  GetParent()->ScreenToClient(&rc);
  if(styles[i] & sp_StyleLeft)
    rc.left += dif.x;
  if(styles[i] & sp_StyleRight)
    rc.right += dif.x;
  if(styles[i] & sp_StyleTop)
    rc.top += dif.y;
  if(styles[i] & sp_StyleBottom)
    rc.bottom += dif.y;
  bool canmove = (rc.bottom>rc.top && rc.left<rc.right);
  return canmove;
}

void ZSplitter::OnLButtonUp( UINT nFlags, CPoint point )
{
  releaseCapture();
  CWnd::OnLButtonUp( nFlags, point );
}

void ZSplitter::releaseCapture()
{
  if ( m_bInDragTestNow )
  {                                     // drag was not initiated
    m_bInDragTestNow = false;
    ReleaseCapture();
  }
}

void ZSplitter::OnRButtonDown( UINT nFlags, CPoint point )
{
  releaseCapture();
  CWnd::OnRButtonDown( nFlags, point );
}

void ZSplitter::OnRButtonUp( UINT nFlags, CPoint point )
{
  releaseCapture();
  CWnd::OnLButtonUp( nFlags, point );
}

BOOL ZSplitter::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
  if ( pWnd==this && nHitTest==HTCLIENT )
  {
    static int _afx_idcPrimaryLast = 0;
    static HCURSOR _afx_hcurLast = NULL;
    static HCURSOR  _afx_hcurDestroy = 0;
    int idcPrimary = 0;
    if(ownStyle==sp_StyleRect)
    {
			idcPrimary = AFX_IDC_SMALLARROWS;
    }
    else if(ownStyle==sp_StyleHoriz)
    {
			idcPrimary = AFX_IDC_HSPLITBAR;
    }
    else if(ownStyle==sp_StyleVert)
    {
			idcPrimary = AFX_IDC_VSPLITBAR;
    }

    if (idcPrimary != 0)
    {
      HCURSOR hcurToDestroy = NULL;
      if (idcPrimary != _afx_idcPrimaryLast)
      {
        HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(idcPrimary), RT_GROUP_CURSOR);
        
        // load in another cursor
        hcurToDestroy = _afx_hcurDestroy;
        
        // Note: If this LoadCursor call fails, it is likely that
        //  _AFX_NO_SPLITTER_RESOURCES is defined in your .RC file.
        // To correct the situation, remove the following line from your
        //  resource script:
        //      #define _AFX_NO_SPLITTER_RESOURCES
        // This should be done using the Resource.Set Includes... command.
        
        if ((_afx_hcurDestroy = _afx_hcurLast =
          ::LoadCursor(hInst, MAKEINTRESOURCE(idcPrimary))) == NULL)
        {
          TRACE0("Warning: Could not find splitter cursor - using system provided alternative.\n");
          return CWnd::OnSetCursor(pWnd,nHitTest,message);
        }
        _afx_idcPrimaryLast = idcPrimary;
      }
      ASSERT(_afx_hcurLast != NULL);
      ::SetCursor(_afx_hcurLast);
      ASSERT(_afx_hcurLast != hcurToDestroy);
      if (hcurToDestroy != NULL)
        ::DestroyCursor(hcurToDestroy); // destroy after being set
    }
    return true;
  }
  return CWnd::OnSetCursor(pWnd,nHitTest,message);
}


BEGIN_MESSAGE_MAP(ZSplitter2, ZSplitter)
  ON_WM_PAINT()
END_MESSAGE_MAP()
