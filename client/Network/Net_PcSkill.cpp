 #include "../global_def.h"

extern i3socket_t gsSocket;	
extern i3sizebuf_t netMessage; 





int GTH_SendMessage_Request_SkillLevelUp( int skillIdx, int genType )
{
	if( !GTH_LockSending() ) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_SKILLLEVELUP );
		MSG_WriteByte(&netMessage, skillIdx);	
		MSG_WriteByte(&netMessage, genType);
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Reply_SkillLevelUp()
{
	GTH_UnlockSending();

	char szTemp[256];

	int reply, error;
	int skillIdx, iLevel, genType, genCapability, level;
	int skillTableIdx;

	reply = MSG_ReadByte();
	if( !reply )
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_SKILL_4 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,385) );
			break;
		case ERROR_SKILL_5 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,415) );
			break;
		case ERROR_SKILL_6 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,413) );
			break;
		case ERROR_SKILL_7 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,414) );
			break;
		}
		return 0;
	}

	skillIdx = MSG_ReadByte();
	iLevel = MSG_ReadLong();
	genType = MSG_ReadByte();
	genCapability = MSG_ReadLong();

	g_cgv.myCharacterInfo->skill[skillIdx].iLevel = iLevel;
	g_cgv.myCharacterInfo->genCapability[genType] = genCapability;
	
	skillTableIdx = g_cgv.myCharacterInfo->skill[skillIdx].tableIdx;

	level = g_cgv.myCharacterInfo->skill[skillIdx].level.gen[genType];

	
	


	
	CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
	if(pSkillTable!=NULL) 
	{
		

		
		sprintf( szTemp, g_LPACK.GetMassage(0,388), g_LPACK.Get(LPACK_TYPE_SKILL,pSkillTable->m_NameIdx), 
			g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, genType ), level );
	}
	else
	{
		sprintf( szTemp, g_LPACK.GetMassage(0,388), "pSkillTable == NULL", 
			g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, genType ), level );
	}

	g_ifMng->AddSysMessage( szTemp );
	g_ifMng->m_learnWin->MakeLearnSkillInfo();

	return 1;
}

int GTH_SendMessage_Request_LearnSkill( int invenPos )
{
	if( !GTH_LockSending() ) return false;

	MSG_BeginWriting(&netMessage);
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage, CC_REQUEST_LEARN_SKILL );
		MSG_WriteByte(&netMessage, invenPos );	
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting( &netMessage );

	return 1;
}

int GTH_ProcessMessage_Reply_LearnSkill()
{
	GTH_UnlockSending();

	int reply, error;
	int inventoryPos, skillIdx, skillTableIdx, iLevel;
	
	reply = MSG_ReadByte();
	if( reply )
	{
		inventoryPos = MSG_ReadByte();
		skillIdx = MSG_ReadByte();
		skillTableIdx = MSG_ReadShort();
		iLevel = MSG_ReadLong();

		int itemIdx;
		item_t *item;

		itemIdx = g_cgv.myCharacterInfo->inventory[inventoryPos];
		item = &g_cgv.myCharacterInfo->item[itemIdx];

		GTH_DeleteMyItem( &g_cgv.myCharacterInfo->item[itemIdx] );
		g_cgv.myCharacterInfo->inventory[inventoryPos] = -1;

		
		g_cgv.myCharacterInfo->skill[skillIdx].tableIdx = skillTableIdx;
		g_cgv.myCharacterInfo->skill[skillIdx].iLevel = iLevel;
		g_cgv.myCharacterInfo->skillNumber ++;
	}
	else
	{
		error = MSG_ReadByte();
		switch( error )
		{
		case ERROR_SKILL_0 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,390) );
			break;
		case ERROR_SKILL_1 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,389) );
			break;
		case ERROR_SKILL_2 :
			
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,391) );
			break;
		case ERROR_SKILL_3 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,407), g_LPACK.GetMassage(0,391) );
			break;
		case ERROR_SKILL_4 :
			g_ifMng->SetMessage( g_LPACK.GetMassage(0,407), g_LPACK.GetMassage(0,391) );
			break;
		}
	}
	return 1;
}

int GTH_ProcessMessage_Reply_AddSkill()
{
	int skillNumber;
	int skillIdx, iLevel;
	int skillTableIdx;

	skillNumber = MSG_ReadByte();
	
	for( int i = 0; i < skillNumber; i ++ )
	{
		skillIdx = MSG_ReadByte();
		skillTableIdx = MSG_ReadShort();
		iLevel = MSG_ReadLong();
		g_SkillLogic.GTH_AddSkill( skillIdx, skillTableIdx, iLevel );
	}

	return 1;
}


bool	GTH_ProcessMessage_Repl_SkillError()
{
	BYTE ErrorIdx = MSG_ReadByte();

	switch(ErrorIdx) {

	
	case SKILL_ERROR_BATTLE_TEST:
		g_ifMng->AddSysMessage(g_LPACK.Get(LPACK_TYPE_SKILL,4005), GTHCOLOR_SYSMESSAGE );
		break;

	
	case SKILL_ERROR_BATTLE:
		g_ifMng->AddSysMessage(g_LPACK.Get(LPACK_TYPE_SKILL,4005), GTHCOLOR_SYSMESSAGE );
		break;
	

	
	case SKILL_ERROR_PK_DISABLE:
		g_ifMng->AddSysMessage( g_LPACK.Get(LPACK_TYPE_SKILL,4006), GTHCOLOR_SYSMESSAGE );
		break;

	
	case SKILL_ERROR_COOLTIME:
		g_ifMng->AddSysMessage( g_LPACK.Get(LPACK_TYPE_SKILL,4007), GTHCOLOR_SYSMESSAGE );
		break;
	}

	return true;
}