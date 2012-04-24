




#include "../global_def.h"





NewDanBattleSystem::NewDanBattleSystem()
{	
	m_bActive			 = TRUE;
	m_nMinPcCount		 = 0;	
	m_nBattleTimeForMin1 = 0;	
	m_nBattleTimeForMin2 = 0;	
	m_fItemDropRate		 = 1.0f;
	m_EffectFlag		 = 0;	


	for (INT typeidx =0; typeidx  < noticetype::max_notice; typeidx++)
	memset(&m_notice[typeidx], 0x00, sizeof(sNoticeMsg));

}

void NewDanBattleSystem::InitDanBattleInfo()
{	
	m_nMinPcCount		 = 0;	
	m_nBattleTimeForMin1 = 0;	
	m_nBattleTimeForMin2 = 0;	
	m_fItemDropRate		 = 1.0f;
	m_EffectFlag		 = 0;

	memset(&m_DanBattleInfo, 0x00, sizeof(DanBattleInfo));
	m_DanBattleInfo.AcceptDanInfo.nDanIdx	= -1;	
	m_DanBattleInfo.RequestDanInfo.nDanIdx	= -1;
	m_DanBattleInfo.nDanBattleIdx = -1;
	m_DanBattleInfo.nBattleMapIdx = -1;



	
	g_NewDanBattleSystem.v_EffectPC.clear();

}

NewDanBattleSystem::~NewDanBattleSystem()
{

}







void NewDanBattleSystem::Update()
{	
	switch ( m_DanBattleInfo.BattleStatus )
	{

	case DanBattleInfo::enumBattleStatus::BATTLESTATUS_NONE: 
		break;
		
	case DanBattleInfo::enumBattleStatus::BATTLESTATUS_READY:
		BattleReady();
		break;
		
	case DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE:
		Battle();
		break;
		
	case DanBattleInfo::enumBattleStatus::BATTLESTATUS_END:
		BattleEnd();
		break;
	}			

}







void NewDanBattleSystem::BattleReady()
{
	
	if ( m_DanBattleInfo.fWaitTImeForReady > 0.0f ){
		m_DanBattleInfo.fWaitTImeForReady -= g_timer.GetElapsedMilliTime();
	}

	
	ShoeEffectCount(m_DanBattleInfo.fWaitTImeForReady);
	
}






void NewDanBattleSystem::Battle()
{

	
	if ( m_DanBattleInfo.fRemainSec > 0.0f ){
		m_DanBattleInfo.fRemainSec -= g_timer.GetElapsedMilliTime();
	}

	
		ShoeEffectCount(m_DanBattleInfo.fRemainSec);
	

}







void NewDanBattleSystem::BattleEnd()
{

}






NewDanBattleSystem::DanBaseInfo* NewDanBattleSystem::GetMyDanBattleInfo()
{	
	if ( m_DanBattleInfo.RequestDanInfo.nDanIdx == g_cgv.myCharacterInfo->guildIdx)
		return &(m_DanBattleInfo.RequestDanInfo);
	else	
		return &(m_DanBattleInfo.AcceptDanInfo);

}






NewDanBattleSystem::DanBaseInfo* NewDanBattleSystem::GetDestDanBattleInfo()
{
	if ( m_DanBattleInfo.RequestDanInfo.nDanIdx != g_cgv.myCharacterInfo->guildIdx)
		return &(m_DanBattleInfo.RequestDanInfo);
	else
		return &(m_DanBattleInfo.AcceptDanInfo);
}







NewDanBattleSystem::DanBaseInfo* NewDanBattleSystem::GetRequestDanBattleInfo()
{
	return &(m_DanBattleInfo.RequestDanInfo);

}






NewDanBattleSystem::DanBaseInfo* NewDanBattleSystem::GetAcceptDanBattleInfo()
{
	return &(m_DanBattleInfo.AcceptDanInfo);
}








NewDanBattleSystem::DanBattleInfo* 	NewDanBattleSystem::GetDanBattleInfo()
{
	return &m_DanBattleInfo;
}




VOID NewDanBattleSystem::ShoeEffectCount(INT RemainTime)
{
	UINT RemainMin	= RemainTime/60000;
	UINT RemainnSec = (RemainTime%60000) / 1000;

	ShowLogInfo("RemainMin = %d, RemainSec = %d, danflag=%d", 
		RemainMin, RemainnSec, m_DanBattleInfo.BattleStatus);
	
	UINT MaxCheckCount = 0;
	UINT MinCheckCount = 0;
	int NowEffectID = 0;
	if ( m_DanBattleInfo.BattleStatus == DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE){
		MaxCheckCount = 10;
		MinCheckCount = 0;
		NowEffectID = 64-(RemainnSec);
	}
	else{
		MaxCheckCount = 11;
		MinCheckCount = 1;
		NowEffectID = 64-(RemainnSec-1);
	}	
	


	
	
	
	if ( (RemainnSec >= MinCheckCount && RemainnSec <= MaxCheckCount) && RemainMin == 0 )		
	{		
		if ( m_EffectFlag & (1 << NowEffectID) )
			return ;		
		
		char szMessage[256];
		if (m_DanBattleInfo.BattleStatus == DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		{
			if (NowEffectID == 64)
				return;	

			
			_snprintf(szMessage, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 596), RemainnSec);					
			szMessage[256-1] = NULL;
			g_ifMng->AddSysMessage(szMessage, GTHCOLOR_WORLDMESSAGE);
			
			_snprintf(szMessage, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 597), RemainnSec);		
			szMessage[256-1] = NULL;

			
			AddDanBattleNoticeMsg(szMessage, GTHCOLOR_ERRORMESSAGE);			
			
		}
		else
		{			
			if (NowEffectID == 64)
			{
				g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( 44 ), GTH_SOUND_PLAY_ONCE );
				m_EffectFlag |= 1 << NowEffectID;
				return;	
			}
			
			
			_snprintf(szMessage, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2, 598), RemainnSec -1);		
			szMessage[256-1] = NULL;
			g_ifMng->AddSysMessage(szMessage, GTHCOLOR_WORLDMESSAGE);
			
			
		}
			
			
			
			g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, (NowEffectID), 
				g_pApp->m_myCharacter->position,
				NULL, 	true, 
				g_pApp->m_myCharacter->entityType, 
				g_pApp->m_myCharacter->idxOnServer,
				0.0f, 0.0f, 0.0f, -1.0f,
				false );	
			
			
			
			
			
			
			
			
			if ( NowEffectID == 54)
				g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( 45 ), GTH_SOUND_PLAY_ONCE );
			
			
			
			m_EffectFlag |= 1 << NowEffectID;
			ShowLogInfo("RemainMin = %d, RemainSec = %d", RemainMin, RemainnSec);
		}

		

}
void NewDanBattleSystem::SetDanBattleStatus(DanBattleInfo::enumBattleStatus status)
{
	m_DanBattleInfo.BattleStatus = status;
}







BOOL NewDanBattleSystem::IsDanBattleEnemy(int pcidx)
{

	if ( (g_pApp->m_myCharacter->worldIdx == 0) ||
		(g_pApp->m_myCharacter->worldIdx == 9) )
		return FALSE;

	if ( g_cgv.syncCharacter[0].worldIdx != m_DanBattleInfo.nBattleMapIdx )
		return FALSE;

	if ( pcidx < 0 || pcidx >= MAX_CHARACTERS)
		return FALSE;

	if ( m_DanBattleInfo.BattleStatus != DanBattleInfo::enumBattleStatus::BATTLESTATUS_BATTLE)
		return FALSE;

	if ( g_cgv.syncCharacter[pcidx].nDanBattleIdx < 0 )
		return FALSE;

	if ( g_cgv.syncCharacter[pcidx].entityType != ENTITY_PC)
		return FALSE;

	if ( g_cgv.syncCharacter[pcidx].nDanBattleIdx != 
		g_cgv.syncCharacter[0].nDanBattleIdx)
		return FALSE;

	
	if ( g_cgv.syncCharacter[pcidx].guildIdx == g_cgv.syncCharacter[0].guildIdx)
		return FALSE;

	
	return TRUE;
}


void NewDanBattleSystem::AddDanBattleNoticeMsg( char* message, DWORD color, noticetype type)
{
	
	if ( type <0 || type >= noticetype::max_notice)
		return;
	
	memset(&m_notice[type], 0x00, sizeof(sNoticeMsg));
	
	m_notice[type].m_noticeColor = color;
	m_notice[type].m_noticeTime = g_timer.GetAppMilliTime();

	strncpy(m_notice[type].m_noticeMsg, message, 256);
	m_notice[type].m_noticeMsg[255] = NULL;	

}

void NewDanBattleSystem::RenderDanBattle_Notice(void)
{
	
	
	g_chatManager->SetZPos();
	g_chatManager->SetWPos();


	

	for (int nNoticeidx = 0; nNoticeidx <max_notice; nNoticeidx++)
	{
		if( m_notice[nNoticeidx].m_noticeTime + 8000.0f > g_timer.GetAppMilliTime() && 
			m_notice[nNoticeidx].m_noticeMsg[0] != 0 )
		{
			int x =0, y = 0;
			int size = 0;			
			switch(nNoticeidx)
			{
			case noticetype::notice:
				{				
					y = 55;		
					size = g_chatManager->GetChatStrSizeChat( m_notice[nNoticeidx].m_noticeMsg );
					x = g_ifMng->m_clientWidth / 2 - size / 2;
				}
				break;
			case noticetype::killpoint_notice:
				{
					y = g_ifMng->m_iPos[IF_HOT_WIN].clientY - g_ifMng->m_iPos[IF_HOT_WIN].sizeY - 20;
					x = 5;
				}
				break;
			case noticetype::plunder_notice:
				{
					
					y = g_ifMng->m_iPos[IF_HOT_WIN].clientY - g_ifMng->m_iPos[IF_HOT_WIN].sizeY;
					x = 5;
				}
				break;
			}
			
			g_chatManager->SetColor( m_notice[nNoticeidx].m_noticeColor );						
			g_chatManager->Output( x, y + 17, m_notice[nNoticeidx].m_noticeMsg, true );
		}
	}
}
