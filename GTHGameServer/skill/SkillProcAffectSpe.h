



#if !defined(AFX_SKILLPROCAFFECTSPE_H__C512C7E8_AA20_4FEE_9836_43C1F9C104F5__INCLUDED_)
#define AFX_SKILLPROCAFFECTSPE_H__C512C7E8_AA20_4FEE_9836_43C1F9C104F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillProcAffectSpe  
{
public:
	CSkillProcAffectSpe();
	virtual ~CSkillProcAffectSpe();

	int			GetType(BYTE _Type);
	int			GetSpecialIdx(BYTE _Type, void *pPc,CStr_Skill_Msg *msg);
	float		GetSpecialData(BYTE _Type,BYTE _Index, void *pPc,CStr_Skill_Msg *msg);
	bool		Proc(int nSkillID,void *pPc,AFFECTDATA_SPECIAL *spe);
};

#endif 
