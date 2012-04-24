# Microsoft Developer Studio Project File - Name="Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Common - Win32 DBG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak" CFG="Common - Win32 DBG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common - Win32 REL" (based on "Win32 (x86) Static Library")
!MESSAGE "Common - Win32 DBG" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GTHClient/Common", DVSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common - Win32 REL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Common___Win32_REL"
# PROP BASE Intermediate_Dir "Common___Win32_REL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Common___Win32_REL"
# PROP Intermediate_Dir "Common___Win32_REL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/CommonRelease.lib"

!ELSEIF  "$(CFG)" == "Common - Win32 DBG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Common___Win32_DBG"
# PROP BASE Intermediate_Dir "Common___Win32_DBG"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Common___Win32_DBG"
# PROP Intermediate_Dir "Common___Win32_DBG"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/CommonDebug.lib"

!ENDIF 

# Begin Target

# Name "Common - Win32 REL"
# Name "Common - Win32 DBG"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dfont.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\diutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dmutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dxutil.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\d3dapp.h
# End Source File
# Begin Source File

SOURCE=.\d3dfont.h
# End Source File
# Begin Source File

SOURCE=.\d3dres.h
# End Source File
# Begin Source File

SOURCE=.\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\diutil.h
# End Source File
# Begin Source File

SOURCE=.\dmutil.h
# End Source File
# Begin Source File

SOURCE=.\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\dxutil.h
# End Source File
# End Group
# End Target
# End Project
