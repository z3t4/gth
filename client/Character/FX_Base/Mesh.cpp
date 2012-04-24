 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

int		g_allocatedMemory;

 
 
 
FX_CMesh::~FX_CMesh()
{
}

 
 
 
VOID	FX_CMesh::Initialize	(	LPDIRECT3DDEVICE8	lpD3DDevice		)
{
	 
	m_lpD3DDevice		=	lpD3DDevice;

	 
	memset ( &m_meshes,	0,	sizeof (m_meshes) );

	m_dwNumNormalVertices	=	0;

	return;
}


 
 
 
VOID	FX_CMesh::Clear	()
{
	int		index,	subindex;
	MESH_t	*thisMesh	=	&m_meshes [0];
	for	(	index	=	0;	\
			index	<	m_iNumLODMesh + 1;	\
			++index,	++thisMesh	)
	{
		for	(	subindex	=	0;	\
				subindex	<	thisMesh ->numVertices;	\
				++subindex		)
		{
			GTH_SAFE_DELETE ( thisMesh ->influences [subindex].boneInfluences );
		}

		GTH_SAFE_DELETE_ARRAY ( thisMesh ->influences );
		GTH_SAFE_DELETE_ARRAY ( thisMesh ->meshchunk ->faceLists );
		GTH_SAFE_DELETE_ARRAY ( thisMesh ->meshchunk ->vertexMaps );
		GTH_SAFE_DELETE_ARRAY ( thisMesh ->meshchunk );
	}

	return;
}

 
 
 
bool	FX_CMesh::LoadData	(	TCHAR		*strFilename,
								CFileMng	*fileMng	)
{
	strcpy	(	m_strFileName, strFilename	);

	 
	if	(	!m_strFileName	)
	{
		 
		return	FALSE;
	}
	int		fileSize;
	BYTE	*pFileBuffer;
	int		offset	=	0;
	pFileBuffer	=	fileMng->ReadFileFromPack ( strFilename );
	fileSize	=	(int)fileMng ->GetFileSizeFromPack ( strFilename );

	g_allocatedMemory	=	0;

#define	__CALC_ALLOCATED_MEMORY(a)	if( g_resCounter->Enabled() ) { g_allocatedMemory += (a); }
	

	char	version [MAX_PATH];
	GetDataFromBuffer ( (void *)version, pFileBuffer, strlen("VERSION"), offset );
	version [strlen("VERSION")]	=	NULL;
	if	(	strcmp ( version, "VERSION" )	)
	{
		m_fileVersion	=	100;
		offset	=	4;
	}
	else
	{
		GetDataFromBuffer ( (void *)version, pFileBuffer, 3, offset );
		version [3]	=	NULL;
		m_fileVersion	=	atoi ( version );
		if	(	m_fileVersion	<	100	)
		{
			 

			m_fileVersion	=	100;
		}
		GetDataFromBuffer ( (void *)version, pFileBuffer, 4, offset );
		version [4]	=	NULL;
		if	(	strcmp ( version, "MESH" )	)
		{
			 
		}
	}

	 
	int		iDescLength;
	GetDataFromBuffer	(	(void *)&iDescLength,
							pFileBuffer,
							sizeof(int),
							offset		);
	GetDataFromBuffer	(	(void *)m_strDesc,
							pFileBuffer,
							iDescLength,
							offset		);
	m_strDesc [iDescLength]	=	'\0';

	 
	GetDataFromBuffer	(	(void *)&m_pMeshInfo,
							pFileBuffer,
							sizeof(MESHINFO_t),
							offset		);

	 
	GetDataFromBuffer	(	(void *)&m_pBoneStID,
							pFileBuffer,
							sizeof(FX_BONESTRUCT_ID),
							offset		);

	 
	INT			iMeshNameCount;
	GetDataFromBuffer	(	(void *)&iMeshNameCount,
							pFileBuffer,
							sizeof(int),
							offset		);
	GetDataFromBuffer	(	(void *)m_strMeshName,
							pFileBuffer,
							iMeshNameCount,
							offset		);
	m_strMeshName	[iMeshNameCount]	=	'\0';

	 
	GetDataFromBuffer	(	(void *)&m_meshes [0].numVertices,
							pFileBuffer,
							sizeof(int),
							offset		);

	m_meshes [0].influences	=	new INFLUENCE	[m_meshes [0].numVertices];
	__CALC_ALLOCATED_MEMORY( sizeof(INFLUENCE) * m_meshes[ 0 ].numVertices - sizeof(INFLUENCE*) )

	 
	UINT		iTemp;
	GetDataFromBuffer	(	(void *)&iTemp,
							pFileBuffer,
							sizeof(int),
							offset		);
	
				 
	UINT		iNumMesh;
	GetDataFromBuffer	(	(void *)&iNumMesh,
							pFileBuffer,
							sizeof(UINT),
							offset		);
	m_meshes [0].meshchunk	=	new	MESHCHUNK;
	__CALC_ALLOCATED_MEMORY( sizeof(MESHCHUNK) - sizeof(MESHCHUNK*) )

	 
	for	(	int	iInfluenceID	=	0;
				iInfluenceID	<	m_meshes [0].numVertices;
				++iInfluenceID
		)	 
	{
		INFLUENCE*		pInfluenceInst;
		pInfluenceInst	=	&m_meshes [0].influences [iInfluenceID];

		 
		GetDataFromBuffer	(	(void *)&pInfluenceInst->iNumBoneInfluences,
								pFileBuffer,
								sizeof(UINT),
								offset		);

		if	(	pInfluenceInst->iNumBoneInfluences	>	0	)
		{
			pInfluenceInst->boneInfluences	=	new BONEINFLUENCE	[pInfluenceInst->iNumBoneInfluences];
			 
			__CALC_ALLOCATED_MEMORY( sizeof(BONEINFLUENCE) * pInfluenceInst->iNumBoneInfluences - sizeof(BONEINFLUENCE*) )
		}
		else
			pInfluenceInst->boneInfluences	=	NULL;

			 
		GetDataFromBuffer	(	(void *)&pInfluenceInst->boneInfluences	[0],
								pFileBuffer,
								sizeof (BONEINFLUENCE)	*	pInfluenceInst->iNumBoneInfluences,
								offset		);
	}	 

	MESHCHUNK*	pBipedMeshInst;
	pBipedMeshInst	=	m_meshes [0].meshchunk;

	 
	GetDataFromBuffer	(	(void *)&pBipedMeshInst->material,
							pFileBuffer,
							sizeof(MATERIAL),
							offset		);
	
	 
	GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumImageMap,
							pFileBuffer,
							sizeof(UINT),
							offset		);
		 
	GetDataFromBuffer	(	(void *)&pBipedMeshInst->iImageMaps	[0],
							pFileBuffer,
							sizeof	(int)	*	pBipedMeshInst->iNumImageMap,
							offset		);

	 
	GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumFace,
							pFileBuffer,
							sizeof(UINT),
							offset		);
	pBipedMeshInst->faceLists	=	new	FACEINDEX	[pBipedMeshInst->iNumFace];
	 
	__CALC_ALLOCATED_MEMORY( sizeof(FACEINDEX) * pBipedMeshInst->iNumFace - sizeof(FACEINDEX*) )

	GetDataFromBuffer	(	(void *)&pBipedMeshInst->faceLists [0],
							pFileBuffer,
							sizeof (FACEINDEX)	*	pBipedMeshInst->iNumFace,
							offset		);

	 
	GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumVertexMap,
							pFileBuffer,
							sizeof(UINT),
							offset		);
	pBipedMeshInst->vertexMaps	=	new VERTEXMAP	[pBipedMeshInst->iNumVertexMap];
	__CALC_ALLOCATED_MEMORY( sizeof(VERTEXMAP) * pBipedMeshInst->iNumVertexMap - sizeof(VERTEXMAP*) )

	 
	GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumVertexColor,
							pFileBuffer,
							sizeof(UINT),
							offset		);

	 
	for	(	int	iVertexID	=	0;
				iVertexID	<	pBipedMeshInst->iNumVertexMap;
				++iVertexID
		)
	{
		VERTEXMAP*			pVertexMapInst;
		pVertexMapInst	=	&pBipedMeshInst->vertexMaps	[iVertexID];

		GetDataFromBuffer	(	(void *)&pVertexMapInst->iVertexID,
								pFileBuffer,
								sizeof(int),
								offset		);

		if	(	pBipedMeshInst->iNumVertexColor	>	0	)
		{
			GetDataFromBuffer	(	(void *)&pVertexMapInst->clrVertexColor,
									pFileBuffer,
									sizeof(D3DXCOLOR),
									offset		);
		}	 

		GetDataFromBuffer	(	(void *)&pVertexMapInst->VertexTC [0],
								pFileBuffer,
								sizeof	(TEXCOORD)	*	pBipedMeshInst->iNumImageMap,
								offset		);
	}	 

		 
	GetDataFromBuffer	(	(void *)&m_meshes [0].meshchunk ->bound_min,
							pFileBuffer,
							sizeof (vec3_t),
							offset		);
	GetDataFromBuffer	(	(void *)&m_meshes [0].meshchunk ->bound_max,
							pFileBuffer,
							sizeof (vec3_t),
							offset		);

	if	(	fileSize	<=	offset	)
	{
		m_iNumLODMesh	=	0;
	}
	else
	{
		GetDataFromBuffer	(	(void *)&m_iNumLODMesh,
								pFileBuffer,
								sizeof (int),
								offset		);

		LoadLODMeshes (	pFileBuffer,	offset );
	}

	GTH_SAFE_FREE ( pFileBuffer );

	 
	__CALC_ALLOCATED_MEMORY( sizeof(FX_CMesh) - sizeof(FX_CMesh*) )

	 
	if	(	g_resCounter->Enabled()	)
	{
		char	*lowercase	=	_strlwr( _strdup( strFilename ) );

		if	(	strstr( lowercase, "mon" )	)
		{
			g_resCounter->Add_DataType( RESCOUNTER_MESH_CHAR_MONSTER, g_allocatedMemory );
		}
		else if	(	strstr( lowercase, "ava" ) )
		{
			g_resCounter->Add_DataType( RESCOUNTER_MESH_CHAR_PC, g_allocatedMemory );
		}
		else if (	strstr( lowercase, "npc" )	)
		{
			g_resCounter->Add_DataType( RESCOUNTER_MESH_CHAR_NPC, g_allocatedMemory );
		}
		free( lowercase );
	}

	return	TRUE;
}

 
 
 
 
void	FX_CMesh::LoadLODMeshes	(	BYTE	*in_buffer,	
									int		offset	)
{
	int		index;

	for	(	index	=	1;	\
			index	<	( 1 + m_iNumLODMesh );	\
			++index		)
	{
		 
		GetDataFromBuffer	(	(void *)&m_meshes [index].numVertices,
								in_buffer,
								sizeof(int),
								offset		);

		 
		GetDataFromBuffer	(	(void *)&m_meshes [index].numVertices,
								in_buffer,
								sizeof(int) * 2,
								offset		);

		 
		for	(	int	iInfluenceID	=	0;
					iInfluenceID	<	m_meshes [index].numVertices;
					++iInfluenceID
			)	 
		{
			INFLUENCE	*pInfluenceInst;
			pInfluenceInst	=	&m_meshes [index].influences [iInfluenceID];

			 
			GetDataFromBuffer	(	(void *)&pInfluenceInst->iNumBoneInfluences,
									in_buffer,
									sizeof(UINT),
									offset		);

			if	(	pInfluenceInst->iNumBoneInfluences	>	0	)
			{
				pInfluenceInst->boneInfluences	=	new BONEINFLUENCE	[pInfluenceInst->iNumBoneInfluences];
				 
				__CALC_ALLOCATED_MEMORY( sizeof(BONEINFLUENCE) * pInfluenceInst->iNumBoneInfluences - sizeof(BONEINFLUENCE*) )
			}
			else
				pInfluenceInst->boneInfluences	=	NULL;

				 
			GetDataFromBuffer	(	(void *)&pInfluenceInst->boneInfluences	[0],
									in_buffer,
									sizeof (BONEINFLUENCE)	*	pInfluenceInst->iNumBoneInfluences,
									offset		);
		}	 

		MESHCHUNK*	pBipedMeshInst;
		pBipedMeshInst	=	m_meshes [index].meshchunk;

		 
		GetDataFromBuffer	(	(void *)&pBipedMeshInst->material,
								in_buffer,
								sizeof(MATERIAL),
								offset		);
		
		 
		GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumImageMap,
								in_buffer,
								sizeof(UINT),
								offset		);
			 
		GetDataFromBuffer	(	(void *)&pBipedMeshInst->iImageMaps	[0],
								in_buffer,
								sizeof	(int)	*	pBipedMeshInst->iNumImageMap,
								offset		);

		 
		GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumFace,
								in_buffer,
								sizeof(UINT),
								offset		);
		pBipedMeshInst->faceLists	=	new	FACEINDEX	[pBipedMeshInst->iNumFace];
		 
		__CALC_ALLOCATED_MEMORY( sizeof(FACEINDEX) * pBipedMeshInst->iNumFace - sizeof(FACEINDEX*) )

		GetDataFromBuffer	(	(void *)&pBipedMeshInst->faceLists [0],
								in_buffer,
								sizeof (FACEINDEX)	*	pBipedMeshInst->iNumFace,
								offset		);

		 
		GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumVertexMap,
								in_buffer,
								sizeof(UINT),
								offset		);
		pBipedMeshInst->vertexMaps	=	new VERTEXMAP	[pBipedMeshInst->iNumVertexMap];
		 
		__CALC_ALLOCATED_MEMORY( sizeof(VERTEXMAP) * pBipedMeshInst->iNumVertexMap - sizeof(VERTEXMAP*) )

		 
		GetDataFromBuffer	(	(void *)&pBipedMeshInst->iNumVertexColor,
								in_buffer,
								sizeof(UINT),
								offset		);

		 
		for	(	int	iVertexID	=	0;
					iVertexID	<	pBipedMeshInst->iNumVertexMap;
					++iVertexID
			)
		{
			VERTEXMAP*			pVertexMapInst;
			pVertexMapInst	=	&pBipedMeshInst->vertexMaps	[iVertexID];

			GetDataFromBuffer	(	(void *)&pVertexMapInst->iVertexID,
									in_buffer,
									sizeof(int),
									offset		);

			if	(	pBipedMeshInst->iNumVertexColor	>	0	)
			{
				GetDataFromBuffer	(	(void *)&pVertexMapInst->clrVertexColor,
										in_buffer,
										sizeof(D3DXCOLOR),
										offset		);
			}	 

			GetDataFromBuffer	(	(void *)&pVertexMapInst->VertexTC [0],
									in_buffer,
									sizeof	(TEXCOORD)	*	pBipedMeshInst->iNumImageMap,
									offset		);
		}	 

			 
		GetDataFromBuffer	(	(void *)&m_meshes [index].meshchunk ->bound_min,
								in_buffer,
								sizeof (vec3_t),
								offset		);
		GetDataFromBuffer	(	(void *)&m_meshes [index].meshchunk ->bound_max,
								in_buffer,
								sizeof (vec3_t),
								offset		);
	}

	return;
}

 
 
 
HRESULT		FX_CMesh::RestoreDeviceObjects	()
{
	return	S_OK;
}

 
 
 
void	FX_CMesh::Cleanup()
{
	int		index,	subindex;
	MESH_t	*thisMesh	=	&m_meshes [0];
	for	(	index	=	0;	\
			index	<	m_iNumLODMesh + 1;	\
			++index,	++thisMesh	)
	{
		for	(	subindex	=	0;	\
				subindex	<	thisMesh ->numVertices;	\
				++subindex		)
		{
			GTH_SAFE_DELETE ( thisMesh ->influences [subindex].boneInfluences );
		}

		GTH_SAFE_DELETE_ARRAY ( thisMesh ->influences );
		GTH_SAFE_DELETE_ARRAY ( thisMesh ->meshchunk ->faceLists );
		GTH_SAFE_DELETE_ARRAY ( thisMesh ->meshchunk ->vertexMaps );
		GTH_SAFE_DELETE_ARRAY ( thisMesh ->meshchunk );
	}

	return;
}