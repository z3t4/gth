 
 
 
 
 
 
 
 
 
#ifndef	_CHARACTER_MANAGER_CLASS_JOIDJ0U309U20
#define _CHARACTER_MANAGER_CLASS_JOIDJ0U309U20

#include "Common.h"
#pragma warning	( disable : 4786 )
#include <list>

#ifdef	_FX_CLIENT

struct	SHADOWTESTFORMAT
{
	D3DXVECTOR4		position;
	D3DCOLOR		color;
};

#define		D3DFVF_SHADOWTEST		(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

typedef	std::list< Fx_CHARACTER_t * >	LIST_CREATURE;
typedef	LIST_CREATURE::iterator			ITOR_LIST_CREATURE;

extern const int	SHADOW_TEX_DIMENSION;

#endif	 

 
 
 
 
class FX_CCharManager
{
public:
	
	enum EFFECT_TYPE
	{
		EFFECT_NORMAL=0,
		EFFECT_MISSILE,
		EFFECT_BASIC_ATTACK_EFFECT,
	};

	 
public:
	FX_CCharManager();
	~FX_CCharManager();

	 
	 
	 
public:
	Fx_CHARACTER_t		m_Characters		[MAX_CHAR_A_SCENE];
	Fx_CHARACTER_t		m_CharactersBackup	[MAX_CHAR_A_SCENE];
	int					m_iNumCharacters;
	int					m_iNumBackupCharacters;

	Fx_CHARACTER_t		m_creatures [MAX_CREATURE_A_SCENE];
	Fx_CHARACTER_t		m_creaturesBackup [MAX_CREATURE_A_SCENE];
	int					m_numCreatures;
	int					m_numBackupCreatures;

		 
	FX_CSkelManager		*m_pSkelManager;
	FX_CAnimManager		*m_pAnimManager;
	FX_CSkinManager		*m_pSkinManager;
	FX_CItemManager		*m_pItemManager;
	FX_CCharLookupMng	*m_pCharLookupMng;
	FX_CAuraManager		*m_pAuraManager;
	FX_CCharShaderMng	*m_pCharShaderMng;
	FX_CSkinRenderer	*m_pSkinRenderer;
	CTagManager			*m_pTagManager;
	CMeshManager		*m_pMeshManager;
	FX_CBoneManager		*m_pBoneManager;

	CD3DTextureMng		*m_pTexMng;
	CFileMng			*m_pFileMng;
	CScript				*m_pScript;
	CTimer				*m_timer;
	CShaderMng			*m_shaderMng;
	CD3DRender			*m_D3DRender;
	CMDLMng             *m_MDLMng;     

	FX_CCharUtil		*m_pCharUtil;
	FX_CMathTbl			*m_pMathTbl;

	CFrustum			*m_frustum;
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	int					m_totalDrawnPrims;
	float				m_minSquareDist;
	float				m_currentTime;
	float				m_elapsedTime;

		 
	vec3_t				m_tbOriginalCamPos;
	float				m_tbDuration;
	double				m_tbStartTime;
	float				m_tbPreperationDuration;
	float				m_tbPower;
	bool				m_tbUse;
	bool				m_tbDo;
	bool				m_tbPrepare;


	int					m_forceUpdateSkin;	 

 
#ifdef	_FX_CLIENT
		 
	LPDIRECT3DTEXTURE8	m_shadowTexture;		 
	LPDIRECT3DSURFACE8	m_shadowSurface;
	matrix4x4_t			m_shadowView;			 
	matrix4x4_t			m_shadowProj;			 
	
	LPDIRECT3DSURFACE8	m_backBuffer;
	LPDIRECT3DSURFACE8	m_zBuffer;
	LPDIRECT3DVERTEXBUFFER8		m_shadowVB;
	bool				m_drawShadows;			 


	CD3DBuffer			*m_shadowVBNeo;
	vec4_t				m_shadowLightVector;
	vec4_t				m_shadowPlane;
	vec4_t				m_shadowNormalizedPlane;
	matrix4x4_t			m_shadowMatrix;
	matrix4x4_t			m_shadowTranslation;
	float				m_sqrtTwo;



		 
	material_t			m_savedMaterial;

	LIST_CREATURE		m_emptyCreatureList;
	LIST_CREATURE		m_usedCreatureList;

#endif

 
#ifdef	_FX_TOOL
public:
	FX_CMeshTool		*m_pMeshTool;

#endif	 

 
#ifdef	_FXSKELDRAW
public:
	FX_CSkelDraw		*m_pSkelDraw;
	int					m_coordBoneIdx;

#endif	 

	 
	 
	 
public:
	bool		LoadAllResources();
	bool		PrepareBuffers();
	bool		ReleaseBuffers();

	bool		UpdateCharacterState();		 
	bool		UpdateAnimations();			 
	bool		RenderItems();				 
	bool		RenderAuras();
	void		PrepareSkinRendering	();
	bool		RenderSkinsNormal	();
	bool		RenderEffects	();
	
	void		ProcessComboAttack	();

#ifdef	_FX_CLIENT
	bool		RenderSkinsDisappear	();
#endif	 

	void		Cleanup();					 

		 
	void		UpdatePosition	(	const int	iCharID,	const vec3_t	pos,	const vec3_t	angle	);
		 
	void		StopAnimation	(	const int	charID,	const bool	stop	);	 






		 
	void		SetTexManager	(	CD3DTextureMng	*texMng		);
	void		SetFileManager	(	CFileMng		*fileMng	);
	void		SetScript		(	CScript			*script		);
	void		SetDevice		(	LPDIRECT3DDEVICE8	device	);
	void		SetTimer		(	CTimer			*timer		);
	void		SetShaderMng	(	CShaderMng		*shaderMng	);
	void		SetRenderer		(	CD3DRender		*d3dRender	);
	void		SetFrustum		(	CFrustum		*frustum	)	{	m_frustum	=	frustum;	}	
	void        SetMDLMng( CMDLMng *MDLMng )   { m_MDLMng = MDLMng; }



	
	void		RestoreDeviceObject	();			 


	void		GetBoundChar	(	Fx_CHARACTER_t	*in_char,	bboxf_t		&out_bound	);	 
	void		GetBoundCharPreTransform	(	Fx_CHARACTER_t	*in_char,	bboxf_t	&out_bound	);



		 
	void		UpdateAura (	Fx_CHARACTER_t	*pThisChar,		KEYFRAME	*pThisFrame,	TAG		*pThisTag,	const bool	in_rightHanded	);
	void		PreUpdateAura	(	Fx_CHARACTER_t	*pThisChar, FX_CSkel	*pSkel	);
	
	int			GetNumDrawnPrimitive ()			{	return	m_totalDrawnPrims;	};

	void			StartFrame ();			 



	int			GetSkinID	(	const int	in_type,	const int	in_internalID	);

	void		SetDefaultCharacter	(	const int iCharID );
	void		SetDefaultSkin		(	const int iCharID,	const int in_type );

	int			CheckSkinUpdateTime (	Fx_CHARACTER_t	*in_char	);
	void		UpdateSkinUpdateTime	(	Fx_CHARACTER_t	*in_char	);
	void		SetFPS (	Fx_CHARACTER_t	*in_char,	const int	in_fps	);
	int			GetForceUpdateSkin ()		{	return	m_forceUpdateSkin;	};
	void		SetForceUpdateSkin ()		{	m_forceUpdateSkin	=	true;	};
	void		ReleaseForceUpdateSkin ()	{	m_forceUpdateSkin	=	false;	};
	void		EndFrame ();

	void		MakeInterpolator	(	Fx_CHARACTER_t	*in_char,	const float	in_duration,	const int	in_startAnim,		
										const int	in_endAnim,	const int	in_option	);
	
	bool		InterpolateAnim	(	Fx_CHARACTER_t	*in_char	);

	void		FadeAura	(	const int	in_charID	);
	void		InitAnimation	(	const int	iCharID, 	const int	aniSetType,	const int	aniSetIndex, 	const int	aniSetID	);
	

	void		FixCharacterPosition	();


	void		InitNewChar	(	Fx_CHARACTER_t	*in_char	);

	

	bool		CheckAnimation	(	const Fx_CHARACTER_t	*in_char,	const int	in_type,		const int	in_index, 	const int	in_ID,	const int	in_groupType	);

	
	
	float		GetInitialHeight( Fx_CHARACTER_t *in_char, int in_animIdx );
	float		GetInitialTall( Fx_CHARACTER_t *in_char, int in_animIdx  );
	float		GetInitialRadius( Fx_CHARACTER_t *in_char, int in_animIdx  );


	void		SetRenderState	();
	void		CheckAuraExist	(	const int	in_charIdx	);
	void		UpdateSysVBInfo	();		 




	void		EquipItem	( const int iCharID,	const itemTable_t *pItem	);

	bool		CharacterCollideCheck	(	Fx_CHARACTER_t	*in_char	);

		 
	int			SetCharacterSpeed	(	Fx_CHARACTER_t	*in_char,	float	in_percent	);

 
#ifdef	_FXSKELDRAW
	void		ClearSkelData	();
	void		SetSkelDrawData ();
	void		RenderSkel		();
	void		SetSkelBBoxData	(	const bboxf_t		in_bound	);
	void		RenderSkelBBox	();
#endif

 
private:
	void		TransformAuraVertices (	const KEYFRAME		*pThisFrame,	const TAG			*pThisTag,	
										const vec3_t		in_vecstart,	const vec3_t		in_vecend,
										const matrix4x4_t	*in_matTransform,
										vec3_t	out_vecstart,	vec3_t	out_vecend	);

	void		SetSkinUpdateTime	(	Fx_CHARACTER_t	*in_char,	const float	in_time	);
	int			GetFPS (	Fx_CHARACTER_t	*in_char	);
	float		GetSPF (	Fx_CHARACTER_t	*in_char	);
	

	void		PrepareNewAura	(	Fx_CHARACTER_t	*in_char	);


public:


 
#ifdef	_FX_CLIENT
public:
	void	AddEffect	( int type, int in_effectID, Fx_CHARACTER_t *in_char , float in_zpos );

	void	ChangeAnimation		(	const int	iCharID, 	const int	aniSetType,		const int	aniSetIndex, 	const int	aniSetID,	const float	*in_animSpeed, 
									const bool	in_forceChage	);

	void	EquipAllItem	(	const int	iCharID	);
	void	SetInitialDataForNewChar	(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	void	SetInitialDataForExistChar	(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	void	SetPCSpecificDataNormal		(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	void	SetPCSpecificDataTransform	(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	void	SetNonPCSpecificData		(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	void	CheckNewAddedEffect			(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	
	void	GenerateCharacter		(	syncCharacter_t	*data,	int		numData		);
	void	GenerateCharacterAffect	(	Fx_CHARACTER_t *pNewChar, int affectNum, int affectIdx[] );
	void	GenerateMyCharacter		();
	void	GenerateOtherCharacter	(	int	iCharID	);
	float	GetAnimFraction	(	Fx_CHARACTER_t	*in_char	);
	float	GetCharPanelScreenPos	(	Fx_CHARACTER_t	*in_char,	vec3_t		out_pos	);
	Fx_CHARACTER_t*		SearchCursorEntityNeo2	(	const vec3_t	in_camPos,			const vec3_t	in_camAngle,
													matrix4x4_t		*in_camTransMat,	const float		in_minSquareDist,
													const vec2_t	in_mousePos,		const vec2_t	in_viewportSize	);
	void	SetAnimationSpeedInSec	(	Fx_CHARACTER_t	*in_char,	const float	in_second, int isNext	);
	void	SetTargetID		();

	void	SetTrembling	(	const vec3_t	in_position,	const float		in_preperation,		const float		in_duration,	const float		in_power	);
	void	StopTrembling	();
	void	CheckTrembling	();
	void	SetCharTrembling	(	const int	in_charIdx,	const float		in_duration,	const float		in_power	);
	void	StopCharTrembling	(	const int	in_charIdx	);
	void	CheckCharTrembling	();

	void	RenderShadow	();
	void	MakeShadow	(	Fx_CHARACTER_t	*in_char	);
	



	
#endif	 

 
#ifdef	_FX_TOOL
public:
	int		CheckCorrectBone	(	Fx_CHARACTER_t	*pThisChar	);
	void	GenerateAniToolsCharacter	(	int	entityType,	int	skinIdx,	int	aniIdx	);
	Fx_CHARACTER_t*		SearchCursorEntity	(	const vec3_t	pos,		const vec3_t	in_camangle,
												matrix4x4_t		*transMat,	const float		minSquareDist, 
												const vec2_t	mouse,		const vec2_t	viewport	);

#endif	 

public:

	int		StartNewComboAttack	(	Fx_CHARACTER_t	*in_char	);
	int		SetNextBeginFrame	(	Fx_CHARACTER_t	*in_char	);
	int		SetNextDamageFrame	(	Fx_CHARACTER_t	*in_char	);
	int		SetNextFreeFrame	(	Fx_CHARACTER_t	*in_char	);
	int		GotDamageMessage	(	int		in_defenderIdx,	int		in_defenderType,	int		in_attackerIdx,	int		in_attackerType,	int		in_damage,	int		in_stun		);
	int		FindDamageBufferEntry	(	Fx_CHARACTER_t	*in_char,	bool	in_used		);
	int		DeleteDamageBufferEntry	(	Fx_CHARACTER_t	*in_char,	int		in_index	);
	int		ProcessDamageFrame	(	Fx_CHARACTER_t	*in_char	);
	void	ClearDamageBuffer	(	Fx_CHARACTER_t	*in_char	);
	int		SetNextStartFrame	(	Fx_CHARACTER_t	*in_char	);
	void	ShowAttackEffect	(	Fx_CHARACTER_t	*in_attacker,	int		damage = 0	);		 

	Fx_CHARACTER_t*	FindCharacterByServerIndex	(	int		in_serverIdx,	int		in_type		);
	int		ShowDamage	(	Fx_CHARACTER_t	*in_char,	int		in_entryIdx	);		 

	float	CalcAttackLength	(	Fx_CHARACTER_t	*in_char	);

	int		SetCharacterColorChanging	(	Fx_CHARACTER_t	*in_char,	vec3_t	in_startColor,	vec3_t	in_endColor,	float	in_applyTime	);
	void	ApplyCharacterColorChanging	(	Fx_CHARACTER_t	*in_char	);
	void	RestoreCharacterColorChanging	(	Fx_CHARACTER_t	*in_char	);
	void	SetCharacterColorChangingDefined	(	Fx_CHARACTER_t	*in_char,	int		in_color,	float	in_applyTime	);

	void	MakeEmptyCreatureList	();
	Fx_CHARACTER_t*	GetEmptyCreature	();
	void	SetEmptyCreature	(	Fx_CHARACTER_t	*in_creature	);
	bool	UpdateAnimationsCreature	();
	int		CheckSkinUpdateTimeCreature	(	Fx_CHARACTER_t	*in_creature	);
	void	FixCreaturePosition	();
	void	GenerateCharacterCreature	();
	void	CheckNewCreature	(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	void	CheckExistCreature	(	Fx_CHARACTER_t	*in_char,	syncCharacter_t	*in_data	);
	
		 
	void		ProcessComboAttack	(	Fx_CHARACTER_t	*in_char	);
	inline void		CalcComboAttackTime	(	Fx_CHARACTER_t	*in_char	);
	inline float	CalcFreeComboAttackTime	(	Fx_CHARACTER_t	*in_char	);
		
	bool	GetPositionForTeleport	(	int		in_targetIndex,	int		in_targetType,	vec3_t	out_position	);

	
	Fx_CHARACTER_t * GetCharacter(int Index)
	{
		if(Index < 0 || Index >= MAX_CHAR_A_SCENE) return NULL;
		return &m_Characters[Index];
	}
};


 
 
 


#ifdef	_FX_CLIENT
 
 
 
 
inline bool		FX_CCharManager::CheckAnimation	(	const Fx_CHARACTER_t	*in_char,
													const int		in_type,
													const int		in_index,
													const int		in_ID,
													const int		in_groupType	)
{

	int aniInfoIdx = m_pCharLookupMng ->GetAnimIDFromTable (	in_char ->entityType,	in_char ->tableIdx,	in_type,	in_index,	in_ID, in_char->ID	);

	
	if( aniInfoIdx < 0 || aniInfoIdx >= MAX_COUNT_ANIM )
		return false;

	if	(	m_pCharLookupMng ->m_aniInfo [aniInfoIdx].groupID	==	in_groupType	)
		return	true;

	return	false;
}

#endif	 

#endif
