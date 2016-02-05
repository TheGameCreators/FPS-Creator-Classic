/////////////////////////////////////////////////////////////////////
// Template: TTCExpand
// Purpose : Template to expand or collapse all items of a CTreeCtrl
//				derived object
// Created : SEN (1- Jun - 2001)
// History : None
// 
// Copyright (c) 2001 by Francisco José Sen del Prado
// All rights reserved.
/////////////////////////////////////////////////////////////////////


// TCExpand.h : header file
//

#if !defined(_TCEXPAND_H__INCLUDED_)
#define _TCEXPAND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
	#pragma message ("To avoid this warning message place #include <afxtempl.h> in your stdafx.h precompiler headers file")
	#include <afxtempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// TTCExpand (Declaration)

template <class T>
class TTCExpand : public T
{
// Operations
public:
	void ExpandItem(HTREEITEM hti= TVI_ROOT, UINT nCode= TVE_EXPAND);
	CString GetItemPathI ( HTREEITEM hItem );
};


/////////////////////////////////////////////////////////////////////////////
// TTCExpand (Implementation)

template <class T>
void TTCExpand<T>::ExpandItem(HTREEITEM hti, UINT nCode)
{
	bool bOk= true;

	bOk= Expand(hti, nCode) != 0;

	
	if (bOk)
	{
		HTREEITEM htiChild= GetChildItem(hti);

		

		while (htiChild != NULL)
		{
			bool bValid = true;

			CString b = this->GetItemText ( htiChild );
			CString path = GetItemPathI( htiChild );
		
			if ( theApp.m_TreeGroups.GetExpanded( path ) == 0 ) bValid = false;
			//if ( b == _T("scenery") )
			//	bValid = false;

			if ( bValid ) ExpandItem(htiChild, nCode);

			htiChild= GetNextSiblingItem(htiChild);
		}
	}
}

template <class T>
CString TTCExpand<T>::GetItemPathI ( HTREEITEM hItem ) 
{
	CString sRet;

	do
	{
		NETRESOURCE* const pNetResource = ( NETRESOURCE* ) ( this->GetItemData ( hItem ) );
		
		if ( pNetResource )
		{
			sRet = CString ( pNetResource->lpRemoteName ) + _T( '\\' ) + sRet; 		
			break;
		}
		
		sRet  = this->GetItemText   ( hItem ) + _T ( '\\' ) + sRet; 
		hItem = this->GetParentItem ( hItem );
	}
	while ( hItem );

	return sRet;
}

#endif // !defined(_TCEXPAND_H__INCLUDED_)
