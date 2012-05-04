# Microsoft Developer Studio Project File - Name="GTH" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GTH - Win32 DBG PACK Malaysia
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GTH.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GTH.mak" CFG="GTH - Win32 DBG PACK Malaysia"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GTH - Win32 DBG PACK Chinese" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 DBG I3D Chinese" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 DBG I3D" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 DBG PACK" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL PACK" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL PACK Taiwan" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL I3D" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 DBG I3D OFF" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 DBG I3D REMOTE" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL PACK_Chinese" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL PACK CHEATON" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 DBG PACK Malaysia" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL PACK Malaysia" (based on "Win32 (x86) Application")
!MESSAGE "GTH - Win32 REL PACK_TO_GM" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GTHClient", DVSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GTH - Win32 DBG PACK Chinese"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_DBG_PACK_Chinese"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_PACK_Chinese"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_DBG_PACK_Chinese"
# PROP Intermediate_Dir "GTH___Win32_DBG_PACK_Chinese"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_FX_CLIENT" /D "_GTH_CHINESE_VERSION" /D "_GTH_ONLINE_VERSION" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_CHINESE_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /D "_GTH_CHEAT_ENABLED" /D "WINDOW_MODE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CorePackRelease.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib /nologo /subsystem:windows /machine:I386 /out:".\exe\Client.exe" /libpath:"./lib"
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dinput8.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibPackDebug.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /out:"../client_dev/dbg_pack_on_chinese.exe" /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GTH - Win32 DBG I3D Chinese"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GTH___Win32_DBG_I3D_Chinese"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_I3D_Chinese"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GTH___Win32_DBG_I3D_Chinese"
# PROP Intermediate_Dir "GTH___Win32_DBG_I3D_Chinese"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_CHINESE_VERSION" /D "NO_GAMEGUARD" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_CHINESE_VERSION" /D "_GTH_NEW_VERSION" /D "NO_GAMEGUARD" /D "_FMOD_SOUND" /D "WINDOW_MODE" /D "_GTH_CHEAT_ENABLED" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CoreDebug.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"exe\Client.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 ijl15.lib dinput8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibI3DDebug.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /out:"..\client_dev\dbg_i3d_on_chinese.exe" /pdbtype:sept /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GTH - Win32 DBG I3D"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GTH___Win32_DBG_I3D"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_I3D"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GTH___Win32_DBG_I3D"
# PROP Intermediate_Dir "GTH___Win32_DBG_I3D"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOREAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /D "_GTH_CHEAT_ENABLED" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CoreDebug.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"exe/0212_d_on.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibI3DDebug.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\exe\dbg_i3d_client.exe" /pdbtype:sept /libpath:"./lib"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "GTH - Win32 DBG PACK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GTH___Win32_DBG_PACK"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_PACK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GTH___Win32_DBG_PACK"
# PROP Intermediate_Dir "GTH___Win32_DBG_PACK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOERAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /D "_GTH_CHEAT_ENABLED" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CorePackDebug.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"D:\GTH_TestServer\GTH_TestClient_1230\0106_dbg_new_fmod_pack.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibPackDebug.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\exe\dbg_pack_client.exe" /pdbtype:sept /libpath:"./lib" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GTH - Win32 REL PACK"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_PACK"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_PACK"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_PACK"
# PROP Intermediate_Dir "GTH___Win32_REL_PACK"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /D "WINDOW_MODE" /D "_GTH_CHEAT_ENABLED" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CorePackRelease.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib /nologo /subsystem:windows /machine:I386 /out:".\exe\Client.exe" /libpath:"./lib"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib mfclib.lib /nologo /subsystem:windows /pdb:"GTH___Win32_REL_PACK_TO_GM/client_kr_gm.pdb" /map:"GTH___Win32_REL_PACK_TO_GM/client_kr_gm.map" /debug /machine:I386 /out:".\exe\client.exe" /libpath:"./lib" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GTH - Win32 REL PACK Taiwan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_PACK_Taiwan"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_PACK_Taiwan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_PACK_Taiwan"
# PROP Intermediate_Dir "GTH___Win32_REL_PACK_Taiwan"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_TAIWAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_TAIWAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND_GTH_ZIP_VERSION_PACK" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CoreDebug.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib /nologo /subsystem:windows /machine:I386 /out:".\exe\Client.exe" /libpath:"./lib"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib /nologo /subsystem:windows /machine:I386 /out:"./exe/rel_pack_on_taiwan.exe" /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "GTH - Win32 REL I3D"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_I3D"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_I3D"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_I3D"
# PROP Intermediate_Dir "GTH___Win32_REL_I3D"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOREAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_NEW_SERVER" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CorePackRelease.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib /nologo /subsystem:windows /machine:I386 /out:".\exe\Client.exe" /libpath:"./lib"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibI3DRelease.lib /nologo /subsystem:windows /machine:I386 /out:"./exe/rel_on_i3d.exe" /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "GTH - Win32 DBG I3D OFF"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GTH___Win32_DBG_I3D_OFF"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_I3D_OFF"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GTH___Win32_DBG_I3D_OFF"
# PROP Intermediate_Dir "GTH___Win32_DBG_I3D_OFF"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOREAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_OFFLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOREAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_NEW_SERVER" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib CoreDebug.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"./exe/dbg_i3d_on.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibI3DDebug.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../exe/dbg_i3d_off.exe" /pdbtype:sept /libpath:"./lib"

!ELSEIF  "$(CFG)" == "GTH - Win32 DBG I3D REMOTE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GTH___Win32_DBG_I3D_REMOTE"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_I3D_REMOTE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GTH___Win32_DBG_I3D_REMOTE"
# PROP Intermediate_Dir "GTH___Win32_DBG_I3D_REMOTE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOREAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_NEW_SERVER" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "NO_GAMEGUARD" /D "_GTH_KOREAN_VERSION" /D "_GTH_CHEAT_ENABLED" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_NEW_SERVER" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibI3DDebug.lib /nologo /subsystem:windows /debug /machine:I386 /out:"./exe/dbg_i3d_on.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibI3DDebug.lib /nologo /subsystem:windows /debug /machine:I386 /out:"D:\SourceSafe_Clinet\EXE\dbg_i3d_on.exe" /pdbtype:sept /libpath:"./lib"

!ELSEIF  "$(CFG)" == "GTH - Win32 REL PACK_Chinese"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_PACK_Chinese0"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_PACK_Chinese0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_PACK_Chinese0"
# PROP Intermediate_Dir "GTH___Win32_REL_PACK_Chinese0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_CHINESE_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib /nologo /subsystem:windows /map /machine:I386 /out:"../exe/rel_pack_on.exe" /libpath:"./lib"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib imm32.lib NPGameLib.lib AntiCpCnt.lib /nologo /subsystem:windows /map /machine:I386 /out:"..\client_dev\rel_pack_Chinese.exe" /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "GTH - Win32 REL PACK CHEATON"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_PACK_CHEATON"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_PACK_CHEATON"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_PACK_CHEATON"
# PROP Intermediate_Dir "GTH___Win32_REL_PACK_CHEATON"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /Fr /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "_GTH_CHEAT_ENABLED" /D "NO_GAMEGUARD" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib /nologo /subsystem:windows /map /machine:I386 /out:".\EXE\rel_pack_on.exe" /libpath:"./lib"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\exe\rel_pack_Cheat_on.exe" /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GTH - Win32 DBG PACK Malaysia"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GTH___Win32_DBG_PACK_Malaysia"
# PROP BASE Intermediate_Dir "GTH___Win32_DBG_PACK_Malaysia"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GTH___Win32_DBG_PACK_Malaysia"
# PROP Intermediate_Dir "GTH___Win32_DBG_PACK_Malaysia"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /D "_GTH_CHEAT_ENABLED" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "./DX Folder" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FXDEBUG" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_MALAYSIA_CHN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "NO_GAMEGUARD" /D "_GTH_CHEAT_ENABLED" /D "_TERRA" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibPackDebug.lib mfclib.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:".\exe\dbg_pack_client.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 ijl15.lib dinput8.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathDebug.lib CommonDebug.lib CoreDebug.lib GTH_ZLibPackDebug.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\exe\client_my.exe" /pdbtype:sept /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "GTH - Win32 REL PACK Malaysia"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_PACK_Malaysia"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_PACK_Malaysia"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_PACK_Malaysia"
# PROP Intermediate_Dir "GTH___Win32_REL_PACK_Malaysia"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /Fr /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_MALAYSIA_CHN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "_TERRA" /D "_AHNLAB_HACKSHIELD" /D "NO_GAMEGUARD" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib /nologo /subsystem:windows /map /machine:I386 /out:".\EXE\rel_pack_on.exe" /libpath:"./lib"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib NPGameLib.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib mfclib.lib /nologo /subsystem:windows /map /machine:I386 /out:".\EXE\rel_pack_on.exe" /libpath:"./lib"
# SUBTRACT LINK32 /pdb:none /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copy Excutefile and Compress....
PostBuild_Cmds=copy .\exe\client.exe d:\hackshield\	d:\hackshield\UPX -9 d:\hackshield\client.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "GTH - Win32 REL PACK_TO_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "GTH___Win32_REL_PACK_TO_GM"
# PROP BASE Intermediate_Dir "GTH___Win32_REL_PACK_TO_GM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "GTH___Win32_REL_PACK_TO_GM"
# PROP Intermediate_Dir "GTH___Win32_REL_PACK_TO_GM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /Fr /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./DX Folder" /D "_LIB" /D "WIN32" /D "NDEBUG" /D "MBCS" /D "_FX_CLIENT" /D "_GTH_ONLINE_VERSION" /D "_GTH_KOREAN_VERSION" /D "_GTH_NEW_VERSION" /D "_FMOD_SOUND" /D "_GTH_ZIP_VERSION_PACK" /D "_NEW_SERVER" /D "WINDOW_MODE" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib mfclib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\exe\client.exe" /libpath:"./lib" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ijl15.lib dxerr8.lib dxguid.lib strmiids.lib dsound.lib dsound.lib dinput8.lib dxerr8.lib d3dxof.lib dxguid.lib imm32.lib netlib.lib ws2_32.lib zlib.lib d3dx8dt.lib d3d8.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fmodvc.lib MathRelease.lib CommonRelease.lib CoreRelease.lib GTH_ZLibPackRelease.lib mfclib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\exe\client_kr_gm.exe" /libpath:"./lib" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=CopyExcuteFIle And Compress.... TarGet : E:\Project\GTH_PatchData\KOREA\NProtect
PostBuild_Cmds=E:\PROJECT\GTH_PatchData\KOREA\NProtect\npgeclient1.exe /gth:gth:E:\PROJECT\GTH_PatchData\KOREA\NProtect\client.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "GTH - Win32 DBG PACK Chinese"
# Name "GTH - Win32 DBG I3D Chinese"
# Name "GTH - Win32 DBG I3D"
# Name "GTH - Win32 DBG PACK"
# Name "GTH - Win32 REL PACK"
# Name "GTH - Win32 REL PACK Taiwan"
# Name "GTH - Win32 REL I3D"
# Name "GTH - Win32 DBG I3D OFF"
# Name "GTH - Win32 DBG I3D REMOTE"
# Name "GTH - Win32 REL PACK_Chinese"
# Name "GTH - Win32 REL PACK CHEATON"
# Name "GTH - Win32 DBG PACK Malaysia"
# Name "GTH - Win32 REL PACK Malaysia"
# Name "GTH - Win32 REL PACK_TO_GM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Logic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Logic\Cursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\EffectMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\EntityMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\ItemMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\Move.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\RainDropper.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\SnowDropper.cpp
# End Source File
# Begin Source File

SOURCE=.\Logic\TimeSystem.cpp
# End Source File
# End Group
# Begin Group "Model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Model\MDL.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MDLEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MDLEffectRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MDLItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MDLMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MDLRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MDLWorld.cpp
# End Source File
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\BGMusic.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound\MusicMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound\MusicMng_fmod.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound\Samples.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundMng.cpp
# End Source File
# End Group
# Begin Group "Client Process"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\App.cpp
# End Source File
# Begin Source File

SOURCE=.\AppLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\AppSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\CharProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\GameProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadingProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\StartProcess.cpp
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "GameEvent"

# PROP Default_Filter ""
# Begin Group "ChristMasEvent"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\ChristMasEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ChristMasEvent.h
# End Source File
# End Group
# End Group
# Begin Group "PostSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\PostFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\PostFunc.h
# End Source File
# Begin Source File

SOURCE=.\game\PostSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\PostSystem.h
# End Source File
# End Group
# Begin Group "Flag"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\GameConfigFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GameConfigFlag.h
# End Source File
# End Group
# Begin Group "WorldChange"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\WorldChangeFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\WorldChangeFunc.h
# End Source File
# Begin Source File

SOURCE=.\game\WorldChangeSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\WorldChangeSystem.h
# End Source File
# End Group
# Begin Group "WeatherSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\WeatherSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\WeatherSystem.h
# End Source File
# End Group
# Begin Group "HelperSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\HelperSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\HelperSystem.h
# End Source File
# Begin Source File

SOURCE=.\game\IFHelperWin.cpp
# End Source File
# Begin Source File

SOURCE=.\game\IFHelperWin.h
# End Source File
# End Group
# Begin Group "ExpManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\ExpEventMng.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ExpEventMng.h
# End Source File
# End Group
# Begin Group "CashMallItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\CashMall_Item.cpp
# End Source File
# Begin Source File

SOURCE=.\game\CashMall_Item.h
# End Source File
# Begin Source File

SOURCE=.\game\CashMallItem_optionStone.cpp
# End Source File
# Begin Source File

SOURCE=.\game\CashMallItem_OptionStone.h
# End Source File
# End Group
# Begin Group "GambleSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\GambleSystem.h
# End Source File
# Begin Source File

SOURCE=.\game\GambleSystemFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GambleSystemFunc.h
# End Source File
# End Group
# Begin Group "ItemExtendSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\ItemExtendSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemExtendSystem.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemExtendSystemFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemExtendSystemFunc.h
# End Source File
# End Group
# Begin Group "NEW_DAN_BATTLE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\NewDanBattleFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\NewDanBattleFunc.h
# End Source File
# Begin Source File

SOURCE=.\game\NewDanBattleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\NewDanBattleSystem.h
# End Source File
# Begin Source File

SOURCE=.\game\tagNewDanPacket.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\game\ChatCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\game\client_global.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ConfigApp.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Dan.cpp
# End Source File
# Begin Source File

SOURCE=.\game\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GameUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Option.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Token.cpp
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Group "PC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\net_pc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Net_PcCommunity.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Net_PcItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Net_PcParty.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Net_PcSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Net_PcTrade.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Network\net_event.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\net_item.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\net_main.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\net_master.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\net_npc.cpp
# End Source File
# End Group
# Begin Group "Character"

# PROP Default_Filter ""
# Begin Group "Manager"

# PROP Default_Filter ""
# Begin Group "CharManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Managers\CharManager_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharManager_Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharManager_Creature.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharManager_Generate.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharManager_Tool.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Character\FX_Managers\AnimationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\AnimationManager.inl
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\AuraManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharLookupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharShaderManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\ItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\SkeletonManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\SkinManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\TagManager.cpp
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Renderer\AuraRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Renderer\ItemRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Renderer\SkinRenderer.cpp
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Base\Animation.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Base\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Base\Skeleton.cpp
# End Source File
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Etcs\Utilities.cpp
# End Source File
# End Group
# End Group
# Begin Group "Overlay"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Overlay\ChatManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Overlay\ChatManager_Chn.cpp
# End Source File
# Begin Source File

SOURCE=.\Overlay\ChatManager_Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Overlay\ChatManager_Jpn.cpp
# End Source File
# Begin Source File

SOURCE=.\Overlay\ChatManager_Kor.cpp
# End Source File
# Begin Source File

SOURCE=.\Overlay\ChatManager_Twn.cpp
# End Source File
# End Group
# Begin Group "TableStruct"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TableStruct\GameTable.cpp
# End Source File
# Begin Source File

SOURCE=.\TableStruct\TableLoader.cpp
# End Source File
# End Group
# Begin Group "Language"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Language\Language.cpp
# End Source File
# Begin Source File

SOURCE=.\Language\LanguageManager.cpp
# End Source File
# End Group
# Begin Group "IF"

# PROP Default_Filter ""
# Begin Group "IF Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IF\IFCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFCharacterInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFCharacterInventory.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFCommunity.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFGame.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFItemSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFLoginWin.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFRolloverWin.cpp
# End Source File
# End Group
# Begin Group "IF Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IF\IFBarCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFButtonCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFControl.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFIconCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFNumCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFSpinCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFText.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFWindow.cpp
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IF\EventCraftSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\EventCraftSystem.h
# End Source File
# Begin Source File

SOURCE=.\IF\Item_ScrollWin.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\Item_ScrollWin.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemRecycleProc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemRecycleProc.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemTableData.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemTableData.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemUseProc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemUseProc.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\IF\IFLoginManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFManager_Rollover.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFMouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\IF\IFPhysics.cpp
# End Source File
# End Group
# Begin Group "ClientScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClientScript\ClientFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientScript\ClientScript.cpp
# End Source File
# End Group
# Begin Group "DebuggingLog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DebuggingLog\Ahndbglog.cpp
# End Source File
# Begin Source File

SOURCE=.\DebuggingLog\VUtil.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CGameVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\DxInput.cpp
# End Source File
# Begin Source File

SOURCE=.\global_def.cpp
# End Source File
# Begin Source File

SOURCE=.\GTH.cpp
# End Source File
# Begin Source File

SOURCE=.\GTH.rc
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Input.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Logic Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Logic\Cursor.h
# End Source File
# Begin Source File

SOURCE=.\Logic\EffectMng.h
# End Source File
# Begin Source File

SOURCE=.\Logic\EntityMng.h
# End Source File
# Begin Source File

SOURCE=.\Logic\ItemMng.h
# End Source File
# Begin Source File

SOURCE=.\Logic\Move.h
# End Source File
# Begin Source File

SOURCE=.\Logic\RainDropper.h
# End Source File
# Begin Source File

SOURCE=.\Logic\SnowDropper.h
# End Source File
# Begin Source File

SOURCE=.\Logic\TimeSystem.h
# End Source File
# End Group
# Begin Group "Model Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Model\MDL.h
# End Source File
# Begin Source File

SOURCE=.\Model\MDLMng.h
# End Source File
# End Group
# Begin Group "Sound Head"

# PROP Default_Filter ""
# Begin Group "fmod"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Sound\fmod.h
# End Source File
# Begin Source File

SOURCE=.\Sound\fmod_errors.h
# End Source File
# Begin Source File

SOURCE=.\Sound\fmoddyn.h
# End Source File
# Begin Source File

SOURCE=.\Sound\wincompat.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Sound\MusicMng.h
# End Source File
# Begin Source File

SOURCE=.\Sound\MusicMng_fmod.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundID.h
# End Source File
# Begin Source File

SOURCE=.\Sound\SoundMng.h
# End Source File
# End Group
# Begin Group "Game Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\client_common.h
# End Source File
# Begin Source File

SOURCE=.\game\client_global.h
# End Source File
# Begin Source File

SOURCE=.\game\ConfigApp.h
# End Source File
# Begin Source File

SOURCE=.\game\Dan.h
# End Source File
# Begin Source File

SOURCE=.\game\game.h
# End Source File
# Begin Source File

SOURCE=.\game\gameError.h
# End Source File
# Begin Source File

SOURCE=.\game\Option.h
# End Source File
# Begin Source File

SOURCE=.\game\Token.h
# End Source File
# End Group
# Begin Group "Network Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\net_main.h
# End Source File
# Begin Source File

SOURCE=.\Network\netlib.h
# End Source File
# Begin Source File

SOURCE=.\Network\protocol.h
# End Source File
# End Group
# Begin Group "Character Head"

# PROP Default_Filter ""
# Begin Group "Manager Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Managers\AnimationManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\AuraManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\CharacterManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharLookupManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\CharShaderManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\ItemManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\SkeletonManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\SkinManager.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Managers\TagManager.h
# End Source File
# End Group
# Begin Group "Renderer Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Renderer\AuraRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Renderer\ItemRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Renderer\SkinRenderer.h
# End Source File
# End Group
# Begin Group "Base Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Base\Animation.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Base\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Base\Skeleton.h
# End Source File
# End Group
# Begin Group "Etc Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Character\FX_Etcs\RenderRelated.h
# End Source File
# Begin Source File

SOURCE=.\Character\FX_Etcs\Utilities.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Character\Common.h
# End Source File
# End Group
# Begin Group "Overlay Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Overlay\ChatManager.h
# End Source File
# Begin Source File

SOURCE=.\imm\ime.h
# End Source File
# End Group
# Begin Group "TableStruct Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TableStruct\GameTable.h
# End Source File
# Begin Source File

SOURCE=.\TableStruct\TableCommon.h
# End Source File
# Begin Source File

SOURCE=.\TableStruct\TableOldStruct.h
# End Source File
# Begin Source File

SOURCE=.\TableStruct\TableStruct.h
# End Source File
# End Group
# Begin Group "Language Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Language\Language.h
# End Source File
# Begin Source File

SOURCE=.\Language\LanguageManager.h
# End Source File
# End Group
# Begin Group "IF Head"

# PROP Default_Filter ""
# Begin Group "IF Window Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IF\IFCharacter.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFCommunity.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFGame.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFItemSystem.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFLoginWin.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFRolloverWin.h
# End Source File
# End Group
# Begin Group "IF Control Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IF\IFBarCtl.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFButtonCtl.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFComboBox.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFControl.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFIconCtl.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFNumCtl.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFSpinCtl.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFText.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFWindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\IF\IFCommon.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFID.H
# End Source File
# Begin Source File

SOURCE=.\IF\IFManager.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFMouseCursor.h
# End Source File
# Begin Source File

SOURCE=.\IF\IFPhysics.h
# End Source File
# End Group
# Begin Group "ClientScript Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClientScript\ClientScript.h
# End Source File
# End Group
# Begin Group "DebuggingLog Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DebuggingLog\Ahndbglog.h
# End Source File
# Begin Source File

SOURCE=.\DebuggingLog\VCommon.h
# End Source File
# Begin Source File

SOURCE=.\DebuggingLog\VUtil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CGameVersion.h
# End Source File
# Begin Source File

SOURCE=.\DxInput.h
# End Source File
# Begin Source File

SOURCE=.\FuntionObject.h
# End Source File
# Begin Source File

SOURCE=.\global_def.h
# End Source File
# Begin Source File

SOURCE=.\GTH.h
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\Input.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\arrowcop.cur
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\DirectX.ico
# End Source File
# End Group
# Begin Group "Text File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exe\data\Tutorial.txt
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# End Group
# Begin Group "KJG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CTools.cpp
# End Source File
# Begin Source File

SOURCE=.\CTools.h
# End Source File
# Begin Source File

SOURCE=.\tagGCPacket.h
# End Source File
# Begin Source File

SOURCE=.\tagMCPacket.h
# End Source File
# End Group
# Begin Group "Battle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AttackProc.cpp
# End Source File
# Begin Source File

SOURCE=.\AttackProc.h
# End Source File
# Begin Source File

SOURCE=.\GameTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GameTarget.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillApp.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillApp.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataGlobal.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillFile.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillFile.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillItem.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillItem.h
# End Source File
# Begin Source File

SOURCE=.\game\SkillLogic.cpp
# End Source File
# Begin Source File

SOURCE=.\game\SkillLogic.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTable_DataDefine.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTable_DataValue.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTableDefine.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTableMap.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTableMap.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTransInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTransInfo.h
# End Source File
# End Group
# Begin Group "Terra ICT"

# PROP Default_Filter ""
# Begin Group "GIFT CARD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Terra ICT\GIFT CARD\GiftCard.cpp"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\GIFT CARD\GiftCard.h"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\GIFT CARD\IFGiftCardWin.cpp"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\GIFT CARD\IFGiftCardWin.h"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\GIFT CARD\tagGiftCardPacket.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\Terra ICT\CTerraCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\CTerraCtrl.h"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\CTerraLpack.cpp"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\CTerraLpack.h"
# End Source File
# Begin Source File

SOURCE=".\Terra ICT\tagTerraPacket.h"
# End Source File
# End Group
# Begin Group "GolryunBattle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle.h
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle_Command.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle_Command.h
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle_Decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle_Decoder.h
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle_Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\GolryunBattle_Encoder.h
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\IFGolryunBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GolryunBattle\IFGolryunBattle.h
# End Source File
# End Group
# Begin Group "Guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Guild\CGuildCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Guild\CGuildCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Guild\IFPactMarkManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Guild\IFPactMarkManager.h
# End Source File
# End Group
# Begin Group "GM(Game Master)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl.h"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl_ChatBlockCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl_ChatBlockCtrl.h"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl_PKTimeCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl_PKTimeCtrl.h"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\tagGMPacket.h"
# End Source File
# End Group
# Begin Group "GonryunBattlePractice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GonryunBattlePractice\GonryunBattlePractice.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunBattlePractice\GonryunBattlePractice.h
# End Source File
# End Group
# End Target
# End Project
