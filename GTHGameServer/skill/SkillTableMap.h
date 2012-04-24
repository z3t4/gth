



#if !defined(AFX_SKILLTABLEMAP_H__43AFC84D_AEC8_4BD1_9965_C61D49301226__INCLUDED_)
#define AFX_SKILLTABLEMAP_H__43AFC84D_AEC8_4BD1_9965_C61D49301226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CSkillTableMap
{
	private:
public:
	CSkillTableMap();
	~CSkillTableMap();

	bool			SAFE_ARRAY(int index);

	void			ClearDB();
	CSkilTable *	FindDB(int nID);
	void			InsertDB(int nID, CSkilTable *pData);

	CSkilTable *	m_SkillDB[MAX_SKILL_NUMBER];

};		


extern CSkillTableMap g_SkillTableDB;
#endif 
