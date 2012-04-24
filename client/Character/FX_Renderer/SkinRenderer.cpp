 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

#define		g_quatAfterTransform	GLOBAL_VEC4_1
#define		g_quatConjugate			GLOBAL_VEC4_2
#define		g_quatBuffer			GLOBAL_VEC4_3

#define		g_matWorld		GLOBAL_DXMATRIX_1

 
 
 
FX_CSkinRenderer::FX_CSkinRenderer	()
{
	Initialize();
	
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_SKINRENDERER, sizeof(FX_CSkinRenderer) );
	}
}


 
 
 
bool	FX_CSkinRenderer::Initialize	()
{
	memset( m_charVB, 0, sizeof( m_charVB ) );
	memset( m_charIB, 0, sizeof( m_charIB ) );
	memset( m_creatureVB, 0, sizeof( m_creatureVB ) );
	memset( m_creatureIB, 0, sizeof( m_creatureIB ) );

	m_iCurBaseIndex	=	0;

	return	true;
}

 
 
 
void	FX_CSkinRenderer::SetDevice	(	LPDIRECT3DDEVICE8	device	)
{
	m_pd3dDevice	=	device;
}

 
 
 
bool	FX_CSkinRenderer::CreateBuffers	()
{
	int		index;

	for	(	index	=	0;	\
			index	<	MAX_CHAR_A_SCENE;	\
			++index		)
	{
		m_charVB[ index ]	=	new	CD3DBuffer ();
		m_charIB[ index ]	=	new	CD3DBuffer ();

		m_charVB[ index ]->Create(	VERTEX_BUFFER_SIZE,
									GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_TEXVERTEX(1),
									GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER	);
		m_charIB[ index ]->Create(	INDEX_BUFFER_SIZE,
									GTH_FORMAT_INDEX16,
									GTH_INDEX_BUFFER | GTH_DYNAMIC_BUFFER	);
	}

	for	(	index	=	0;	\
			index	<	MAX_CREATURE_A_SCENE;	\
			++index		)
	{
		m_creatureVB[ index ]	=	new	CD3DBuffer();
		m_creatureIB[ index ]	=	new	CD3DBuffer();

		m_creatureVB[ index ]->Create(	VERTEX_BUFFER_SIZE,
									GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_TEXVERTEX(1),
									GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER	);
		m_creatureIB[ index ]->Create(	INDEX_BUFFER_SIZE,
									GTH_FORMAT_INDEX16,
									GTH_INDEX_BUFFER | GTH_DYNAMIC_BUFFER	);
	}

	return	true;
}

 
 
 
bool	FX_CSkinRenderer::TransformMesh	(	Fx_CHARACTER_t	*in_char,
											const FX_CMesh	*pMesh,
											const KEYFRAME	*pKeyFrames,
											const int		in_sysVBNum,
											const int		in_skinPartID	)
{
	int		index;
	UINT	subindex;
	int		globalBoneID;
	TEXTUREVERTEX	*sysVB;
	float	numToMultiply;

	 
	sysVB	=	m_vertexBuffer;
	for	(	index	=	0;	\
			index	<	in_sysVBNum;	\
			++index,	++sysVB		)
	{
		sysVB ->vecPos [0]		=	sysVB ->vecPos [1]		=	sysVB ->vecPos [2]		=	0.0f;
		sysVB ->vecNormal [0]	=	sysVB ->vecNormal [1]	=	sysVB ->vecNormal [2]	=	0.0f;
		
		m_pVertexMap	=	&pMesh->m_meshes[ in_char->stat_LODLevel ].meshchunk->vertexMaps[ index ];
		m_pInfluence	=	&pMesh->m_meshes[ in_char->stat_LODLevel ].influences[ m_pVertexMap->iVertexID ];

		if	(	1	==	m_pInfluence->iNumBoneInfluences	)
		{
			m_pBoneInfluence	=	&m_pInfluence->boneInfluences	[0];

				 
			globalBoneID	=	m_pSkelManager->m_iBoneLookUp[ in_char->attr_skelID ][ m_pBoneInfluence->iBoneID ];

			D3DXQuaternionConjugate ( (D3DXQUATERNION *)g_quatConjugate,	(D3DXQUATERNION *)pKeyFrames [globalBoneID].quatCoordinate );

			 
			Fx_Quat3Multiply( g_quatAfterTransform,	g_quatBuffer,	pKeyFrames [globalBoneID].quatCoordinate,	m_pBoneInfluence ->quatPos,	g_quatConjugate );

			sysVB ->vecPos [0]	=	g_quatAfterTransform [0]	+	pKeyFrames [globalBoneID].vecOffset [0];
			sysVB ->vecPos [1]	=	g_quatAfterTransform [1]	+	pKeyFrames [globalBoneID].vecOffset [1];
			sysVB ->vecPos [2]	=	g_quatAfterTransform [2]	+	pKeyFrames [globalBoneID].vecOffset [2];

			 
			Fx_Quat3Multiply( g_quatAfterTransform,	g_quatBuffer,	pKeyFrames [globalBoneID].quatCoordinate,	m_pBoneInfluence ->quatNormal,	g_quatConjugate );

			VectorCopy ( sysVB ->vecNormal,	g_quatAfterTransform );
		}
		else
		{
			for	(	subindex	=	0;	\
					subindex	<	m_pInfluence ->iNumBoneInfluences;	\
					++subindex		)
			{
				m_pBoneInfluence	=	&m_pInfluence ->boneInfluences [subindex];

					 
				globalBoneID	=	m_pSkelManager->m_iBoneLookUp[ in_char->attr_skelID ][ m_pBoneInfluence->iBoneID ];
	
				D3DXQuaternionConjugate	( (D3DXQUATERNION *)g_quatConjugate,	(D3DXQUATERNION *)pKeyFrames [globalBoneID].quatCoordinate );
				 
				Fx_Quat3Multiply( g_quatAfterTransform,	g_quatBuffer,	pKeyFrames [globalBoneID].quatCoordinate,	m_pBoneInfluence ->quatPos,	g_quatConjugate	);
	
				sysVB ->vecPos [0]	+=	m_pBoneInfluence ->fWeight * ( g_quatAfterTransform [0]	+ pKeyFrames [globalBoneID].vecOffset [0] );
				sysVB ->vecPos [1]	+=	m_pBoneInfluence ->fWeight * ( g_quatAfterTransform [1]	+ pKeyFrames [globalBoneID].vecOffset [1] );
				sysVB ->vecPos [2]	+=	m_pBoneInfluence ->fWeight * ( g_quatAfterTransform [2]	+ pKeyFrames [globalBoneID].vecOffset [2] );
	
				 
				Fx_Quat3Multiply( g_quatAfterTransform,	g_quatBuffer,	pKeyFrames [globalBoneID].quatCoordinate,	m_pBoneInfluence ->quatNormal,	g_quatConjugate );
	
				sysVB ->vecNormal [0]	+=	m_pBoneInfluence->fWeight * g_quatAfterTransform [0];
				sysVB ->vecNormal [1]	+=	m_pBoneInfluence->fWeight * g_quatAfterTransform [1];
				sysVB ->vecNormal [2]	+=	m_pBoneInfluence->fWeight * g_quatAfterTransform [2];
			}
			switch	(	m_pInfluence ->iNumBoneInfluences	)
			{
			case	2:
				numToMultiply	=	0.2f;
				break;
			case	3:
				numToMultiply	=	0.333333f;
				break;
			case	4:
				numToMultiply	=	0.25f;
				break;
			case	5:
				numToMultiply	=	0.2f;
				break;
			default:
				ShowError ( "Bone Influence 개수가 6개 이상입니다!",	__FILE__,	__LINE__	);
				exit (0);
			}
			sysVB ->vecNormal [0]	*=	numToMultiply;
			sysVB ->vecNormal [1]	*=	numToMultiply;
			sysVB ->vecNormal [2]	*=	numToMultiply;
		}
		sysVB ->tu	=	m_pVertexMap ->VertexTC [0].tu;		 
		sysVB ->tv	=	1.0f - m_pVertexMap ->VertexTC [0].tv;
	}

	CopyVertex( in_char, m_vertexBuffer, in_sysVBNum, in_skinPartID );

	return	true;
}

 
 
 
bool	FX_CSkinRenderer::PrepareRender	()
{
	m_D3DRender->SetVertexShader( GTH_CHAR_FORMAT );

	return	true;
}

 
 
 
bool	FX_CSkinRenderer::Render (	Fx_CHARACTER_t	*in_char,
									int				*in_numprims,
									const int		in_skinPartIdx	)
{
	m_D3DRender->RenderDefault	(	in_char->ren_vtxBufferIdx[ in_skinPartIdx ].vbStart,
									in_char->ren_vtxBufferIdx[ in_skinPartIdx ].vbNumber,
									in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibStart,
									in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibNumber	);

	*in_numprims	+=	int(in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibNumber / 3);

	return	true;
}

 
 
 
 
void	FX_CSkinRenderer::ShaderRender (	Fx_CHARACTER_t	*in_char,
											const int		in_shaderRef,
											int				*in_numprims,
											const int		in_skinPartIdx	)
{
	shader_t	*thisShader	=	&m_shaderMng ->m_shaders[ in_shaderRef ];
#ifdef	_FX_CLIENT
	if	(	thisShader ->flags	&	SHADER_TCGEN	)
		m_shaderMng ->PrepareTCGen (	thisShader,	in_char->position	);
#endif
	
	m_D3DRender ->RenderMDLCustom	(	thisShader,
										in_char->ren_vtxBufferIdx[ in_skinPartIdx ].vbStart,
										in_char->ren_vtxBufferIdx[ in_skinPartIdx ].vbNumber,
										in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibStart,
										in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibNumber	);

	*in_numprims	+=	int(in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibNumber / 3);

	return;
}

 
 
 
void	FX_CSkinRenderer::ReleaseBuffers	()
{
	int		index;
	for	(	index	=	0;	\
			index	<	MAX_CHAR_A_SCENE;	\
			++index		)
	{
		if	(	m_charVB[ index ]	)
			m_charVB[ index ]->Release();
		if	(	m_charIB[ index ]	)
			m_charIB[ index ]->Release();

		GTH_SAFE_DELETE ( m_charVB[ index ] );
		GTH_SAFE_DELETE ( m_charIB[ index ] );
	}

	for	(	index	=	0;	\
			index	<	MAX_CREATURE_A_SCENE;	\
			++index		)
	{
		if	(	m_creatureVB[ index ]	)
			m_creatureVB[ index ]->Release();
		if	(	m_creatureIB[ index ]	)
			m_creatureIB[ index ]->Release();

		GTH_SAFE_DELETE ( m_creatureVB[ index ] );
		GTH_SAFE_DELETE ( m_creatureIB[ index ] );
	}

	return;
}


 
 
 
void	FX_CSkinRenderer::Cleanup	()
{
	ReleaseBuffers();
	
	return;
}

 
 
 
 
void	FX_CSkinRenderer::SetIndices (	Fx_CHARACTER_t	*in_char,
										const FACEINDEX	*in_indices,
										const int		in_numIndices,
										const int		in_skinPartIdx	)
{
	CD3DBuffer	*vb	=	in_char->isCreature		?	m_creatureVB[ in_char->ID ]		:	m_charVB[ in_char->ID ];
	CD3DBuffer	*ib	=	in_char->isCreature		?	m_creatureIB[ in_char->ID ]		:	m_charIB[ in_char->ID ];
	
	WORD	*buffer;
	WORD	*word	=	(WORD *)in_indices;
	WORD	base	=	(WORD)vb->GetBase();
	WORD	indices	=	3 * in_numIndices;

	ib->Lock ( in_numIndices * 3 );

	buffer	=	(WORD *)ib->GetBuffer();
	for	(	int	index	=	0;	\
				index	<	indices;	\
				++index, ++buffer, ++word	)
	{
		(*buffer)	=	(*word)	+ base;
	}

	ib->Unlock();

	in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibStart		=	ib->GetBase();
	in_char->ren_vtxBufferIdx[ in_skinPartIdx ].ibNumber	=	in_numIndices * 3;

	return;
}

 
 
 
 
void	FX_CSkinRenderer::CopyVertex (	Fx_CHARACTER_t	*in_char,
										TEXTUREVERTEX	*in_sysVB,
										const int		in_sysVBNum,
										const int		in_skelPartIdx	)
{
	CD3DBuffer	*vb	=	in_char->isCreature		?	m_creatureVB[ in_char->ID ]		:	m_charVB[ in_char->ID ];
	CD3DBuffer	*ib	=	in_char->isCreature		?	m_creatureIB[ in_char->ID ]		:	m_charIB[ in_char->ID ];

	vb->Lock( in_sysVBNum );

	memcpy( vb->GetBuffer(), in_sysVB, sizeof(TEXTUREVERTEX) * in_sysVBNum );

	vb->Unlock();

	in_char->ren_vtxBufferIdx[ in_skelPartIdx ].vbStart		=	vb->GetBase();
	in_char->ren_vtxBufferIdx[ in_skelPartIdx ].vbNumber	=	in_sysVBNum;

	return;
}

 
 
 
 
void	FX_CSkinRenderer::RenderShadow		(	Fx_CHARACTER_t	*in_char,	
												FX_CSkinManager	*skinManager	)
{
	int		index;
	CD3DBuffer	*vb	=	in_char->isCreature		?	m_creatureVB[ in_char->ID ]		:	m_charVB[ in_char->ID ];
	CD3DBuffer	*ib	=	in_char->isCreature		?	m_creatureIB[ in_char->ID ]		:	m_charIB[ in_char->ID ];

	m_D3DRender->SetIndices( ib->GetD3DIndexBuffer(),	0L );
	m_D3DRender->SetStreamSource( 0, vb->GetD3DVertexBuffer(), sizeof( TEXTUREVERTEX ) );
	for	(	index	=	0;	\
			index	<	MAX_COUNT_SKIN_PART;	\
			++index		)
	{
		if	(	in_char->stat_skinID[ index ]	<	0	)
			continue;

		m_D3DRender->RenderDefault(	in_char->ren_vtxBufferIdx[ index ].vbStart,
									in_char->ren_vtxBufferIdx[ index ].vbNumber,
									in_char->ren_vtxBufferIdx[ index ].ibStart,
									in_char->ren_vtxBufferIdx[ index ].ibNumber	);
	}
}


 
 
 
 
 
 
 
void	FX_CSkinRenderer::SetStreamSource	(	Fx_CHARACTER_t	*in_char,	
												const int		in_streamIdx	)
{
	m_D3DRender->SetStreamSource(	in_streamIdx,
									( in_char->isCreature ? m_creatureVB[ in_char->ID ] : m_charVB[ in_char->ID ] )->GetD3DVertexBuffer(),
									sizeof( TEXTUREVERTEX )		);
}

 
 
 
 
 
 
 
void	FX_CSkinRenderer::DiscardIndexBuffer	(	Fx_CHARACTER_t	*in_char	)
{
	( in_char->isCreature ? m_creatureIB[ in_char->ID ] : m_charIB[ in_char->ID ] )->Discard();
}

 
 
 
 
 
 
 
void	FX_CSkinRenderer::DiscardVertexBuffer	(	Fx_CHARACTER_t	*in_char	)
{
	( in_char->isCreature ? m_creatureVB[ in_char->ID ] : m_charVB[ in_char->ID ] )->Discard();
}

 
 
 
 
 
 
 
LPDIRECT3DINDEXBUFFER8	FX_CSkinRenderer::GetIndexBuffer	(	Fx_CHARACTER_t	*in_char	)
{
	return	( in_char->isCreature	?	m_creatureIB[ in_char->ID ]		:	m_charIB[ in_char->ID ]	)->GetD3DIndexBuffer();
}
