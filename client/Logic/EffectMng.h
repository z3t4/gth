



#if !defined(AFX_EFFECTMNG_H__0E5BD017_F677_4A69_A9C5_3F36717684FB__INCLUDED_)
#define AFX_EFFECTMNG_H__0E5BD017_F677_4A69_A9C5_3F36717684FB__INCLUDED_

#define MAX_SIZE_EFFECT_ENTITY   1024

typedef struct effectentity_s
{
	
	int     idxMDLEntity;
	int		spawnflags;
	int     idxSelf;

	int     effectID;
	GTH_EFFECT_CLASS_TYPE  classType;

	vec3_t  origin;
	vec3_t  angles;
	vec3_t  target;
	int     isHitted;

	int attackerType;
	int attackerID;
	int defenderType;
	int defenderID;

	vec3_t  mins;
	vec3_t  maxs;

	int     linkedCluster;

	effectentity_s  *next;
	effectentity_s  *prev;

	int		effect_level;
	
	 
	bool	processDamage;
	bool	sendAttackMessage;







	
	enum enumEffectType
	{
		EFFECT_TYPE_NONE=0,
		EFFECT_TYPE_PC_SUMMON,
		EFFECT_TYPE_PC_MOVE_SUMMON,
	};

	
	
	
	int						EffectType;

	

} effectentity_t;

class CEffectMng  
{
public:

	CBSP       *m_world;      
	CMDLMng    *m_MDLMng;     
	CScript    *m_script;     
	CIFNumCtl *m_numberCtl;  
	
	effectentity_t  m_entities   [ MAX_SIZE_EFFECT_ENTITY ];
	int             m_entityInUse[ MAX_SIZE_EFFECT_ENTITY ];

	effectentity_t *m_linked;

	int       m_currEntity;
	vec3_t    m_camPos;

    
	BYTE       *m_visData;
	CFrustum  *m_frustum;

	DWORD    m_frameFlag;
	float    m_visibleDist;
	
	CEffectMng();
	virtual ~CEffectMng();

	void  SetScript( CScript *script )   { m_script = script; }
	void  SetMDLMng( CMDLMng *MDLMng )   { m_MDLMng = MDLMng; }
	void  SetWorldModel( CBSP *world )   { m_world = world; }
	void  SetNumberCtl( CIFNumCtl *numberCtl )   { m_numberCtl = numberCtl; }
	void  SetFrustum( CFrustum    *frustum )     { m_frustum = frustum; }
	void  SetFrameFlag( DWORD frameFlag )         { m_frameFlag = frameFlag; }
	

	void  Initialize( float visibleDist );
	inline int ChechVisibleDistance( vec3_t origin );
	void  Process( vec3_t pos , DWORD frameFlag );
	void  ProcessEntityFrame();

	 
	 
	int		Add(	GTH_EFFECT_CLASS_TYPE	classType,	int	effectID,	vec3_t	origin,	vec3_t	target	=	NULL,
					vec3_t	angles	=	NULL,	float	hitDiff	=	-20.0f,
					int	attackerType	=	-1,	int	attackerID	=	-1,	int	defenderType	=	-1,	int	defenderID	=	-1,	int	effect_level	=	0,
					int	follow_attacker	=	false,	float	offset_x	=	0.0f,	float	offset_y	=	0.0f,	float	offset_z	=	0.0f,

					
					float	in_lifeTime	=	-1.0f,	int		in_follow_repetition	=	true,int effect_type = 0);

	 
	 
	int		AddSelfEffect(	GTH_EFFECT_CLASS_TYPE	classType,	int	effectID,	vec3_t	origin,	int	effect_level	=	0,	int	follow_attacker	=	false,
							int	attackerType	=	-1,	int	attackerIdx	=	-1,	float	offset_x	=	0.0f,	float	offset_y	=	0.0f,	float	offset_z	=	0.0f,

							
							float	in_lifeTime	=	-1.0f,	int		in_follow_repetition	=	true ,int effect_type = 0);

	 
	effectentity_t*		DeleteSelf	(	effectentity_t	*in_entity	);

	int   Delete( int idxEntity );
	void  ClearUseFlag();
	inline int Cull( vec3_t mins , vec3_t maxs , vec3_t position , int linkedCluster );
};

#endif 
