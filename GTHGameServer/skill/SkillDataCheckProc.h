



#if !defined(AFX_SKILLDATACHECKPROC_H__58F15E15_DEF2_4990_A861_7B332E6E2B0A__INCLUDED_)
#define AFX_SKILLDATACHECKPROC_H__58F15E15_DEF2_4990_A861_7B332E6E2B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillDataCheckProc  
{
public:
	CSkillDataCheckProc();
	virtual ~CSkillDataCheckProc();

	bool		CheckSkillProc(void *pPc,CStr_Skill_Msg *msg);
};

extern CSkillDataCheckProc g_SkillDataCheckProc;

#endif 
