



#if !defined(AFX_SKILLDATAMEGPROC_H__2F292335_32E0_4A79_B216_9B78DCB5197C__INCLUDED_)
#define AFX_SKILLDATAMEGPROC_H__2F292335_32E0_4A79_B216_9B78DCB5197C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillDataMsgProc  
{
public:
	CSkillDataMsgProc();
	virtual ~CSkillDataMsgProc();

	int GetType(int _Type,void *pPc,int nSkillID);
	int GetFromIdx(int _Type,void *pPc,int nSkillID);
	int GetFromType(int _Type,void *pPc,int nSkillID);
	int GetName(int _Type,message_t *message,void *pPc,int nSkillID);
	int GetToType(int _Type,void *pPc,int nSkillID);
	int GetToIdx(int _Type,void *pPc,int nSkillID);
	int GetWorldIdx(int _Type,void *pPc,int nSkillID);
	int GetSendTime(int _Type,void *pPc,int nSkillID);
	int GetReceiveTime(int _Type,void *pPc,int nSkillID);
	
	int GetData(int _Type,void *pPc,CStr_Skill_Msg *msg);


};

extern CSkillDataMsgProc g_SkillDataMsgProc;

#endif 
