
#ifndef _NestorSplitterDlg_h_
#define _NestorSplitterDlg_h_

#include "zsplitter.h"

#include <vector>
#include <set>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// GrowingRect

class ZSplitterDlg
{
public:
  ZSplitterDlg(CWnd* dialog);

  void init();

  void addControls(int count,bool,...);
  void addControl(int id1);
  
  void onDestroy();

  void onSize(UINT nType, int cx, int cy);

protected:
  void reinit();
  
  virtual ZSplitter2* createSplitter() {return new ZSplitter2;}

private:
  typedef unsigned char (ZSplitterDlg::*Call)(int,int);
  const unsigned char _empty;

  struct GrowingRect : public CRect
  {
    GrowingRect(int x,int y) : m_end(false) {left=x;right=-1;top=y;bottom=y;}
    bool m_end;
  };

  /////////////////////////////////////////////////////////////////////////////
  // SplitterRect

  struct SplitterRect : public CRect
  {
    SplitterRect(Call get=NULL,bool isHoriz=false)
      : CRect(0,0,0,0),m_get(get),m_isHoriz(isHoriz){}

    int getCenter() { return (bottom+top)/2;}

    CRect getRect(CRect& fullRect,int l=-1,int r=-1);

    bool          m_defaultHoriz;
    vector<int>   m_ids;

    // only add splitters
    Call          m_get;
    bool          m_isHoriz;
    std::set<int> m_newpoints;
    map<int,int/*SplitterStyle*/> m_windows;

  };


  struct AddControlCall
  {
    AddControlCall(bool h) : m_defaultHoriz(h) {}
	bool m_defaultHoriz;
	vector<int>   m_controls;

  };

  void addControls(AddControlCall& call);

  vector<ZSplitter2*>     m_splitterWnds;
  vector<SplitterRect>    m_rects;
  vector<GrowingRect>     m_growingRects;
  vector<AddControlCall>  m_first_init_rects;

  void fillRect(CRect& rc);

  void makeSplitters(Call get,int height,int width,bool isHoriz);
  void checkLineStop(int x);
  void checkRectStop(int y,Call get,bool isHoriz);
  void drawSplitters();
  void doDrawSplitters();

  unsigned char getXY(int x,int y) {return m_pixels[y*m_width + x];}
  unsigned char getYX(int y,int x) {return m_pixels[y*m_width + x];}
  void fillSpace(int x,int y);

  CRect m_fullRect;
  unsigned char* m_pixels;   
  int   m_width,m_height;
  int   m_widthControls,m_heightControls;
  int   m_rectsCount;
  int   m_currentRect;
  int   m_countContols;
  CWnd* m_dialog;

  map<int,int/*SplitterStyle*/> m_SizeableWindows;
  ZSplitter2*                   m_bottomID;
  ZSplitter2*                   m_rightID;
  ZSplitter2*                   m_sizableWnd;
  int                           m_cx,m_cy;

};

#ifdef _AFXDLL
#define BEGIN_T_MESSAGE_MAP(prefix,theClass, baseClass) \
	prefix const AFX_MSGMAP* PASCAL theClass::_GetBaseMessageMap() \
		{ return &baseClass::messageMap; } \
	prefix const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return &theClass::messageMap; } \
	prefix AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
	{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
	prefix AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
	{ \

#else
#define BEGIN_T_MESSAGE_MAP(prefix,theClass, baseClass) \
	prefix const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return &theClass::messageMap; } \
	prefix AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
	{ &baseClass::messageMap, &theClass::_messageEntries[0] }; \
	prefix AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
	{ \

#endif


template<class T,class SPLITTER = ZSplitter2>
class ZSplitterDlgImpl : public T, protected  ZSplitterDlg
{
 protected:
  ZSplitterDlgImpl() : ZSplitterDlg(this) {}
  ZSplitterDlgImpl(UINT nIDTemplate, CWnd* pParentWnd = NULL) 
    : T(nIDTemplate), ZSplitterDlg(this) {}
  ZSplitterDlgImpl(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL) 
    : T(lpszTemplateName), ZSplitterDlg(this) {}

  void OnDestroy() {  onDestroy();}
  void OnSize(UINT nType, int cx, int cy) 
  {
	  T::OnSize(nType, cx, cy);
    onSize(nType,cx,cy);
  }

  virtual ZSplitter2* createSplitter() {return new SPLITTER;}

  DECLARE_MESSAGE_MAP()
};

#define ZSplitterDlgImpl_T_SPLITTER ZSplitterDlgImpl<T,S>
#define template_T_S template<class T,class S>
 
// .NET does not like this 0 - what does it do anyhoo?
//BEGIN_T_MESSAGE_MAP(template_T_S,ZSplitterDlgImpl_T_SPLITTER,T)
//	ON_WM_DESTROY()
//  ON_WM_SIZE()
//END_MESSAGE_MAP()


#endif // _NestorSplitterDlg_h_
