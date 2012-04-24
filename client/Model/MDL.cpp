



#include "../global_def.h"





CMDL::CMDL()
{
	m_buffer		=	NULL;

	
	m_storedMeshes	=	NULL;
	m_iBufferSize	=	0;
	m_soundIdx		=	-1;
}

CMDL::~CMDL()
{
	GTH_SAFE_FREE ( m_buffer );

	
	GTH_SAFE_FREE ( m_storedMeshes );
}

void CMDL::Release()
{
	GTH_SAFE_FREE ( m_buffer );

	
	GTH_SAFE_FREE ( m_storedMeshes );
}

#define lump_data(x)    (m_buffer+head->lumps[(x)].offset)
#define lump_size(x)    (head->lumps[(x)].size)


bool CMDL::Create( char *fname, int iSize )
{
	mdlhead_t	*head;
	int			i;
	char		*ext;

	strcpy( m_name , fname );

	m_iBufferSize	=	iSize;
	m_buffer		=	m_fileMng->ReadFileFromPack( m_name );

	if( !m_buffer ) 
	{
		if( ext = CFileMng::GetFileExt( m_name ) ) strcpy( ext , "mdl" );
		else strcat( m_name , ".mdl" );

		m_buffer = m_fileMng->ReadFileFromPack( m_name );
		if( !m_buffer ) return false;
	}

	head				=	(mdlhead_t *) m_buffer;
	
	m_boneFrames		=	(mdlboneframe_t *) lump_data( GTH_MODEL_BONEFRAME );
	m_numBoneFrames		=	lump_size( GTH_MODEL_BONEFRAME ) / sizeof( mdlboneframe_t );

	m_tags				=	(mdltag3x3_t *) lump_data( GTH_MODEL_TAG );
	m_numTags			=	lump_size( GTH_MODEL_TAG ) / sizeof( mdltag3x3_t );

	m_meshes			=	(mdlmesh_t *) lump_data( GTH_MODEL_MESH );
	m_numMeshes			=	lump_size( GTH_MODEL_MESH ) / sizeof( mdlmesh_t );

	m_shaderRefs		=	(mdlshaderref_t *) lump_data( GTH_MODEL_SHADERREFS );
	m_numShaderRefs		=	lump_size( GTH_MODEL_SHADERREFS ) / sizeof( mdlshaderref_t );

	m_vertices			=	(mdlvertex_t *) lump_data( GTH_MODEL_VERTICES );
	m_numVertices		=	lump_size( GTH_MODEL_VERTICES ) / sizeof( mdlvertex_t );

	m_elems				=	(element_t *) lump_data( GTH_MODEL_ELEMENTS );
	m_numElems			=	lump_size( GTH_MODEL_ELEMENTS ) / sizeof( element_t );

	m_boundVertices		=	(mdlboundvertex_t *) lump_data( GTH_MODEL_BOUND_VERTICES );
	m_numBoundVertices	=	lump_size( GTH_MODEL_BOUND_VERTICES ) / sizeof( mdlboundvertex_t );

	m_boundElems		=	(element_t *) lump_data( GTH_MODEL_BOUND_ELEMENTS );
	m_numBoundElems		=	lump_size( GTH_MODEL_BOUND_ELEMENTS ) / sizeof( element_t );

	m_boundPlanes		=	(mdlboundplane_t *) lump_data( GTH_MODEL_BOUND_PLANES );
	m_numBoundPlanes	=	lump_size( GTH_MODEL_BOUND_PLANES ) / sizeof( mdlboundplane_t );

	m_boundMeshes		=	(mdlboundmesh_t *) lump_data( GTH_MODEL_BOUND_MESH );
	m_numBoundMeshes	=	lump_size( GTH_MODEL_BOUND_MESH ) / sizeof( mdlboundmesh_t );

	VectorCopy( m_mins , head->mins );
	VectorCopy( m_maxs , head->maxs );

	ClearBounds( m_mins , m_maxs );
	for( i = 0 ; i < m_numVertices ; i++ )
		AddPointToBounds( m_vertices[i].point , m_mins , m_maxs );
	
	m_numFrameVertices	=	0;
	m_numFrameElems		=	0;
	
	for( i = 0 ; i < m_numMeshes ; i++ )
	{
		m_numFrameVertices += m_meshes[i].numVertices;
		m_numFrameElems    += m_meshes[i].numElems;
	}

	for( i = 0 ; i < m_numShaderRefs ; i++ )
	{
		CFileMng::ConvertUnixPath( m_shaderRefs[i].name );
		if( ext = CFileMng::GetFileExt( m_shaderRefs[i].name ) ) *( ext - 1 ) = 0;
	}
 
	
	m_iStoredMeshesSize	=	m_numMeshes;

	


	return true;
}

bool CMDL::Recreate()
{
	mdlhead_t	*head;
	int			i;
	char		*ext;

	m_buffer		=	m_fileMng->ReadFileFromPack( m_name );

	if( !m_buffer ) 
	{
		if( ext = CFileMng::GetFileExt( m_name ) ) strcpy( ext , "mdl" );
		else strcat( m_name , ".mdl" );

		m_buffer = m_fileMng->ReadFileFromPack( m_name );
		if( !m_buffer ) return false;
	}

	head				=	(mdlhead_t *) m_buffer;
	
	m_boneFrames		=	(mdlboneframe_t *) lump_data( GTH_MODEL_BONEFRAME );
	m_numBoneFrames		=	lump_size( GTH_MODEL_BONEFRAME ) / sizeof( mdlboneframe_t );

	m_tags				=	(mdltag3x3_t *) lump_data( GTH_MODEL_TAG );
	m_numTags			=	lump_size( GTH_MODEL_TAG ) / sizeof( mdltag3x3_t );

	m_meshes			=	(mdlmesh_t *) lump_data( GTH_MODEL_MESH );
	m_numMeshes			=	lump_size( GTH_MODEL_MESH ) / sizeof( mdlmesh_t );

	m_shaderRefs		=	(mdlshaderref_t *) lump_data( GTH_MODEL_SHADERREFS );
	m_numShaderRefs		=	lump_size( GTH_MODEL_SHADERREFS ) / sizeof( mdlshaderref_t );

	m_vertices			=	(mdlvertex_t *) lump_data( GTH_MODEL_VERTICES );
	m_numVertices		=	lump_size( GTH_MODEL_VERTICES ) / sizeof( mdlvertex_t );

	m_elems				=	(element_t *) lump_data( GTH_MODEL_ELEMENTS );
	m_numElems			=	lump_size( GTH_MODEL_ELEMENTS ) / sizeof( element_t );

	m_boundVertices		=	(mdlboundvertex_t *) lump_data( GTH_MODEL_BOUND_VERTICES );
	m_numBoundVertices	=	lump_size( GTH_MODEL_BOUND_VERTICES ) / sizeof( mdlboundvertex_t );

	m_boundElems		=	(element_t *) lump_data( GTH_MODEL_BOUND_ELEMENTS );
	m_numBoundElems		=	lump_size( GTH_MODEL_BOUND_ELEMENTS ) / sizeof( element_t );

	m_boundPlanes		=	(mdlboundplane_t *) lump_data( GTH_MODEL_BOUND_PLANES );
	m_numBoundPlanes	=	lump_size( GTH_MODEL_BOUND_PLANES ) / sizeof( mdlboundplane_t );

	m_boundMeshes		=	(mdlboundmesh_t *) lump_data( GTH_MODEL_BOUND_MESH );
	m_numBoundMeshes	=	lump_size( GTH_MODEL_BOUND_MESH ) / sizeof( mdlboundmesh_t );

	VectorCopy( m_mins , head->mins );
	VectorCopy( m_maxs , head->maxs );

	ClearBounds( m_mins , m_maxs );
	for( i = 0 ; i < m_numVertices ; i++ )
		AddPointToBounds( m_vertices[i].point , m_mins , m_maxs );
	
	m_numFrameVertices	=	0;
	m_numFrameElems		=	0;
	
	for( i = 0 ; i < m_numMeshes ; i++ )
	{
		m_numFrameVertices += m_meshes[i].numVertices;
		m_numFrameElems    += m_meshes[i].numElems;
	}

	for( i = 0 ; i < m_numShaderRefs ; i++ )
	{
		CFileMng::ConvertUnixPath( m_shaderRefs[i].name );
		if( ext = CFileMng::GetFileExt( m_shaderRefs[i].name ) ) *( ext - 1 ) = 0;
	}

	return true;
}

int CMDL::Load( char *fname )
{
	
	
	char *ext;

	strcpy( m_name , fname );

	if( !g_fileMng.RegisterFileFromPack( m_name, (RHANDLE)this, this ) ) 
	{
		if( ext = CFileMng::GetFileExt( m_name ) )
			strcpy( ext , "mdl" );
		else
			strcat( m_name , ".mdl" );

		if( !g_fileMng.RegisterFileFromPack( m_name, (RHANDLE)this, this ) ) return false;
	}

	return true;
}

int CMDL::GetShaderRef( mdlshaderref_t **shaderRef )
{
	*shaderRef = m_shaderRefs;
	return m_numShaderRefs;
}

void CMDL::GenNormals()
{
	int sizeVertex;
	int sizeElem;
	int sizeFrame;
	mdlmesh_t *mesh;
	int i, j, k;
	vec3_t face[3];
	vec3_t normal;
	mdlvertex_t *vertices;
	element_t   *elems;

	for( i = 0 ; i < m_numMeshes ; i++ )
	{
		mesh = &m_meshes[ i ];
		sizeVertex = mesh->numVertices;
		sizeElem   = mesh->numElems;
		sizeFrame  = mesh->numMeshFrames;

		for( j = 0 ; j < sizeFrame ; j++ )
		{
			vertices = &m_vertices[ mesh->vertexOffset + sizeVertex * j ];
			for( k = 0 ; k < sizeVertex ; k++ )
			{
				VectorClear( vertices[ k ].normal );
			}
		}

		elems = &m_elems[ mesh->elemOffset ];
		for( j = 0 ; j < sizeFrame ; j++ )
		{
			vertices = &m_vertices[ mesh->vertexOffset + sizeVertex * j ];
			for( k = 0 ; k < sizeElem ; k += 3 )
			{
				VectorCopy( face[0] , vertices[ elems[ k ] ].point );
				VectorCopy( face[1] , vertices[ elems[ k + 1 ] ].point );
				VectorCopy( face[2] , vertices[ elems[ k + 2 ] ].point );
				
				MakeTriNormal( (float*) face , normal );
				
				VectorAdd( vertices[ elems[ k ] ].normal     , normal , vertices[ elems[ k ] ].normal );
				VectorAdd( vertices[ elems[ k + 1 ] ].normal , normal , vertices[ elems[ k + 1 ] ].normal );
				VectorAdd( vertices[ elems[ k + 2 ] ].normal , normal , vertices[ elems[ k + 2 ] ].normal );
			}

			
			for( k = 0 ; k < sizeVertex ; k++ )
				VectorScale( vertices[ k ].normal , vertices[ k ].normal , 0.5f );
				
		}
	}
}