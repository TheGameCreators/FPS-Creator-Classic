// ZSplitterDlgEx.h
// Designed by Mike Melnikov 

#ifndef _NestorSplitterDlgEx_h_
#define _NestorSplitterDlgEx_h_

#include "ZSplitterDlg.h"

#include "ResizableDialog.h"

class ZSplitterDlgResizable : public ZSplitterDlgImpl<CResizableDialog>
{
 protected:
	ZSplitterDlgResizable()
    : inherited() {}
	ZSplitterDlgResizable(UINT nIDTemplate, CWnd* pParentWnd = NULL)
    : inherited(nIDTemplate,pParentWnd){}
  ZSplitterDlgResizable(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
    : inherited(lpszTemplateName,pParentWnd) {}

  void ArrangeLayout()
  {
    inherited::ArrangeLayout();
    reinit();
  }

  LONG OnMoveElement(UINT , LONG awnd)
  {
    HWND wnd = (HWND)awnd;
    ReAddAnchor(wnd);
    return 0;
  }

  DECLARE_MESSAGE_MAP()
  typedef ZSplitterDlgImpl<CResizableDialog> inherited;
};



#endif // _NestorSplitterDlgEx_h_
