 
 
 
 
 
 
 
 
 

 
 
 


 
 
 
#include "../../global_def.h"
#include "../Common.h"
#include <stdio.h>
#include <time.h>
#include <crtdbg.h>

 
 
 
const vec4_t		UNIT_X_QUAT		=	{	1.0f,	0.0f,	0.0f,	0.0f	};
const matrix4x4_t	IDENTITY_MATRIX;
const vec3_t		ZERO_VECTOR3	=	{	0.0f,	0.0f,	0.0f	};

vec4_t			GLOBAL_QUAT_1		=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_QUAT_2		=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_QUAT_3		=	{	0.0f,	0.0f,	0.0f,	0.0f	};

vec3_t			GLOBAL_DXVEC3_1		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_2		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_3		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_4		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_5		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_6		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_7		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_8		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_9		=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_10	=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_DXVEC3_11	=	{	0.0f,	0.0f,	0.0f	};

vec4_t			GLOBAL_DXVEC4_1		=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_DXVEC4_2		=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_DXVEC4_3		=	{	0.0f,	0.0f,	0.0f,	0.0f	};

matrix4x4_t		GLOBAL_DXMATRIX_1;

vec3_t			GLOBAL_VEC3_1	=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_VEC3_2	=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_VEC3_3	=	{	0.0f,	0.0f,	0.0f	};
vec3_t			GLOBAL_VEC3_4	=	{	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_VEC4_1	=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_VEC4_2	=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_VEC4_3	=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_VEC4_4	=	{	0.0f,	0.0f,	0.0f,	0.0f	};
vec4_t			GLOBAL_VEC4_5	=	{	0.0f,	0.0f,	0.0f,	0.0f	};

matrix4x4_t		GLOBAL_MATRIX_1;
matrix4x4_t		GLOBAL_MATRIX_2;
matrix4x4_t		GLOBAL_SCALEMATRIX;

bboxf_t			GLOBAL_BBOX		=	{	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f	};

#ifdef	_FXDEBUG
char			DBGSTRING [MAX_PATH];
int				g_count_startframe	=	0;
int				g_count_render		=	0;
#endif

const double	HALFPI_DEGREE	=	__HALFPI * 180;
const float		FX_DEGTORAD		=	__DEG2RAD;

#define		g_vec_start				GLOBAL_DXVEC3_1
#define		g_vec_end				GLOBAL_DXVEC3_2
#define		g_vec_start_trans		GLOBAL_VEC3_1
#define		g_vec_end_trans			GLOBAL_VEC3_2

#define		g_quatTarget			GLOBAL_VEC4_1
#define		g_quatConjugate			GLOBAL_VEC4_2
#define		g_quatAfterTransform	GLOBAL_VEC4_3
#define		g_vecAfterTransform		GLOBAL_VEC3_1
#define		g_mat					GLOBAL_MATRIX_1
#define		g_vecTemp				GLOBAL_VEC4_4
#define		g_buffer				GLOBAL_VEC4_5

#define		g_temporaryPanelPos		GLOBAL_VEC3_1
#define		g_temporaryPanelBBox	GLOBAL_BBOX

 
#define		g_charBBox				GLOBAL_BBOX

 
#define		g_matRotation			GLOBAL_MATRIX_1
#define		g_matTranslation		GLOBAL_MATRIX_2

#ifdef	_FXSKELDRAW
#	define		g_bbox				GLOBAL_BBOX
#endif

const int SHADOW_TEX_DIMENSION	=	256;



#ifdef _FX_CLIENT

int GetSkinInfoWithItem( int pcClass, int pcJob, const itemTable_t *pItem )
{
	
	if ( pItem->skinModelID[pcClass] > 0 )	
		return pItem->skinModelID[pcClass];
	
	return pItem->skinModelID_job[pcJob];
}

#endif

 
 
 
FX_CCharManager::FX_CCharManager()
{
	srand( (unsigned)time( NULL ) );

	m_pSkelManager		=	new FX_CSkelManager ();
	m_pAnimManager		=	new	FX_CAnimManager ();
	m_pSkinManager		=	new FX_CSkinManager ();
	m_pItemManager		=	new	FX_CItemManager ();
	m_pCharLookupMng	=	new	FX_CCharLookupMng ();
	m_pAuraManager		=	new FX_CAuraManager ();

		 
	m_pTagManager		=	new	CTagManager ();
	m_pMeshManager		=	new CMeshManager ();
		 
	m_pBoneManager		=	new	FX_CBoneManager ();

		 
	m_pMathTbl			=	new	FX_CMathTbl ();
	m_pCharUtil			=	new	FX_CCharUtil ();

	m_pSkinRenderer		=	new	FX_CSkinRenderer ();
	m_pSkinManager ->SetSkinRenderer (	m_pSkinRenderer	);

	m_pCharShaderMng	=	new	FX_CCharShaderMng ();

#ifdef	_FXSKELDRAW
	m_pSkelDraw			=	new	FX_CSkelDraw (	1.0f,	2.0f,	30.0f	);
	m_coordBoneIdx		=	0;
#endif

#ifdef	_FX_TOOL
	m_pMeshTool			=	new	FX_CMeshTool ();
#endif

	 
	m_pSkelManager ->SetBoneManager		(	m_pBoneManager		);

	m_pSkinManager ->SetCharManager		(	this				);
	m_pSkinManager ->SetCharUtil		(	m_pCharUtil			);
	m_pSkinManager ->SetSkelManager		(	m_pSkelManager		);
	m_pSkinManager ->SetCharShaderMng	(	m_pCharShaderMng	);

	m_pItemManager ->SetTagManager		(	m_pTagManager		);
	m_pItemManager ->SetBoneManager		(	m_pBoneManager		);
	m_pItemManager ->SetCharManager		(	this				);
	m_pItemManager ->SetSkelManager		(	m_pSkelManager		);
	m_pItemManager ->SetSkinManager		(	m_pSkinManager		);
	m_pItemManager ->SetCharUtil		(	m_pCharUtil			);
	m_pItemManager ->SetCharShaderMng	(	m_pCharShaderMng	);
	
	m_pTagManager ->SetMathTbl	(	m_pMathTbl	);

	m_pAuraManager ->SetCharManager	(	this	);
	m_pAuraManager ->SetCharShaderMng	(	m_pCharShaderMng	);

	m_pCharUtil ->SetMathTbl	(	m_pMathTbl	);

	m_pCharShaderMng ->SetCharUtil (	m_pCharUtil		);
	
	m_iNumCharacters	=	0;
	m_currentTime		=	-1.0f;

#ifdef	_FX_CLIENT
		 
	m_tbDo		=	false;
	m_tbPrepare	=	false;

	m_tbUse		=	true;
	VectorSet	(	m_tbOriginalCamPos,	0.0f,	0.0f,	0.0f	);
	m_tbStartTime	=	0;
	m_tbDuration	=	0;
	m_tbPreperationDuration	=	0;

	m_drawShadows	=	false;

	memset ( m_shadowLightVector, 0, sizeof(vec4_t) );
	m_shadowLightVector [3]	=	1.0f;
	memset ( m_shadowPlane, 0, sizeof(vec4_t) );
	m_shadowPlane [2]	=	1.0f;
	memset ( &m_shadowMatrix, 0, sizeof(matrix4x4_t) );
	m_sqrtTwo	=	sqrt ( 2.0f );

	m_shadowVBNeo	=	NULL;


#endif	 

	MakeEmptyCreatureList();
	
	 
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_CHARACTERMANAGER, sizeof(FX_CCharManager) );
	}
}

 
 
 
FX_CCharManager::~FX_CCharManager()
{
}

 
 
 
void	FX_CCharManager::SetTexManager	(	CD3DTextureMng	*texMng	)
{
	m_pTexMng	=	texMng;

	m_pItemManager ->SetTexManager	(	m_pTexMng	);
	m_pSkinManager ->SetTexManager	(	m_pTexMng	);
	m_pAuraManager ->SetTexManager	(	m_pTexMng	);
	m_pCharShaderMng ->SetTexManager(	m_pTexMng	);
}

 
 
 
void	FX_CCharManager::SetFileManager	(	CFileMng	*fileMng	)
{
	m_pFileMng	=	fileMng;

	m_pAnimManager -> SetFileManager	(	m_pFileMng	);
	m_pItemManager -> SetFileManager	(	m_pFileMng	);
	m_pSkinManager -> SetFileManager	(	m_pFileMng	);
	m_pSkelManager -> SetFileManager	(	m_pFileMng	);
	m_pCharLookupMng -> SetFileManager	(	m_pFileMng	);

	m_pMathTbl ->SetFileManager	( m_pFileMng );
	m_pCharUtil ->SetFileManager( m_pFileMng );

#ifdef	_FX_TOOL
	m_pMeshTool ->SetFileManager (	m_pFileMng	);
#endif
}

 
 
 
void	FX_CCharManager::SetScript	(	CScript	*script	)
{
	m_pScript	=	script;

	m_pAnimManager -> SetScript		(	m_pScript	);
	m_pItemManager -> SetScript		(	m_pScript	);
	m_pSkelManager -> SetScript		(	m_pScript	);
	m_pSkinManager -> SetScript		(	m_pScript	);
	m_pTagManager -> SetScript		(	m_pScript	);
	m_pMeshManager -> SetScript		(	m_pScript	);
	m_pBoneManager -> SetScript		(	m_pScript	);
	m_pAuraManager ->SetScript		(	m_pScript	);

	m_pCharUtil ->SetScript	(	m_pScript	);

	m_pCharShaderMng ->SetScript (	m_pScript	);

#ifdef	_FX_TOOL
	m_pMeshTool -> SetScript		(	m_pScript	);
#endif

}

 
 
 
void	FX_CCharManager::SetTimer	(	CTimer	*timer	)
{
	m_pItemManager	->SetTimer	(	timer	);

	m_timer	=	timer;

	m_currentTime	=	m_timer ->GetAppTime ();
}

 
 
 
 
void	FX_CCharManager::SetRenderer (	CD3DRender		*d3dRender	)
{
	m_D3DRender	=	d3dRender;

	m_pItemManager ->SetRenderer (	d3dRender	);
	m_pSkinManager ->SetRenderer (	d3dRender	);
	m_pAuraManager ->SetRenderer (	d3dRender	);

#ifdef	_FXSKELDRAW
	m_pSkelDraw ->SetRenderer (	d3dRender	);
#endif

	return;
}

 
 
 
 
void	FX_CCharManager::SetShaderMng	(	CShaderMng		*shaderMng	)
{
	m_shaderMng	=	shaderMng;

	m_pSkinManager ->SetShaderMng (	shaderMng	);
	m_pAuraManager ->SetShaderMng (	shaderMng	);
	m_pCharShaderMng ->SetShaderMng (	shaderMng	);

	return;
}

 
 
 
bool	FX_CCharManager::LoadAllResources()
{
	int		result;

		 
	m_pTagManager -> Load	(	FILENAME_TAGLIST	);
	m_pTagManager -> LoadTagScript ();
	m_pMeshManager -> Load	(	FILENAME_MESHLIST	);
	m_pBoneManager -> Load	(	FILENAME_BONELIST	);

	 
#ifdef	_FX_TOOL
	m_pMeshTool ->Do ();
#endif

	 
	result	=	m_pSkelManager ->LoadSkelList ();
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load skeleton resources!"		)
#endif

	 
	result	=	m_pAnimManager ->LoadAnimList ();
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load animation resources!"		)
#endif

	 
	result	=	m_pAnimManager ->LoadAMInfos ();
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load attack animation infos!"		)
#endif

	 
	result	=	m_pCharShaderMng ->LoadTextureList ();
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load texture resources!"		)	
#endif

	 
	result	=	m_pCharShaderMng ->LoadEffectTextureList ();
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load effect shader list resources!"		)	
#endif

	 
	result	=	m_pSkinManager ->LoadSkinList ();
	result	+=	m_pSkinManager ->LoadDefSkinList ();

#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load skinning mesh resources!"	)
#endif

	 
	result	=	m_pItemManager ->LoadItemList ();
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load non-skinning mesh resources!"	)
#endif

		 
	m_pCharLookupMng ->InitTableInfo ();
	m_pCharLookupMng ->LoadTableInfoFromBuffer ( "internal_infos/charlookup.bdt" );
	m_pSkelManager ->MakeLookupTable ();
	m_pMathTbl ->Load ();

	return	true;
}

 
 
 
void	FX_CCharManager::RestoreDeviceObject	()
{
	if	(	g_cgv.currentWorldIdx	<	0	)
	{
		m_pCharShaderMng ->PrepareCharShaderLogin ();
	}
	else
	{
		m_pCharShaderMng ->PrepareCharShaderCommon ();
	}
	m_pCharShaderMng ->PrepareEffectShader ();

	m_pItemManager ->UpdateTextureID ();
	m_pSkinManager ->UpdateTextureID ();
}


 
 
 
void	FX_CCharManager::SetDevice	(	LPDIRECT3DDEVICE8	device	)
{
	m_pd3dDevice	=	device;

		 
	m_pItemManager ->SetDevice	(	m_pd3dDevice	);
	m_pSkinManager ->SetDevice	(	m_pd3dDevice	);
	m_pAuraManager ->SetDevice	(	m_pd3dDevice	);

#ifdef	_FX_CLIENT
	if	(	FAILED ( m_pd3dDevice ->CreateVertexBuffer	(	24 * sizeof (SHADOWTESTFORMAT),
															0,	D3DFVF_SHADOWTEST,
															D3DPOOL_MANAGED,	&m_shadowVB	) )	)
	{
		ShowMessage ( "ERROR!",	"CreateVertexBuffer Failed!",	__FILE__,	__LINE__ );
		return;
	}

    SHADOWTESTFORMAT	*v;

    if	(	FAILED ( m_shadowVB ->Lock ( 0,	0,	(BYTE **)&v,	0 ) )	)
	{
		ShowMessage ( "ERROR!",	"Lock Failed!",	__FILE__,	__LINE__ );
		return;
	}

	float	shadowSize	=	SHADOW_TEX_DIMENSION;
	float	depth		=	shadowSize * 0.01f;
	memset ( v, 0, 24 * sizeof(SHADOWTESTFORMAT) );

	v [0].position	=	v [12].position	=	D3DXVECTOR4 ( 0.0f, 0.0f, 0.0f, 1.0f );
	v [1].position	=	v [4].position	=	v [19].position	=	v [22].position	=	D3DXVECTOR4 ( shadowSize, 0.0f, 0.0f, 1.0f );
	v [2].position	=	v [3].position	=	D3DXVECTOR4 ( 0.0f, depth, 0.0f, 1.0f );
	v [5].position	=	D3DXVECTOR4 ( shadowSize, depth, 0.0f, 1.0f );
	v [13].position	=	v [16].position	=	D3DXVECTOR4 ( depth, 0.0f, 0.0f, 1.0f );
	v [18].position	=	D3DXVECTOR4 ( shadowSize - depth, 0.0f, 0.0f, 1.0f );

	v [6].position	=	D3DXVECTOR4 ( 0.0f, shadowSize - depth, 0.0f, 1.0f );
	v [7].position	=	v [10].position	=	D3DXVECTOR4 ( shadowSize, shadowSize - depth, 0.0f, 1.0f );
	v [8].position	=	v [9].position	=	v [14].position	=	v [15].position	=	D3DXVECTOR4 ( 0.0f, shadowSize, 0.0f, 1.0f );
	v [11].position	=	v [23].position	=	D3DXVECTOR4 ( shadowSize, shadowSize, 0.0f, 1.0f );
	v [17].position	=	D3DXVECTOR4 ( depth, shadowSize, 0.0f, 1.0f );
	v [20].position	=	v [21].position	=	D3DXVECTOR4 ( shadowSize - depth, shadowSize, 0.0f, 1.0f );

	if	(	FAILED ( m_shadowVB ->Unlock () )	)
	{
		ShowMessage ( "ERROR!",	"Unlock Failed!",	__FILE__,	__LINE__ );
		return;
	}
#endif	 

#ifdef	_FXSKELDRAW
	m_pSkelDraw ->SetDevice		(	m_pd3dDevice	);
#endif

}

 
 
 
bool	FX_CCharManager::PrepareBuffers()
{
	m_pItemManager->PrepareRenderBuffers();
	m_pSkinManager->PrepareRenderBuffers();
	m_pAuraManager ->PrepareRenderBuffers ();

#ifdef	_FXSKELDRAW
	m_pSkelDraw ->PrepareRenderBuffers ();
#endif

	m_shadowVBNeo	=	new	CD3DBuffer ();

	int		indicesize	=	(AURA_MAX_DIVIDE + 1) * (MAX_AURA_IN_CHAR - 1) * 6;

	 
	m_shadowVBNeo->Create ( SHADOW_VERTEX_BUFFER_SIZE, GTH_FORMAT_XYZ | GTH_FORMAT_TEXVERTEX(1), GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER );

	 
	if	(	FAILED ( D3DXCreateTexture (	m_pd3dDevice,	SHADOW_TEX_DIMENSION,	SHADOW_TEX_DIMENSION,	1, 
											D3DUSAGE_RENDERTARGET,	D3DFMT_A1R5G5B5,	D3DPOOL_DEFAULT,
											&m_shadowTexture ) )	)
	{
		 
		m_shadowTexture	=	NULL;
		return	false;
	}

	 
	if	(	FAILED ( m_pd3dDevice ->GetRenderTarget ( &m_backBuffer ) )	)
	{
		m_backBuffer	=	NULL;
		return	false;
	}

     
	if	(	FAILED ( m_pd3dDevice ->GetDepthStencilSurface ( &m_zBuffer ) )	)
	{
		m_zBuffer	=	NULL;
		return	false;
	}

	 
	if	(	FAILED ( m_shadowTexture ->GetSurfaceLevel ( 0,	&m_shadowSurface ) )	)
	{
		m_shadowSurface	=	NULL;
		return	false;
	}



	return	true;
}

 
 
 
bool	FX_CCharManager::ReleaseBuffers()
{
	m_pItemManager->ReleaseRenderBuffers();
	m_pSkinManager->ReleaseRenderBuffers();
	m_pAuraManager ->ReleaseRenderBuffers ();

#ifdef	_FXSKELDRAW
	m_pSkelDraw ->ReleaseRenderBuffers ();
#endif

	if	(	m_shadowVBNeo	)	m_shadowVBNeo ->Release ();
	GTH_SAFE_DELETE ( m_shadowVBNeo );

	GTH_SAFE_RELEASE ( m_backBuffer );
	GTH_SAFE_RELEASE ( m_zBuffer );
	GTH_SAFE_RELEASE ( m_shadowVB );
	GTH_SAFE_RELEASE ( m_shadowSurface );
	GTH_SAFE_RELEASE ( m_shadowTexture );

	return true;
}

 
 
 
 
bool	FX_CCharManager::UpdateAnimations	()
{
	Fx_CHARACTER_t	*pThisChar	=	NULL;			 
	FX_CSkel		*pSkel		=	NULL;			 
	FX_CAnim		*pAnim		=	NULL;			 

	int				endAnim		=	0;
	int				nextIdx		=	0;		 
	Fx_ANIM_INFO_t	*nextState	=	NULL;	 

	trace_t		trace;
	vec3_t		minForTrace , maxForTrace;
	vec3_t		origin , destination;
	minForTrace [0]	=	minForTrace [1]	=	minForTrace [2]	=	-10.0f;
	maxForTrace [0]	=	maxForTrace [1]	=	maxForTrace [2]	=	10.0f;

	for	(	int	index	=	0;	\
				index	<	m_iNumCharacters;	\
				++index		)
	{
		pThisChar	=	&m_Characters [index];

		 
		 
		 
		 
		GetBoundChar( pThisChar, g_charBBox );
		if	(	m_frustum ->Cull_Exact (	&g_charBBox [0],	&g_charBBox [3]	)	) 
		{
			pThisChar ->ren_auraRHandData.render	=	false;
			pThisChar ->ren_auraLHandData.render	=	false;
			pThisChar ->flag_culled	=	true;
			pThisChar ->flag_outOfMySight	=	true;
		}
		else
		{
			pThisChar ->flag_culled	=	false;
			 
			if	(	index	!=	0	&& pThisChar->entityType != ENTITY_PC )
			{
				if	(	g_camera .m_world	)
				{
					origin [0]	=	m_Characters [0].position [0];
					origin [1]	=	m_Characters [0].position [1];
					origin [2]	=	m_Characters [0].stat_fakeHeight;
					destination [0]	=	pThisChar ->position [0];
					destination [1]	=	pThisChar ->position [1];
					destination [2]	=	pThisChar ->stat_fakeHeight;
					CCamera	*pCamera	=	&g_camera;
					trace	=	g_camera .m_world ->Trace ( origin , minForTrace , maxForTrace , destination , MASK_PLAYERSOLID );
					if	(	trace.fraction	<	1.0f	)
						pThisChar ->flag_outOfMySight	=	true;
					else
						pThisChar ->flag_outOfMySight	=	false;
				}
				else
				{
					pThisChar ->flag_outOfMySight	=	false;
				}
			}
			else
			{
				pThisChar ->flag_outOfMySight	=	false;
			}
		}

		if	(	pThisChar ->ani_curIdx	<	0	)
		{
			g_charManager->ChangeAnimation(pThisChar->ID, ANIMTYPE_BYITEM, 0, 0, NULL, true );
			return true;
		}

		if	(	(	pThisChar ->attr_skelID	<	0	)	||
				(	pThisChar ->ani_curIdx	<	0	)	)
		{
			ErrorLog( "attr_skelID : %d, ani_curIdx : %d", pThisChar->attr_skelID, pThisChar->ani_curIdx );
			pThisChar->ren_renderThisFrame	=	false;
			continue;
		}

			 
		pSkel	=	&m_pSkelManager ->m_Skels [pThisChar ->attr_skelID].skeleton;
		pAnim	=	&m_pAnimManager ->m_Anims [pThisChar ->ani_curIdx].animation;

		 
		 
		 
		 
		if	(	pThisChar ->flag_interpolation	)
		{
			if	(	InterpolateAnim( pThisChar )	)
			{
				

				
				pThisChar ->ip_endAnim ->TransformKeyframes ( pThisChar	);

				FadeAura (index);

				continue;
			}
		}

		 
		 
		 
		 
			 
		switch (	pThisChar ->ani_option	)
		{
		case	ANIMOPTION_START_NEW_ANIM:
			pThisChar ->ani_fraction	=	0.0f;
			pThisChar ->stat_sound	=	CHAR_SOUND_ENABLE;
			if( pThisChar->ani_curSetID == ANIM_ITEM_RUN || pThisChar->ani_curSetID == ANIM_ITEM_WALK )
			{
				if( pThisChar->entityType == ENTITY_PC )
				{
					g_soundMng->m_pcSound[pThisChar->tableIdx].playCount[ANIM_ITEM_RUN] = 2;
					g_soundMng->m_pcSound[pThisChar->tableIdx].playCount[ANIM_ITEM_WALK] = 2;
				}
				else
				{
					g_soundMng->m_monSound[pThisChar->tableIdx].playCount[ANIM_ITEM_RUN] = 2;
					g_soundMng->m_monSound[pThisChar->tableIdx].playCount[ANIM_ITEM_WALK] = 2;
				}
			}

			memset	(	pThisChar ->key_curIndex,	0,	sizeof (int) * MAX_COUNT_BONE_IN_CHAR	);
			memset	(	pThisChar ->key_nextIndex,	0,	sizeof (int) * MAX_COUNT_BONE_IN_CHAR	);
			memset	(	pThisChar ->key_displace,		0,	sizeof (float) * MAX_COUNT_BONE_IN_CHAR	);
			pThisChar ->ani_elapsed		=	0.0f;
			pThisChar ->ani_oldFrame			=	0.0f;
			pThisChar ->ani_option		=	ANIMOPTION_NORMAL;
			 
			if	(	(	pThisChar ->ani_curGroup	==	FX_ANIM_GROUP_ATTACK	)	&&
					(	m_pAnimManager->ShowAttackAura( pThisChar->ani_curIdx )	)	)
			{
				

				if	(	pThisChar ->flag_useRHandAura	)
				{
					pThisChar ->ren_auraRHandData.made			=	false;
					pThisChar ->ren_auraRHandData.nextUpdate	=	AURA_UPDATE_TIME;
					pThisChar ->ren_auraRHandData.render		=	true;
				}
				if	(	pThisChar ->flag_useLHandAura	)
				{
					pThisChar ->ren_auraLHandData.made		=	false;
					pThisChar ->ren_auraLHandData.nextUpdate	=	AURA_UPDATE_TIME;
					pThisChar ->ren_auraLHandData.render		=	true;
				}
			}
			else
			{
				

				if	(	pThisChar ->flag_useRHandAura	)
				{
					pThisChar ->ren_auraRHandData.render	=	false;
				}
				if	(	pThisChar ->flag_useLHandAura	)
				{
					pThisChar ->ren_auraLHandData.render		=	false;
				}
			}

			 
			if	(	m_pAnimManager ->IsComboAttack ( pThisChar ->ani_curIdx )  )
			{
				StartNewComboAttack ( pThisChar );

				 
				if	(	! m_pAnimManager->IsSkillAttack( pThisChar->ani_curIdx )	)
				{
					 
					const float	MULTIPLIER_FOR_COMBO	=	0.8f;
					float	overallTime	=	pThisChar->stat_attackSpeed * m_pAnimManager ->GetComboCount ( pThisChar ->ani_curIdx ) * MULTIPLIER_FOR_COMBO;

					pThisChar ->atk_originalAttackTime	=	m_pAnimManager ->m_Anims [pThisChar ->ani_curIdx].animation.m_totalTime * 1000.0f;
					pThisChar ->atk_freeAttackTime		=	m_pAnimManager ->CalcFreeTimeVariable ( pThisChar ->ani_curIdx, overallTime );
					if( pThisChar->atk_freeAttackTime < 500.0f )
					{
						pThisChar ->atk_originalAttackTime = overallTime;
						pThisChar->atk_freeAttackTime = overallTime;
					}

					SetAnimationSpeedInSec ( pThisChar, pThisChar ->atk_originalAttackTime, false );
					
					SetCharacterSpeed ( pThisChar, pThisChar->generalSpeed );				
					
				}
				else
				{
					pThisChar->atk_originalAttackTime	=	m_pAnimManager->m_Anims[ pThisChar->ani_curIdx ].animation.m_totalTime * 1000.0f;
					pThisChar->atk_freeAttackTime		=	m_pAnimManager->m_Anims[ pThisChar->ani_curIdx ].animation.m_totalTime * 1000.0f;
					SetAnimationSpeedInSec ( pThisChar, pThisChar ->atk_originalAttackTime, false );
					

					
					SetCharacterSpeed ( pThisChar, pThisChar->generalSpeed );
				}
			}
			else
			{
#pragma chMSG( "삭제. 이거 이렇게 처리하면 안됨." )
				if	(	pThisChar ->ani_curGroup	==	FX_ANIM_GROUP_ATTACK	)
					pThisChar ->atk_changeEvent	=	false;
				else
					pThisChar ->atk_changeEvent	=	true;
			}
				 
			if	(	(	CalcAttackLength( pThisChar )	>=	500.0f	)	&&
					(	pThisChar ->ani_curSetID	>=	ANIM_ITEM_ATTACK_0	)	&&
					(	pThisChar ->ani_curSetID	<=	ANIM_ITEM_ATTACK_4	)	&&

					(	pThisChar ->ani_curSetType == ANIMTYPE_BYITEM )		)	
			{
				if( pThisChar->targetIdx == - 1 || pThisChar->targetIdx == pThisChar->idxOnServer )
					break;
				
				g_charManager ->AddEffect ( 1, 0, pThisChar, 50.0f );
			}
			break;
		default:
			break;
		}

		if	(	true	==	pThisChar->ani_curState.type.movable	)
		{
			pThisChar ->ani_elapsed	+=	m_elapsedTime;
		}

		pAnim ->m_pSkeleton			=	pSkel;

		
		if	(	pThisChar ->ani_curSpeed	<=	0	)
		{
			pThisChar ->ani_curSpeed	=	1.0f;
			pThisChar ->ani_curRSpeed	=	1.0f;
		}

#ifdef	_FX_CLIENT
		if	(	pThisChar ->ani_curGroup	==	FX_ANIM_GROUP_ATTACK	)
		{
			endAnim	=	pAnim ->CheckKeyframeState	(	pThisChar,	ANIMCHECK_NORMAL	);
		}
		else
#endif	 
			endAnim	=	pAnim ->CheckKeyframeState	(	pThisChar,	ANIMCHECK_NORMAL	);

		 
		if	(	CheckSkinUpdateTime( pThisChar )	||	GetForceUpdateSkin()	)
		{
			pAnim ->MakeInterpolatedVertices	(	pThisChar,	endAnim	);

			PreUpdateAura (	pThisChar,	pSkel	);
		}

		int		startAnim;
		switch	(	endAnim	)
		{
		case	ANIMCHECK_RESULT_NORMAL: 
			break;
		case	ANIMCHECK_RESULT_END:
				 
			pThisChar ->ani_option	=	ANIMOPTION_START_NEW_ANIM;

			startAnim	=	pThisChar ->ani_curIdx;
			if	(	false	==	pThisChar ->ani_curState.type.repeatable	)
			{
				if	(	pThisChar->ani_nextGroup	==	FX_ANIM_GROUP_NULL	)
				{
					pThisChar ->ani_curIdx		=	m_pCharLookupMng ->GetAnimIDFromTable (	pThisChar ->entityType,		pThisChar ->tableIdx,
																							pThisChar ->ani_curSetType,	pThisChar ->ani_curItemSetIndex,	pThisChar ->ani_curSetID, pThisChar->ID );
					pThisChar ->ani_curGroup	=	FX_ANIM_GROUP_MOVE;
					pThisChar ->ani_curState.type.repeatable	=	true;
					pThisChar ->ani_curState.type.changable		=	true;
				}
				else
				{
					pThisChar ->ani_curIdx			=	pThisChar ->ani_nextIdx;
					pThisChar ->ani_curGroup		=	pThisChar ->ani_nextGroup;
					pThisChar ->ani_curState.type.repeatable	=	pThisChar ->ani_nextState.type.repeatable;
					pThisChar ->ani_curState.type.changable		=	pThisChar ->ani_nextState.type.changable;

					pThisChar ->ani_curSetID			=	pThisChar ->ani_nextSetID;
					pThisChar ->ani_curItemSetIndex		=	pThisChar ->ani_nextItemSetIndex;
					pThisChar ->ani_curSetType			=	pThisChar ->ani_nextSetType;

					pThisChar ->ani_curSpeed	=	pThisChar ->ani_nextSpeed;
					pThisChar ->ani_curRSpeed	=	pThisChar ->ani_nextRSpeed;

				}
				
				pThisChar->ani_nextIdx			=	-1;
				pThisChar->ani_nextGroup		=	FX_ANIM_GROUP_NULL;

				MakeInterpolator( pThisChar, pThisChar->ani_curSpeed * INTERPOLATE_FRACTION, startAnim, pThisChar->ani_curIdx, INTERPOLATE_NORMAL );
			}
			break;
		case	ANIMCHECK_RESULT_EOP:
		case	ANIMCHECK_RESULT_EOA:
			break;
		}
	}

	ProcessComboAttack();

	return	UpdateAnimationsCreature ();
	
}

 
 
 
 
 
 
void	FX_CCharManager::ProcessComboAttack	()
{
	Fx_CHARACTER_t	*pThisChar;

	for	(	int	index	=	0;	\
				index	<	m_iNumCharacters;	\
				++index		)
	{
		pThisChar	= NULL;
		pThisChar	=	&m_Characters [index];

		
		

		

		
		float fCalAttackLength =0.0f;

		
		if( pThisChar->entityType == ENTITY_PC && pThisChar->isTransform)
		{
			float mfAttackLength,pfAttackLength;

			
			mfAttackLength = FindTransformAttackLength(pThisChar->transformEntityIdx, pThisChar->transformType);

			
			pfAttackLength = (float)CalcAttackLength( pThisChar );

			
			
			if(mfAttackLength > pfAttackLength) 
			{
				fCalAttackLength = pfAttackLength;
			}
			
			else 
			{
				fCalAttackLength = mfAttackLength;
			}
		}
		else
		{
			fCalAttackLength = CalcAttackLength( pThisChar );
		}

		

		 
		
		
		
		if	(	m_pAnimManager ->IsComboAttack ( pThisChar ->ani_curIdx )	&& pThisChar->entityType == ENTITY_PC	)
		{
			pThisChar ->atk_resend		=	false;
			pThisChar ->atk_changeEvent	=	false;

			
			
			if(fCalAttackLength	>=	500.0f && g_cgv.saveEvent != GTH_EV_CHAR_SKILL)
			
			{
				 
				SetNextBeginFrame ( pThisChar );
			}
			 
			else if	(	(	(float)pThisChar ->atk_nextBeginFrame	<	pThisChar ->ani_oldFrame	)	&&
						(	pThisChar ->atk_nextBeginFrame	>=	0	)	&&
						(	index	==	0	)	)

			{
				 
				 
				 

					
					 
					if( pThisChar->atk_indexBegin > 0 )
						pThisChar->atk_resend	=	true;	

				 
				SetNextBeginFrame ( pThisChar );
			}

			 
			if	(	(	pThisChar ->atk_nextDamageFrame	<	pThisChar ->ani_oldFrame	)	&&
					(	pThisChar ->atk_nextDamageFrame	>=	0	)	&&
					(	CalcAttackLength ( pThisChar )	<	500.0f	)	)
			{
				 
				ProcessDamageFrame ( pThisChar );

				 
				SetNextDamageFrame ( pThisChar );
			}

			 
			if	(	(	pThisChar ->atk_nextFreeFrame	<	pThisChar ->ani_oldFrame	)	&&
					(	pThisChar ->atk_nextFreeFrame	>=	0	)	)
			{
				 
				pThisChar->atk_changeEvent	=	true;

				SetAnimationSpeedInSec ( pThisChar, pThisChar ->atk_freeAttackTime, false );

				 
				SetNextFreeFrame ( pThisChar );






			}

			 
			if	(	(	pThisChar ->atk_nextStartFrame	<	pThisChar ->ani_oldFrame	)	&&
					(	pThisChar ->atk_nextStartFrame	>=	0	)	)
			{
				 

				SetAnimationSpeedInSec ( pThisChar, pThisChar ->atk_originalAttackTime, false );

				 
				SetNextStartFrame ( pThisChar );
			}
		}

		
		
		if	(fCalAttackLength	>=	500.0f	&& g_cgv.saveEvent != GTH_EV_CHAR_SKILL	)
		
		{
			if	(	pThisChar->atk_sendRangeAttackResend	)
			{
				
				
				pThisChar->atk_resend	=	true;		
				g_cgv.resendAttack = true;
				
				
				if( pThisChar->ID == 0 ) 
				{
					if( !( pThisChar->ani_curSetID == ANIM_ITEM_SKILLCASTING ||
							pThisChar->ani_curSetID == ANIM_ITEM_CASTINGIDLE ||
							pThisChar->ani_curSetType == ANIMTYPE_BYSKILL ) && g_SkillApp.skillResendFlag <= 0 )
					{
						
						GTH_SendEventMessage_Attack();
					}
				}
				pThisChar->atk_sendRangeAttackResend	=	false;
			}
			else
				pThisChar->atk_resend	=	false;
			if	(	pThisChar->atk_processDamage	)
			{
				
				ProcessDamageFrame( pThisChar );
				pThisChar->atk_processDamage	=	false;
			}
		}
		
	}
}

 
 
 
bool	FX_CCharManager::UpdateCharacterState	()
{
	int		result;

	result	=	m_pItemManager ->UpdateCharBuffer ();
#ifdef	_FXDEBUGA
	if	(	!result	)
	{
		ShowError	(	_T	("Failed to sort item character buffer!"),
						__FILE__,
						__LINE__	);
		return	false;
	}
#endif

	result	=	m_pSkinManager ->UpdateCharBuffer ();
#ifdef	_FXDEBUG
	if	(	!result	)
	{
		ShowError	(	_T	("Failed to sort skin character buffer!"),
						__FILE__,
						__LINE__	);
		return	false;
	}
#endif

	return	true;
}

 
 
 
 
void	FX_CCharManager::GetBoundChar	(	Fx_CHARACTER_t	*in_char,
											bboxf_t			&out_bound	)
{
	bboxf_t			bound;

	GetBoundCharPreTransform( in_char, bound );

	m_pCharUtil->TransformBBox( bound, deg2rad( in_char->angles[ YAW ] ) + __HALFPI, 0, in_char->position, out_bound );
	
#ifdef	_FXSKELDRAW


#endif

	return;
}

 
 
 
 
void	FX_CCharManager::StartFrame ()
{
	m_pCharShaderMng ->UpdateFrameFlag ();
	m_totalDrawnPrims	=	0;

	for	(	int	index	=	0;	\
				index	<	m_iNumCharacters;	\
				++index	)
	{
		for	(	int	subindex	=	0;	\
					subindex	<	MAX_COUNT_ITEM_PART;	\
					++subindex	)
		{
			m_Characters [index].flag_tagMatrixMade [subindex]	=	false;
		}
	}

	m_currentTime	=	m_timer ->GetAppTime ();
	m_elapsedTime	=	m_timer ->GetElapsedTime ();

	return;
}

 
 
 
 
void	FX_CCharManager::EndFrame ()
{
	if	(	m_iNumCharacters	==	0	)	
		return;

	Fx_CHARACTER_t	*thisChar	=	&m_Characters[ 0 ];
	for	(	int	index	=	0;	\
				index	<	m_iNumCharacters;	\
				++index,	++thisChar	)
	{
		if	(	CheckSkinUpdateTime( thisChar )	)
			UpdateSkinUpdateTime( thisChar );
		thisChar->flag_forceUpdate	=	false;
	}

#ifdef	_FX_CLIENT

	

	CheckTrembling ();
	CheckCharTrembling ();
	ReleaseForceUpdateSkin ();

#endif	 

#ifdef	_FXSKELDRAW
	m_pSkelDraw ->ClearAll ();
	if	(	! m_Characters [0].flag_culled	)
	{
		GetBoundChar ( 0,	g_bbox );
		SetSkelBBoxData ( g_bbox );
	}
#endif

	return;
}

 
 
 
bool	FX_CCharManager::RenderItems	()
{
	int		result;

	result	=	m_pItemManager->RenderAll (	&m_totalDrawnPrims	);
#ifdef	_FXDEBUG
	if	(	! result	)
	{
		ShowError	(	_T	("Failed to render items!"),	__FILE__,	__LINE__	);
		return	false;
	}
#endif

	return	true;
}

 
 
 
 
 
 
 
void	FX_CCharManager::PrepareSkinRendering	()
{
	if	(	m_iNumCharacters	<=	0	)
		return;

	int		index;
	Fx_CHARACTER_t	*thisChar;
	for	(	index	=	0;	\
			index	<	MAX_CHAR_A_SCENE;	\
			++index		)
	{
		thisChar	=	&m_Characters[ index ];
		if	(	thisChar->flag_culled	||
				! GTH_RenderGM( thisChar ) || thisChar->bHide == true 	)
		{
			thisChar->ren_renderThisFrame	=	false;
		}
		else
		{
			thisChar->ren_renderThisFrame	=	true;

			if	(	GetForceUpdateSkin()			||
					CheckSkinUpdateTime( thisChar )	||
					thisChar->flag_forceUpdate	)
			{
				m_pSkinManager->DiscardIndexBuffer( thisChar );
				m_pSkinManager->DiscardVertexBuffer( thisChar );
			}
		}
	}

	ITOR_LIST_CREATURE	itor;
	for	(	itor	=	m_usedCreatureList.begin();	\
			itor	!=	m_usedCreatureList.end();	\
			++itor	)
	{
		thisChar	=	(*itor);
		if	(	thisChar->flag_culled	)
		{
			thisChar->ren_renderThisFrame	=	false;
		}
		else
		{
			thisChar->ren_renderThisFrame	=	true;

			if	(	GetForceUpdateSkin()			||
					CheckSkinUpdateTime( thisChar )	||
					thisChar->flag_forceUpdate	)
			{
				m_pSkinManager->DiscardIndexBuffer( thisChar );
				m_pSkinManager->DiscardVertexBuffer( thisChar );
			}
		}
	}
}

 
 
 
 
bool	FX_CCharManager::RenderSkinsNormal	()
{
	if	(	m_iNumCharacters	<=	0	)
		return	true;
	
	m_pSkinManager->RenderAllNormal ( &m_totalDrawnPrims );

	return	true;
}

 
 
 
 
bool	FX_CCharManager::RenderEffects	()
{
	Fx_CHARACTER_t	*thisChar;

	for	(	int	index	=	0;	\
				index	<	m_iNumCharacters;	\
				++index		)
	{
		thisChar	=	&m_Characters [index];
		if	(	( thisChar ->entityType	==	ENTITY_MONSTER )	&&
				( thisChar ->flag_drawSpawnEffect )		)
		{
			 
			if( thisChar->stat_alive == MON_ALIVE_STATE_SPAWN && thisChar->entityType != ENTITY_PC ) 
			{
				thisChar ->flag_drawSpawnEffect	=	false;
				 
				g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 9 , thisChar ->position , NULL , NULL , 0.0f , thisChar ->entityType , thisChar ->idxOnServer );
			} else if( thisChar->stat_alive == MON_ALIVE_STATE_FADE && thisChar->entityType != ENTITY_PC ) 
			{
				thisChar ->flag_drawSpawnEffect	=	false;
				 
				g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 8 , thisChar ->position , NULL , NULL , 0.0f , thisChar ->entityType , thisChar ->idxOnServer );
			}
		}
	}

	return	true;
}

#ifdef	_FX_CLIENT

 
 
 
 
bool	FX_CCharManager::RenderSkinsDisappear	()
{
	if	(	m_iNumCharacters	<=	0	)
		return	true;

	m_pSkinManager->RenderAllDisappear ( &m_totalDrawnPrims );

	return	true;
}

#endif	 


 
 
 
 
bool	FX_CCharManager::RenderAuras	()
{
	Fx_CHARACTER_t	*thisChar;
	int itemTableIdx;

	for	(	int	index	=	0;	\
				index	<	m_iNumCharacters;	\
				++index		)
	{
		thisChar	=	&m_Characters [index];

		
		if( thisChar->entityType == ENTITY_PC )
		{
			if( thisChar->equipment[0] >= 0 ) 
			{
				itemTableIdx = thisChar->equipment[0];
				if( g_itemClassTable[g_itemTable[itemTableIdx].classIdx].attackAtt )
				{
					if( g_itemTable[itemTableIdx].atkLength > 250 ) continue;
				}
			}
		}

		if	(	thisChar ->flag_useRHandAura	&&	thisChar ->ren_auraRHandData.render	&&	(	thisChar ->ren_auraRHandData.num_used	>	1	)	)
		{
			if	(	m_pAuraManager->MakePreVertexBuffer( &thisChar->ren_auraRHandData )	)
				m_pAuraManager ->Render (	thisChar,	&m_totalDrawnPrims,	thisChar ->ren_auraRHandData.shaderIdx,	thisChar ->ren_auraRHandData.num_used	);
		}
		if	(	thisChar ->flag_useLHandAura		&&	thisChar ->ren_auraLHandData.render	&&	(	thisChar ->ren_auraLHandData.num_used	>	1	)	)
		{
			if	(	m_pAuraManager->MakePreVertexBuffer( &thisChar->ren_auraLHandData )	)
				m_pAuraManager ->Render (	thisChar,	&m_totalDrawnPrims,	thisChar ->ren_auraLHandData.shaderIdx,	thisChar ->ren_auraLHandData.num_used	);
		}
	}

	return	true;
}

 
 
 
 
void	FX_CCharManager::FixCharacterPosition	()
{
	int		index;
	vec3_t	trembleDelta;
	vec3_t	trembleDeltaBase;
	Fx_CHARACTER_t	*thisChar;

	trembleDeltaBase [0]	=	sin ( g_timer .GetAppTime () * 35.0f ) * 10.0f;
	trembleDeltaBase [1]	=	cos ( g_timer .GetAppTime () * 34.0f ) * 10.0f;
	trembleDeltaBase [2]	=	sin ( g_timer .GetAppTime () * 33.0f ) * cos ( g_timer .GetAppTime () * 32.0f ) * 10.0f;

	for	(	index	=	0;	\
			index	<	m_iNumCharacters;	\
			++index		)
	{
		thisChar	=	&m_Characters [index];

		thisChar ->stat_fakeHeight	=	thisChar ->position [2]	+	thisChar ->stat_height;

		D3DXMatrixRotationZ (	(D3DXMATRIX *)&g_matRotation,	deg2rad ( thisChar ->angles[ YAW ] ) + __HALFPI	);

		 
		if	(	thisChar ->tb_do	)
		{
			VectorScale ( trembleDelta, trembleDeltaBase, thisChar ->tb_power );

			D3DXMatrixTranslation ( (D3DXMATRIX *)&g_matTranslation, 
							thisChar ->position [0] + trembleDelta [0], thisChar ->position [1] + trembleDelta [1],	thisChar ->stat_fakeHeight + trembleDelta [2] );
		}
		else
			D3DXMatrixTranslation	(	(D3DXMATRIX *)&g_matTranslation,	thisChar ->position [0],	thisChar ->position [1],	thisChar ->stat_fakeHeight	);

		GLOBAL_SCALEMATRIX.a00	=	thisChar ->stat_scale;
		GLOBAL_SCALEMATRIX.a11	=	thisChar ->stat_scale;
		GLOBAL_SCALEMATRIX.a22	=	thisChar ->stat_scale;

		D3DXMatrixMultiply ( (D3DXMATRIX *)&g_buffer,	(D3DXMATRIX *)&GLOBAL_SCALEMATRIX,	(D3DXMATRIX *)&g_matRotation );
		D3DXMatrixMultiply ( (D3DXMATRIX *)&thisChar ->stat_matTransform,	(D3DXMATRIX *)&g_buffer,	(D3DXMATRIX *)&g_matTranslation );
	}

	FixCreaturePosition();

	return;
}

 
 
 
void	FX_CCharManager::StopAnimation	(	const int		charID,
											const bool	stop	)
{
	Fx_CHARACTER_t*		pThisChar	=	&m_Characters [charID];

	if	(	stop	)
		pThisChar ->ani_curState.type.movable	=	true;
	else
		pThisChar ->ani_curState.type.movable	=	false;

	return;
}

 
 
 
 
void	FX_CCharManager::UpdateSysVBInfo ()
{
	int		index;
	int		count	=	0;

	Fx_CHARACTER_t	*thisChar	=	&m_Characters[0];
	for	(	index	=	0;	\
			index	<	m_iNumCharacters;	\
			++index,	++thisChar	)
	{
		if	(	index	==	0	)
		{
			 
			SetFPS( thisChar, 50 );
			SetSkinUpdateTime( thisChar, 0.0f );
		}
		else
		{
			if	(	thisChar->entityType	==	ENTITY_PC	)
				SetFPS( thisChar, 30 );		 
			else if	(	thisChar->entityType	==	ENTITY_MONSTER	||
						thisChar->entityType	==	ENTITY_NPC )
				SetFPS( thisChar, 30 );		 

			SetSkinUpdateTime( thisChar, index * 0.02f );
		}
	}

	ITOR_LIST_CREATURE	itor;
	for	(	itor	=	m_usedCreatureList.begin();	\
			itor	!=	m_usedCreatureList.end();	\
			++itor	)
	{
		thisChar	=	(*itor);
		SetFPS( thisChar, 20 );
		SetSkinUpdateTime( thisChar, thisChar->ID * 0.02f );
	}

	SetForceUpdateSkin ();

	return;
}

 
 
 
 
void	FX_CCharManager::CheckAuraExist	(	const int	in_charIdx	)
{
	Fx_CHARACTER_t	*thisChar	=	&m_Characters [in_charIdx];	

	thisChar ->ren_auraRHandData.height	=	-1.0f;
	thisChar ->ren_auraLHandData.height	=	-1.0f;

	if	(	thisChar ->stat_itemID [ITEM_PART_HAND_R]	==	-1	) 
			thisChar ->flag_useRHandAura	=	false;
	else
	{
		thisChar ->flag_useRHandAura	=	true;
		thisChar ->ren_auraRHandData.height	=	m_pItemManager ->m_Items [thisChar ->stat_itemID [ITEM_PART_HAND_R]].mesh.m_meshes [0].meshchunk ->bound_min [5];
	}

	if	(	thisChar ->stat_itemID [ITEM_PART_HAND_L]	==	-1	) 
			thisChar ->flag_useLHandAura		=	false;
	else
	{
		thisChar ->flag_useLHandAura		=	true;
		thisChar ->ren_auraLHandData.height	=	m_pItemManager ->m_Items [thisChar ->stat_itemID [ITEM_PART_HAND_L]].mesh.m_meshes [0].meshchunk ->bound_min [5];
	}

	return;
}

 
 
 
 
void	FX_CCharManager::InitAnimation	(	const int	iCharID, 	
											const int	aniSetType,		
											const int	aniSetIndex, 	
											const int	aniSetID	)
{
	 
	Fx_CHARACTER_t*		pThisChar	=	&m_Characters [iCharID];

	Fx_ANIM_INFO_t	*nextState;

	int				nextIdx;

	if	(	aniSetType < 0	||	aniSetIndex < 0	||	aniSetID < 0	)	return;

	pThisChar->ani_curSetType		=	aniSetType;
	pThisChar->ani_curItemSetIndex	=	aniSetIndex;
	pThisChar->ani_curSetID			=	aniSetID;

	if( pThisChar->entityType == ENTITY_PC && pThisChar->isTransform )
	{
		nextIdx		=	m_pCharLookupMng ->GetAnimIDFromTable (	ENTITY_MONSTER,	pThisChar->transformEntityIdx, aniSetType,	aniSetIndex,	aniSetID, pThisChar->ID	);

		
		if( nextIdx < 0 || nextIdx >= MAX_COUNT_ANIM )
			return;

		nextState	=	&m_pCharLookupMng ->m_aniInfo [nextIdx];
	}
	else
	{
		nextIdx		=	m_pCharLookupMng ->GetAnimIDFromTable (	pThisChar ->entityType,	pThisChar ->tableIdx,	aniSetType,	aniSetIndex,	aniSetID, pThisChar->ID	);

		
		if( nextIdx < 0 || nextIdx >= MAX_COUNT_ANIM )
			return;

		nextState	=	&m_pCharLookupMng ->m_aniInfo [nextIdx];
	}

	pThisChar ->ani_nextIdx		=	-1;
	pThisChar ->ani_nextGroup	=	FX_ANIM_GROUP_NULL;

	pThisChar ->ani_option		=	ANIMOPTION_START_NEW_ANIM;

	pThisChar ->ani_curIdx			=	nextIdx;
	pThisChar ->ani_curGroup		=	nextState ->groupID;
	pThisChar ->ani_curState.value	=	nextState ->state.value;

	return;
}

 
 
 
 
int		FX_CCharManager::CheckSkinUpdateTime (	Fx_CHARACTER_t	*in_char	)
{
	if	(	in_char->ren_nextTime	<=	0	)
		return	false;

	if	(	m_currentTime	>	in_char->ren_nextTime	)
		return	true;
	else
		return	false;
}

 
 
 
 
void	FX_CCharManager::UpdateSkinUpdateTime (	Fx_CHARACTER_t	*in_char	)
{
	in_char->ren_nextTime	+=	GetSPF( in_char );

	return;
}

 
 
 
 
bool	FX_CCharManager::InterpolateAnim	(	Fx_CHARACTER_t	*in_char	)
{
	KEYFRAME		*startFrame	=	NULL;
	KEYFRAME		*endFrame	=	NULL;
	float			disp;
	
	if	(	(	in_char->ip_curTime	<	0	)	||
			(	m_elapsedTime	<	0	)	||
			(	in_char->ip_totalTime	<=	0	)	)
	{
		ErrorLog( "entityType : %d, name : %s, ip_curTime : %f, m_elapsedTime : %f, ip_totalTime : %f", in_char->entityType, in_char->name, in_char->ip_curTime, m_elapsedTime, in_char->ip_totalTime );
		in_char->flag_interpolation	=	false;
		return	false;
	}
	if	(	in_char->ip_endAnim	==	NULL	)
	{
		ErrorLog( "entityType : %d, name : %s, ip_endAnim == NULL", in_char->entityType, in_char->name );
		in_char->flag_interpolation	=	false;
		return	false;
	}

	if	(	true	==	in_char->ani_curState.type.movable	)
	{
		in_char->ip_curTime	+=	( m_elapsedTime * in_char->ren_generalSpeed * 0.01f );
	}

	 
	if	(	in_char->ip_curTime	>	in_char->ip_totalTime	)
	{
		in_char->flag_interpolation	=	false;
		return	false;
	}

	disp	=	in_char->ip_curTime / in_char->ip_totalTime;

	for	(	int	index	=	0;	\
				index	<	in_char->ip_startAnim->m_iNumTrack;	\
				++index		)
	{
		switch	( in_char->ip_option )
		{
		case	INTERPOLATE_NORMAL:
			startFrame	=	&in_char->ip_startAnim->m_Tracks[ index ].pKeyframe[ in_char->ip_startAnim->m_Tracks[ index ].iNumKeyframe - 1 ];
			endFrame	=	&in_char->ip_endAnim->m_Tracks[ index ].pKeyframe[ 0 ];
			break;
		case	INTERPOLATE_USE_CURFRAME:
			startFrame	=	&in_char->key_copiedFrames[ index ];
			endFrame	=	&in_char->ip_endAnim->m_Tracks[ index ].pKeyframe[ 0 ];
			break;
		case	INTERPOLATE_USE_EOA:
			startFrame	=	&in_char->key_copiedFrames[ index ];
			endFrame	=	&in_char->ip_endAnim->m_Tracks[ index ].pKeyframe[in_char->ip_endAnim->m_eopframeIdx[ index ] ];
			break;
		}

		in_char->ip_startAnim->Interpolate( startFrame, endFrame, disp, &in_char->key_curFrames[ index ] );
	}

	return	true;
}

 
 
 
 
void	FX_CCharManager::MakeInterpolator	(	Fx_CHARACTER_t	*in_char,
												const float	in_duration,	
												const int	in_startAnim,	
												const int	in_endAnim,
												const int	in_option		)
{
	in_char->flag_interpolation	=	true;
	in_char->ip_option			=	in_option;
	in_char->ip_totalTime		=	in_duration;
	in_char->ip_curTime			=	0.0f;

	in_char->ip_startAnim	=	&m_pAnimManager->m_Anims[ in_startAnim ].animation;
	in_char->ip_endAnim		=	&m_pAnimManager->m_Anims[ in_endAnim ].animation;

	in_char->ip_endAnim->m_pSkeleton	=	&m_pSkelManager->m_Skels[ in_char->attr_skelID ].skeleton;

	switch	(	in_option )
	{
	case	INTERPOLATE_NORMAL:
		break;
	case	INTERPOLATE_USE_CURFRAME:
	case	INTERPOLATE_USE_EOA:
		memcpy( &in_char->key_copiedFrames[ 0 ], &in_char->key_curFrames[ 0 ], sizeof(in_char->key_curFrames) );
		break;
	}

	return;
}

 
 
 
 
void	FX_CCharManager::FadeAura	(	const int	in_charID	)
{
	Fx_CHARACTER_t	*thisChar	=	&m_Characters [in_charID];

	if	(	thisChar ->flag_useRHandAura	&&	thisChar ->ren_auraRHandData.render		)
		m_pAuraManager ->FadeAura (	thisChar ->ren_auraRHandData );
	if	(	thisChar ->flag_useLHandAura		&&	thisChar ->ren_auraLHandData.render		)
		m_pAuraManager ->FadeAura (	thisChar ->ren_auraLHandData );

	return;
}

 
 
 
 
void	FX_CCharManager::PreUpdateAura	(	Fx_CHARACTER_t	*pThisChar, 
											FX_CSkel		*pSkel		)
{
	if	(	pThisChar ->flag_useRHandAura	&&	pThisChar ->ren_auraRHandData.render	)	
	{
		if	(	pThisChar ->stat_itemID [ITEM_PART_HAND_R]	<	0	)
		{
			ErrorLog( "entityType : %d, name : %s, In state of not setting right weapon, Try to draw aura!", pThisChar->entityType, pThisChar->name );
			return;
		}
		 
		ITEM_t		*pThisItem	=	&m_pItemManager ->m_Items [pThisChar ->stat_itemID [ITEM_PART_HAND_R]];
		KEYFRAME	*pThisFrame	=	&pThisChar ->key_resultFrames [	
											m_pSkelManager ->m_iBoneLookUp [pThisChar ->attr_skelID][pThisItem ->iParentTagIndex]
															];
		TAG			*pThisTag	=	&pSkel ->m_Tag [pThisItem ->iTagIndex];
		
		UpdateAura	(	pThisChar,	pThisFrame,	pThisTag,	true	);
	}

	if	(	pThisChar ->flag_useLHandAura	&&	pThisChar ->ren_auraLHandData.render		)
	{
		if	(	pThisChar ->stat_itemID [ITEM_PART_HAND_L]	<	0	)
		{
			ErrorLog( "entityType : %d, name : %s, In state of not setting left weapon, Try to draw aura!", pThisChar->entityType, pThisChar->name );
			return;
		}
		 
		ITEM_t		*pThisItem	=	&m_pItemManager ->m_Items [pThisChar ->stat_itemID [ITEM_PART_HAND_L]];
		KEYFRAME	*pThisFrame	=	&pThisChar ->key_resultFrames [	
											m_pSkelManager ->m_iBoneLookUp [pThisChar ->attr_skelID] [ pThisItem ->iParentTagIndex]
															];
		TAG			*pThisTag	=	&pSkel ->m_Tag [pThisItem ->iTagIndex];

		UpdateAura	(	pThisChar,	pThisFrame,	pThisTag,	false	);
	}

	return;
}

 
 
 
 
void	FX_CCharManager::GetBoundCharPreTransform	(	Fx_CHARACTER_t	*in_char,
														bboxf_t			&out_bound	)
{
	float	spf;

	if	(	in_char->flag_interpolation	)
	{
		m_pAnimManager->GetBoundBox( in_char, 0, out_bound );
	}
	else
	{
		
		
		assert(in_char->ani_curIdx >= 0);
		
		spf		=	m_pAnimManager->m_Anims[ in_char->ani_curIdx ].animation.m_framePerTime * in_char->ani_curRSpeed;
		int		thisFrame	=	in_char->ani_elapsed * spf + FX_ANIM_ERROR;
		if	(	thisFrame	>=	m_pAnimManager->m_Anims[ in_char->ani_curIdx ].animation.m_totalFrames	)
			thisFrame	=	m_pAnimManager->m_Anims[ in_char->ani_curIdx ].animation.m_totalFrames - 1;
		m_pAnimManager->GetBoundBox( in_char, thisFrame, out_bound );
	}

	return;
}

 
 
 
 
void	FX_CCharManager::SetSkinUpdateTime (	Fx_CHARACTER_t	*in_char,	
												const float		in_basetime	)
{
	const float	spf		=	GetSPF( in_char );

	in_char->ren_nextTime	=	in_basetime + ( int((m_currentTime - in_basetime) / spf) + 1 ) * spf;

	return;
}

 
 
 
 
void	FX_CCharManager::SetFPS	(	Fx_CHARACTER_t	*in_char,
									const int		in_fps	)
{
	in_char->ren_updateFPS	=	in_fps;
	in_char->ren_updateSPF	=	(1.0f / in_fps);

	return;
}

 
 
 
 
int		FX_CCharManager::GetFPS (	Fx_CHARACTER_t	*in_char	)
{
	return	in_char->ren_updateFPS;
}

 
 
 
 
float	FX_CCharManager::GetSPF (	Fx_CHARACTER_t	*in_char	)
{
	return	in_char->ren_updateSPF;
}

 
 
 
 
void	FX_CCharManager::UpdateAura (	Fx_CHARACTER_t	*pThisChar,
										KEYFRAME		*pThisFrame,
										TAG				*pThisTag,
										const bool		in_rightHand	)
{
	g_vec_end [0]	=	0.0f;
	g_vec_end [1]	=	0.0f;

	if	(	in_rightHand	)
	{
		g_vec_end [2]	=	pThisChar ->ren_auraRHandData.height;
		 
		
			 
		TransformAuraVertices	(	pThisFrame,		pThisTag,
									ZERO_VECTOR3,	g_vec_end,
									(matrix4x4_t *)&pThisChar ->stat_matTransform,
									g_vec_start_trans,	g_vec_end_trans	);
		if	(	! pThisChar ->ren_auraRHandData.made	)
		{	 
			m_pAuraManager ->SetInitialPos (	&pThisChar ->ren_auraRHandData,		g_vec_start_trans,	g_vec_end_trans	);
			pThisChar ->ren_auraRHandData.made		=	true;
		}
		else
		{
			if	(	true	==	pThisChar ->ani_curState.type.movable		)
				if	(	m_pAuraManager ->Update	(	pThisChar ->ren_auraRHandData.nextUpdate,	pThisChar ->ani_elapsed	)	)
					m_pAuraManager ->UpdatePos	(	pThisChar ->ren_auraRHandData,	g_vec_start_trans,	g_vec_end_trans	);
		}
		VectorCopy ( pThisChar ->ren_auraRHandData.xformed_start,	g_vec_start_trans );
		VectorCopy ( pThisChar ->ren_auraRHandData.xformed_end,		g_vec_end_trans );

		 
		pThisChar ->ren_auraRHandData.xformed_end [0]	=	pThisChar ->ren_auraRHandData.xformed_start	[0]	+
								(pThisChar ->ren_auraRHandData.xformed_end [0] - pThisChar ->ren_auraRHandData.xformed_start [0]) * 10.0f;
		pThisChar ->ren_auraRHandData.xformed_end [1]	=	pThisChar ->ren_auraRHandData.xformed_start	[1]	+
								(pThisChar ->ren_auraRHandData.xformed_end [1] - pThisChar ->ren_auraRHandData.xformed_start [1]) * 10.0f;
		pThisChar ->ren_auraRHandData.xformed_end [2]	=	pThisChar ->ren_auraRHandData.xformed_start	[2]	+
								(pThisChar ->ren_auraRHandData.xformed_end [2] - pThisChar ->ren_auraRHandData.xformed_start [2]) * 10.0f;
	}
	else
	{
		g_vec_end [2]	=	pThisChar ->ren_auraLHandData.height;
		 
		
			 
		TransformAuraVertices	(	pThisFrame,		pThisTag,
									ZERO_VECTOR3,	g_vec_end,
									(matrix4x4_t *)&pThisChar ->stat_matTransform,
									g_vec_start_trans,	g_vec_end_trans	);

		if	(	! pThisChar ->ren_auraLHandData.made	)
		{	 
			m_pAuraManager ->SetInitialPos (	&pThisChar ->ren_auraLHandData,	g_vec_start_trans,	g_vec_end_trans	);
			pThisChar ->ren_auraLHandData.made		=	true;
		}
		else
		{
			if	(	true	==	pThisChar ->ani_curState.type.movable		)
				if	(	m_pAuraManager ->Update	(	pThisChar ->ren_auraLHandData.nextUpdate,	pThisChar ->ani_elapsed	)	)
					m_pAuraManager ->UpdatePos	(	pThisChar ->ren_auraLHandData,	g_vec_start_trans,	g_vec_end_trans	);
		}
		VectorCopy ( pThisChar ->ren_auraRHandData.xformed_start,	g_vec_start_trans );
		VectorCopy ( pThisChar ->ren_auraRHandData.xformed_end,		g_vec_end_trans );
	}

	return;
}

 
 
 
void	FX_CCharManager::UpdatePosition	(	const int		iCharID,
											const vec3_t	position,
											const vec3_t	angle	)
{
	Fx_CHARACTER_t*		pThisChar	=	&m_Characters [iCharID];

	pThisChar ->position [0]	=	position [0];
	pThisChar ->position [1]	=	position [1];
	pThisChar ->position [2]	=	position [2];

	pThisChar ->angles [0]	=	angle [0];
	pThisChar ->angles [1]	=	angle [1];
	pThisChar ->angles [2]	=	angle [2];

	return;
}

 
 
 
 
int		FX_CCharManager::GetSkinID (	const int	in_type,
										const int	in_internalID	)
{
	return	m_pSkinManager ->MakeSkinID	(	in_type,	in_internalID	);
}

 
 
 
 
void	FX_CCharManager::TransformAuraVertices (	const KEYFRAME		*pThisFrame,
													const TAG			*pThisTag,
													const vec3_t		in_vecstart,
													const vec3_t		in_vecend,
													const matrix4x4_t	*in_matTransform,
													vec3_t				out_vecstart,
													vec3_t				out_vecend		)
{
	g_quatTarget [0]	=	pThisTag ->position [0];
	g_quatTarget [1]	=	pThisTag ->position [1];
	g_quatTarget [2]	=	pThisTag ->position [2];
	g_quatTarget [3]	=	0.0f;

	D3DXQuaternionConjugate	(	(D3DXQUATERNION *)g_quatConjugate,	(D3DXQUATERNION *)pThisFrame ->quatCoordinate	);
	
	Fx_Quat3Multiply( g_quatAfterTransform, g_buffer, pThisFrame ->quatCoordinate, g_quatTarget, g_quatConjugate );
	
	g_vecAfterTransform [0]		=	g_quatAfterTransform [0]	+	pThisFrame ->vecOffset [0];
	g_vecAfterTransform [1]		=	g_quatAfterTransform [1]	+	pThisFrame ->vecOffset [1];
	g_vecAfterTransform [2]		=	g_quatAfterTransform [2]	+	pThisFrame ->vecOffset [2];
	
	D3DXQuaternionMultiply	(	(D3DXQUATERNION *)g_quatAfterTransform,	(D3DXQUATERNION *)pThisFrame ->quatCoordinate,	(D3DXQUATERNION *)pThisTag ->rotation	);
	D3DXQuaternionInverse	(	(D3DXQUATERNION *)g_quatAfterTransform,	(D3DXQUATERNION *)g_quatAfterTransform	);
	
	D3DXMatrixTransformation ( (D3DXMATRIX *)&g_mat,	NULL,	NULL,	NULL,	NULL,	(D3DXQUATERNION *)g_quatAfterTransform,	(D3DXVECTOR3 *)g_vecAfterTransform );

	D3DXMatrixMultiply ( (D3DXMATRIX *)&g_mat,	(D3DXMATRIX *)&g_mat,	(D3DXMATRIX *)in_matTransform );

	in_matTransform ->Transform ( out_vecstart, g_mat,	in_vecstart );
	in_matTransform ->Transform ( out_vecend,	g_mat,	in_vecend );

	return;
}

 
 
 
 
void	FX_CCharManager::InitNewChar	(	Fx_CHARACTER_t	*in_char	)
{
	PrepareNewAura	(	in_char	);

	in_char ->stat_sound	=	CHAR_SOUND_DISABLE;
	in_char ->soundTime		=	g_timer .GetAppMilliTime ();

	


	in_char ->flag_disappear		=	false;
	in_char ->flag_appTimeStamped	=	false;
	in_char ->flag_disappearSound	=   false;

	in_char ->stat_LODLevel			=	0;

	in_char ->flag_drawSpawnEffect	=	false;

	in_char ->atk_resend			=	false;

	return;
}

 
 
 
 
void	FX_CCharManager::PrepareNewAura (	Fx_CHARACTER_t	*in_char	)
{
	in_char ->ren_auraRHandData.made		=	false;
	in_char ->ren_auraLHandData.made		=	false;
	in_char ->ren_auraRHandData.shaderIdx	=	CHAR_SHADER_AURA_1;
	in_char ->ren_auraLHandData.shaderIdx	=	CHAR_SHADER_AURA_1;

	return;
}

 
 
 
 
void	FX_CCharManager::SetDefaultCharacter (	const int	iCharID		)
{
	Fx_CHARACTER_t	*pNewChar	=	&m_Characters [iCharID];
	
	for	(	int	j	=	0;	\
				j	<	MAX_COUNT_ITEM_PART;	\
				j++		)
		pNewChar ->stat_itemID [j]	=	-1;
	
	SetDefaultSkin (	iCharID,	SKIN_PART_HAIR	);
	SetDefaultSkin (	iCharID,	SKIN_PART_HEAD	);
	SetDefaultSkin (	iCharID,	SKIN_PART_UPPER	);
	SetDefaultSkin (	iCharID,	SKIN_PART_LOWER	);
	SetDefaultSkin (	iCharID,	SKIN_PART_HAND	);
	SetDefaultSkin (	iCharID,	SKIN_PART_FOOT	);
	SetDefaultSkin (	iCharID,	SKIN_PART_LEG	);

	pNewChar ->stat_skinID [SKIN_PART_MONSTER]	=	-1;
}

 
 
 
 
void	FX_CCharManager::SetDefaultSkin (	const int	iCharID, 
											const int	in_type		)
{
	int		defaultSkinpart;

	int		skinPartID;
	int		genType;

	Fx_CHARACTER_t	*pNewChar;

	pNewChar	=	&m_Characters [iCharID];

	 
	genType		=	pNewChar ->tableIdx;

	pNewChar	=	&m_Characters [iCharID];

	if		(	in_type	==	SKIN_PART_HAIR	)	defaultSkinpart	=	pNewChar ->skinPart [0];
	else if	(	in_type	==	SKIN_PART_HEAD	)	defaultSkinpart	=	pNewChar ->skinPart [1];
	else if	(	in_type	==	SKIN_PART_UPPER	)	defaultSkinpart	=	pNewChar ->skinPart [2];
	else if	(	in_type	==	SKIN_PART_LOWER	)	defaultSkinpart	=	pNewChar ->skinPart [3];
	else	defaultSkinpart	=	0;

	skinPartID	=	m_pSkinManager ->GetDefaultSkinID (	genType,	in_type,	defaultSkinpart		);

	pNewChar ->stat_skinID [in_type]	=	m_pSkinManager ->MakeSkinID (	in_type,	skinPartID	);
}

 
 
 
void	FX_CCharManager::Cleanup	()
{
	m_pSkelManager ->Cleanup ();
	m_pAnimManager ->Cleanup ();
	m_pSkinManager ->Cleanup ();
	m_pItemManager ->Cleanup ();
	m_pAuraManager ->Cleanup ();
	m_pMeshManager ->Cleanup ();
	m_pTagManager ->Cleanup ();
	m_pBoneManager ->Cleanup ();

	GTH_SAFE_DELETE	(	m_pItemManager	);
	GTH_SAFE_DELETE	(	m_pSkinManager	);
	GTH_SAFE_DELETE	(	m_pAnimManager	);
	GTH_SAFE_DELETE	(	m_pSkelManager	);

	GTH_SAFE_DELETE	(	m_pAuraManager	);
	GTH_SAFE_DELETE	(	m_pCharLookupMng	);
	GTH_SAFE_DELETE	(	m_pCharShaderMng	);

	GTH_SAFE_DELETE	(	m_pTagManager	);
	GTH_SAFE_DELETE	(	m_pMeshManager	);
	GTH_SAFE_DELETE	(	m_pBoneManager	);

	GTH_SAFE_DELETE	(	m_pMathTbl	);
	GTH_SAFE_DELETE	(	m_pCharUtil	);

#ifdef	_FXSKELDRAW
	GTH_SAFE_DELETE	( m_pSkelDraw );
#endif

#ifdef	_FX_TOOL
	GTH_SAFE_DELETE	( m_pMeshTool );
#endif

	return;
}

 
 
 
 
void	FX_CCharManager::SetRenderState ()
{
	m_D3DRender ->SetCharacterRenderState ();

	return;
}

 
 
 
 
void	FX_CCharManager::EquipItem (	const int			iCharID, 
										const itemTable_t	*pItem		)
{
	Fx_CHARACTER_t	*pNewChar	=	&m_Characters [iCharID];
	const int		tableIdx	=	pNewChar ->tableIdx;
	int				i;
	int				skinModelID;
 
	
	
	
	
	if( pItem ->skinningYn )
	{
		if( pItem->skinningPosition	< 0 )		return;

		skinModelID	=	GetSkinInfoWithItem( pNewChar->tableIdx, pNewChar->pcJob, pItem );

		if( skinModelID < 0 )
		{
			SetDefaultSkin( iCharID, pItem->skinningPosition );
		}
		else
		{
			pNewChar->stat_skinID [pItem->skinningPosition] = GetSkinID( pItem->skinningPosition,	skinModelID );
		}
	}
	
	else
	{
		
		if( pItem->skinningPosition	==	ITEM_WEAPON	- 0x10 )
		{
			
			if(	pItem->attackAni[pNewChar->tableIdx] != pNewChar->ani_curItemSetIndex ) 
				InitAnimation( iCharID,	ANIMTYPE_BYITEM, pItem->attackAni[pNewChar->tableIdx],	0 );
		}
		
		for	( i	= 0; i < MAX_COUNT_ITEM_PART; i ++ ) 
		{
			
			if( pItem->tagModelID[i] != -1 ) pNewChar ->stat_itemID [i] = pItem ->tagModelID [i];

			pNewChar ->stat_moveID [i]	=	pItem ->tagMoveID [i];
		}
	}
}

 
 
 
 
bool	FX_CCharManager::CharacterCollideCheck	(	Fx_CHARACTER_t	*in_char	)
{
	vec3_t	displace;

	if	(	(	in_char ->ren_closerChar	)	&&
			(	in_char ->ren_closerChar ->entityType	==	ENTITY_MONSTER	)	)
	{
		VectorSubtract ( displace,	in_char ->position,	in_char ->ren_closerChar ->position );
		if	(	VectorLength ( displace )	<	( in_char ->stat_radius + in_char ->ren_closerChar ->stat_radius )	)
		{
			in_char ->stat_collidedChar	=	in_char ->ren_closerChar;
			return	true;
		}
	}

	if	(	(	in_char ->ren_fartherChar	)	&&
			(	in_char ->ren_fartherChar ->entityType	==	ENTITY_MONSTER	)	)
	{
		VectorSubtract ( displace,	in_char ->position,	in_char ->ren_fartherChar ->position );
		if	(	VectorLength ( displace )	<	( in_char ->stat_radius + in_char ->ren_fartherChar ->stat_radius )	)
		{
			in_char ->stat_collidedChar	=	in_char ->ren_fartherChar;
			return	true;
		}
	}

	in_char ->stat_collidedChar	=	NULL;
	return	false;
}

 
 
 
 
 
void	FX_CCharManager::SetCharacterColorChangingDefined	(	Fx_CHARACTER_t	*in_char,
																int				in_color,
																float			in_applyTime	)
{
	if	(	in_color	>=	CHAR_COLOR_MAX	)
		return;

	vec3_t	startColor, endColor;

	switch	(	in_color	)
	{
	case	CHAR_COLOR_RED:
		VectorSet ( startColor, 1.1f, 0.5f, 0.5f );
		VectorSet ( endColor, 1.1f, 0.8f, 0.8f );
		break;
	case	CHAR_COLOR_GREEN:
		VectorSet ( startColor, 0.5f, 1.1f, 0.5f );
		VectorSet ( endColor, 0.8f, 1.1f, 0.8f );
		break;
	case	CHAR_COLOR_BLUE:
		VectorSet ( startColor, 0.5f, 0.5f, 1.1f );
		VectorSet ( endColor, 0.8f, 0.8f, 1.1f );
		break;
	case	CHAR_COLOR_YELLOW:
		VectorSet ( startColor, 1.1f, 1.1f, 0.5f );
		VectorSet ( endColor, 1.1f, 1.1f, 0.8f );
		break;
	case	CHAR_COLOR_CYAN:
		VectorSet ( startColor, 0.5f, 1.1f, 1.1f );
		VectorSet ( endColor, 0.8f, 1.1f, 1.1f );
		break;
	case	CHAR_COLOR_MAGENTA:
		VectorSet ( startColor, 1.1f, 0.5f, 1.1f );
		VectorSet ( endColor, 1.1f, 0.8f, 1.1f );
		break;
	}

	SetCharacterColorChanging( in_char, startColor, endColor, in_applyTime );
	return;
}

 
 
 
 
int		FX_CCharManager::SetCharacterColorChanging	(	Fx_CHARACTER_t	*in_char,
														vec3_t	in_startDiffuse,
														vec3_t	in_endDiffuse,
														float	in_applyTime	)
{
	in_char->cc_inUse		=	true;
	in_char->cc_timeElapsed	=	0.0f;
	VectorCopy ( in_char->cc_lightColorStart, in_startDiffuse );
	VectorCopy ( in_char->cc_lightColorEnd, in_endDiffuse );

	return	true;
}

 
 
 
 
void	FX_CCharManager::ApplyCharacterColorChanging	(	Fx_CHARACTER_t	*in_char	)
{
	if	(	! in_char->cc_inUse		)
		return;

	material_t	material;
	vec3_t		color;

	in_char->cc_timeElapsed	+=	m_elapsedTime;

	memset ( &material, 0, sizeof(material_t) );

	color [0]	=	( in_char->cc_lightColorEnd [0] - in_char->cc_lightColorStart [0] ) * fast_sin ( in_char->cc_timeElapsed ) + in_char->cc_lightColorStart [0];
	color [1]	=	( in_char->cc_lightColorEnd [1] - in_char->cc_lightColorStart [1] ) * fast_sin ( in_char->cc_timeElapsed ) + in_char->cc_lightColorStart [1];
	color [2]	=	( in_char->cc_lightColorEnd [2] - in_char->cc_lightColorStart [2] ) * fast_sin ( in_char->cc_timeElapsed ) + in_char->cc_lightColorStart [2];

	ColorSet ( material.diffuse, color [0], color [1], color [2], 1.0f );
	ColorSet ( material.ambient, color [0], color [1], color [2], 1.0f );

	m_D3DRender ->SetMaterial( &material );
}

 
 
 
 
void	FX_CCharManager::RestoreCharacterColorChanging	(	Fx_CHARACTER_t	*in_char	)
{
	if	(	! in_char->cc_inUse		)
		return;

	m_D3DRender ->SetMaterial ( &g_timeSystem ->m_material );
}

 
 
 
 
 
 
bool	FX_CCharManager::GetPositionForTeleport	(	int		in_targetIndex,
													int		in_targetType,
													vec3_t	out_position	)
{
	Fx_CHARACTER_t	*target;
	trace_t		trace;
	target	=	FindCharacterByServerIndex( in_targetIndex, in_targetType );
	
	if( target == NULL )
		return false;

	vec3_t	minForTrace , maxForTrace;
	vec3_t	character, targetpos;
	VectorCopy( character, m_Characters[ 0 ].position );
	VectorCopy( targetpos, target->position );
	character[ 2 ]	+=	m_Characters[ 0 ].stat_height;
	targetpos[ 2 ]	+=	target->stat_height;
	VectorSet( minForTrace, -10.0f, -10.0f, -10.0f );
	VectorSet( maxForTrace, 10.0f, 10.0f, 10.0f );

	trace	=	g_camera.m_world->Trace( character, minForTrace, maxForTrace, targetpos, MASK_PLAYERSOLID );
	if	(	trace.fraction	<	1.0f	)
	{
		return	false;
	}

	vec3_t	subtract;
	VectorSubtract( subtract, target->position, m_Characters[0].position );
	VectorNormalize( subtract );
	VectorScale( subtract, subtract, 100.0f );
	VectorSubtract( out_position, target->position, subtract );

	return	true;
}

 
 
 
 
 
 
inline void		FX_CCharManager::CalcComboAttackTime	(	Fx_CHARACTER_t	*in_char	)
{
	in_char->atk_originalAttackTime	=	m_pAnimManager->m_Anims[ in_char->ani_curIdx ].animation.m_totalTime * 1000.0f;
	in_char->atk_freeAttackTime		=	in_char->atk_originalAttackTime;
}

 
 
 
 
 
 
inline float	FX_CCharManager::CalcFreeComboAttackTime	(	Fx_CHARACTER_t	*in_char	)
{
	assert( ( in_char->atk_curCombo >= 0 ) && ( in_char->atk_curCombo < m_pAnimManager->GetComboCount( in_char->ani_curIdx ) ) );

	static	CParamBlock_CalcFreeTime	paramBlock;

	paramBlock.SetAnimIndex( in_char->ani_curIdx );
	paramBlock.SetCurrentComboCount( in_char->atk_curCombo );
	paramBlock.SetDesiredTime( in_char->stat_attackTime );

	return	m_pAnimManager->CalcFreeTime( paramBlock );
}

 
 
 
 
 
 
 
 
 
#ifdef	_FXSKELDRAW

 
 
 
 
void	FX_CCharManager::ClearSkelData ()
{
	m_pSkelDraw ->ClearAll ();

	return;
}

 
 
 
 
void	FX_CCharManager::SetSkelDrawData ()
{
	m_pSkelDraw ->ClearAll ();

	int		numBones	=	m_pSkelManager ->m_Skels [m_Characters [0].attr_skelID].skeleton.m_iNumBone;

	for	(	int	index	=	2;	\
				index	<	numBones;	\
				++index		)
	{
		m_pSkelDraw ->AddBone	(	m_Characters [0].key_resultFrames [index].vecOffset,
									m_Characters [0].key_resultFrames [m_pSkelManager ->m_Skels [m_Characters [0].attr_skelID].skeleton.m_pBones [index].iParentID].vecOffset	);
	}

	return;
}

 
 
 
 
void	FX_CCharManager::RenderSkel ()
{
	KEYFRAME	*thisFrame	=	&m_Characters [0].key_resultFrames [m_pSkelManager ->m_Skels [m_Characters [0].attr_skelID].skeleton.m_pBones [m_coordBoneIdx].iParentID];

	matrix4x4_t		mat_trans;
	matrix4x4_t		mat_rot;
	vec4_t			quat;

	D3DXQuaternionInverse	(	(D3DXQUATERNION *)quat,	(D3DXQUATERNION *)thisFrame ->quatCoordinate	);

	D3DXMatrixTranslation	(	(D3DXMATRIX *)&mat_trans,	thisFrame ->vecOffset [0],	thisFrame ->vecOffset [1],	thisFrame ->vecOffset [2]	);
	D3DXMatrixRotationQuaternion	(	(D3DXMATRIX *)&mat_rot,	(D3DXQUATERNION *)quat	);

	D3DXMatrixMultiply ( (D3DXMATRIX *)&mat_rot,	(D3DXMATRIX *)&mat_rot,	(D3DXMATRIX *)&mat_trans );

	m_pSkelDraw ->SetWorldMatrix ( m_Characters [0].stat_matTransform );
	m_pSkelDraw ->SetCoordMatrix ( mat_rot );
	m_pSkelDraw ->RenderAll ();

	return;
}

 
 
 
 
void	FX_CCharManager::SetSkelBBoxData (	const bboxf_t	in_bound	)
{
	vec3_t	start;
	vec3_t	end;

	VectorSet ( start,	in_bound [0],	in_bound [1],	in_bound [2] );
	VectorSet ( end,	in_bound [3],	in_bound [1],	in_bound [2] );
	m_pSkelDraw ->AddBone	(	start,	end	);
	VectorSet ( end,	in_bound [0],	in_bound [4],	in_bound [2] );
	m_pSkelDraw ->AddBone	(	start,	end	);
	VectorSet ( end,	in_bound [0],	in_bound [1],	in_bound [5] );
	m_pSkelDraw ->AddBone	(	start,	end	);

	VectorSet ( start,	in_bound [3],	in_bound [4],	in_bound [5] );
	VectorSet ( end,	in_bound [0],	in_bound [4],	in_bound [5] );
	m_pSkelDraw ->AddBone	(	start,	end	);
	VectorSet ( end,	in_bound [3],	in_bound [1],	in_bound [5] );
	m_pSkelDraw ->AddBone	(	start,	end	);
	VectorSet ( end,	in_bound [3],	in_bound [4],	in_bound [2] );
	m_pSkelDraw ->AddBone	(	start,	end	);

	return;
}

 
 
 
 
void	FX_CCharManager::RenderSkelBBox ()
{
	matrix4x4_t		mat;
	m_pSkelDraw ->SetWorldMatrix	(	mat	);
	m_pSkelDraw ->RenderBone ();

	return;
}

#endif	 