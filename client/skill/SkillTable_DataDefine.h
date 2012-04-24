#ifndef SKILL_TABLE_DATADEFINE_H
#define SKILL_TABLE_DATADEFINE_H



#define	MAX_COUNT_AFFECT_DATA			5
#define	MAX_COUNT_AFFECT_SPE_DATA		8
#define MAX_SKILL_CAST_COOL_SIZE		12

#define MAX_SKILL_ATTACK_TYPE			16
#define MAX_SKILL_AFFECT_TYPE			11
#define SKILL_AFFECT_TOGGLE_DEFAULT_DELAY_TIME	1000

#define SAFE_DIVIDE(a,b,c) (a != 0 && b !=0 )  ? ( c = ( a / b )) : c = 0


#define		MAX_SKILL_CHECK_FLAG			5

#define  MAX_SKILL_SUB_ACTION		3
#define  MAX_SKILL_VALUE_LIST		10
#define	 MAX_SKILL_PARTY_CHECK		3


#define		MAX_SKILL_MESSAGE_DATA			6

class CSkillDataValue
{
public:
	CSkillDataValue()
	{
		Init();
	}

	void Init()
	{
		m_Type =0;
		m_nIndex=0;
	}

public:
	BYTE	m_Type;
	int		m_nIndex;
};

class CSkillDataCast
{
public:
	CSkillDataCast()
	{
		Init();
	}

	~CSkillDataCast()
	{
	}

	void Init()
	{
		m_Type =0;
	}
	
public:
	BYTE m_Type;
};



class CSkillDataCommon
{
public:
	CSkillDataCommon()
	{
		Init();
	}

	void Init()
	{
		int i=0;
		for(i=0;i<MAX_COUNT_AFFECT_DATA;i++)
		{
			m_Data[i].Init();
			m_OnceData[i].Init();
		}

		m_bUse=false;
	}

	bool	Use()
	{
		return m_bUse;
	}
	
public:
	bool				m_bUse;
	CSkillDataValue		m_Data[MAX_COUNT_AFFECT_DATA];	
	CSkillDataValue		m_OnceData[MAX_COUNT_AFFECT_DATA];
};


class CSkillDataFormat_Attack
{
public:
	enum
	{
		MAX_SKILL_ATTACK_ETC_DATA=3,
	};
		

	CSkillDataFormat_Attack()
	{
		Init();
	}

	void Init()
	{
		m_AtkType		=0;

		m_HitPower		=0;
		m_HitRate		=0;
		m_PhyMinDmg		=0;
		m_PhyMaxDmg		=0;
		m_PosMinDmg		=0;
		m_PosMaxDmg		=0;
		m_NegMinDmg		=0;
		m_NegMaxDmg		=0;

		m_CriticalRate	=0;
		m_CriticalDamage=0;

		m_DelayTime		=0;
		m_RepeatNum		=0;
		m_RepeatDelayTime=0;

		m_EtcType=0;
		
		for(int i=0;i<MAX_SKILL_ATTACK_ETC_DATA;i++)
		{
			m_EtcData[i].Init();
		}

		m_bUse			=false;
	}

	bool	Use()
	{
		return m_bUse;
	}

public:
	bool			m_bUse;
	BYTE			m_AtkType;
	
	BYTE			m_HitPower;
	BYTE			m_HitRate;
	BYTE			m_PhyMinDmg;
	BYTE			m_PhyMaxDmg;
	BYTE			m_PosMinDmg;
	BYTE			m_PosMaxDmg;
	BYTE			m_NegMinDmg;
	BYTE			m_NegMaxDmg;

	BYTE			m_CriticalRate;
	BYTE			m_CriticalDamage;

	BYTE			m_DelayTime;
	BYTE			m_RepeatNum;
	BYTE			m_RepeatDelayTime;

	BYTE			m_EtcType;

	
	CSkillDataValue	m_EtcData[MAX_SKILL_ATTACK_ETC_DATA];
};





class CSkillDataAffectSpe
{
public:
	CSkillDataAffectSpe()
	{
		Init();
	}
	void Init()
	{
		for(int i=0;i<MAX_COUNT_AFFECT_SPE_DATA;i++)
		{
			m_Data[i].Init();				
		}
		m_bUse=false;

		m_SpecialIdx=0;
		m_SpecialType=0;
		m_IsTransform=0;
		m_TransformEntityIndex=0;
		m_TransformType=0;

		m_bUse=false;
	}

	bool	Use()
	{
		return m_bUse;
	}

public:
	bool				m_bUse;
	BYTE				m_SpecialType;
	BYTE				m_SpecialIdx;

	BYTE				m_IsTransform;
	int					m_TransformEntityIndex;
	BYTE				m_TransformType;

	CSkillDataValue		m_Data[MAX_COUNT_AFFECT_SPE_DATA];
};


class CSkillDataFormat_Affect
{
public:
	CSkillDataFormat_Affect()
	{
		Init();
	}

	void Init()
	{
		m_Type			=0;
		m_AffectType	=0;
		m_StartTime		=0;
		m_KeepTime		=0;
		m_Level			=0;
		m_HitRate		=0;
		m_SkillTableIdx =0;
		m_RaCostPerSec	=0;
		m_SaCostPerSec	=0;
		m_DelayTime		=0;

		m_Group			=0;
		m_GroupLevel	=0;


		m_bUse			=false;
	}

	CSkillDataCommon		*GetCommon()
	{
		return &m_Common;
	}

	CSkillDataAffectSpe		*GetSpe()
	{
		return &m_Spe;
	}

	bool			Use()
	{
		return m_bUse;
	}

public:
	bool	m_bUse;
	BYTE	m_Type;
	BYTE	m_AffectType;
	BYTE	m_StartTime;
	BYTE	m_KeepTime;
	BYTE	m_Level;
	BYTE	m_HitRate;
	BYTE	m_SkillTableIdx;
	BYTE	m_RaCostPerSec;
	BYTE	m_SaCostPerSec;
	BYTE	m_DelayTime;

	BYTE	m_Group;
	BYTE	m_GroupLevel;


	CSkillDataCommon		m_Common;
	CSkillDataAffectSpe		m_Spe;
};





class CSkillData_Message
{
public:
	CSkillData_Message()
	{
		Init();
	}

	~CSkillData_Message()
	{
	}

	void Init()
	{
		m_Type=0;
		m_FromIdx=0;
		m_FromType=0;
		m_Name=0;
		m_ToType=0;
		m_ToIdx=0;
		m_WorldIdx=0;
		m_SendTime=0;
		m_ReceiveTime=0;

		int i;
		for(i=0;i<MAX_SKILL_MESSAGE_DATA;i++)	m_Data[i].Init();
	}
	
public:
	
	BYTE	m_Type;
	BYTE	m_FromIdx;
	BYTE	m_FromType;
	BYTE	m_Name;
	BYTE	m_ToType;
	BYTE	m_ToIdx;
	BYTE	m_WorldIdx;
	BYTE	m_SendTime;
	BYTE	m_ReceiveTime;

	CSkillDataValue  m_Data[MAX_SKILL_MESSAGE_DATA];
};








class CSkillDataSubACtion
{
public:
	CSkillDataSubACtion()
	{
		Init();
	}

	~CSkillDataSubACtion()
	{
	}

	void Init()
	{
		m_Type=0;
		m_fValue=0.0f;
	}
	
public:
	BYTE		m_Type;
	float		m_fValue;
};

#define MAX_SKILL_PARTY_CHECK 3

class CSkillDataParty
{
public:
	CSkillDataParty()
	{
		Init();
	}

	~CSkillDataParty()
	{
	}

	void Init()
	{
		m_Type=0;
		
		for(int i=0;i<MAX_SKILL_PARTY_CHECK;i++)
		{
			m_Check[i]=0;
		}
	}

public:

	BYTE	m_Type;
	BYTE	m_Check[MAX_SKILL_PARTY_CHECK];

};







class CSkillDataFormat
{
public:
	enum
	{
		
		MAX_NUM_OF_DATA=3
	};

	CSkillDataFormat()
	{
		Init();
	}

	void Init()
	{
		m_InfoType		=0;
		m_TargetType	=0;
		m_TargetIdx		=0;
	

		m_MultiSkill	=0;
		m_MessageType	=0;

		m_ClassType		=0;

		m_Attack.Init();
		m_Affect.Init();
		m_Message.Init();

		int i;
		for(i=0;i<MAX_SKILL_CHECK_FLAG;i++)	m_CheckFlag[i] = 0;
		for(i=0;i<MAX_NUM_OF_DATA;i++)			m_Data[i] = 0;

		for(i=0;i<MAX_SKILL_SUB_ACTION;i++)	
		{
			m_SubAtion[i].m_Type = 0;
			m_SubAtion[i].m_fValue = 0;
		}

		for(i=0;i<MAX_SKILL_VALUE_LIST;i++)	
		{
			m_ValueList[i].Init();
		}
	}

	CSkillDataFormat_Attack		*GetAttack()
	{
		return &m_Attack;
	}

	CSkillDataFormat_Affect		*GetAffect()
	{
		return &m_Affect;
	}

	CSkillData_Message			*GetMessage()
	{
		return &m_Message;
	}

	CSkillDataCast				*GetCast()
	{
		return &m_Cast;
	}

	CSkillDataValue	 *GetValue(int index)
	{
		if(index <0  || index >= MAX_SKILL_VALUE_LIST ) return NULL;

		return &m_ValueList[index];
	}

	bool	DeleteValue(int index)
	{
		if(index <0  || index >= MAX_SKILL_VALUE_LIST ) return false;
		m_ValueList[index].Init();

		for(int i=index;i<MAX_SKILL_VALUE_LIST-1;i++)
		{
			m_ValueList[i] = m_ValueList[i+1];
		}

		m_ValueList[MAX_SKILL_VALUE_LIST-1].Init();
		return true;
	}

	void	ClearValueList()
	{
		for(int i=0;i<MAX_SKILL_VALUE_LIST;i++)
		{
			m_ValueList[i].Init();
		}		
	}

	bool	InsertValue(CSkillDataValue &value)
	{
		for(int i=0;i<MAX_SKILL_VALUE_LIST;i++)
		{
			if(!m_ValueList[i].m_Type)
			{
				m_ValueList[i] = value;
				return true;
			}
		}
		return false;
	}

	CSkillDataParty * GetParty()
	{
		return &m_Party;
	}

public:
	BYTE				m_InfoType;
	BYTE				m_TargetType;
	BYTE				m_TargetIdx;

	BYTE				m_ClassType; 

	BYTE				m_MessageType;;
	BYTE				m_MultiSkill;	
	BYTE				m_CheckFlag[MAX_SKILL_CHECK_FLAG];

	
	BYTE						m_Data[MAX_NUM_OF_DATA];

	CSkillDataValue				m_ValueList[MAX_SKILL_VALUE_LIST];
	CSkillDataSubACtion			m_SubAtion[MAX_SKILL_SUB_ACTION];
	CSkillDataFormat_Attack		m_Attack;
	CSkillDataFormat_Affect		m_Affect;
	CSkillData_Message			m_Message;
	CSkillDataCast				m_Cast;
	CSkillDataParty				m_Party;
};




class CSkillCastValue
{
public:
	CSkillCastValue()
	{
		Init();
	}

	void Init()
	{
		m_nIndex=0;
		m_nSa_Comsumption=0;
		m_fCasting_Time=0;
		m_fCool_Time=0.0f;
	}

public:
	int		m_nIndex;
	int		m_nSa_Comsumption;
	float	m_fCasting_Time;
	float	m_fCool_Time;
};


class CSkillCastData
{
public:
	CSkillCastData()
	{
		
	}

	~CSkillCastData()
	{
		Clear();
	}
	
	bool Insert(int nID, CSkillCastValue *pTemp)
	{
		int _Type = nID / 100;
		if(_Type < 0 || _Type >= MAX_SKILL_NUMBER) return false;

		m_List[_Type].push_back(pTemp);
		return true;
	}

	CSkillCastValue * Get(int nID)
	{
		
		int Type = nID / 100;
		int index = (nID % 100)-1;

		if(Type < 0 || Type >= MAX_SKILL_NUMBER) return NULL;
		if(index <0 || index >= m_List[Type].size()) return NULL;

		return m_List[Type][index];
	}

	void Clear()
	{
		for(int i=0;i<MAX_SKILL_NUMBER;i++)
		{
			int size = m_List[i].size();

			for(int j=0;j<size;j++)
			{
				CSkillCastValue *temp = m_List[i][j];
				SAFE_DELETE(temp);
			}

			m_List[i].clear();
		}
	}

public:
	vector <CSkillCastValue *> m_List[MAX_SKILL_NUMBER];
};







class CSkillTableData
{
public:
	CSkillTableData()
	{
		m_nLine		=0;
		m_bType		=0;
		m_nIndex	=0;
		m_fValue	=0;
		m_bGenGrade	=0;
		m_bGenCapablity=0;
		m_bMaxLv	=0;
	}

public:
	int			m_nLine;
	BYTE		m_bType;
	int			m_nIndex;
	float		m_fValue;
	BYTE		m_bGenGrade;
	BYTE		m_bGenCapablity;
	BYTE		m_bMaxLv;
};


class CSkillDataTableList
{
public:
	
	CSkillDataTableList()
	{

	}

	~CSkillDataTableList()
	{
		Clear();
	}
	
	bool Insert(int _Type, CSkillTableData *pTemp)
	{
		if(_Type < 0 || _Type >= NUM_SKILL_TYPE) return false;

		m_List[_Type].push_back(pTemp);
		return true;
	}

	CSkillTableData * Get(int Type,int index)
	{
		if(Type < 0 || Type >= NUM_SKILL_TYPE) return NULL;
		if(index <0 || index >= m_List[Type].size()) return NULL;

		return m_List[Type][index];
	}

	void Clear()
	{
		for(int i=0;i<NUM_SKILL_TYPE;i++)
		{
			int size = m_List[i].size();

			for(int j=0;j<size;j++)
			{
				CSkillTableData *temp = m_List[i][j];
				SAFE_DELETE(temp);
			}

			m_List[i].clear();
		}
	}

public:

	vector <CSkillTableData *> m_List[NUM_SKILL_TYPE];
};


#endif