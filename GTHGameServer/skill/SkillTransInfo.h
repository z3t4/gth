



#if !defined(AFX_SKILLTRANSINFO_H__E2BEA656_07D6_4B99_AA4B_160158A63005__INCLUDED_)
#define AFX_SKILLTRANSINFO_H__E2BEA656_07D6_4B99_AA4B_160158A63005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CSkillTransformUnit
{
public:
	CSkillTransformUnit()
	{
		Init();
	}

	~CSkillTransformUnit()
	{
	}

	void	Init()
	{
		m_nSkillID =-1;
		m_nTransformEntityIdx=-1;
		m_nEffectID=-1;
	}

public:
	int		m_nSkillID;
	int		m_nTransformEntityIdx;
	int		m_nEffectID;
};

class CSkillTransformData
{
public:
	CSkillTransformData()
	{
	}

	~CSkillTransformData()
	{
	}

	CSkillTransformUnit *	Get(int Index);
	void					Insert(CSkillTransformUnit *temp);
	void					Clear();
	int						GetEffectID(const int _EntityIdx);

public:
	vector <CSkillTransformUnit *> m_TransData;
};

class CSkillTransInfo  
{
public:
	CSkillTransformData	m_TransList;

public:
	CSkillTransInfo();
	virtual ~CSkillTransInfo();

	CSkillTransformData	* Get() 
	{
		return &m_TransList;
	}	
};

extern CSkillTransInfo m_SkillTransInfo;

#endif 
