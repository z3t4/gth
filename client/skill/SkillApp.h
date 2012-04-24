



#if !defined(AFX_SKILLAPP_H__1F4D6A97_F467_44B7_956C_A83C5DCF5D36__INCLUDED_)
#define AFX_SKILLAPP_H__1F4D6A97_F467_44B7_956C_A83C5DCF5D36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum
{
	SKILL_ERROR_BATTLE_TEST=1,		
	SKILL_ERROR_BATTLE,				
	SKILL_ERROR_PK_DISABLE,			
	SKILL_ERROR_COOLTIME,			
};

class CSkillApp  
{
public:
	int skillResendFlag;

public:
	CSkillApp();
	virtual ~CSkillApp();

	int			Recv_ProcessEventMessage_Skill();
	int			Recv_ProcessEventMessage_SkillCasting();
	int			GTH_SkillCastingProcess();

	void		GTH_Character_Skill( Fx_CHARACTER_t *character );
	void		GTH_Character_SkillCasting( Fx_CHARACTER_t* character );
	void		ActionProcess( int id );
	int			CheckActionTarget( int entityType = ENTITY_PC, int checkDistanceFlag = false );
};

extern CSkillApp g_SkillApp;

#endif 
