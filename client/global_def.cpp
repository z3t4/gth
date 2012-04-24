#include "../global_def.h"




CShaderMng      *g_modelShaderMng;
CShaderMng      *g_charShaderMng;
CMove           *g_move;
CEntityMng      *g_entityMng;
CItemMng        *g_itemMng;
CEffectMng      *g_effectMng;



CCursor         *g_cursor;
CTimeSystem     *g_timeSystem;
CMusicMng       *g_musicMng;
CSoundMng		*g_soundMng;
CTutorialMng	*g_TutorialMng;				
CLanguageManager g_LPACK;					



input_t         *input;   

FX_CCharManager *g_charManager;

GTH*             g_pApp;
HINSTANCE        g_hInst;

timecheck_t      check;


CChatManager	*g_chatManager;
CInputMng		*g_input;
CIFManager		*g_ifMng;



CGuildCtrl*      g_cpGuildCtrl=NULL;


CDanBattle		g_DanBattle;


CGonryunBattlePractice	g_GonryunBattlePractic;






CChristMasEvent g_ChristMasEvent;


CPostFunc		g_PostFunc;


CWeatherSystem		  g_WeatherSystem;


CHelperSystem		  g_HelperSystem;




CGiftCard			  g_GiftCard;



CExpRateMng			 g_ExpRateMng;


NewDanBattleSystem	  g_NewDanBattleSystem;




 
chatMessage	g_chatMessageBuffer [MAX_CHAT_MESSAGE_COUNT];
int			g_chatMessageCount	=	0;
float		g_chatMessageTimeClamp [MAX_CHAT_MESSAGE_COUNT];

	 
#ifndef	_GTH_ONLINE_VERSION
	int	g_speedModifier	=	1.0f;
#endif
