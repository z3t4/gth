# Microsoft Developer Studio Project File - Name="Core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Core - Win32 DBG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Core.mak" CFG="Core - Win32 DBG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Core - Win32 REL" (based on "Win32 (x86) Static Library")
!MESSAGE "Core - Win32 DBG" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GTHClient/Core", DVSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Core - Win32 REL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Core___Win32_REL"
# PROP BASE Intermediate_Dir "Core___Win32_REL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Core___Win32_REL"
# PROP Intermediate_Dir "Core___Win32_REL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /GX /O2 /I "../DX Folder" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FX_CLIENT" /YX /FD /c
# ADD CPP /nologo /ML /W3 /GX /O2 /I "../DX Folder" /D "_FX_CLIENT" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/Core.lib"
# ADD LIB32 /nologo /out:"../lib/CoreRelease.lib"

!ELSEIF  "$(CFG)" == "Core - Win32 DBG"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Core___Win32_DBG"
# PROP BASE Intermediate_Dir "Core___Win32_DBG"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Core___Win32_DBG"
# PROP Intermediate_Dir "Core___Win32_DBG"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../DX Folder" /D "_FXDEBUG" /D "_FX_CLIENT" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../lib/CoreDebug.lib"
# ADD LIB32 /nologo /out:"../lib/CoreDebug.lib"

!ENDIF 

# Begin Target

# Name "Core - Win32 REL"
# Name "Core - Win32 DBG"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BSP.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Collide.cpp
# End Source File
# Begin Source File

SOURCE=.\Core_def.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DRender.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DTextureMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Decal.cpp
# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\FileMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Frustum.cpp
# End Source File
# Begin Source File

SOURCE=.\Patch.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\ResManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\ShaderMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Shadow.cpp
# End Source File
# Begin Source File

SOURCE=.\Sky.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Travel.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BSP.h
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Collide.h
# End Source File
# Begin Source File

SOURCE=.\Core.h
# End Source File
# Begin Source File

SOURCE=.\Core_def.h
# End Source File
# Begin Source File

SOURCE=.\D3DBuffer.h
# End Source File
# Begin Source File

SOURCE=.\D3DDevice.h
# End Source File
# Begin Source File

SOURCE=.\D3DRender.h
# End Source File
# Begin Source File

SOURCE=.\D3DTextureMng.h
# End Source File
# Begin Source File

SOURCE=.\Decal.h
# End Source File
# Begin Source File

SOURCE=.\Display.h
# End Source File
# Begin Source File

SOURCE=.\FileMng.h
# End Source File
# Begin Source File

SOURCE=.\Frustum.h
# End Source File
# Begin Source File

SOURCE=.\Gth_Bsp.h
# End Source File
# Begin Source File

SOURCE=.\ResManager.h
# End Source File
# Begin Source File

SOURCE=.\ResourceCounter.h
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\Shader.h
# End Source File
# Begin Source File

SOURCE=.\ShaderMng.h
# End Source File
# Begin Source File

SOURCE=.\Surfaceflags.h
# End Source File
# Begin Source File

SOURCE=.\TcMod.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\WaveFunc.h
# End Source File
# End Group
# End Target
# End Project
