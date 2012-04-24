#ifndef _SKILL_TABLE_DEFINE
#define _SKILL_TABLE_DEFINE








#define SKILL_PARTYGEN_LEN		8
#define SKILL_VALUE_NAME_LEN	128
#define SKILL_NAME_LEN			64
#define SKILL_DESC_LEN			128
#define SKILL_UNRESERVED_LEN	127
#define SKILL_EFFECT_NOTDEFINED 10

class CSkillEffect
{
public:
	CSkillEffect()
	{

	}

	void Init()
	{
		m_nClassType=0;
		m_nEffectID=0;
		
		memset(&m_nEffectLevel,0,sizeof(m_nEffectLevel));
		m_fFraction=0.0f;
		m_nZpos=50;

		m_nTarget=0;
		m_nActivity=0;	

		m_nIsSubEffect=0;
		m_nSubEffectID=0;

		memset(&m_nNotDefined,0,sizeof(m_nNotDefined));
	}

public:
	int				m_nClassType;
	int				m_nEffectID;
	int				m_nEffectLevel[MAX_COUNT_EFFECT_LEVEL];
	float			m_fFraction;
	int				m_nZpos;

	int				m_nTarget;
	int				m_nActivity;	

	int				m_nIsSubEffect;
	int				m_nSubEffectID;

	int				m_nNotDefined[SKILL_EFFECT_NOTDEFINED];
} ;


class CSkillRequire
{
public:
	CSkillRequire()
	{
		Init();
	}

	void Init()
	{
		m_nGenLevel=0;	
		m_nBaseLevel=0;	

		memset(&m_nSkillID,0,sizeof(m_nSkillID));

		m_nItemType	=-1;
		m_nItem=-1;	
		m_nLength=0;

		for(int i=0;i<SKILL_PARTYGEN_LEN;i++)	m_nPartyGen[i]=-1;
	}
	
public:
	int				m_nGenLevel;	
	int				m_nBaseLevel;	
	int				m_nSkillID[NUM_OF_REQUIRE_SKILL];	

	int				m_nItemType;	
	int				m_nItem;		
	int				m_nLength;
	int				m_nPartyGen[SKILL_PARTYGEN_LEN];	
};

class CSkilValue
{	
public:
	CSkilValue()
	{
		Init();
	}

	void Init()
	{
		m_nType		=0;
		m_nInitial	=0;
		m_nLevelUp	=0;
		m_nMaxLevel	=0;
		m_nGenGradeType	=0;
		m_nGenGradeClass=0;
		
		
		strcpy(m_chName, "¾øÀ½");
	}
	
public:
	BYTE			m_nType;
	float			m_nInitial;	
	float			m_nLevelUp;	
	int				m_nMaxLevel;
	int				m_nGenGradeType;
	int				m_nGenGradeClass;
	char			m_chName[SKILL_VALUE_NAME_LEN];	
};



class CSkilTable
{
public:
	CSkilTable()
	{
		Init();
	}

	void Init()
	{
		m_nId				=-1;			
		m_NameIdx			=0;		
		m_nIconID			=-1;		
		m_nSkillClass		=0;	
		m_DescIdx			=0;		
		m_nUsageType		=0;		
		m_nJobType			=0;			
		m_nAutoObtainFlag	=0;	
		m_nAnimID			=0;			

		
		m_nAttackSkillFlag	=0;	
		m_nAttackAfterFlag	=0;
		m_nTargetType		=0;		
		m_nNumValue			=NUM_OF_SKILL_VALUE;
		m_nSphereCost		=0;
		m_nCastCoolIndex	=-1;

		memset(&m_nUnreserved,0,sizeof(m_nUnreserved));

		m_seEffect.Init();
		m_sqRequire.Init();
		m_DataFormat.Init();
	}

	CSkillEffect		*	GetSkillEffect()
	{
		return &m_seEffect;
	}

	CSkillRequire *  GetSkillRequire()
	{
		return &m_sqRequire;
	}


	CSkillDataFormat		*GetDataFormat()
	{
		return &m_DataFormat;
	}

public:

	
	int						m_nId;								
	int						m_NameIdx;							
	int						m_nIconID;							
	int						m_nSkillClass;						
	int						m_DescIdx;							
	int						m_nUsageType;						
	int						m_nJobType;							
	int						m_nAutoObtainFlag;					
	int						m_nAnimID;							
	
	
	int						m_nAttackSkillFlag;
	int						m_nAttackAfterFlag;
	int						m_nTargetType;						
	int						m_nNumValue;
	int						m_nSphereCost;
	int						m_nUnreserved[SKILL_UNRESERVED_LEN];

	int						m_nCastCoolIndex;					

	
	CSkillEffect			m_seEffect;		
	CSkillRequire			m_sqRequire;
	CSkillDataFormat		m_DataFormat;

};

#endif