



#if !defined(AFX_ATTACKPROC_H__6B6881F6_63C9_47BE_8986_1BD1EAB7A485__INCLUDED_)
#define AFX_ATTACKPROC_H__6B6881F6_63C9_47BE_8986_1BD1EAB7A485__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 




enum
{
	SKILL_RESEND_INIT,
	SKILL_RESEND_PREPARE,
	SKILL_RESEND_COMPLETE
};

class CAttackProc  
{
public:
	int			m_ntempAttackSuccessCount;
	int			m_ntempAttackMissCount;

public:
	CAttackProc();
	virtual ~CAttackProc();

	int			InTargetList( char* name );
	int			IsSaftTargeting();
	void		GTH_SendEventMessage_Attack();
	int			GTH_SendEventMessage_Skill();
	int			GTH_ProcessEventMessage_Attack();
	int			GTH_ProcessEventMessage_Damage();

	
	
	bool		GTH_EnableSkillExcuteDist(int SkillIndex,int TargetType,float fGap=2.0f, float fAddGap=0.0f);
};

extern CAttackProc g_AttackProc;

#endif 
