# Microsoft Developer Studio Project File - Name="Zlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Zlib - Win32 DBG PACK
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Zlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Zlib.mak" CFG="Zlib - Win32 DBG PACK"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Zlib - Win32 DBG I3D" (based on "Win32 (x86) Static Library")
!MESSAGE "Zlib - Win32 REL I3D" (based on "Win32 (x86) Static Library")
!MESSAGE "Zlib - Win32 DBG PACK" (based on "Win32 (x86) Static Library")
!MESSAGE "Zlib - Win32 REL PACK" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GTHClient/ZLIB", DVSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Zlib - Win32 DBG I3D"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Zlib___Win32_DBG_I3D"
# PROP BASE Intermediate_Dir "Zlib___Win32_DBG_I3D"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Zlib___Win32_DBG_I3D"
# PROP Intermediate_Dir "Zlib___Win32_DBG_I3D"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/GTH_ZLibI3DDebug.lib"

!ELSEIF  "$(CFG)" == "Zlib - Win32 REL I3D"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Zlib___Win32_REL_I3D"
# PROP BASE Intermediate_Dir "Zlib___Win32_REL_I3D"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Zlib___Win32_REL_I3D"
# PROP Intermediate_Dir "Zlib___Win32_REL_I3D"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_GTH_ZIP_VERSION_PACK" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"Release/Zlib.pch" /YX /Fo"Release/" /Fd"Release/" /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/GTH_ZLibI3DRelease.lib"

!ELSEIF  "$(CFG)" == "Zlib - Win32 DBG PACK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Zlib___Win32_DBG_PACK"
# PROP BASE Intermediate_Dir "Zlib___Win32_DBG_PACK"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Zlib___Win32_DBG_PACK"
# PROP Intermediate_Dir "Zlib___Win32_DBG_PACK"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_GTH_ZIP_VERSION_PACK" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/GTH_ZLibPackDebug.lib"

!ELSEIF  "$(CFG)" == "Zlib - Win32 REL PACK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Zlib___Win32_REL_PACK"
# PROP BASE Intermediate_Dir "Zlib___Win32_REL_PACK"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Zlib___Win32_REL_PACK"
# PROP Intermediate_Dir "Zlib___Win32_REL_PACK"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_GTH_ZIP_VERSION_PACK" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_GTH_ZIP_VERSION_PACK" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/GTH_ZLibPackRelease.lib"

!ENDIF 

# Begin Target

# Name "Zlib - Win32 DBG I3D"
# Name "Zlib - Win32 REL I3D"
# Name "Zlib - Win32 DBG PACK"
# Name "Zlib - Win32 REL PACK"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Unpak.cpp
# End Source File
# Begin Source File

SOURCE=.\unzip.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Unpak.h
# End Source File
# Begin Source File

SOURCE=.\unzip.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# End Group
# End Target
# End Project
