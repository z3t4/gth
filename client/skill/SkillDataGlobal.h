



#if !defined(AFX_SKILLDATAGLOBAL_H__F408CB5B_CC06_470D_A4BC_1970931C1288__INCLUDED_)
#define AFX_SKILLDATAGLOBAL_H__F408CB5B_CC06_470D_A4BC_1970931C1288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define CHARACTER_DEFAULT_GEN_LEVEL 1
class CSkillDataGlobal  
{
public:
	CSkillDataGlobal();
	virtual ~CSkillDataGlobal();
public:
	
	CSkillTableData *		GetValue(BYTE _Type,int _nSkillID,int _nMySkillIndex,int nSkillLevel);
	int						GetGenLevel(void *pPc,int nSkillID,int AttrGen);	
	CSkillCastValue *		GetCastValue(int _Type,int skillTableIdx,int nLevel=-1);
	int						GetGrade(int pcJob);

	int						GetLimitlevel(int skillTableIdx,int nGenType);
	int						GetGradeClass(int skillTableIdx,int nGenType);
	void					Clear();
	
};

extern CSkillCastData	g_SkillCastDataList;
extern CSkillDataTableList g_SkillDataTableList;

extern CSkillDataGlobal g_SkillDataGlobal;

#endif 
