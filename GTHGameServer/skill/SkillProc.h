



#if !defined(AFX_SKILLPROC_H__1D5E71BE_020A_417C_B7ED_E12B3419AC52__INCLUDED_)
#define AFX_SKILLPROC_H__1D5E71BE_020A_417C_B7ED_E12B3419AC52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define SKILL_TEST_VERSION 1

class CSkillProc  
{
public:
	CSkillProc();
	virtual ~CSkillProc();

	bool		Active(void *pPc,CStr_Skill_Msg *msg);
	bool		BasicActive(void *pPc,CStr_Skill_Msg *msg);
	bool		AttackActive(void *pPc,CStr_Skill_Msg *msg);
	bool		AffectActive(void *pPc,CStr_Skill_Msg *msg);
	bool		AffectCommonActive(void *pPc,CStr_Skill_Msg *msg);
	bool		AffectSpe(void *pPc,CStr_Skill_Msg *msg);
	bool		MessageActive(void *pPc,CStr_Skill_Msg *msg);

	int			Proc(void *pPc,CStr_Skill_Msg *msg);


	bool		CastActive(void *pPc,CStr_Skill_Msg *msg);
	bool		SendMessageProc(void *pPc,CStr_Skill_Msg *msg);
	bool		MakeMessageProc(void *pPc,CStr_Skill_Msg *msg);
	void		MultiDamageSkill(void *pPc,CStr_Skill_Msg *msg,int Index);

	CSkillProcAffectSpe		*GetAffectSpe()
	{
		return &m_AffectSpe;
	}

public:
	CSkillProcBasic			m_BasicDataProc;
	CSkillProcAttack		m_AttackDataProc;
	CSkillProcAffect		m_AffectDataProc;
	CSkillProcAffectCommon  m_AffectCommonDataProc;
	CSkillProcAffectSpe		m_AffectSpe;
};


extern CSkillProc g_SkillProc;

#endif 
