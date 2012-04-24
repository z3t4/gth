



#if !defined(AFX_SKILLDATAAFFECT_H__E793C64B_7BFF_4977_A01F_431B4FC63176__INCLUDED_)
#define AFX_SKILLDATAAFFECT_H__E793C64B_7BFF_4977_A01F_431B4FC63176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillProcAffect  
{
public:
	CSkillProcAffect();
	virtual ~CSkillProcAffect();

	int GetAffType(BYTE _Type);
	int GetAffectType(BYTE _Type);
	int GetStartTime(BYTE _Type,CStr_Skill_Msg *msg);
	int GetKeepTime(BYTE _Type,CStr_Skill_Msg *msg);
	int GetPosMinDmg(BYTE _Type,CStr_Skill_Msg *msg);
	int GetHitRate(BYTE _Type,CStr_Skill_Msg *msg);

	int GetDelayTime(BYTE _Type,CStr_Skill_Msg *msg);
};

#endif 
