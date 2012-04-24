



#include "../global_def.h"
#include "IFCommon.h"


#include "../CTools.h"
extern CTools gcTools;





CItem_ScrollWin::CItem_ScrollWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_ITEM_SCROLLWIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_bgMessage = new CIFControl( this );

	m_OkBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	Enable( false ,NULL,-1,0);

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_ITEM_SCROLL_WINDOW_TYPE );

	Init();
}

CItem_ScrollWin::~CItem_ScrollWin()
{

}


void CItem_ScrollWin::Init()
{
	m_InventoryItemPos=-1;
	m_RequestPlayerName[0]=NULL;
	m_Title[0]=NULL;
	m_ItemName[0]=NULL;
	m_nWindowType = enumWINDOW_TYPE_ENUM::WT_NORMAL;
	m_SendTime=0;

	m_bSending=false;
}

void CItem_ScrollWin::Decoder()
{
	short PacketID =MSG_ReadShort();
	switch(PacketID){
	case enumPacketID::GSC_ERROR_MES:
		GSC_Error_Mes();
		break;  
		
	case enumPacketID::GSC_STATE_MES:
		GSC_State_Mes();
		break;  
		
	case enumPacketID::GSC_REQUEST_PC_SUMMON:
		GSC_PC_Summon_Mes();
		break;  

	case enumPacketID::GSC_REQUEST_PC_MOVE_SUMMON:
		GSC_PC_Move_Summon_Mes();
		break;

	case enumPacketID::GSC_REPLY_REDAY_PC_MOVE_SUMMON:
		Gsc_Reply_Ready_Pc_Move_Summon();
		break;

	}
	return;
}



void CItem_ScrollWin::GSC_Error_Mes()
{
	short ErrorCode =MSG_ReadShort();

	char	RequesterName[NAMESTRING+1];
	char	TargetterName[NAMESTRING+1];

	sstrncpy( RequesterName, MSG_ReadString(), NAMESTRING);
	RequesterName[NAMESTRING]=NULL;

	sstrncpy( TargetterName, MSG_ReadString(), NAMESTRING);
	TargetterName[NAMESTRING]=NULL;

	SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE);
	Enable( false ,NULL,-1,0);
	Init();

	

	switch(ErrorCode)
	{
	case ERROR_NULL_TARGET_PLAYER:
		g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 559),RequesterName,TargetterName); 
		break;

	case ERROR_ITEM_NOT_HAVE:
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 560));
		break;

	case ERROR_INVALID_ITEM:
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 561));
		break;

	case ERROR_NOT_MOVE_SUMMON_ITEM:
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 562));
		break;

	case ERROR_NOT_SUMMON_ITEM:
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 563));
		break;

	case ERROR_NULL_REQUEST_PLAYER:
		g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 564),TargetterName,RequesterName);
		break;

	case ERROR_TARGET_PLAYER_NOT_USE_AREA:
		g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 565), TargetterName);
		break;

	case ERROR_REQUEST_PLAYER_NOT_USE_AREA:
		g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 565),RequesterName );
		break;

	case ERROR_NOT_USE_ME_TO_ME:
		g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 566) );
		break;
	}
	return;
}




void CItem_ScrollWin::GSC_State_Mes()
{
	short StateCode =MSG_ReadShort();

	char	RequesterName[NAMESTRING+1];
	char	TargetterName[NAMESTRING+1];


	SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE);
	Enable( false ,NULL,-1,0);
	Init();

	switch(StateCode)
	{
	case STATE_MSGBOX_POPUP_SUCC:
	case STATE_MSGBOX_POPUP_FAIL:
		{
			int invenPos =MSG_ReadLong();

			sstrncpy( RequesterName, MSG_ReadString(), NAMESTRING);
			RequesterName[NAMESTRING]=NULL;

			sstrncpy( TargetterName, MSG_ReadString(), NAMESTRING);
			TargetterName[NAMESTRING]=NULL;

			if(STATE_MSGBOX_POPUP_SUCC == StateCode)
			{
				
				if(true == IfWorldChangEffecting())		
				{
					g_ifMng->AddSysMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 586) ); 
					return;
				}				

				g_ifMng->AddSysVAMessage( D3DCOLOR_ARGB(255, 255, 125, 100),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 567),TargetterName);	
				gcTools.DeleteInventoryItem(invenPos);
			}
			else
			{
				g_ifMng->SetVAMessage(g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 568),TargetterName);	
			}
		}
		break;

	case STATE_TARGET_PLAYER_REFUSE:
	case STATE_TARGET_PLAYER_OK:
		{
			sstrncpy( RequesterName, MSG_ReadString(), NAMESTRING);
			RequesterName[NAMESTRING]=NULL;

			sstrncpy( TargetterName, MSG_ReadString(), NAMESTRING  );
			TargetterName[NAMESTRING]=NULL;

			if(STATE_TARGET_PLAYER_OK == StateCode)
			{
				g_ifMng->AddSysVAMessage( D3DCOLOR_ARGB(255, 255, 125, 100),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 569), TargetterName);
			}
			else
			{
				g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 570) , TargetterName);
			}
		}
		break;

	case STATE_TARGET_PLAYER_MOVE_REFUSE:
	case STATE_TARGET_PLAYER_MOVE_OK:
		{
			m_TargetPlayerWorldIdx    = MSG_ReadByte();
			MSG_ReadPosition(m_TargetPlayerPosition);

			sstrncpy( RequesterName, MSG_ReadString(), NAMESTRING);
			RequesterName[NAMESTRING]=NULL;

			sstrncpy( m_TargetPlayerName, MSG_ReadString(), NAMESTRING  );
			m_TargetPlayerName[NAMESTRING]=NULL;

			bool ResultAllow = IsAllowWorldChange();
			if(false == ResultAllow)
			{
				g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 571) ,RequesterName);

				
				gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_ERROR_MES,ERROR_REQUEST_PLAYER_NOT_USE_AREA,RequesterName,m_TargetPlayerName);
				return;
			}

			if(STATE_TARGET_PLAYER_MOVE_OK == StateCode)
			{				
				
				if(true == IfWorldChangEffecting())		
				{
					g_ifMng->AddSysMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 586) );
					return;
				}

				g_ifMng->AddSysVAMessage( D3DCOLOR_ARGB(255, 255, 125, 100),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 572), m_TargetPlayerName);
				PC_Move_Summon_Proc();
			}
			else
			{
				g_ifMng->SetVAMessage( g_LPACK.GetMessage(0, 322), g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 573), m_TargetPlayerName);
			}
		}
		break;
	}
	return;
}



void CItem_ScrollWin::GSC_PC_Summon_Mes()
{
	short subType = MSG_ReadShort();
	int InventoryItemPos = MSG_ReadLong();

	m_TargetPlayerWorldIdx    = MSG_ReadByte();
	MSG_ReadPosition(m_TargetPlayerPosition);

	sstrncpy( m_RequestPlayerName, MSG_ReadString(), NAMESTRING );
	m_RequestPlayerName[NAMESTRING]=NULL;

	
	int resultAllow = IsAllowWorldChange();
	if(false == resultAllow )	
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_FAIL,InventoryItemPos,m_RequestPlayerName);
		return;
	}

	
	if(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE != g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode)
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_FAIL,InventoryItemPos,m_RequestPlayerName);
		return;
	}

	char szTemp[256];
	wsprintf(szTemp, g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 574),m_RequestPlayerName);

	bool result = g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, 
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_REQUEST_PC_SUMMON, 10000.0f );

	
	if(false == result)
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_FAIL,InventoryItemPos,m_RequestPlayerName);
	}
	
	else
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_SUCC,InventoryItemPos,m_RequestPlayerName);
	}

	return;
}

void CItem_ScrollWin::Select_PC_Summon(int _flag)
{
	
	if(enumTargetPlayerReply::REPLY_OK == _flag)
	{
		
		if(true == IfWorldChangEffecting())		
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 586) );
			return;
		}

		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
			WorldChangeSystem::WORLD_CHANGE_EFFECT_IDX,
								g_pApp->m_myCharacter->position,
								NULL, 
								true, 
								g_pApp->m_myCharacter->entityType, 
								g_pApp->m_myCharacter->idxOnServer,
								0.0f, 
								0.0f, 
								0.0f, 
								-1.0f,
								false,
								effectentity_t::enumEffectType::EFFECT_TYPE_PC_SUMMON);
		
		SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_ITEM_MODE);
	}
	else
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REPLY_PC_SUMMON,enumTargetPlayerReply::REPLY_REFUSE,m_RequestPlayerName);
	}
	return;
}


void CItem_ScrollWin::Send_Reply_PC_Summon(int _flag)
{
	g_ifMng->RestartInterface( 1 );
	gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REPLY_PC_SUMMON,enumTargetPlayerReply::REPLY_OK,m_TargetPlayerWorldIdx,m_TargetPlayerPosition,m_RequestPlayerName);
	return;
}







void CItem_ScrollWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_SCROLL_WIN );

	m_textCtl[IT_ITEM_SCROLLWIN_TITLE]->PrepareControl( IF_SCROLL_WIN_TITLE );
	m_textCtl[IT_ITEM_SCROLLWIN_ITEM_NAME]->PrepareControl( IF_SCROLL_WIN_ITEM_NAME );


	m_OkBtn->PrepareControl( IF_SCROLL_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_SCROLL_WIN_CANCEL_BTN_0 );

	
	m_bgMessage->PrepareControl( IF_SCROLL_WIN_MESSAGE );

	int cx, cy;
	cx = m_bgMessage->GetXPos();
	cy = m_bgMessage->GetYPos();

	g_chatManager->SetInput( cx, cy, NAMESTRING  , NAMESTRING, INPUT_INDEX_ITEM_SCROLL_WINDOW_ID, 1, 130, true );

}

void CItem_ScrollWin::Enable(int state,const item_t *_item,int _InventoryItemPos,int _type)
{ 
	
	if(1 == state)
	{
		if(true == IfWorldChangEffecting())		
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 586) );
			return;
		}
	}

	CIFWindow::Enable(state);

	m_InventoryItemPos	= _InventoryItemPos;
	m_nWindowType		= _type;

	m_bSending = false;
	m_SendTime = 0;

	if(NULL != _item )
	{
		
		strncpy(m_ItemName,_item->name,MAXBYTES_ITEM_NAME);
		m_Title[MAXBYTES_ITEM_NAME]=NULL;
	}

	if(m_nWindowType == WT_PC_SUMMON)
	{
		strncpy(m_Title,g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 575),MAXBYTES_TITLE);
		m_Title[MAXBYTES_TITLE]=NULL;
	}
	else if(m_nWindowType == WT_PC_MOVE_SUMMON)
	{
		strncpy(m_Title,g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 576),MAXBYTES_TITLE);
		m_Title[MAXBYTES_TITLE]=NULL;
	}

	if(1 == state)
	{
		g_chatManager->SetInputState( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID, 1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID );
	}

	return;
}

int CItem_ScrollWin::WindowFrame()
{
	m_textCtl[IT_ITEM_SCROLLWIN_TITLE]->SetText(m_Title);
	m_textCtl[IT_ITEM_SCROLLWIN_ITEM_NAME]->SetText(m_ItemName);

	if(m_nWindowType == WT_PC_SUMMON)
	{
		m_OkBtn->SetButtonText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 577),D3DCOLOR_ARGB(155,253, 237, 193));
		m_cancelBtn->SetButtonText(g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 578) ,D3DCOLOR_ARGB(155,253, 237, 193) );
	}
	else if(m_nWindowType == WT_PC_MOVE_SUMMON)
	{
		m_OkBtn->SetButtonText(  g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 579),D3DCOLOR_ARGB(155,253, 237, 193));
		m_cancelBtn->SetButtonText( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 580),D3DCOLOR_ARGB(155,253, 237, 193) );
	}

	g_chatManager->m_ix[INPUT_INDEX_ITEM_SCROLL_WINDOW_ID] = m_bgMessage->GetXPos();
	g_chatManager->m_iy[INPUT_INDEX_ITEM_SCROLL_WINDOW_ID] = m_bgMessage->GetYPos();

	
	SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_CHECK);

	return( IF_CTL_NONE );
}

void CItem_ScrollWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_OkBtn->RenderTextBtn();
	m_cancelBtn->RenderTextBtn();

	m_bgMessage->Render();
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_ITEM_SCROLLWIN_TITLE]->SetColor(D3DCOLOR_ARGB(155,253, 237, 193));
		m_textCtl[IT_ITEM_SCROLLWIN_TITLE]->Render(IF_TEXT_OUTPUT_CENTER);

		m_textCtl[IT_ITEM_SCROLLWIN_ITEM_NAME]->SetColor(D3DCOLOR_ARGB(155,253, 237, 193));
		m_textCtl[IT_ITEM_SCROLLWIN_ITEM_NAME]->Render(IF_TEXT_OUTPUT_CENTER);

		g_chatManager->RenderInputFrame( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID );
	}
	g_chatManager->EndFrame();


	
	if(true == IfWorldChangEffecting())
	{
		g_ifMng->m_Item_ScrollWin->Select_PC_Summon(CItem_ScrollWin::enumTargetPlayerReply::REPLY_REFUSE);
		Enable( false ,NULL,-1,0);
		Init();
		return;
	}

	
	if(true == m_bSending)
	{
		if(timeGetTime() -  m_SendTime > 20000)
		{
			g_ifMng->AddSysMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 587) );

			Enable( false ,NULL,-1,0);
			Init();

			
			if(true == IfWorldChangEffecting())		
			{				
				return;
			}

			SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE);
		}
	}
}


int CItem_ScrollWin::WindowSelection()
{
	int ret;

	ret = Selection();
	if( ret == IF_LBDN ) 
	{
		g_chatManager->SetInputState( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID, 1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID );
		return( IF_CTL_SELECT );
	}

	return IF_CTL_NONE;
}


int CItem_ScrollWin::WindowProcess()
{
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_chatManager->InitInputStr( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID );
		g_chatManager->SetInputState( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		Enable( false ,NULL,-1,0);
		Init();
		return IF_CTL_NONE;
	}

	if( m_OkBtn->ProcessButton() == IF_CTL_SELECT )
	{
		char msg[CHATSTRING];
		strcpy( msg, g_chatManager->GetInputStr( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID ) );

		g_chatManager->InitInputStr( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID );
		g_chatManager->SetInputState( INPUT_INDEX_ITEM_SCROLL_WINDOW_ID, -1 );
		g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

		
		if(true == IfWorldChangEffecting())		
		{
			g_ifMng->AddSysMessage(g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 587));
			return IF_CTL_SELECT;
		}

		if(m_nWindowType == WT_PC_SUMMON)
		{
			
			gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REQUEST_PC_SUMMON,m_InventoryItemPos,msg);

			
			SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_ITEM_MODE);

			m_SendTime= timeGetTime();
			m_bSending=true;
		}
		else if(m_nWindowType == WT_PC_MOVE_SUMMON)
		{
			int resultAllow = IsAllowWorldChange();
			if(false == resultAllow) 
			{
				g_ifMng->SetMessage( g_LPACK.GetMessage(0, 322),g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 581) );
				return IF_CTL_NONE;
			}

			
			gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REQUEST_PC_MOVE_SUMMON,m_InventoryItemPos,msg);

			
			SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_ITEM_MODE);

			m_SendTime= timeGetTime();
			m_bSending=true;
		}



		return IF_CTL_NONE;
	}
	
	return IF_CTL_NONE;
}









void CItem_ScrollWin::GSC_PC_Move_Summon_Mes()
{
	
	if(true == IfWorldChangEffecting())		
	{
		g_ifMng->AddSysMessage( g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 586) );
		return;
	}

	short subType = MSG_ReadShort();
	int InventoryItemPos = MSG_ReadLong();

	sstrncpy( m_RequestPlayerName, MSG_ReadString(), NAMESTRING );
	m_RequestPlayerName[NAMESTRING]=NULL;

	char szTemp[256];
	wsprintf(szTemp, g_LPACK.GetMessage(LPACK_TYPE_NORMAL2, 582),m_RequestPlayerName);

	
	if(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_NONE != g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode)
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_FAIL,InventoryItemPos,m_RequestPlayerName);
		return;
	}

	bool result = g_ifMng->SetMessage( g_LPACK.GetMessage(0,322), szTemp, 
		MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_REQUEST_PC_MOVE_SUMMON, 10000.0f );

	
	if(false == result)
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_FAIL,InventoryItemPos,m_RequestPlayerName);
	}
	
	else
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_STATE_MES,STATE_MSGBOX_POPUP_SUCC,InventoryItemPos,m_RequestPlayerName);
	}

	return;
}

void CItem_ScrollWin::Select_PC_Move_Summon(int _flag)
{
	
	if(true == IfWorldChangEffecting())
	{
		gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REPLY_PC_MOVE_SUMMON,CItem_ScrollWin::enumTargetPlayerReply::REPLY_REFUSE,m_RequestPlayerName);
		return ;
	}

	gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REPLY_PC_MOVE_SUMMON,_flag,m_RequestPlayerName);
	return;
}


void CItem_ScrollWin::Send_Reday_PC_Move_Summon(int _flag)
{
	
	gcTools.CcToGs(ITEM_SUMMON_SYSTEM,CC_REDAY_PC_MOVE_SUMMON,_flag,m_TargetPlayerWorldIdx,m_TargetPlayerPosition,m_TargetPlayerName);
}

void CItem_ScrollWin::Gsc_Reply_Ready_Pc_Move_Summon()
{
	g_ifMng->RestartInterface( 1 );
	GTH_SendMessage_ReplySummons(1);
	return;
}

void CItem_ScrollWin::PC_Move_Summon_Proc()
{
	
	g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, 
		WorldChangeSystem::WORLD_CHANGE_EFFECT_IDX,
							g_pApp->m_myCharacter->position,
							NULL, 
							true, 
							g_pApp->m_myCharacter->entityType, 
							g_pApp->m_myCharacter->idxOnServer,
							0.0f, 
							0.0f, 
							0.0f, 
							-1.0f,
							false,
							effectentity_t::enumEffectType::EFFECT_TYPE_PC_MOVE_SUMMON);
	
	SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_ITEM_MODE);
	return;
}

bool CItem_ScrollWin::IsAllowWorldChange()
{
	
	int resultAllow = IsAllowWorldChange_ForPcEvent();
	if(1 != resultAllow )	
	{		
		return false;
	}

	
	if( g_ifMng->m_boothWin->State() ) 
	{
		return false;
	}

	return true;
}



bool CItem_ScrollWin::IfWorldChangEffecting()
{
	
	if(WorldChangeSystem::enumWorldChangeSystemMode::WORLD_CHANGESYSTEM_EFFECT_MODE == g_cgv.myCharacterInfo->m_WorldChageSystem.m_SystemMode)
	{		
		return true;
	}

	if(1 == g_ifMng->m_changeStateFlag)
	{		
		return true;
	}

	if( g_cgv.state != CLIENT_GAME_PROCESS)
	{		
		return true;
	}


	return false;
}

bool CItem_ScrollWin::IfWorldChangeing()
{
	if(true == IfWorldChangEffecting())
	{
		return true;
	}

	if(1 == CIFWindow::State())
	{
		return true;
	}

	return false;
}