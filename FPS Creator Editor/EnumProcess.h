// EnumProcess.h: interface for the CEnumProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMPROCESS_H__E2207EFB_B378_44D7_83EC_16406F03E22D__INCLUDED_)
#define AFX_ENUMPROCESS_H__E2207EFB_B378_44D7_83EC_16406F03E22D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "psapi.h"
#include <tlhelp32.h>


#define USE_VDMDBG // Uncomment this if you don´t wat to use vdmdbg at all

#ifdef USE_VDMDBG 
#include <vdmdbg.h>
#include <afxcoll.h>
#include <afxtempl.h>
#endif // USE_VDMDBG


namespace ENUM_METHOD 
{const int NONE    = 0x0;
 const int PSAPI   = 0x1;
 const int TOOLHELP= 0x2;
 const int PROC16  = 0x4;
} 

const int MAX_FILENAME = 256;

#ifdef USE_VDMDBG 
// 16-bit Enumeration functions
typedef INT (WINAPI *PFVDMEnumTaskWOWEx)( 
  DWORD dwProcessId, TASKENUMPROCEX fp, LPARAM lparam 
); 
#endif // USE_VDMDBG

// Functions loaded from PSAPI
typedef BOOL (WINAPI *PFEnumProcesses)(
	DWORD * lpidProcess, DWORD cb, DWORD * cbNeeded
);

typedef BOOL (WINAPI *PFEnumProcessModules)(
    HANDLE hProcess, HMODULE * lphModule, DWORD cb, LPDWORD lpcbNeeded
);

typedef DWORD (WINAPI *PFGetModuleFileNameEx)(
  HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename, DWORD nSize         
);

//Functions loaded from Kernel32
typedef HANDLE (WINAPI *PFCreateToolhelp32Snapshot)(
  DWORD dwFlags,       
  DWORD th32ProcessID  
);

typedef BOOL (WINAPI *PFProcess32First)(
  HANDLE hSnapshot,      
  LPPROCESSENTRY32 lppe  
);

typedef BOOL (WINAPI *PFProcess32Next)(
  HANDLE hSnapshot,      
  LPPROCESSENTRY32 lppe  
);
 
typedef BOOL (WINAPI *PFModule32First)(
  HANDLE hSnapshot,     
  LPMODULEENTRY32 lpme  
);

typedef BOOL (WINAPI *PFModule32Next)(
  HANDLE hSnapshot,     
  LPMODULEENTRY32 lpme  
);



  
class CEnumProcess  
{
public:

    struct CProcessEntry
    {
     LPTSTR lpFilename;
     DWORD  dwPID;
     WORD   hTask16;
     // Constructors/Destructors
     CProcessEntry() : dwPID(0), hTask16(0) 
     {lpFilename = new TCHAR[MAX_FILENAME];}
     CProcessEntry(CProcessEntry &e) : dwPID(e.dwPID), hTask16(e.hTask16)
     {strcpy(lpFilename, e.lpFilename);}
     virtual ~CProcessEntry()
     {delete[] lpFilename;}
    };

    struct CModuleEntry
    {
     LPTSTR lpFilename;
     PVOID pLoadBase;
     PVOID pPreferredBase;
     // Constructors/Destructors
     CModuleEntry() : pLoadBase(NULL), pPreferredBase(NULL)
     {lpFilename = new TCHAR[MAX_FILENAME];}
     CModuleEntry(CModuleEntry &e) : pLoadBase(e.pLoadBase), pPreferredBase(e.pPreferredBase)
     {strcpy(lpFilename, e.lpFilename);}
     virtual ~CModuleEntry()
     {delete[] lpFilename;}
    };

    CEnumProcess();
	virtual ~CEnumProcess();

    BOOL GetModuleNext(DWORD dwPID, CModuleEntry* pEntry);
	BOOL GetModuleFirst(DWORD dwPID, CModuleEntry* pEntry);
	BOOL GetProcessNext(CProcessEntry *pEntry);    
    BOOL GetProcessFirst(CProcessEntry* pEntry);

	int GetAvailableMethods();
    int GetSuggestedMethod();
    int SetMethod(int method);



protected:

	PVOID GetModulePreferredBase(DWORD dwPID, PVOID pModBase);
    // General members
	int m_method;

    // PSAPI related members
    HMODULE PSAPI;   //Handle to the module
    int m_MAX_COUNT; 
    DWORD* m_pProcesses, *m_pCurrentP; // Process identifiers
    long m_cProcesses, m_cModules;     // Number of Processes/Modules found
    HMODULE* m_pModules, *m_pCurrentM; // Handles to Modules 
    // PSAPI related functions
    PFEnumProcesses       FEnumProcesses;           // Pointer to EnumProcess
    PFEnumProcessModules  FEnumProcessModules; // Pointer to EnumProcessModules
    PFGetModuleFileNameEx FGetModuleFileNameEx;// Pointer to GetModuleFileNameEx
	BOOL FillPStructPSAPI(DWORD pid, CProcessEntry* pEntry);
	BOOL FillMStructPSAPI(DWORD dwPID, HMODULE mMod, CModuleEntry* pEntry);

    // ToolHelp related members
    HANDLE m_hProcessSnap, m_hModuleSnap;
    HMODULE TOOLHELP;   //Handle to the module (Kernel32)
    PROCESSENTRY32 m_pe;
    MODULEENTRY32  m_me;
    // ToolHelp related functions
    PFCreateToolhelp32Snapshot FCreateToolhelp32Snapshot;
    PFProcess32First FProcess32First;
    PFProcess32Next  FProcess32Next;
    PFModule32First  FModule32First;
    PFModule32Next   FModule32Next;   

#ifdef USE_VDMDBG
    // 16-bit enumeration
    HMODULE VDMDBG;  // Handle to module (vdmdbg.h)
    PFVDMEnumTaskWOWEx FVDMEnumTaskWOWEx;
#endif // USE_VDMDBG


};

#endif // !defined(AFX_ENUMPROCESS_H__E2207EFB_B378_44D7_83EC_16406F03E22D__INCLUDED_)
