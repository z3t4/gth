



#if !defined(AFX_ENTITYMNG_H__60CAD205_9328_4435_8694_F707431E296E__INCLUDED_)
#define AFX_ENTITYMNG_H__60CAD205_9328_4435_8694_F707431E296E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define MAX_SIZE_ITEM_NAME 32
#define MAX_SIZE_WORLD_ENTITY    1024
#define MAX_SIZE_ENTITY_STRING   1024

#define MAX_SIZE_STRING 32
typedef char entitystring_t[ MAX_SIZE_STRING ];

#define MAX_SIZE_ACTIVATOR 128

#define ENTITY_OVER_THAN_VISIBLE      0
#define ENTITY_OVER_THAN_CHARACTER    1
#define ENTITY_CLOSED_THAN_CHARACTER  2

typedef enum
{
	GTH_ENTITY_PLAY_FORWARD  = 1 << 0 ,
	GTH_ENTITY_PLAY_REVERSE  = 1 << 1 ,
	GTH_ENTITY_PLAY_BACKSTEP = 1 << 2 ,
	GTH_ENTITY_PLAY_ONCE     = 1 << 3 ,
	GTH_ENTITY_PLAY_LOOP     = 1 << 4 ,
	GTH_ENTITY_PLAY_TIME     = 1 << 5
} GTH_ENTITY_PLAY_TYPE;

typedef struct
{
	void   *startEntity;
	void   *destEntity;
	void   *currEntity;
	int     playType;
	float   moveSpeed;
	float   startTime;
	float   currChainTime;
	float   currChainCompleteTime;
	int     characterID;
	int     playing;
} activator_t;

typedef struct worldentity_s
{
	int     inuse;

	int     classType;
	int	    spawnflags;

	int     idxMDLEntity;
	char    itemName[ MAX_SIZE_ITEM_NAME ];

	vec3_t  origin;
	vec3_t  angles;

	vec3_t  mins;
	vec3_t  maxs;

	float   radius;
	int     linkedCluster;

	float   effectiveDist;
	char   *target;
	char   *targetName;
	char   *message;

	char   *sound;
		
	float	soundRangeX;
	float	soundRangeY;
	int		soundIdx;
	int		soundInPlaying;
		

	float   speed;
	float   wait;
	int     spawnid;

	worldentity_s  *targetEntity;
	worldentity_s  *prevEntity;
	activator_t    *activator;

	worldentity_s  *prev;
	worldentity_s  *next;
} worldentity_t;

#define GTH_ENTITY_VISIBLE_QUALITY_LOW   1
#define GTH_ENTITY_VISIBLE_QUALITY_HIGH  0

class CEntityMng  
{
public:

	CBSP      *m_world;       
	CMDLMng   *m_MDLMng;      
	CScript   *m_script;      
	CTimer    *m_timer;       
	CCamera   *m_camera;      
	CMove     *m_move;        
	FX_CCharManager *m_characterMng;   

	worldentity_t   m_entities[ MAX_SIZE_WORLD_ENTITY ];
	entitystring_t  m_strings[ MAX_SIZE_ENTITY_STRING ];

	int       m_numEntities;
	int       m_numStrings;
	vec3_t    m_camPos;
	vec3_t    m_palyerPos;
	float     m_camDist;
	float     m_viewSightMinHeight;

	int       m_spawnID;

	activator_t     m_activator[ MAX_SIZE_ACTIVATOR ];
	int             m_numActivator;

	worldentity_t *m_currCameraPathEntity;
	worldentity_t *m_currPathEntity;

	worldentity_t *m_linked;

    
	BYTE       *m_visData;
	CFrustum  *m_frustum;

	DWORD    m_frameFlag;
	float    m_visibleDist;
	float    m_removeDist;
	int      m_visibleQuality;

	CEntityMng();
	virtual ~CEntityMng();

	void  SetScript( CScript *script )   { m_script = script; }
	void  SetMDLMng( CMDLMng *MDLMng )   { m_MDLMng = MDLMng; }
	void  SetWorldModel( CBSP *world )   { m_world = world; }
	void  SetTimer( CTimer *timer )      { m_timer = timer; }
	void  SetCamara( CCamera *camera )   { m_camera = camera; }
	void  SetMover( CMove *move )        { m_move = move; }
	void  SetCharacterMng( FX_CCharManager *characterMng )   { m_characterMng = characterMng; }
	void  SetFrustum( CFrustum    *frustum )     { m_frustum = frustum; }
	void  SetFrameFlag( DWORD frameFlag )         { m_frameFlag = frameFlag; }
	void  SetvisibleQuality( int quality )        { m_visibleQuality = quality; }
	

	int   GetSpawnID() { return m_spawnID; }

	int   LoadEntity( char *fname );
	int   ParseEntityToken( worldentity_t *entity , char *token );
	int   GetClassType( char *token );
	void  AddLink( worldentity_t  *entity );
	worldentity_t *SearchTarget( char *target );
	worldentity_t *SearchTarget( char *target , int classType );
	void  Initialize( float visibleDist );
	inline int ChechVisibleDistance( vec3_t origin , vec3_t mins , vec3_t maxs , float radius );
	inline int ChechEffectiveDistance( vec3_t origin , float effectiveDist );
	inline int ChechEffectiveCamDistance( vec3_t origin , float effectiveDist );
		
	inline int CheckEffectiveSoundDistance( vec3_t origin , float range_x, float range_y );
	int   CheckEntityCharCollision( worldentity_t *entity, bboxf_t bound );
		
	void  Process( vec3_t palyerPos , vec3_t camPos , DWORD frameFlag , float camDist );
	void  ProcessEntity();
	char *GetNewString( char *str );
	inline int Cull( vec3_t mins , vec3_t maxs , vec3_t position , int linkedCluster );
	float  CalculateRadius( vec3_t mins , vec3_t maxs );

	activator_t *GetNewActivator();
	int   ActivateCameraPath( char *start , char *dest , int playType );
	int   ActivatePath( int characterID , char *start , char *dest , int playType );
	void  Deactivate( worldentity_t *entity );

	void  ProcessCameraPath( worldentity_t *entity );
	int   LinkCameraPathChain( worldentity_t *entity , activator_t *activator );
	void  SetCameraState( activator_t *activator , worldentity_t *startEnt , worldentity_t *destEnt , float currRate );

	void  ProcessPath( worldentity_t *entity );
	int   LinkPathChain( worldentity_t *entity , activator_t *activator );
	void  SetPathState( activator_t *activator , worldentity_t *startEnt , worldentity_t *destEnt , float currRate );

};

#endif 
