



#if !defined(AFX_SKILLCAST_H__8888D8C9_0EC5_4305_8AB8_FF5D2CF34ECD__INCLUDED_)
#define AFX_SKILLCAST_H__8888D8C9_0EC5_4305_8AB8_FF5D2CF34ECD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillCast  
{
public:
	CSkillCast();
	virtual ~CSkillCast();

	int		RaProc(BYTE _Type,void *pPc,CStr_Skill_Msg *msg);

};

extern CSkillCast g_SkillCast;

#endif 
