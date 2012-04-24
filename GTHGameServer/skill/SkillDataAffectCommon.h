



#if !defined(AFX_SKILLDATAAFFECTCOMMON_H__A0452C32_E973_408A_9D97_4246253E6348__INCLUDED_)
#define AFX_SKILLDATAAFFECTCOMMON_H__A0452C32_E973_408A_9D97_4246253E6348__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillProcAffectCommon  
{
public:
	CSkillProcAffectCommon();
	virtual ~CSkillProcAffectCommon();

	short GetData(int idx,BYTE _Type,BYTE _Index,void *pPc,CStr_Skill_Msg *msg);

	
	short DeleteBuffSkill(void *pPc);

	
	short OnceAffectProcess(int idx,BYTE _Type,BYTE _Index,void *pPc,CStr_Skill_Msg *msg);

};

#endif 
