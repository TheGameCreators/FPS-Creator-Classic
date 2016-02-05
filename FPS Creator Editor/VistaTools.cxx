///////////////////////////////
/* VistaTools.cxx - version 1.0

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 2006.  WinAbility Software Corporation. All rights reserved.

Author: Andrei Belogortseff [ http://www.tweak-uac.com ]

TERMS OF USE: You are free to use this file in any way you like, 
for both the commercial and non-commercial purposes, royalty-free,
AS LONG AS you agree with the warranty disclaimer above, 
EXCEPT that you may not remove or modify this or any of the 
preceeding paragraphs. If you make any changes, please document 
them in the MODIFICATIONS section below. If the changes are of general 
interest, please let us know and we will consider incorporating them in 
this file, as well.

If you use this file in your own project, an acknowledgement will be appreciated, 
although it's not required.

SUMMARY:

This file contains several Vista-specific functions helpful when dealing with the 
"elevation" features of Windows Vista. See the descriptions of the functions below
for information on what each function does and how to use it.

This file contains the Win32 stuff only, it can be used with or without other frameworks, 
such as MFC, ATL, etc.

HOW TO USE THIS FILE:

Make sure you have the latest Windows SDK (see msdn.microsoft.com for more information)
or this file may not compile!

This is a "combo" file that contains both the declarations (usually placed in the .h files) 
as well as the definitions (usually placed in the .cpp files) of the functions. 

To get the declarations only, include it as you would any .h file, for example:

#include "VistaTools.cxx"

To get both the declarations and definitions, define IMPLEMENT_VISTA_TOOLS before including the file:

#define IMPLEMENT_VISTA_TOOLS
#include "VistaTools.cxx"

(The above should be done once and only once per project).

The file VistaTools.cxx can be included in the VisualStudio projects, but it should be 
excluded from the build process (because its contents is compiled when it is included 
in another .cpp file with IMPLEMENT_VISTA_TOOLS defined, as shown above.)

MODIFICATIONS:
	v.1.0 (2006-Dec-16) created by Andrei Belogortseff.

*/

#if ( NTDDI_VERSION < NTDDI_LONGHORN )
#	error NTDDI_VERSION must be defined as NTDDI_LONGHORN or later
#endif

//////////////////////////////////////////////////////////////////
// if ASSERT was not defined already, lets define our own version,
// to use the CRT assert() 

#ifndef ASSERT
#	ifdef _DEBUG
#		include <assert.h>
#		define ASSERT(x) assert( x )
#		define ASSERT_HERE assert( FALSE )
#	else// _DEBUG
#		define ASSERT(x) 
#	endif//_DEBUG
#endif//ASSERT

///////////////////////////////////////////////////////////////////
// a handy macro to get the number of characters (not bytes!) 
// a string buffer can hold

#ifndef _tsizeof 
#	define _tsizeof( s )  (sizeof(s)/sizeof(s[0]))
#endif//_tsizeof

#include <comdef.h>
#include <taskschd.h>

BOOL IsVista();

/*
Use IsVista() to determine whether the current process is running under Windows Vista or 
(or a later version of Windows, whatever it will be)

Return Values:
	If the function succeeds, and the current version of Windows is Vista or later, 
		the return value is TRUE. 
	If the function fails, or if the current version of Windows is older than Vista 
		(that is, if it is Windows XP, Windows 2000, Windows Server 2003, Windows 98, etc.)
		the return value is FALSE.
*/

HRESULT
GetElevationType( __out TOKEN_ELEVATION_TYPE * ptet );

/*
Use GetElevationType() to determine the elevation type of the current process.

Parameters:

ptet
	[out] Pointer to a variable that receives the elevation type of the current process.

	The possible values are:

	TokenElevationTypeDefault - User is not using a "split" token. 
		This value indicates that either UAC is disabled, or the process is started
		by a standard user (not a member of the Administrators group).

	The following two values can be returned only if both the UAC is enabled and
	the user is a member of the Administrator's group (that is, the user has a "split" token):

	TokenElevationTypeFull - the process is running elevated. 

	TokenElevationTypeLimited - the process is not running elevated.

Return Values:
	If the function succeeds, the return value is S_OK. 
	If the function fails, the return value is E_FAIL. To get extended error information, 
	call GetLastError().
*/

HRESULT 
IsElevated( __out_opt BOOL * pbElevated = NULL );

/*
Use IsElevated() to determine whether the current process is elevated or not.

Parameters:

pbElevated
	[out] [optional] Pointer to a BOOL variable that, if non-NULL, receives the result.

	The possible values are:

	TRUE - the current process is elevated.
		This value indicates that either UAC is enabled, and the process was elevated by 
		the administrator, or that UAC is disabled and the process was started by a user 
		who is a member of the Administrators group.

	FALSE - the current process is not elevated (limited).
		This value indicates that either UAC is enabled, and the process was started normally, 
		without the elevation, or that UAC is disabled and the process was started by a standard user. 

Return Values
	If the function succeeds, and the current process is elevated, the return value is S_OK. 
	If the function succeeds, and the current process is not elevated, the return value is S_FALSE. 
	If the function fails, the return value is E_FAIL. To get extended error information, 
	call GetLastError().
*/

BOOL 
RunAsStdUser(	
	__in		HWND	hwnd, 
	__in		LPCTSTR pszPath, 
	__in_opt	LPCTSTR pszParameters	= NULL, 
	__in_opt	LPCTSTR pszDirectory	= NULL );

/*
Use RunAsStdUser() to start a non-elevated process. If the current process is not elevated,
it calls ShellExecuteEx() to start the new process. If the current process is elevated,
it uses Vista Task Scheduler to create a non-elevated task. 

Parameters:

hwnd
	[in] Window handle to any message boxes that the system might produce while executing this function.

pszPath
	[in] Address of a null-terminated string that specifies the executable file that 
		should be used to start the process.

pszParameters
	[in] [optional] Address of a null-terminated string that contains the command-line parameters for 
		the process. If NULL, no parameters are passed to the process.

pszDirectory
	[in] [optional] Address of a null-terminated string that specifies the name of the working directory. 
		If NULL, the current directory is used as the working directory. .

Return Values
	If the function succeeds, the return value is TRUE. 
	If the function fails, the return value is FALSE. To get extended error information, 
	call GetLastError().

NOTE: For this function to work, the application must be marked with the asInvoker or 
highestAvailable attributes in its manifest. If the executable to be started is marked 
as requireAdministrator, it will be started elevated! 
*/

BOOL 
RunElevated(
	__in		HWND	hwnd, 
	__in		LPCTSTR pszPath, 
	__in_opt	LPCTSTR pszParameters	= NULL, 
	__in_opt	LPCTSTR pszDirectory	= NULL );

/*
Use RunElevated() to start an elevated process. This function calls ShellExecEx() with the verb "runas" 
to start the elevated process.

Parameters:

hwnd
	[in] Window handle to any message boxes that the system might produce while executing this function.

pszPath
	[in] Address of a null-terminated string that specifies the name of the executable file that 
		should be used to start the process.

pszParameters
	[in] [optional] Address of a null-terminated string that contains the command-line parameters for the process. 
		If NULL, no parameters are passed to the process.

pszDirectory
	[in] [optional] Address of a null-terminated string that specifies the name of the working directory. 
		If NULL, the current directory is used as the working directory. .

Return Values
	If the function succeeds, the return value is TRUE. 
	If the function fails, the return value is FALSE. To get extended error information, 
	call GetLastError().

NOTE: This function will start a process elevated no matter which attribute (asInvoker, 
highestAvailable, or requireAdministrator) is specified in its manifest, and even if 
there is no such attribute at all.
*/

#ifdef IMPLEMENT_VISTA_TOOLS

# pragma comment(lib, "taskschd.lib")
# pragma comment(lib, "comsupp.lib")
# pragma comment(lib, "credui.lib")

//////////////////////////////////////////////////////////
// MyShellExec is just a wrapper around a call to ShellExecuteEx, 
// to be able to specify the verb easily.

BOOL 
MyShellExec(	HWND hwnd, 
				LPCTSTR pszVerb, 
				LPCTSTR pszPath, 
				LPCTSTR pszParameters = NULL,
				LPCTSTR pszDirectory = NULL )
{
	SHELLEXECUTEINFO shex;

	memset( &shex, 0, sizeof( shex) );

	shex.cbSize			= sizeof( SHELLEXECUTEINFO ); 
	shex.fMask			= 0; 
	shex.hwnd			= hwnd;
	shex.lpVerb			= pszVerb; 
	shex.lpFile			= pszPath; 
	shex.lpParameters	= pszParameters; 
	shex.lpDirectory	= pszDirectory; 
	shex.nShow			= SW_NORMAL; 
 
	return ::ShellExecuteEx( &shex );
}

BOOL IsVista()
{
	OSVERSIONINFO osver;

	osver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	
	if (	::GetVersionEx( &osver ) && 
			osver.dwPlatformId == VER_PLATFORM_WIN32_NT && 
			(osver.dwMajorVersion >= 6 ) )
		return TRUE;

	return FALSE;
}

HRESULT
GetElevationType( __out TOKEN_ELEVATION_TYPE * ptet )
{
	ASSERT( IsVista() );
	ASSERT( ptet );

	HRESULT hResult = E_FAIL; // assume an error occured
	HANDLE hToken	= NULL;

	if ( !::OpenProcessToken( 
				::GetCurrentProcess(), 
				TOKEN_QUERY, 
				&hToken ) )
	{
		ASSERT( FALSE );
		return hResult;
	}

	DWORD dwReturnLength = 0;

	if ( !::GetTokenInformation(
				hToken,
				TokenElevationType,
				ptet,
				sizeof( *ptet ),
				&dwReturnLength ) )
	{
		ASSERT( FALSE );
	}
	else
	{
		ASSERT( dwReturnLength == sizeof( *ptet ) );
		hResult = S_OK;
	}

	::CloseHandle( hToken );

	return hResult;
}

HRESULT 
IsElevated( __out_opt BOOL * pbElevated ) //= NULL )
{
	ASSERT( IsVista() );

	HRESULT hResult = E_FAIL; // assume an error occured
	HANDLE hToken	= NULL;

	if ( !::OpenProcessToken( 
				::GetCurrentProcess(), 
				TOKEN_QUERY, 
				&hToken ) )
	{
		ASSERT( FALSE );
		return hResult;
	}

	TOKEN_ELEVATION te = { 0 };
	DWORD dwReturnLength = 0;

	if ( !::GetTokenInformation(
				hToken,
				TokenElevation,
				&te,
				sizeof( te ),
				&dwReturnLength ) )
	{
		ASSERT( FALSE );
	}
	else
	{
		ASSERT( dwReturnLength == sizeof( te ) );

		hResult = te.TokenIsElevated ? S_OK : S_FALSE; 

		if ( pbElevated)
			*pbElevated = (te.TokenIsElevated != 0);
	}

	::CloseHandle( hToken );

	return hResult;
}

//////////////////////////////////////////////////////////////////////
// RunAsStdUser is based on the MSDN sample "Registration Trigger Example" 
//
// First, some helper classes, to make the interfaces release themselves on exit.
// Sure, we could use the templates for that, but let's take it easy here. 

class ITaskServiceHelper
{
public:
	ITaskServiceHelper()		{	p = NULL;	}
	~ITaskServiceHelper()		{	if ( p ) p->Release();	}

	ITaskService * p;
};

class ITaskFolderHelper
{
public:
	ITaskFolderHelper()			{	p = NULL;	}
	~ITaskFolderHelper()		{	if ( p ) p->Release();	}

	ITaskFolder * p;
};

class ITaskDefinitionHelper
{
public:
	ITaskDefinitionHelper()		{	p = NULL;	}
	~ITaskDefinitionHelper()	{	if ( p ) p->Release();	}

	ITaskDefinition * p;
};

class IRegistrationInfoHelper
{
public:
	IRegistrationInfoHelper()	{	p = NULL;	}
	~IRegistrationInfoHelper()	{	if ( p ) p->Release();	}

	IRegistrationInfo * p;
};

class IPrincipalHelper
{
public:
	IPrincipalHelper()			{	p = NULL;	}
	~IPrincipalHelper()			{	if ( p ) p->Release();	}

	IPrincipal * p;
};

class ITaskSettingsHelper
{
public:
	ITaskSettingsHelper()		{	p = NULL;	}
	~ITaskSettingsHelper()		{	if ( p ) p->Release();	}

	ITaskSettings * p;
};

class ITriggerCollectionHelper
{
public:
	ITriggerCollectionHelper()	{	p = NULL;	}
	~ITriggerCollectionHelper()	{	if ( p ) p->Release();	}

	ITriggerCollection * p;
};

class ITriggerHelper
{
public:
	ITriggerHelper()			{	p = NULL;	}
	~ITriggerHelper()			{	if ( p ) p->Release();	}

	ITrigger * p;
};

class IRegistrationTriggerHelper
{
public:
	IRegistrationTriggerHelper(){	p = NULL;	}
	~IRegistrationTriggerHelper(){	if ( p ) p->Release();	}

	IRegistrationTrigger * p;
};

class IActionCollectionHelper
{
public:
	IActionCollectionHelper()	{	p = NULL;	}
	~IActionCollectionHelper()	{	if ( p ) p->Release();	}

	IActionCollection * p;
};

class IActionHelper
{
public:
	IActionHelper()				{	p = NULL;	}
	~IActionHelper()			{	if ( p ) p->Release();	}

	IAction * p;
};

class IExecActionHelper
{
public:
	IExecActionHelper()			{	p = NULL;	}
	~IExecActionHelper()		{	if ( p ) p->Release();	}

	IExecAction * p;
};

class IRegisteredTaskHelper
{
public:
	IRegisteredTaskHelper()		{	p = NULL;	}
	~IRegisteredTaskHelper()	{	if ( p ) p->Release();	}

	IRegisteredTask * p;
};

///////////////////////////////////////////////////////////////////
// DO is a handy macro that makes the code a bit more readable:
// it checks the result of action and returns if tehre is an error.
// Sure, we could also use throw/catch, etc., but lets keep things simple here.

#define DO( action )		\
	if( FAILED( action ) )	\
    {						\
		ASSERT( FALSE );	\
        return FALSE;		\
    }

/////////////////////////////////////////////
// now the function itself:

BOOL 
RunAsStdUser(
	__in		HWND	hwnd, 
	__in		LPCTSTR pszPath, 
	__in_opt	LPCTSTR pszParameters, //	= NULL, 
	__in_opt	LPCTSTR pszDirectory ) //	= NULL );
{
	ASSERT( IsVista() );
	ASSERT( pszPath );

	if ( S_FALSE == IsElevated() ) 
	{
		// if the current process is not elevated, we can use ShellExecuteAs directly! 
	
		return MyShellExec( hwnd, 
							NULL, 
							pszPath, 
							pszParameters, 
							pszDirectory );
	}

	// If process is elevated, we ask Task Scheduler to start a task for us.
	// I wish Microsoft would have provided an easier way for starting a non-elevated task,
	// like another verb for ShellExecuteEx, but they chose not to. 
	// I hope they had good reasons for that.

	HRESULT hr = 0;

    //  Choose a name for the task.

    LPCWSTR pszTaskName = L"RunAsStdUser Task";

    //  Create an instance of the Task Service. 
    
	ITaskServiceHelper iService;

    DO( hr = CoCreateInstance( CLSID_TaskScheduler,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_ITaskService,
						   (void**)&iService.p ) )
        
    //  Connect to the task service.
    DO( iService.p->Connect( _variant_t(), _variant_t(), _variant_t(), _variant_t()) )

    //  Get the pointer to the root task folder.  This folder will hold the
    //  new task that is registered.

	ITaskFolderHelper iRootFolder;

    DO( iService.p->GetFolder( _bstr_t( L"\\") , &iRootFolder.p ) )
    
    //  If the same task exists, remove it.
    iRootFolder.p->DeleteTask( _bstr_t( pszTaskName), 0  ); // ignore error message, if any
    
    //  Create the task builder object to create the task.

	ITaskDefinitionHelper iTask;

    DO( iService.p->NewTask( 0, &iTask.p ) )

    //  Get the registration info for setting the identification.
    
	IRegistrationInfoHelper iRegInfo;

    DO( iTask.p->get_RegistrationInfo( &iRegInfo.p ) )
    
    DO( iRegInfo.p->put_Author( L"RunAsStdUser" ) )

    //  Create the principal for the task

	IPrincipalHelper iPrincipal;

    DO( iTask.p->get_Principal( &iPrincipal.p ) )
    
    //  Set up principal information: 
    DO( iPrincipal.p->put_Id( _bstr_t(L"RunAsStdUser_Principal") ) )

    DO( iPrincipal.p->put_LogonType( TASK_LOGON_INTERACTIVE_TOKEN ) )

    //  Run the task with the least privileges (LUA) 
    DO( iPrincipal.p->put_RunLevel( TASK_RUNLEVEL_LUA ) )

    //  Create the settings for the task

	ITaskSettingsHelper iSettings;

    DO( iTask.p->get_Settings( &iSettings.p ) )
    
    //  Set setting values for the task.
    DO( iSettings.p->put_StartWhenAvailable(VARIANT_BOOL(true)) )

    //  Get the trigger collection to insert the registration trigger.

	ITriggerCollectionHelper iTriggerCollection;

    DO( iTask.p->get_Triggers( &iTriggerCollection.p ) )

    //  Add the registration trigger to the task.

	ITriggerHelper iTrigger;

    DO( iTriggerCollection.p->Create( TASK_TRIGGER_REGISTRATION, &iTrigger.p ) )
    
	IRegistrationTriggerHelper iRegistrationTrigger;

    DO( iTrigger.p->QueryInterface( 
        IID_IRegistrationTrigger, (void**) &iRegistrationTrigger.p ) )

    DO( iRegistrationTrigger.p->put_Id( _bstr_t( L"RunAsStdUser_Trigger" ) ) )
    
    //  Define the delay for the registration trigger.
    DO( iRegistrationTrigger.p->put_Delay( L"PT0S" ) ) // 0 second delay of execution; 

    //  Add an Action to the task. This task will execute notepad.exe.     

	IActionCollectionHelper iActionCollection;

    //  Get the task action collection pointer.
    DO( iTask.p->get_Actions( &iActionCollection.p ) )
    
    //  Create the action, specifying that it is an executable action.
	IActionHelper iAction;

    DO( iActionCollection.p->Create( TASK_ACTION_EXEC, &iAction.p ) )

	IExecActionHelper iExecAction;

    //  QI for the executable task pointer.
    DO( iAction.p->QueryInterface( 
        IID_IExecAction, (void**) &iExecAction.p ) )

    //  Set the path of the executable to notepad.exe.
    DO( iExecAction.p->put_Path( _bstr_t( pszPath ) ) )

    if( pszParameters )
	{
		DO( iExecAction.p->put_Arguments( _bstr_t( pszParameters ) ) )
	}

    if( pszDirectory )
	{
		DO( iExecAction.p->put_WorkingDirectory( _bstr_t( pszDirectory ) ) )
	}

    //  Save the task in the root folder.

	IRegisteredTaskHelper iRegisteredTask;

    DO( iRootFolder.p->RegisterTaskDefinition(
            _bstr_t( pszTaskName ),
            iTask.p,
            TASK_CREATE_OR_UPDATE, 
            _variant_t(), 
            _variant_t(), 
            TASK_LOGON_INTERACTIVE_TOKEN,
            _variant_t(L""),
            &iRegisteredTask.p) )

    return TRUE;
}

////////////////////////////////
// RunElevated simply calls ShellExecuteEx with the verb "runas" to start the elevated process.
// I wish there was a just as easy way to start a non-elevated process, as well.

BOOL 
RunElevated( 
	__in		HWND	hwnd, 
	__in		LPCTSTR pszPath, 
	__in_opt	LPCTSTR pszParameters, //	= NULL, 
	__in_opt	LPCTSTR pszDirectory ) //	= NULL );
{
	return MyShellExec( hwnd, _T("runas"), pszPath, pszParameters, pszDirectory );
}

#endif// IMPLEMENT_VISTA_TOOLS