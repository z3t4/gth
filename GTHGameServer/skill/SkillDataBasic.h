



#if !defined(AFX_SKILLDATABASIC_H__46296A91_0EEC_44B1_9194_A22020663B4F__INCLUDED_)
#define AFX_SKILLDATABASIC_H__46296A91_0EEC_44B1_9194_A22020663B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSkillProcBasic  
{
public:
	CSkillProcBasic();
	virtual ~CSkillProcBasic();

	int			GetInfoType(BYTE _Type);
	int			GetTargetType(BYTE _Type,void *pPc);
	int			GetTargetIdx(BYTE _Type,void *pPc,int index=0);
};

#endif 
