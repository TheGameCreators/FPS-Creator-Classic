#if !defined(AFX_OBJECTCOLOR_H__7B88BA06_94E7_46A1_8E51_4A7946B2DA0E__INCLUDED_)
#define AFX_OBJECTCOLOR_H__7B88BA06_94E7_46A1_8E51_4A7946B2DA0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectProperty.h : header file
//

#include "ObjectColor.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty

class CObjectColor : public CBCGColorProp
{
	//DECLARE_DYNAMIC(CObjectColor)

// Construction
public:
	

// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectColor)
	virtual void OnDrawValue (CDC* pDC, CRect rect);
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CObjectColor();

	
	
	// Generated message map functions
protected:
	
	//{{AFX_MSG(CObjectColor)
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTCOLOR_H__7B88BA06_94E7_46A1_8E51_4A7946B2DA0E__INCLUDED_)
