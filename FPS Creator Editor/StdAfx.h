// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AAD08EA3_DEB3_43DA_AAFC_51CEE872B27C__INCLUDED_)
#define AFX_STDAFX_H__AAD08EA3_DEB3_43DA_AAFC_51CEE872B27C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)
#pragma warning (disable : 4355)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
// 170105
//#define _UNICODE

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <bcgcb.h>			// BCG Control Bar

#include "Global.h"



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AAD08EA3_DEB3_43DA_AAFC_51CEE872B27C__INCLUDED_)


// FOR UNICODE
// add this to project settings->link->output->entry point symbol
//wWinMainCRTStartup