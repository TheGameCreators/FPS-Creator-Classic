
#include "stdafx.h"

#include "ZSplitterDlg.h"

#include <assert.h>

#define WM_Splitter_FIRST 50000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRect ZSplitterDlg::SplitterRect::getRect(CRect& fullRect,int l/*=-1*/,int r/*=-1*/)
{
  if(l<0)
    l = left,r=right;
  CRect rc;
  int  yCenter = getCenter();
  int  yMinus = 1;
  int  yPlus = 3;
  if(m_isHoriz)
  {
    rc.left = l + fullRect.left;
    rc.top  = yCenter - yMinus + fullRect.top;
    rc.right = r + fullRect.left;
    rc.bottom = rc.top + yPlus;
  }
  else
  {
    rc.left = yCenter - yMinus + fullRect.left;
    rc.top  = l + fullRect.top;
    rc.right =  rc.left + yPlus;
    rc.bottom = r + fullRect.top;
  }
  return rc;
}



ZSplitterDlg::ZSplitterDlg(CWnd* dialog) : m_fullRect(0,0,0,0),_empty(0xFF)
{
  m_pixels = 0;
  m_rectsCount = 0;
  m_dialog = dialog;
  m_sizableWnd = 0;
  m_cx = m_cy = 0;
  m_rightID = m_bottomID = 0;
}

void ZSplitterDlg::onDestroy()
{
  for(int i=0;i<(int)m_splitterWnds.size();i++)
  {
    m_splitterWnds[i]->DestroyWindow();
    delete m_splitterWnds[i];
  }
}

void ZSplitterDlg::reinit()
{
  m_fullRect.SetRectEmpty();

  m_SizeableWindows.clear();
  m_sizableWnd = 0;
  //m_cx = m_cy = 0;
  m_bottomID = m_rightID = 0;
  m_rectsCount = 0;

  onDestroy();
  
  m_splitterWnds.clear();
  m_rects.clear();
  m_growingRects.clear();
  
  for(int i=0;i<(int)m_first_init_rects.size();i++)
	  addControls(m_first_init_rects[i]);
  init();
}

void ZSplitterDlg::addControl(int id1)
{
  addControls(1,true,id1);
}

void ZSplitterDlg::addControls(int count,bool h,...)
{
  AddControlCall call(h);

  va_list arg_ptr;
  va_start(arg_ptr, h);
  for(int k=0;k<count;k++)
  {
    int id = va_arg(arg_ptr,int);
	call.m_controls.push_back(id);
  }
  va_end(arg_ptr);
  
  addControls(call);
  
  m_first_init_rects.push_back(call);
}

void ZSplitterDlg::addControls(AddControlCall& call)
{
  SplitterRect rect;
  rect.m_defaultHoriz = call.m_defaultHoriz;

  DWORD st = m_dialog->GetStyle();
  for(int k=0;k<(int)call.m_controls.size();k++)
  {
    int id = call.m_controls[k];

    rect.m_ids.push_back(id);
    CRect rc,rc1,rc2;
    m_dialog->GetClientRect(&rc1);
    m_dialog->GetWindowRect(&rc2);

    m_dialog->GetDlgItem(id)->GetWindowRect(&rc);

    rc.OffsetRect(-rc2.left,-rc2.top); 

    if(st & WS_CAPTION)
      rc.OffsetRect(0,-GetSystemMetrics(SM_CYCAPTION)); 
    if(st & WS_DLGFRAME)
      rc.OffsetRect(-GetSystemMetrics(SM_CYFRAME),-GetSystemMetrics(SM_CYFRAME));
    
    rect.UnionRect(&rect,&rc);
  }

  m_fullRect.UnionRect(&m_fullRect,rect);

  m_rects.push_back(rect);
}

void ZSplitterDlg::fillRect(CRect& rc)
{
  if(!rc.IsRectEmpty())
  {
    for(int j=0;j<=rc.Height();j++)
       memset(m_pixels + ((rc.top - m_fullRect.top) +j)*m_width + (rc.left-m_fullRect.left),m_rectsCount,sizeof(unsigned char)*(rc.Width()+1));
    m_rectsCount++;
  }
}

void ZSplitterDlg::init()
{
  m_countContols = m_rects.size();
  m_widthControls = m_fullRect.Width()+1;
  m_heightControls = m_fullRect.Height()+1;
  m_width =  m_widthControls + 6;
  m_height = m_heightControls + 6;
  int count = m_width*m_height;
  m_pixels = new unsigned char[count];
  memset(m_pixels,_empty,sizeof(unsigned char)*count);
  for(int r=0;r<(int)m_rects.size();r++)
    fillRect(m_rects[r]);

  makeSplitters(&ZSplitterDlg::getYX,m_width,m_height,false);
  makeSplitters(&ZSplitterDlg::getXY,m_height,m_width,true);

  drawSplitters();
  doDrawSplitters();

  delete m_pixels;

  m_sizableWnd = createSplitter();
  int sizableId = WM_Splitter_FIRST+m_rects.size()+1;
  m_sizableWnd->Create(m_dialog, WS_VISIBLE,sizableId);
  CRect rc(m_fullRect.right,m_fullRect.bottom,m_fullRect.right+6,m_fullRect.bottom+6);
  m_sizableWnd->MoveWindow(&rc);
  m_sizableWnd->setStyle(sp_StyleRect);

  for(map<int,int>::iterator win=m_SizeableWindows.begin();win!=m_SizeableWindows.end();win++)
    m_sizableWnd->addWindow(win->first,win->second);
  assert(m_bottomID);
  assert(m_rightID);
  if(m_bottomID)
    m_bottomID->addWindow(sizableId,sp_StyleVert);
  if(m_rightID)
    m_rightID->addWindow(sizableId,sp_StyleHoriz);

  m_splitterWnds.push_back(m_sizableWnd);
}

#pragma warning (disable : 4035)    // no return value
unsigned char InvokeMethod(void* pfunction, void* pthis, int x, int y)
{
  _asm push   y
  _asm push   x
  _asm mov    ecx,pthis
  _asm call   pfunction
}

void ZSplitterDlg::checkLineStop(int x)
{
  if(m_growingRects[m_currentRect].right==-1)
    m_growingRects[m_currentRect].right = x-1;     // found first right border
  else if(m_growingRects[m_currentRect].right != x-1)
  {
    m_growingRects[m_currentRect].m_end = true;         // stop this rect
    m_growingRects.push_back(GrowingRect(m_growingRects[m_currentRect].left,m_growingRects[m_currentRect].bottom));  // make new rect
    m_currentRect = m_growingRects.size() - 1;
  }
}

void ZSplitterDlg::checkRectStop(int y,Call get,bool isHoriz)
{
  for(int g=m_growingRects.size()-1;g>=0;g--)
  {
    if(m_growingRects[g].m_end || m_growingRects[g].bottom!=y)
    {
      if(m_growingRects[g].Width()>10)
      {
        SplitterRect sRect(get,isHoriz);
        sRect.CRect::operator=(m_growingRects[g]);
        sRect.m_ids.push_back(WM_Splitter_FIRST+m_rects.size());
        m_rects.push_back(sRect);
      }
      m_growingRects.erase(m_growingRects.begin() + g);
    }
  }
}
void ZSplitterDlg::makeSplitters(Call get,int height,int width,bool isHoriz)
{
  for(int y=0;y<height;y++)
  {
    bool line = false;
    for(int x=0;x<width;x++)
    {
      if(InvokeMethod(*(void**)&get,this,x,y)!=_empty)
      {
        if(line) // stop line
        {
          line = false;
          checkLineStop(x);
        }
      }
      else if(!line) // begin line
      {
        bool update = false;
        for(int g=0;g<(int)m_growingRects.size();g++)
        {
          if(m_growingRects[g].left == x)
          {
            m_growingRects[g].bottom = y;
            m_currentRect = g;
            update = true;
            break;
          }
        }
        if(!update)
        {
          m_growingRects.push_back(GrowingRect(x,y));
          m_currentRect = m_growingRects.size() - 1;
        }
        line = true;
      }
    }
    if(line)
      checkLineStop(width); // stop right border
    
    checkRectStop(y,get,isHoriz);
  }
  checkRectStop(height,get,isHoriz);
}

void ZSplitterDlg::drawSplitters()
{
  // fill splitters rects
  for(int ii=m_countContols;ii<(int)m_rects.size();ii++)
  {
    assert(m_rects[ii].m_get);
    fillRect(m_rects[ii].getRect(m_fullRect));
  }

  for(int i=m_countContols;i<(int)m_rects.size();i++)
  {
    SplitterRect& rect = m_rects[i];

    // find intersection
    std::set<int> points;
    points.insert(rect.left);
    points.insert(rect.right);

    for(int j=m_countContols;j<(int)m_rects.size();j++)
    {
      if(rect.m_isHoriz!=m_rects[j].m_isHoriz &&
         m_rects[j].getCenter()>=rect.left && 
         m_rects[j].getCenter()<=rect.right)
      {
        points.insert(m_rects[j].getCenter());
      }
    }

    //check segment
    unsigned char result;

    std::set<int>::iterator p1 = points.begin();
    std::set<int>::iterator p2 = points.begin();

    int height = rect.m_isHoriz ? m_height : m_width ;

    for(p2++;p2!=points.end();p1++,p2++)
    {
      for(int xx=*p1;xx<*p2;xx++)
      {
        if(rect.top>0)
        {
          if((result=InvokeMethod(*(void**)&rect.m_get,this,xx,rect.top-1))!=_empty)// before top
          {
            rect.m_windows[result] |= rect.m_isHoriz ? sp_StyleBottom : sp_StyleRight;
            if(result<m_countContols)
            {
              rect.m_newpoints.insert(*p1);
              rect.m_newpoints.insert(*p2);
            }
          }
        }
        if(rect.bottom<height-1)
        {
          if((result=InvokeMethod(*(void**)&rect.m_get,this,xx,rect.bottom+1))!=_empty)// now we next after bottom
          {
            rect.m_windows[result] |= rect.m_isHoriz ? sp_StyleTop : sp_StyleLeft;
            if(result<m_countContols)
            {
              rect.m_newpoints.insert(*p1);
              rect.m_newpoints.insert(*p2);
            }
          }
        }
      }
    }
  }
}

void ZSplitterDlg::doDrawSplitters()
{
  for(int i=m_countContols;i<(int)m_rects.size();i++)
  {
    SplitterRect& rect = m_rects[i];
    if(rect.m_newpoints.size()<2)
      continue;
    CRect rc = rect.getRect(m_fullRect,*rect.m_newpoints.begin(),*rect.m_newpoints.rbegin());

    ZSplitter2* wnd = createSplitter();
    wnd->Create(m_dialog, WS_VISIBLE,rect.m_ids[0]);
    wnd->MoveWindow(&rc);

    wnd->setStyle(rect.m_isHoriz ? sp_StyleVert : sp_StyleHoriz);
    m_splitterWnds.push_back(wnd);

    int sizeableStyle = 0;
    if(rc.top>m_fullRect.bottom)
    {
      sizeableStyle = sp_StyleBottom;
      m_bottomID = wnd;
      m_SizeableWindows[rect.m_ids[0]] |= sp_StyleVert;
    }
    if(rc.left>m_fullRect.right)
    {
      sizeableStyle = sp_StyleRight;
      m_rightID = wnd;
      m_SizeableWindows[rect.m_ids[0]] |= sp_StyleHoriz;
    }
    
    for(map<int,int>::iterator win=rect.m_windows.begin();win!=rect.m_windows.end();win++)
    {
      if(win->first>=m_countContols && (win->second==sp_StyleHoriz || win->second==sp_StyleVert))
        continue;
      for(int w=0;w<(int)m_rects[win->first].m_ids.size();w++)
      {
        int id = m_rects[win->first].m_ids[w];
        if(id<WM_Splitter_FIRST || m_rects[id - WM_Splitter_FIRST].m_newpoints.size()>1 )
        {
          if(w==0 || rect.m_isHoriz==m_rects[win->first].m_defaultHoriz)
          {
            if(sizeableStyle)
              m_SizeableWindows[id] |= sizeableStyle;
            wnd->addWindow(id,win->second);
          }
          else
          {
            int st = !rect.m_isHoriz ? sp_StyleHoriz : sp_StyleVert;
            if(sizeableStyle)
              m_SizeableWindows[id] |= st;
            wnd->addWindow(id,st);
          }
        }
      }
    }
  }
}

void ZSplitterDlg::onSize(UINT nType, int cx, int cy) 
{
  if(m_sizableWnd)
    m_sizableWnd->move(CPoint(0,0),CPoint(cx-m_cx,cy-m_cy));

  m_cx = cx;
  m_cy = cy;
}


