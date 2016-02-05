
#ifndef _NestorSplitter_h_
#define _NestorSplitter_h_

#include <vector>
using namespace std;


#define WM_MOVE_ZSPLITTER WM_USER + 110

enum SplitterStyle
{
    sp_StyleNone    = 0x00000,
    sp_StyleTop     = 0x00001,
    sp_StyleBottom  = 0x00002,
    sp_StyleLeft    = 0x00004,
    sp_StyleRight   = 0x00008,
    sp_StyleHoriz   = sp_StyleLeft|sp_StyleRight,
    sp_StyleVert    = sp_StyleTop|sp_StyleBottom,
    sp_StyleRect    = sp_StyleVert|sp_StyleHoriz,
    sp_StyleSplit   = 0x0000F,
    sp_StyleHorizSplit = sp_StyleSplit | sp_StyleHoriz,
    sp_StyleVertSplit  = sp_StyleSplit | sp_StyleVert,
};

class ZSplitter : public CWnd
{
  public:
    ZSplitter();
    ~ZSplitter();

  public:
    void    addWindow(int idx,int style);
    void    setStyle(int style) {ownStyle=style;}
    void    move(CPoint,CPoint);
  public:

  // --------------------------------------------------------- implementation
  protected:
    CPoint                    m_pointDragStart;
    bool                      m_bInDragTestNow;

    afx_msg void OnMouseMove( UINT nFlags, CPoint point );
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
    afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnRButtonUp( UINT nFlags, CPoint point );

    BOOL  OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message );

    void releaseCapture();
    bool countRect( CRect& rect, int idx,CPoint dif);

    vector<int>             idxs;
    vector<int>             styles;
    int                     ownStyle;

  DECLARE_DYNAMIC(ZSplitter)
	DECLARE_MESSAGE_MAP()
};

class ZSplitter2 : public ZSplitter
{
 public:
  typedef ZSplitter inherited;

  BOOL Create( CWnd* pParentWnd, DWORD dwStyle,UINT nID)
  {
    CREATESTRUCT cs;  memset(&cs, 0, sizeof(cs));
    cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(0,IDC_ARROW));
    cs.style = dwStyle;
    cs.hInstance = AfxGetInstanceHandle();
    cs.hwndParent = pParentWnd->GetSafeHwnd();
    return inherited::Create( cs.lpszClass, cs.lpszName, cs.style, CRect(0,0,0,0), pParentWnd,nID);
  }
  virtual void OnPaint()
  {
    CPaintDC  dc(this);
    CRect rc; 
    GetClientRect(&rc);
    dc.FillSolidRect(&rc,GetSysColor(COLOR_WINDOW));
    //dc.Draw3dRect(&rc, RGB(255, 0, 0), RGB(0, 255, 0));
  }
  DECLARE_MESSAGE_MAP()
};


#endif // _NestorSplitter_h_
