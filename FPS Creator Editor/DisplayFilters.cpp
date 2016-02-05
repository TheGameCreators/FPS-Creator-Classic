// DisplayFilters.cpp : implementation file
//

#include "stdafx.h"
#include "DisplayFilters.h"


// CDisplayFilters dialog

IMPLEMENT_DYNAMIC(CDisplayFilters, CDialog)

CDisplayFilters::CDisplayFilters(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayFilters::IDD, pParent)
{

}

CDisplayFilters::~CDisplayFilters()
{
}

void CDisplayFilters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDisplayFilters, CDialog)
END_MESSAGE_MAP()


// CDisplayFilters message handlers
