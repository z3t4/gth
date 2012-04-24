 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
FX_CItemRenderer::FX_CItemRenderer	()
{
	m_VB	=	NULL;
	m_IB	=	NULL;

	Initialize();
	
	if	(	g_resCounter->Enabled()	)
	{
		g_resCounter->Add_DataType( RESCOUNTER_CHAR_ITEMRENDERER, sizeof(FX_CItemRenderer) );
	}
}


 
 
 
bool	FX_CItemRenderer::Initialize	()
{
	return	true;
}


 
 
 
bool	FX_CItemRenderer::CreateBuffers	()
{
	m_VB	=	new	CD3DBuffer ();
	m_IB	=	new	CD3DBuffer ();

	UINT	iVerticesCount	=	0;
	UINT	iIndicesCount	=	0;

	 
	m_pItemManager->CountVertices(	iVerticesCount,
									iIndicesCount	);

#ifdef	_FXDEBUG
	CHECKERROR	(	iVerticesCount	<=	0,	"버텍스 카운트가 잘못되었음!"	)
	CHECKERROR	(	iIndicesCount	<=	0,	"인덱스 카운트가 잘못되었음!"	)
#endif

	 
	m_VB ->Create	(	iVerticesCount,
								GTH_FORMAT_XYZ | GTH_FORMAT_NORMAL | GTH_FORMAT_TEXVERTEX(1),
								GTH_VERTEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY	);

	 
	m_IB ->Create	(	iIndicesCount,
								GTH_FORMAT_INDEX16,
								GTH_INDEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY	);

	return	true;
}


 
 
 
bool	FX_CItemRenderer::FillBuffers	()
{
	 
	
	TEXTUREVERTEX*		pVertices;				 
	WORD*				pIndices;				 

	 
	pVertices	=	(TEXTUREVERTEX *)m_VB ->Lock	();

	 
	pIndices	=	(WORD *)m_IB ->Lock	();

	FX_CMesh*	pMesh;			 
	VERTEXMAP*	pVertexMap;		 

	int		iCurTotalVertices	=	0;

	 
	for	(	int	index	=	0;
				index	<	m_pItemManager->m_iNumItems;
				++index		)		
	{
		pMesh		=	&m_pItemManager->m_Items [index].mesh;
		pVertexMap	=	pMesh ->m_meshes [0].meshchunk ->vertexMaps;

		 
		for	(	int	subindex	=	0;	\
					subindex	<	pMesh ->m_meshes [0].meshchunk ->iNumVertexMap;	\
					++subindex	)
		{
			VectorCopy ( pVertices ->vecPos,	pMesh ->m_meshes [0].influences [pVertexMap [subindex].iVertexID].boneInfluences [0].quatPos );
			VectorCopy ( pVertices ->vecNormal,	pMesh ->m_meshes [0].influences [pVertexMap [subindex].iVertexID].boneInfluences [0].quatNormal );

			pVertices ->tu	=	pVertexMap [subindex] .VertexTC	[0].tu;
			pVertices ->tv	=	1.0f - pVertexMap [subindex].VertexTC [0].tv;
			++pVertices;
		}

		 
		for	(	subindex	=	0;	\
				subindex	<	pMesh ->m_meshes [0].meshchunk ->iNumFace;	\
				++subindex	)
		{
			*pIndices++	=	(WORD) pMesh ->m_meshes [0].meshchunk ->faceLists [subindex].index [0]	+	iCurTotalVertices;
			*pIndices++	=	(WORD) pMesh ->m_meshes [0].meshchunk ->faceLists [subindex].index [1]	+	iCurTotalVertices;
			*pIndices++	=	(WORD) pMesh ->m_meshes [0].meshchunk ->faceLists [subindex].index [2]	+	iCurTotalVertices;
		}
		iCurTotalVertices	+=	pMesh ->m_meshes [0].meshchunk ->iNumVertexMap;
	}

	 
	m_VB ->Unlock	();

	 
	m_IB ->Unlock	();

	return	true;
}


 
 
 
bool	FX_CItemRenderer::PrepareRender	()
{
	m_D3DRender ->SetVertexShader	(	GTH_CHAR_FORMAT	);
	m_D3DRender ->SetStreamSource	(	0,	m_VB ->GetD3DVertexBuffer(),	sizeof (TEXTUREVERTEX)	);
	m_D3DRender ->SetIndices	(	m_IB ->GetD3DIndexBuffer (),	0L	);

	return	true;
}


 
 
 
bool	FX_CItemRenderer::Render	(	const UINT	&iVertexStart,
										const UINT	&iNumVertex,
										const UINT	&iIndexStart,
										const UINT	&iPrimitiveCount		)
{
	m_pd3dDevice->DrawIndexedPrimitive	(	D3DPT_TRIANGLELIST,
											iVertexStart,
											iNumVertex,
											iIndexStart,
											iPrimitiveCount	);

	return	true;
}


 
 
 
void	FX_CItemRenderer::ReleaseBuffers	()
{
	if	(	m_VB	)	m_VB ->Release ();
	if	(	m_IB	)	m_IB ->Release ();
	GTH_SAFE_DELETE ( m_VB );
	GTH_SAFE_DELETE ( m_IB );

	return;
}


 
 
 
void	FX_CItemRenderer::Cleanup	()
{
	ReleaseBuffers ();
	
	return;
}

 
 
 
 
void	FX_CItemRenderer::RenderShadow	(	Fx_CHARACTER_t		*in_char,
											FX_CItemManager		*itemManager, 
											FX_CBoneManager		*boneManager,
											matrix4x4_t			*in_transform	)
{
	int		index;
	int		indices;
	int		vertices;
	ITEM_t	*pThisItem;
	matrix4x4_t	unTransformed;
	matrix4x4_t	transformed;

	m_D3DRender ->SetStreamSource	(	0,	m_VB ->GetD3DVertexBuffer(),	sizeof (TEXTUREVERTEX)	);
	m_D3DRender ->SetIndices	(	m_IB ->GetD3DIndexBuffer (),	0L	);

	for	(	index	=	0;	\
			index	<	MAX_COUNT_ITEM_PART;	\
			++index		)
	{
		if	(	in_char ->stat_itemID [index]	<	0	)	continue;

		pThisItem	=	&itemManager ->m_Items [in_char ->stat_itemID [index]];
		indices		=	pThisItem ->mesh.m_meshes [0].meshchunk ->iNumFace * 3;
		vertices	=	pThisItem ->mesh.m_meshes [0].meshchunk ->iNumVertexMap;

		itemManager ->GetTagMatrixUntransformed	(	in_char,
													index,
													boneManager ->GetIndex ( itemManager ->m_pTagManager ->GetParentName (index) ),
													unTransformed	);
		D3DXMatrixMultiply ( (D3DXMATRIX *)&transformed,	(D3DXMATRIX *)&unTransformed,	(D3DXMATRIX *)in_transform );

		m_D3DRender ->SetTransform ( D3DTS_WORLD,	&transformed );

		m_D3DRender ->RenderDefault	(	pThisItem ->iVertexStart,
										pThisItem ->iNumVertex,
										pThisItem ->iIndexStart,
										pThisItem ->iPrimitiveCount * 3	);
	}
	
}