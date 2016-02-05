
#if !defined(AFX_NCFileExportDialog_H__BAABDC84_DDDA_11D2_A614_0060085FE616__INCLUDED_)
#define AFX_NCFileExportDialog_H__BAABDC84_DDDA_11D2_A614_0060085FE616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cNewCustomFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC ( cNewCustomFileDialog )

	public:
		//cCustomFileDialog ( const CString &title, const CString &filter, const CString &fileName = _T( "" ) );
		cNewCustomFileDialog ( bool bType, CString dir, const CString &title, const CString &filter, const CString &fileName = _T( "" ) );

		virtual ~cNewCustomFileDialog ( );

	protected:
		virtual void OnTypeChange();

		//{{AFX_MSG(CFileExportDialog)
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		TCHAR* m_pTitleBuffer;
		TCHAR* m_pFilterBuffer;
		TCHAR* m_pFileBuffer;
		int		m_CurrentFilterIndex;
};

/*
#define FILEEXPORTDIALOG_BASECLASS CFileDialog

class CFileExportDialog : public FILEEXPORTDIALOG_BASECLASS
{
  typedef FILEEXPORTDIALOG_BASECLASS base;

	DECLARE_DYNAMIC(CFileExportDialog)

  // @access Public Member Functions and Variables
public:

  // @cmember
  // constructor, creates dialog title and filter buffers
  CFileExportDialog(const CString &title, const CString &filter, 
                     const CString &fileName = "");

  // @cmember
  // destructor, destroys dialog title and filter buffers
  virtual ~CFileExportDialog();

  // @cmember,mfunc
  // return current filter index (useful if two file formats have same extension)
  int GetFilterIndex() const { return m_CurrentFilterIndex; }

  // @access Protected Member Functions and Variables
protected:

  // @cmember
  // called when user selects other filetype, changes extension
  virtual void OnTypeChange();

  //{{AFX_MSG(CFileExportDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  // @access Private Member Functions and Variables
private:

  char *m_pTitleBuffer;      // @cmember contains title string
  char *m_pFilterBuffer;     // @cmember contains filter string
  char *m_pFileBuffer;       // @cmember contains filename string
  int m_CurrentFilterIndex;  // @cmember current file type
};
*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCFileExportDialog_H__BAABDC84_DDDA_11D2_A614_0060085FE616__INCLUDED_)
