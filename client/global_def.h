#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H





#ifdef _GTH_CHINESE_VERSION
	
	#define	_GTH_ENABLE_GOODNOTICE	

	
	#define _GTH_FLAG_RELOGIN
#endif


#ifdef _DEBUG
	
	#define _GTH_FLAG_RELOGIN
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <dshow.h>
#include <dinput.h>					

#pragma warning( disable : 4786 )	


#include "DebuggingLog/Ahndbglog.h"

#include "DebuggingLog/VCommon.h"
#include "DebuggingLog/VUtil.h"

#include "./core/core.h"

#define WM_GRAPHNOTIFY WM_APP + 1   


#define GTH_DEPTH_BUFFER_ZBUFFER D3DZB_USEW

typedef int     tri_t[3];
typedef unsigned short   tri_short_t[3];


typedef int             GTH_RET_VALUE;

#define NULL                    0


#define QUEST_SCROLL_INDEX		1000
#define	QUESTSCROLL_ITEM_INDEX	869

#define	AUTOQUESTSCROLL_ITEM_INDEX	45


#define	SUMMONCARD_ITEM_INDEX	868





#include "../TableStruct/TableCommon.h"



#include "../game/ItemExtendSystemFunc.h"


#include "../game/ItemExtendSystem.h"

 
#ifdef	_FMOD_SOUND
#	include "sound/fmod.h"
#	include "sound/fmod_errors.h"
#	include	"sound/MusicMng_fmod.h"
#else

#endif


#include "game/postsystem.h"																	



#include "game/CashMallItem_OptionStone.h"

#include "model/MDL.h"
#include "model/MDLMng.h"
#include "logic/ItemMng.h"
#include "logic/move.h"
#include "logic/RainDropper.h"
#include "logic/SnowDropper.h"

#include "language/Language.h"											
#include "language/LanguageManager.h"									


#include "logic/TimeSystem.h"



#include "game/WorldChangeSystem.h"


#include "../overlay/chatManager.h"		
#include "game/client_common.h"			

#include "character/CharacterManager.h"

#include "logic/EntityMng.h"

#include "input.h"			
#include "DxInput.h"		
#include "GTH.h"

#include "sound/soundMng.h"


#include "game/client_global.h"			

#include "logic/EffectMng.h"
#include "logic/Cursor.h"

#include "clientscript/clientscript.h"									

#include "game/dan.h"																	

#include "game/Manager.h"

#include "GonryunBattlePractice\GonryunBattlePractice.h"
#include "FuntionObject.h"



#include "GameTarget.h"
#include "AttackProc.h"

#include "skill/SkillTable_DataValue.h"
#include "skill/SkillTable_DataDefine.h"
#include "skill/SkillTableDefine.h"
#include "skill/SkillApp.h"
#include "skill/SkillTableMap.h"

#include "game/skilllogic.h"

#include "skill/SkillDataGlobal.h"
#include "skill/SkillFile.h"
#include "skill/SkillItem.h"
#include "skill/SkillTransInfo.h"


#include "game/ChristMasEvent.h"


#include "game/PostFunc.h"


#include "game/GameConfigFlag.h"


#include "game/WorldChangeFunc.h"


#include "game/WeatherSystem.h"


#include "game/CashMall_Item.h"



#include "game/ItemTableData.h"
#include "game/ItemUseProc.h"
#include "game/ItemRecycleProc.h"


#include "game/HelperSystem.h"




#include "Terra ICT/GIFT CARD/GiftCard.h"


#include "game/ExpEventMng.h"


#include "game/GambleSystem.h"
#include "game/GambleSystemFunc.h"


#include "game/NewDanBattleFunc.h"
#include "game/NewDanBattleSystem.h"







extern CShaderMng      *g_modelShaderMng;
extern CShaderMng      *g_charShaderMng;
extern CMove           *g_move;
extern CEntityMng      *g_entityMng;
extern CItemMng        *g_itemMng;
extern CEffectMng      *g_effectMng;

extern CCursor         *g_cursor;
extern CTimeSystem     *g_timeSystem;
extern CMusicMng       *g_musicMng;

extern input_t         *input;   
extern FX_CCharManager *g_charManager;

extern GTH*             g_pApp;
extern HINSTANCE        g_hInst;


extern CChatManager	   *g_chatManager;

extern CSoundMng	   *g_soundMng;
extern CInputMng	   *g_input;
extern CIFManager	   *g_ifMng;




extern CTutorialMng		*g_TutorialMng;
extern CLanguageManager  g_LPACK;			





extern CDanBattle		g_DanBattle;													





extern CGonryunBattlePractice g_GonryunBattlePractic;







extern CChristMasEvent		  g_ChristMasEvent;


extern CPostFunc			  g_PostFunc;


extern CWeatherSystem		  g_WeatherSystem;


extern CHelperSystem		  g_HelperSystem;




extern CGiftCard			  g_GiftCard;


extern CExpRateMng			  g_ExpRateMng;


extern NewDanBattleSystem	  g_NewDanBattleSystem;







typedef struct
{
	CTimer timer;
	float  time0;
	float  time1;
	float  time2;
	float  time3;
	float  time4;
	float  time5;
	float  time6;
	float  time7;
	float  time8;
	float  time9;
	float  time10;
	float  time11;
	float  time12;
	float  time13;
	float  time14;
} timecheck_t;

extern timecheck_t check;

 
#define	MAX_CHAT_MESSAGE_COUNT	4
#define	MAX_ALLOWED_CHAT_TIME	5000



#ifdef _GTH_CHINESE_VERSION
	#define	SERVER_CONFIG_FILENAME	"server_chn.cfg"
#else
	#define	SERVER_CONFIG_FILENAME	"server.cfg"
#endif



extern chatMessage	g_chatMessageBuffer [MAX_CHAT_MESSAGE_COUNT];
extern int			g_chatMessageCount;
extern float		g_chatMessageTimeClamp [MAX_CHAT_MESSAGE_COUNT];

 
#ifndef	_GTH_ONLINE_VERSION
	extern int	g_speedModifier;
#endif


#include "Guild\CGuildCtrl.h"
extern CGuildCtrl* g_cpGuildCtrl;


#endif	