/////////////////////////////////////////////////////////////////////
// Template: TTCIterator
// Purpose : Template to iterate all the elements of a CTreeCtrl
//				derived object
// Created : SEN (1- Jun - 2001)
// History : None
// 
// Copyright (c) 2001 by Francisco José Sen del Prado
// All rights reserved.
/////////////////////////////////////////////////////////////////////


// TCIterator.h : header file
//

#if !defined(_TCITERATOR_H__INCLUDED_)
#define _TCITERATOR_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
	#pragma message ("To avoid this warning message place #include <afxtempl.h> in your stdafx.h precompiler headers file")
	#include <afxtempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// TTCIterator (Declaration)

template <class T>
class TTCIterator : public T
{
public:
// Construction
	TTCIterator();

// Operations
public:
	void Initialize(bool bDeepIteration= true);

	HTREEITEM GetNext();

// Implementation
private:
	bool m_bDeepIteration;

	CList<HTREEITEM, HTREEITEM> m_hlTail;
};


/////////////////////////////////////////////////////////////////////////////
// TTCIterator (Implementation)

template <class T>
TTCIterator<T>::TTCIterator()
{
	m_bDeepIteration= true;
}

template <class T>
void TTCIterator<T>::Initialize(bool bDeepIteration)
{
	m_bDeepIteration= bDeepIteration;

	m_hlTail.AddTail(GetRootItem());
}

template <class T>
HTREEITEM TTCIterator<T>::GetNext()
{
	HTREEITEM htiRet= NULL;

	if (m_hlTail.GetHeadPosition() != NULL)
	{
		htiRet= m_hlTail.GetHead();
		
		m_hlTail.RemoveHead();

		// Siblings
		if (GetNextSiblingItem(htiRet) != NULL)
			m_hlTail.AddHead(GetNextSiblingItem(htiRet));

		// Children
		if (ItemHasChildren(htiRet))
		{
			HTREEITEM hItem = GetChildItem(htiRet);
			if ( hItem )
			{
				if (m_bDeepIteration) m_hlTail.AddHead( hItem );
				else m_hlTail.AddTail( hItem );
			}
		}
	}

	return htiRet;
}

#endif // !defined(_TCITERATOR_H__INCLUDED_)
