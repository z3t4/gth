



#if !defined(AFX_MDLMNG_H__86623AB5_EAFE_455F_BA31_CAACB52436A6__INCLUDED_)
#define AFX_MDLMNG_H__86623AB5_EAFE_455F_BA31_CAACB52436A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#define GTH_MODEL_FORMAT       ( GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_TEXVERTEX( 1 ) )

#define MAX_SIZE_MDL_STATIC_VERTEX_BUFFER_BANK   16
#define MAX_SIZE_MDL_DYNAMIC_VERTEX_BUFFER       1024 * 60
#define MAX_SIZE_MDL_DYNAMIC_INDEX_BUFFER        1024 * 180
#define MAX_SIZE_MDL_TEMP_VERTEX_BUFFER          1024 * 10

#define MAX_BANKSIZE_VERTEXBUFFER            1024 * 64




#define MAX_SIZE_MDL				  600


#define WORLD_ENTITY_MAX_PARTS        4
#define ITEM_ENTITY_MAX_PARTS         4

#define EFFECT_ENTITY_MAX_PARTS       16
#define EFFECT_ENTITY_MAX_STATES      4

#define MAX_SIZE_MDL_WORLD_CLASS      256
#define MAX_SIZE_MDL_ITEM_CLASS       256
#define MAX_SIZE_MDL_EFFECT_CLASS     256

#define MAX_SIZE_MDL_WORLD_ENTITY     1024
#define MAX_SIZE_MDL_ITEM_ENTITY      128
#define MAX_SIZE_MDL_EFFECT_ENTITY    1024

typedef enum
{
	GTH_ANIM_PLAY_ONCE = 0 ,
	GTH_ANIM_PLAY_LOOP ,
	GTH_ANIM_PLAY_TIME
} GTH_ANIM_PLAY_TYPE;

typedef enum
{
	GTH_SYNC_APPTIME = 0 ,
	GTH_SYNC_EVENT ,
	GTH_SYNC_ANIMATION
} GTH_SYNC_TYPE;



typedef enum
{
	GTH_WORLD_ENTITY_TYPE_NONE = 0 ,
	GTH_WORLD_ENTITY_TYPE_MODEL ,
	GTH_WORLD_ENTITY_TYPE_PLAYER_POS ,
	GTH_WORLD_ENTITY_TYPE_TELEPORT ,
	GTH_WORLD_ENTITY_TYPE_ACTIVATOR ,
	GTH_WORLD_ENTITY_TYPE_DEACTIVATOR ,
	GTH_WORLD_ENTITY_TYPE_DOOR ,
	GTH_WORLD_ENTITY_TYPE_PATH ,
	GTH_WORLD_ENTITY_TYPE_CAMERA_PATH ,
	GTH_WORLD_ENTITY_TYPE_SOUND
} GTH_WORLD_ENTITY_TYPE;

typedef struct mdlworldentity_s
{
	vec3_t  origin;
	float   rotate;
	vec3_t  mins;
	vec3_t  maxs;
	
	int     idxClass;
	int		visible;

		
	vec3_t  *collisionVertex [WORLD_ENTITY_MAX_PARTS];
	int     num_cvertex[WORLD_ENTITY_MAX_PARTS];
		
    
	mdlworldentity_s  *next;
	mdlworldentity_s  *prev;
	mdlworldentity_s  *renderNext;
	mdlworldentity_s  *blendNext;
} mdlworldentity_t;

typedef struct
{
    char        mdlName[64];
	char        tagName[32];
	float       msecPerFrame;
    float       rotSpeed;
    float       scale;
    int         mdlIdx;
	int         onlyTag;
	
} mdlworldclasspart_t;

typedef struct
{
    
	int     classType;
	char    itemName[64];
    int     loaded;
    int     numParts;
    float   bobHeight;
    float   bobSpeed;
	vec3_t  mins;
	vec3_t  maxs;
	mdlworldclasspart_t parts[ WORLD_ENTITY_MAX_PARTS ];
	 
	int		hideFlag;
		
} mdlworldclass_t;

typedef struct
{
	int    idxClass;
	mdlworldentity_t *linked;
} mdlworldentitylink_t;




typedef enum
{
	GTH_ITEM_CLASS_NONE = 0 ,
	GTH_ITEM_CLASS_WEAPON ,
	GTH_ITEM_CLASS_UPPER ,
	GTH_ITEM_CLASS_LOWER ,
	GTH_ITEM_CLASS_PROTECTOR ,
	GTH_ITEM_CLASS_HAND ,
	GTH_ITEM_CLASS_FOOT ,
	GTH_ITEM_CLASS_AIR_PROTECTOR ,
	GTH_ITEM_CLASS_ACCESSORY,
	GTH_ITEM_CLASS_RESERVED
} GTH_ITEM_CLASS_TYPE;

typedef struct mdlitementity_s
{
	vec3_t  origin;
	float   rotate;
	vec3_t  mins;
	vec3_t  maxs;
	
	int     idxClass;
	int		visible;
	
	GTH_SYNC_TYPE       syncType;
	GTH_ANIM_PLAY_TYPE  animType;
	float               animTime;
    
	mdlitementity_s  *next;
	mdlitementity_s	 *prev;
} mdlitementity_t;

typedef struct
{
    char    mdlName[64];
	char    tagName[32];
	float   msecPerFrame;
    float   rotSpeed;
    float   scale;
    int     mdlIdx;
	int     onlyTag;
	vec3_t  color;
	
} mdlitemclasspart_t;

typedef struct
{
    char    className[64];
	int     classType;
	char    itemName[64];
	int     itemID;
    int     loaded;
    int     numParts;
    float   bobHeight;
    float   bobSpeed;
	vec3_t  mins;
	vec3_t  maxs;
	mdlitemclasspart_t parts[ ITEM_ENTITY_MAX_PARTS ];
} mdlitemclass_t;

typedef struct
{
	int    idxClass;
	mdlitementity_t *linked;
} mdlitementitylink_t;




typedef enum
{
	GTH_EFFECT_CLASS_NONE = 0 ,
	GTH_EFFECT_CLASS_MODEL ,
	GTH_EFFECT_CLASS_MULTI_MODEL ,
	GTH_EFFECT_CLASS_POINTER
} GTH_EFFECT_CLASS_TYPE;

typedef struct mdleffectentity_s
{
	vec3_t  origin;
	vec3_t  target;
	vec3_t  angles;
	vec3_t  mins;
	vec3_t  maxs;

	
	bool    m_SendAttackMessage;
	
	int		idxEntity;		 
	int     idxClass;
	int		visible;
	float   startTime;
	float   completeTime;
	float   animTime;
	int     state;
	int     firedStep;
	float   firedScale;
	float   hitDiff;

	int		effect_level;		 

	vec3_t	offset;
	int		follow_attacker;
	int		attackerIdx;
	int		attackerType;		 
	int		defenderIdx;		 
	int		defenderType;		 
	float	lifeTime;			 

	 
	bool	soundflag [EFFECT_ENTITY_MAX_STATES][EFFECT_ENTITY_MAX_PARTS];

	 
	float	velocity;

	 
	float	backup_startTime;
	vec3_t	backup_origin;
	vec3_t	backup_angles;

	mdleffectentity_s  *next;
	mdleffectentity_s  *prev;

	
	int		effectID;
} mdleffectentity_t;

#define  GTH_EFFECT_STATE_NORMAL  0
#define  GTH_EFFECT_STATE_ARMED   0
#define  GTH_EFFECT_STATE_FIRED   1
#define  GTH_EFFECT_STATE_HITTED  2
#define  GTH_EFFECT_STATE_MISSED  3

#define GTH_EFFECT_NOT_COMPLETED          0
#define GTH_EFFECT_ALL_STATE_COMPLETED    1
#define GTH_EFFECT_ARMED_STATE_COMPLETED  2
#define GTH_EFFECT_FIRED_STATE_COMPLETED  3
#define GTH_EFFECT_ERROR                  4

#define  MAX_SIZE_GTH_EFFECT_CLASS_STATE 4

#define  GTH_EFFECT_ANIMTIME_FULL -1.0f

#define GTH_EFFECT_FIRED_STEP_COMPLETE 4  
#define GTH_EFFECT_FIRED_STEP_DIVINE   0.25f

	 
#define GTH_EFFECT_HIT_STEP_COMPLETE 50  
#define	GTH_EFFECT_HIT_STEP_DIVINE   0.02f
	

 	
#define	MAX_SIZE_EFFECT_LEVEL		8
	

typedef struct
{
    char    mdlName[64];
	char    tagName[32];
	float   startTime;
	float   endTime;
	float   msecPerFrame;
    float   rotSpeed;
    float   scale;
    int     mdlIdx;
	int     onlyTag;
	int     numFrames;

	vec3_t	offset;			 
	vec3_t	angleOffset;	 
	float	selfRotSpeed;	 
	int		req_level;
	int		billboard;

	 


	
} mdleffectclasspart_t;

typedef struct
{
	int     state;
	int     numParts;
    float   bobHeight;
    float   bobSpeed;
	float   completeTime[MAX_SIZE_EFFECT_LEVEL];	 
	float   normalSize;
	int     soundID;
	int     loaded;
	int		projectile;			 
	float	projectileHeight;	 
	float	startTime;			  
	vec3_t  mins;
	vec3_t  maxs;
	int		setground;								 
	mdleffectclasspart_t parts[ EFFECT_ENTITY_MAX_PARTS ];
} mdleffectclassstate_t;


typedef struct
{
	int     classType;
	char    effectName[64];
	int     effectID;
    int     loaded;
	float   completeTime;
	mdleffectclassstate_t states[ EFFECT_ENTITY_MAX_STATES ];
	char	effectDesc[256];
	int		maxLevel;
	int		inuse;				 
} mdleffectclass_t;

typedef struct
{
	int    idxClass;
	mdleffectentity_t *linked;
} mdleffectentitylink_t;

typedef struct
{
	int worldClasses [ MAX_SIZE_MDL_WORLD_CLASS ];
	int itemClasses  [ MAX_SIZE_MDL_ITEM_CLASS ];
	int effectClasses[ MAX_SIZE_MDL_EFFECT_CLASS ];
} linkedcount_t;

 
struct	Fx_CHARACTER_t;

class CMDLMng  
{
public:

	CMDL  m_MDLs[ MAX_SIZE_MDL ];

	mdlworldclass_t       m_MDLWorldClasses [ MAX_SIZE_MDL_WORLD_CLASS ];
	mdlitemclass_t        m_MDLItemClasses  [ MAX_SIZE_MDL_ITEM_CLASS ];
	mdleffectclass_t      m_MDLEffectClasses[ MAX_SIZE_MDL_EFFECT_CLASS ];

	mdlworldentity_t      m_MDLWorldEntities [ MAX_SIZE_MDL_WORLD_ENTITY ];
	mdlitementity_t       m_MDLItemEntities  [ MAX_SIZE_MDL_ITEM_ENTITY ];
	mdleffectentity_t     m_MDLEffectEntities[ MAX_SIZE_MDL_EFFECT_ENTITY ];

	mdlworldentitylink_t  m_linkedMDLWorldEntities [ MAX_SIZE_MDL_WORLD_CLASS ];
	mdlitementitylink_t   m_linkedMDLItemEntities  [ MAX_SIZE_MDL_ITEM_CLASS ];
	mdleffectentitylink_t m_linkedMDLEffectEntities[ MAX_SIZE_MDL_EFFECT_CLASS ];

	mdlworldentity_t     *m_linkedRenderMDLWorldEntities[ MAX_SIZE_MDL_WORLD_CLASS ];
	mdlworldentity_t     *m_linkedBlendMDLWorldEntities [ MAX_SIZE_MDL_WORLD_CLASS ];
	
	int  m_worldInUse [ MAX_SIZE_MDL_WORLD_ENTITY ];
	int  m_itemInUse  [ MAX_SIZE_MDL_ITEM_ENTITY ];
	 


	linkedcount_t  m_linkedVisibleCount;
	
	int  m_numMDLs;
	int  m_numMDLWorldClasses;
	int  m_numMDLItemClasses;
	int  m_numMDLEffectClasses;

	int  m_currMDLWorldEntity;
	int  m_currMDLItemEntity;
	int  m_currMDLEffectEntity;
	
	
	CShaderMng     *m_shaderMng;   
	CD3DTextureMng *m_texMng;      
	CD3DRender     *m_D3DRender;   
	CTimer         *m_timer;       
	CFileMng       *m_fileMng;     
	CScript        *m_script;      
	CMusicMng      *m_musicMng;    

	
	LPDIRECT3DDEVICE8 m_pD3DDevice;  
	int      m_totalRenderPolygon;

	
	matrix4x4_t  *m_envMat;
	vec3_t        m_camPos;
	vec3_t        m_camDir;
	float         m_currTime;
	float         m_currMilliTime;

	
	
	int			m_numBank;
	int			m_numStaticElems;
	int			m_prevNumStaticVerts;
	int			m_currNumStaticVerts;
	CResManager	m_resManager;

	int             m_bankSize[ MAX_SIZE_MDL_STATIC_VERTEX_BUFFER_BANK ];
	CD3DBuffer     *m_staticVB[ MAX_SIZE_MDL_STATIC_VERTEX_BUFFER_BANK ];
	CD3DBuffer     *m_dynamicVB;
	CD3DBuffer     *m_dynamicIB;
	CD3DBuffer     *m_staticEB;

	
	BYTE    *m_visData;

	
	DWORD      m_format;
	int        m_currBank;
	wavefunc_t m_waveFunc;
	matrix4x4_t  m_transform;
	int        m_isInterpolated;
	float      m_interPol;
	int        m_currFrame;
	
	DWORD      m_frameFlag;

	int        m_focusedItem;
	int        m_focusedWorld;

	int        m_useFocusItem;
	int        m_useFocusWorld;
	DWORD      m_focusedItemColor;

	
	unsigned long	m_mdlRscSize;

	
	CMDLMng();
	virtual ~CMDLMng();

	void  SetShaderMng( CShaderMng *shaderMng )     { m_shaderMng = shaderMng; }
	void  SetTexMng( CD3DTextureMng *texMng )       { m_texMng = texMng; }
	void  SetDevice( LPDIRECT3DDEVICE8 pD3DDevice ) { m_pD3DDevice = pD3DDevice; }
	void  SetFileMng( CFileMng *fileMng )           { m_fileMng = fileMng; }
	void  SetTimer( CTimer *timer )                 { m_timer = timer; }
	void  SetRenderer( CD3DRender *d3dRender )      { m_D3DRender = d3dRender; }
	void  SetScript( CScript *script )              { m_script = script; }
	void  SetEnvironmentMatrix( matrix4x4_t *mat )  { m_envMat = mat; }
	void  SetVisData( BYTE *vis )                   { m_visData = vis; }
	void  SetFrameFlag( DWORD frameFlag )           { m_frameFlag = frameFlag; }
	void  SetMusicMng( CMusicMng *musicMng )        { m_musicMng = musicMng; }
	
	void  SetCamera( vec3_t pos , vec3_t dir );
	void  SetFocusedItemColor( int alpha , int red , int green , int blue )	{ m_focusedItemColor = MAKE_COLOR_ARGB( alpha , red , green , blue ); }
	void  SetFocusItemUsage( int usage )            { m_useFocusItem = usage; }
	void  SetFocusWorldUsage( int usage )           { m_useFocusWorld = usage; }

	void  Release();
	int   PrepareMDLShader();
	int   Initialize();
	int   Restore( LPDIRECT3DDEVICE8 pD3DDevice );
	int   GenDynamicMem();
	void  PrepareVertexBuffer();
	int   UploadVertexBuffer();
	int   LoadMDL( char *MDLName );

	
	
	CMDL*	GetMDL( int mdlIdx );
	int   PrepareMDLShader( int mdlIdx );
	void	CalcVertexBuffer( CMDL *mdl );
	void	EndOfCalcVertexBuffer();
	void	RestoreVertexInfo( int mdlIdx );

	
	int   LoadWorldClass( char *fname );
	void  ParseWorldClassPart( mdlworldclasspart_t *part );
	void  ParseWorldClassToken( mdlworldclass_t *mdlClass , char *token );
	int   GetWorldClassType( char *token );
	int   LinkMDLWorldClass( int idxMDLClass );
	
	void  SetMDLWorldEntityOrigin( int idxMDLEntity , vec3_t origin );
	void  SetMDLWorldEntityAngles( int idxMDLEntity , vec3_t angles );
	mdlworldentity_t     *GetMDLWorldEntity( int idxMDLEntity );
	mdlworldentitylink_t *GetLinkedMDLWorldEntity( int idxClass );
	void  RenderWorldEntity();
	void  RenderBlendWorldEntity();
		
	void  MakeTransformedMDL( mdlworldentity_t *entity, int idxClass );
		

	int   AddWorldEntity( char *itemName , vec3_t origin , vec3_t angles );
	int   DeleteWorldEntity( int idxMDLEntity );
	void  ClearWorldUseFlag();
	void  CalcWorldBoundBox( mdlworldclass_t *mdlWorldClass );
	int   GetMDLWorldClassBoundBox( int idxMDLEntity , vec3_t mins , vec3_t maxs );

	
	int   LoadItemClass( char *fname );
	void  ParseItemClassPart( mdlitemclasspart_t *part );
	void  ParseItemClassToken( mdlitemclass_t *mdlClass , char *token );
	int   GetItemClassType( char *token );
	int   LinkMDLItemClass( int idxMDLClass );
	void  SetMDLItemEntityOrigin( int idxMDLEntity , vec3_t origin );
	void  SetMDLItemEntityAngles( int idxMDLEntity , vec3_t angles );
	mdlitementity_t     *GetMDLItemEntity( int idxMDLEntity );
	mdlitementitylink_t *GetLinkedMDLItemEntity( int idxClass );
	void  RenderItemEntity();

	int   InitItemClass();
	int   AddItemEntity( GTH_ITEM_CLASS_TYPE classType , int itemID , vec3_t origin , vec3_t angles 
						  , GTH_SYNC_TYPE syncType = GTH_SYNC_APPTIME  
						  , GTH_ANIM_PLAY_TYPE animType = GTH_ANIM_PLAY_LOOP , float animTime = 0.0f );
	int   DeleteItemEntity( int idxMDLEntity );
	void  ClearItemUseFlag();
	void  CalcItemBoundBox( mdlitemclass_t *mdlItemClass );
	int   GetMDLItemClassBoundBox( int idxMDLEntity , vec3_t mins , vec3_t maxs );

	
	int   LoadEffectClass( char *fname );
	void  ParseEffectClassState( mdleffectclassstate_t *destState );
	void  ParseEffectClassPart( mdleffectclasspart_t *part );
	void  ParseEffectClassToken( mdleffectclass_t *mdlClass , char *token );
	int   GetEffectClassStateType( char *token );
	float GetEffectClassAnimTime( char *token );
	int   GetEffectClassType( char *token );
	int   LinkMDLEffectClass( int idxMDLClass );
	void  SetMDLEffectEntityOrigin( int idxMDLEntity , vec3_t origin );
	void  SetMDLEffectEntityAngles( int idxMDLEntity , vec3_t angles );
	mdleffectentity_t     *GetMDLEffectEntity( int idxMDLEntity );
	mdleffectentitylink_t *GetLinkedMDLEffectEntity( int idxClass );
	void  RenderEffectEntity();

	int   InitEffectClass();

	 
	 
	int   AddEffectEntity(	int	entityIndex,	GTH_EFFECT_CLASS_TYPE	classType,	int	effectID,	vec3_t	origin,	vec3_t	target,
							vec3_t	angles,	float	hitDiff,	int	effect_level	=	0,	int	follow_attacker	=	false,
							int	attackerIdx	=	-1,	int	attackerType	=	-1,	int	defenderIdx	=	-1,	int defenderType	=	-1,
							float	offset_x	=	0.0f,	float	offset_y	=	0.0f,	float	offset_z	=	0.0f,	float	in_lifeTime	=	-1.0f,	int	in_follow_repetition	=	true	);
	int   DeleteEffectEntity( int idxMDLEntity );
	void  ClearEffectUseFlag();
	void  CalcEffectBoundBox( mdleffectclassstate_t *state );
	int   GetMDLEffectClassBoundBox( int idxMDLEntity , int state , vec3_t mins , vec3_t maxs );
		
	float GetEffectStateCompleteTime( int idxClass , int state , float startTime,  int effect_level = 0 );
		
	int   IsOverEffectCompleteTime( int idxMDLEntity );
	void  SetEffectTarget( mdleffectentity_t *mdlEntity , vec3_t target , vec3_t angles );
	int   SetFiredState( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs );
	int   SetHittedState( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs );
	int   SetMissedState( int idxMDLEntity , vec3_t target , vec3_t mins , vec3_t maxs );
		
	void  ClearClassSoundFlag( mdleffectclass_t *effectclass );
		

	
	void  Render();
	void  PrepareInterpolateMDL( CMDL *mdl , mdlmesh_t *mesh );
	void  PrepareMDL( CMDL *mdl , mdlmesh_t *mesh );
	void  RenderBound( CMDL *mdl );
	void  PrepareTCGenMDLMesh( CMDL *mdl , mdlmesh_t *mesh );
	void  PrepareMDLMesh( CMDL *mdl , mdlmesh_t *mesh );
	void  PrepareInterpolateTCGenMDLMesh( CMDL *mdl , mdlmesh_t *mesh );
	void  PrepareInterpolateMDLMesh( CMDL *mdl , mdlmesh_t *mesh );

	void  RenderBBox( vec3_t mins , vec3_t maxs );

	void  ClearItemLinkedCount();
	void  ClearEffectLinkedCount();
	void  ClearRenderWorldLink();
	void  ClearBlendWorldLink();

	int   *GetWorldVisibleLinkedCount()   { return m_linkedVisibleCount.worldClasses; }
	int   *GetItemVisibleLinkedCount()    { return m_linkedVisibleCount.itemClasses; }
	int   *GetEffectVisibleLinkedCount()  { return m_linkedVisibleCount.effectClasses; }

	void	BindSoundSample	();

	 

	int		IsOverEffectCompleteTime_Neo	(	int		idxMDLEntity,	bool	&processDamage,	bool	&sendAttackMessage	);
	int		SetFiredState_Neo ( int idxMDLEntity, vec3_t target, vec3_t mins, vec3_t maxs );
	int		SetHittedState_Neo ( int idxMDLEntity, vec3_t target, vec3_t mins, vec3_t maxs );
	int		SetMissedState_Neo ( int idxMDLEntity, vec3_t target, vec3_t mins, vec3_t maxs );
	void	RenderEffectEntity_Neo ();

	 
	mdleffectentity_t*	DeleteEffectManagerEntity	(	mdleffectentity_t	*in_entity	);

	bool	RenderEffectPreCheck	(	mdleffectentity_t	**in_entity,	Fx_CHARACTER_t		**out_attacker,	Fx_CHARACTER_t		**out_defender		);
	float	RenderEffectEvaluateFunction	(	mdleffectclassstate_t	*in_classState	);
	bool	RenderEffectCheckRenderCondition	(	mdleffectclasspart_t	*in_classPart,	Fx_CHARACTER_t	*in_attacker,	Fx_CHARACTER_t	*in_defender,	int		in_followAttacker,
													int		in_state,	int		in_effectLevel,	float	in_startTime,	float	&in_partStartTime	);
	void	RenderEffectPlaySound	(	bool	in_soundflag[][EFFECT_ENTITY_MAX_PARTS],	vec3_t	in_origin,	int		in_state,	CMDL	*in_mdl,	int		in_subindex	);
	void	RenderEffectCalcCurrentFrame	(	CMDL	*in_mdl,	mdleffectclasspart_t	*in_classPart,	float	in_startTime,	float	in_partStartTime	);
	void	RenderEffectPrepareBuffers	(	CMDL	*in_mdl	);
	bool	RenderEffectSetTransform	(	mdleffectclasspart_t	*in_classPart,	int		in_followAttacker,	vec3_t	in_offset,	int		in_state,	vec3_t	in_origin,	vec3_t	in_angles,
											mdleffectclassstate_t	*in_classState,	Fx_CHARACTER_t	*in_attacker,	Fx_CHARACTER_t	*in_defender,	vec3_t	out_position,	float	in_bob	);
	void	RenderEffectDoRender	(	CMDL	*in_mdl,	vec3_t	in_position	);
};


#endif 
