

#include "stdafx.h"
#include <dlgs.h>
#include "FileDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC ( cNewCustomFileDialog, CFileDialog )

BEGIN_MESSAGE_MAP(cNewCustomFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CFileExportDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static TCHAR *newdup(const TCHAR *str)
{
	// 170105
	//TCHAR *ptr = new TCHAR[wcslen(str) + 1];
	TCHAR *ptr = new TCHAR[strlen(str) + 1];
	// 170105
	//wcscpy(ptr, str);
	strcpy(ptr, str);
	return ptr;
}

static int Split ( TCHAR *str,TCHAR *word[],int max,TCHAR c )
{
	int n=0; TCHAR quote='\0';

	if (!str)
		return n;

	for (;;) 
	{
		while (*str==c)
			str++;

		if (!*str || n==max)
			return n;

		if (!quote && (*str=='"' || *str=='\''))
		{ 
			quote=*str;
			str++;
		}

		word[n++]=str++;

		while (*str && (!quote && *str!=c || quote && *str!=quote))
			str++;

		if (quote && *str==quote)
		{
			quote='\0';
			*str='\0';
			str++;
		}

		if (!*str)
			return n;

		if (n<max)
			*str++ ='\0';
	}
	
}

//cCustomFileDialog::cCustomFileDialog ( const CString &title, const CString &filter, const CString &fileName ) : CFileDialog ( false, _T ( "" ), fileName )
cNewCustomFileDialog::cNewCustomFileDialog ( bool bType, CString dir, const CString &title, const CString &filter, const CString &fileName ) : CFileDialog ( bType, _T ( "" ), fileName )
{
	m_pTitleBuffer   = newdup ( title );
	m_ofn.lpstrTitle = m_pTitleBuffer;

	TCHAR*	tempBuffer = newdup ( filter );
	TCHAR*	word[40];

	int n = Split ( tempBuffer, word, 20, '|' );

	TCHAR **fptr = new TCHAR *[n];
  
	m_pFilterBuffer = new TCHAR[filter.GetLength()+n*8+1];
	m_pFilterBuffer[0]='\0';

	for (int i = 0; i < n; ++i)
	{
		// copy format description and add pipe
		// 170105
		//wcscat(m_pFilterBuffer,word[i]);
		//wcscat(m_pFilterBuffer,_T("|"));
		strcat(m_pFilterBuffer,word[i]);
		strcat(m_pFilterBuffer,_T("|"));

		// extract filter from format description
		TCHAR *part1[2];
		int m = Split(word[i],part1,2,'(');

		ASSERT(m == 2);
		TCHAR *part2[2];
		m = Split(part1[1],part2,2,')');

		// 170105
		//ASSERT((m == 1 || m == 2) && !wcsncmp(part2[0],_T("*."),2));
		ASSERT((m == 1 || m == 2) && !strncmp(part2[0],_T("*."),2));

		// 170105
		//wcscat(m_pFilterBuffer,part2[0]);
		//wcscat(m_pFilterBuffer,_T("|"));
		strcat(m_pFilterBuffer,part2[0]);
		strcat(m_pFilterBuffer,_T("|"));
		
		fptr[i] = part2[0]+1;
	}

	// replace pipes by nul characters
	// 170105
	//int l = wcslen(m_pFilterBuffer);
	int l = strlen(m_pFilterBuffer);

	for (i = 0; i < l; ++i)
	{
		if (m_pFilterBuffer[i] == '|')
			m_pFilterBuffer[i] = '\0';
	}

	// set filter string
	m_ofn.lpstrFilter = m_pFilterBuffer;

	m_ofn.lpstrInitialDir = dir;

	// set default filename in dialog
	m_pFileBuffer = new TCHAR[256];
	m_ofn.lpstrFile = m_pFileBuffer;
	m_ofn.nMaxFile = 256;

	if (fileName.IsEmpty())
	{
		// 170105
		//swprintf(m_pFileBuffer, _T("*%s"),fptr[0]);
		sprintf(m_pFileBuffer, _T("*%s"),fptr[0]);
	}
	else
	{
		// 170105
		//wcscpy(m_pFileBuffer, fileName);
		strcpy(m_pFileBuffer, fileName);
	}

	// set filter index from default filename
	m_ofn.nFilterIndex = m_CurrentFilterIndex = 1;

	if (fileName.IsEmpty())
	{

	}
	else
	{
		// 170105
		//TCHAR *ext = wcschr(CString(fileName),'.');
		TCHAR *ext = (TCHAR*)strchr(CString(fileName),'.');

		if (ext)
		{
			for (int i = 0; i < n; ++i)
				//170105
			//if (!wcscmp(ext, fptr[i]))
			if (!strcmp(ext, fptr[i]))
			{
				m_ofn.nFilterIndex = m_CurrentFilterIndex = i + 1; 
				break;
			}
		}
	}

	// free temporary buffers
	delete tempBuffer;
	delete [] fptr;
}

cNewCustomFileDialog::~cNewCustomFileDialog ( )
{
	delete [] m_pTitleBuffer;
	delete [] m_pFilterBuffer;
	delete [] m_pFileBuffer;
}

void cNewCustomFileDialog::OnTypeChange ( )
{
	// get current filename
	CWnd *fileNameBox = GetParent()->GetDlgItem(edt1);
	ASSERT_VALID(fileNameBox);
	CString fileName; fileNameBox->GetWindowText(fileName);

	// get current extension
	CWnd *typeNameBox = GetParent()->GetDlgItem(cmb1);
	ASSERT_VALID(typeNameBox);
	CString typeName; typeNameBox->GetWindowText(typeName);
	TCHAR *tempBuffer = newdup(typeName);
	TCHAR *ptr[2];
	int n = Split(tempBuffer, ptr, 2, '*');
	ASSERT(n == 2);
	n = Split(ptr[1], ptr, 2, ')');
	ASSERT(n == 1);
	typeName = ptr[0];

	// add or substute new extension in current filename
	if (fileName.IsEmpty())
	{
		fileName = "*" + typeName;
	}
	else
	{
		int index = fileName.Find('.');
		if (index == -1) 
		{
			fileName = fileName + typeName;
		}
		else
		{
			fileName = fileName.Left(index) + typeName;
		}
	}

	fileNameBox->SetWindowText(fileName);

	// get current filter index
	m_CurrentFilterIndex = ((CComboBox *)typeNameBox)->GetCurSel() + 1;

	// free temporary buffer
	delete tempBuffer;
}
