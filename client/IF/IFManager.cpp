



#include "../global_def.h"
#include "IFCommon.h"
#include "Shlwapi.h"
#include "../game/GolryunBattle/GolryunBattle.h"


#include "..\CTools.h"
extern CTools gcTools;
extern CItemUseProc g_ItemUseProc;





#define		NOTICE_RENDER_TIME			8000.0f


extern int g_freeMove;

CIFManager::CIFManager()
{
	
	

	
	memset( m_tempString, 0, sizeof( m_tempString ) );

	
	m_Visible = TRUE;
	m_VisibleName = TRUE;

}

CIFManager::~CIFManager()
{
}




void CIFManager::InitDeviceObjects()
{
	m_iPos			= new IFControlInfo_t[NUM_IF_POSITION];
	m_iBackPos		= new IFControlInfo_t[NUM_IF_POSITION];
	
	for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ ) m_winControl[i] = NULL;
	

	
	m_msgWin		= new CIFMsgWin;
	m_iconCtl		= new CIFIconCtl;
	m_cursor		= new CIFMouseCursor;
	m_numCtl		= new CIFNumCtl;

#ifdef _GTH_MALAYSIA_CHN_VERSION
	
	m_jystechLogo	= new CIFControl;
	m_malLogo		= new CIFControl;
	
#endif


	
	m_startLogo		= new CIFControl;

	
	m_loginWin		= new CIFLoginWin;

	
	m_loadingLogo	= new CIFControl;

	
	m_nameWin		= new CIFNameWin;
	m_charWin		= new CIFCharWin;
	m_itemWin		= new CIFItemWin;
	m_menuWin		= new CIFMenuWin;
	m_chatWin		= new CIFChatWin;
	m_storeWin		= new CIFStoreWin;
	m_rolloverWin	= new CIFRolloverWin;
	m_skillWin		= new CIFSkillWin;
	m_questWin		= new CIFQuestWin;
	m_optionWin		= new CIFOptionWin;
	m_tradeWin		= new CIFTradeWin;
	m_stackWin		= new CIFStackWin;
	m_dlgWin		= new CIFDlgWin;
	m_chatOptionWin = new CIFChatOptionWin;
	m_worldChatWin	= new CIFWorldChatWin;
	m_hotWin		= new CIFHotWin;
	m_depotWin		= new CIFDepotWin;
	m_learnWin		= new CIFLearnWin;
	m_helpWin		= new CIFHelpWin;
	m_partyWin		= new CIFPartyWin;
	m_gmWin			= new CIFGmWin;
	m_precocityWin	= new CIFPrecocityWin;
	m_craftWin		= new CIFCraftWin;
	
	
	m_EventCraftWin	= new CIFEventCraftWin;

	m_upgradeWin	= new CIFUpgradeWin;
	m_boothWin		= new CIFBoothWin;
	m_mapWin		= new CIFMapWin;
	m_miniMapWin	= new CIFMiniMapWin;
	m_tutorialWin	= new CIFTutorialWin;
	m_tipWin		= new CIFTipWin;
	m_settingWin	= new CIFSettingWin;
	m_craftHelpWin	= new CIFCraftHelpWin;
	
	m_communityWin	= new CIFCommunityWin;
	m_mailWin		= new CIFMailWin;
	m_mailEditWin	= new CIFMailEditWin;
	m_friendWin		= new CIFFriendWin;
	
	m_questscrollWin = new CIFQuestScrollWin;
	
	m_guildWin		= new CIFGuildWin;
	m_guildAuthWin	= new CIFGuildAuthWin;
	
	m_guildPvpWin	= new CIFGuildPvPWin;
	m_guildArenaWin	= new CIFGuildArenaWin;
	

	
	m_autoQuestWin	= new CIFAutoQuestWin;
	m_npcInvWin	= new CIFNpcInvWin;
	

	
	m_battleTicketWin	= new CIFBattleTicketWin;
	m_shareWin			= new CIFShareWin;
	m_tournamentWin     = new CIFTournamentWin;
	m_prizeWin			= new CIFPrizeWin;
	m_golryunInfoWin	= new CIFGolryunInfoWin;
	m_observerMenuWin	= new CIFObserverMenuWin;
	m_observerInfoWin   = new CIFObserverInfoWin;
	m_tournamentMiniWin	= new CIFTournamentMiniWin;
	

	
	m_guildArenaInfoWin = new CIFGuild_battle_info_win;	

	
	
	m_guildMarkMng	= new CIFPMManager;

	
	m_surgeryWin	= new CIFSurgeryWin;
	

	
	m_pgonryunBattlePractice	= new CIFGonryunBattlePractice;
	m_pgonryunBattleBattingwindow =new CIFBattingWindow;
	m_pGonryunBattleProgressInfo = new CIFGonryunBattleProgressInfo;
	

	
	m_PostWin					= new CIFPostWin;
	m_mailBoxWin				= new CIFMailBoxWin;
	

	
	m_WorldChangeWin			= new CIFWorld_ChangeWin;

	
	m_HelperWin					= new CIFHelperWin;
	

	
	m_GiftCardWin				= new CIFGiftCardWin;

	
	m_Item_ScrollWin			= new CItem_ScrollWin;

	
	m_ServerListWin				= new CIFServer_ListWin ;
	

	
	m_CashMallItem				= new CIFCashMallItem;

	
	m_DanWarRequestWin			= new CIFDanWarRequestWin;

	
	m_DanWarBattleWin			= new CIFDanWarBattleWin;




	m_controlFlag			=	false;
	m_changeStateFlag		=	false;
	m_renderLoadingFlag		=	false;
	m_renderFlag			=	true;
	m_rolloverAddFirstFlag	=	true;
	m_mapType				=	0;
	m_noticeMsg[0]			=	0;
	m_noticeTime			=	0.0f;
	
	m_msgTitleStr[0]		=	0;
	m_msgStr[0]				=	0;
	m_rolloverText[0]		=	0;
	m_GmNotice				= FALSE;


#ifdef  _GTH_MALAYSIA_CHN_VERSION
		m_bLogoStep = IF_JYSTECH_LOGO;
#else
		m_bLogoStep = IF_START_LOGO;
#endif


}

void CIFManager::SetTexManager( CD3DTextureMng *texMng )
{
	m_texMng = texMng;

	
	m_msgWin->SetTexManager( texMng );
	m_iconCtl->SetTexManager( texMng );
	m_numCtl->SetTexManager( texMng );

#ifdef _GTH_MALAYSIA_CHN_VERSION
	
	m_jystechLogo->SetTexManager( texMng );
	m_malLogo->SetTexManager( texMng );
	
#endif
	
	
	m_startLogo->SetTexManager( texMng );

	
	m_loginWin->SetTexManager( texMng );

	
	m_loadingLogo->SetTexManager( texMng );

	
	m_rolloverWin->SetTexManager( texMng );	
	m_itemWin->SetTexManager( texMng );
	m_depotWin->SetTexManager( texMng );
	m_precocityWin->SetTexManager( texMng );
	m_craftWin->SetTexManager( texMng );

	
	m_EventCraftWin->SetTexManager( texMng );
	

	m_upgradeWin->SetTexManager( texMng );
	m_nameWin->SetTexManager( texMng );
	m_charWin->SetTexManager( texMng );
	m_skillWin->SetTexManager( texMng );
	m_questWin->SetTexManager( texMng );
	m_hotWin->SetTexManager( texMng );
	m_learnWin->SetTexManager( texMng );
	m_chatWin->SetTexManager( texMng );
	m_storeWin->SetTexManager( texMng );
	m_tradeWin->SetTexManager( texMng );
	m_dlgWin->SetTexManager( texMng );
	m_chatOptionWin->SetTexManager( texMng );
	m_worldChatWin->SetTexManager( texMng );
	m_partyWin->SetTexManager( texMng );
	m_boothWin->SetTexManager( texMng );
	m_menuWin->SetTexManager( texMng );
	m_optionWin->SetTexManager( texMng );
	m_stackWin->SetTexManager( texMng );
	m_helpWin->SetTexManager( texMng );
	m_gmWin->SetTexManager( texMng );
	m_mapWin->SetTexManager( texMng );
	m_miniMapWin->SetTexManager( texMng );
	m_tutorialWin->SetTexManager( texMng );
	m_tipWin->SetTexManager( texMng );
	m_settingWin->SetTexManager( texMng );
	m_craftHelpWin->SetTexManager( texMng );
	
	m_communityWin->SetTexManager( texMng );
	m_mailWin->SetTexManager( texMng );
	m_mailEditWin->SetTexManager( texMng );
	m_friendWin->SetTexManager( texMng );
	
	m_questscrollWin->SetTexManager( texMng );
	
	m_guildWin->SetTexManager( texMng );
	m_guildAuthWin->SetTexManager( texMng );
	
	m_guildPvpWin->SetTexManager( texMng );
	m_guildArenaWin->SetTexManager( texMng );
	
	m_autoQuestWin->SetTexManager( texMng );
	m_npcInvWin->SetTexManager( texMng );
	

	
	m_guildArenaInfoWin->SetTexManager( texMng );
	
	
	m_surgeryWin->SetTexManager( texMng );
	
	
	
	m_pgonryunBattlePractice->SetTexManager(texMng);
	m_pgonryunBattleBattingwindow->SetTexManager(texMng);
	m_pGonryunBattleProgressInfo->SetTexManager(texMng);
	

	
	m_battleTicketWin->SetTexManager( texMng );
	m_shareWin->SetTexManager( texMng );
	m_tournamentWin->SetTexManager( texMng );
	m_prizeWin->SetTexManager( texMng );
	m_golryunInfoWin->SetTexManager( texMng );
	m_observerMenuWin->SetTexManager( texMng );
	m_observerInfoWin->SetTexManager( texMng );
	m_tournamentMiniWin->SetTexManager( texMng );
	

	
	m_PostWin->SetTexManager(texMng);
	m_mailBoxWin->SetTexManager(texMng);
	

	
	m_WorldChangeWin->SetTexManager(texMng);


	m_HelperWin->SetTexManager(texMng);


	
	m_GiftCardWin->SetTexManager(texMng);

	
	m_ServerListWin->SetTexManager(texMng);
	

	
	m_CashMallItem->SetTexManager(texMng);

	
	m_Item_ScrollWin->SetTexManager(texMng);

	
	m_DanWarRequestWin->SetTexManager(texMng);
	m_DanWarBattleWin->SetTexManager(texMng);

}

void CIFManager::SetFileManager( CFileMng *fileMng	)
{
	m_fileMng = fileMng;
	
	m_iconCtl->SetFileManager( fileMng );
	m_cursor->SetFileManager( fileMng );
}

void CIFManager::SetScript( CScript *script )
{
	m_script = script;

	m_iconCtl->SetScript( script );
}

void CIFManager::SetDevice( LPDIRECT3DDEVICE8 device )
{
	m_pd3dDevice = device;

	
	m_msgWin->SetDevice( device );
	m_iconCtl->SetDevice( device );
	m_numCtl->SetDevice( device );
	m_cursor->SetDevice( device );

#ifdef _GTH_MALAYSIA_CHN_VERSION
	
	m_jystechLogo->SetDevice( device );
	m_malLogo->SetDevice( device );
	
#endif

	
	m_startLogo->SetDevice( device );

	
	m_loginWin->SetDevice( device );

	
	m_loadingLogo->SetDevice( device );
	
	
	m_rolloverWin->SetDevice( device );
	m_itemWin->SetDevice( device );
	m_depotWin->SetDevice( device );
	m_precocityWin->SetDevice( device );
	m_craftWin->SetDevice( device );

	
	m_EventCraftWin->SetDevice( device );
	
	m_upgradeWin->SetDevice( device );
	m_nameWin->SetDevice( device );
	m_charWin->SetDevice( device );
	m_skillWin->SetDevice( device );
	m_questWin->SetDevice( device );
	m_learnWin->SetDevice( device );
	m_hotWin->SetDevice( device );
	m_chatWin->SetDevice( device );
	m_storeWin->SetDevice( device );
	m_tradeWin->SetDevice( device );
	m_partyWin->SetDevice( device );
	m_boothWin->SetDevice( device );
	m_dlgWin->SetDevice( device );
	m_chatOptionWin->SetDevice( device );
	m_worldChatWin->SetDevice( device );
	m_menuWin->SetDevice( device );
	m_optionWin->SetDevice( device );
	m_stackWin->SetDevice( device );
	m_helpWin->SetDevice( device );
	m_gmWin->SetDevice( device );
	m_mapWin->SetDevice( device );
	m_miniMapWin->SetDevice( device );
	m_tutorialWin->SetDevice( device );
	m_tipWin->SetDevice( device );
	m_settingWin->SetDevice( device );
	m_craftHelpWin->SetDevice( device );
	
	m_communityWin->SetDevice( device );
	m_mailWin->SetDevice( device );
	m_mailEditWin->SetDevice( device );
	m_friendWin->SetDevice( device );
	
	m_questscrollWin->SetDevice( device );
	
	m_guildWin->SetDevice( device );
	m_guildAuthWin->SetDevice( device );
	
	m_guildPvpWin->SetDevice( device );
	m_guildArenaWin->SetDevice( device );
	
	m_autoQuestWin->SetDevice( device );
	m_npcInvWin->SetDevice( device );
	
	m_guildArenaInfoWin->SetDevice( device );
	
	
	m_surgeryWin->SetDevice( device );
	

	
	m_pgonryunBattlePractice->SetDevice(device);
	m_pgonryunBattleBattingwindow->SetDevice(device);
	m_pGonryunBattleProgressInfo->SetDevice(device);
	

	
	m_battleTicketWin->SetDevice( device );
	m_shareWin->SetDevice( device );
	m_tournamentWin->SetDevice( device );
	m_prizeWin->SetDevice( device );
	m_golryunInfoWin->SetDevice( device );
	m_observerMenuWin->SetDevice( device );
	m_observerInfoWin->SetDevice( device );
	m_tournamentMiniWin->SetDevice( device );
	

	
	m_PostWin->SetDevice(device);
	m_mailBoxWin->SetDevice(device);
	

	
	m_WorldChangeWin->SetDevice(device);

	m_HelperWin->SetDevice(device);


	
	m_GiftCardWin->SetDevice(device);

	
	m_ServerListWin->SetDevice(device);
	
	
	
	m_CashMallItem->SetDevice(device);

	
	m_Item_ScrollWin->SetDevice(device);

	
	m_DanWarRequestWin->SetDevice(device);
	m_DanWarBattleWin->SetDevice(device);
}

void CIFManager::SetRenderState()
{
	
	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING         , FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE         , D3DCULL_NONE ); 
	m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE     , TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ZBIAS            , 0 );
	m_pd3dDevice->SetRenderState( D3DRS_ZFUNC            , D3DCMP_LESSEQUAL );
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE     , TRUE );
	
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE  , TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF         , 0x00 );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC        , D3DCMP_GREATER );


	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA    );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA   );

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,	TRUE );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_TEXTURETRANSFORMFLAGS , D3DTTFF_DISABLE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP );

	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLOROP   , D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1 , D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2 , D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP   , D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	m_pd3dDevice->SetTextureStageState( 1 , D3DTSS_COLOROP   , D3DTOP_DISABLE );
	m_pd3dDevice->SetTextureStageState( 1 , D3DTSS_ALPHAOP   , D3DTOP_DISABLE );

	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_MINFILTER , D3DTEXF_LINEAR );
    m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_MAGFILTER , D3DTEXF_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0 , D3DTSS_MIPFILTER , D3DTEXF_NONE );

	
	if(g_d3dRender.m_bFog)
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, true);


}

void CIFManager::RestartInterface( int type )
{
	switch( type )
	{
	case 0 :
		m_msgWin->Enable( false );
		m_nameWin->Enable();
		m_charWin->Enable( false );
		m_itemWin->Enable( false );
		m_menuWin->Enable();
		m_chatWin->Enable( false );
		m_storeWin->Enable( false );
		m_rolloverWin->Enable( false );
		m_skillWin->Enable( false );
		m_questWin->Enable( false );
		m_optionWin->Enable( false );
		m_tradeWin->Enable( false );
		m_stackWin->Enable( false );
		m_dlgWin->Enable();
		m_chatOptionWin->Enable( false );
		m_worldChatWin->Enable( false );
		m_hotWin->Enable();
		m_depotWin->Enable( false );
		m_learnWin->Enable( false );
		m_helpWin->Enable( false );
		m_partyWin->Enable( false );
		m_gmWin->Enable( false );
		m_precocityWin->Enable( false );
		m_craftWin->Enable( false );

		
		m_EventCraftWin->Enable( false );
	
		m_upgradeWin->Enable( false );
		m_mapWin->Enable( false );
		m_boothWin->Enable( false );
		m_tutorialWin->Enable( false );
		m_tipWin->Enable( false );
		m_settingWin->Enable( false );
		m_craftHelpWin->Enable( false );
		
		m_communityWin->Enable( false );
		m_mailWin->Enable( false );
		m_mailEditWin->Enable( false );
		m_friendWin->Enable( false );
		
		m_questscrollWin->Enable( false );
		
		m_guildWin->Enable( false );
		m_guildAuthWin->Enable( false );
		
		m_guildPvpWin->Enable( false );
		m_guildArenaWin->Enable( false );
		
		m_autoQuestWin->Enable(false);
		m_npcInvWin->Enable( false );

		
		m_guildArenaInfoWin->Enable(false);

		
		m_surgeryWin->Enable(false);
		
	
		
		m_pgonryunBattlePractice->Enable(false);
		m_pgonryunBattleBattingwindow->Enable(false);
		m_pGonryunBattleProgressInfo->Enable(false);
		

		
		m_battleTicketWin->Enable( false );
		m_shareWin->Enable( false );
		m_tournamentWin->Enable( false );
		m_prizeWin->Enable( false );
		m_golryunInfoWin->Enable( false );
		m_observerMenuWin->Enable( false );
		m_observerInfoWin->Enable( false );
		m_tournamentMiniWin->Enable( false );
		

		
		m_PostWin->Enable(false);
		m_mailBoxWin->Enable(false);
		

		
		m_WorldChangeWin->Enable(false);


		m_HelperWin->Enable(false);


		
		m_GiftCardWin->Enable(false);

		
		m_ServerListWin->Enable(false);
		

		
		m_CashMallItem->Enable(false);

		
		m_Item_ScrollWin->Enable(false,NULL,-1,0);

		
		m_DanWarRequestWin->Enable(false);
		m_DanWarBattleWin->Enable(false);

		
		


		
		break;
	case 1 :
		
		if( m_tradeWin->State() )
		{
			m_tradeWin->Enable( false );
			GTH_SendMessage_PCTrade_Exit();
		}
		
		if( m_pgonryunBattleBattingwindow->State() )
		{
			m_pgonryunBattleBattingwindow->Enable( false );
			GTH_SendMessage_PCTrade_ExitBatting();
	
		}
		
		
		if( m_boothWin->State() ) 
		{
			
			m_boothWin->Enable( false );

		}
		if( CheckConditionalWindow() )
		{
			if( m_storeWin->State() || m_chatWin->State() ||
				m_learnWin->State() || m_depotWin->State() || 
				m_precocityWin->State() ) GTH_SendMessage_RequestQuitNpcChat();
			
			m_storeWin->Enable( false );
			m_chatWin->Enable( false );
			m_learnWin->Enable( false );
			m_depotWin->Enable( false );
			m_precocityWin->Enable( false );

			if( m_craftWin->State() ) GTH_SendMessage_ItemSystem_CraftClose();

			
			if( m_EventCraftWin->State() ) GTH_SendMessage_ItemSystem_EventCraftClose();


			if( m_upgradeWin->State() ) GTH_SendMessage_ItemSystem_UpgradeClose();
			if( m_questscrollWin->State() ) GTH_SendMessage_QuestScroll_ObtainClose();
			if( m_npcInvWin->State() ) GTH_SendMessage_ItemSystem_NpcInventoryClose();	

			m_craftWin->Enable( false );
		
			
			m_EventCraftWin->Enable( false );

			m_upgradeWin->Enable( false );
			m_questscrollWin->Enable( false );			
			m_npcInvWin->Enable( false );				
		}
		break;
	}
}




int CIFManager::LoadConfig( char* filename )
{
	char*		token;
	int			n;
	int			iID;
	int			texIdx;
	char		szTemp[256];

	n = NUM_IF_POSITION;

#ifdef _DEBUG
	bool	gIfCheckFlag[NUM_IF_POSITION];
	memset(&gIfCheckFlag,0,sizeof(gIfCheckFlag));
#endif

	m_script->Load( filename );
	m_script->BeginParse();
	
	while( n > 0 )
	{
		token = m_script->GetNextToken( true );
		if( !token[0] ) 		break;
		if( !stricmp( token, "NUMBER_OF_TEXTURE_INFO" ) )
		{
			token = m_script->GetNextToken( true );
			m_numTexInfo = atoi( token );
		}
		else if( !stricmp( token, "TEXTURE_INFO" ) )
		{
			
			token = m_script->GetNextToken( true );
			texIdx = atoi( token );

			
			token = m_script->GetNextToken( true );
			if( !stricmp( token, "TEX_GROUP_ALL" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_ALL;
			else if( !stricmp( token, "TEX_GROUP_LOGIN" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_LOGIN;
			else if( !stricmp( token, "TEX_GROUP_GAME" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_GAME;
			else if( !stricmp( token, "TEX_GROUP_LOGO" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_LOGO;
			else if( !stricmp( token, "TEX_GROUP_LOADING" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_LOADING;
			else if( !stricmp( token, "TEX_GROUP_CHAR" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_CHAR;
			else if( !stricmp( token, "TEX_GROUP_NOTICE" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_NOTICE;
			
			else if( !stricmp( token, "TEX_GROUP_JYSTECH_LOGO" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_JYSTECH_LOGO;
			else if( !stricmp( token, "TEX_GROUP_MAL_LOGO" ) ) m_texInfo[texIdx].group = IF_TEX_GROUP_MAL_LOGO;
			

			else m_texInfo[texIdx].group = -1;

			
			token = m_script->GetNextToken( true );
			strcpy( m_texInfo[texIdx].filename, token );

			
			token = m_script->GetNextToken( true );
			m_texInfo[texIdx].texSizeX = atoi( token );
			token = m_script->GetNextToken( true );
			m_texInfo[texIdx].texSizeY = atoi( token );
		}
		else
		{
			if( ( iID = GetInterfaceParentPositionID( token ) ) != -1 )
			{

#ifdef _DEBUG
				gIfCheckFlag[iID]=true;
#endif
				m_iPos[iID].ID = iID;
				
				
				token = m_script->GetNextToken( true );
				m_iPos[iID].texIndex = atoi( token );
				
				
				token = m_script->GetNextToken( true );
				m_iPos[iID].texCoordX = atoi( token );
				token = m_script->GetNextToken( true );
				m_iPos[iID].texCoordY = atoi( token );
				
				token = m_script->GetNextToken( true );
				m_iPos[iID].sizeX = atoi( token );
				token = m_script->GetNextToken( true );
				m_iPos[iID].sizeY = atoi( token );
				
				token = m_script->GetNextToken( true );
				m_iPos[iID].clientX = GetInterfacePositionID( token );

				token = m_script->GetNextToken( true );
				m_iPos[iID].clientY = GetInterfacePositionID( token );

				
				token = m_script->GetNextToken( true );
				m_iPos[iID].parentID = GetInterfaceParentPositionID( token );

				n --;
			}
			else
			{
				sprintf( szTemp, "'%s' interface ID is not valid", token );
				MessageBox( NULL, szTemp, "IF_ERROR!", MB_OK );

				break;
			}
		}
	}
	if( n > 0 ) 
	{
#ifdef _DEBUG
		for(int k=0;k<NUM_IF_POSITION;k++)
		{
			if(!gIfCheckFlag[k])
			{
  				sprintf( szTemp, "'%s' IF Data ...NOT FOUND!!", IFPositionIDString[k] );
				MessageBox( NULL, szTemp, "IF_ERROR!", MB_OK );
			}
		}
#endif

		
  		sprintf( szTemp, "'%s' interface ID is unknow", IFPositionIDString[NUM_IF_POSITION - n] );
		MessageBox( NULL, szTemp, "IF_ERROR!", MB_OK );
		
		return( false );
	}

	for( int i = 0; i < NUM_IF_POSITION; i ++ )
	{
		memcpy( &m_iBackPos[i], &m_iPos[i], sizeof( IFControlInfo_t ) );
	}

	
	m_iconCtl->LoadConfig( "if_item.cfg" );
	
	m_skillWin->MakeSortSkillIdxTable();

	return( true );
}

int CIFManager::GetInterfacePositionID( char* posID )
{
	if( !stricmp( posID, "LEFT" ) ) return( IF_POSITION_LEFT );
	else if( !stricmp( posID, "RIGHT" ) ) return( IF_POSITION_RIGHT );
	else if( !stricmp( posID, "TOP" ) ) return( IF_POSITION_TOP );
	else if( !stricmp( posID, "BOTTOM" ) ) return( IF_POSITION_BOTTOM );
	else if( !stricmp( posID, "CENTER" ) ) return( IF_POSITION_CENTER );
	else if( !stricmp( posID, "FULL" ) ) return( IF_POSITION_FULL );
	else if( !stricmp( posID, "QUARTER_BOTTOM" ) ) return( IF_POSITION_QUARTER_BOTTOM );
	else return( atoi( posID ) );
}


void CIFManager::GetInterfacePositionWidth( int id, int &x, int &size )
{
	int pID;
	int px, psize;
	float s;

	pID = m_iPos[id].parentID;

	
	if( pID >= 0 ) GetInterfacePositionWidth( pID, px, psize );
	else
	{
		px = 0;
		psize = m_clientWidth;
	}
	switch( m_iBackPos[id].clientX )
	{
	case IF_POSITION_LEFT :
		x = 0;
		size = m_iBackPos[id].sizeX;
		break;
	case IF_POSITION_RIGHT :
		x = ( psize - m_iBackPos[id].sizeX );
		size = m_iBackPos[id].sizeX;
		break;
	case IF_POSITION_CENTER :
		x = ( psize / 2 - m_iBackPos[id].sizeX / 2 );
		size = m_iBackPos[id].sizeX;
		break;
	case IF_POSITION_FULL :
		x = 0;
		size = psize;
		break;
	default :
		if( pID == -2 )
		{
			s = (float)m_clientWidth / 1024.0f;
			x = (int)( (float)m_iPos[id].clientX * s );
			size = (int)( (float)m_iPos[id].sizeX * s );
		}
		else
		{
			x = m_iPos[id].clientX;
			size = m_iPos[id].sizeX;
		}
		break;
	}
	return;
}


void CIFManager::GetInterfacePositionHeight( int id, int &y, int &size )
{
	int pID;
	int py, psize;
	float s;

	pID = m_iPos[id].parentID;

	
	if( pID >= 0 ) GetInterfacePositionHeight( pID, py, psize );
	else
	{
		py = 0;
		psize = m_clientHeight;
	}
	switch( m_iBackPos[id].clientY )
	{
	case IF_POSITION_TOP :
		y = 0;
		size = m_iBackPos[id].sizeY;
		break;
	case IF_POSITION_BOTTOM :
		y = ( psize - m_iBackPos[id].sizeY );
		size = m_iBackPos[id].sizeY;
		break;
	case IF_POSITION_CENTER :
		y = ( psize / 2 - m_iPos[id].sizeY / 2 );
		size = m_iBackPos[id].sizeY;
		break;
	case IF_POSITION_FULL :
		y = 0;
		size = psize;
		break;
	case IF_POSITION_QUARTER_BOTTOM :
		y = ( psize - psize / 4  ) - m_iPos[id].sizeY / 2;
		break;
	default :
		if( pID == -2 )
		{
			s = (float)m_clientHeight / 768.0f;
			y = (int)( (float)m_iPos[id].clientY * s );
			size = (int)( (float)m_iPos[id].sizeY * s );
		}
		else
		{
			y = m_iPos[id].clientY;
			size = m_iPos[id].sizeY;
		}
		break;
	}
	return;
}

int CIFManager::GetInterfaceParentPositionID( char* id )
{
	int i;
	for( i = 0; i < NUM_IF_POSITION; i ++ )
	{
		if( !stricmp( id, IFPositionIDString[i] ) ) 
		{
			return( i );
		}
		
		if( !stricmp( id, "NULL" ) ) return( -1 );
		else if( !stricmp( id, "NULL2" ) ) return( -2 );
	}
	return( -1 );
}




void CIFManager::AddTextureToManager( int group )
{
	int i;
	char filename[128];
	for( i = 0; i < m_numTexInfo; i ++ )
	{
		if( m_texInfo[i].group == group )
		{
			if( group == IF_TEX_GROUP_LOADING )
			{
#ifdef _GTH_KOREAN_VERSION
				sprintf( filename, "loading_%d.dds", ( rand() % 18 ) + 1 );
#else				
				sprintf( filename, "loading_%d.dds", ( rand() % 15 ) + 1 );
#endif
				strcpy( m_texInfo[i].filename, filename );
			}
			m_texMng->Add( m_texInfo[i].filename, OWNER_TYPE_INTERFACE );
		}
	}
	m_mapWin->AddTextureToManager();
}

void CIFManager::UpdateTextureID( int group )
{
	int i;
	for( i = 0; i < m_numTexInfo; i ++ )
	{
		if( m_texInfo[i].group == group )
			m_texInfo[i].texID = m_texMng->Search( m_texInfo[i].filename, OWNER_TYPE_INTERFACE );
	}
	m_mapWin->UpdateTextureID();
}

void CIFManager::PrepareAllControl( int width, int height )
{
	if( m_controlFlag ) return;

	int i, size;

	m_clientWidth = width;
	m_clientHeight = height;

	for( i = 0; i < NUM_IF_POSITION; i ++ )
	{
		memcpy( &m_iPos[i], &m_iBackPos[i], sizeof( IFControlInfo_t ) );
		size = m_iPos[i].sizeX;
		GetInterfacePositionWidth( i, m_iPos[i].clientX, size );
		size = m_iPos[i].sizeY;
		GetInterfacePositionHeight( i, m_iPos[i].clientY, size );
	}
	


	
	m_msgWin->PrepareControl();

	
	
#ifdef _GTH_MALAYSIA_CHN_VERSION
	m_jystechLogo->PrepareControl( IF_JYSTECH_LOGO );
	m_jystechLogo->SetFade( 1500.0f, 2500.0f );

	m_malLogo->PrepareControl( IF_MAL_LOGO );
	m_malLogo->SetFade( 1500.0f, 2500.0f );
	
	m_startLogo->PrepareControl( IF_START_LOGO );
	m_startLogo->SetFade( 1500.0f, 3000.0f );
	
#else	
	m_startLogo->PrepareControl( IF_START_LOGO );
	m_startLogo->SetFade( 1500.0f, 5000.0f );
#endif
	
	

	
	m_loginWin->PrepareControl();

	
	m_loadingLogo->PrepareControl( IF_LOADING_LOGO );

	m_rolloverWin->PrepareControl();
	m_itemWin->PrepareControl();
	m_depotWin->PrepareControl();
	m_precocityWin->PrepareControl();
	m_craftWin->PrepareControl();

	
	m_EventCraftWin->PrepareControl();


	m_upgradeWin->PrepareControl();
	m_nameWin->PrepareControl();
	m_charWin->PrepareControl();
	m_skillWin->PrepareControl();
	m_questWin->PrepareControl();
	m_hotWin->PrepareControl();
	m_learnWin->PrepareControl();
	m_chatWin->PrepareControl();
	m_tradeWin->PrepareControl();
	m_storeWin->PrepareControl();
	m_partyWin->PrepareControl();
	m_dlgWin->PrepareControl();
	m_chatOptionWin->PrepareControl();
	m_worldChatWin->PrepareControl();
	m_boothWin->PrepareControl();
	m_menuWin->PrepareControl();
	m_optionWin->PrepareControl();
	m_stackWin->PrepareControl();
	m_helpWin->PrepareControl();
	m_gmWin->PrepareControl();
	m_mapWin->PrepareControl();
	m_miniMapWin->PrepareControl();
	m_tutorialWin->PrepareControl();
	m_tipWin->PrepareControl();
	m_settingWin->PrepareControl();
	m_craftHelpWin->PrepareControl();
	
	m_communityWin->PrepareControl();
	m_mailWin->PrepareControl();
	m_mailEditWin->PrepareControl();
	m_friendWin->PrepareControl();
	
	m_questscrollWin->PrepareControl();
	
	m_guildWin->PrepareControl();
	m_guildAuthWin->PrepareControl();
	
	m_guildPvpWin->PrepareControl();
	m_guildArenaWin->PrepareControl();

	
	m_autoQuestWin->PrepareControl();
	m_npcInvWin->PrepareControl();
	

	
	m_guildArenaInfoWin->PrepareControl();

	
	m_surgeryWin->PrepareControl();
	

	
	m_pgonryunBattlePractice->PrepareControl();
	m_pgonryunBattleBattingwindow->PrepareControl();
	m_pGonryunBattleProgressInfo->PrepareControl();
	

	
	m_battleTicketWin->PrepareControl();
	m_shareWin->PrepareControl();
	m_tournamentWin->PrepareControl();
	m_prizeWin->PrepareControl();
	m_golryunInfoWin->PrepareControl();
	m_observerMenuWin->PrepareControl();
	m_observerInfoWin->PrepareControl();
	m_tournamentMiniWin->PrepareControl();
	
	
	
	
	
	
	m_guildMarkMng->CreateTextureForPact( CIFPMManager::MAXNUM_REGISTER_MARKTEXTURE);


	
	m_PostWin->PrepareControl();
	m_mailBoxWin->PrepareControl();
	

	
	m_WorldChangeWin->PrepareControl();


	m_HelperWin->PrepareControl();


	
	m_GiftCardWin->PrepareControl();

	
	m_ServerListWin->PrepareControl();
	

	
	m_CashMallItem->PrepareControl();

	
	m_Item_ScrollWin->PrepareControl();

	
	m_DanWarRequestWin->PrepareControl();
	m_DanWarBattleWin->PrepareControl();


	m_controlFlag = true;
}

void CIFManager::UpdateAllControl()
{
	
	m_msgWin->UpdateControl();

#ifdef _GTH_MALAYSIA_CHN_VERSION
	
	m_jystechLogo->UpdateControl();
	m_malLogo->UpdateControl();
	
#endif


	
	m_startLogo->UpdateControl();

	
	m_loginWin->UpdateControl();

	
	m_loadingLogo->UpdateControl();

	
	m_rolloverWin->UpdateControl();
	m_itemWin->UpdateControl();
	m_depotWin->UpdateControl();
	m_precocityWin->UpdateControl();
	m_craftWin->UpdateControl();

	
	m_EventCraftWin->UpdateControl();

	m_upgradeWin->UpdateControl();
	m_nameWin->UpdateControl();
	m_charWin->UpdateControl();
	m_skillWin->UpdateControl();
	m_questWin->UpdateControl();
	m_hotWin->UpdateControl();
	m_learnWin->UpdateControl();
	m_chatWin->UpdateControl();
	m_storeWin->UpdateControl();
	m_tradeWin->UpdateControl();
	m_dlgWin->UpdateControl();
	m_chatOptionWin->UpdateControl();
	m_worldChatWin->UpdateControl();
	m_partyWin->UpdateControl();
	m_boothWin->UpdateControl();
	m_menuWin->UpdateControl();
	m_optionWin->UpdateControl();
	m_stackWin->UpdateControl();
	m_helpWin->UpdateControl();
	m_gmWin->UpdateControl();
	m_mapWin->UpdateControl();
	m_miniMapWin->UpdateControl();
	m_tutorialWin->UpdateControl();
	m_tipWin->UpdateControl();
	m_settingWin->UpdateControl();
	m_craftHelpWin->UpdateControl();
	
	m_communityWin->UpdateControl();
	m_mailWin->UpdateControl();
	m_mailEditWin->UpdateControl();
	m_friendWin->UpdateControl();
	
	m_questscrollWin->UpdateControl();
	
	m_guildWin->UpdateControl();
	m_guildAuthWin->UpdateControl();
	
	m_guildPvpWin->UpdateControl();
	m_guildArenaWin->UpdateControl();
	
	m_autoQuestWin->UpdateControl();
	m_npcInvWin->UpdateControl();
	

	m_guildArenaInfoWin->UpdateControl();

	
	m_surgeryWin->UpdateControl();
	
	
	
	m_pgonryunBattlePractice->UpdateControl();
	m_pgonryunBattleBattingwindow->UpdateControl();
	m_pGonryunBattleProgressInfo->UpdateControl();
	

	
	m_battleTicketWin->UpdateControl();
	m_shareWin->PrepareControl();
	m_tournamentWin->UpdateControl();
	m_prizeWin->UpdateControl();
	m_golryunInfoWin->UpdateControl();
	m_observerMenuWin->UpdateControl();
	m_observerInfoWin->UpdateControl();
	m_tournamentMiniWin->UpdateControl();
	


	
	m_PostWin->UpdateControl();
	m_mailBoxWin->UpdateControl();
	

	
	m_WorldChangeWin->UpdateControl();


	m_HelperWin->UpdateControl();


	
	m_GiftCardWin->UpdateControl();

	
	m_ServerListWin->UpdateControl();
	

	
	m_CashMallItem->UpdateControl();

	
	m_Item_ScrollWin->UpdateControl();

	
	m_DanWarRequestWin->UpdateControl();
	m_DanWarBattleWin->UpdateControl();
}
void CIFManager::FinalCleanup()
{
	GTH_SAFE_DELETE_ARRAY	( m_iPos );
	GTH_SAFE_DELETE_ARRAY	( m_iBackPos );

	
	GTH_SAFE_DELETE			( m_msgWin );
	GTH_SAFE_DELETE			( m_iconCtl );
	GTH_SAFE_DELETE			( m_cursor );
	GTH_SAFE_DELETE			( m_numCtl );

#ifdef _GTH_MALAYSIA_CHN_VERSION
	
	GTH_SAFE_DELETE			( m_jystechLogo );
	GTH_SAFE_DELETE			( m_malLogo );
	
#endif
	

	
	GTH_SAFE_DELETE			( m_startLogo );

	
	GTH_SAFE_DELETE			( m_loginWin );

	
	GTH_SAFE_DELETE			( m_loadingLogo );

	
	GTH_SAFE_DELETE			( m_nameWin );
	GTH_SAFE_DELETE			( m_charWin );
	GTH_SAFE_DELETE			( m_itemWin );
	GTH_SAFE_DELETE			( m_skillWin );
	GTH_SAFE_DELETE			( m_questWin );
	GTH_SAFE_DELETE			( m_hotWin );
	GTH_SAFE_DELETE			( m_depotWin );
	GTH_SAFE_DELETE			( m_learnWin );
	GTH_SAFE_DELETE			( m_precocityWin );
	GTH_SAFE_DELETE			( m_craftWin );

	
	GTH_SAFE_DELETE			( m_EventCraftWin );

	GTH_SAFE_DELETE			( m_upgradeWin );
	GTH_SAFE_DELETE			( m_chatWin );
	GTH_SAFE_DELETE			( m_storeWin );
	GTH_SAFE_DELETE			( m_tradeWin );
	GTH_SAFE_DELETE			( m_partyWin );
	GTH_SAFE_DELETE			( m_dlgWin );
	GTH_SAFE_DELETE			( m_chatOptionWin );
	GTH_SAFE_DELETE			( m_worldChatWin );
	GTH_SAFE_DELETE			( m_boothWin );
	GTH_SAFE_DELETE			( m_menuWin );
	GTH_SAFE_DELETE			( m_rolloverWin );
	GTH_SAFE_DELETE			( m_optionWin );
	GTH_SAFE_DELETE			( m_stackWin );
	GTH_SAFE_DELETE			( m_helpWin );
	GTH_SAFE_DELETE			( m_gmWin );
	GTH_SAFE_DELETE			( m_mapWin );
	GTH_SAFE_DELETE			( m_miniMapWin );
	GTH_SAFE_DELETE			( m_tutorialWin );
	GTH_SAFE_DELETE			( m_tipWin );
	GTH_SAFE_DELETE			( m_settingWin );
	GTH_SAFE_DELETE			( m_craftHelpWin );
	
	GTH_SAFE_DELETE			( m_communityWin );
	GTH_SAFE_DELETE			( m_mailWin );
	GTH_SAFE_DELETE			( m_mailEditWin );
	GTH_SAFE_DELETE			( m_friendWin );
	
	GTH_SAFE_DELETE			( m_questscrollWin );
	
	GTH_SAFE_DELETE			( m_guildWin );
	
	
	GTH_SAFE_DELETE			( m_guildMarkMng );
	GTH_SAFE_DELETE			( m_guildAuthWin );
	
	GTH_SAFE_DELETE			( m_guildPvpWin );
	GTH_SAFE_DELETE			( m_guildArenaWin );

	
	GTH_SAFE_DELETE			( m_autoQuestWin );
	GTH_SAFE_DELETE			( m_npcInvWin );
	

	
	GTH_SAFE_DELETE			( m_guildArenaInfoWin );

	
	GTH_SAFE_DELETE			( m_surgeryWin );
	

	
	GTH_SAFE_DELETE			(m_pgonryunBattlePractice);
	GTH_SAFE_DELETE			(m_pgonryunBattleBattingwindow);
	GTH_SAFE_DELETE			(m_pGonryunBattleProgressInfo);
	

	
	GTH_SAFE_DELETE			( m_battleTicketWin );
	GTH_SAFE_DELETE			( m_shareWin );
	GTH_SAFE_DELETE			( m_tournamentWin );
	GTH_SAFE_DELETE			( m_prizeWin );
	GTH_SAFE_DELETE			( m_golryunInfoWin );
	GTH_SAFE_DELETE			( m_observerMenuWin );
	GTH_SAFE_DELETE			( m_observerInfoWin );
	GTH_SAFE_DELETE			( m_tournamentMiniWin );
	


	
	GTH_SAFE_DELETE			(m_PostWin);
	GTH_SAFE_DELETE			(m_mailBoxWin);
	GTH_SAFE_DELETE			(m_WorldChangeWin);
	


	GTH_SAFE_DELETE			(m_HelperWin);


	
	GTH_SAFE_DELETE			(m_GiftCardWin);
	

	
	GTH_SAFE_DELETE         (m_ServerListWin);	
	

	
	GTH_SAFE_DELETE			(m_CashMallItem);
	
	GTH_SAFE_DELETE			(m_Item_ScrollWin);	

	
	GTH_SAFE_DELETE			(m_DanWarRequestWin);
	GTH_SAFE_DELETE			(m_DanWarBattleWin);
}


void CIFManager::InvalidateDeviceObjects()
{
	

	
	m_iconCtl->Release();
	m_numCtl->Release();
	m_msgWin->Release();

	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :	case CLIENT_LOGIN_PROCESS : case CLIENT_CHARACTER_PROCESS :
		
#ifdef _GTH_MALAYSIA_CHN_VERSION
		
		m_jystechLogo->Release();
		m_malLogo->Release();
		
#endif

		
		m_startLogo->Release();

		
		m_loginWin->Release();

		
		m_ServerListWin->Release();


		break;

	case CLIENT_LOADING_PROCESS :

		
		m_loadingLogo->Release();
		break;

	case CLIENT_GAME_PROCESS : 

		
		m_rolloverWin->Release();
		m_itemWin->Release();
		m_depotWin->Release();
		m_precocityWin->Release();
		m_craftWin->Release();

		
		m_EventCraftWin->Release();

		m_upgradeWin->Release();
		m_nameWin->Release();
		m_charWin->Release();
		m_skillWin->Release();
		m_questWin->Release();
		m_hotWin->Release();
		m_learnWin->Release();
		m_chatWin->Release();
		m_storeWin->Release();
		m_tradeWin->Release();
		m_dlgWin->Release();
		m_chatOptionWin->Release();
		m_worldChatWin->Release();
		m_partyWin->Release();
		m_boothWin->Release();
		m_menuWin->Release();
		m_optionWin->Release();
		m_stackWin->Release();
		m_helpWin->Release();
		m_gmWin->Release();
		m_mapWin->Release();
		m_miniMapWin->Release();
		m_tutorialWin->Release();
		m_tipWin->Release();
		m_settingWin->Release();
		m_craftHelpWin->Release();
		
		m_communityWin->Release();
		m_mailWin->Release();
		m_mailEditWin->Release();
		m_friendWin->Release();
		
		m_questscrollWin->Release();
		
		m_guildWin->Release();
		m_guildAuthWin->Release();
		
		m_guildPvpWin->Release();
		m_guildArenaWin->Release();

		
		m_autoQuestWin->Release();
		m_npcInvWin->Release();
		

		m_guildArenaInfoWin->Release();

		
		m_surgeryWin->Release();
		
	
		
		m_pgonryunBattlePractice->Release();
		m_pgonryunBattleBattingwindow->Release();
		m_pGonryunBattleProgressInfo->Release();
		

		
		m_battleTicketWin->Release();
		m_shareWin->Release();
		m_tournamentWin->Release();
		m_prizeWin->Release();
		m_golryunInfoWin->Release();
		m_observerMenuWin->Release();
		m_observerInfoWin->Release();
		m_tournamentMiniWin->Release();
		

		
		m_guildMarkMng->InvalidateDeviceObjects();

		
		m_PostWin->Release();
		m_mailBoxWin->Release();
		

		
		m_WorldChangeWin->Release();


		m_HelperWin->Release();


		
		m_GiftCardWin->Release();

		
		m_CashMallItem->Release();
		
		m_Item_ScrollWin->Release();
		
		
		m_DanWarRequestWin->Release();
		m_DanWarBattleWin->Release();
		
		break;
	}

	g_input->InvalidateDeviceObject ();
}
void CIFManager::RestoreDeviceObjects()
{
	
	

	
	m_iconCtl->PrepareBuffer();
	m_numCtl->PrepareBuffer();
	m_msgWin->PrepareBuffer();

	
	AddTextureToManager( 0 );
	UpdateTextureID( 0 );
	m_iconCtl->AddTextureToManager();
	m_iconCtl->UpdateTextureID();

	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :	
	case CLIENT_LOGIN_PROCESS :
	case CLIENT_CHARACTER_PROCESS :

		assert(NULL != g_cpGuildCtrl);
		g_cpGuildCtrl->Reset();

#ifdef _GTH_MALAYSIA_CHN_VERSION
		
		m_jystechLogo->PrepareBuffer();	
		m_malLogo->PrepareBuffer();	
		
#endif

		
		
		m_startLogo->PrepareBuffer();	
		
		m_loginWin->PrepareBuffer();	

		
		m_ServerListWin->PrepareBuffer();
		
		

		
		AddTextureToManager( IF_TEX_GROUP_JYSTECH_LOGO );
		UpdateTextureID( IF_TEX_GROUP_JYSTECH_LOGO );
		
		AddTextureToManager( IF_TEX_GROUP_MAL_LOGO );
		UpdateTextureID( IF_TEX_GROUP_MAL_LOGO );
		


		
		AddTextureToManager( IF_TEX_GROUP_NOTICE );
		UpdateTextureID( IF_TEX_GROUP_NOTICE );
	
		AddTextureToManager( IF_TEX_GROUP_LOGO );
		UpdateTextureID( IF_TEX_GROUP_LOGO );
		AddTextureToManager( IF_TEX_GROUP_LOGIN );
		UpdateTextureID( IF_TEX_GROUP_LOGIN );
		break;

	case CLIENT_LOADING_PROCESS :

		
		m_loadingLogo->PrepareBuffer();	
		AddTextureToManager( IF_TEX_GROUP_LOADING );
		UpdateTextureID( IF_TEX_GROUP_LOADING );
		break;

	case CLIENT_GAME_PROCESS : 
		
		
		m_rolloverWin->PrepareBuffer();
		m_itemWin->PrepareBuffer();
		m_depotWin->PrepareBuffer();
		m_precocityWin->PrepareBuffer();
		m_craftWin->PrepareBuffer();

		
		m_EventCraftWin->PrepareBuffer();

		m_upgradeWin->PrepareBuffer();
		m_nameWin->PrepareBuffer();
		m_charWin->PrepareBuffer();
		m_skillWin->PrepareBuffer();
		m_questWin->PrepareBuffer();
		m_hotWin->PrepareBuffer();		
		m_learnWin->PrepareBuffer();
		m_chatWin->PrepareBuffer();
		m_storeWin->PrepareBuffer();
		m_tradeWin->PrepareBuffer();
		m_dlgWin->PrepareBuffer();
		m_chatOptionWin->PrepareBuffer();
		m_worldChatWin->PrepareBuffer();
		m_partyWin->PrepareBuffer();
		m_boothWin->PrepareBuffer();
		m_menuWin->PrepareBuffer();
		m_optionWin->PrepareBuffer();
		m_stackWin->PrepareBuffer();
		m_helpWin->PrepareBuffer();
		m_gmWin->PrepareBuffer();
		m_mapWin->PrepareBuffer();
		m_miniMapWin->PrepareBuffer();
		m_tutorialWin->PrepareBuffer();
		m_tipWin->PrepareBuffer();
		m_settingWin->PrepareBuffer();
		m_craftHelpWin->PrepareBuffer();
		
		m_communityWin->PrepareBuffer();
		m_mailWin->PrepareBuffer();
		m_mailEditWin->PrepareBuffer();
		m_friendWin->PrepareBuffer();
		
		m_questscrollWin->PrepareBuffer();
		
		m_guildWin->PrepareBuffer();
		m_guildAuthWin->PrepareBuffer();
		
		m_guildPvpWin->PrepareBuffer();
		m_guildArenaWin->PrepareBuffer();
		
		m_autoQuestWin->PrepareBuffer();
		m_npcInvWin->PrepareBuffer();
		

		
		m_guildArenaInfoWin->PrepareBuffer();

		
		m_surgeryWin->PrepareBuffer();
		

		
		m_pgonryunBattlePractice->PrepareBuffer();
		m_pgonryunBattleBattingwindow->PrepareBuffer();
		m_pGonryunBattleProgressInfo->PrepareBuffer();
		

		
		m_battleTicketWin->PrepareBuffer();
		m_shareWin->PrepareBuffer();
		m_tournamentWin->PrepareBuffer();
		m_prizeWin->PrepareBuffer();
		m_golryunInfoWin->PrepareBuffer();
		m_observerMenuWin->PrepareBuffer();
		m_observerInfoWin->PrepareBuffer();
		m_tournamentMiniWin->PrepareBuffer();
		


		
		m_PostWin->PrepareBuffer();
		m_mailBoxWin->PrepareBuffer();
		

		
		m_WorldChangeWin->PrepareBuffer();


		m_HelperWin->PrepareBuffer();


		
		m_GiftCardWin->PrepareBuffer();

		
		m_CashMallItem->PrepareBuffer();

		
		m_Item_ScrollWin->PrepareBuffer();

		
		m_DanWarRequestWin->PrepareBuffer();
		m_DanWarBattleWin->PrepareBuffer();

		LoadGuildMark();
		
		if(FALSE == g_cpGuildCtrl->Prepare()){
			OutputDebugString("error CGuildCtrl::Prepare()");
		}

		

		AddTextureToManager( IF_TEX_GROUP_GAME );
		UpdateTextureID( IF_TEX_GROUP_GAME );
		break;
	}
}
	
void CIFManager::UpdateAllBuffer()
{
	

	m_msgWin->UpdateBuffer();

	
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :	case CLIENT_LOGIN_PROCESS : case CLIENT_CHARACTER_PROCESS :

#ifdef _GTH_MALAYSIA_CHN_VERSION
		
		m_jystechLogo->UpdateBuffer();
		m_malLogo->UpdateBuffer();
		
#endif

		
		m_startLogo->UpdateBuffer();
		
		m_loginWin->UpdateBuffer();
		break;

	case CLIENT_LOADING_PROCESS :
		
		m_loadingLogo->UpdateBuffer();
		break;

	case CLIENT_GAME_PROCESS : 
		
		m_rolloverWin->UpdateBuffer();
		m_itemWin->UpdateBuffer();
		m_depotWin->UpdateBuffer();
		m_precocityWin->UpdateBuffer();
		m_craftWin->UpdateBuffer();

		
		m_EventCraftWin->UpdateBuffer();

		m_upgradeWin->UpdateBuffer();
		m_nameWin->UpdateBuffer();
		m_charWin->UpdateBuffer();
		m_skillWin->UpdateBuffer();
		m_questWin->UpdateBuffer();
		m_hotWin->UpdateBuffer();
		m_learnWin->UpdateBuffer();
		m_chatWin->UpdateBuffer();
		m_storeWin->UpdateBuffer();
		m_tradeWin->UpdateBuffer();
		m_dlgWin->UpdateBuffer();
		m_chatOptionWin->UpdateBuffer();
		m_worldChatWin->UpdateBuffer();
		m_partyWin->UpdateBuffer();
		m_boothWin->UpdateBuffer();
		m_menuWin->UpdateBuffer();
		m_optionWin->UpdateBuffer();
		m_stackWin->UpdateBuffer();
		m_helpWin->UpdateBuffer();
		m_gmWin->UpdateBuffer();
		m_mapWin->UpdateBuffer();
		m_miniMapWin->UpdateBuffer();
		m_tutorialWin->UpdateBuffer();
		m_tipWin->UpdateBuffer();
		m_settingWin->UpdateBuffer();
		m_craftHelpWin->UpdateBuffer();
		
		m_communityWin->UpdateBuffer();
		m_mailWin->UpdateBuffer();
		m_mailEditWin->UpdateBuffer();
		m_friendWin->UpdateBuffer();
		
		m_questscrollWin->UpdateBuffer();
		
		m_guildWin->UpdateBuffer();
		m_guildAuthWin->UpdateBuffer();
		
		m_guildPvpWin->UpdateBuffer();
		m_guildArenaWin->UpdateBuffer();
		
		m_autoQuestWin->UpdateBuffer();
		m_npcInvWin->UpdateBuffer();
		

		
		m_guildArenaInfoWin->UpdateBuffer();
		
		
		m_surgeryWin->UpdateBuffer();
		
		
		
		m_pgonryunBattlePractice->UpdateBuffer();
		m_pgonryunBattleBattingwindow->UpdateBuffer();
		m_pGonryunBattleProgressInfo->UpdateBuffer();
		

		
		m_battleTicketWin->UpdateBuffer();
		m_shareWin->UpdateBuffer();
		m_tournamentWin->UpdateBuffer();
		m_prizeWin->UpdateBuffer();
		m_golryunInfoWin->UpdateBuffer();
		m_observerMenuWin->UpdateBuffer();
		m_observerInfoWin->UpdateBuffer();
		m_tournamentMiniWin->UpdateBuffer();
		


		
		m_PostWin->UpdateBuffer();
		m_mailBoxWin->UpdateBuffer();
		

		
		m_WorldChangeWin->UpdateBuffer();

		m_HelperWin->UpdateBuffer();


		
		m_GiftCardWin->UpdateBuffer();

		
		m_CashMallItem->UpdateBuffer();

		
		m_Item_ScrollWin->UpdateBuffer();

		
		m_DanWarRequestWin->UpdateBuffer();
		m_DanWarBattleWin->UpdateBuffer();

			

		break;
	}
}

void CIFManager::PrepareRender()
{	
	if( !g_pApp->m_renderEnabled ) return;
	
	m_numCtl->ProcessDamage();

	
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :	case CLIENT_LOGIN_PROCESS : case CLIENT_CHARACTER_PROCESS :
#ifdef _GTH_MALAYSIA_CHN_VERSION
		
		m_jystechLogo->UpdateBuffer();
		m_malLogo->UpdateBuffer();
		
#endif

		
		m_startLogo->UpdateBuffer();
		
		
		m_loginWin->WindowFrame();
		m_loginWin->UpdateBuffer();

		
		m_ServerListWin->WindowFrame();
		m_ServerListWin->UpdateBuffer();
		
		break;

	case CLIENT_LOADING_PROCESS :
		
		m_loadingLogo->UpdateBuffer();
		break;
		
	case CLIENT_GAME_PROCESS : 
		
		for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
		{
			m_winControl[i]->UpdateControl();
			m_winControl[i]->WindowFrame();
			m_winControl[i]->UpdateBuffer();
		}
		
		g_GonryunBattlePractic.ProcessGonryunEffect();
		

		
		g_GolryunBattle.ProcessEffectCount();
		
		break;
	}
	
	
	m_msgWin->UpdateControl();
	m_msgWin->WindowFrame();
	m_msgWin->UpdateBuffer();



}



void CIFManager::LogoRender()
{
	switch(m_bLogoStep)
	{
		case IF_JYSTECH_LOGO :		m_jystechLogo->RenderFade();
			break;

		case IF_MAL_LOGO :			m_malLogo->RenderFade();
			break;

		case IF_START_LOGO :		m_startLogo->RenderFade();
			break;
	}
}



void CIFManager::Render()
{

	if(	!g_pApp->m_renderEnabled )
	{
		return;
	}

	int i;

	SetRenderState();

	m_numCtl->Render();

	
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :	
		
		LogoRender();
		
		break;
	case CLIENT_LOGIN_PROCESS : case CLIENT_CHARACTER_PROCESS :
		m_loginWin->Render();
		
		m_ServerListWin->Render();
		break;

	case CLIENT_LOADING_PROCESS :
		m_loadingLogo->Render();
		break;

	case CLIENT_GAME_PROCESS : 
		{
			for( i = NUM_IF_WINDOW_TYPE - 1; i >=0; i -- )
			{
				
				if ( m_Visible == FALSE)					
				{
					if ( (m_winControl[i]->GetWindowType() != IF_DANWAR_REQUEST_WIN_TYPE) &&
						(m_winControl[i]->GetWindowType() != IF_DANWAR_BATTLE_WIN_TYPE) )	
						continue;
				}

				m_winControl[i]->Render();
			}
			
			m_rolloverWin->Render();
			m_rolloverWin->Enable( false );
		}
		break;
	}

	m_msgWin->Render();
}


void CIFManager::AddFocus( CIFWindow *pCtl )
{
	int i;
	for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if( m_winControl[i] == NULL ) break;
	}
	if( i == NUM_IF_WINDOW_TYPE ) return;

	m_winControl[i] = pCtl;
}

int CIFManager::SetFocus( CIFWindow *pCtl )
{
	int i, n;
	CIFWindow *pNewWinControl[NUM_IF_WINDOW_TYPE];

	
	if( pCtl == m_winControl[0] ) return 0;

	for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if( m_winControl[i] == pCtl ) break;
	}
	if( i == NUM_IF_WINDOW_TYPE ) return 0;

	pNewWinControl[0] = pCtl;
	n = 1;
	for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if( m_winControl[i] == pCtl ) continue;
		pNewWinControl[n] = m_winControl[i];
		n ++;
	}
	for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		m_winControl[i] = pNewWinControl[i];
	}

	return 1;
}



void CIFManager::LogoProcess()
{

	switch(m_bLogoStep)
	{
	case IF_JYSTECH_LOGO :
		{
			if(m_jystechLogo->m_fadeFlag == 4 )
			{				
				m_bLogoStep = IF_MAL_LOGO;
			}
			
			if( m_jystechLogo->m_fadeFlag == 2 && g_input->GetLBDown() ) 
			{
				m_jystechLogo->m_fadeStart = g_timer.GetAppMilliTime();
				m_jystechLogo->m_fadeFlag = 3;
			}		
		}
		break;

	case IF_MAL_LOGO :
		{
			if(m_malLogo->m_fadeFlag == 4 )
			{
				m_bLogoStep = IF_START_LOGO;
			}
			
			if( m_malLogo->m_fadeFlag == 2 && g_input->GetLBDown() ) 
			{
				m_malLogo->m_fadeStart = g_timer.GetAppMilliTime();
				m_malLogo->m_fadeFlag = 3;
			}
		}
		break;

	case IF_START_LOGO :
		{
			if(m_startLogo->m_fadeFlag == 4 )
			{
				m_changeStateFlag = true;
			}

			if( m_startLogo->m_fadeFlag == 2 && g_input->GetLBDown() ) 
			{
				m_startLogo->m_fadeStart = g_timer.GetAppMilliTime();
				m_startLogo->m_fadeFlag = 3;
			}

		}
		break;
	}
}


int CIFManager::ProcessInterface()
{
	int i, ret, forcedOut = false, changeFocusFlag = false;
	int dx, dy;

	
	ProcessCursor();

	if( !m_changeStateFlag  )
	{
		


		if( m_msgWin->State() )
		{
			m_msgWin->WindowSelection();
			m_msgWin->WindowProcess();
			return( IF_CTL_NONE );
		}

		
		if( m_worldChatWin->State()){
			m_worldChatWin->WindowSelection();
			m_worldChatWin->WindowProcess();
			return( IF_CTL_NONE );
		}
		



		
		
		if( m_stackWin->State() )
		{
			m_stackWin->WindowSelection();
			m_stackWin->WindowProcess();
			return( IF_CTL_NONE );
		}
		
		if ( m_WorldChangeWin->State())
		{
			m_WorldChangeWin->WindowSelection();
			m_WorldChangeWin->WindowProcess();
			return( IF_CTL_NONE );
		}

		
		if ( m_ServerListWin->State())
		{
			m_ServerListWin->WindowSelection();
			m_ServerListWin->WindowProcess();
			return( IF_CTL_NONE );
		}	
		


		






	}

	
	switch( g_cgv.state )
	{
	case CLIENT_START_PROCESS :	
		{
			
#if defined (_GTH_TERRA_VERSION) || (_GTH_MALAYSIA_CHN_VERSION)
			
			if( g_cgv.restartState == RESTART_CHARACTER || g_cgv.restartState == RESTART_SERVER )
			{
				g_ifMng->m_loginWin->RequestLogin();
				return( IF_CTL_EVENT );
			}
			else if( m_changeStateFlag )
			{
				if( !g_cgv.onMasterServer ) 
				{
					g_ifMng->m_loginWin->ChangeLoginStep( 4 );
					g_ifMng->m_loginWin->MakeMasterServerGroupList();
				}
				else 
				{
					g_ifMng->m_loginWin->ChangeLoginStep( 0 );
					g_ifMng->m_loginWin->RequestLogin();
				}
				m_changeStateFlag = false;
				return( IF_CTL_EVENT );

			}
			else 
			{
				
				LogoProcess();					
			}
		}
#else
			
			if( g_cgv.restartState == RESTART_CHARACTER || g_cgv.restartState == RESTART_SERVER )
			{
				g_ifMng->m_loginWin->RequestLogin();
				return( IF_CTL_EVENT );
			}
			else if( m_changeStateFlag )
			{
				
#ifdef _GTH_CHINESE_VERSION
				if( !g_cgv.onMasterServer ) 
				{
#ifdef _GTH_ENABLE_GOODNOTICE
					if(g_ifMng->m_loginWin->GetFirstConnectMasterServer())
						g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_NOTICE );
					else
					{
						g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_MASTER_SERVER );
						g_ifMng->m_loginWin->MakeMasterServerGroupList();
					}						
#else
					g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_MASTER_SERVER );
					g_ifMng->m_loginWin->MakeMasterServerGroupList();
#endif

					
				}
				
				else 
				{
					g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_IDPW );
				}
#else
				g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_IDPW );
#endif
				
				m_changeStateFlag = false;
				return( IF_CTL_EVENT );
			}
			else 
			{
				if( m_startLogo->m_fadeFlag == 4 )
				{
					m_changeStateFlag = true;
				}
				if( m_startLogo->m_fadeFlag == 2 && g_input->GetLBDown() ) 
				{
					m_startLogo->m_fadeStart = g_timer.GetAppMilliTime();
					m_startLogo->m_fadeFlag = 3;
				}
			}
		}
#endif
		break;
		
	case CLIENT_LOGIN_PROCESS : 
		{
#ifdef _GTH_ONLINE_VERSION
			if( m_changeStateFlag )
			{
				g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_CHARACTER );
				
				m_changeStateFlag = false;
				return( IF_CTL_EVENT );
			}
#else
			g_ifMng->m_loginWin->ChangeLoginStep( LOGIN_STATE_SELECT_CHARACTER );
			
			m_changeStateFlag = false;
			return( IF_CTL_EVENT );
#endif
			m_loginWin->WindowSelection();
			switch( m_loginWin->WindowProcess() )
			{
				case IF_CTL_NONE :
					return 0;
				break;
			}
		}
		break;
	case CLIENT_CHARACTER_PROCESS :
		{
			if( m_changeStateFlag )
			{
				
				m_changeStateFlag = false;
				
				if( g_cgv.restartState == RESTART_RESERVER ) return( IF_CTL_EXIT );
				else return( IF_CTL_EVENT );
			}
			m_loginWin->WindowSelection();
			switch( m_loginWin->WindowProcess() )
			{
				case IF_CTL_NONE :
					return 0;
				break;
			}
		}
		break;

	case CLIENT_LOADING_PROCESS :
		{
			
			if( !g_cgv.changeServerFlag ) 
			{
				GTH_SetWorldInfo();
			}
			if( m_changeStateFlag )
			{
				
				if( !m_renderLoadingFlag )
				{
					m_renderLoadingFlag = true;
					return( IF_CTL_NONE );
				}
				
				m_changeStateFlag = false;
				
				g_cgv.changeServerFlag = true;
				g_cgv.sendGameServerConnectFlag = true;
				return( IF_CTL_EVENT );
			}
		}
		break;

	case CLIENT_GAME_PROCESS : 
		{
			if( m_changeStateFlag )
			{
				
				m_changeStateFlag = false;
				
				if( g_cgv.restartState != RESTART_NONE ) return( IF_CTL_EXIT );
				else return( IF_CTL_EVENT );
			}

			
			if( m_tipWin->CanOpen() )
			{
				m_tipWin->Enable(true);
				m_tipWin->Open();
			}
			
			
			dx = g_input->GetDeltaX();
			dy = g_input->GetDeltaY();
			m_winControl[0]->UpdateMoving( dx, dy );
			
			for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
			{
				if( !m_winControl[i]->State() ) continue;

				ret = m_winControl[i]->WindowSelection();
				
				switch( ret )
				{
				case IF_CTL_BEGIN_MOVING :
					if( SetFocus( m_winControl[i] ) ) changeFocusFlag = true;
					m_winControl[0]->BeginMoving();
					forcedOut = true;
					break;
				case IF_CTL_SELECT :
					if( SetFocus( m_winControl[i] ) ) changeFocusFlag = true;
					forcedOut = true;
					break;
				}
				if( forcedOut ) break;
			}
			

			
			 
			 

#ifndef	_GTH_NO_INTERFACE
			for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
			{
				if( !m_winControl[i]->State() ) continue;
				
				
				switch( m_winControl[i]->WindowProcess() )
				{
				case IF_CTL_SELECT :
					return 0;
					break;
				}
				if( m_winControl[i]->Selection() == IF_NOT_IN_POS ) continue;
				break;
			}
#endif
		}
		break;
	}

	return IF_CTL_NONE;
}

int	CIFManager::ProcessSelection()
{	
	int i;


#if defined _GTH_CHINESE_VERSION || _GTH_TAIWAN_VERSION	|| (_GTH_MALAYSIA_CHN_VERSION ) 
	if ( g_chatManager->IsInnerStatusWind() ) return 1;
#endif


	
	if( g_ifMng->m_msgWin->State() )
	{
		if( g_ifMng->m_msgWin->Selection() != IF_NOT_IN_POS ) return 1;
	}
	for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if( !m_winControl[i]->State() ) continue;
		
		if( m_winControl[i]->GetWindowType() == IF_MAP_WINDOW_TYPE )
		{
			CIFMapWin *pMapWin = (CIFMapWin *)m_winControl[i];
			if( pMapWin->m_titleBar->Selection() != IF_NOT_IN_POS ) return 1;
		}
		else if( m_winControl[i]->Selection() != IF_NOT_IN_POS ) 
		{
			return 1;
		}
	}
	
	for( i = 0; i < MAX_NUMBER_OF_PARTY_MEMBER; i ++ )
	{
		if( !m_partyWin->m_memberWin[i]->State() ) continue;
		if( m_partyWin->m_memberWin[i]->Selection() != IF_NOT_IN_POS ) return 1;
	}

	if( CheckConditionalWindow() ) return 0;

	
	
	
	if( g_input->GetLBUp() )
	{
		int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;
		float distance;
		vec3_t pos;
		char szTemp[256];
		
		if( mouseItemIdx < 0 ) return 0;
		
		int itemtableidx=g_cgv.myCharacterInfo->item[mouseItemIdx].itemTableIdx;
		if(g_cgv.myCharacterInfo->item[mouseItemIdx].itemTableIdx == AUTOQUESTSCROLL_ITEM_INDEX) return 0;
		if(g_itemTable[itemtableidx].etcItemType == ETCITEM_TYPE_AUTOQUESTITEM) return 0;
		
		g_cursor->GetTouchedPos( pos );
		pos[2] += ITEMOBJECT_DISPLAY_HEIGHT;
		distance = GTH_GetDistance( g_pApp->m_myCharacter->position, pos );
		if ( distance < AVAILABLE_DISTANCE_DROPITEM )
		{

			VectorCopy( m_itemDropPos, pos );
		}
		else
		{	
			
			VectorSubtract( pos, pos, g_pApp->m_myCharacter->position );
			VectorNormalize( pos );
			VectorScale( pos, pos, AVAILABLE_DISTANCE_DROPITEM * 0.8f );
			VectorAdd( pos, pos, g_pApp->m_myCharacter->position );

			VectorCopy( m_itemDropPos, pos );
		}

		
		







		
		sprintf( szTemp, g_LPACK.GetMassage( 0, 635 ), g_cgv.myCharacterInfo->item[mouseItemIdx].name );
		g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_DROP_ITEM );
		return 1;
	}
	return 0;
}

void CIFManager::ProcessCursor()
{
	int mouseItemIdx = g_cgv.myCharacterInfo->mouseInventory;
	int itemTableIdx, cursorID;

	if( g_input->GetCursorState( -1, ACTION_CURSOR_TYPE ) || g_input->GetCursorState( -1, SKILL_CURSOR_TYPE ) )
	{
		if( g_input->KeyUp( DIK_ESCAPE ) ) g_input->ChangeCursor( CURSOR_POINTER, MOUSE_CURSOR_TYPE );
		return;
	}

	if( g_input->GetCursorState( CURSOR_REPAIR, MOUSE_CURSOR_TYPE ) && m_storeWin->State() )
	{
		if( g_input->KeyUp( DIK_ESCAPE ) ) g_input->ChangeCursor( CURSOR_POINTER, MOUSE_CURSOR_TYPE );
		return;
	}

	if( mouseItemIdx < 0 ) 
	{

		
		if ( g_cgv.pTargetChar != NULL)
		{
			if ( g_cgv.pTargetChar->entityType == ENTITY_PC && g_cgv.pTargetChar->idxOnServer != g_pApp->m_myCharacter->idxOnServer )
			{
				

				if( g_AttackProc.IsSaftTargeting() )
				{

					{

						for( int i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
						{
							if ( m_winControl[i]->State() && m_winControl[i]->Selection() != IF_NOT_IN_POS )
								break;
						}

						if ( i == NUM_IF_WINDOW_TYPE )
						{
							g_input->ChangeCursor( CURSOR_ATTACK, MOUSE_CURSOR_TYPE );
							return;
						}
					}
				}
			}
		}

		g_input->ChangeCursor( CURSOR_POINTER, MOUSE_CURSOR_TYPE );
		return;
	}

	itemTableIdx = g_cgv.myCharacterInfo->item[mouseItemIdx].itemTableIdx;
	cursorID = g_itemTable[itemTableIdx].iconItemID;
	
	g_input->ChangeCursor( cursorID, ITEM_CURSOR_TYPE);
}

int CIFManager::ProcessIFKeyState()
{


	if( g_input->KeyEvent( DIK_LALT ) || g_input->KeyEvent( DIK_RALT ) )
	{	
		
		if( g_input->KeyDown( DIK_B ) )
		{
			if( m_craftHelpWin->ToggleState() )
			{
				SetFocus( m_craftHelpWin );
				m_craftHelpWin->SetCraftHelpWin( "" );
			}
			else
			{
				g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );
			}
		}
		
		else if( g_input->KeyDown( DIK_C ) )
		{
			if( m_charWin->ToggleState() ) SetFocus( m_charWin );
		}
		
		else if( g_input->KeyDown( DIK_D ) && g_cgv.isDeveloper )
		{
			g_cgv.displayDebugMessage = !g_cgv.displayDebugMessage;
		}
#ifndef	_GTH_ONLINE_VERSION
		
		else if( g_input->KeyDown( DIK_F ) )
		{
			extern int	g_freeMove;
			g_freeMove	=	! g_freeMove;
		}
#else
		
		else if( g_input->KeyDown( DIK_F ) )
		{
			if( m_communityWin->ToggleState() ) SetFocus( m_communityWin );
		}
#endif



		
		else if( g_input->KeyDown( DIK_G ) )
		{
			if( m_guildWin->ToggleState() )	SetFocus( m_guildWin );
		}
		
		else if( g_input->KeyDown( DIK_H ) )
		{
			if( m_helpWin->ToggleState() ) SetFocus( m_helpWin );
		}
		
		else if( g_input->KeyDown( DIK_I ) )
		{
			
			
			if(g_GonryunBattlePractic.m_ibattleMode!=PCB_NONE)
			{
				return 0;	
			}
			
			if( m_itemWin->ToggleState() ) SetFocus( m_itemWin );
		}
		
		else if( g_input->KeyDown( DIK_M ) )
		{
			m_mapWin->ToggleState();
		}
		
		else if( g_input->KeyDown( DIK_O ) )
		{
			if( m_optionWin->ToggleState() ) SetFocus( m_optionWin );
		}
		
		else if( g_input->KeyDown( DIK_Q ) )
		{
			if( m_questWin->ToggleState() ) SetFocus( m_questWin );
		}
		
		else if( g_input->KeyDown( DIK_S ) )
		{
			if( m_skillWin->ToggleState() ) SetFocus( m_skillWin );
		}
		
		else if( g_input->KeyDown( DIK_W ) )
		{
			GTH_SendMessage_RequestWeaponChange();
		}
		
		else if( g_input->KeyDown( DIK_X ) )
		{
			
			g_ifMng->m_ServerListWin->Enable(FALSE);
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,320), g_LPACK.GetMassage(0,321), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_EXIT_CLIENT );
		}
		
		else if( g_input->KeyDown( DIK_Z ) )
		{
			if( GTH_IsGameMaster( g_pApp->m_myCharacter ) )
			{
				if( m_gmWin->ToggleState() ) SetFocus( m_gmWin );
			}
		}
		
		 
#ifdef	_GTH_ONLINE_VERSION
		else if( g_input->KeyDown( DIK_1 ) ) m_hotWin->m_curPage = 0;
		else if( g_input->KeyDown( DIK_2 ) ) m_hotWin->m_curPage = 1;
		else if( g_input->KeyDown( DIK_3 ) ) m_hotWin->m_curPage = 2;
		else if( g_input->KeyDown( DIK_4 ) ) m_hotWin->m_curPage = 3;
#else
		else if( g_input->KeyDown( DIK_1 ) )
		{
			g_speedModifier	=	1.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_2 ) )
		{
			g_speedModifier	=	2.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_3 ) )
		{
			g_speedModifier	=	3.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_4 ) )
		{
			g_speedModifier	=	4.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_5 ) )
		{
			g_speedModifier	=	5.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_6 ) )
		{
			g_speedModifier	=	6.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_7 ) )
		{
			g_speedModifier	=	7.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
		else if( g_input->KeyDown( DIK_8 ) )
		{
			g_speedModifier	=	8.0f;
			g_pApp->m_myCharacter->moveSpeed	=	3.0f * g_speedModifier;
		}
#endif
	}
	
	
	
	if( g_input->KeyDown( DIK_ESCAPE ) && !m_msgWin->State() )
	{
		if( !g_pApp->m_myCharacter->isAlive && g_cgv.state == CLIENT_GAME_PROCESS ) 
		{
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,418), MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_RESTART );
		}
	}

	
	
	if (g_ifMng->CheckConditionalWindow() == FALSE)
	{
		
		if( g_input->KeyDown( DIK_F1 ) ) m_hotWin->HotKeyProcess( 0 );
		else if( g_input->KeyDown( DIK_F2 ) ) m_hotWin->HotKeyProcess( 1 );
		else if( g_input->KeyDown( DIK_F3 ) ) m_hotWin->HotKeyProcess( 2 );
		else if( g_input->KeyDown( DIK_F4 ) ) m_hotWin->HotKeyProcess( 3 );
		else if( g_input->KeyDown( DIK_F5 ) ) m_hotWin->HotKeyProcess( 4 );
		else if( g_input->KeyDown( DIK_F6 ) ) m_hotWin->HotKeyProcess( 5 );
		else if( g_input->KeyDown( DIK_F7 ) ) m_hotWin->HotKeyProcess( 6 );
		else if( g_input->KeyDown( DIK_F8 ) ) m_hotWin->HotKeyProcess( 7 );
		else if( g_input->KeyDown( DIK_F12 ) ) m_renderFlag = !m_renderFlag;	
	}
	
	if( g_input->KeyDown( DIK_INSERT ) ) g_SkillApp.ActionProcess( ACTION_SIT );

	return 1;
}


void CIFManager::SetVAMessage(char* title, LPCTSTR lpszFormat, ...)
{
	if( m_msgWin->State() )
	{
		if( m_msgWin->m_buttonType != MSG_BUTTONTYPE_NONE ) return;
	}

	va_list argList;
	va_start(argList, lpszFormat);
	char szOutputMessage[256];
	vsprintf(szOutputMessage, lpszFormat, argList);
	va_end(argList);

	m_msgType = IF_MESSAGE_NONE;
	
	m_msgWin->Enable();
	m_msgWin->m_buttonType = MSG_BUTTONTYPE_OK;
	if( title == NULL ) g_ifMng->m_msgTitleStr[0] = 0;
	else 
	{		
		strncpy( g_ifMng->m_msgTitleStr, title ,255);
		g_ifMng->m_msgStr[255] = NULL;
	}
	strncpy( g_ifMng->m_msgStr, szOutputMessage,255 );
	g_ifMng->m_msgStr[255] = NULL;
	g_ifMng->m_msgWaitTime = -1.0f ;
	g_ifMng->m_msgTime = g_timer.GetAppMilliTime(); 

	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_NOTICE ), GTH_SOUND_PLAY_ONCE );	
}


bool CIFManager::SetMessage( char* title, char* msg, int button, int type, float limitTime )
{
	if( m_msgWin->State() )
	{
		if( m_msgWin->m_buttonType != MSG_BUTTONTYPE_NONE ) return false;
		else if( type == IF_MESSAGE_EXIT_CLIENT && button == 0 ) return false;
	}

	m_msgType = type;
	
	m_msgWin->Enable();
	m_msgWin->m_buttonType = button;
	


	


	

	if( title == NULL ) g_ifMng->m_msgTitleStr[0] = 0;
	else 
	{		
		
		strncpy( g_ifMng->m_msgTitleStr, title ,255);
		g_ifMng->m_msgStr[255] = NULL;
	}
	
	
	strncpy( g_ifMng->m_msgStr, msg,255 );
	g_ifMng->m_msgStr[255] = NULL;
	

	g_ifMng->m_msgWaitTime = limitTime;
	g_ifMng->m_msgTime = g_timer.GetAppMilliTime(); 

	
	g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_NOTICE ), GTH_SOUND_PLAY_ONCE );

	return true;
}

void CIFManager::ExitMessage()
{
	m_msgWin->Enable( false );
}

void CIFManager::AddSysMessage( char* message, DWORD color )
{
	g_ifMng->m_dlgWin->AddChatMessage( message, color );
}


void CIFManager::AddSysVAMessage( DWORD color , LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	char szOutputMessage[256];
	vsprintf(szOutputMessage, lpszFormat, argList);
	va_end(argList);
	
	g_ifMng->m_dlgWin->AddChatMessage( szOutputMessage, color );
}

void CIFManager::AddNoticeMessage( char* message, DWORD color )
{
	strcpy( m_noticeMsg, message );
	m_noticeColor = color;
	m_noticeTime = g_timer.GetAppMilliTime();
}

void CIFManager::AddChatMessage( char* message, DWORD color, char* nameMsg, int type )
{
	char szTemp[2048];
	memset( szTemp, 0, sizeof( szTemp ) );

	if( nameMsg == NULL )
	{
		strcpy( szTemp, message );
		g_ifMng->m_dlgWin->AddChatMessage( szTemp, color );
	}
	else
	{
		strcpy( szTemp, nameMsg );
		
#if defined _GTH_TERRA_VERSION || _GTH_TAIWAN_VERSION	
		strcat( szTemp, ": " );
#else
		strcat( szTemp, ":" );
#endif

		
		
		strcat( szTemp, message );
		g_ifMng->m_dlgWin->AddChatMessage( szTemp, color, type );

	}
}



void GTH_ProcessMessage_ReplyLogin_Continue(void);


int CIFManager::MessageProcessOk()
{

	
	
	if(IF_MESSAGE_ITEM_USE_RESERVED < m_msgType)
	{
		ExitMessage();

		g_ItemUseProc.MessageProcessOk(m_msgType);
		return( IF_CTL_NONE );
	}	

	switch( m_msgType )
	{
	case IF_MESSAGE_EXIT_CLIENT :
		if (g_TutorialMng->GetTotutirlMode() ) g_TutorialMng->SaveHistory("history.his");	

		g_pApp->m_bAppExit = TRUE;
		return( IF_CTL_NONE );
		break;
	case IF_MESSAGE_DELETE_CHARACTER :
		m_loginWin->m_deleteFlag = true;
#ifdef _GTH_ONLINE_VERSION
		ExitMessage();
		m_loginWin->SetDeleteCharacter();
#endif
		break;
		
		
	case IF_MESSAGE_STORE_BUY_ITEM :
		
		if (g_ifMng->m_storeWin->m_StoreType == CIFStoreWin::GAMBLE_ITEM_STORE)
		{
			
			g_ifMng->m_storeWin->m_LockGambleStore = TRUE;

			GTH_SendMessageRequest_BuyGambleItem(m_storeWin->m_storeItem[m_storeWin->m_buyItemIdx].idxOnServer);
		}
		else
			GTH_SendMessage_RequestBuyItem( m_storeWin->m_storeItem[m_storeWin->m_buyItemIdx].idxOnServer );		
		
		ExitMessage();
		break;
	case IF_MESSAGE_STORE_SELL_ITEM :
		GTH_SendMessage_RequestSellItem();
		ExitMessage();
		break;
	case IF_MESSAGE_STORE_BUY_SKILL :
		GTH_SendMessage_Request_BuySkill( m_storeWin->m_storeItem[m_storeWin->m_buyItemIdx].idxOnServer );
		ExitMessage();
		break;
	case IF_MESSAGE_STORE_BUY_STACK_ITEM :
		GTH_SendMessage_RequestBuyItem( m_storeWin->m_storeItem[m_stackWin->m_saveInvenPos].idxOnServer, m_stackWin->m_count );
		ExitMessage();
		break;
	case IF_MESSAGE_STORE_CHARGE_RA :
		GTH_SendMessage_RequestEpackStore( m_stackWin->m_chargeType, m_stackWin->m_count );
		ExitMessage();
		break;
	case IF_MESSAGE_STORE_CHARGE_SA :
		GTH_SendMessage_RequestEpackStore( m_stackWin->m_chargeType, m_stackWin->m_count );
		ExitMessage();
		break;

	case IF_MESSAGE_USE_PORTAL :
		ExitMessage();
		GTH_SendMessage_Request_UsePortal( g_cgv.saveInventoryIdx );
		break;
	case IF_MESSAGE_LEARN_SKILL : 
		ExitMessage();
		GTH_SendMessage_Request_LearnSkill( g_cgv.saveInventoryIdx );
		break;
	
		
	case IF_MESSAGE_PCTRADE_REQUEST :
		GTH_SendMessage_PCTrade_Admit();
		ExitMessage();
		break;
	case IF_MESSAGE_PARTY_REPLY_ORGANIZE :
		ExitMessage();
		GTH_SendMessage_Party_Admit_Organize();
		break;
	case IF_MESSAGE_PARTY_REPLY_JOIN :
		ExitMessage();
		GTH_SendMessage_Party_Admit_Join();
		break;
	case IF_MESSAGE_PARTY_OUT :
		ExitMessage();
		GTH_SendMessage_Party_Out();
		break;
	case IF_MESSAGE_PARTY_FORCEDOUT :
		ExitMessage();
		GTH_SendMessage_Party_ForcedOut( m_partyWin->GetPartyMemberName() );
		break;
	case IF_MESSAGE_PARTY_DISORGANIZE :
		ExitMessage();
		GTH_SendMessage_Party_Disorganize();
		break;
	case IF_MESSAGE_SKILL_LEVEL_UP :
		ExitMessage();
		GTH_SendMessage_Request_SkillLevelUp( m_learnWin->m_skillInfo[m_learnWin->m_selectedIdx].idx, m_learnWin->m_genType );
		break;
	case IF_MESSAGE_BINDING :
		ExitMessage();
		GTH_SendMessage_RequestBinding( 0, m_saveBindingIdx, m_saveBindingCost );
		break;
	case IF_MESSAGE_RESTART :
		ExitMessage();
		
		if ( g_ifMng->m_stackWin->State() )
		{
			g_ifMng->m_stackWin->m_saveInvenPos = -1;
			g_ifMng->m_stackWin->m_saveItemPrice = 0;
			g_ifMng->m_stackWin->m_stackType = -1;
			g_ifMng->m_stackWin->Enable(FALSE);
		}
		if( !g_pApp->m_myCharacter->isAlive ) GTH_SendEventMessage_Respawn();
		break;
	case IF_MESSAGE_RESTART_CHARACTER :
		ExitMessage();
		
		if ( g_ifMng->m_stackWin->State() )
		{
			g_ifMng->m_stackWin->m_saveInvenPos = -1;
			g_ifMng->m_stackWin->m_saveItemPrice = 0;
			g_ifMng->m_stackWin->m_stackType = -1;
			g_ifMng->m_stackWin->Enable(FALSE);
		}
		g_cgv.restartState = RESTART_CHARACTER;
		m_changeStateFlag = true;
		



		g_GolryunBattle.SetupReStartGame();
		
		g_NewDanBattleSystem.InitDanBattleInfo();
		
		break;
	case IF_MESSAGE_ALLREPAIR_ITEM :
		ExitMessage();
		GTH_SendMessage_RequestRepairItem( 2, 0 );
		break;
	case IF_MESSAGE_REPAIR_ITEM :
		ExitMessage();
		GTH_SendMessage_RequestRepairItem( m_itemWin->m_repairType, m_itemWin->m_repairPos );
		break;
	case IF_MESSAGE_EPACK_HOLE_ITEM :
		ExitMessage();
		GTH_SendMessage_RequestDestroyItem();
		break;
	case IF_MESSAGE_STOP_QUEST :
		ExitMessage();
		GTH_SendMessage_DeleteQuestStatus( m_questWin->m_selectedQuestIdx );
		break;
	case IF_MESSGAE_GM_OK :
		ExitMessage();
		m_gmWin->ProcessGM();
		break;
	case IF_MESSAGE_SUMMONS :
		ExitMessage();
		GTH_SendMessage_ReplySummons( 1 );
		break;

	
	case IF_MESSAGE_MONSTER_SUMMON:
		ExitMessage();
		

		
		GTH_SendMessage_RequestDestroyItem(g_cgv.saveInventoryIdx);
		break;

	case IF_MESSAGE_TRANSFER_WITH_NPC :
		ExitMessage();
		GTH_SendMessage_RequestBinding( 1, m_saveBindingIdx, m_saveBindingCost );
		break;
	
	case IF_MESSAGE_DEPOT_ITEM :
		ExitMessage();
		GTH_SendMessage_ItemSystem_DepotItem( m_depotWin->m_saveDepotPos );
		break;
	case IF_MESSAGE_PRECOCITY_ITEM :
		ExitMessage();
		GTH_SendMessage_ItemSystem_PrecocityItem( m_precocityWin->m_savePrecocityPos );
		break;

	case IF_MESSAGE_CRAFT_COMPLETE :
		ExitMessage();
		m_craftWin->StartCraft( 10000.0f );
		break;
	case IF_MESSAGE_BUY_BOOTH_ITEM :
		ExitMessage();
		GTH_SendMessage_ItemSystem_BoothBuyItem( m_boothWin->m_saveItemIdx, m_stackWin->m_count );
		break;

	case IF_MESSAGE_DROP_ITEM :
		ExitMessage();
		GTH_SendMessage_DropItem( g_cgv.myCharacterInfo->mouseInventory, m_itemDropPos );
		break;
	case IF_MESSAGE_UPGRADE_COMPLETE :
		ExitMessage();
		m_upgradeWin->StartUpgrade( 10000.0f );
		break;
	case IF_MESSAGE_INITSTATUS :
		ExitMessage();
		GTH_SendMessage_QuestLogic_InitStatus();
		break;
	
	
	
	
		
		
		break;
	case IF_MESSAGE_FRIENDS_REPLY_INVITE :
		ExitMessage();
		GTH_SendMessage_Friends_Accept( g_cgv.myCharacterInfo->tempFriendIdx );
		break;
	case IF_MESSAGE_FRIENDS_REQUEST_REMOVE :
		ExitMessage();
		GTH_SendMessage_Friends_Remove( g_ifMng->m_friendWin->GetSelectedFriendName() );
		break;

	
	case IF_MESSAGE_GUILD_INVITE_REPLY :
		ExitMessage();
		GTH_SendMessage_Guild_Invite_Reply( 1 );
		break;

	
	case IF_MESSAGE_QUESTSCROLL_OBTAIN_PREPARE :
		ExitMessage();
		m_questscrollWin->StartWriting( 6000.0f );
		break;
	case IF_MESSAGE_QUESTSCROLL_OBTAIN_COMPLETE :
		ExitMessage();
		g_ifMng->m_questscrollWin->m_viewItemFlag = false;
		break;

	
	case IF_MESSAGE_QUESTSCROLL_ACTIVE :
		ExitMessage();
		GTH_SendMessage_RequestDestroyItem( g_cgv.saveInventoryIdx );
		break;

	case IF_MESSAGE_GUILD_REQUEST_DISORGANIZE1:
	{
		char buffer[256];

		ExitMessage();
		
		sprintf( buffer, g_LPACK.GetMessage(0,875), g_cgv.myCharacterInfo->guild.name );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), buffer, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_DISORGANIZE2, 10000.0f );
		break;
	}

	case IF_MESSAGE_GUILD_REQUEST_DISORGANIZE2:
		ExitMessage();
		GTH_SendMessage_Guild_Disorganize_Request();
		break;

	case IF_MESSAGE_GUILD_REQUEST_SECEDE:
		ExitMessage();
		GTH_SendMessage_Guild_Secede_Request();
		break;

	case IF_MESSAGE_GUILD_REQUEST_BANISH:
		ExitMessage();
		GTH_SendMessage_Guild_Banish_Request( m_tempString );
		break;

	case IF_MESSAGE_GUILD_REQUEST_CHANGE_MASTER1:
	{
		char buffer[256];

		ExitMessage();
		
		sprintf( buffer, g_LPACK.GetMessage(9,37), m_tempString );
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), buffer, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_GUILD_REQUEST_CHANGE_MASTER2, 10000.0f );
	}
		break;

	case IF_MESSAGE_GUILD_REQUEST_CHANGE_MASTER2:
		ExitMessage();
		GTH_SendMessage_Guild_Change_Master_Request( m_tempString );
		break;
		
	case IF_MESSAGE_GUILD_REQUEST_ADD_SUBMASTER:
		ExitMessage();
		GTH_SendMessage_Guild_Add_Submaster_Request( m_tempString );
		break;

	case IF_MESSAGE_GUILD_REQUEST_ADD_FACTIONMASTER:
		ExitMessage();
		GTH_SendMessage_Guild_Add_Factionmaster_Request( m_tempString );
		break;

	case IF_MESSAGE_GUILD_REQUEST_DELETEMASTER:
		ExitMessage();
		GTH_SendMessage_Guild_Delete_Master_Request( m_tempString );
		break;
		
	case IF_MESSAGE_DELETE_MAIL :
		ExitMessage();
		g_ifMng->m_mailWin->DeleteSelectedMails();
		break;

		
	case IF_MESSAGE_USE_POTION :
		{
			ExitMessage();

			
			if(gcTools.IfSkillCasting_NotItemUse()) break;
			
			
			if(!g_SkillItem.CheckValid_UseSkillItem(g_cgv.saveInventoryIdx)) break;

		}
		
		break;
		

	
	case IF_MESSAGE_DEAD_USE_SKILL:
		{
			ExitMessage();

			
			
			

			
			g_cgv.saveTargetType	= ENTITY_PC;
			g_cgv.saveTargetIdx		= g_cgv.myPCIdxOnServer;
			
			GTH_SendMessage_Request_UsePotion(g_cgv.saveInventoryIdx);
		}
		break;


		
	case IF_MESSAGE_DAN_BATTLE_SUGGEST_ACCEPT:
		ExitMessage();
		g_DanBattle.SendSuggestResult(TRUE);			

		
		
		g_ifMng->m_guildArenaWin->m_RegBteamIdx = g_ifMng->m_guildArenaWin->m_MyTextIdxinStandby;
		g_ifMng->m_guildArenaInfoWin->SetBattleTeams(g_ifMng->m_guildArenaWin->m_RegAteamIdx, 
			g_ifMng->m_guildArenaWin->m_RegBteamIdx);
		break;
		
	
	
	case IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING:
		GTH_SendMessage_PCTrade_AdmitBatting();
		ExitMessage();
		
		break;
	case IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_GENERATE:
		ExitMessage();
		g_GonryunBattlePractic.SendSuggestResultGenerate(TRUE);
		
		break;
	
	

	
	case IF_MESSAGE_TRANSPORT_GOLRYUN_ACCEPT:
		ExitMessage();
		g_GolryunBattle.ProcessTransGolryunBattle();
		g_ifMng->m_chatWin->Enable( false );
		break;

	case IF_MESSAGE_TRANSPORT_ARAYAN_ACCEPT:
		ExitMessage();
		GTH_SendMessage_TransportWorld( 0, 0 );
		g_ifMng->m_chatWin->Enable( false );
		break;

	case IF_MESSAGE_GB_BATTLE_SUGGEST_ACCEPT:
		ExitMessage();
		g_GolryunBattle.ProcessSuggestBattle();
		g_ifMng->m_chatWin->Enable( false );
		break;
	


	
	case IF_MESSAGE_USE_EVENT_SNOW_ITEM:
		ExitMessage();		
		
		




			g_WeatherSystem.GTH_SendMessage_Request_WeatherSetting(
				TIME_SNOW, 
				CWeatherSystem::enumProgressLimitTime::DEFAULT_LIMIT_TIME,
				1, g_cgv.saveInventoryIdx);


		break;	
	
	case IF_MESSAGE_USE_EVENT_RAIN_ITEM:
		ExitMessage();		
		g_WeatherSystem.GTH_SendMessage_Request_WeatherSetting(
			TIME_RAIN, 
			CWeatherSystem::enumProgressLimitTime::DEFAULT_LIMIT_TIME,
			1, g_cgv.saveInventoryIdx);
		break;

	
	case IF_MESSAGE_FREE_USER_SELECT_SLOT:
		ExitMessage();
		g_ifMng->m_loginWin->SelectSlot(true);
		break;

	case IF_MESSAGE_REPLY_LOGIN:
		GTH_ProcessMessage_ReplyLogin_Continue();
		break;

	
	case IF_MESSAGE_MAGIC_BOOK:
		ExitMessage();

		
		if(gcTools.IfSkillCasting_NotItemUse()) break;
		
		GTH_SendMessage_Request_MasicBook(g_cgv.saveInventoryIdx);
		break;
		
	
	case IF_MESSAGE_USE_EPACK_CHARG_SOUL:
		ExitMessage();		
		GTH_SendMessage_Request_EpackChargSoul(g_cgv.saveInventoryIdx);
		break;

	case IF_MESSAGE_POSTPACKAGE_REMAIN_EXIT:
		ExitMessage();
		g_ifMng->m_mailBoxWin->Enable(false);
		g_PostFunc.GTH_SendMessage_MailBox_Exit();		
		break;

	case IF_MESSAGE_HELPERSYSTEM_DOYOUWANTME:
		ExitMessage();
		g_HelperSystem.GTH_SendMessage_HelpSystem_AnswerYesOrNo(1);
		break;

	case IF_MESSAGE_HELPERSYSTEM_GIVEPOINT:
		ExitMessage();
		if(g_HelperSystem.m_MyPointInfo.iMyPoint == 0)
			
			g_ifMng->AddSysMessage(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 308));
		else
		{
			
			g_ifMng->m_stackWin->InitStack(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 302), NULL, IF_STACK_TYPE_HELPERSYSTEM_MYPOINT);
		}
		break;

	case IF_MESSAGE_HELPERSYSTEM_DOYOUWANTPORTAL:
		ExitMessage();
		g_HelperSystem.GTH_SendMessage_HelpSystem_SpawnMeAnswer(1);
		break;

	
	case IF_MESSAGE_POSTPACKAGE_SEND:
		ExitMessage();
		g_PostFunc.GTH_SendMessage_PostSystem_Send();
		g_chatManager->SetInputState(INPUT_INDEX_POST_NAME, -1);
		g_chatManager->SetInputState(INPUT_INDEX_POST_MESSAGE, -1);
		g_chatManager->SetInputState(INPUT_INDEX_POST_TITLE, -1);
		g_chatManager->ChangeInputMessage(INPUT_INDEX_DIALOGBOX);
		g_ifMng->m_mailBoxWin->Enable(false);		
		break;

	
	case IF_MESSAGE_MODIFY_OPTION:
		{
			ExitMessage();
			enum CashMallItem_OptionStone::enumUseItemMode mode =
				g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemMode;

			int OptionArryIdx = g_ifMng->m_CashMallItem->m_OptionArryIdx;
			GTH_SendMessageRequest_ItemModify(mode, OptionArryIdx);
		}
		break;
	
	case IF_MESSAGE_MODIFY_OPTION_SUCCECS:
		{
			ExitMessage();
			g_ifMng->m_CashMallItem->Enable(false);
			GTH_SendMessageRequest_OptionItemWinClose();
		}
		break;

	case IF_MESSAGE_INIT_AWEAKEN:
		{		
			ExitMessage();		
			int ItemPos = g_cgv.myCharacterInfo->m_CashMall_OptionStone.m_iUsingItemInvenIdx;
			GTH_SendMessageRequest_ReverseOfMemory(ItemPos);
		}
		break;

	case IF_MESSAGE_MODIFY_ITEM_DESTROY:
		{
			ExitMessage();					
			g_ifMng->m_CashMallItem->Enable(false);
			GTH_SendMessageRequest_OptionItemWinClose();
		}break;
		
		
		
		
	
	case IF_MESSAGE_REQUEST_PC_SUMMON:
		{
			ExitMessage();
			g_ifMng->m_Item_ScrollWin->Select_PC_Summon(CItem_ScrollWin::enumTargetPlayerReply::REPLY_OK);
		}
		break;

	case IF_MESSAGE_REQUEST_PC_MOVE_SUMMON:
		{
			ExitMessage();
			g_ifMng->m_Item_ScrollWin->Select_PC_Move_Summon(CItem_ScrollWin::enumTargetPlayerReply::REPLY_OK);
		}
		break;

	


	default :
		ExitMessage();
		break;
	}
	return( IF_CTL_SELECT );
}


int CIFManager::MessageProcessCancel()
{
	switch( m_msgType )
	{
		
	case IF_MESSAGE_EXIT_CLIENT:
		ExitMessage();
		if (g_ifMng->m_loginWin->m_loginStep == LOGIN_STATE_SELECT_SERVER)
			g_ifMng->m_ServerListWin->Enable(TRUE);
		break;
		

		
	case IF_MESSAGE_DELETE_CHARACTER_COMPLETE :
		m_loginWin->m_bDeleteCharFlag = false;
		ExitMessage();
		break;
	case IF_MESSAGE_PCTRADE_REQUEST : 
		GTH_SendMessage_PCTrade_Reject( false );
		ExitMessage();
		break;
	case IF_MESSAGE_PCTRADE_WAITREPLY : 
		GTH_SendMessage_PCTrade_Reject( true );
		ExitMessage();
		break;
	case IF_MESSAGE_PARTY_REQUEST_ORGANIZE :
		ExitMessage();
		GTH_SendMessage_Party_Reject_Organize( true );
		break;
	case IF_MESSAGE_PARTY_REPLY_ORGANIZE :
		ExitMessage();
		GTH_SendMessage_Party_Reject_Organize( false );
		break;
	case IF_MESSAGE_PARTY_REQUEST_JOIN :
		ExitMessage();
		GTH_SendMessage_Party_Reject_Join( true );
		break;
	case IF_MESSAGE_PARTY_REPLY_JOIN :
		ExitMessage();
		GTH_SendMessage_Party_Reject_Join( false );
		break;
	case IF_MESSAGE_PARTY_OUT :
		ExitMessage();
		break;
	case IF_MESSAGE_PARTY_FORCEDOUT :
		ExitMessage();
		break;
	case IF_MESSAGE_PARTY_DISORGANIZE :
		ExitMessage();
		break;
	case IF_MESSAGE_SUMMONS :
		ExitMessage();
		GTH_SendMessage_ReplySummons( 0 );
		break;

	
	case IF_MESSAGE_MONSTER_SUMMON:
		ExitMessage();
		break;

	case IF_MESSAGE_CRAFT_COMPLETE :
		ExitMessage();
		g_ifMng->m_craftWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		break;

	case IF_MESSAGE_UPGRADE_COMPLETE :
		ExitMessage();
		g_ifMng->m_upgradeWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		break;

	
	
	case IF_MESSAGE_FRIENDS_REQUEST_INVITE :
		ExitMessage();
		GTH_SendMessage_Friends_Reject( g_cgv.myCharacterInfo->tempFriendIdx );
		break;
	case IF_MESSAGE_FRIENDS_REPLY_INVITE :
		ExitMessage();
		
		break;
	case IF_MESSAGE_FRIENDS_REQUEST_REMOVE :
		ExitMessage();
		break;
	
	
	
	case IF_MESSAGE_GUILD_INVITE_REPLY :
		ExitMessage();
		GTH_SendMessage_Guild_Invite_Reply( 0 );
		break;
	case IF_MESSAGE_GUILD_INVITE_CANCEL :
		ExitMessage();
		GTH_SendMessage_Guild_Invite_Cancel();
		break;
	
	case IF_MESSAGE_QUESTSCROLL_OBTAIN_PREPARE :
		ExitMessage();
		g_ifMng->m_questscrollWin->m_okBtn->SetButtonState( IF_BTN_TYPE_ENABLE );
		break;

		
	case IF_MESSAGE_DAN_BATTLE_SUGGEST_ACCEPT:
		ExitMessage();
		g_DanBattle.SendSuggestResult(FALSE);

		
		g_ifMng->m_guildArenaWin->Enable(FALSE);
		

		break;
		
	
	case IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_GENERATE:
		ExitMessage();
		
		
			g_GonryunBattlePractic.SendSuggestResultGenerate(FALSE);
		
	
	
		
		
		break;
	case IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING:
		ExitMessage();
		GTH_SendMessage_PCTrade_RejectBatting();


	
	
		
		break;
	case IF_MESSAGE_GONRYUN_BATTLE_PRACTICE_BATTING_WATEREPLAY:
		ExitMessage();
		break;

	
	case IF_MESSAGE_GUILD_REQUEST_DISORGANIZE1:
	case IF_MESSAGE_GUILD_REQUEST_DISORGANIZE2:
	case IF_MESSAGE_GUILD_REQUEST_SECEDE:
	case IF_MESSAGE_GUILD_REQUEST_BANISH:
		ExitMessage();
		break;

	
	case IF_MESSAGE_TRANSPORT_GOLRYUN_ACCEPT:
		ExitMessage();	
		g_ifMng->m_chatWin->Enable( false );
		break;
	case IF_MESSAGE_GB_BATTLE_SUGGEST_ACCEPT:
		ExitMessage();
		g_ifMng->m_chatWin->Enable( false );
		break;
	
		
	
	case IF_MESSAGE_FREE_USER_SELECT_SLOT:
		ExitMessage();
		g_ifMng->m_loginWin->SelectSlot(false);
		break;

	
	case IF_MESSAGE_MAGIC_BOOK:
		ExitMessage();
		break;


	case IF_MESSAGE_HELPERSYSTEM_DOYOUWANTME:
		ExitMessage();
		g_HelperSystem.GTH_SendMessage_HelpSystem_AnswerYesOrNo(0);
		
		g_ifMng->SetMessage(g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 297));
		break;

	case IF_MESSAGE_HELPERSYSTEM_GIVEPOINT:
		ExitMessage();
		break;

	case IF_MESSAGE_HELPERSYSTEM_DOYOUWANTPORTAL:
		ExitMessage();
		g_HelperSystem.GTH_SendMessage_HelpSystem_SpawnMeAnswer(0);
		break;


	
	case IF_MESSAGE_REQUEST_PC_SUMMON:
		{
			ExitMessage();
			g_ifMng->m_Item_ScrollWin->Select_PC_Summon(CItem_ScrollWin::enumTargetPlayerReply::REPLY_REFUSE);
		}
		break;

	case IF_MESSAGE_REQUEST_PC_MOVE_SUMMON:
		{
			ExitMessage();
			g_ifMng->m_Item_ScrollWin->Select_PC_Move_Summon(CItem_ScrollWin::enumTargetPlayerReply::REPLY_REFUSE);
		}
		break;

	

	default :
		ExitMessage();
		break;
	}
	return( IF_CTL_SELECT );
}

int CIFManager::CheckConditionalWindow()
{
	if( g_ifMng->m_msgWin->State() ) return 1;
	if( g_ifMng->m_tradeWin->State() ) return 1;
	if( g_ifMng->m_storeWin->State() ) return 1;
	if( g_ifMng->m_chatWin->State() ) return 1;
	if( g_ifMng->m_learnWin->State() ) return 1;
	if( g_ifMng->m_depotWin->State() ) return 1;
	if( g_ifMng->m_precocityWin->State() ) return 1;
	if( g_ifMng->m_craftWin->State() ) return 1;

	
	if( g_ifMng->m_EventCraftWin->State() ) return 1;

	
	if( g_ifMng->m_upgradeWin->State() ) return 1;
	if( g_ifMng->m_boothWin->State() ) return 1;
	
	if( g_ifMng->m_questscrollWin->State() ) return 1;
	
	if( g_ifMng->m_autoQuestWin->State() ) return 1;
	if( g_ifMng->m_npcInvWin->State() ) return 1;
	
    if(g_ifMng->m_pgonryunBattleBattingwindow->State()) return 1;	
	

	
	if(g_ifMng->m_PostWin->State())
		return 1;

	if(g_ifMng->m_mailBoxWin->State())
		return 1;
	

	
	if ( g_ifMng->m_CashMallItem->State())
		return 1;

	
	if ( g_ifMng->m_WorldChangeWin->State())
		return 1;

	
	if ( g_ifMng->m_DanWarRequestWin->State() )
		return 1;	

	
	if ( g_ifMng->m_shareWin->State() )
		return 1;		

	return 0;
}
 
char* CIFManager::GetNumberString( int number )
{
	NUMBERFMT nFmt = { 0, 0, 3, ".", ",", 1 };
	static char szOUT[128];
	char szIN[128];
	itoa( number, szIN, 10 );
	::GetNumberFormat(NULL, NULL, szIN, &nFmt, szOUT, 64);
	return( &szOUT[0] );
}

char* CIFManager::GetTimeString( int sec )
{
	char szTemp[128], szTemp2[128];
	int tempTime;
	int second, minute, hour, day;

	szTemp[0] = 0;

	if( sec < 0 )
	{
		sprintf( szTemp2, g_LPACK.GetMassage( 0, 598 ), 0 );
		strcat( szTemp, szTemp2 );
		return( &szTemp[0] );
	}

	
	second = sec % 60;

	
	tempTime = sec / 60;
	minute = tempTime % 60;

	
	hour = tempTime / 60;

	
	day = hour / 24;
	hour = hour % 24;

	if( day > 0 )
	{
		sprintf( szTemp2, g_LPACK.GetMassage( 0, 595 ), day );
		strcat( szTemp, szTemp2 );
	}
	if( hour > 0 )
	{
		sprintf( szTemp2, g_LPACK.GetMassage( 0, 596 ), hour );
		strcat( szTemp, szTemp2 );
	}
	if( minute > 0 )
	{
		sprintf( szTemp2, g_LPACK.GetMassage( 0, 597 ), minute );
		strcat( szTemp, szTemp2 );
	}
	if( second > 0 )
	{
		sprintf( szTemp2, g_LPACK.GetMassage( 0, 598 ), second );
		strcat( szTemp, szTemp2 );
	}
	return( &szTemp[0] );
}

 
 
 
 
 
void	CIFManager::ChangeResolution	(	int	in_width,
											int	in_height	)
{
	m_clientWidth	=	in_width;
	m_clientHeight	=	in_height;

	int		index;
	int		size;
	for	(	index	=	0;	\
			index	<	NUM_IF_POSITION;	\
			++index		)
	{
		memcpy ( &m_iPos [index], &m_iBackPos [index], sizeof (IFControlInfo_t) );
		size	=	m_iPos [index].sizeX;
		GetInterfacePositionWidth ( index, m_iPos [index].clientX, size );
		size	=	m_iPos [index].sizeY;
		GetInterfacePositionHeight ( index, m_iPos [index].clientY, size );
	}

	
	m_msgWin->PrepareControl();

	
	m_startLogo->PrepareControl( IF_START_LOGO );

	
	m_loginWin->PrepareControl();

	
	m_loadingLogo->PrepareControl( IF_LOADING_LOGO );
	
	
	m_rolloverWin->PrepareControl();
	m_itemWin->PrepareControl();
	m_depotWin->PrepareControl();
	m_nameWin->PrepareControl();
	m_charWin->PrepareControl();
	m_skillWin->PrepareControl();
	m_questWin->PrepareControl();
	m_hotWin->PrepareControl();
	m_learnWin->PrepareControl();
	m_chatWin->PrepareControl();
	m_storeWin->PrepareControl();
	m_tradeWin->PrepareControl();
	m_dlgWin->PrepareControl();
	m_chatOptionWin->PrepareControl();
	m_worldChatWin->PrepareControl();
	m_partyWin->PrepareControl();
	m_boothWin->PrepareControl();
	m_menuWin->PrepareControl();
	m_optionWin->PrepareControl();
	m_stackWin->PrepareControl();
	m_helpWin->PrepareControl();
	m_gmWin->PrepareControl();
	m_mapWin->PrepareControl();
	m_miniMapWin->PrepareControl();
	
	m_communityWin->PrepareControl();
	m_mailWin->PrepareControl();
	m_mailEditWin->PrepareControl();
	m_friendWin->PrepareControl();
	
	m_questscrollWin->PrepareControl();
	
	m_guildWin->PrepareControl();
	m_guildAuthWin->PrepareControl();
	
	m_guildPvpWin->PrepareControl();
	m_guildArenaWin->PrepareControl();

	
	m_guildArenaInfoWin->PrepareControl();

	
	m_autoQuestWin->PrepareControl();

	
	m_surgeryWin->PrepareControl();
	
	m_pgonryunBattlePractice->PrepareControl();
	m_pgonryunBattleBattingwindow->PrepareControl();
	m_pGonryunBattleProgressInfo->PrepareControl();
	

	
	m_battleTicketWin->PrepareControl();
	m_shareWin->PrepareControl();
	m_tournamentWin->PrepareControl();
	m_prizeWin->PrepareControl();
	m_golryunInfoWin->PrepareControl();
	m_observerMenuWin->PrepareControl();
	m_observerInfoWin->PrepareControl();
	m_tournamentMiniWin->PrepareControl();
	


	
	m_PostWin->PrepareControl();
	m_mailBoxWin->PrepareControl();
	

	
	m_WorldChangeWin->PrepareControl();


	m_HelperWin->PrepareControl();


	
	m_GiftCardWin->PrepareControl();

	
	m_ServerListWin->PrepareControl();
	

	
	m_CashMallItem->PrepareControl();

	
	m_Item_ScrollWin->PrepareControl();

	
	m_DanWarRequestWin->PrepareControl();
	m_DanWarBattleWin->PrepareControl();


}

void CIFManager::InitSoundVolume()
{
	g_musicMng->SetBgmVolumePercentage( m_settingWin->m_bgmScrollBar->m_current * 10 );
	g_musicMng->SetEffectVolumePercentage( m_settingWin->m_effectScrollBar->m_current * 10 );
}








void CIFManager::RenderChar(void)
{
	if( g_cgv.state != CLIENT_GAME_PROCESS ) return;
	if(	! g_pApp ->m_renderEnabled)	return;
	
	
	
	m_guildMarkMng->PrepareToRegisterDrawee();


	g_chatManager->SetOutputType( CHAT_OUTPUT_BORDER );
	g_chatManager->BeginFrame();

	{
		char			name[256]="";
		vec3_t			pos;
		float			zPos=0.0f, wPos=0.0f;
		int				x=0, y=0, size=0;
		Fx_CHARACTER_t* character=NULL;
		int				maxsize=0;
		
		for(int idx = 0; idx < g_charManager->m_iNumCharacters; idx++){
			character = &g_charManager->m_Characters[idx];			
			if( character->flag_culled ) continue;			
			
			if(FALSE ==  RenderChar_SetChrNameColor(character)) continue;			
			RenderChar_SetChrName(name,character);

			
			RenderChar_DanBattleEffect(character);	

			
			wPos = g_charManager->GetCharPanelScreenPos( character, pos );
			zPos = pos[2];
			size = g_chatManager->GetChatStrSize( name );
			maxsize = size;

			x = (int)pos[0];
			y = (int)pos[1];
			x = x - size / 2;
			y -= 30;
			
			g_chatManager->SetZPos( zPos );
			g_chatManager->SetWPos( wPos );
			if( x < 0 || y < 0 || x > g_ifMng->m_clientWidth || y > g_ifMng->m_clientHeight ) continue;
			g_chatManager->SetOutputType( CHAT_OUTPUT_BORDER );			

			
			if ( g_ifMng->m_VisibleName == TRUE )
			{			
				g_chatManager->TextOut( x, y, name );
				RenderChar_Guild(character,maxsize,pos,zPos,wPos);
				RenderChar_render_select_sphere(character,x,y,maxsize,zPos,wPos);
				RenderChar_booth(character,pos);
				RenderChar_Bubble(character,pos);			
			}
			

		}
	}

	RenderChar_Notice();
	
	
	
	g_NewDanBattleSystem.RenderDanBattle_Notice();
	
	
	g_chatManager->SetReverseSize( 5 );
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->EndFrame();

	m_guildMarkMng->FinishToRegisterDrawee();

	
	if ( g_ifMng->m_VisibleName == TRUE )
	m_guildMarkMng->DrawPactMarks();

}






BOOL CIFManager::RenderChar_isOutputGuild(Fx_CHARACTER_t*  character)
{
	syncCharacter_t* pSyncCharacter=NULL;
	pSyncCharacter = &g_cgv.syncCharacter[character->ID];

	if( pSyncCharacter->entityType != ENTITY_PC ) return FALSE;
	if( pSyncCharacter->guildIdx < 0) return FALSE;
	if( TRUE == pSyncCharacter->hide) return FALSE;
	if((TRUE == pSyncCharacter->waitingRoom) && (pSyncCharacter->joinNum > 0)) return FALSE;
	if(FALSE == GTH_RenderGM( character ) ) return FALSE;

	return TRUE;
}


BOOL CIFManager::RenderChar_SetChrNameColor(Fx_CHARACTER_t* character)
{
	switch(character->entityType){
		case ENTITY_MONSTER:
			if( !( character->stat_alive == MON_ALIVE_STATE_NORMAL ||
			       character->stat_alive == MON_ALIVE_STATE_SPAWN || 
			       character->stat_alive == MON_ALIVE_STATE_CORPSE ) ) return FALSE;
			g_chatManager->SetColor( D3DCOLOR_ARGB( 255, 255, 55, 40 ) );
			return TRUE;

		case ENTITY_NPC:

			if( !( character->stat_alive == MON_ALIVE_STATE_NORMAL ||
			       character->stat_alive == MON_ALIVE_STATE_SPAWN || 
			       character->stat_alive == MON_ALIVE_STATE_CORPSE ) ) return FALSE;
			g_chatManager->SetColor( 0xff6495ed );
			return TRUE;

		case ENTITY_PC:
			{
				static const DWORD g_chaosColor[] =
				{
					0xffffffff,		
					0xffff69b4,		
					0xffff0000,		
					0xffa52a2a,		
					0xffa52a2a,		
				};

				g_chatManager->SetColor( 0xffffffff );
				if( !GTH_IsGameMaster( character ) ) 
				{
					
					int chaosColorIdx = GetChaosClass( character );
					if(chaosColorIdx == 0) g_GolryunBattle.SetManorGrade(character);
					else g_chatManager->SetColor( g_chaosColor[chaosColorIdx] );			
				}else {
					if(FALSE == GTH_RenderGM( character ) ) return FALSE;
					g_chatManager->SetColor( D3DCOLOR_ARGB(255,255,255,100) );
				}				
			}
			return TRUE;
	}
	return FALSE;
}



void CIFManager::RenderChar_SetChrName(char* pName,const Fx_CHARACTER_t* character)
{
	switch(character->entityType){
		case ENTITY_PC:
			

			if(TRUE == character->m_bWaitingRoom){				
				sprintf( pName, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,396), character->m_JoinNum );
				break;
			}
			
			if(character->m_JoinNum > 0){
				sprintf( pName, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,396), character->m_JoinNum );
				break;
			}

			if(TRUE == g_cgv.syncCharacter[character->ID].hide){
				strcpy(pName, "????");
				break;
			}

			strcpy( pName, character->name);
			break;

		default:
			strcpy( pName, character->name);
			break;

	}
	
	char additioanl_name[_MAX_PATH]="";
	if( GTH_IsGameMaster( g_pApp->m_myCharacter) || (TRUE == RenderChar_isCheatEnable())){
		switch(character->entityType){
			case ENTITY_PC:
				if(TRUE == character->m_bWaitingRoom)
					sprintf(additioanl_name,"[%s(LV.%d)]",character->name, character->stat_alive);
				else
					sprintf(additioanl_name,"(LV.%d)",character->stat_alive);
				break;

			default:
			case ENTITY_MONSTER:
				sprintf(additioanl_name,"(LV.%d AI.%d)",
					g_monsterTable[character->tableIdx].level, 
					g_monsterTable[character->tableIdx].AIScriptIdx);
				break;
		}
	}

	strcat(pName,additioanl_name);
}


void CIFManager::
RenderChar_booth(Fx_CHARACTER_t* character,const vec3_t pos)
{
	
	if( character->entityType != ENTITY_PC ) return;
	if( 0 == character->boothState) return;

	
	int x = (int)pos[0];
	int y = (int)pos[1];
	char name[256]="";

	
	if( character->boothState == 1 ){
		sprintf( name, "%s", g_LPACK.GetMassage( 0, 629 ) );
	}else{
		sprintf( name, "%s%s", g_LPACK.GetMassage( 0, 655 ), character->boothName );
	}

	int size = g_chatManager->GetChatStrSizeChat( name );	
	x = x - size / 2;
	
	if(TRUE == RenderChar_isOutputGuild(character))
		y -= 63;
	else
		y -= 50;

	g_chatManager->SetColor( 0xff7cfc00 );	
	g_chatManager->Output( x, y, name, true);
}




void CIFManager::RenderChar_Bubble(Fx_CHARACTER_t *character,const vec3_t pos)
{
	if( character->entityType != ENTITY_PC ) return;

	g_chatManager->SetColor( 0xffffffff );	
	if( NULL == character->chatMsg[0]) return;
	if( character->chatMsgTime + PC_CHAT_MESSAGE_RENDER_TIME < g_timer.GetAppMilliTime() )
	{
		memset( character->chatMsg, 0, sizeof( chatMessage ) );
		return;
	}

	int size = g_chatManager->GetChatStrSizeChat( character->chatMsg );
	int x = (int)pos[0];
	int y = (int)pos[1];
	x = x - size / 2;

	
	if(TRUE == RenderChar_isOutputGuild(character))
		y -= 83;
	else
		y -= 70;

	g_chatManager->Output( x, y, character->chatMsg, true );
}





void CIFManager::RenderChar_Notice(void)
{
	
	
	g_chatManager->SetZPos();
	g_chatManager->SetWPos();
	if( m_noticeTime + NOTICE_RENDER_TIME > g_timer.GetAppMilliTime() && m_noticeMsg[0] != 0 )
	{
		int y=30;
		
		g_chatManager->SetColor( m_noticeColor );
		
		
		int size = 0;
		int x =0;
		if ( m_GmNotice == TRUE )
		{			
			size = g_chatManager->GetChatStrSizeChat( g_LPACK.GetMassage(0,338) );
			x = m_clientWidth / 2 - size / 2;
		g_chatManager->Output( x, y, g_LPACK.GetMassage(0,338), true );
		}

		size = g_chatManager->GetChatStrSizeChat( m_noticeMsg );
		x = m_clientWidth / 2 - size / 2;
		g_chatManager->Output( x, y + 17, m_noticeMsg, true );
	}
}






void CIFManager::
RenderChar_render_select_sphere(
	Fx_CHARACTER_t* character,
	const int x,
	const int y,
	const int maxsize,
	const float zPos,
	const float wPos)
{
	g_chatManager->SetColor( 0xffffffff );
	
	
	if(NULL == g_cgv.pTargetChar){
		m_nameWin->m_leftSelectCharSphere->Enable( false );
		m_nameWin->m_rightSelectCharSphere->Enable( false );
		return;
	}
	
	
	if( character->idxOnServer == g_cgv.pTargetChar->idxOnServer && character->entityType == g_cgv.pTargetChar->entityType )
	{
		m_nameWin->m_leftSelectCharSphere->m_info[0].clientX = x - m_nameWin->m_leftSelectCharSphere->m_info[0].sizeX - 10;
		m_nameWin->m_leftSelectCharSphere->m_info[0].clientY = y - 5;
		m_nameWin->m_rightSelectCharSphere->m_info[0].clientX = x + maxsize + 10;
		m_nameWin->m_rightSelectCharSphere->m_info[0].clientY = y - 5;
		

		m_nameWin->m_leftSelectCharSphere->Update3DBuffer( zPos, wPos );
		m_nameWin->m_rightSelectCharSphere->Update3DBuffer( zPos, wPos );
		m_nameWin->m_leftSelectCharSphere->Enable();
		m_nameWin->m_rightSelectCharSphere->Enable();

		g_ifMng->SetRenderState();
		m_nameWin->m_leftSelectCharSphere->Render();
		m_nameWin->m_rightSelectCharSphere->Render();
	}
}





BOOL CIFManager::RenderChar_isCheatEnable(void)
{
	return FALSE;
}


void CIFManager::
RenderChar_Guild(
	Fx_CHARACTER_t *character,
	int maxsize,
	const vec3_t pos,
	const float zPos,
	const float wPos)
{
	if(FALSE == RenderChar_isOutputGuild(character)) return;


	char guildName[256]="";	
	struct CGuildCtrl::tagSummaryRecord TempGuildSummaryRecord;
	if(FALSE == g_cpGuildCtrl->GetSummaryRecord(g_cgv.syncCharacter[character->ID].guildIdx,TempGuildSummaryRecord)) return;	
	StrCpyN(guildName, TempGuildSummaryRecord.Name,GUILDNAMESTRING);
	guildName[GUILDNAMESTRING]=NULL;

	
	int size = g_chatManager->GetChatStrSize( guildName );
	if( maxsize < size ) maxsize = size;

	int x = (int)pos[0];
	int y = (int)pos[1];
	x = x - size / 2;
	y -= ( 30 + 13 );
	
	g_chatManager->SetColor( 0xffffff00 );			
	

	
	
	if ( g_pApp->m_myCharacter->worldIdx== DAN_BATTLEMAP_NUM &&
		g_ifMng->m_guildArenaWin->m_ReadyBattleFlag != CIFGuildArenaWin::STATE_EVENT)		
	{
		if (g_cgv.syncCharacter[character->ID].guildIdx == g_cgv.myCharacterInfo->guildIdx)
			g_chatManager->SetColor(D3DCOLOR_ARGB(255,0,0,255));
		else 
			g_chatManager->SetColor(D3DCOLOR_ARGB(255,255,0,0));
	}				
	
	g_chatManager->TextOut( x, y-5, guildName );					
	m_guildMarkMng->RegisterPactMarkDrawee( x-18, y-9, zPos, wPos, g_cgv.syncCharacter[character->ID].guildIdx );
	x = (int)pos[0];
	x = x - maxsize / 2;

	
	
	y += 13;
}


void CIFManager::RenderChar_DanBattleEffect(Fx_CHARACTER_t* character)
{
	syncCharacter_t* pSyncChar = NULL;
	if ( character->ID < 0 || character->ID>= MAX_CHARACTERS)
		return;

	pSyncChar = &g_cgv.syncCharacter[character->ID];
	
	if ( g_NewDanBattleSystem.GetDanBattleInfo()->BattleStatus != 
		NewDanBattleSystem::DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		return;

	
	if ( (g_cgv.syncCharacter[0].nDanBattleIdx >= 0 ) &&
		( g_cgv.syncCharacter[0].worldIdx == g_NewDanBattleSystem.GetDanBattleInfo()->nBattleMapIdx) )
	{
		for (int nIDx = 0; nIDx < g_NewDanBattleSystem.v_EffectPC.size(); nIDx++){
			if ( g_NewDanBattleSystem.v_EffectPC[nIDx] == character->idxOnServer)return;
		}			
		
		
		if ( TRUE == g_NewDanBattleSystem.IsDanBattleEnemy(character->ID) )	
		{	
			
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL,	154, pSyncChar->position,NULL,  true, pSyncChar->entityType, 
				pSyncChar->idxOnServer,	0.0f,0.0f,0.0f,	-1.0f,true, FALSE );
			g_NewDanBattleSystem.v_EffectPC.push_back(pSyncChar->idxOnServer);
			return;

			
		}
		else
		{
			
			if ( pSyncChar->guildIdx == g_cgv.myCharacterInfo->guildIdx && 
				pSyncChar->nDanBattleIdx == g_NewDanBattleSystem.GetDanBattleInfo()->nDanBattleIdx &&
				pSyncChar->entityType == ENTITY_PC)
			{
				
				g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL,	153, pSyncChar->position,NULL,  true, pSyncChar->entityType,
				pSyncChar->idxOnServer,	0.0f,0.0f,0.0f,	-1.0f,true, FALSE );

				g_NewDanBattleSystem.v_EffectPC.push_back(pSyncChar->idxOnServer);
				return;
			}
		}
	}	
}