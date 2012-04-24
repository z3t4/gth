


#include <stdio.h>
#include "bsp.h"
#include "gth_bsp.h"
#include "d3drender.h"
#include "d3dtexturemng.h"
#include "filemng.h"





CBSP::CBSP()
{
	m_subDivTolerance = 20;
	m_maxPatchLevel   = 4;

	m_format = GTH_BSP_FORMAT;
	m_shadowFormat = GTH_SHADOW_FORMAT;
	VectorClear( m_camPos );
	VectorClear( m_camDir );

	m_frameFlag = 0;
	m_shadowFrameFlag = 0;
	m_visibleDist = 30000.0f;
	m_removeDist  = 30000.0f;

	
	m_skyHeight = 8000.0f;
	m_visData = NULL;


	m_shaderMaps = NULL;

	m_numBank = 0;

	m_dynamicVB = NULL;
	m_dynamicIB = NULL;
	m_deformVB  = NULL;
	m_staticEB  = NULL;
	m_tempVB    = NULL;

	m_visibleQuality = GTH_WORLD_VISIBLE_QUALITY_HIGH;
}


CBSP::~CBSP()
{
	GTH_SAFE_DELETE( m_dynamicVB );
	GTH_SAFE_DELETE( m_dynamicIB );
	GTH_SAFE_DELETE( m_shadowVB );
	GTH_SAFE_DELETE( m_deformVB );
	GTH_SAFE_DELETE( m_staticEB );
	GTH_SAFE_DELETE( m_tempVB );

	for( int i = 0 ; i < m_numBank ; i++ )
		GTH_SAFE_DELETE( m_staticVB[i] );

	GTH_SAFE_DELETE( m_shaderMaps );
	GTH_SAFE_FREE( m_buffer );
}


void CBSP::Release()
{
	int i;

	m_dynamicVB->Release();
	m_dynamicIB->Release();
	m_shadowVB->Release();
	
	for( i = 0 ; i < m_numBank ; i++ )
		m_staticVB[i]->Release();

	for( i = m_shaderType->baseDefault ; i < m_shaderType->sizeDefault ; i++ )
		g_texMng.Release( m_shaderMaps[i].shader , OWNER_TYPE_BSP );
		
	for( i = m_shaderType->baseCustom ; i < m_numShaderRefs ; i++ )
		g_shaderMng.Release( m_shaderMaps[i].shader );
	
	for( i = 0 ; i < m_numLightmaps ; i++ )
		g_texMng.Release( m_lmapTexRefs[i] , OWNER_TYPE_BSP_LIGHTMAP );
}


#define lump_data(x)    (m_buffer+head->lumps[(x)].offset)
#define lump_size(x)    (head->lumps[(x)].size)

int CBSP::Load( char *mapName )
{
	bspheader_t *head;

	strcpy( m_name , mapName );

	m_buffer = g_fileMng.ReadFileFromPack( mapName );
	if( !m_buffer ) return false;

	head = (bspheader_t *) m_buffer;

	if( strcmp( head->signature , GTH_SIGNATURE ) )
	{
		GTH_SAFE_DELETE( m_buffer );
		return false;
	}
	if( head->version != GTH_BSPVERSION )
	{
		GTH_SAFE_DELETE( m_buffer );
		return false;
	}
	
	m_shaderType = &head->shaderTypeHead;

	m_shaderRefs = (shaderref_t *) lump_data( GTH_SHADERREFS );
	m_numShaderRefs = lump_size( GTH_SHADERREFS ) / sizeof( shaderref_t );

	m_planes = (plane_t *) lump_data( GTH_PLANES );
	m_numPlanes = lump_size( GTH_PLANES ) / sizeof( plane_t );

	m_nodes = (node_t *) lump_data( GTH_NODES );
	m_numNodes = lump_size( GTH_NODES ) / sizeof( node_t );

	m_leaves = (leaf_t *) lump_data( GTH_LEAFS );
	m_numLeaves = lump_size( GTH_LEAFS ) / sizeof( leaf_t );

	m_faceLists = (facelist_t *) lump_data( GTH_FACELISTS );
	m_numFaceLists = lump_size( GTH_FACELISTS ) / sizeof( facelist_t );

	m_brushLists = (brushlist_t *) lump_data( GTH_BRUSHELISTS );
	m_numBrushLists = lump_size( GTH_BRUSHELISTS ) / sizeof( brushlist_t );

	m_models = (model_t *) lump_data( GTH_MODELS );
	m_numModels = lump_size( GTH_MODELS ) / sizeof( model_t );

	m_brushes = (brush_t *) lump_data( GTH_BRUSHES );
	m_numBrushes = lump_size( GTH_BRUSHES ) / sizeof( brush_t );

	m_brushSides = (brushside_t *) lump_data( GTH_BRUSHSIDES );
	m_numBrushSides = lump_size( GTH_BRUSHSIDES ) / sizeof( brushside_t );

	m_vertices = (vertex_t *) lump_data( GTH_VERTICES );
	m_numVertices = lump_size( GTH_VERTICES ) / sizeof( vertex_t );

	m_elems = (element_t *) lump_data( GTH_ELEMENTS );
	m_numElems = lump_size( GTH_ELEMENTS ) / sizeof( element_t );

	m_faces = (face_t *) lump_data( GTH_FACES );
	m_numFaces = lump_size( GTH_FACES ) / sizeof( face_t );

	m_lightmaps = (lightmap_t *) lump_data( GTH_LIGHTMAPS );
	m_numLightmaps = lump_size( GTH_LIGHTMAPS ) / sizeof( lightmap_t );

	m_gridLights = (gridlight_t *) lump_data( GTH_GRIDLIGHTS );
	m_numGridLights = lump_size( GTH_GRIDLIGHTS ) / sizeof( gridlight_t );

	m_vis = (visibility_t *) lump_data( GTH_VISIBILITY );

	m_leafBaseFaces = (leafbaseface_t *) lump_data( GTH_LEAFBASEFACES );

	m_gridFaceLumpHead = (gridfacelumphead_t *) lump_data( GTH_GRIDFACELUMPHEAD );
	
	m_gridFaceLumps    = (gridfacelump_t *) lump_data( GTH_GRIDFACELUMP );
	m_numGridFaceLumps = lump_size( GTH_GRIDFACELUMP ) / sizeof( gridfacelump_t );

	m_gridFaceLists    = (gridfacelist_t *) lump_data( GTH_GRIDFACELISTS );
	m_numGridFaceLists = lump_size( GTH_GRIDFACELISTS ) / sizeof( gridfacelist_t );

	if( !m_shaderMaps )
	{
		m_shaderMaps = new shadermap_t[ m_numShaderRefs ];
		if( !m_shaderMaps )
		{
			GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::Load::m_shaderMaps" );
			GTH_SAFE_DELETE( m_buffer );
			return false;
		}
	}
	memset( m_shaderMaps , 0 , sizeof( shadermap_t ) * m_numShaderRefs );

	return true;
}

int CBSP::Initialize( float visibleDist )
{
	m_visibleDist = visibleDist;
	m_visData     = NULL;

	if(	!PrepareShaderMap() )
		return false;

	PrepareVertexBuffer();

	if(	!GenDynamicMem() )
		return false;

	return true;
}

int CBSP::Restore()
{
	if( !PrepareShaderMap() )
		return false;

	for( int i = 0 ; i < m_numBank ; i++ )
	{
		if( m_bankSize[i] <= 0 ) continue;


		m_staticVB[i]->Create(m_bankSize[ i ],
			                  m_format,
							  GTH_VERTEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY );
	}


	m_dynamicVB->Create(MAX_SIZE_DYNAMIC_VERTEX_BUFFER,
		                m_format,
						GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY);


	m_dynamicIB->Create(MAX_SIZE_DYNAMIC_INDEX_BUFFER,
		                GTH_FORMAT_INDEX16,
						GTH_INDEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );


	m_shadowVB->Create(MAX_SIZE_DYNAMIC_SHADOW_VERTEX_BUFFER,
		               m_shadowFormat,
					   GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY);

	return UploadVertexBuffer();
}

int CBSP::GenDynamicMem()
{
	int i;
	
	for( i = 0 ; i < m_numBank ; i++ )
	{
		if( m_bankSize[i] <= 0 ) continue;

		m_staticVB[i] = new CD3DBuffer();
		if( !m_staticVB[i] )
		{
			GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_staticVB" );
			return false;
		}
		m_staticVB[i]->Create(m_bankSize[i],
			                  m_format,
							  GTH_VERTEX_BUFFER | GTH_STATIC_BUFFER | GTH_WRITEONLY);
	}

	m_dynamicVB = new CD3DBuffer();
	if( !m_dynamicVB )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_dynamicVB" );
		return false;
	}
	m_dynamicVB->Create(MAX_SIZE_DYNAMIC_VERTEX_BUFFER,
		                m_format,
						GTH_VERTEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY);

	m_dynamicIB = new CD3DBuffer();
	if( !m_dynamicIB )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_dynamicIB" );
		return false;
	}
	m_dynamicIB->Create(MAX_SIZE_DYNAMIC_INDEX_BUFFER,
		                GTH_FORMAT_INDEX16,
						GTH_INDEX_BUFFER | GTH_DYNAMIC_BUFFER | GTH_WRITEONLY );

	m_shadowVB = new CD3DBuffer();
	if( !m_shadowVB )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_shadowVB" );
		return false;
	}
	m_shadowVB->Create(MAX_SIZE_DYNAMIC_SHADOW_VERTEX_BUFFER,
		               m_shadowFormat,
					   GTH_VERTEX_BUFFER|GTH_DYNAMIC_BUFFER|GTH_WRITEONLY);

	if( m_numDeformVerts > 0 )
	{
		m_deformVB = new vertex_t[ m_numDeformVerts ];
		if( !m_deformVB )
		{
			GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_deformVB" );
			return false;
		}
	}
	
	if( m_numStaticElems > 0 )
	{
		m_staticEB = new WORD[ m_numStaticElems ];
		if( !m_staticEB )
		{
			GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_staticEB" );
			return false;
		}
	}

	m_tempVB   = new vertex_t[ MAX_SIZE_TEMP_VERTEX_BUFFER ];
	if( !m_tempVB )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CBSP::GenDynamicMem::m_tempVB" );
		return false;
	}

	return true;
}


int CBSP::PrepareShaderMap()
{
	int i;
	int idxShader;
	char lightmap[128];
	char path[128];
	
	int len;

	shader_t *shader;
	for( i = m_shaderType->baseDefault ; i < m_shaderType->sizeDefault ; i++ )
	{
		m_shaderRefs[i].shader = -1;
		m_shaderMaps[i].shader = g_texMng.Add( m_shaderRefs[i].name , OWNER_TYPE_BSP );
		m_shaderMaps[i].flags = 0;
	}

	for( i = m_shaderType->baseCustom ; i < m_numShaderRefs ; i++ )
	{
		idxShader = g_shaderMng.SearchShader( m_shaderRefs[i].name );
		if( idxShader < 0 ) 
		{
			GTH_ERROR( GTH_ERROR_PRINT , "Shader not founded" );
			return false;
		}

		g_shaderMng.InitShader( idxShader );
		
		shader = g_shaderMng.GetShader( idxShader );

		m_shaderRefs[i].shader = idxShader;
		m_shaderMaps[i].shader = idxShader;
		m_shaderMaps[i].flags = shader->flags;
		shader->shaderRefIdx = i;
	}

	strcpy( path , m_name );
	len = strlen( path );
	for( i = len - 1 ; i >= 0 ; i-- )
	{
		if( path[i] == '.' )
			break;
	}

	if( i > 0 )
		path[i] = 0;

	for( i = 0 ; i < m_numLightmaps ; i++ )
	{
		sprintf( lightmap , "%s_lightmaps/LM%03d.dds" , path , i );
		m_lmapTexRefs[i] = g_texMng.Add( lightmap , OWNER_TYPE_BSP_LIGHTMAP );
		
		
		
		
		
	}

	return true;
}


void CBSP::PrepareVertexBuffer()
{
	shadermap_t *shaderMap;
	shaderLMmap_t *LMMap;
	face_t *face;
	int i;
	int currShader = -2;
	int currLM = -2;
	int prevNumStaticVerts = 0;
	int currNumStaticVerts = 0;
	int idxShader;
	int idxLM;
	int checkedSky;
	int numVerts;
	int numElems;

	m_numDeformVerts = 0;
	m_numStaticElems = 0;
	m_numBank = 0;
	
	for( i = 0 ; i < m_numFaces ; i++ )
	{
		face = &m_faces[i];
		idxShader = face->shader;
		idxLM = face->lmap;
		if( currShader != idxShader )
		{
			shaderMap = &m_shaderMaps[ idxShader ];
			currShader = idxShader;
			currLM = -2;
			checkedSky = false;
		}

		if( currLM != idxLM )
		{
			if( currNumStaticVerts >= MAX_BANKSIZE_VERTEXBUFFER )
			{
				m_bankSize[ m_numBank ] = prevNumStaticVerts;
				currNumStaticVerts -= prevNumStaticVerts;
				m_numBank++;
				LMMap->bank = m_numBank;  
			}

			LMMap = &shaderMap->LMMaps[ idxLM ];
			LMMap->bank = m_numBank;
			prevNumStaticVerts = currNumStaticVerts;
			currLM = idxLM;
		}

		if( !( shaderMap->flags & SHADER_DEFORMVERTEXES ) 
			&& !( shaderMap->flags & SHADER_SKY ) )
		{
			if( face->type != FACETYPE_PATCH )
			{
				currNumStaticVerts += face->vertexSize;
				m_numStaticElems += face->elemSize;
			}
			else
			{
				if( !CalcPatchSize( face , &numVerts ,  &numElems ) )
					continue;

				currNumStaticVerts += numVerts;
				m_numStaticElems += numElems;
			}
		}
		else if( !( shaderMap->flags & SHADER_DEFORMVERTEXES ) && ( shaderMap->flags & SHADER_SKY ) ) 
		{
			if( !checkedSky )
			{
				CalcSkySize( shaderMap->flags , &numVerts ,  &numElems );
				
				currNumStaticVerts += numVerts;
				m_numStaticElems += numElems;
				checkedSky = true;
			}
		}
		else  
		{
			if( shaderMap->flags & SHADER_SKY )
				continue; 

			if( face->type != FACETYPE_PATCH )
			{
				m_numDeformVerts += face->vertexSize;
				m_numStaticElems += face->elemSize;
			}
			else
			{
				if( !CalcPatchSize( face , &numVerts ,  &numElems ) )
					continue;

				m_numDeformVerts += numVerts;
				m_numStaticElems += numElems;
			}
		}
	}

	m_bankSize[ m_numBank++ ] = currNumStaticVerts;
}

int CBSP::UploadVertexBuffer()
{
	shadermap_t* shaderMap;
	shaderLMmap_t* LMMap;
	face_t* face;
	face_t* skyFace;
	vertex_t* vert;
	element_t* elem;
	int idxShader;
	int idxLM;
	int currBank = 0;
	int currShader = -2;
	int currLM = -2;
	int currNumStaticVerts = 0;
	int currNumStaticElems = 0;
	int currNumDeformVerts = 0;
	int checkedSky;
	int elemSize;
	int i;
	int j;

	WORD* ptrEB;
	WORD* ptrStaticEB = m_staticEB;
	vertex_t* ptrStaticVB = (vertex_t *) m_staticVB[0]->Lock();
	vertex_t* ptrDeformVB  = m_deformVB;

	for( i = 0 ; i < m_numFaces ; i++ )
	{
		face = &m_faces[i];
		idxShader = face->shader;
		idxLM = face->lmap;
		if( currShader != idxShader )
		{
			shaderMap = &m_shaderMaps[ idxShader ];
			currShader = idxShader;
			currLM = -2;
			checkedSky = false;
			skyFace = NULL;
		}

		if( currLM != idxLM )
		{
			LMMap = &shaderMap->LMMaps[ idxLM ];
			if( currBank != LMMap->bank )
			{
				m_staticVB[ currBank ]->Unlock();
				ptrStaticVB  = (vertex_t *) m_staticVB[ LMMap->bank ]->Lock();
				currBank = LMMap->bank;
				currNumStaticVerts = 0;
			}

			LMMap->baseVB = currNumStaticVerts;
			currLM = idxLM;
		}

		if( !( shaderMap->flags & SHADER_DEFORMVERTEXES ) && !( shaderMap->flags & SHADER_SKY ) )
		{
			if( face->type != FACETYPE_PATCH )
			{
				face->offsetVB = currNumStaticVerts;
				face->offsetEB = currNumStaticElems;

				vert = &m_vertices[ face->vertexOffset ];
				memcpy( ptrStaticVB , vert , sizeof( vertex_t ) * face->vertexSize );

				elem = &m_elems[ face->elemOffset ];
				elemSize = face->elemSize;
				ptrEB = ptrStaticEB;
				for( j = 0 ; j < elemSize ; j++ , elem++ , ptrEB++ )
					*ptrEB = (WORD) ( face->offsetVB + *elem );

				face->sizeVB = face->vertexSize;
				face->sizeEB = face->elemSize;
			}
			else 
			{
				face->offsetVB = currNumStaticVerts;
				face->offsetEB = currNumStaticElems;

				GenPatch( face , face->offsetVB , ptrStaticVB , ptrStaticEB );
			}

			currNumStaticVerts += face->sizeVB;
			currNumStaticElems += face->sizeEB;
				
			ptrStaticVB += face->sizeVB;
			ptrStaticEB += face->sizeEB;

			LMMap->sizeVB += face->sizeVB;
		}
		else if( !( shaderMap->flags & SHADER_DEFORMVERTEXES ) && ( shaderMap->flags & SHADER_SKY ) )
		{
			if( !checkedSky )
			{
				face->offsetVB = currNumStaticVerts;
				face->offsetEB = currNumStaticElems;

				GenSky( face , face->offsetVB , shaderMap->flags , ptrStaticVB , ptrStaticEB );

				currNumStaticVerts += face->sizeVB;
				currNumStaticElems += face->sizeEB;

				ptrStaticVB += face->sizeVB;
				ptrStaticEB += face->sizeEB;
				
				LMMap->sizeVB += face->sizeVB;

				skyFace = face;
				checkedSky = true;
			}
			else
			{
				face->offsetVB = skyFace->offsetVB;
				face->offsetEB = skyFace->offsetEB;

				face->sizeVB = skyFace->sizeVB;
				face->sizeEB = skyFace->sizeEB;
			}
		}
		else  
		{
			if( shaderMap->flags & SHADER_SKY )
				continue; 

			if( face->type != FACETYPE_PATCH )
			{
				face->offsetVB = currNumDeformVerts;
				face->offsetEB = currNumStaticElems;

				vert = &m_vertices[ face->vertexOffset ];
				memcpy( ptrDeformVB , vert , sizeof( vertex_t ) * face->vertexSize );

				elem = &m_elems[ face->elemOffset ];
				elemSize = face->elemSize;
				ptrEB = ptrStaticEB;
				for( j = 0 ; j < elemSize ; j++ , elem++ , ptrEB++ )
					*ptrEB = (WORD) *elem;

				face->sizeVB = face->vertexSize;
				face->sizeEB = face->elemSize;

			}
			else
			{
				face->offsetVB = currNumDeformVerts;
				face->offsetEB = currNumStaticElems;

				GenPatch( face , 0 , ptrDeformVB , ptrStaticEB );
			}

			currNumDeformVerts += face->sizeVB;
			currNumStaticElems += face->sizeEB;
				
			ptrDeformVB += face->sizeVB;
			ptrStaticEB += face->sizeEB;

			LMMap->sizeVB += face->sizeVB;
		}

	}

	m_staticVB[ currBank ]->Unlock();

	return true;
}


