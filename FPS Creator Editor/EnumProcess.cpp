// EnumProcess.cpp: implementation of the CEnumProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnumProcess.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#ifdef USE_VDMDBG 
BOOL CALLBACK Enum16Proc( DWORD dwThreadId, WORD hMod16, WORD hTask16, PSZ pszModName, PSZ pszFileName, LPARAM lpUserDefined );

CTypedPtrArray<CPtrArray, CEnumProcess::CProcessEntry*> g_Processes16;
#endif // USE_VDMDBG



CEnumProcess::CEnumProcess() : m_pProcesses(NULL), m_pModules(NULL), m_pCurrentP(NULL), m_pCurrentM(NULL)
{
	m_hProcessSnap = INVALID_HANDLE_VALUE;
    m_hModuleSnap = INVALID_HANDLE_VALUE;

    PSAPI = ::LoadLibrary(_TEXT("PSAPI"));
    if (PSAPI)  
        {// Setup variables
         m_MAX_COUNT = 256;
         m_cProcesses = 0;
         m_cModules   = 0;

         // Find PSAPI functions
         FEnumProcesses = (PFEnumProcesses)::GetProcAddress(PSAPI, _TEXT("EnumProcesses"));
         FEnumProcessModules = (PFEnumProcessModules)::GetProcAddress(PSAPI, _TEXT("EnumProcessModules"));
         #ifdef UNICODE
         FGetModuleFileNameEx = (PFGetModuleFileNameEx)::GetProcAddress(PSAPI, _TEXT("GetModuleFileNameExW"));
         #else
         FGetModuleFileNameEx = (PFGetModuleFileNameEx)::GetProcAddress(PSAPI, _TEXT("GetModuleFileNameExA"));
         #endif
        }

    TOOLHELP = ::LoadLibrary(_TEXT("Kernel32"));
    if (TOOLHELP)  
        {// Setup variables
         m_pe.dwSize = sizeof(m_pe);
         m_me.dwSize = sizeof(m_me);
         // Find ToolHelp functions
		 
         FCreateToolhelp32Snapshot = (PFCreateToolhelp32Snapshot)::GetProcAddress(TOOLHELP, _TEXT("CreateToolhelp32Snapshot"));
         FProcess32First = (PFProcess32First)::GetProcAddress(TOOLHELP, _TEXT("Process32First"));
         FProcess32Next = (PFProcess32Next)::GetProcAddress(TOOLHELP, _TEXT("Process32Next"));
         FModule32First = (PFModule32First)::GetProcAddress(TOOLHELP, _TEXT("Module32First"));
         FModule32Next = (PFModule32Next)::GetProcAddress(TOOLHELP, _TEXT("Module32Next"));
        }

#ifdef USE_VDMDBG
    VDMDBG = ::LoadLibrary(_TEXT("VDMDBG"));
    if (VDMDBG)
        {// Setup variables
          
         // Find VDMdbg functions
         FVDMEnumTaskWOWEx = (PFVDMEnumTaskWOWEx)::GetProcAddress(VDMDBG, "VDMEnumTaskWOWEx");
        }
#endif // USE_VDMDBG
    
    // Find the preferred method of enumeration
    m_method = ENUM_METHOD::NONE;
    int method = GetAvailableMethods();
    if (method == (method|ENUM_METHOD::PSAPI))    m_method = ENUM_METHOD::PSAPI;
    if (method == (method|ENUM_METHOD::TOOLHELP)) m_method = ENUM_METHOD::TOOLHELP;
    if (method == (method|ENUM_METHOD::PROC16))   m_method += ENUM_METHOD::PROC16;

}

CEnumProcess::~CEnumProcess()
{if (m_pProcesses) {delete[] m_pProcesses;}
 if (m_pModules)   {delete[] m_pModules;}
 if (PSAPI) FreeLibrary(PSAPI);
 if (TOOLHELP) FreeLibrary(TOOLHELP);
 if (INVALID_HANDLE_VALUE != m_hProcessSnap) ::CloseHandle(m_hProcessSnap);
 if (INVALID_HANDLE_VALUE != m_hModuleSnap)  ::CloseHandle(m_hModuleSnap);
 
#ifdef USE_VDMDBG
 if (VDMDBG) FreeLibrary(VDMDBG);
 while (g_Processes16.GetSize()>0)
 {CProcessEntry* tmp = g_Processes16[0];
  g_Processes16.RemoveAt(0);
  delete tmp;
 }
#endif // USE_VDMDBG
}



int CEnumProcess::GetAvailableMethods()
{int res = 0;
 // Does all psapi functions exist?
 if (PSAPI&&FEnumProcesses&&FEnumProcessModules&&FGetModuleFileNameEx) 
     res += ENUM_METHOD::PSAPI;
 // How about Toolhelp?
 if (TOOLHELP&&FCreateToolhelp32Snapshot&&FProcess32Next&&FProcess32Next&&FModule32First&&FModule32Next) 
     res += ENUM_METHOD::TOOLHELP;

#ifdef USE_VDMDBG
 // And 16-bit enumeration?
 if (VDMDBG&&FVDMEnumTaskWOWEx && res > ENUM_METHOD::NONE)
     res += ENUM_METHOD::PROC16;
#endif // USE_VDMDBG

 return res;
}

int CEnumProcess::SetMethod(int method)
{int avail = GetAvailableMethods();

 if (method != ENUM_METHOD::PROC16 && avail == (method|avail)) 
            m_method = method;

 return m_method;
}

int CEnumProcess::GetSuggestedMethod()
{return m_method;
}
// Retrieves the first process in the enumeration. Should obviously be called before
// GetProcessNext
////////////////////////////////////////////////////////////////////////////////////
BOOL CEnumProcess::GetProcessFirst(CEnumProcess::CProcessEntry *pEntry)
{if (ENUM_METHOD::NONE == m_method) return FALSE; 


if ((ENUM_METHOD::TOOLHELP|m_method) == m_method)
// Use ToolHelp functions
// ----------------------
{m_hProcessSnap = FCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
 if (INVALID_HANDLE_VALUE == m_hProcessSnap) return FALSE;

 if (!FProcess32First(m_hProcessSnap, &m_pe)) return FALSE;
 pEntry->dwPID = m_pe.th32ProcessID;
 strcpy(pEntry->lpFilename, m_pe.szExeFile);
}
else
// Use PSAPI functions
// ----------------------
{
 if (m_pProcesses) {delete[] m_pProcesses;}
 m_pProcesses = new DWORD[m_MAX_COUNT];
 m_pCurrentP = m_pProcesses;
 DWORD cbNeeded = 0;
 BOOL OK = FEnumProcesses(m_pProcesses, m_MAX_COUNT*sizeof(DWORD), &cbNeeded);

 // We might need more memory here..
 if (cbNeeded >= m_MAX_COUNT*sizeof(DWORD)) 
    {m_MAX_COUNT += 256;
     return GetProcessFirst(pEntry); // Try again.
    }

 if (!OK) return FALSE;
 m_cProcesses = cbNeeded/sizeof(DWORD); 
 return FillPStructPSAPI(*m_pProcesses, pEntry);
}

 return TRUE;
}

// Returns the following process
////////////////////////////////////////////////////////////////
BOOL CEnumProcess::GetProcessNext(CEnumProcess::CProcessEntry *pEntry)
{if (ENUM_METHOD::NONE == m_method) return FALSE; 
 pEntry->hTask16 = 0;

#ifdef USE_VDMDBG
 // We have some 16-bit processes to get
 if (g_Processes16.GetSize() > 0)
 {CEnumProcess::CProcessEntry *pTmp = g_Processes16[0];
  g_Processes16.RemoveAt(0);
  strcpy(pEntry->lpFilename, pTmp->lpFilename);
  pEntry->dwPID = pTmp->dwPID;
  pEntry->hTask16 = pTmp->hTask16;
  delete pTmp;
  return TRUE;
 }
#endif // USE_VDMDBG

// Use ToolHelp functions
// ----------------------
if ((ENUM_METHOD::TOOLHELP|m_method) == m_method)
{
 if (!FProcess32Next(m_hProcessSnap, &m_pe)) return FALSE;
 pEntry->dwPID = m_pe.th32ProcessID;
 strcpy(pEntry->lpFilename, m_pe.szExeFile);
}
else
// Use PSAPI functions
// ----------------------
{if (--m_cProcesses <= 0) return FALSE;
 FillPStructPSAPI(*++m_pCurrentP, pEntry);
}

#ifdef USE_VDMDBG
// Check for 16-bit processes?
if ((ENUM_METHOD::PROC16|m_method) == m_method)
   // is this a NTVDM?
   if( 0 == _stricmp( pEntry->lpFilename+(strlen(pEntry->lpFilename)-9), "NTVDM.EXE"))
     {
      FVDMEnumTaskWOWEx( pEntry->dwPID, (TASKENUMPROCEX) &Enum16Proc, (LPARAM) pEntry->dwPID);
     }
#endif // USE_VDMDBG

 return TRUE;
}


BOOL CEnumProcess::GetModuleFirst(DWORD dwPID, CEnumProcess::CModuleEntry *pEntry)
{if (ENUM_METHOD::NONE == m_method) return FALSE; 
// Use ToolHelp functions
// ----------------------
if ((ENUM_METHOD::TOOLHELP|m_method) == m_method)
{if (INVALID_HANDLE_VALUE != m_hModuleSnap)  ::CloseHandle(m_hModuleSnap);
 m_hModuleSnap = FCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

 if(!FModule32First(m_hModuleSnap, &m_me)) return FALSE;

 pEntry->pLoadBase = m_me.modBaseAddr;
 strcpy(pEntry->lpFilename, m_me.szExePath);
 pEntry->pPreferredBase = GetModulePreferredBase(dwPID, m_me.modBaseAddr);
 return TRUE;
}
else
// Use PSAPI functions
// ----------------------
{if (m_pModules) {delete[] m_pModules;}
 m_pModules = new HMODULE[m_MAX_COUNT];
 m_pCurrentM = m_pModules;
 DWORD cbNeeded = 0;
 HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
 if (hProc)
 {BOOL OK = FEnumProcessModules(hProc, m_pModules, m_MAX_COUNT*sizeof(HMODULE), &cbNeeded);
  CloseHandle(hProc);

  // We might need more memory here..
  if (cbNeeded >= m_MAX_COUNT*sizeof(HMODULE)) 
    {m_MAX_COUNT += 256;
     return GetModuleFirst(dwPID, pEntry); // Try again.
    }

  if (!OK) return FALSE;

  m_cModules = cbNeeded/sizeof(HMODULE); 
  return FillMStructPSAPI(dwPID, *m_pCurrentM, pEntry);
 }
 return FALSE;
}
}


BOOL CEnumProcess::GetModuleNext(DWORD dwPID, CEnumProcess::CModuleEntry *pEntry)
{if (ENUM_METHOD::NONE == m_method) return FALSE; 

// Use ToolHelp functions
// ----------------------
if ((ENUM_METHOD::TOOLHELP|m_method) == m_method)
{
 if(!FModule32Next(m_hModuleSnap, &m_me)) return FALSE;

 pEntry->pLoadBase = m_me.modBaseAddr;
 strcpy(pEntry->lpFilename, m_me.szExePath);
 pEntry->pPreferredBase = GetModulePreferredBase(dwPID, m_me.modBaseAddr);
 return TRUE;
}
else
// Use PSAPI functions
// ----------------------
{if (--m_cModules <= 0) return FALSE;
 return FillMStructPSAPI(dwPID, *++m_pCurrentM, pEntry);
}

}



BOOL CEnumProcess::FillPStructPSAPI(DWORD dwPID, CEnumProcess::CProcessEntry* pEntry)
{pEntry->dwPID = dwPID;

 // Open process to get filename
 HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
 if (hProc)
    {HMODULE hMod;
     DWORD size;
     // Get the first module (the process itself)
     if( FEnumProcessModules(hProc, &hMod, sizeof(hMod), &size) )
         {
          //Get filename
          if( !FGetModuleFileNameEx( hProc, hMod, pEntry->lpFilename, MAX_FILENAME) )
                  { 
                   strcpy(pEntry->lpFilename, "N/A (error)");  
                  }
         }
     CloseHandle(hProc);
    }
 else
     strcpy(pEntry->lpFilename, "N/A (security restriction)");

 return TRUE;
}


BOOL CEnumProcess::FillMStructPSAPI(DWORD dwPID, HMODULE mMod, CEnumProcess::CModuleEntry *pEntry)
{HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
 if (hProc)
 {if( !FGetModuleFileNameEx( hProc, mMod, pEntry->lpFilename, MAX_FILENAME) )
        {
         strcpy(pEntry->lpFilename, "N/A (error)");  
        }  
  pEntry->pLoadBase = (PVOID) mMod;
  pEntry->pPreferredBase = GetModulePreferredBase(dwPID, (PVOID)mMod);
  CloseHandle(hProc);
  return TRUE;
 }
 return FALSE;
}



PVOID CEnumProcess::GetModulePreferredBase(DWORD dwPID, PVOID pModBase)
{if (ENUM_METHOD::NONE == m_method) return NULL; 
 HANDLE hProc = OpenProcess(PROCESS_VM_READ, FALSE, dwPID);
 if (hProc)
 {IMAGE_DOS_HEADER idh;
  IMAGE_NT_HEADERS inh;
  //Read DOS header
  ReadProcessMemory(hProc, pModBase, &idh, sizeof(idh), NULL);

  if (IMAGE_DOS_SIGNATURE == idh.e_magic) // DOS header OK?
      // Read NT headers at offset e_lfanew 
      ReadProcessMemory(hProc, (PBYTE)pModBase + idh.e_lfanew, &inh, sizeof(inh), NULL);

  CloseHandle(hProc); 
    
  if (IMAGE_NT_SIGNATURE == inh.Signature) //NT signature OK?
   // Get the preferred base...
   return (PVOID) inh.OptionalHeader.ImageBase; 

 }

 return NULL; //didn't find anything useful..
}


#ifdef USE_VDMDBG
BOOL CALLBACK Enum16Proc( DWORD dwThreadId, WORD hMod16, WORD hTask16, PSZ pszModName, PSZ pszFileName, LPARAM lpUserDefined )
{CEnumProcess::CProcessEntry* pEntry = new CEnumProcess::CProcessEntry();
 strcpy(pEntry->lpFilename, pszFileName);
 pEntry->dwPID = (DWORD) lpUserDefined;
 pEntry->hTask16 = hTask16;
 g_Processes16.Add(pEntry);
 
 return FALSE;
}
#endif // USE_VDMDBG

