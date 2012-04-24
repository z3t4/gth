



#if !defined(AFX_IFMANAGER_H__86A8040D_25CC_469E_A444_A21402266A97__INCLUDED_)
#define AFX_IFMANAGER_H__86A8040D_25CC_469E_A444_A21402266A97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "IFCommon.h"

class CIFManager  
{

public:

	int								m_controlFlag;
	
	IFControlInfo_t					*m_iPos;
	IFControlInfo_t					*m_iBackPos;

	IFTextureInfo_t					m_texInfo[MAX_COUNT_IF_TEXTURE];
	int								m_numTexInfo;
	
	int								m_clientWidth;
	int								m_clientHeight;


	char							m_msgTitleStr[256];
	char							m_msgStr[256];
	int								m_msgType;
	float							m_msgTime;
	float							m_msgWaitTime;

	int								m_renderLoadingFlag;
	int								m_changeStateFlag;

	
	int								m_renderFlag;
	int								m_saveBindingIdx;
	int								m_saveBindingCost;

	char							m_rolloverText[2048];

	int								m_rolloverAddFirstFlag;
	int								m_mapType;
	vec3_t							m_itemDropPos;

	
	float							m_noticeTime;
	char							m_noticeMsg[256];
	DWORD							m_noticeColor;
	
	BOOL							m_GmNotice;
	
	CD3DTextureMng					*m_texMng;
	CFileMng						*m_fileMng;
	CScript							*m_script;
	LPDIRECT3DDEVICE8				m_pd3dDevice;

	
	CIFControl						*m_jystechLogo;
	CIFControl						*m_malLogo;
	BYTE							m_bLogoStep;
	
	
	
	
	CIFControl						*m_startLogo;
	CIFControl						*m_loadingLogo;

	
	CIFMsgWin						*m_msgWin;
	CIFLoginWin						*m_loginWin;
	CIFNameWin						*m_nameWin;
	CIFCharWin						*m_charWin;
	CIFItemWin						*m_itemWin;
	
	CIFMenuWin						*m_menuWin;
	CIFChatWin						*m_chatWin;
	CIFStoreWin						*m_storeWin;
	CIFRolloverWin					*m_rolloverWin;

	CIFSkillWin						*m_skillWin;
	CIFQuestWin						*m_questWin;
	CIFOptionWin					*m_optionWin;
	CIFTradeWin						*m_tradeWin;
	CIFStackWin						*m_stackWin;
	CIFDlgWin						*m_dlgWin;
	CIFChatOptionWin				*m_chatOptionWin;
	CIFWorldChatWin					*m_worldChatWin;
	CIFLearnWin						*m_learnWin;
	CIFDepotWin						*m_depotWin;
	CIFHotWin						*m_hotWin;
	CIFHelpWin						*m_helpWin;
	CIFPartyWin						*m_partyWin;
	CIFGmWin						*m_gmWin;
	CIFPrecocityWin					*m_precocityWin;
	CIFCraftWin						*m_craftWin;
	
	
	CIFEventCraftWin				*m_EventCraftWin;

	CIFUpgradeWin					*m_upgradeWin;
	CIFMapWin						*m_mapWin;
	CIFMiniMapWin					*m_miniMapWin;
	CIFBoothWin						*m_boothWin;
	CIFTutorialWin					*m_tutorialWin;
	CIFTipWin						*m_tipWin;
	CIFSettingWin					*m_settingWin;
	CIFCraftHelpWin					*m_craftHelpWin;
	
	CIFCommunityWin					*m_communityWin;
	CIFMailWin						*m_mailWin;
	CIFMailEditWin					*m_mailEditWin;
	CIFFriendWin					*m_friendWin;
	
	CIFQuestScrollWin				*m_questscrollWin;
	
	CIFGuildWin						*m_guildWin;
	CIFGuildAuthWin					*m_guildAuthWin;

	
	CIFGuildPvPWin					*m_guildPvpWin;
	CIFGuildArenaWin				*m_guildArenaWin;
	
	CIFGuild_battle_info_win		*m_guildArenaInfoWin;
	
	

	
	CIFAutoQuestWin                 *m_autoQuestWin;
	CIFNpcInvWin					*m_npcInvWin;
	

	
	CIFSurgeryWin					*m_surgeryWin;
	

	
	CIFBattleTicketWin				*m_battleTicketWin;
	CIFShareWin						*m_shareWin;
	CIFTournamentWin				*m_tournamentWin;
	CIFPrizeWin						*m_prizeWin;
	CIFGolryunInfoWin				*m_golryunInfoWin;
	CIFObserverMenuWin				*m_observerMenuWin;
	CIFObserverInfoWin				*m_observerInfoWin;
	CIFTournamentMiniWin			*m_tournamentMiniWin;
	

	
	CIFIconCtl						*m_iconCtl;
	
	CIFMouseCursor					*m_cursor;
	
	CIFNumCtl						*m_numCtl;
	
	
	CIFWindow						*m_winControl[NUM_IF_WINDOW_TYPE];
	
	
	CIFPMManager					*m_guildMarkMng;

	
	CIFGonryunBattlePractice		*m_pgonryunBattlePractice;
	CIFBattingWindow				*m_pgonryunBattleBattingwindow;
	CIFGonryunBattleProgressInfo	*m_pGonryunBattleProgressInfo;
	
	
	char							m_tempString[CHATSTRING];


	
	CIFPostWin						*m_PostWin;
	CIFMailBoxWin					*m_mailBoxWin;

	
	CIFWorld_ChangeWin				*m_WorldChangeWin;
	
		

	CIFHelperWin						*m_HelperWin;


	
	CIFGiftCardWin					*m_GiftCardWin;

	
	CIFServer_ListWin				*m_ServerListWin;
	

	
	CIFCashMallItem					*m_CashMallItem;

	
	CItem_ScrollWin					*m_Item_ScrollWin;
	
	
	CIFDanWarRequestWin				*m_DanWarRequestWin;

	
	CIFDanWarBattleWin				*m_DanWarBattleWin;
	

	
	BOOL							m_Visible;
	BOOL							m_VisibleName;
	

	
	CIFManager();
	virtual ~CIFManager();

	
	int					LoadConfig( char *filename );

	int					GetInterfacePositionID( char* posID );
	void				GetInterfacePositionWidth( int id, int &x, int &size );
	void				GetInterfacePositionHeight( int id, int &y, int &size );
	int					GetInterfaceParentPositionID( char* id );

	
	void				SetTexManager	( CD3DTextureMng *texMng );
	void				SetFileManager	( CFileMng *fileMng	);
	void				SetScript		( CScript *script );
	void				SetDevice		( LPDIRECT3DDEVICE8 device );
	void				SetRenderState();


	void				UpdateTextureID( int group );
	void				AddTextureToManager( int group );
	
	void				FinalCleanup();
	void				InitDeviceObjects();						
	void				InvalidateDeviceObjects();					
	void				RestoreDeviceObjects();						
	
	void				UpdateAllBuffer();
	void				PrepareAllControl( int width, int height );
	void				UpdateAllControl();
	void				ChangeResolution( int in_width,	int	in_height );

	int					ProcessInterface();
	void				PrepareRender();
	void				Render();
	

	int					ProcessSelection();
	void				ProcessCursor();

	void				RestartInterface( int type );
	int					CheckConditionalWindow();
	char*				GetNumberString( int number );
	char*				GetTimeString( int sec );
	
	void				InitSoundVolume();

	
	void				InitRolloverText();
	void				AddRolloverText( char *str, int colorIdx = -1, int lineFeed = true );
	void				MakeRolloverText( int x, int y, int forcedWinFlag = true, int sx = -1, int sy = -1, int arrangeType = CHAT_ARRANGE_CENTER );

	void				MakeItemRollover( item_t *item, int numFlag = false );
	void				MakeActionRollover( int idx );

	void				MakeSkillRollover( int idx, int tableFlag );
	void				MakeSkillRollover_GetValue(int _MySkillIndex,int skillTableIdx,SKILL_GEN_LEVEL &skillLevel);

	void				MakeHotRollover( int idx );
	void				MakeItemOptionRender( item_t *item );
	char*				GetItemRequireJobText( itemTable_t *pItem );
	char*				GetItemRequireStatText( itemTable_t *pItem );
	char*				GetItemAvailableOnText( int optionIdx );


	
	void				AddFocus( CIFWindow *pCtl );
	int					SetFocus( CIFWindow *pCtl );
	CIFWindow*			GetFocus() { return( m_winControl[0] ); }

	int					ProcessIFKeyState();

	
	
	void		SetVAMessage(char* title, LPCTSTR lpszFormat, ...);

	bool		SetMessage( char* title, char* msg, int button = MSG_BUTTONTYPE_OK, int type = IF_MESSAGE_NONE, float limitTime = -1.0f );
	int			MessageProcessOk();
	int			MessageProcessCancel();
	void		ExitMessage();

	
	void		AddSysVAMessage( DWORD color , LPCTSTR lpszFormat, ...);
	
	void		AddSysMessage( char* message, DWORD color = GTHCOLOR_SYSMESSAGE );
	void		AddNoticeMessage( char* message, DWORD color = GTHCOLOR_NOTICEMESSAGE );
	void		AddChatMessage( char* message, DWORD color, char* nameMsg, int type = IF_CHATMSG_TYPE_SYSTEM );

	
	
	

	
	void		LogoProcess();
	void		LogoRender();
	


	void RenderChar(void);
	
	BOOL RenderChar_SetChrNameColor(Fx_CHARACTER_t* character);
	void RenderChar_SetChrName(char* pName,const Fx_CHARACTER_t* character);
	void RenderChar_booth(Fx_CHARACTER_t* character,const vec3_t pos);
	void RenderChar_Notice(void);
	void RenderChar_Guild(Fx_CHARACTER_t *character,int maxsize,const vec3_t pos,const float zPos,const float wPos);
	void RenderChar_render_select_sphere(Fx_CHARACTER_t* character,const int x,const int y,const int maxsize,const float zPos,const float wPos);	
	void RenderChar_Bubble(Fx_CHARACTER_t *character,const vec3_t pos);	
	BOOL RenderChar_isOutputGuild(Fx_CHARACTER_t*  character);
	BOOL RenderChar_isCheatEnable(void);

	
	void RenderChar_DanBattleEffect(Fx_CHARACTER_t* character);
	
};

#endif 
