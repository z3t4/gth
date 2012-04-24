#ifndef _SKILL_LOGIC_H
#define _SKILL_LOGIC_H

#define MAX_COUNT_SKILL_MULTI_TARGET		8

enum
{
	SKILL_WEPON_TYPE=0,
	SKILL_ITEM_TYPE,
};


class CSkillLogic
{
public:
	bool		m_bSendTransform;

public:
	CSkillLogic();
	~CSkillLogic();

	bool		GetMyCharTransform() { return m_bSendTransform; 	}
	void		SetMyCharTransform(bool flag) { m_bSendTransform = flag; }
	bool		CancelTransform(bool bSendCheck = false);

public:
	int			GTH_HaveSkill( int skillTableIdx );
	void		GTH_AddSkill( int idx, int skillTableIdx, int initLevel );

	void		GTH_GetSkillValue( int skillIdx, float value[] );
	void		GTH_GetSkillValue( int skillTableIdx, float value[], SKILL_GEN_LEVEL skillLevel );


	int			GTH_SkillProcess( int UseType, int Idx );
	void		GTH_SkillCoolTimeProcess();
	int			GTH_SkillClickEvent();
	void		GTH_AddSkillEffect( CSkillEffect *pEffect, Fx_CHARACTER_t *attacker, Fx_CHARACTER_t *defender, int effectLevel );
	int			GTH_SkillExecute( Fx_CHARACTER_t* character, int tableIdx, int level, int multiTargetNumber, int multiTargetType[], int multiTargetIdx[] ,int targetType);
	int			GTH_GetSkillEffectLevel( int skillTableIdx, int skillLevel );

	
	int			GTH_BeforeTeleportExecute( Fx_CHARACTER_t* character, int pcjob);
	void		GTH_AddSkillCastingEffect( Fx_CHARACTER_t *character, float castingTime );
	int			GTH_SkillCheckActive( int skillIdx );
	float		GTH_GetSkillActiveDistance();
	void		CheckSkillExecption(int weaponType, int skillIdx, int& aniID, CSkillEffect *pEffect);

	
	float		GTH_GetSkillActiveDistance(int SkillIndex,int TargetType);
	int			GTH_SkillProcess_TargetSetting(int &SkillTargetType,vec3_t &touched,vec3_t  &delta);

	
	void		UnSelectSkill();

	
	void		CancelSkillProcess();
	int			GTH_SkillProcess_Skill(int Idx);
	bool		GTH_EnableSkillProcess(int Idx);
	int			GTH_SkillProcess_Item(int Idx);


	
	void		GTH_DeleteSKill(int idx);

};

extern CSkillLogic	g_SkillLogic;
#endif