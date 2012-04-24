#ifndef _SKILL_LOGIC_H
#define _SKILL_LOGIC_H




#define		MAX_COUNT_AUTO_OBTAIN_SKILL		200

enum
{
	SKILL_VALUE_0		=	3,
	SKILL_VALUE_1		=	4,
	SKILL_VALUE_2		=	5,
	SKILL_VALUE_3		=	6,
	SKILL_VALUE_4		=	7,
	SKILL_VALUE_5		=	8,
	SKILL_VALUE_6		=	9,
};

struct SKILLSTATUS
{
	int		level;
	int		tableIdx;
};

struct AUTO_OBTAIN_SKILL
{
	SKILLSTATUS		status[MAX_COUNT_AUTO_OBTAIN_SKILL];
	int				count;
};

typedef struct
{
	
	AUTO_OBTAIN_SKILL		autoObtainSkill[GEN_NUMBER];

} skillObtain_t;


class CSkillLogic
{
public:
	CSkillLogic()
	{
	}

	~CSkillLogic()
	{
	}

	void	SKILL_Empty( void* pPc, float* pValue );
	void	SKILL_MakeAutoObtain();
	int		SKILL_IsValid( playerCharacter_t* pc, int skillIdx );
	void	SKILL_GetAutoObtain( playerCharacter_t *pc, int newLevel, int skillIdx[], int &num );
	void	SKILL_GetInitSkillLevel( int skillTableIdx, SKILL_GEN_LEVEL *level );
	int		SKILL_Add( playerCharacter_t *pc, int skillTableIdx );
	int		SKILL_LevelUp( playerCharacter_t* pc, int idx, int genType );
	void	SKILL_SendMessageMultiTarget( playerCharacter_t *pc, messageInfo_t *msginfo, int targetNumber, int dupflag, int delay );
	void	SKILL_GetValue( playerCharacter_t *pc, int skillIdx, float value[] );
	int		SKILL_CheckPassive( playerCharacter_t* pc, int skillIdx );
	void	SKILL_ApplyPassive( playerCharacter_t* pc );
	int		SKILL_CheckActive( playerCharacter_t* pc, int skillIdx );
	void	SKILL_ApplyActive( playerCharacter_t* pc, int skillIdx, int &coolTime );
	CSkillCastValue *	SKILL_GetValueNotOwnSkill( int skillTableIdx, int level);

	
	void	SKILL_ApplyActiveNotOwnSkill( playerCharacter_t *pc, int skillTableIdx, int skillLevel );
	void	SKILL_Monster_ApplyActive( monsterCharacter_t* pMonster, int skillIdx, int& coolTime );
	void	SKILL_Monster_GetValue( monsterCharacter_t *pMonster, int tableIdx, float value[] );
		

	void	GTH_ProcessMessage_Request_Use_Potion();

	
	void	GTH_UseSkillItem_DeleteSend(int invenPos);
public:

};


#ifndef _SKILL_DEFINE_
#define _SKILL_DEFINE_

#define MAX_COUNT_SKILL_MULTI_TARGET		8

typedef struct
{
	int				targetNumber;
	int				targetType[MAX_COUNT_SKILL_MULTI_TARGET];
	int				targetIdx[MAX_COUNT_SKILL_MULTI_TARGET];
} skillReturnData_t;

struct SKILLFUNC
{
	void (*pFunc)( void* pc, float* pParam );
};

#endif

extern skillReturnData_t	g_skillMultiTarget;
extern float				g_tempCoolTime;
extern int					g_affectSkillLevel;


extern CSkillLogic			g_SkillLogic;


#endif