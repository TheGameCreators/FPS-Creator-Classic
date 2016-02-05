// ZSplitterDlg.cpp
// Designed by Mike Melnikov  zmike.net

#include "stdafx.h"

#include "ZSplitterDlgEx.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ZSplitterDlgResizable,ZSplitterDlgResizable::inherited)
  ON_MESSAGE (WM_MOVE_ZSPLITTER, OnMoveElement)
END_MESSAGE_MAP()
