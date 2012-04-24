



#include "../Global.h"

#include "..\Tools\CTools.h"
extern CTools* gcpTools;

CSkillDataMsgProc g_SkillDataMsgProc;





CSkillDataMsgProc::CSkillDataMsgProc()
{

}

CSkillDataMsgProc::~CSkillDataMsgProc()
{

}



int CSkillDataMsgProc::GetType(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_TYPE: return MSG_ETC;
		break;
	}

	return NO_TYPE;
}


int CSkillDataMsgProc::GetFromIdx(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_FROMIDX:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				return pc->idx;
			}
		}
		break;
	}

	
	return NO_TYPE;
}

int CSkillDataMsgProc::GetFromType(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_FROMTYPE: return ENTITY_PC;
		break;
	}

	
	return NO_TYPE;
}

int CSkillDataMsgProc::GetName(int _Type,message_t *message,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_NAME:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				int len = strlen(pc->name);
				if(len <0 || len > 255)
				{
					
				}

				strncpy( message->fromNameV2, pc->name,NAMESTRING);
				message->fromNameV2[NAMESTRING]=NULL;
				return true;
			}
		}
		break;
	}

	
	return NO_TYPE;
}


int CSkillDataMsgProc::GetToType(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_TOTYPE:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				return pc->targetType;
			}
		}
		break;
	}

	
	return NO_TYPE;
}

int CSkillDataMsgProc::GetToIdx(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_TOIDX:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				return pc->targetIdx;
			}
		}
		break;
	}

	
	return NO_TYPE;
}

int CSkillDataMsgProc::GetWorldIdx(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_WORLDIDX:
		{
			playerCharacter_t *pc	=	static_cast<playerCharacter_t*>(pPc);
			if(pc)
			{
				return pc->worldIdx;
			}
		}

		break;
	}

	
	return NO_TYPE;
}


int CSkillDataMsgProc::GetSendTime(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_SENDTIME: return g_globalTime;
		break;
	}


	return NO_TYPE;
}

int CSkillDataMsgProc::GetReceiveTime(int _Type,void *pPc,int nSkillID)
{
	switch(_Type) {
	case SKILL_MES_RECEIVETIME: return g_globalTime + 1500;
		break;
	}

	return NO_TYPE;
}



int CSkillDataMsgProc::GetData(int _Type,void *pPc,CStr_Skill_Msg *msg)
{

	switch(_Type) {
	case SKILL_MES_DATA_MSG_ETC_RESURRECTION:		return MSG_ETC_RESURRECTION;
		break;
	case SKILL_MES_DATA_RAREC:		
		{
				float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_MSG_TYPE_RAREC,msg);
				return fVal;
			}	
		break;
	case SKILL_MES_DATA_SAREC:		
		{
				float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_MSG_TYPE_SAREC,msg);
				return fVal;
			}	
		break;
	case SKILL_MES_DATA_EXRECRATE:
		{
			playerCharacter_t* pPlayer=NULL;

			pPlayer =  gcpTools->GetPlayerRecordPointer(msg->message->toIdx);
			if(NULL == pPlayer) return 0;

				float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_MSG_TYPE_EXP_REC_RATE,msg);

			float recoveryExp= pPlayer->reduceExp * (fVal * 0.01f);
			return recoveryExp;
			}	
		break;

	
	case SKILL_MES_DATA_RA_REC_RATE:
		{
			playerCharacter_t* pPlayer=NULL;

			pPlayer =  gcpTools->GetPlayerRecordPointer(msg->message->toIdx);
			if(NULL == pPlayer) return 0;

			float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_MSG_TYPE_RA_REC_RATE,msg);

			float fRa= pPlayer->calMaxRA * (fVal * 0.01);
			return fRa;
		}
		break;

	case SKILL_MES_DATA_SA_REC_RATE:
		{
			playerCharacter_t* pPlayer=NULL;

			pPlayer =  gcpTools->GetPlayerRecordPointer(msg->message->toIdx);
			if(NULL == pPlayer) return 0;

			float fVal = (int)g_SkillDataGlobal.GetValue(msg->pPc,SKILL_MSG_TYPE_SA_REC_RATE,msg);
			float fSa= pPlayer->calMaxSA * (fVal * 0.01);
			return fSa;
		}
		break;
	
	}
	
	return NO_TYPE;
}

