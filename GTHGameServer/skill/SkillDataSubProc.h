



#if !defined(AFX_SKILLDATASUBPROC_H__3C7DA846_1062_437E_93AD_9311A50B8D02__INCLUDED_)
#define AFX_SKILLDATASUBPROC_H__3C7DA846_1062_437E_93AD_9311A50B8D02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillDataSubProc  
{
public:
	CSkillDataSubProc();
	virtual ~CSkillDataSubProc();

	void	SKILL_AttractAttention(void *pPc,CStr_Skill_Msg *msg);
	
	bool	SubActionProc(void *pPc,CStr_Skill_Msg *msg);

};

extern CSkillDataSubProc g_SkillDataSubProc;

#endif 
