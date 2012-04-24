# Microsoft Developer Studio Project File - Name="masterserver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=masterserver - Win32 TEST_SERVER_Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "masterserver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "masterserver.mak" CFG="masterserver - Win32 TEST_SERVER_Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "masterserver - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "masterserver - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "masterserver - Win32 TEST_SERVER_Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MasterServer_Kor", PBUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "masterserver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MASTER_SERVER" /D "_TEST_SERVER" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /machine:I386 /out:"exe/gth_mserver.exe" /libpath:"./lib"

!ELSEIF  "$(CFG)" == "masterserver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MASTER_SERVER" /D "_TEST_SERVER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\exe\gth_mserver.exe" /pdbtype:sept /libpath:"./lib"

!ELSEIF  "$(CFG)" == "masterserver - Win32 TEST_SERVER_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "masterserver___Win32_TEST_SERVER_Debug"
# PROP BASE Intermediate_Dir "masterserver___Win32_TEST_SERVER_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "masterserver___Win32_TEST_SERVER_Debug"
# PROP Intermediate_Dir "masterserver___Win32_TEST_SERVER_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_TEST_SERVER" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"exe/gth_mserver.exe" /pdbtype:sept /libpath:"./lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"./exe/gth_mserver.exe" /pdbtype:sept /libpath:"./lib"

!ENDIF 

# Begin Target

# Name "masterserver - Win32 Release"
# Name "masterserver - Win32 Debug"
# Name "masterserver - Win32 TEST_SERVER_Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\network\net_main.cpp
# End Source File
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\etc\Days.cpp
# End Source File
# Begin Source File

SOURCE=.\etc\Days.h
# End Source File
# Begin Source File

SOURCE=.\etc\math.cpp
# End Source File
# Begin Source File

SOURCE=.\etc\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\etc\Token.cpp
# End Source File
# End Group
# Begin Group "Billing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Billing\BillingForChina.cpp
# End Source File
# Begin Source File

SOURCE=.\Billing\CriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=.\Billing\tcpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Billing\Thread.cpp
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\GameConfigFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\game\GameConfigFlag.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CheckSum.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\logic.cpp
# End Source File
# Begin Source File

SOURCE=.\winmain.cpp
# End Source File
# Begin Source File

SOURCE=.\winmain.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Network Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\network\net_main.h
# End Source File
# Begin Source File

SOURCE=.\network\protocol.h
# End Source File
# End Group
# Begin Group "Etc Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\etc\math.h
# End Source File
# Begin Source File

SOURCE=.\etc\Timer.h
# End Source File
# Begin Source File

SOURCE=.\etc\Token.h
# End Source File
# End Group
# Begin Group "Billing Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Billing\Billing.h
# End Source File
# Begin Source File

SOURCE=.\Billing\CriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\Billing\tcpSocket.h
# End Source File
# Begin Source File

SOURCE=.\Billing\Thread.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CheckSum.h
# End Source File
# Begin Source File

SOURCE=.\dataStruct.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\logic.h
# End Source File
# Begin Source File

SOURCE=.\monitor.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\winmain.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ms.ico
# End Source File
# Begin Source File

SOURCE=.\small.ico
# End Source File
# End Group
# Begin Group "Script Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exe\tables\chargenerate_kr.tbl
# End Source File
# Begin Source File

SOURCE=.\exe\server.cfg
# End Source File
# End Group
# Begin Group "KJG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPremiumPlayerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CPremiumPlayerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CTools.cpp
# End Source File
# Begin Source File

SOURCE=.\CTools.h
# End Source File
# Begin Source File

SOURCE=.\tagMCPacket.h
# End Source File
# End Group
# Begin Group "ODBC(Open Database Connectivity)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\database.cpp
# End Source File
# Begin Source File

SOURCE=.\database.h
# End Source File
# End Group
# Begin Group "Terra"

# PROP Default_Filter ""
# Begin Group "Billing No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Terra\Billing\A must be read this document terra billing apply manual.txt"
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_CircularQueueCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_CircularQueueCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_CommunicationCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_CommunicationCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_Decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_Decoder.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_Encoder.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_LogCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_LogCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_ReceiveCircualrQueueCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\CTerraBillingCtrl_ReceiveCircualrQueueCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\tagAddBill.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\tagTerraBilling.h
# End Source File
# Begin Source File

SOURCE=.\Terra\Billing\tagTerraPacket.h
# End Source File
# End Group
# End Group
# Begin Group "Log"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\log\CLogFileCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\log\CLogFileCtrl.h
# End Source File
# Begin Source File

SOURCE=.\log\CLogSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\log\CLogSystem.h
# End Source File
# End Group
# Begin Group "HiCocoon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HiCocoon\DaumGameAuth.cpp
# End Source File
# Begin Source File

SOURCE=.\HiCocoon\DaumGameAuth.h
# End Source File
# Begin Source File

SOURCE=.\HiCocoon\DaumGameCrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\HiCocoon\DaumGameCrypt.h
# End Source File
# Begin Source File

SOURCE=.\HiCocoon\HiCocoonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HiCocoon\HiCocoonCtrl.h
# End Source File
# End Group
# End Target
# End Project
