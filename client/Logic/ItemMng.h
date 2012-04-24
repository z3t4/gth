



#if !defined(AFX_ITEMMNG_H__7174EEC8_D829_4A1E_BA95_1C22603BDD83__INCLUDED_)
#define AFX_ITEMMNG_H__7174EEC8_D829_4A1E_BA95_1C22603BDD83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define MAX_SIZE_ITEM_NAME 32
#define MAX_SIZE_ITEM_ENTITY    1024

typedef enum
{
	OBJECT_TYPE_ITEM		=	0,
	OBJECT_TYPE_PORTAL,
	
	
	OBJECT_TYPE_NAK,
	OBJECT_TYPE_ETC,
} ITEM_OBJECT_TYPE_t;

typedef struct
{
	int		idxOnServer;
	int		itemTableIdx;
	vec3_t	position;

	
	GTH_ITEM_CLASS_TYPE classType;
	int		itemID;
} syncItemList_t;



typedef struct itementity_s
{
	
	int     itemID;
	int     idxMDLEntity;
	int		spawnflags;



	int     idx;
	int		idxOnServer;
	int     itemTableIdx;


	GTH_ITEM_CLASS_TYPE  classType;

	vec3_t  origin;
	vec3_t  angles;

	vec3_t  mins;
	vec3_t  maxs;

	int     linkedCluster;
	int     visible;
	
	
	
	float	squareDist;
	itementity_s	*closerEntity;
	itementity_s	*fartherEntity;

	itementity_s  *next;
	itementity_s  *prev;
} itementity_t;

class CItemMng  
{
public:

	CBSP      *m_world;     
	CMDLMng   *m_MDLMng;     
	CScript   *m_script;      

	itementity_t  m_entities   [ MAX_SIZE_ITEM_ENTITY ];
	int           m_entityInUse[ MAX_SIZE_ITEM_ENTITY ];

	itementity_t *m_linked;

	int       m_currEntity;
	vec3_t    m_camPos;

    
	BYTE       *m_visData;
	CFrustum  *m_frustum;

	DWORD    m_frameFlag;
	float    m_visibleDist;
	float    m_minSquareDist;

	CItemMng();
	virtual ~CItemMng();

	void  SetScript( CScript *script )   { m_script = script; }
	void  SetMDLMng( CMDLMng *MDLMng )   { m_MDLMng = MDLMng; }
	void  SetWorldModel( CBSP *world )   { m_world = world; }
	void  SetFrustum( CFrustum  *frustum )       { m_frustum = frustum; }
	void  SetFrameFlag( DWORD frameFlag )         { m_frameFlag = frameFlag; }
	
	void  Initialize( float visibleDist );
	inline int ChechVisibleDistance( vec3_t origin );
	void  Process( vec3_t pos , DWORD frameFlag );
	void  ProcessEntityFrame();
	inline int Cull( vec3_t mins , vec3_t maxs , vec3_t position , int linkedCluster );

	int   Add( GTH_ITEM_CLASS_TYPE classType , int itemID , vec3_t origin , int idxOnServer, int itemTableIdx );
	int   Delete( int idxEntity );
	void  ClearUseFlag();

	int   FindEntity ( int idxOnServer );
	void  SyncItemObject( syncItemList_t *syncItem, int numSyncItem );

	itementity_t  *SearchCursorEntity( vec3_t pos , matrix4x4_t *transMat , float minSquareDist , vec2_t mouse , vec2_t viewport );
	itementity_t *SearchShortestEntity( vec3_t pos );
	float GetMinSquareDist()  { return m_minSquareDist; }

	
	
	itementity_t*	SearchCursorEntityNeo(	const vec3_t in_camPos, const vec3_t in_camAngle, matrix4x4_t *in_camTransMat, const float in_minSquareDist,
											const vec2_t in_mousePos,const vec2_t in_viewportSize );
};

#endif 
