 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"
#include "Mmsystem.h"

#define		g_rotmatrix			GLOBAL_DXMATRIX_1

#define		g_resultMatrix		GLOBAL_MATRIX_1

 
 
 
FX_CSkinManager::FX_CSkinManager	()
{
	int i, j, k;

	for	(	int	index	=	0;	\
				index	<	MAX_COUNT_SKIN_PART;	\
				index++	)
		m_meshOffset[index]	=	-1;	

	for( i = 0; i < NUM_GEN_TYPE; i ++ )
	{
		for( j = 0; j < MAX_COUNT_SKIN_PART; j ++ )
		{
			m_defSkinList[i].count[j] = 0;
			for( k = 0; k < MAX_DEF_SKIN; k ++ )
				m_defSkinList[i].list[j][k] = -1;
		}
	}
	
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_SKINMANAGER, sizeof(FX_CSkinManager) );
	}
}

 
 
 
bool	FX_CSkinManager::LoadSkinList	()
{
	int		index,	subindex;
	UINT	iRefID;
	int		errorval;
	int		oldcount;

	m_iNumSkins	=	0;
	for	(	index	=	0;	\
			index	<	MAX_COUNT_SKIN_PART;	\
			++index		)
	{
		switch ( index )
		{
		case	SKIN_PART_HAIR:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_HAIR	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_HEAD:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_HEAD	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_UPPER:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_UPPER	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_LOWER:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_LOWER	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_HAND:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_HAND	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_LEG:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_LEG	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_FOOT:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_FOOT	);
			if	(	! errorval )	continue;
			break;
		case	SKIN_PART_MONSTER:
			errorval	=	m_pScript->Load	(	FILENAME_SKIN_MONSTER	);
			if	(	! errorval )	continue;
			break;
		}

		m_pScript->BeginParse	();

		oldcount	=	m_iNumSkins;
		m_meshOffset [index]	=	oldcount;
		m_iNumSkins	+=	atoi ( m_pScript->GetNextToken() );

		for	(	subindex	=	oldcount;	\
				subindex	<	m_iNumSkins;	\
				++subindex	)
		{
			strcpy	(	m_SkinBuffer [subindex].name,
						m_pScript->GetNextToken()		);
			m_SkinBuffer [subindex].id	=	subindex;
			sprintf	(	m_SkinBuffer [subindex].texname,
						"%s",
						m_pScript->GetNextToken()		);
			iRefID	=	m_pCharShaderMng ->SearchShaderRef (	m_SkinBuffer [subindex].texname	);
	
			LoadSkin(	m_SkinBuffer [subindex].name,
						m_SkinBuffer [subindex].id,
						iRefID	);
			m_Skins	[subindex].partID	=	index;
		}
	}

	return	true;
}

 
 
 
 
void	FX_CSkinManager::SetShaderMng	(	CShaderMng		*shaderMng	)
{
	m_shaderMng	=	shaderMng;

	m_pSkinRenderer ->SetShaderMng (	shaderMng	);

	return;
}


 
 
 
void	FX_CSkinManager::UpdateTextureID	()
{
	for	(	int	index	=	0;	\
				index	<	m_iNumSkins;	\
				++index		)
	{
		m_Skins	[ m_SkinBuffer [index].id ].iRefID	
			=	m_pCharShaderMng ->SearchShaderRef (	m_SkinBuffer [index].texname	);
	}
}

 
 
 
bool	FX_CSkinManager::LoadSkin	(	char				*pszSkinName,
										const unsigned int	&iSkinID,
										const unsigned int	&iRefID		)
{
	SKIN_t*		pSkin;

	pSkin	=	&m_Skins	[iSkinID];

	pSkin->mesh.LoadData	(	pszSkinName,
								m_fileMng	);

	pSkin->iRefID	=	iRefID;

	return	true;
}

 
 
 
void	FX_CSkinManager::SetDevice	(	LPDIRECT3DDEVICE8	device	)
{
	m_pd3dDevice	=	device;

	m_pSkinRenderer->SetDevice	(	m_pd3dDevice	);
}

 
 
 
void	FX_CSkinManager::SetTexManager	(	CD3DTextureMng	*texMng	)
{
	m_texMng	=	texMng;
}

 
 
 
 
void	FX_CSkinManager::SetRenderer (	CD3DRender		*d3dRender	)
{
	m_D3DRender	=	d3dRender;

	m_pSkinRenderer ->SetRenderer (	d3dRender	);

	return;
}

 
 
 
bool	FX_CSkinManager::PrepareRenderBuffers	()
{
	m_pSkinRenderer->CreateBuffers();

	return	true;
}


 
 
 
bool	FX_CSkinManager::ReleaseRenderBuffers	()
{
	m_pSkinRenderer ->ReleaseBuffers();
		
	return	true;
}

 
 
 
bool	FX_CSkinManager::RenderAllNormal	(	int		*in_numprims	)
{
	SKIN_t			*pThisSkin;
	CHAR_BUFFER_t	*pCharBuffer;	 
	int				iRefIndex;		 
	KEYFRAME		*pKeyFrames;	 
	Fx_CHARACTER_t	*pCharacter;	 
	extern Fx_CHARACTER_t	*g_mouseSelectedCharacter;
	int				index;

	int				forceUpdate	=	m_pCharManager ->GetForceUpdateSkin ();

#ifdef	_FX_CLIENT
	Fx_CHARACTER_t	*pTargetChar	=	NULL;

	if	(	g_cgv.state	==	CLIENT_GAME_PROCESS	)
	{
		if	(	g_mouseSelectedCharacter	!=	NULL	&&	g_cgv.pTargetChar	!=	NULL	)
			pTargetChar	=	g_cgv.pTargetChar;
		if	(	g_mouseSelectedCharacter	!=	NULL	&&	g_cgv.pTargetChar	==	NULL	)
			pTargetChar	=	g_mouseSelectedCharacter;
		if	(	g_mouseSelectedCharacter	==	NULL	&&	g_cgv.pTargetChar	!=	NULL	)
			pTargetChar	=	g_cgv.pTargetChar;
		if	(	g_mouseSelectedCharacter	==	NULL	&&	g_cgv.pTargetChar	==	NULL	)
			pTargetChar	=	NULL;
	}
	else
			pTargetChar	=	g_mouseSelectedCharacter;
#endif	 

	m_pSkinRenderer ->PrepareRender ();

	for	(	index	=	0;
			index	<	m_iNumMeshInBuffer;
			++index		)
	{
		pCharBuffer		=	&m_CharBuffer [index];

		pThisSkin		=	&m_Skins[ pCharBuffer->meshID ];
		iRefIndex		=	pThisSkin ->iRefID;

		SetTextureByRef ( iRefIndex );

		for	(	int	subindex	=	0;	\
					subindex	<	pCharBuffer->count;	\
					++subindex		)		
		{
			pCharacter	=	pCharBuffer->characterPtr [subindex];

			if	(	pCharacter->isCreature	)
			{
				int	aaa	=	0;
			}

			 
			if	(	pCharacter ->flag_disappear	)	continue;
			if	(	! pCharacter->ren_renderThisFrame	)	continue;

			m_pSkinRenderer->SetStreamSource( pCharacter, 0 );

			 
			 
			if	(	! pCharacter->isCreature	&&
					pCharacter->ID	==	0	)
				m_pCharManager ->ApplyCharacterColorChanging( pCharacter );

			if	(	m_pCharManager->CheckSkinUpdateTime( pCharacter )	||
					forceUpdate		||
					pCharacter->flag_forceUpdate	)
			{  
				pKeyFrames	=	pCharacter ->key_resultFrames;

				 
				m_pSkinRenderer->TransformMesh	(	pCharacter,
													&pThisSkin->mesh,
													pKeyFrames,
													GetNumVertex( pCharacter->stat_skinID [pThisSkin ->partID],	pCharacter->stat_LODLevel ),
													pThisSkin->partID	);
				m_pSkinRenderer ->SetIndices (	pCharacter,
												pThisSkin->mesh.m_meshes[ pCharacter->stat_LODLevel ].meshchunk->faceLists,
												pThisSkin->mesh.m_meshes[ pCharacter->stat_LODLevel ].meshchunk->iNumFace,
												pThisSkin->partID	);
			}

			m_D3DRender ->SetIndices( m_pSkinRenderer->GetIndexBuffer( pCharacter ), 0L );

#ifdef	_FX_CLIENT
			if	(	(	pTargetChar	!=	pCharacter	)	||
					(	(	pCharacter ->entityType	!=	ENTITY_PC	)	&&
						(	pCharacter ->stat_alive	>	MON_ALIVE_STATE_CORPSE	)	)	)
#else
			if	(	true	)
#endif	 
			{
				if	(	m_pCharShaderMng ->CheckIsShader ( iRefIndex )	)
				{
					if	(	pCharacter->cr_inUse	)
					{
						matrix4x4_t	*mat_world	=	m_pCharManager->m_pItemManager->GetTagMatrixOut( pCharacter->cr_creature, ITEM_PART_CREATURE_TYPE_0 );
						if	(	! mat_world	)
							continue;
						m_D3DRender->SetTransform( D3DTS_WORLD, mat_world );
					}
					else
					{
						m_D3DRender->SetTransform( D3DTS_WORLD, &pCharacter->stat_matTransform );
					}
					m_pSkinRenderer->ShaderRender(	pCharacter, m_pCharShaderMng->GetShaderIdx( iRefIndex ), in_numprims, pThisSkin->partID );
				}
				else
				{
					if	(	pCharacter->cr_inUse	)
					{
						matrix4x4_t	*mat_world	=	m_pCharManager->m_pItemManager->GetTagMatrixOut( pCharacter->cr_creature, ITEM_PART_CREATURE_TYPE_0 );
						if	(	! mat_world	)
							continue;
						m_D3DRender->SetTransform( D3DTS_WORLD, mat_world );
					}
					else
					{
						m_D3DRender->SetTransform( D3DTS_WORLD, &pCharacter->stat_matTransform );
					}
					m_pSkinRenderer->Render( pCharacter, in_numprims, pThisSkin->partID );
				}
			}
			else
			{
				if	(	pCharacter->cr_inUse	)
				{
					matrix4x4_t	*mat_world	=	m_pCharManager->m_pItemManager->GetTagMatrixOut( pCharacter->cr_creature, ITEM_PART_CREATURE_TYPE_0 );
					if	(	! mat_world	)
						continue;
					m_D3DRender->SetTransform( D3DTS_WORLD, mat_world );
				}
				else
				{
					m_D3DRender->SetTransform( D3DTS_WORLD, &pCharacter->stat_matTransform );
				}

				if	(	m_pCharShaderMng ->CheckIsShader ( iRefIndex )	)
				{
					m_pSkinRenderer->ShaderRender(	pCharacter, m_pCharShaderMng->GetShaderIdx( iRefIndex ), in_numprims, pThisSkin->partID );
				}
				else
				{
					m_pSkinRenderer->Render( pCharacter, in_numprims, pThisSkin->partID );
				}
			}	 

			 
			if	(	! pCharacter->isCreature	&&
					pCharacter->ID	==	0	)
				m_pCharManager ->RestoreCharacterColorChanging( pCharacter );
		}	 
	}


	return	true;
}

#ifdef	_FX_CLIENT

 
 
 
bool	FX_CSkinManager::RenderAllDisappear	(	int		*in_numprims	)
{
	SKIN_t			*pThisSkin;
	CHAR_BUFFER_t	*pCharBuffer;	 
	int				iRefIndex;		 
	KEYFRAME		*pKeyFrames;	 
	Fx_CHARACTER_t	*pCharacter;	 
	extern Fx_CHARACTER_t	*g_mouseSelectedCharacter;
	int				index;

	int				forceUpdate	=	m_pCharManager ->GetForceUpdateSkin ();
	bool			flag_disappear;

	Fx_CHARACTER_t	*pTargetChar	=	NULL;

	if	(	g_cgv.state	==	CLIENT_GAME_PROCESS	)
	{
		if	(	g_mouseSelectedCharacter	!=	NULL	&&	g_cgv.pTargetChar	!=	NULL	)
			pTargetChar	=	g_cgv.pTargetChar;
		if	(	g_mouseSelectedCharacter	!=	NULL	&&	g_cgv.pTargetChar	==	NULL	)
			pTargetChar	=	g_mouseSelectedCharacter;
		if	(	g_mouseSelectedCharacter	==	NULL	&&	g_cgv.pTargetChar	!=	NULL	)
			pTargetChar	=	g_cgv.pTargetChar;
		if	(	g_mouseSelectedCharacter	==	NULL	&&	g_cgv.pTargetChar	==	NULL	)
			pTargetChar	=	NULL;
	}
	else
			pTargetChar	=	g_mouseSelectedCharacter;

	m_pSkinRenderer ->PrepareRender ();
	
	for	(	index	=	0;
			index	<	m_iNumMeshInBuffer;
			++index		)
	{
		pCharBuffer		=	&m_CharBuffer [index];

		pThisSkin		=	&m_Skins[ pCharBuffer->meshID ];
		iRefIndex		=	pThisSkin ->iRefID;
		flag_disappear	=	false;

		SetTextureByRef ( iRefIndex );

		for	(	int	subindex	=	0;	\
					subindex	<	pCharBuffer->count;	\
					++subindex		)		
		{
			pCharacter	=	pCharBuffer->characterPtr [subindex];

			if	(	! pCharacter ->flag_disappear	)	continue;
			if	(	! pCharacter ->flag_disappearSound )
			{
				pCharacter ->flag_disappearSound = true;
			}
			if	(	! pCharacter->ren_renderThisFrame	)	continue;

			m_pSkinRenderer->SetStreamSource( pCharacter, 0 );

			if	(	! pCharacter ->flag_appTimeStamped		)
			{
				g_effectMng ->Add ( GTH_EFFECT_CLASS_MODEL , 8 , pCharacter ->position , NULL , pCharacter ->angles , 0.0f , pCharacter ->entityType , pCharacter ->idxOnServer );
				pCharacter ->ren_stampedAppTime	=	g_timer.m_currSysTime;
				pCharacter ->flag_appTimeStamped	=	true;
			}
			m_pCharShaderMng ->PrepareTimeEffectShaderByRef ( CHAR_SHADER_MONSTER_DEAD_1,	(g_timer .m_currSysTime - pCharacter ->ren_stampedAppTime) );
			flag_disappear	=	true;

			if	(	m_pCharManager->CheckSkinUpdateTime( pCharacter )	||
					forceUpdate		||
					pCharacter->flag_forceUpdate	)
			{  
				pKeyFrames	=	pCharacter ->key_resultFrames;

				 
				m_pSkinRenderer->TransformMesh	(	pCharacter,
													&pThisSkin ->mesh,
													pKeyFrames,
													GetNumVertex( pCharacter->stat_skinID[ pThisSkin->partID ],	pCharacter->stat_LODLevel ),
													pThisSkin->partID	);
				m_pSkinRenderer ->SetIndices(	pCharacter,
												pThisSkin->mesh.m_meshes[ pCharacter->stat_LODLevel ].meshchunk->faceLists,
												pThisSkin->mesh.m_meshes[ pCharacter->stat_LODLevel ].meshchunk->iNumFace,
												pThisSkin->partID	);
			}
			m_D3DRender->SetIndices( m_pSkinRenderer->GetIndexBuffer( pCharacter ), 0L );

			if	(	(	pTargetChar	!=	pCharacter	)	||
					(	(	pCharacter ->entityType	!=	ENTITY_PC	)	&&
						(	pCharacter ->stat_alive	>	MON_ALIVE_STATE_CORPSE	)	)	)
			{
				float	modifier	=	(g_timer .m_currSysTime - pCharacter ->ren_stampedAppTime)	+	1.0f;
				GLOBAL_SCALEMATRIX.a00	=	GLOBAL_SCALEMATRIX.a11	=	1.0f / modifier;
				GLOBAL_SCALEMATRIX.a22	=	modifier;
				modifier	=	(modifier - 1.0f) * 2.0f;
				D3DXMatrixRotationZ	(	(D3DXMATRIX *)&g_rotmatrix,	modifier	);
				modifier	*=	10.0f;
				pCharacter ->stat_matTransform.a23	+=	modifier;
				D3DXMatrixMultiply (	(D3DXMATRIX *)&g_resultMatrix,	(D3DXMATRIX *)&GLOBAL_SCALEMATRIX,	(D3DXMATRIX *)&g_rotmatrix	);
				D3DXMatrixMultiply (	(D3DXMATRIX *)&g_resultMatrix,	(D3DXMATRIX *)&g_resultMatrix,		(D3DXMATRIX *)&pCharacter ->stat_matTransform	);
				m_D3DRender ->SetTransform	(	D3DTS_WORLD,	&g_resultMatrix	);
				pCharacter ->stat_matTransform.a23	-=	modifier;

				m_pSkinRenderer->ShaderRender(	pCharacter, m_pCharShaderMng->GetEffectShaderIdx( CHAR_SHADER_MONSTER_DEAD_1 ), in_numprims, pThisSkin->partID );
			}
		}	 
	}

	return	true;
}

#endif	 

 
 
 
bool	FX_CSkinManager::SetRenderState	()
{
	return	true;
}


 
 
 
bool	FX_CSkinManager::UpdateCharBuffer	()
{
	 
	for	(	int	index	=	0;
				index	<	MAX_COUNT_SKIN_MESH;
				++index		)
	{
		m_iBufferIndexMap [index]	=	-1;
	}
	m_iNumMeshInBuffer	=	0;

	Fx_CHARACTER_t	*pCharacter;		 
	int				iSkinID;			 
	CHAR_BUFFER_t	*pCharBuffer;		 

	 
	for	(	index	=	0;	\
			index	<	m_pCharManager->m_iNumCharacters;	\
			++index		)
	{
		 
		pCharacter	=	&m_pCharManager->m_Characters [index];
		
		 
		for	(	int	subindex	=	0;
					subindex	<	MAX_COUNT_SKIN_PART;
					++subindex	)
		{
			 
			iSkinID		=	pCharacter->stat_skinID [subindex];

			 
			if	(	iSkinID	==	-1	)
				continue;

			if	(	m_iBufferIndexMap [iSkinID]	==	-1	)
			{	 
				 
				m_iBufferIndexMap [iSkinID]	=	m_iNumMeshInBuffer;		 
				 
				pCharBuffer					=	&m_CharBuffer [m_iNumMeshInBuffer];
				pCharBuffer->meshID		=	iSkinID;
				pCharBuffer->count		=	1;
				pCharBuffer->characterPtr[ 0 ]	=	pCharacter;
				 
				++m_iNumMeshInBuffer;
			}
			else
			{	 
				 
				pCharBuffer		=	&m_CharBuffer [m_iBufferIndexMap [iSkinID]];
				 
				++(pCharBuffer->count);
				pCharBuffer->characterPtr[ pCharBuffer->count - 1 ]	=	pCharacter;
			}
		}
	}

	ITOR_LIST_CREATURE	itor;
	for	(	itor	=	m_pCharManager->m_usedCreatureList.begin();	\
			itor	!=	m_pCharManager->m_usedCreatureList.end();	\
			++itor	)
	{
		 
		pCharacter	=	(*itor);
		
		 
		for	(	int	subindex	=	0;
					subindex	<	MAX_COUNT_SKIN_PART;
					++subindex	)
		{
			 
			iSkinID		=	pCharacter->stat_skinID [subindex];

			 
			if	(	iSkinID	==	-1	)
				continue;

			if	(	m_iBufferIndexMap [iSkinID]	==	-1	)
			{	 
				 
				m_iBufferIndexMap [iSkinID]	=	m_iNumMeshInBuffer;		 
				 
				pCharBuffer					=	&m_CharBuffer [m_iNumMeshInBuffer];
				pCharBuffer->meshID		=	iSkinID;
				pCharBuffer->count		=	1;
				pCharBuffer->characterPtr[ 0 ]	=	pCharacter;
				 
				++m_iNumMeshInBuffer;
			}
			else
			{	 
				 
				pCharBuffer		=	&m_CharBuffer [m_iBufferIndexMap [iSkinID]];
				 
				++(pCharBuffer->count);
				pCharBuffer->characterPtr[ pCharBuffer->count - 1 ]	=	pCharacter;
			}
		}
	}

	return	true;		
}


 
 
 
void	FX_CSkinManager::Cleanup	()
{
	for	(	int	index	=	0;
				index	<	m_iNumSkins;
				++index		)
	{
		m_Skins [index]	.mesh	.Cleanup();
	}

	m_pSkinRenderer->Cleanup();

	GTH_SAFE_DELETE ( m_pSkinRenderer );

	return;
}

 
 
 
 
int		FX_CSkinManager::MakeSkinID	(	const int	in_type,
										const int	in_internalID	)
{
	if( in_internalID < 0 ) return( -1 );
	else return	m_meshOffset [in_type] + in_internalID;
}

 
 
 
 
void	FX_CSkinManager::SplitSkinID	(	const int	in_skinID,
											int			&out_type,
											int			&out_internalID		)
{
	for	(	int	index	=	0;	\
				index	<	MAX_COUNT_SKIN_PART;	\
				++index		)
	{
		if	(	m_meshOffset [index]	<	in_skinID	)
			continue;
		break;
	}

	out_type		=	index - 1;
	out_internalID	=	in_skinID - m_meshOffset [out_type];

	return;
}

 
 
 
 
void	FX_CSkinManager::SetSkelManager	(	FX_CSkelManager		*skelManager	)
{
	m_pSkelManager	=	skelManager;

	m_pSkinRenderer ->SetSkelManager (	m_pSkelManager	);

	return;
}

 
 
 
 
int		FX_CSkinManager::LoadDefSkinList ()
{
	char	*token;

		 
	memset	(	m_defSkinList,	0,	sizeof (m_defSkinList)	);

	m_pScript ->Load	(	FILENAME_SKINDFT_LIST	);
	m_pScript ->BeginParse ();

	while ( 1 )
	{
		token	=	m_pScript ->GetNextToken ( true );
		if (	! token [0]	)	break;

		if (	token [0]	!=	'{'	)	continue;

		while ( 1 )
		{
			token	=	m_pScript ->GetNextToken ( true );

			if (	! token [0]	)	break;
			if	(	token [0]	==	'}'	)	break;

			ParseSubDefSkinList (	token	);
		}
	}

	return	true;
}

 
 
 
 
int		FX_CSkinManager::ParseSubDefSkinList (	char	*in_token	)
{
	static int		gen_type;
	char			*token;

	if	(	0	==	_stricmp (	in_token,	"gen_type"	)	)
	{
		token	=	m_pScript ->GetNextToken ( true );
		if	(	0	==	_stricmp (	token,	"mentality"	)	)
		{
			gen_type	=	GEN_TYPE_SPIRIT;
		}
		else if	(	0	==	_stricmp (	token,	"wind"	)	)
		{
			gen_type	=	GEN_TYPE_WIND;
		}
		else if	(	0	==	_stricmp (	token,	"water"	)	)
		{
			gen_type	=	GEN_TYPE_WATER;
		}
		else if	(	0	==	_stricmp (	token,	"earth"	)	)
		{
			gen_type	=	GEN_TYPE_SOIL;
		}
		else if	(	0	==	_stricmp (	token,	"poison"	)	)
		{
			gen_type	=	GEN_TYPE_POISON;
		}
		else if	(	0	==	_stricmp (	token,	"iron"	)	)
		{
			gen_type	=	GEN_TYPE_IRON;
		}
		else if	(	0	==	_stricmp (	token,	"fire"	)	)
		{
			gen_type	=	GEN_TYPE_FIRE;
		}
		else if	(	0	==	_stricmp (	token,	"body"	)	)
		{
			gen_type	=	GEN_TYPE_BODY;
		}
	}
	else if	(	0	==	_stricmp (	in_token,	"hair"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_HAIR],	m_defSkinList [gen_type].count [SKIN_PART_HAIR]	);
	}
	else if	(	0	==	_stricmp (	in_token,	"head"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_HEAD],	m_defSkinList [gen_type].count [SKIN_PART_HEAD]	);
	}
	else if	(	0	==	_stricmp (	in_token,	"upper"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_UPPER],	m_defSkinList [gen_type].count [SKIN_PART_UPPER]	);
	}
	else if	(	0	==	_stricmp (	in_token,	"lower"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_LOWER],	m_defSkinList [gen_type].count [SKIN_PART_LOWER]	);
	}
	else if	(	0	==	_stricmp (	in_token,	"hand"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_HAND],	m_defSkinList [gen_type].count [SKIN_PART_HAND]	);
	}
	else if	(	0	==	_stricmp (	in_token,	"leg"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_LEG],	m_defSkinList [gen_type].count [SKIN_PART_LEG]	);
	}
	else if	(	0	==	_stricmp (	in_token,	"foot"	)	)
	{
		ParseNextLevel (	m_defSkinList [gen_type].list [SKIN_PART_FOOT],	m_defSkinList [gen_type].count [SKIN_PART_FOOT]	);
	}

	return	true;
}

 
 
 
 
int		FX_CSkinManager::ParseNextLevel (	int		*out_list,
											int		&out_count	)
{
	char	*token;

	token	=	m_pScript ->GetNextToken ( true );
	if	(	token [0]	!=	'{'	)	return	-1;

	out_count	=	0;
	while ( 1 )
	{
		token	=	m_pScript ->GetNextToken ( true );

		if	(	token [0]	==	'}'	)	break;

		out_list [out_count++]	=	atoi (	token	);
	}

	return	true;
}

 
 
 
 
void	FX_CSkinManager::GetDefaultSkin	(	const int	in_gentype,	
											const int	in_type,
											int			*out_list,	
											int			&out_count	)
{
	out_count	=	m_defSkinList [in_gentype].count [in_type];
	
	for	(	int	index	=	0;	\
				index	<	out_count;	\
				++index		)
	{
		out_list [index]	=	m_defSkinList [in_gentype].list [in_type][index];
	}
	
	return;
}

 
 
 
 
int		FX_CSkinManager::GetDefaultSkinID	(	const int	in_gentype, 
												const int	in_type, 
												const int	idx		)
{
	
	if( m_defSkinList[in_gentype].count[in_type] == 0 ) return( -1 );
	return( m_defSkinList[in_gentype].list[in_type][idx] );
}

 
 
 
 
int		FX_CSkinManager::GetNumVertex	(	const int	in_skinID,	
											const int	in_LODLevel	)
{
	return	m_Skins [in_skinID].mesh.m_meshes [in_LODLevel].meshchunk ->iNumVertexMap;
}

 
 
 
 
int		FX_CSkinManager::GetNumIndex	(	const int	in_skinID,	
											const int	in_LODLevel	)
{
	return	(m_Skins [in_skinID].mesh.m_meshes [in_LODLevel].meshchunk ->iNumFace * 3);
}

 
 
 
 
void	FX_CSkinManager::SetTextureByRef	(	int		in_ref	)
{
	if	(	m_pCharShaderMng ->CheckIsShader ( in_ref )	)
	{
		m_pCharShaderMng ->PrepareShaderByRef (	in_ref	);
	}
	else
	{
		m_D3DRender ->SetTexture	(	0,	m_pCharShaderMng ->GetD3DTextureByRef ( in_ref )	);
	}
}

 
 
 
 
 
 
 
void	FX_CSkinManager::DiscardAllIndexBuffers	()
{
	int		index;
	Fx_CHARACTER_t	*thisChar	=	&m_pCharManager->m_Characters[0];
	for	(	index	=	0;	\
			index	<	MAX_CHAR_A_SCENE;	\
			++index,	++thisChar	)
	{
		m_pSkinRenderer->DiscardIndexBuffer( thisChar );
	}
}

 
 
 
 
 
 
 
void	FX_CSkinManager::DiscardIndexBuffer	(	Fx_CHARACTER_t	*in_char	)
{
	m_pSkinRenderer->DiscardIndexBuffer( in_char );
}

 
 
 
 
 
 
 
void	FX_CSkinManager::DiscardVertexBuffer	(	Fx_CHARACTER_t	*in_char	)
{
	m_pSkinRenderer->DiscardVertexBuffer( in_char );
}

 
 
 
 
 
 
 
void	FX_CSkinManager::DiscardAllVertexBuffers	()
{
	int		index;
	Fx_CHARACTER_t	*thisChar	=	&m_pCharManager->m_Characters[0];
	for	(	index	=	0;	\
			index	<	MAX_CHAR_A_SCENE;	\
			++index		)
	{
		m_pSkinRenderer->DiscardVertexBuffer( thisChar );
	}

}