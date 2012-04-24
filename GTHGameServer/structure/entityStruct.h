
#define	G_PHYSICS								0		
#define G_MENTALITY								1		
#define	G_FIRE									2		
#define G_WATER									3		
#define G_LIGHTNING								4		
#define G_WIND									5		
#define G_POISON								6		
#define G_SOIL									7		

#define GEN_NUMBER								8		
#define GEN_HALF_NUMBER							4		



#define	NAMESTRING						20
#define	DOCUMENTSTRING					255


#include "..\tagObject.h"



typedef enum 
{
	GTH_EV_CHAR_IDLE,				
	GTH_EV_CHAR_WALK,				
	GTH_EV_CHAR_WALKLEFT,			
	GTH_EV_CHAR_WALKRIGHT,			
	GTH_EV_CHAR_BACK,				
	GTH_EV_CHAR_BACKLEFT,			
	GTH_EV_CHAR_BACKRIGHT,			
	GTH_EV_CHAR_RUN,				
	GTH_EV_CHAR_RUNLEFT,			
	GTH_EV_CHAR_RUNRIGHT,			
	GTH_EV_CHAR_TURNLEFT,			
	GTH_EV_CHAR_TURNRIGHT,			
	GTH_EV_CHAR_MOVELEFT,			
	GTH_EV_CHAR_MOVERIGHT,			
	GTH_EV_CHAR_ATTACK,				
	GTH_EV_CHAR_DIE,				
	GTH_EV_CHAR_STUN,				
	GTH_EV_CHAR_DAMAGE,				
	GTH_EV_CHAR_RESPAWN,			
	GTH_EV_CHAR_SKIP,				
	GTH_EV_CHAR_MOUSEWALK,			
	GTH_EV_CHAR_MOUSERUN,			
	GTH_EV_CHAR_SIT,				
	GTH_EV_CHAR_STAND,				
	GTH_EV_CHAR_SKILL,				
	GTH_EV_CHAR_SKILLCASTING,		
	GTH_EV_CHAR_PREATTACK,			
	
	GTH_EV_CHAR_BEFORE_TELEPORT,	
	GTH_EV_CHAR_TELEPORT,			
	
	GTH_EV_CHAR_FREEZE,				
	
	GTH_EV_CHAR_PUSHED,				
	GTH_EV_CHAR_HIDE,				

	
	GTH_EV_CHAR_SHOUT,				
	GTH_EV_CHAR_BOW,				
} i3characterEvent_t;



typedef enum
{
	ENTITY_PC,
	ENTITY_MONSTER,
	ENTITY_NPC,
	ENTITY_OBJECT
} entityType_t;


typedef enum
{
	TRANSFORM_NORMAL		=	0,		
	TRANSFORM_ETHEREAL,					
} transformType_t;



#define MAX_NUMBER_OF_MESSAGE			20

typedef enum
{
	MSG_ATKDATA_ATTACK_TYPE		=	0,
	MSG_ATKDATA_PHYSIC_DMG =1,
	MSG_ATKDATA_WEAPON_TYPE =2,
	MSG_ATKDATA_POSITIVE_DMG =3,
	MSG_ATKDATA_NEGATIVE_DMG =4,
	MSG_ATKDATA_HITRATE=5,

	
	MSG_ATKDATA_HITPOWER =6,

	MSG_ATKDATA_STUN_FLAG = 7,	
	MSG_ATKDATA_CRITICAL_DAMAGE =8,					
	MSG_ATKDATA_CRITICAL_RATE=9,				

	MSG_ATKDATA_ETC_INFO=10,
	MSG_ATKDATA_ETC_DATA_0=11,
	MSG_ATKDATA_ETC_DATA_1=12,

	
	MSG_ATKDATA_ETC_DATA_2=13,					
	MSG_ATKDATA_ETC_DATA_3=14,					
	MSG_SKILL_TABLE_INDEX=15,					


	MAX_NUMBER_OF_MESSAGE_DATA = 23
};





enum
{
	MSG_ATKDATA_STUN_TYPE_NONE,				
	MSG_ATKDATA_STUN_TYPE_CRITICAL,			
	MSG_ATKDATA_STUN_TYPE_SHOCK,			
	MSG_ATKDATA_STUN_TYPE_STUN,				
	MSG_ATKDATA_STUN_TYPE_FREEZE,			
	MSG_ATKDATA_STUN_TYPE_PUSHED,			
};

typedef enum
{
	MSG_AFFDATA_AFFECT_TYPE		=	0,
	MSG_AFFDATA_AFFECT_HITTYPE,
	MSG_AFFDATA_HITRATE,
	MSG_AFFDATA_LEVEL,
	MSG_AFFDATA_KEEPTIME,
	MSG_AFFDATA_SKILLTABLEIDX,

	
	MSG_AFFDATA_RACOST_PER_SEC,
	MSG_AFFDATA_SACOST_PER_SEC,
	MSG_AFFDATA_DELAY_TIME,

	CONVERT_MSG_TO_AFFECT_SIZE,
};

typedef struct
{

	

	int						type;							
	int						fromType;
	int						fromIdx;						
	char					fromNameV2[NAMESTRING+1];

	int						toType;
	int						toIdx;							
	char					toNameV2[NAMESTRING+1];

	int						sendTime;						
	int						receiveTime;					
	int						worldIdx;						
	int						vaildLength;

	
	
	int						repeatNum;						
	int						repeatDelayTime;

	

	
	int						Type[MAX_NUMBER_OF_MESSAGE_DATA];
	int						data[MAX_NUMBER_OF_MESSAGE_DATA];

} message_t;

#define MAX_COUNT_MSGINFO_DATA		16


#define	MAX_COUNT_AFFECT_DATA		5



typedef enum
{
	
	AFFECT_NONE				=	-1,
	AFFECT_ATTACK			=	0,
	AFFECT_DEFENSE,
	AFFECT_ASSIST,
	AFFECT_SPECIAL,
	AFFECT_TOGGLE,
	AFFECT_CURSE,

	NUM_AFFECT_TYPE,
	
} AFFECT_TYPE_t;




struct AFFECTDATA_COMMON
{
	short	type[MAX_COUNT_AFFECT_DATA];
	short	data[MAX_COUNT_AFFECT_DATA];
};

struct AFFECTDATA_SPECIAL
{
	short		specialType;				
	short		specialIdx;					

	short		specialData[8];
};

typedef struct
{
	int				type;				
	
	int				startTime;			
	int				keepTime;			
	int				forcedDelete;		

	int				flagApply;			

	int				level;
	int				skillTableIdx;

	
	short			raCostPerSec;
	short			saCostPerSec;
	short			delayTime;

	union
	{
		int					data[MAX_COUNT_AFFECT_DATA];
		AFFECTDATA_COMMON	common;
		AFFECTDATA_SPECIAL	spe;
	};

} affect_t;



enum
{
	MSGINFO_TYPE_COMMON		=	0,
	MSGINFO_TYPE_ATTACK,
	MSGINFO_TYPE_AFFECT,
};

struct MSGINFO_COMMON
{
	int			data[MAX_COUNT_MSGINFO_DATA];
};

enum
{
	MSGINFO_ATK_TYPE_NORMAL,
	MSGINFO_ATK_TYPE_MAGIC,
	MSGINFO_ATK_TYPE_EFFECT,
	MSGINFO_ATK_TYPE_NONE,
};

enum
{
	MSGINFO_AFF_TYPE_NORMAL,						
	MSGINFO_AFF_TYPE_POSITIVE,
	MSGINFO_AFF_TYPE_NEGATIVE,
	MSGINFO_AFF_TYPE_NONE,
};

enum
{
	
	
	MSGINFO_ATK_ETC_INFO_NONE			=	0,
	MSGINFO_ATK_ETC_INFO_RASTEAL,
	MSGINFO_ATK_ETC_INFO_STUN,						
	MSGINFO_ATK_ETC_INFO_SHOCK,						
	MSGINFO_ATK_ETC_INFO_FREEZE,					
	MSGINFO_ATK_ECT_INFO_PUSHED,					
	MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_NUMBER,	
	
	MSGINFO_ATK_ETC_MULTIDAMAGE_DELAY,				
	MSGINFO_ATK_ETC_INFO_HIDE,						

	MSGINFO_ATK_ETC_DEC_DAMAGE_AS_TARGET_100_ELSE_LITE_DAMAGE, 
	
	MSGINFO_ATK_ETC_RA_RATIO_DAMAGE,		
};

struct MSGINFO_ATTACK
{
	short			atkType;
	
	
	short			hitPower;
	short			hitRate;
	short			phyMinDmg;
	short			phyMaxDmg;
	short			posMinDmg;
	short			posMaxDmg;
	short			negMinDmg;
	short			negMaxDmg;

	short			criticalRate;
	short			criticalDamage;

	short			delayTime;					
	short			repeatNum;
	short			repeatDelayTime;			

	short			etcInfo;
	short			etcData0;
	short			etcData1;
};

struct MSGINFO_AFFECT
{
	short			type;
	short			affectType;

	int				startTime;
	int				keepTime;
	
	byte			level;
	byte			hitRate;
	short			skillTableIdx;

	
	short			raCostPerSec;
	short			saCostPerSec;
	short			delayTime;

	union
	{
		int					data[MAX_COUNT_AFFECT_DATA];
		AFFECTDATA_COMMON	common;
		AFFECTDATA_SPECIAL	spe;
	};
};

typedef struct
{
	int			infoType;
	int			targetType;
	int			targetIdx;
	
	union
	{
		MSGINFO_COMMON	common;
		MSGINFO_ATTACK	attack;
		MSGINFO_AFFECT	affect;
	};

} messageInfo_t;


struct SKILL_GEN_LEVEL
{
	char		gen[GEN_HALF_NUMBER];
};


typedef struct
{
	int			idx;
	int			tableIdx;
	union
	{
		SKILL_GEN_LEVEL		level;
		DWORD				iLevel;
	};

} skill_t;

