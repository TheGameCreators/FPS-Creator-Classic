/////////////////////////////////////////////////////////////////////
// Template: TTCPersist
// Purpose : Template to allow CTreeCtrl derived objects state to
// 				survive after a reload of the control
// Created : SEN (1- Jun - 2001)
// History : None
// 
// Copyright (c) 2001 by Francisco José Sen del Prado
// All rights reserved.
/////////////////////////////////////////////////////////////////////


// TCPersist.h : header file
//

#if !defined(_TCPERSIST_H__INCLUDED_)
#define _TCPERSIST_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXTEMPL_H__
	#pragma message ("To avoid this warning message place #include <afxtempl.h> in your stdafx.h precompiler headers file")
	#include <afxtempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// TTCPersist (Declaration)

template <class T>
class TTCPersist : public T
{
// Construction
public:
	TTCPersist();

// Operations
public:
	void SaveStatus();
	void RestoreStatus();
	void ResetStatus();

// Implementation
private:
	CString GetItemPath(HTREEITEM hti);

	CString		m_strFirstVisible;
	HTREEITEM	m_htiFirstVisible;

	CMapStringToString	m_strmExpanded,
						m_strmSelected;

	void SaveStatus_R	(HTREEITEM hItem);
	void RestoreStatus_R(HTREEITEM hItem);
};


/////////////////////////////////////////////////////////////////////////////
// TTCPersist (Implementation)

template <class T>
TTCPersist<T>::TTCPersist()
{
	m_htiFirstVisible= NULL;
}

template <class T>
void TTCPersist<T>::SaveStatus()
{
	m_strmExpanded.RemoveAll();
	m_strmSelected.RemoveAll();

	// First visible
	m_strFirstVisible= GetItemPath(GetFirstVisibleItem());

	SaveStatus_R(GetRootItem());
}

template <class T>
void TTCPersist<T>::RestoreStatus()
{
	m_htiFirstVisible= NULL;

	RestoreStatus_R(GetRootItem());

	// First visible
	if (m_htiFirstVisible != NULL)
	{
		SelectSetFirstVisible(m_htiFirstVisible);
	}
}

template <class T>
void TTCPersist<T>::ResetStatus()
{
	m_strmExpanded.RemoveAll();
	m_strmSelected.RemoveAll();

	// First visible
	m_strFirstVisible.Empty();
	m_htiFirstVisible= NULL;
}

template <class T>
CString TTCPersist<T>::GetItemPath(HTREEITEM hti)
{
	CString strRet;

	if (hti != NULL)
	{
		CString strAux= GetItemText(hti),
				strAux2= GetItemPath(GetParentItem(hti));

		if (strAux2.IsEmpty())
			strRet= strAux;
		else
			strRet= strAux2 + "\\" + strAux;
	}

	return strRet;
}

template <class T>
void TTCPersist<T>::SaveStatus_R(HTREEITEM hItem)
{
	if (hItem != NULL)
	{
		TRACE("Element: %s\n", (LPCSTR)GetItemPath(hItem));

		UINT uState= GetItemState(hItem, TVIS_EXPANDED | TVIS_SELECTED);

		if ((uState & TVIS_SELECTED) != 0)
		{
			CString strAux= GetItemPath(hItem);

			m_strmSelected[strAux]= "";
		}

		if (ItemHasChildren(hItem))
		{
			if ((uState & TVIS_EXPANDED) != 0)
			{
				CString strAux= GetItemPath(hItem);

				m_strmExpanded[strAux]= "";
			}

			// Recursive call
			SaveStatus_R(GetChildItem(hItem));
		}
	}

	// Recursive call
	if (GetNextSiblingItem(hItem) != NULL)
		SaveStatus_R(GetNextSiblingItem(hItem));
}

template <class T>
void TTCPersist<T>::RestoreStatus_R(HTREEITEM hItem)
{
	if (hItem != NULL)
	{
		CString strItem= GetItemPath(hItem),
				strAux;

		if (strItem == m_strFirstVisible)
		{
			ASSERT(m_htiFirstVisible == NULL);

			m_htiFirstVisible= hItem;
		}

		if (m_strmSelected.Lookup(strItem, strAux))
		{
			SelectItem(hItem);
		}

		if (m_strmExpanded.Lookup(strItem, strAux))
		{
			Expand(hItem, TVE_EXPAND);

			if (ItemHasChildren(hItem))
			{
				// Recursive call
				RestoreStatus_R(GetChildItem(hItem));
			}
		}
	}

	// Recursive call
	if (GetNextSiblingItem(hItem) != NULL)
		RestoreStatus_R(GetNextSiblingItem(hItem));
}

#endif // !defined(_TCPERSIST_H__INCLUDED_)
