





#include "../global.h"
#include "SkillApp.h"



CSkillProc g_SkillProc;






CSkillProc::CSkillProc()
{

}

CSkillProc::~CSkillProc()
{
}

bool CSkillProc::Active(void *pPc,CStr_Skill_Msg *msg)
{
	playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);

	messageInfo_t	msginfo;
	message_t		message;

	memset( &msginfo, 0, sizeof( messageInfo_t ) );
	memset( &message, 0, sizeof( message ) );

	
	msg->message = &message;
	msg->msginfo = &msginfo;
	msg->pPc	 = pPc;

	
	
	pc->generalSpeed = 100;
	
	
	

	
	bool bCheck = g_SkillDataCheckProc.CheckSkillProc(pPc,msg);
	if(!bCheck) return false;

	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(msg->nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;

	
	msg->pDataFormat = pDataFormat;

	CSkillDataParty * pParty = pDataFormat->GetParty();
	if(pParty == NULL) return false;

	
	message.data[MSG_SKILL_TABLE_INDEX] = msg->nSkillID;

	
	if(pParty->m_Type)
	{
		g_SkillPartyProc.Proc(pPc,msg);
	}
	else
	{
		
		CastActive(pPc,msg);

		Proc(pPc,msg);
	}

	
	g_SkillDataSubProc.SubActionProc(pPc,msg);	

	return true;
}

int CSkillProc::Proc(void *pPc,CStr_Skill_Msg *msg)
{
	CSkillDataFormat *pDataFormat = msg->pDataFormat;

	
	if(pDataFormat->m_MessageType ==SKILL_MSG_TYPE_ONLYSEND)
	{	
		
		MessageActive(pPc,msg);	
		
		AI_SendMessage( msg->message );
	}
	
	else if(pDataFormat->m_MessageType)
	{
		
		BasicActive(pPc,msg);

		
		
		if(msg->pcIdx > -1)
		{
			
			if( pDataFormat->m_ClassType != SKILL_CLASS_PC)		return 0;
		}
		else if(msg->monsterIdx > -1)
		{
			
			if( pDataFormat->m_ClassType != SKILL_CLASS_MON)	return 0;
		}

		
		if(pDataFormat->GetAttack()->Use())
			AttackActive(pPc,msg);

		
		if(pDataFormat->GetAffect()->Use())
			AffectActive(pPc,msg);

		
		if(pDataFormat->GetAffect()->GetCommon()->Use())
			AffectCommonActive(pPc,msg);

		
		if(pDataFormat->GetAffect()->GetSpe()->Use())
			AffectSpe(pPc,msg);

		
		if(pDataFormat->m_MessageType ==SKILL_MSG_TYPE_PASSIVE) return true;

		if(pDataFormat->m_MultiSkill)
		{
			for(int i=0;i<pDataFormat->m_MultiSkill;i++)
			{
				MultiDamageSkill(pPc,msg,i);
				MakeMessageProc(pPc,msg);
				SendMessageProc(pPc,msg);
			}
		}
		else
		{
			MakeMessageProc(pPc,msg);
			SendMessageProc(pPc,msg);
		}
	}

	return true;
}


bool CSkillProc::MakeMessageProc(void *pPc,CStr_Skill_Msg *msg)
{
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(msg->nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;

	switch(pDataFormat->m_ClassType) 
	{
	case SKILL_CLASS_PC:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
				AI_MakeMessage( pc, msg->message, MESSAGE_USEINFO, msg->msginfo );
			else 
			{
				return false;
			}
		}
		break;

	case SKILL_CLASS_MON:
		{
			monsterCharacter_t* pMon = static_cast<monsterCharacter_t*>(pPc);
			if(pMon)
				AI_MakeMessage( pMon, msg->message, MESSAGE_USEINFO, msg->msginfo );
			else 
			{
				return false;
			}
				
		}
		break;
	}

	return true;
}

bool CSkillProc::SendMessageProc(void *pPc,CStr_Skill_Msg *msg)
{
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(msg->nSkillID);
	if(pSkillTable==NULL) return false;

	CSkillDataFormat *pDataFormat = pSkillTable->GetDataFormat();
	if(pDataFormat == NULL) return false;

	switch(pDataFormat->m_MessageType) {
	case SKILL_MSG_TYPE_ONLYSEND:
	case SKILL_MSG_TYPE_MAKESEND:
		{
			AI_SendMessage( msg->message );
		}
		break;
	case SKILL_MSG_TYPE_ATTACK_LENGTH:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				CAISendMessage AIMsg;
				AIMsg.range			= pc->calAttackLength;
				AIMsg.rangeType		= MSG_RANGE_TYPE_EXCEPT_PARTY;
				AIMsg.m_pSkillMsg	= msg;
				AI_SendMessage( msg->message,&AIMsg);
			}
		}
		break;
	case SKILL_MSG_TYPE_RANGE_TYPE_TARGET:
		{
			float fRange = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RANGE,msg);

			CAISendMessage AIMsg;
			AIMsg.range			= (int)fRange;
			AIMsg.rangeType		= MSG_RANGE_TYPE_TARGET;
			AIMsg.m_pSkillMsg	= msg;
			AI_SendMessage( msg->message,&AIMsg);
		}
		break;
	case SKILL_MSG_TYPE_RANGE:
		{
			float fRange = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RANGE,msg);
			CAISendMessage AIMsg;
			AIMsg.range			= fRange;
			AIMsg.m_pSkillMsg	= msg;
			AI_SendMessage( msg->message,&AIMsg);
		}
		break;
	
	case SKILL_MSG_TYPE_RANGE_OF_SELF:
		{
			float fRange = g_SkillDataGlobal.GetValue(pPc,SKILL_TYPE_RANGE,msg);
			CAISendMessage AIMsg;
			AIMsg.range			= fRange;
			AIMsg.rangeType		= MSG_RANGE_TYPE_MYSELF;
			AIMsg.m_pSkillMsg	= msg;
			AI_SendMessage( msg->message,&AIMsg);
		}
		break;
	}

	return true;
}

bool CSkillProc::BasicActive(void *pPc,CStr_Skill_Msg *msg)
{
	CSkillDataFormat *pDataFormat = msg->pDataFormat;
	messageInfo_t	*msginfo =msg->msginfo;

	
	if(pDataFormat->m_InfoType )
		msginfo->infoType		= m_BasicDataProc.GetInfoType(pDataFormat->m_InfoType);

	if(pDataFormat->m_TargetIdx )
		msginfo->targetIdx		= m_BasicDataProc.GetTargetIdx(pDataFormat->m_TargetIdx,pPc);

	if(pDataFormat->m_TargetType )
		msginfo->targetType		= m_BasicDataProc.GetTargetType(pDataFormat->m_TargetType,pPc);

	return true;
}

bool CSkillProc::AttackActive(void *pPc,CStr_Skill_Msg *msg)
{
	messageInfo_t	*msginfo =msg->msginfo;
	CSkillDataFormat_Attack *pAttack		= msg->pDataFormat->GetAttack();
	if(pAttack == NULL) return false;

	if(pAttack->m_AtkType )
		msginfo->attack.atkType			= m_AttackDataProc.GetAttackType(pAttack->m_AtkType,pPc,msg->nSkillID);

	
	if(pAttack->m_HitPower )
		msginfo->attack.hitPower		= m_AttackDataProc.GetHitPower(pAttack->m_HitPower,msg);

	if(pAttack->m_HitRate )
		msginfo->attack.hitRate			= m_AttackDataProc.GetHiteRate(pAttack->m_HitRate,msg);

	if(pAttack->m_PhyMinDmg )
		msginfo->attack.phyMinDmg		= m_AttackDataProc.GetPhyMinDmg(pAttack->m_PhyMinDmg,msg);

	if(pAttack->m_PhyMaxDmg )
		msginfo->attack.phyMaxDmg		= m_AttackDataProc.GetPhyMaxDmg(pAttack->m_PhyMaxDmg,msg);

	if(pAttack->m_PosMinDmg )
		msginfo->attack.posMinDmg		= m_AttackDataProc.GetPosMinDmg(pAttack->m_PosMinDmg,msg);

	if(pAttack->m_PosMaxDmg )
		msginfo->attack.posMaxDmg		= m_AttackDataProc.GetPosMaxDmg(pAttack->m_PosMaxDmg,msg);







	if(pAttack->m_CriticalRate )
		msginfo->attack.criticalRate	= m_AttackDataProc.GetCriticalRate(pAttack->m_CriticalRate,msg);

	if(pAttack->m_CriticalDamage )
		msginfo->attack.criticalDamage	= m_AttackDataProc.GetCriticalDamage(pAttack->m_CriticalDamage,msg);

	if(pAttack->m_DelayTime )
		msginfo->attack.delayTime		= m_AttackDataProc.GetDelayTime(pAttack->m_DelayTime,msg);

	if(pAttack->m_RepeatNum )
		msginfo->attack.repeatNum		= m_AttackDataProc.GetRepeatNum(pAttack->m_RepeatNum,msg);

	if(pAttack->m_RepeatDelayTime )
		msginfo->attack.repeatDelayTime = m_AttackDataProc.GetRepeatDelayTime(pAttack->m_RepeatDelayTime,msg);

	
	if(pAttack->m_EtcData[0].m_nIndex )
	{
		msginfo->attack.etcInfo			= m_AttackDataProc.GetEtcType(pAttack->m_EtcData[0].m_nIndex,msg);
		msginfo->attack.etcData0		= m_AttackDataProc.GetEtcInfo(pAttack->m_EtcData[0].m_nIndex,msg);
	}

	
	if(pAttack->m_EtcData[1].m_nIndex )
	{
		msginfo->attack.etcInfo			= m_AttackDataProc.GetEtcType(pAttack->m_EtcData[0].m_nIndex,msg);
		msginfo->attack.etcData1		= m_AttackDataProc.GetEtcInfo(pAttack->m_EtcData[1].m_nIndex,msg);
	}
		

	return true;
}

bool CSkillProc::AffectActive(void *pPc,CStr_Skill_Msg *msg)
{
	messageInfo_t	*msginfo =msg->msginfo;
	CSkillDataFormat_Affect *pAffect		= msg->pDataFormat->GetAffect();
	if(pAffect == NULL) return false;

	if(pAffect->m_Type)
		msginfo->affect.type			= m_AffectDataProc.GetAffType(pAffect->m_Type);

	if(pAffect->m_AffectType)
		msginfo->affect.affectType		= m_AffectDataProc.GetAffectType(pAffect->m_AffectType);

	if(pAffect->m_StartTime )
		msginfo->affect.startTime		= m_AffectDataProc.GetStartTime(pAffect->m_StartTime,msg);

	if(pAffect->m_KeepTime )
		msginfo->affect.keepTime		= m_AffectDataProc.GetKeepTime(pAffect->m_KeepTime,msg);

	if(pAffect->m_Level )
		msginfo->affect.level			= m_AffectDataProc.GetPosMinDmg(pAffect->m_Level,msg);

	if(pAffect->m_HitRate)
		msginfo->affect.hitRate			= m_AffectDataProc.GetHitRate(pAffect->m_HitRate,msg);

	if(pAffect->m_SkillTableIdx)
		msginfo->affect.skillTableIdx	= pAffect->m_SkillTableIdx;

	if(pAffect->m_DelayTime)
		msginfo->affect.delayTime		= m_AffectDataProc.GetDelayTime(pAffect->m_DelayTime,msg);

	return true;
}


bool CSkillProc::AffectCommonActive(void *pPc,CStr_Skill_Msg *msg)
{
	CSkillDataFormat_Affect *pAffect	= msg->pDataFormat->GetAffect();
	if(pAffect == NULL) return false;

	CSkillDataCommon *pCommon			= pAffect->GetCommon();
	if(pCommon == NULL) return false;

	
	for(int i=0;i<MAX_COUNT_AFFECT_DATA;i++)
	{
		if(pCommon->m_Data[i].m_Type)
		{
			m_AffectCommonDataProc.GetData(i,pCommon->m_Data[i].m_Type,pCommon->m_Data[i].m_nIndex,pPc,msg);
		}
		else
		{
			
			break;
		}
	}	

	
	for(i=0;i<MAX_COUNT_AFFECT_DATA;i++)
	{
		if(pCommon->m_OnceData[i].m_Type)
		{
			m_AffectCommonDataProc.OnceAffectProcess(i,
				pCommon->m_OnceData[i].m_Type,pCommon->m_OnceData[i].m_nIndex,pPc,msg);
		}
		else
		{
			break;
		}
	}	


	return true;
}

bool CSkillProc::AffectSpe(void *pPc,CStr_Skill_Msg *msg)
{
	messageInfo_t	*msginfo =msg->msginfo;
	CSkillDataFormat_Affect *pAffect	= msg->pDataFormat->GetAffect();
	if(pAffect == NULL) return false;

	CSkillDataAffectSpe *pSpe			= pAffect->GetSpe();
	if(pSpe == NULL) return false;

	if(pSpe->m_SpecialType)
		msginfo->affect.spe.specialType			= m_AffectSpe.GetType(pSpe->m_SpecialType);

	if(pSpe->m_SpecialIdx)
		msginfo->affect.spe.specialIdx			= m_AffectSpe.GetSpecialIdx(pSpe->m_SpecialIdx,pPc,msg);

	for(int i=0;i<MAX_COUNT_AFFECT_SPE_DATA;i++)
	{
		if(pSpe->m_Data[i].m_Type)
		{
			msginfo->affect.spe.specialData[i]	= m_AffectSpe.GetSpecialData(pSpe->m_Data[i].m_Type,pSpe->m_Data[i].m_nIndex,pPc,msg);
		}
		else
		{
			
			break;
		}			
	}

	
	msginfo->affect.spe.specialData[MAX_COUNT_AFFECT_SPE_DATA-1]	= msg->nMySkillIndex;
	return true;
}


bool CSkillProc::MessageActive(void *pPc,CStr_Skill_Msg *msg)
{
	message_t		*message = msg->message;
	CSkillData_Message *pMessage		= msg->pDataFormat->GetMessage();
	if(pMessage == NULL) return false;

	if(pMessage->m_Type)
		message->type			= g_SkillDataMsgProc.GetType(pMessage->m_Type,pPc,msg->nSkillID);

	if(pMessage->m_FromIdx)
		message->fromIdx		= g_SkillDataMsgProc.GetFromIdx(pMessage->m_FromIdx,pPc,msg->nSkillID);

	if(pMessage->m_FromType)
		message->fromType		= g_SkillDataMsgProc.GetFromType(pMessage->m_FromType,pPc,msg->nSkillID);

	if(pMessage->m_Name)
		g_SkillDataMsgProc.GetName(pMessage->m_Name,message,pPc,msg->nSkillID);

	if(pMessage->m_ToType)
		message->toType			= g_SkillDataMsgProc.GetToType(pMessage->m_ToType,pPc,msg->nSkillID);

	if(pMessage->m_ToIdx)
		message->toIdx			= g_SkillDataMsgProc.GetToIdx(pMessage->m_ToIdx,pPc,msg->nSkillID);

	if(pMessage->m_WorldIdx)
		message->worldIdx		= g_SkillDataMsgProc.GetWorldIdx(pMessage->m_WorldIdx,pPc,msg->nSkillID);

	if(pMessage->m_SendTime)
		message->sendTime		= g_SkillDataMsgProc.GetSendTime(pMessage->m_SendTime,pPc,msg->nSkillID);

	if(pMessage->m_ReceiveTime)
		message->receiveTime	= g_SkillDataMsgProc.GetReceiveTime(pMessage->m_ReceiveTime,pPc,msg->nSkillID);


	for(int i=0;i<MAX_SKILL_MESSAGE_DATA;i++)
	{
		if(pMessage->m_Data[i].m_nIndex)
		{
			
			message->data[i]	= g_SkillDataMsgProc.GetData(pMessage->m_Data[i].m_nIndex,pPc,msg);
		}
		else
		{
			break;
		}
	}		

			
	for(i=0;i<MAX_SKILL_MESSAGE_DATA;i++)
	{
		if(pMessage->m_Data[i].m_Type)
		{
			message->Type[i]	= pMessage->m_Data[i].m_Type;
		}
		else
		{
			break;
		}
	}		
			
	return true;
}


bool CSkillProc::CastActive(void *pPc,CStr_Skill_Msg *msg)
{
	
	BasicActive(pPc,msg);

	CSkillDataCast *pCast		= msg->pDataFormat->GetCast();
	if(pCast == NULL) return false;

	if(pCast->m_Type)
	{
		g_SkillCast.RaProc(pCast->m_Type,pPc,msg);
	}
	
	return true;
}
	

void CSkillProc::MultiDamageSkill(void *pPc,CStr_Skill_Msg *msg,int Index)
{
	CSkillDataFormat_Attack *pAttack		= msg->pDataFormat->GetAttack();
	if(pAttack == NULL) return;

	messageInfo_t	*msginfo =msg->msginfo;
	if(msginfo == NULL) return;

	
	if(msginfo->attack.etcInfo	== MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY)
	{
		msginfo->attack.etcData0		+= 300;
	}
}
	
