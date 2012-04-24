# Microsoft Developer Studio Project File - Name="gameserver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=gameserver - Win32 DEV_TEST_Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gameserver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gameserver.mak" CFG="gameserver - Win32 DEV_TEST_Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gameserver - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "gameserver - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "gameserver - Win32 DEV_TEST_Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GTH_SERVER", LLCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gameserver - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_GTH_KOREAN_VERSION" /D "_GAME_SERVER" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib_release.lib /nologo /subsystem:windows /map /machine:I386 /out:".\exe\GTH_GameSvr_rel.exe" /libpath:"./lib"

!ELSEIF  "$(CFG)" == "gameserver - Win32 Debug"

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
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_GTH_KOREAN_VERSION" /D "_GAME_SERVER" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"msvcprtd.lib" /out:".\exe\GTHGameserver.exe" /pdbtype:sept /libpath:"./lib"
# SUBTRACT LINK32 /profile /nodefaultlib

!ELSEIF  "$(CFG)" == "gameserver - Win32 DEV_TEST_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "gameserver___Win32_DEV_TEST_Debug"
# PROP BASE Intermediate_Dir "gameserver___Win32_DEV_TEST_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "gameserver___Win32_DEV_TEST_Debug"
# PROP Intermediate_Dir "gameserver___Win32_DEV_TEST_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_GTH_KOREAN_VERSION" /D "_GAME_SERVER" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_GTH_KOREAN_VERSION" /D "_GAME_SERVER" /D "_DEV_TEST" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"msvcprtd.lib" /out:".\exe\GTHGameserver.exe" /pdbtype:sept /libpath:"./lib"
# SUBTRACT BASE LINK32 /profile /nodefaultlib
# ADD LINK32 ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib netlib.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"msvcprtd.lib" /out:".\exe\GTHGameserver.exe" /pdbtype:sept /libpath:"./lib"
# SUBTRACT LINK32 /profile /nodefaultlib

!ENDIF 

# Begin Target

# Name "gameserver - Win32 Release"
# Name "gameserver - Win32 Debug"
# Name "gameserver - Win32 DEV_TEST_Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Group "Monster AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai\interpretor.cpp
# End Source File
# Begin Source File

SOURCE=.\ai\monsterAIEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\ai\MonsterAIFunction.cpp
# End Source File
# End Group
# Begin Group "NPC AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai\npcAIEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\ai\npcAIFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\ai\npcInterpretor.cpp
# End Source File
# End Group
# Begin Group "PC AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai\pcAIFunction.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ai\collisionmap.cpp
# End Source File
# Begin Source File

SOURCE=.\ai\pathfinding.cpp
# End Source File
# End Group
# Begin Group "Message"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\message\message.cpp
# End Source File
# Begin Source File

SOURCE=.\message\MessageReceive.cpp
# End Source File
# Begin Source File

SOURCE=.\message\MessageSend.cpp
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\network\Gms_To_Gs.cpp
# End Source File
# Begin Source File

SOURCE=.\network\Gms_To_Gs.h
# End Source File
# Begin Source File

SOURCE=.\network\Gs_To_Cc.cpp
# End Source File
# Begin Source File

SOURCE=.\network\Gs_To_Cc.h
# End Source File
# Begin Source File

SOURCE=.\network\Gs_To_Gms.cpp
# End Source File
# Begin Source File

SOURCE=.\network\Gs_To_Gms.h
# End Source File
# Begin Source File

SOURCE=.\network\net_event.cpp
# End Source File
# Begin Source File

SOURCE=.\network\net_item.cpp
# End Source File
# Begin Source File

SOURCE=.\network\net_main.cpp
# End Source File
# Begin Source File

SOURCE=.\network\net_monster.cpp
# End Source File
# Begin Source File

SOURCE=.\network\net_npc.cpp
# End Source File
# Begin Source File

SOURCE=.\network\net_pc.cpp
# End Source File
# Begin Source File

SOURCE=.\network\net_server.cpp
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "Battle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\battle\Attack.cpp
# End Source File
# Begin Source File

SOURCE=.\battle\Attack.h
# End Source File
# Begin Source File

SOURCE=.\battle\GameTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\battle\GameTarget.h
# End Source File
# End Group
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
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\EventCraftSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\EventCraftSystem.h
# End Source File
# Begin Source File

SOURCE=.\game\Item_Scroll_Proc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Item_Scroll_Proc.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemTableData.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemTableData.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemTxt.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemTxt.h
# End Source File
# Begin Source File

SOURCE=.\game\UseItem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\UseItem.h
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

SOURCE=.\game\CashMallItem_OptionStone.h
# End Source File
# End Group
# Begin Group "GambleSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\GambleSystem.cpp
# End Source File
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

SOURCE=.\game\ItemExtendFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemExtendFunc.h
# End Source File
# Begin Source File

SOURCE=.\game\ItemExtendSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\ItemExtendSystem.h
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

SOURCE=.\game\NewDanBattleFunc_Broadcast.cpp
# End Source File
# Begin Source File

SOURCE=.\game\NewDanBattleFunc_Broadcast.h
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

SOURCE=.\game\Affect.cpp
# End Source File
# Begin Source File

SOURCE=.\game\bopeitem.cpp
# End Source File
# Begin Source File

SOURCE=.\game\CheatCode.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Dan.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Friends.cpp
# End Source File
# Begin Source File

SOURCE=.\game\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game\guild.cpp
# End Source File
# Begin Source File

SOURCE=.\game\monster.cpp
# End Source File
# Begin Source File

SOURCE=.\game\MonsterCal.cpp
# End Source File
# Begin Source File

SOURCE=.\game\npc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\Option.cpp
# End Source File
# Begin Source File

SOURCE=.\game\party.cpp
# End Source File
# Begin Source File

SOURCE=.\game\pc.cpp
# End Source File
# Begin Source File

SOURCE=.\game\pc_logic.cpp
# End Source File
# Begin Source File

SOURCE=.\game\PcCal.cpp
# End Source File
# Begin Source File

SOURCE=.\game\world.cpp
# End Source File
# Begin Source File

SOURCE=.\game\zone.cpp
# End Source File
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\etc\math.cpp
# End Source File
# Begin Source File

SOURCE=.\etc\mtrand.cpp
# End Source File
# Begin Source File

SOURCE=.\etc\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\etc\Token.cpp
# End Source File
# End Group
# Begin Group "TableStruct"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TableStruct\TableCommon.cpp
# End Source File
# End Group
# Begin Group "Debug Info"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DebugInfo\ConsoleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugInfo\VUtil.cpp
# End Source File
# End Group
# Begin Group "Skill"

# PROP Default_Filter ""
# Begin Group "NewVer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\skill\SkillCast.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillCast.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataAffect.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataAffect.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataAffectCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataAffectCommon.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataAttack.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataBasic.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataBasic.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataCheckProc.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataCheckProc.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataGlobal.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataMegProc.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataMegProc.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataSubProc.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataSubProc.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillPartyProc.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillPartyProc.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillProcAffectSpe.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillProcAffectSpe.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTableMap.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTableMap.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTalbeDefine.h
# End Source File
# End Group
# Begin Group "OldVer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\skill\Skill_Ear.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\Skill_Etc.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\Skill_Men.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\skill_mon.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\Skill_Wat.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\Skill_Win.cpp
# End Source File
# End Group
# Begin Group "Proc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\skill\PcSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\PcSkill.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillApp.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillApp.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillDataDefine.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillItem.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillItem.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillLogic.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillLogic.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillProc.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillProc.h
# End Source File
# Begin Source File

SOURCE=.\skill\Skills.h
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTransInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\skill\SkillTransInfo.h
# End Source File
# End Group
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
# Begin Source File

SOURCE=.\EntityListMng.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\log.cpp
# End Source File
# Begin Source File

SOURCE=.\systemAdmin.cpp
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
# Begin Group "AI Header"

# PROP Default_Filter ""
# Begin Group "Monster AI Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai\interpretor.h
# End Source File
# Begin Source File

SOURCE=.\ai\monsterAIEvent.h
# End Source File
# Begin Source File

SOURCE=.\ai\MonsterAIFunction.h
# End Source File
# End Group
# Begin Group "NPC AI Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai\npcInterpretor.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ai\collisionmap.h
# End Source File
# Begin Source File

SOURCE=.\ai\pathfinding.h
# End Source File
# End Group
# Begin Group "Structure Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\structure\AICommon.h
# End Source File
# Begin Source File

SOURCE=.\structure\entityStruct.h
# End Source File
# Begin Source File

SOURCE=.\structure\ItemObjectStruct.h
# End Source File
# Begin Source File

SOURCE=.\structure\monsterStruct.h
# End Source File
# Begin Source File

SOURCE=.\structure\npcStruct.h
# End Source File
# Begin Source File

SOURCE=.\structure\pcStruct.h
# End Source File
# End Group
# Begin Group "Message Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\message\message.h
# End Source File
# Begin Source File

SOURCE=.\message\MessageReceive.h
# End Source File
# Begin Source File

SOURCE=.\message\MessageSend.h
# End Source File
# End Group
# Begin Group "Network Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\network\net_main.h
# End Source File
# Begin Source File

SOURCE=.\network\netlib.h
# End Source File
# Begin Source File

SOURCE=.\network\protocol.h
# End Source File
# Begin Source File

SOURCE=.\network\queue.h
# End Source File
# End Group
# Begin Group "Game Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\Affect.h
# End Source File
# Begin Source File

SOURCE=.\game\bopeitem.h
# End Source File
# Begin Source File

SOURCE=.\game\Dan.h
# End Source File
# Begin Source File

SOURCE=.\game\Friends.h
# End Source File
# Begin Source File

SOURCE=.\game\game.h
# End Source File
# Begin Source File

SOURCE=.\game\gameError.h
# End Source File
# Begin Source File

SOURCE=.\game\guild.h
# End Source File
# Begin Source File

SOURCE=.\game\monster.h
# End Source File
# Begin Source File

SOURCE=.\game\npc.h
# End Source File
# Begin Source File

SOURCE=.\game\Option.h
# End Source File
# Begin Source File

SOURCE=.\game\party.h
# End Source File
# Begin Source File

SOURCE=.\game\pc.h
# End Source File
# Begin Source File

SOURCE=.\game\world.h
# End Source File
# Begin Source File

SOURCE=.\game\zone.h
# End Source File
# End Group
# Begin Group "Etc Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\etc\math.h
# End Source File
# Begin Source File

SOURCE=.\etc\mtrand.h
# End Source File
# Begin Source File

SOURCE=.\etc\Timer.h
# End Source File
# Begin Source File

SOURCE=.\etc\Token.h
# End Source File
# End Group
# Begin Group "TableStruct Header"

# PROP Default_Filter ""
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
# Begin Group "Debug Info Head"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DebugInfo\ConsoleInfo.h
# End Source File
# Begin Source File

SOURCE=.\DebugInfo\VCommon.h
# End Source File
# Begin Source File

SOURCE=.\DebugInfo\VUtil.h
# End Source File
# End Group
# Begin Group "Billing Header"

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
# Begin Group "CAhanLab_CpSrv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AhnLab\AhnLab.cpp
# End Source File
# Begin Source File

SOURCE=.\AhnLab\AhnLab.h
# End Source File
# Begin Source File

SOURCE=.\AhnLab\AntiCpSvr.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\CSAuth.h
# End Source File
# Begin Source File

SOURCE=.\EntityListMng.h
# End Source File
# Begin Source File

SOURCE=.\FuntionObject.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\log.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\systemAdmin.h
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
# Begin Group "Data Files"

# PROP Default_Filter ""
# End Group
# Begin Group "KJG(Kim Jin Gyu)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPremiumPlayerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CPremiumPlayerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\tagGCPacket.h
# End Source File
# End Group
# Begin Group "new"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\new.cpp
# End Source File
# Begin Source File

SOURCE=.\new.h
# End Source File
# End Group
# Begin Group "GolryunBattle"

# PROP Default_Filter ""
# Begin Group "Observer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Observer.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Observer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GolryunBattle\CCSVFileCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CCSVFileCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Common.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Common.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Data.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Data.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_DBV2.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_DBV2.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Decoder.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Encoder.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Log.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Log.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Process.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CGolryunBattle_Process.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CLandList.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CLandList.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CServerLpackCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\CServerLpackCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\tagGolryunBattle.h
# End Source File
# Begin Source File

SOURCE=.\GolryunBattle\tagGolryunBattlePacket.h
# End Source File
# End Group
# Begin Group "ODBC(open database connectivity)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\ODBC(open database connectivity)\database.cpp"
# End Source File
# Begin Source File

SOURCE=".\ODBC(open database connectivity)\database.h"
# End Source File
# Begin Source File

SOURCE=".\ODBC(open database connectivity)\tagDataBase.h"
# End Source File
# Begin Source File

SOURCE=".\ODBC(open database connectivity)\tagQuerySet.h"
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
# Begin Group "GiftCard"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Terra\GiftCard\A must be read this document terra Gift Card apply manul.txt"
# End Source File
# Begin Source File

SOURCE=.\Terra\GiftCard\CGiftCard.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\GiftCard\CGiftCard.h
# End Source File
# Begin Source File

SOURCE=.\Terra\GiftCard\tagGiftCardPacket.h
# End Source File
# End Group
# End Group
# Begin Group "HelperSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HelperSystem\CHelperManager_Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\HelperSystem\CHelperManager_Encoder.h
# End Source File
# Begin Source File

SOURCE=.\HelperSystem\HelperManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HelperSystem\HelperManager.h
# End Source File
# Begin Source File

SOURCE=.\HelperSystem\tagHelper.h
# End Source File
# Begin Source File

SOURCE=.\HelperSystem\tagHelperPacket.h
# End Source File
# End Group
# Begin Group "GonryunBattlePractice"

# PROP Default_Filter ""
# Begin Group "Reword"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_Ability.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_Ability.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_RewordCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_RewordCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_RewordItem.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_RewordItem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_Decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_Decoder.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_Encoder.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_ProcessCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_ProcessCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_WaiterCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPractice_WaiterCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPracticeBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\CGonryunPracticeBattle.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\tagGonryunPractice.h
# End Source File
# Begin Source File

SOURCE=.\GonryunPracticeBattle\tagGonryunPracticePacket.h
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

SOURCE=".\GM(Game Master)\CGMCtrl_ServerStateDataCtrl.cpp"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\CGMCtrl_ServerStateDataCtrl.h"
# End Source File
# Begin Source File

SOURCE=".\GM(Game Master)\tagGMPacket.h"
# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Tools\CTempPlayerInventoryX.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\CTempPlayerInventoryX.h
# End Source File
# Begin Source File

SOURCE=.\Tools\CTools.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools\CTools.h
# End Source File
# Begin Source File

SOURCE=.\LogEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\LogEvent.h
# End Source File
# End Group
# Begin Group "LogCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\log\CLogFileCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\log\CLogFileCtrl.h
# End Source File
# End Group
# Begin Group "Charactor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\YDCDamageCal.cpp
# End Source File
# Begin Source File

SOURCE=.\game\YDCDamageCal.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\compile option manual.txt"
# End Source File
# End Target
# End Project
