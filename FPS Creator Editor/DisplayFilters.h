#pragma once

#include "resource.h"

// CDisplayFilters dialog

class CDisplayFilters : public CDialog
{
	DECLARE_DYNAMIC(CDisplayFilters)

public:
	CDisplayFilters(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayFilters();

// Dialog Data
	enum { IDD = IDD_DISPLAYFILTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
