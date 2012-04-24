



#if !defined(AFX_SKILLPARTYPROC_H__B6C5A52E_CCA6_40DC_B41D_656A584CD081__INCLUDED_)
#define AFX_SKILLPARTYPROC_H__B6C5A52E_CCA6_40DC_B41D_656A584CD081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CSkillPartyProc  
{
public:
	CSkillPartyProc();
	virtual ~CSkillPartyProc();

	bool		IfPartyMember(int MyIndex,int PartyMemberIndex);
	bool		Proc(void *pPc,CStr_Skill_Msg *msg);
	bool		PartyCheck(void* pPc,int nIndex,CStr_Skill_Msg *msg);
	bool		Check(void* pPc,int nIndex,CStr_Skill_Msg *msg);
};

extern CSkillPartyProc g_SkillPartyProc;

#endif 
