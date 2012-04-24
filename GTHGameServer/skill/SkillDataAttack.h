



#if !defined(AFX_SKILLDATAATTACK_H__2982A5F7_FE1E_4316_8AF0_22853F186100__INCLUDED_)
#define AFX_SKILLDATAATTACK_H__2982A5F7_FE1E_4316_8AF0_22853F186100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillProcAttack  
{
public:
	CSkillProcAttack();
	virtual ~CSkillProcAttack();

	int GetAttackType(BYTE _Type,void *pPc,int nSkillID);
	
	int GetHitPower(BYTE _Type,CStr_Skill_Msg *msg);
	int GetHiteRate(BYTE _Type,CStr_Skill_Msg *msg);

	int GetPhyMinDmg(BYTE _Type,CStr_Skill_Msg *msg);

	
	int GetPhyMaxDmg(BYTE _Type,CStr_Skill_Msg *msg);
	int GetPosMinDmg(BYTE _Type,CStr_Skill_Msg *msg);
	int GetPosMaxDmg(BYTE _Type,CStr_Skill_Msg *msg);
	int GetNegMinDmg(BYTE _Type,CStr_Skill_Msg *msg);
	int GetNegMaxDmg(BYTE _Type,CStr_Skill_Msg *msg);
	int GetCriticalRate(BYTE _Type,CStr_Skill_Msg *msg);
	int GetCriticalDamage(BYTE _Type,CStr_Skill_Msg *msg);
	int GetDelayTime(BYTE _Type,CStr_Skill_Msg *msg);
	int GetRepeatNum(BYTE _Type,CStr_Skill_Msg *msg);
	int GetRepeatDelayTime(BYTE _Type,CStr_Skill_Msg *msg);
	int GetEtcInfo(BYTE _Type,CStr_Skill_Msg *msg);
	int GetEtcType(BYTE _Type,CStr_Skill_Msg *msg);

};

#endif 
