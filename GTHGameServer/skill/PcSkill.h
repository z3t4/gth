



#if !defined(AFX_PCSKILL_H__04F7C9B3_4385_4747_92BB_5422F7B36545__INCLUDED_)
#define AFX_PCSKILL_H__04F7C9B3_4385_4747_92BB_5422F7B36545__INCLUDED_

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

class CPcSkill  
{
public:
	CPcSkill();
	virtual ~CPcSkill();

	void GTH_PC_Skill(playerCharacter_t *pc, int skillIdx, int skillTargetType);
	void GTH_PC_SkillNotOwn(playerCharacter_t *pc,int UseType, int invenPos, int SkillTableIndex, int level, int skillTargetType);
	void GTH_PC_SkillCooltime( playerCharacter_t *pc );
	void GTH_PC_SkillExecute( playerCharacter_t *pc );
	void GTH_PC_SkillCasting( playerCharacter_t *pc );

	
	void GTH_ProcessMessage_Request_SkillLevelUp();
	void GTH_ProcessMessage_Request_Learn_Skill();
	void GTH_ProcessEventMessage_Skill(void);

	bool	FindHideSkill(int index);

	
	void	GTH_Recv_UseSkill();
	void	GTH_Recv_UseItemSkill();
	bool	CheckEnableUseSkill(int targetType,int targetIdx,int skillTargetType);

	
	inline bool	CheckEnableUseSkill_Player(const int targetType,const int targetIdx,const int skillTargetType);
	inline bool	CheckEnableUseSkill_Monster(const int targetType,const int targetIdx,const int skillTargetType);

};


extern CPcSkill g_PcSkill;

#endif 
