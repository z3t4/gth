 
 
 
 
 
 
 
 
 
#ifndef	_COMMON_H_JF02J3PPW02990U3042
#define	_COMMON_H_JF02J3PPW02990U3042

#include <windows.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <D3DX8.h>
#include <DXErr8.h>
#include <tchar.h>

#include "../common/D3DApp.h"
#include "../common/D3DUtil.h"
#include "../common/DXUtil.h"
#include "../resource.h"

 
 
 

#include "ObjectDefs.h"
#include "FX_Etcs/RenderRelated.h"

	 
#define		MAX_CHAR_A_SCENE			50			 

#define		MAX_CREATURE_A_SCENE		50

#define		MAX_COUNT_BONE_IN_CHAR		50				

#define		MAX_COUNT_SKEL				200				 

#define		MAX_COUNT_ANIM_VER100		200			 
#define		MAX_COUNT_ANIM_VER110		500			
	
#define		FILESIZE_LOOKUP_VER100		140000
#define		FILESIZE_LOOKUP_VER110		150000

#define		MAX_COUNT_ANIM				2000		 




#define		MAX_COUNT_SKIN_MESH			800			 
#define		MAX_COUNT_ITEM_MESH			500			 




#define		MAX_COUNT_TEXTURE			1000		 

#define		MAX_COUNT_CHAR_PER_ITEM		100			 
#define		MAX_COUNT_AVAIL_ITEM		200			 
	
#define		MAX_COUNT_BONE				150			  
#define		MAX_COUNT_EFFECT_TEXTURE	50			  

#define		RESOURCE_VERSION_VALUE			101
#define		RESOURCE_VERSION_TEXT			"101"

	 
#define		FILE_BASE					_T("./")

#define		FILENAME_BASE				_T("listdata/")
#define		FILENAME_SKEL_LIST			FILENAME_BASE	_T("SkelList.fxd")
#define		FILENAME_ANIM_LIST			FILENAME_BASE	_T("AnimList.fxd")
#define		FILENAME_ITEM_LIST			FILENAME_BASE	_T("ItemList.fxd")
#define		FILENAME_SKIN_LIST			FILENAME_BASE	_T("SkinList.fxd")
#define		FILENAME_TEX_LIST			FILENAME_BASE	_T("TexList.fxd")
#define		FILENAME_AURA_LIST			FILENAME_BASE	_T("AuraList.fxd")
#define		FILENAME_SKINITEM_LIST		FILENAME_BASE	_T("ItemForSkin.fxd")
#define		FILENAME_OBJCLASS_LIST		FILENAME_BASE	_T("ObjClassList.fxd")
#define		FILENAME_EFFECT_TEX_LIST	FILENAME_BASE	_T("effect_texture.fxd")

#define		FILENAME_SKIN_HAIR			FILENAME_BASE	_T("SkinHairList.fxd")
#define		FILENAME_SKIN_HEAD			FILENAME_BASE	_T("SkinHeadList.fxd")
#define		FILENAME_SKIN_UPPER			FILENAME_BASE	_T("SkinUpperList.fxd")
#define		FILENAME_SKIN_LOWER			FILENAME_BASE	_T("SkinLowerList.fxd")
#define		FILENAME_SKIN_HAND			FILENAME_BASE	_T("SkinHandList.fxd")
#define		FILENAME_SKIN_LEG			FILENAME_BASE	_T("SkinLegList.fxd")
#define		FILENAME_SKIN_FOOT			FILENAME_BASE	_T("SkinFootList.fxd")
#define		FILENAME_SKIN_MONSTER		FILENAME_BASE	_T("SkinMonsterList.fxd")
	
#define		INTERNAL_INFO				_T("internal_infos/")
#define		FILENAME_TAGLIST			INTERNAL_INFO	_T("TagListExt_")	RESOURCE_VERSION_TEXT	_T(".dat")
#define		FILENAME_MESHLIST			INTERNAL_INFO	_T("MeshListExt.dat")
#define		FILENAME_BONELIST			INTERNAL_INFO	_T("BoneListExt.dat")
#define		FILENAME_CHARTABLE			INTERNAL_INFO	_T("CharLookupTable.bdt")
#define		FILENAME_SIN_TABLE			INTERNAL_INFO	_T("fsintbl.bdt")
#define		FILENAME_COS_TABLE			INTERNAL_INFO	_T("fcostbl.bdt")
#define		FILENAME_ATTACKMOTION_INFO	INTERNAL_INFO	_T("AttackMotionInfo.dat")

#define		FILENAME_SKINDFT_LIST		_T("defaultchar.cfg")			 

	 
#define		FX_ANIM_ERROR			0.0001f

	 
#define		VERTEX_BUFFER_SIZE			7000
#define		INDEX_BUFFER_SIZE			21000

#define		MAX_NUM_STATIC_EFFECT		10				 

	 
#define		SIZE_PREVB_AURA		3000
#define		SIZE_PREIB_AURA		6000

#ifdef	_FXSKELDRAW
#	define		SIZE_VB_SKEL_RENDER		400
#	define		SIZE_IB_SKEL_RENDER		36
#endif

#define		GTH_CHAR_FORMAT			( GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_TEXVERTEX( 1 ) )
#define		GTH_AURA_FORMAT			( GTH_FORMAT_XYZ | GTH_FORMAT_DIFFUSE | GTH_FORMAT_TEXVERTEX( 1 ) )
#ifdef	_FXSKELDRAW
#	define		GTH_SKEL_FORMAT		( GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL )
#endif
#define		FX_DEFAULT_SKIN_PART_NUMBER	4

#define		FX_MAX_EQUIPMENT_NUMBER		7

#define		NUM_GEN_LOOKUPS		8			 
#define		NUM_MON_LOOKUPS		200			 

#define		SIZE_BONENAME			32
#define		MAX_CHILDBONE			8
#define		MAX_ROOTBONE_ID			8
#define		MAX_TAG					35

#define		MAX_AURA_IN_CHAR		6
#define		AURA_MAX_DIVIDE			10
#define		AURA_NUM_DIVIDE			4		 
#define		AURA_UPDATE_TIME		0.05f

#define		ANIM_FRAME_ERROR_VALUE	9999999.9f

#define		COMBO_DAMAGE_REQUEST_FRAME_OFFSET	5		 

 
 
 

const	float	INTERPOLATE_FRACTION			=	0.4f;

 
enum
{
	MON_ALIVE_STATE_SPAWN		= 0,
	MON_ALIVE_STATE_NORMAL,
	MON_ALIVE_STATE_CORPSE,
	MON_ALIVE_STATE_DISAPPEAR,
	MON_ALIVE_STATE_DIE,
	MON_ALIVE_STATE_FADE,
};

enum
{
	CHAR_ATTACK_PREPARE	=	0,
	CHAR_ATTACK_ENABLE,
	CHAR_ATTACK_DISABLE,

	CHAR_ATTACK_FORCEDWORD	=	0x7fffffff
};

#define		CHAR_ATTACK_SUCCEED		true
#define		CHAR_ATTACK_FAILED		false

enum
{
	CHAR_SOUND_ENABLE,
	CHAR_SOUND_DISABLE,

	CHAR_SOUND_FORCEDWORD	=	0x7fffffff
};

enum
{
	 
	CHAR_SHADER_AURA_1	=	0,
	CHAR_SHADER_AURA_2,
	CHAR_SHADER_AURA_3,
	CHAR_SHADER_AURA_4,
	CHAR_SHADER_CHAR_SELECT_NORMAL,
	CHAR_SHADER_CHAR_SELECT_HOSTILE,
	CHAR_SHADER_CHAR_SELECT_3,
	CHAR_SHADER_CHAR_SELECT_4,
	CHAR_SHADER_ITEM_SELECT_1,
	CHAR_SHADER_ITEM_SELECT_2,
	CHAR_SHADER_ITEM_SELECT_3,
	CHAR_SHADER_ITEM_SELECT_4,
	CHAR_SHADER_MONSTER_DEAD_1,
	
	CHAR_SHADER_FORCEDWORD		=	0x7fffffff	
};

enum
{	 
	INTERPOLATE_NORMAL			=	0,
	INTERPOLATE_USE_CURFRAME,
	INTERPOLATE_USE_EOA,

	INTERPOLATE_FORCEDWORD		=	0x7fffffff
};

enum
{	 
	ANIMOPTION_NORMAL			=	0,
	ANIMOPTION_START_NEW_ANIM,
	ANIMOPTION_COMBO_ATTACK,
	ANIMOPTION_START_ATTACK,
	ANIMOPTION_ACCELERATE_ANIM,			 

	ANIMOPTION_FORCEDWORD		=	0x7fffffff
};

	 
enum
{	ANIMCHECK_NORMAL,
	ANIMCHECK_EOP,
	ANIMCHECK_EOA,
	ANIMCHECK_RESULT_NORMAL,
	ANIMCHECK_RESULT_END,
	ANIMCHECK_RESULT_EOP,
	ANIMCHECK_RESULT_EOA,

	ANIMCHECK_FORCEDWORD		=	0x7fffffff
};

	 
enum
{
	SKIN_PART_HAIR,
	SKIN_PART_HEAD,
	SKIN_PART_UPPER,
	SKIN_PART_LOWER,
	SKIN_PART_HAND,
	SKIN_PART_LEG,
	SKIN_PART_FOOT,
	SKIN_PART_MONSTER,
	MAX_COUNT_SKIN_PART,
	SKIN_PART_FORCEDWORD		=	0x7fffffff
};

	 
enum
{
	ITEM_PART_OVERHEAD_FAR,			
	ITEM_PART_OVERHEAD_NEAR,		
	ITEM_PART_ARM_R,				
	ITEM_PART_ARM_L,				
	ITEM_PART_NECK,					
	ITEM_PART_BODY,					
	ITEM_PART_HAND_R,				
	ITEM_PART_HAND_L,				
	ITEM_PART_BACKHAND_R,			
	ITEM_PART_BACKHAND_L,			
	ITEM_PART_KNEE_R,				
	ITEM_PART_KNEE_L,				
	ITEM_PART_MIDDLEAIR_R,			
	ITEM_PART_MIDDLEAIR_L,			
	ITEM_PART_WAIST,				
	ITEM_PART_SHOULDER,				
	ITEM_PART_BACK,					
	ITEM_PART_WAIST_R,				
	ITEM_PART_WAIST_L,				
	ITEM_PART_ELBOW_R,				
	ITEM_PART_ELBOW_L,				
	ITEM_PART_HAIR,					
	ITEM_PART_HEAD,					
	ITEM_PART_FACE,					
	ITEM_PART_TRANS,				
	ITEM_PART_GROUNDAIR_R,			
	ITEM_PART_GROUNDAIR_L,			
	ITEM_PART_GROUND,				
	ITEM_PART_RESERVED,				
	ITEM_PART_CREATURE_TYPE_0,		
	ITEM_PART_CREATURE_TYPE_1,		
	ITEM_PART_CREATURE_TYPE_2,		
	ITEM_PART_CREATURE_TYPE_3,		
	ITEM_PART_CREATURE_TYPE_4,		
	MAX_COUNT_ITEM_PART,
	ITEM_PART_FORCEDWORD		=	0x7fffffff
};

typedef enum
{
	FX_ANIM_GROUP_NULL	=	-1,
	FX_ANIM_GROUP_MOVE	=	0,
	FX_ANIM_GROUP_BASIC,
	FX_ANIM_GROUP_ATTACK,
	FX_ANIM_GROUP_DEFENSE,
	FX_ANIM_GROUP_MAGIC,
	FX_ANIM_GROUP_SOCIAL,

	FX_ANIM_GROUP_FORCEDWORD	=	0x7fffffff
}	Fx_ANIM_GROUP_t, *Fx_ANIM_GROUP_pt;

typedef union
{
	struct
	{
		unsigned	changable	:1;
		unsigned	repeatable	:1;
		unsigned	movable		:1;
		unsigned	alive		:1;
	}		type;
	DWORD	value;

}	Fx_ANIM_STATE_t, *Fx_ANIM_STATE_pt;

typedef struct
{
	
	int					groupID;
	Fx_ANIM_STATE_t		state;

} Fx_ANIM_INFO_t, *Fx_ANIM_INFO_pt;

	 
struct	Fx_CHARACTER_t;

typedef struct
{
	int				meshID;
	int				count;
	Fx_CHARACTER_t	*characterPtr[ MAX_COUNT_CHAR_PER_ITEM ];
	int				tagID[ MAX_COUNT_CHAR_PER_ITEM ];
	int				parentTagID[ MAX_COUNT_CHAR_PER_ITEM ];
} CHAR_BUFFER_t;

enum
{
	GEN_TYPE_SPIRIT	=	0,
	GEN_TYPE_WIND,
	GEN_TYPE_WATER,
	GEN_TYPE_SOIL,
	GEN_TYPE_POISON,
	GEN_TYPE_IRON,
	GEN_TYPE_FIRE,
	GEN_TYPE_BODY,
	NUM_GEN_TYPE,
};

	 
class	FX_CCharManager;
class	FX_CSkinManager;
class	FX_CSkelManager;
class	FX_CAnimManager;
class	FX_CItemManager;
class	FX_CAuraManager;
class	FX_CCharLookupMng;
class	FX_CCharShaderMng;

class	FX_CItemRenderer;
class	FX_CSkinRenderer;
class	FX_CAuraRenderer;
class	FX_CAnim;
class	FX_CMesh;
class	FX_CSkel;

class	CTagManager;
class	CMeshManager;
class	CBoneManager;

typedef enum
{
	SKIN_HAIR	=	0x00,
	SKIN_HEAD,
	SKIN_UPPER,
	SKIN_LOWER,
	SKIN_HAND,
	SKIN_LEG,
	SKIN_FOOT,
	SKIN_MONSTER,

	ITEM_OVERHEAD	=	0x10,
	ITEM_ARMOR,
	ITEM_AIR,
	ITEM_WEAPON,

	TYPE_DONTCARE	=	-1,
	FORCEDWORD	=	0x7fffffff

}	MESHTYPE_t,	*MESHTYPE_pt;

 
typedef struct
{
	union
	{
		struct
		{
			unsigned	spirit	:1;
			unsigned	wind	:1;
			unsigned	water	:1;
			unsigned	soil	:1;
			unsigned	poison	:1;
			unsigned	iron	:1;
			unsigned	fire	:1;
			unsigned	body	:1;
			unsigned			:0;
		}	bit;
		unsigned int	value;
	}	gen;

	MESHTYPE_t	type;

}	MESHINFO_t,	*MESHINFO_pt;

	 
typedef struct
{
	char		desc [256];
	MESHINFO_t	meshinfo;
}	MESH_FILEINFO;

typedef struct
{
	int		sub [4];
}	FX_BONESTRUCT_ID;

	 
typedef struct
{
	int		vertexCount;
	int		ibStart;
	int		ibNumber;
	int		vbStart;
	int		vbNumber;
}	FX_BUFFERINDEX_t;

	 
enum
{
	ANIM_ITEM_IDLE			= 0,
	ANIM_ITEM_WALK,
	ANIM_ITEM_WALK_BACK,
	ANIM_ITEM_WALK_LEFT,
	ANIM_ITEM_WALK_RIGHT,
	ANIM_ITEM_RUN,
	ANIM_ITEM_PRE_ATTACK,
	ANIM_ITEM_ATTACK_0,
	ANIM_ITEM_ATTACK_1,
	ANIM_ITEM_ATTACK_2,
	ANIM_ITEM_ATTACK_3,					 
	ANIM_ITEM_ATTACK_4,
	ANIM_ITEM_DEFENSE,
	ANIM_ITEM_AVOID,
	ANIM_ITEM_RECOVERY,
	ANIM_ITEM_ATTACKED_0,
	ANIM_ITEM_ATTACKED_1,
	ANIM_ITEM_DEAD_0,
	ANIM_ITEM_DEAD_1,
	ANIM_ITEM_DYING_0,
	ANIM_ITEM_DYING_1,					 
	ANIM_ITEM_FLY,
	ANIM_ITEM_SIT,
	ANIM_ITEM_STAND,
	ANIM_ITEM_LIE,
	ANIM_ITEM_EVENT,
	ANIM_ITEM_SOCIAL,
	ANIM_ITEM_JUMP,
	ANIM_ITEM_SIT_IDLE,
	ANIM_ITEM_SKILLCASTING,
	ANIM_ITEM_CASTINGIDLE,				 

	ANIM_ITEM_SOCIAL_BOW,
	ANIM_ITEM_SOCIAL_SHOUT,	


										 
	
	MAX_ANIMCOUNT_TYPE		= 50,
};

	 
enum
{
	ANIM_COMMON_COMMON_0		= 0,
	ANIM_COMMON_COMMON_1,
	ANIM_COMMON_COMMON_2,
	ANIM_COMMON_COMMON_3,
	ANIM_COMMON_COMMON_4,
	ANIM_COMMON_COMMON_5,
	ANIM_COMMON_COMMON_6,
	ANIM_COMMON_COMMON_7,
	ANIM_COMMON_COMMON_8,
	ANIM_COMMON_COMMON_9,

	MAX_ANIMCOUNT_COMMON	= 100
};

	 
enum
{
	ANIM_SKILL_SKILL_0	=	0,
	ANIM_SKILL_SKILL_1,
	ANIM_SKILL_SKILL_2,
	ANIM_SKILL_SKILL_3,
	ANIM_SKILL_SKILL_4,
	ANIM_SKILL_SKILL_5,
	ANIM_SKILL_SKILL_6,
	ANIM_SKILL_SKILL_7,
	ANIM_SKILL_SKILL_8,
	ANIM_SKILL_SKILL_9,
	
	MAX_ANIMCOUNT_SKILL	= 200,
};

	 
typedef struct
{
	int		anim_type [MAX_ANIMCOUNT_TYPE];
	char	desc[256];
}	ANIM_t;

	 
#define	MAX_ANIMCOUNT_ITEM		10

	 
typedef struct
{
	ANIM_t		anim_item	[MAX_ANIMCOUNT_ITEM];
	int			anim_common [MAX_ANIMCOUNT_COMMON];
	char		commonDesc[256];
	int			anim_skill	[MAX_ANIMCOUNT_SKILL];
	char		skillDesc[256];
}	ANIMSET_t;

	 
typedef enum
{
	ANIMTYPE_BYITEM	=	0,
	ANIMTYPE_BYCOMMON,
	ANIMTYPE_BYSKILL,
}	Fx_CHAR_ANIMTYPE_t;

	 	
typedef struct
{
	int			boneIdx;
	ANIMSET_t	animset;
}	Fx_CHAR_LOOKUP_t;

	 
typedef	struct
{
	int			boneIdx;
	int			anim_monster [MAX_ANIMCOUNT_TYPE];
	char		desc[256];
}	Fx_MON_LOOKUP_t;

	 	
typedef struct
{
	float			frame;
	vec3_t			vecOffset;			
	vec4_t			quatRotate;			 
										 
	vec4_t			quatCoordinate;		 
} KEYFRAME;

			
					
typedef struct
{
	INT				iBoneID;			
	INT				iNumKeyframe;		
	FLOAT			fTimePerKeyframe;	
	KEYFRAME*		pKeyframe;			
} TRACK;

typedef union
{
	struct
	{
		unsigned	subtrack	:1;
		unsigned				:0;
	}	bit;
	unsigned int	value;
}	ANIM_PROPERTY_t;

typedef	struct
{
	vec4_t				xformed_start;
	vec4_t				xformed_end;					 
	DIFFUSETEXVERTEX	start	[MAX_AURA_IN_CHAR];
	int					num_used;
	float				height;							 
	DIFFUSETEXVERTEX	end		[MAX_AURA_IN_CHAR];
	int					shaderIdx;
	float				nextUpdate;						 
	float				correction;
	bool				made;
	bool				render;
}	AURA_t;

typedef struct
{
	bool		used;
	WORD		defenderIdx;
	WORD		defenderType;
	WORD		attackerIdx;		 
	WORD		attackerType;		 
	WORD		attackType;
	WORD		damage;
	DWORD		identifier;
	bool		stun;
	bool		expired;
}	ATTACKINFO_t;

#define	MAX_COUNT_ATTACK_INFOS		10

enum
{
	CHAR_COLOR_RED	=	0,
	CHAR_COLOR_GREEN,
	CHAR_COLOR_BLUE,
	CHAR_COLOR_YELLOW,
	CHAR_COLOR_CYAN,
	CHAR_COLOR_MAGENTA,
	CHAR_COLOR_MAX
};

struct	Fx_CHARACTER_t
{
	entityType_t			entityType;							 
	int						tableIdx;							
	
	int						pcJob;								
	int						targetIdx;							 
	int						targetID;							 
	vec3_t					position;							 
	int						targetType;
	vec3_t					angles;								 
	int					 	ID;									 
	int						useInterpolation;					 
	i3characterEvent_t		oldEvent;							 
	int						skinPart [FX_DEFAULT_SKIN_PART_NUMBER];			 

#ifdef	_FX_CLIENT
	int						idxOnServer;
	int						equipNumber;									 
	char					name [64];
	//lucky 2012 new GM system
	int						gameMaster;
	//end
	char					boothName[64];
	int						boothState;

	int						equipment [FX_MAX_EQUIPMENT_NUMBER];
	i3characterEvent_t		event;								 

	int						worldIdx;							 
	float					mouseAngle;							

	vec3_t					mousePos;							
	int						isAlive;							 
	vec3_t					syncPosition;						 
	float					moveSpeed;

	int						curRA;								 
	int						curSA;								 
	int						calMaxRA;							 
	int						calMaxSA;							 
	float					summonValidTime;					
	int						isTransform;						
	int						transformEntityIdx;					
	int						transformType;						

	chatMessage				chatMsg;
	float					chatMsgTime;
	int						scriptFlag;
	int						isNewChar;
		
	
	int						chaosPoint;							
	int						pvpMode;							
	
	
	
	int						generalSpeed;
	
	

	
	bool					bHide;								

#endif	 

 
	vec3_t					mins;
	vec3_t					maxs;
	vec3_t					forward;
	vec3_t					right;
	vec3_t					up;

 

	 
	float					stat_height;					 
	float					stat_disp_x;					 
	float					stat_disp_y;					 
	float					stat_fakeHeight;				 
	matrix4x4_t				stat_matTransform;				 
	float					stat_scale;						 

	int						stat_sound;						 
	int						stat_recoveryTime;				 
	float					stat_attackSpeed;
	float					stat_radius;					 
	Fx_CHARACTER_t			*stat_collidedChar;				 
	int						stat_alive;						 
	int						stat_itemID	[MAX_COUNT_ITEM_PART];	 
	int						stat_skinID	[MAX_COUNT_SKIN_PART];	 
	int						stat_moveID	[MAX_COUNT_ITEM_PART];	 
	BYTE					stat_attacker;						 	
	BYTE					stat_LODLevel;						 

	float					stat_attackTime;				 

	 
	int						attr_skelID;					 
	float					attr_tall;						 

	 
	int						ani_option;						 
	int						ani_curSetType;					 
	int						ani_curItemSetIndex;			 
	int						ani_curSetID;					 
	int						ani_nextSetType;				 
	int						ani_nextItemSetIndex;
	int						ani_nextSetID;
	float					ani_curSpeed;					 
															 
															 
	float					ani_curRSpeed;					 
															 
	float					ani_nextSpeed;					 
	float					ani_nextRSpeed;					 
	int						ani_curIdx;						 
	Fx_ANIM_STATE_t			ani_curState;					 
	int						ani_curGroup;					 
	int						ani_nextIdx;					 
	Fx_ANIM_STATE_t			ani_nextState;
	int						ani_nextGroup;
	float					ani_elapsed;					 
	float					ani_fraction;					 
	float					ani_oldFrame;					 

	 
	int						ip_option;						 
	float					ip_totalTime;					 
	float					ip_curTime;						 
	FX_CAnim				*ip_startAnim;					 
	FX_CAnim				*ip_endAnim;					 

	 
	KEYFRAME				key_curFrames	[MAX_COUNT_BONE_IN_CHAR];	 
	KEYFRAME				key_resultFrames[MAX_COUNT_BONE_IN_CHAR];	 
	KEYFRAME				key_copiedFrames[MAX_COUNT_BONE_IN_CHAR];	 
	int						key_curIndex	[MAX_COUNT_BONE_IN_CHAR];	 
	int						key_nextIndex	[MAX_COUNT_BONE_IN_CHAR];	 
	float					key_displace	[MAX_COUNT_BONE_IN_CHAR];	 

	 
	int						ren_updateFPS;								 
	float					ren_updateSPF;								 
	FX_BUFFERINDEX_t		ren_vtxBufferIdx[MAX_COUNT_SKIN_PART];		 
	float					ren_nextTime;								 
	AURA_t					ren_auraLHandData;							 
	AURA_t					ren_auraRHandData;							 
	matrix4x4_t				ren_tagMatrixData	[MAX_COUNT_ITEM_PART];	 
	vec3_t					ren_xformedPelvisPos;						 
	float					ren_squareDist;								 
	Fx_CHARACTER_t			*ren_closerChar;							 
	Fx_CHARACTER_t			*ren_fartherChar;							 
	double					ren_stampedAppTime;							 

	int						ren_staticEffectIndex [MAX_NUM_STATIC_EFFECT];
	int						ren_staticEffectLevel [MAX_NUM_STATIC_EFFECT];
	int						ren_staticEffectEntityIndex [MAX_NUM_STATIC_EFFECT];
	float					ren_generalSpeed;
	float					ren_oldGeneralSpeed;
	int						ren_numStaticEffects;
	int						ren_renderThisFrame;						 

	 
	bool					flag_tagMatrixMade	[MAX_COUNT_ITEM_PART];	 
	bool					flag_interpolation;				 
	bool					flag_culled;					 
	bool					flag_useLHandAura;				 
	bool					flag_useRHandAura;				 

	bool					flag_disappear;					 
	bool					flag_appTimeStamped;			 
	int						flag_replayAnim;				 
	bool					flag_disappearSound;			 

	bool					flag_forceUpdate;				 
	bool					flag_drawSpawnEffect;			 
	int						attackComboType;				 
	bool					flag_outOfMySight;				 


#ifdef	_FX_TOOL
	bool					flag_renderCharAura;			 
	bool					flag_renderSkelBound;			 
#endif	 

	 
	int						soundTime;


	ATTACKINFO_t			atk_infoBuffer [MAX_COUNT_ATTACK_INFOS];
	int						atk_curCombo;				 
	int						atk_nextStartFrame;			 
	int						atk_indexStart;
	int						atk_nextBeginFrame;			 
	int						atk_indexBegin;
	int						atk_nextDamageFrame;		 
	int						atk_indexDamage;
	int						atk_nextFreeFrame;			 
	int						atk_indexFree;
	int						atk_changeEvent;			 
	int						atk_resend;
	int						atk_playAnim;
	float					atk_freeAttackTime;			 
	float					atk_originalAttackTime;		 


	int						atk_sendRangeAttackResend;	 
	int						atk_processDamage;

		 
	vec3_t					tb_originalPos;
	float					tb_duration;
	double					tb_startTime;
	float					tb_power;
	bool					tb_do;


		 
	int						cc_inUse;
	vec3_t					cc_lightColorStart;
	vec3_t					cc_lightColorEnd;
	float					cc_timeElapsed;

		 
	int						isCreature;
	int						cr_inUse;
	int						cr_creatureIndex;		 
	Fx_CHARACTER_t			*cr_creature;
	Fx_CHARACTER_t			*cr_owner;
    
	
	Fx_CHARACTER_t()
	{

	}
	

	
	BOOL					m_bWaitingRoom;
	int						m_JoinNum;
	int						m_NameColorIdx;
	BOOL					m_bObserveMode;
	

	
 
#ifndef	_FX_TOOL
	void	GetFwdVector	(	vec3_t	vec	)
	{
		VectorSet ( vec,	forward [0],	forward [1],	forward [2] );
	}
	void	GetRightVector	(	vec3_t	vec	)
	{
		VectorSet ( vec,	right [0],	right [1],	right [2] );
	}
    void	GetUpVector	(	vec3_t	vec	)
	{
		VectorSet ( vec,	up [0],	up [1],	up [2] );
	}
#endif	 
	
	void	SetupVectors ()		{	AngleVectors ( angles,	forward,	right,	up );	}
   
};

 
 
 


	 
typedef struct
{
	char	name [256];
}	Fx_TEX_BUF_t;

typedef struct
{
	char	name [256];
	UINT	id;
}	Fx_ANIM_BUF_t,	Fx_SKEL_BUF_t;

typedef struct
{
	char	name [256];
	UINT	id;
	char	texname [256];
}	Fx_ITEM_BUF_t,	Fx_SKIN_BUF_t;

typedef struct
{
	char	name[ 68 ];
	int		shader;
	int		surfaceFlags;
	int		isShader;
}	CHAR_SHADERREF_t;

#include "FX_Etcs/Utilities.h"
	
#include "FX_Base/Animation.h"
#include "FX_Base/Skeleton.h"
#include "FX_Base/Mesh.h"

#include "FX_Managers/TagManager.h"

#include "FX_Renderer/ItemRenderer.h"
#include "FX_Renderer/SkinRenderer.h"
#include "FX_Renderer/AuraRenderer.h"

#ifdef	_FXSKELDRAW
#	include "FX_Managers/SkelDrawManager.h"
#endif

#ifdef	_OBJECTMNG
#	include "FX_Managers/ObjectManager.h"
#	include "FX_Managers/ObjectClassManager.h"
#endif

#include "FX_Managers/AuraMAnager.h"
#include "CharacterManager.h"
#include "FX_Managers/AnimationManager.h"
#include "FX_Managers/ItemManager.h"
#include "FX_Managers/SkeletonManager.h"
#include "FX_Managers/SkinManager.h"
#include "FX_Managers/CharLookupManager.h"
#include "FX_Managers/CharShaderManager.h"

extern const vec4_t			UNIT_X_QUAT;
extern const matrix4x4_t	IDENTITY_MATRIX;
extern const vec3_t			ZERO_VECTOR3;

extern	vec4_t			GLOBAL_QUAT_1;
extern	vec4_t			GLOBAL_QUAT_2;
extern	vec4_t			GLOBAL_QUAT_3;

extern	vec3_t			GLOBAL_DXVEC3_1;
extern	vec3_t			GLOBAL_DXVEC3_2;
extern	vec3_t			GLOBAL_DXVEC3_3;
extern	vec3_t			GLOBAL_DXVEC3_4;
extern	vec3_t			GLOBAL_DXVEC3_5;
extern	vec3_t			GLOBAL_DXVEC3_6;
extern	vec3_t			GLOBAL_DXVEC3_7;
extern	vec3_t			GLOBAL_DXVEC3_8;
extern	vec3_t			GLOBAL_DXVEC3_9;
extern	vec3_t			GLOBAL_DXVEC3_10;
extern	vec3_t			GLOBAL_DXVEC3_11;

extern	vec4_t			GLOBAL_DXVEC4_1;
extern	vec4_t			GLOBAL_DXVEC4_2;
extern	vec4_t			GLOBAL_DXVEC4_3;

extern	matrix4x4_t			GLOBAL_DXMATRIX_1;

extern	vec3_t				GLOBAL_VEC3_1;
extern	vec3_t				GLOBAL_VEC3_2;
extern	vec3_t				GLOBAL_VEC3_3;
extern	vec3_t				GLOBAL_VEC3_4;

extern	vec4_t				GLOBAL_VEC4_1;
extern	vec4_t				GLOBAL_VEC4_2;
extern	vec4_t				GLOBAL_VEC4_3;
extern	vec4_t				GLOBAL_VEC4_4;
extern	vec4_t				GLOBAL_VEC4_5;

extern	matrix4x4_t			GLOBAL_MATRIX_1;
extern	matrix4x4_t			GLOBAL_MATRIX_2;

extern	matrix4x4_t			GLOBAL_SCALEMATRIX;
extern const float			FX_DEGTORAD;
extern	bboxf_t				GLOBAL_BBOX;	

#ifdef	_FXDEBUG
extern	char				DBGSTRING [MAX_PATH];
#endif

	 
#ifdef	_FX_TOOL
	#pragma	warning	(	disable : 4786	)

	#include		<vector>

	#define		VEC_STRING		std::vector<std::string>

	typedef	char	FILENAME	[256];

	#include	"FX_Tools/Tool.h"
#endif

#endif

