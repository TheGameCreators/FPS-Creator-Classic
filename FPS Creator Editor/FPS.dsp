# Microsoft Developer Studio Project File - Name="FPS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FPS - Win32 Full Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FPS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FPS.mak" CFG="FPS - Win32 Full Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FPS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FPS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "FPS - Win32 Full Debug" (based on "Win32 (x86) Application")
!MESSAGE "FPS - Win32 ReleaseDEMO" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FPS - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:"D:\_CODING\Dark Basic Pro Shared\Dark Basic Pro\Projects\FPSCREATOR\FPSCreator.exe"

!ELSEIF  "$(CFG)" == "FPS - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Fp"Debug/Editor.pch" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"D:\_CODING\Dark Basic Pro Shared\Dark Basic Pro\Projects\FPSCREATOR\FPSCreator.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "FPS - Win32 Full Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FPS___Win32_Full_Debug"
# PROP BASE Intermediate_Dir "FPS___Win32_Full_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FPS___Win32_Full_Debug"
# PROP Intermediate_Dir "FPS___Win32_Full_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Fp"Debug/Editor.pch" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Fp"Debug/Editor.pch" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"..\Runtime\FPSCreator.exe" /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"D:\_CODING\Dark Basic Pro Shared\Dark Basic Pro\Projects\FPSCREATOR\FPSCreator.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "FPS - Win32 ReleaseDEMO"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FPS___Win32_ReleaseDEMO"
# PROP BASE Intermediate_Dir "FPS___Win32_ReleaseDEMO"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FPS___Win32_ReleaseDEMO"
# PROP Intermediate_Dir "FPS___Win32_ReleaseDEMO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "FPSC_DEMO" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:"D:\_CODING\Dark Basic Pro Shared\Dark Basic Pro\Projects\FPSCREATOR\FPSCreator.exe"
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /out:"D:\_CODING\Dark Basic Pro Shared\Dark Basic Pro\Projects\FPSCREATOR\FPSCreator.exe"

!ENDIF 

# Begin Target

# Name "FPS - Win32 Release"
# Name "FPS - Win32 Debug"
# Name "FPS - Win32 Full Debug"
# Name "FPS - Win32 ReleaseDEMO"
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CntrItem.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap15.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DISmainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Editor.ico
# End Source File
# Begin Source File

SOURCE=.\Editor.rc
# End Source File
# Begin Source File

SOURCE=.\res\EditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\lightmapoff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lightmapon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar4a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar5a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar6a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar7a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarCold256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workspace.bmp
# End Source File
# End Group
# Begin Group "Library"

# PROP Default_Filter ""
# Begin Group "Splitter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ZSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\ZSplitter.h
# End Source File
# Begin Source File

SOURCE=.\ZSplitterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ZSplitterDlg.h
# End Source File
# End Group
# Begin Group "NewTabs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TabCtrlSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrlSSL.h
# End Source File
# Begin Source File

SOURCE=.\TabPageSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\TabPageSSL.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Browser.cpp
# End Source File
# Begin Source File

SOURCE=.\Browser.h
# End Source File
# Begin Source File

SOURCE=.\BrowserNew.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowserNew.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Group "Test Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cTestGame.cpp
# End Source File
# Begin Source File

SOURCE=.\cTestGame.h
# End Source File
# End Group
# Begin Group "Build Game"

# PROP Default_Filter ""
# Begin Group "Project"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cBuildGameKeys.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameKeys.h
# End Source File
# Begin Source File

SOURCE=.\cBuildGameWeapons.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameWeapons.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cBuildGame.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGame.h
# End Source File
# Begin Source File

SOURCE=.\cBuildGameBuild.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameBuild.h
# End Source File
# Begin Source File

SOURCE=.\cBuildGameLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameLevel.h
# End Source File
# Begin Source File

SOURCE=.\cBuildGameLight.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameLight.h
# End Source File
# Begin Source File

SOURCE=.\cBuildGameMultiplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameMultiplayer.h
# End Source File
# Begin Source File

SOURCE=.\cBuildGameProject.cpp
# End Source File
# Begin Source File

SOURCE=.\cBuildGameProject.h
# End Source File
# End Group
# Begin Group "About"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\About.h
# End Source File
# End Group
# Begin Group "New Project"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cNewProject.cpp
# End Source File
# Begin Source File

SOURCE=.\cNewProject.h
# End Source File
# End Group
# Begin Group "Level Resize"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cLevelResize.cpp
# End Source File
# Begin Source File

SOURCE=.\cLevelResize.h
# End Source File
# End Group
# Begin Group "Preferences"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cPreferences.cpp
# End Source File
# Begin Source File

SOURCE=.\cPreferences.h
# End Source File
# End Group
# Begin Group "Splash"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cFront.cpp
# End Source File
# Begin Source File

SOURCE=.\cFront.h
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# End Group
# Begin Group "FPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cFPIWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\cFPIWizard.h
# End Source File
# End Group
# Begin Group "Property"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\cProperty.h
# End Source File
# End Group
# Begin Group "Popup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\cPopup.h
# End Source File
# End Group
# Begin Group "Credits"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cCredits.cpp
# End Source File
# Begin Source File

SOURCE=.\cCredits.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cControls.cpp
# End Source File
# Begin Source File

SOURCE=.\cControls.h
# End Source File
# Begin Source File

SOURCE=.\cMessages.cpp
# End Source File
# Begin Source File

SOURCE=.\cMessages.h
# End Source File
# Begin Source File

SOURCE=.\FileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialog.h
# End Source File
# End Group
# Begin Group "Editor"

# PROP Default_Filter ""
# Begin Group "Document"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorDoc.h
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorView.h
# End Source File
# End Group
# Begin Group "Frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editor.h
# End Source File
# Begin Source File

SOURCE=.\EnumProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumProcess.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Entity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ObjectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectProperty.h
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.h
# End Source File
# End Group
# Begin Group "Browser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cLibraryImage.cpp
# End Source File
# Begin Source File

SOURCE=.\cLibraryImage.h
# End Source File
# Begin Source File

SOURCE=.\cLibraryTree.cpp
# End Source File
# Begin Source File

SOURCE=.\cLibraryTree.h
# End Source File
# Begin Source File

SOURCE=.\cLibraryWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\cLibraryWindow.h
# End Source File
# Begin Source File

SOURCE=.\TCExpand.h
# End Source File
# Begin Source File

SOURCE=.\TCIterator.h
# End Source File
# Begin Source File

SOURCE=.\TCPersist.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\cDebug.h
# End Source File
# Begin Source File

SOURCE=.\cDialogEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\cDialogEnd.h
# End Source File
# Begin Source File

SOURCE=.\cDialogFront.cpp
# End Source File
# Begin Source File

SOURCE=.\cDialogFront.h
# End Source File
# Begin Source File

SOURCE=.\IPC.cpp
# End Source File
# Begin Source File

SOURCE=.\IPC.h
# End Source File
# End Target
# End Project
