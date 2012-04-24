



#include "shadermng.h"
#include "camera.h"
#include "script.h"
#include "timer.h"
#include "d3dtexturemng.h"

CShaderMng g_shaderMng;





CShaderMng::CShaderMng()
{


	m_numShaders = 0;

	m_numAnimMaps = 0;
	m_numEnvSkys  = 0;
}

CShaderMng::~CShaderMng()
{
	m_numShaders = 0;

	m_numAnimMaps = 0;
	m_numEnvSkys  = 0;

	delete m_shaders;
}

CShaderMng::CShaderMng( int owner )
{


	m_numShaders = 0;

	m_numAnimMaps = 0;
	m_numEnvSkys  = 0;

	m_owner = owner;
}

int CShaderMng::Init( int sizeShader )
{
	if( sizeShader == 0 ) return true;

	m_maxSizeShader = sizeShader;
	
	m_shaders  = new shader_t[ m_maxSizeShader ];
	if( !m_shaders )
	{
		GTH_ERROR( GTH_ERROR_NOT_ENOUGH_MEMORY , "CShaderMng::Init::m_shaders" );
		return false;
	}
	m_numShaders  = 0;

	return true;
}

int CShaderMng::GetShaderCount( char *fName , DWORD size , BYTE *data )
{
	char *token;
	int count;

	if( data )
	{
		if( !g_script.Load( fName , size , data ) )
			return 0;
	}
	else
	{
		if( !g_script.Load( fName ) )
			return 0;
	}

	g_script.BeginParse();

	count = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;
		
		count++;
		g_script.SkipBracedSection();
	}

	return count;
}

void CShaderMng::Release()
{
	shader_t *shader;
	shaderpass_t *pass;
	int i , j;

	

	for( i = 0 ; i < m_numShaders ; i++ )
	{
		shader = &m_shaders[ i ];

		if( !shader->inited ) continue;

		if( shader->flags & SHADER_SKY_ENVIRON )
		{
			for( i = 0 ; i < SIZE_ENVIRONMENT_SIDE ; i++ )
				g_texMng.Release( shader->envSky->map[ i ] , m_owner );
		}

		pass = &shader->passes[0];
		for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
		{
			if( pass->flags & SHADERPASS_MAP )
				g_texMng.Release( pass->map , m_owner );
			else if( pass->flags & SHADERPASS_MAP_ANIMMAP )
			{
				anim_t *anim = pass->animMap;
				for( j = 0 ; j < anim->numFrames ; j++ )
					g_texMng.Release( anim->maps[ j ] , m_owner );
			}
		}
		shader->inited = false;
	}
}

void CShaderMng::Release( int idxShader )
{
	shader_t*     shader;
	shaderpass_t* pass;
	int i;
	int j;

	
		

	shader = &m_shaders[idxShader];
	if(!shader->inited)
		return;

	if(shader->flags & SHADER_SKY_ENVIRON)
	{
		for(i = 0 ; i < SIZE_ENVIRONMENT_SIDE ; i++)
		{
			g_texMng.Release(shader->envSky->map[i], m_owner);
		}
	}

	pass = &shader->passes[0];
	for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		if(pass->flags & SHADERPASS_MAP)
		{
			g_texMng.Release( pass->map , m_owner );
		}
		else if(pass->flags & SHADERPASS_MAP_ANIMMAP)
		{
			anim_t *anim = pass->animMap;
			for(j = 0 ; j < anim->numFrames ; j++)
			{
				g_texMng.Release( anim->maps[ j ] , m_owner );
			}
		}
	}
	shader->inited = false;
}

int CShaderMng::GetShaderTexRef( char *textureName )
{
	return g_texMng.Add( textureName , m_owner );
}

void CShaderMng::InitShader( int idxShader )
{
	shader_t*     shader;
	shaderpass_t* pass;
	int i;
	int j;

	shader = &m_shaders[idxShader];
	if( shader->inited ) return;

	if( shader->flags & SHADER_SKY_ENVIRON )
	{
		for(i = 0 ; i < SIZE_ENVIRONMENT_SIDE ; i++)
		{
			shader->envSky->texRef[i] = GetShaderTexRef( shader->envSky->map[ i ] );
		}
	}

	pass = &shader->passes[0];
	for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		if( pass->flags & SHADERPASS_MAP )
		{
			pass->texRef = GetShaderTexRef( pass->map );
		}
		else if( pass->flags & SHADERPASS_MAP_ANIMMAP )
		{
			anim_t *anim = pass->animMap;
			for( j = 0 ; j < anim->numFrames ; j++ )
			{
				anim->frames[ j ] = GetShaderTexRef( anim->maps[ j ] );
			}
		}
	}

	shader->inited = true;
}

int CShaderMng::SearchShader( char *name )
{
	for(int i = 0 ; i < m_numShaders ; i++)
	{
		if(!stricmp( name , m_shaders[i].name))
		{
			return i;
		}
	}

	return -1;
}


shader_t *CShaderMng::GetShader( int idx )
{
	shader_t* shader;
	
	shader = &m_shaders[ idx ];
	if( shader->inited )
		return shader;
	else
		return NULL;
}

shader_t *CShaderMng::GetShader( char *name )
{
	shader_t* shader;
	int i;

	for( i = 0 ; i < m_numShaders ; i++ )
	{
		if( !stricmp( name , m_shaders[i].name ) )
		{
			break;
		}
	}
	if( i == m_numShaders )
	{
		return NULL;
	}

	shader = &m_shaders[ i ];
	if( shader->inited )
		return shader;
	else
		return NULL;
}

shader_t *CShaderMng::GetNewShader( char *name )
{
	shader_t *shader;
	int i;

	for( i = 0 ; i < m_numShaders ; i++ )
	{
		if( !stricmp( name , m_shaders[i].name ) )
			return NULL;
	}
	
	shader = &m_shaders[ m_numShaders ];
	shader->flags = 0;
	shader->numPasses = 0;
	shader->inited = false;
	shader->frameFlag = 0;
	strcpy( shader->name , name );

	m_numShaders++;

	return shader;
}

int CShaderMng::MakeDefaultShader( char *name )
{
	shader_t *shader;
	
	shader = &m_shaders[m_numShaders++];
	strcpy( shader->name , name );
	shader->flags = 0;
	shader->numPasses = 2;
	strcpy( shader->passes[0].map , name );
	shader->passes[0].texRef     = GetShaderTexRef( name );
	shader->passes[0].flags      = SHADERPASS_DEPTHWRITE | SHADERPASS_MAP;
	shader->passes[0].depthFunc  = D3DCMP_LESSEQUAL;

	shader->passes[1].flags      = SHADERPASS_MAP_LIGHTMAP | SHADERPASS_BLEND | SHADERPASS_NONDEPTHWRITE;
	shader->passes[1].blend.src  = D3DBLEND_DESTCOLOR;
	shader->passes[1].blend.dest = D3DBLEND_ZERO;

	shader->inited = true;

	return m_numShaders - 1;
}


int CShaderMng::ReadShaderScript( char *fName , DWORD size , BYTE *data )
{
	char *token;
	shader_t *shader;

	if( data )
	{
		if( !g_script.Load( fName , size , data ) )
			return false;
	}
	else
	{
		if( !g_script.Load( fName ) )
			return false;
	}

	g_script.BeginParse();

	while(1)
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;
		
		shader = GetNewShader( token );
		if( !shader )
		{
			g_script.SkipBracedSection();
			continue;
		}

		token = g_script.GetNextToken( true );
		if( token[0] != '{' )
			return false;

		while( 1 )
		{
			token = g_script.GetNextToken( true );
			if( !token[0] ) break;

			if (token[0] == '{') 
			{
				ParseShaderPass( shader );
			}
	        else if (token[0] == '}')    
			    break;
			else
			    ParseShaderToken( shader , token );
		}
		
		

		if( shader->flags & SHADER_SURF_TRANS )
			shader->flags |= SHADER_CULL_NONE;
	}

	return true;
}


void CShaderMng::ParseShaderPass( shader_t *shader )
{
	shaderpass_t *pass;
	char *token;

	pass = &shader->passes[ shader->numPasses ];
	pass->flags = 0;
    pass->texRef = -1;
    pass->depthFunc = D3DCMP_LESSEQUAL;
    pass->rgbGen.type = SHADERPASS_RGBGEN_IDENTITY;
    pass->tcMod.numTcModFuncs = 0;

	while(1)
	{
		token = g_script.GetNextToken( true );
		if( !token[0] ) break;

		if (token[0] == '}') 
		{
			break;
		}
		else
		{
			ParseShaderPassToken( shader, pass, token );
		}
	}

	if( ( pass->flags & SHADERPASS_NONDEPTHWRITE ) && ( pass->flags & SHADERPASS_DEPTHWRITE ) )
	{
		pass->flags &= ~SHADERPASS_NONDEPTHWRITE;
	}
	if( ( pass->flags & SHADERPASS_TCGEN ) )
	{
		shader->flags |= SHADER_TCGEN;
	}

	shader->numPasses++;
}




wavetype_t CShaderMng::ParseWaveFunc( char *param )
{
	if( stricmp( param , "sin" ) )              return WAVE_SIN;
	if( stricmp( param , "triangle" ) )	        return WAVE_TRIANGLE;
	if( stricmp( param , "square" )	)           return WAVE_SQUARE;
	if( stricmp( param , "sawtooth" ) )         return WAVE_SAWTOOTH;
	if( stricmp( param , "inversesawtooth" ) )  return WAVE_INVSAWTOOTH;
	if( stricmp( param , "noise" ) )            return WAVE_NOISE;

    return WAVE_NONE;
}

void CShaderMng::ShaderKeyFunc_skyparms( shader_t *shader )
{
	char envTex[ GTH_MAX_SIZE_FILENAME ];
	
	char *parameter;
	envsky_t *envSky;

	parameter = g_script.GetNextToken(false);
	if( !parameter[0] )
	{
		return;
	}

	if( parameter[0] != '-' )
	{
		shader->flags |= SHADER_SKY_ENVIRON;
	
		envSky = &m_envSkys[ m_numEnvSkys++ ];

		strcpy( envTex , parameter );
		strcat( envTex , "_up" );
		strcpy( envSky->map[0] , envTex );

		strcpy( envTex , parameter );
		strcat( envTex , "_ft" );
		strcpy( envSky->map[1] , envTex );

		strcpy( envTex , parameter );
		strcat( envTex , "_rt" );
		strcpy( envSky->map[2] , envTex );

		strcpy( envTex , parameter );
		strcat( envTex , "_bk" );
		strcpy( envSky->map[3] , envTex );

		strcpy( envTex , parameter );
		strcat( envTex , "_lf" );
		strcpy( envSky->map[4] , envTex );

		strcpy( envTex , parameter );
		strcat( envTex , "_dn" );
		strcpy( envSky->map[5] , envTex );

		shader->envSky = envSky;
	}

	parameter = g_script.GetNextToken(false);
	if( !parameter[0] )
	{
		return;
	}

	if( parameter[0] != '-' )
	{
		shader->skyheight = (float) atof( parameter );
	}
	else
	{
		shader->skyheight = 0.0f;
	}

	g_script.SkipLine();
}

void CShaderMng::ShaderKeyFunc_cull( shader_t *shader )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if( !stricmp( parameter , "none" ) || !stricmp( parameter , "disable" ) || !stricmp( parameter , "twosided" ) )
	{
		shader->flags |= SHADER_CULL;
		shader->flags |= SHADER_CULL_NONE;
	}
	else if( !stricmp( parameter , "back" ) || !stricmp( parameter , "backsided" ) )
	{
		shader->flags |= SHADER_CULL;
		shader->flags |= SHADER_CULL_BACK;
	}
	else if( !stricmp( parameter , "front" ) )
	{
		shader->flags |= SHADER_CULL;
		shader->flags |= SHADER_CULL_FRONT;
	}

	g_script.SkipLine();
}

void CShaderMng::ShaderKeyFunc_deformvertexes( shader_t *shader )
{
	char parameter[10][32];
	char *token;

	int numParms = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	if(!stricmp( parameter[0] , "wave" ))
	{
		if( numParms < 7 )
			return;
		shader->deform.div = 1.0f / (float) atof( parameter[1] );
		shader->deform.func.type = ParseWaveFunc( parameter[2] );
		if( shader->deform.func.type == WAVE_NONE )
			return;

		shader->deform.func.base      = (float)atof( parameter[3] );
		shader->deform.func.amplitude = (float)atof( parameter[4] );
		shader->deform.func.phase     = (float)atof( parameter[5] );
		shader->deform.func.frequency = (float)atof( parameter[6] );
		shader->flags |= SHADER_DEFORMVERTEXES;
	}
	else if( !stricmp( parameter[0] , "normal" ) )
	{
		if( numParms < 6 )
			return;
		shader->deform.div = (float) atof( parameter[1] );
		shader->deform.func.type = ParseWaveFunc( parameter[2] );
		if( shader->deform.func.type == WAVE_NONE )
			return;

		shader->deform.func.base      = (float) atof( parameter[3] );
		shader->deform.func.amplitude = (float) atof( parameter[4] );
		shader->deform.func.phase     = 0.0f;
		shader->deform.func.frequency = (float) atof( parameter[5] );
		shader->flags |= SHADER_DEFORMVERTEXES;
	}
	else if( !stricmp( parameter[0] , "bulge" ) )
	{
		if(numParms < 4)
			return;
	}
	else if( !stricmp( parameter[0] , "move" ) )
	{
		if( numParms < 9 )
			return;
	}
	else if( !stricmp( parameter[0] , "autosprite" ) )
	{
		shader->flags |= SHADER_DEFORMVERT_AUTOSPRITE;
	}
	else if( !stricmp( parameter[0] , "autosprite2" ) )
	{
		shader->flags |= SHADER_DEFORMVERT_AUTOSPRITE2;
	}
}

void CShaderMng::ShaderKeyFunc_sort( shader_t *shader )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if     ( !stricmp( parameter , "portal" ) || !stricmp( parameter , "1" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_PORTAL;
	}
	else if( !stricmp( parameter , "sky" ) || !stricmp( parameter , "2" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_SKY;
	}
	else if( !stricmp( parameter , "opaque" ) || !stricmp( parameter , "3" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_OPAQUE;
	}
	else if( !stricmp( parameter , "banner" ) || !stricmp( parameter , "6" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_BANNER;
	}
	else if( !stricmp( parameter , "underwater" ) || !stricmp( parameter , "8" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_UNDERWATER;
	}
	else if( !stricmp( parameter , "additive" ) || !stricmp( parameter , "9" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_ADDITIVE;
	}
	else if( !stricmp( parameter , "nearest" ) || !stricmp( parameter , "16" ) )
	{
		shader->flags |= SHADER_SORT;
		shader->flags |= SHADER_SORT_NEAREST;
	}

	g_script.SkipLine();
}

void CShaderMng::ShaderKeyFunc_surfaceparm( shader_t *shader )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if     ( !stricmp( parameter , "trans" ) )
	{
		shader->flags |= SHADER_SURF;
		shader->flags |= SHADER_SURF_TRANS;
	}
	else if( !stricmp( parameter , "sky" ) )
	{
		shader->flags |= SHADER_SURF | SHADER_SKY;
		shader->flags |= SHADER_SURF_SKY;
	}
	else if( !stricmp( parameter , "nonsolid" ) )
	{
		shader->flags |= SHADER_SURF;
		shader->flags |= SHADER_SURF_TRANS;
	}
	else if( !stricmp( parameter , "water" ) )
	{
		shader->flags |= SHADER_SURF;
		shader->flags |= SHADER_SURF_TRANS;
	}
	else if( !stricmp( parameter , "nodraw" ) )
	{
		shader->flags |= SHADER_SURF;
		shader->flags |= SHADER_SURF_NODRAW;
	}
	else if( !stricmp( parameter , "nolightmap" ) )
	{
		shader->flags |= SHADER_SURF;
		shader->flags |= SHADER_SURF_NOLIGHTMAP;
	}
	
	g_script.SkipLine();
}

void CShaderMng::ParseShaderToken( shader_t *shader , char *token )
{
	if     ( !stricmp( token , "skyparms" ) )
	{
		ShaderKeyFunc_skyparms( shader );
	}
	else if( !stricmp( token , "cull" ) )
	{
		ShaderKeyFunc_cull( shader );
	}
	else if( !stricmp( token , "deformvertexes" ) )
	{
		ShaderKeyFunc_deformvertexes( shader );
	}
	else if( !stricmp( token , "fogparms" ) )
	{
	}
	else if( !stricmp( token , "nopicmip" ) )
	{
	}
	else if( !stricmp( token , "nomipmap" ) )
	{
		shader->flags |= SHADER_NOMIPMAPS;
	}
	else if( !stricmp( token , "polygonoffset" ) )
	{
	}
	else if( !stricmp( token , "portal" ) )
	{
	}
	else if( !stricmp( token , "sort" ) )
	{
		ShaderKeyFunc_sort( shader );
	}
	else if( !stricmp( token , "tesssize" ) )
	{
	}
	else if( !stricmp( token , "surfaceparm" ) )
	{
		ShaderKeyFunc_surfaceparm( shader );
	}
	else
		g_script.SkipLine();
}

void CShaderMng::ShaderPassKeyFunc_map( shader_t *shader , shaderpass_t *pass )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if     ( !stricmp( parameter , "$lightmap" ) )
		pass->flags |= SHADERPASS_MAP_LIGHTMAP;
	else if( !stricmp( parameter , "$whiteimage" ) )
		pass->flags |= SHADERPASS_MAP_WHITEIMAGE;
	else
	{
		pass->flags |= SHADERPASS_MAP;
		strcpy( pass->map , parameter );
	}
		
	g_script.SkipLine();
}

void CShaderMng::ShaderPassKeyFunc_clampmap( shader_t *shader , shaderpass_t *pass )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	pass->flags |= SHADERPASS_MAP | SHADERPASS_CLAMPMAP;
	strcpy( pass->map , parameter );
		
	g_script.SkipLine();
}

void CShaderMng::ShaderPassKeyFunc_animmap( shader_t *shader , shaderpass_t *pass )
{
	char parameter[20][ GTH_MAX_SIZE_FILENAME ];
	char *token;
	anim_t *anim;

	int numParms = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	pass->flags |= SHADERPASS_MAP_ANIMMAP;

	anim = &m_animMaps[ m_numAnimMaps++ ];
	anim->fps = (float) atof( parameter[0] );
    anim->numFrames = numParms - 1;
    for ( int i = 0 ; i < numParms - 1 ; ++i )
		strcpy( anim->maps[i] , parameter[ i + 1 ] );

	pass->animMap = anim;
}

void CShaderMng::ShaderPassKeyFunc_blendfunc( shader_t *shader , shaderpass_t *pass )
{
	char parameter[10][32];
	char *token;

	int numParms = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	if( numParms == 1 )
    {
	    if( !stricmp( parameter[0] , "blend" ) )
		{
	        pass->blend.src  = D3DBLEND_SRCALPHA;
	        pass->blend.dest = D3DBLEND_INVSRCALPHA;
		}
	    else if( !stricmp( parameter[0] , "filter" ) )
		{
	        pass->blend.src  = D3DBLEND_ZERO;
			pass->blend.dest = D3DBLEND_SRCCOLOR;
		}
	    else if( !stricmp(parameter[0] , "add" ) )
		{
	        pass->blend.src = pass->blend.dest = D3DBLEND_ONE;
		}
	    else
	        return;
    }
    else if( numParms == 2 )
    {
	    for( int i = 0 ; i < 2 ; ++i )
		{
	        int *blend = ( i == 0 ) ? &pass->blend.src : &pass->blend.dest;

	        if     ( !stricmp( parameter[i] , "gl_zero" ) )
		        *blend = D3DBLEND_ZERO;
	        else if( !stricmp( parameter[i] , "gl_one" ) )
		        *blend = D3DBLEND_ONE;
			else if( !stricmp( parameter[i] , "gl_src_color" ) )
		        *blend = D3DBLEND_SRCCOLOR;
			else if( !stricmp( parameter[i] , "gl_src_alpha" ) )
		        *blend = D3DBLEND_SRCALPHA;
	        else if( !stricmp( parameter[i] , "gl_dst_color" ) )
		        *blend = D3DBLEND_DESTCOLOR;
	        else if( !stricmp( parameter[i] , "gl_dst_alpha" ) )
		        *blend = D3DBLEND_DESTALPHA;
			else if( !stricmp( parameter[i] , "gl_one_minus_src_color" ) )
		    	*blend = D3DBLEND_INVSRCCOLOR;
			else if( !stricmp( parameter[i] , "gl_one_minus_src_alpha" ) )
		        *blend = D3DBLEND_INVSRCALPHA;
			else if( !stricmp( parameter[i] , "gl_one_minus_dst_color" ) )
		        *blend = D3DBLEND_INVDESTCOLOR;
	        else if( !stricmp( parameter[i] , "gl_one_minus_dst_alpha" ) )
		        *blend = D3DBLEND_INVDESTALPHA;
			else
				return;
		}
    }

	if( pass->blend.src == D3DBLEND_ONE && pass->blend.dest == D3DBLEND_ZERO )
			return;

	pass->flags |= SHADERPASS_BLEND | SHADERPASS_NONDEPTHWRITE;
}

void CShaderMng::ShaderPassKeyFunc_rgbgen( shader_t *shader , shaderpass_t *pass )
{
	char parameter[10][32];
	char *token;

	int numParms = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	if     ( !stricmp( parameter[0] , "wave" ) )
	{
		if( numParms < 6 ) return;
		pass->rgbGen.func.type = ParseWaveFunc( parameter[1] );
		if( pass->rgbGen.func.type == WAVE_NONE )	return;

		pass->rgbGen.func.base      = (float) atof( parameter[2] );
		pass->rgbGen.func.amplitude = (float) atof( parameter[3] );
		pass->rgbGen.func.phase     = (float) atof( parameter[4] );
		pass->rgbGen.func.frequency = (float) atof( parameter[5] );
		pass->rgbGen.type = SHADERPASS_RGBGEN_WAVE;

		pass->flags |= SHADERPASS_RGBGEN;
	}
	else if( !stricmp( parameter[0] , "identity" ) || !stricmp( parameter[0] , "identitylighting" ) )
	{
		pass->rgbGen.type = SHADERPASS_RGBGEN_IDENTITY;
		pass->flags |= SHADERPASS_RGBGEN;
	}
	else if( !stricmp( parameter[0] , "vertex" ) || !stricmp( parameter[0] , "exactvertex" ) )
	{
		pass->rgbGen.type = SHADERPASS_RGBGEN_VERTEX;
		pass->flags |= SHADERPASS_RGBGEN;
	}
}

void CShaderMng::ShaderPassKeyFunc_alphagen( shader_t *shader , shaderpass_t *pass )
{
	char parameter[10][32];
	char *token;

	int numParms = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	if     ( !stricmp( parameter[0] , "wave" ) )
	{
		if( numParms < 6 ) return;
		pass->alphaGen.func.type = ParseWaveFunc( parameter[1] );
		if( pass->alphaGen.func.type == WAVE_NONE )	return;

		pass->alphaGen.func.base      = (float) atof( parameter[2] );
		pass->alphaGen.func.amplitude = (float) atof( parameter[3] );
		pass->alphaGen.func.phase     = (float) atof( parameter[4] );
		pass->alphaGen.func.frequency = (float) atof( parameter[5] );
		pass->alphaGen.type = SHADERPASS_RGBGEN_WAVE;

		pass->flags |= SHADERPASS_ALPHAGEN;
	}
	else if( !stricmp( parameter[0] , "identity" ) || !stricmp( parameter[0] , "identitylighting" ) )
	{
		pass->alphaGen.type = SHADERPASS_RGBGEN_IDENTITY;
		pass->flags |= SHADERPASS_ALPHAGEN;
	}
	else if( !stricmp( parameter[0] , "vertex" ) || !stricmp( parameter[0] , "exactvertex" ) )
	{
		pass->alphaGen.type = SHADERPASS_RGBGEN_VERTEX;
		pass->flags |= SHADERPASS_ALPHAGEN;
	}
}

void CShaderMng::ShaderPassKeyFunc_tcgen( shader_t *shader , shaderpass_t *pass )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if     ( !stricmp( parameter , "base" ) )
		pass->tcGen = SHADERPASS_TCGEN_BASE;
	else if( !stricmp( parameter , "lightmap" ) )
	{
		pass->tcGen = SHADERPASS_TCGEN_LIGHTMAP;
		
	}
	else if( !stricmp( parameter , "environment" ) )
	{
		pass->tcGen = SHADERPASS_TCGEN_ENVIRONMENT;
		pass->flags |= SHADERPASS_TCGEN;
	}
	else if( !stricmp( parameter , "vector" ) )
	{
	}
		
	g_script.SkipLine();
}

void CShaderMng::ShaderPassKeyFunc_depthfunc( shader_t *shader , shaderpass_t *pass )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if     ( !stricmp( parameter , "equal" ) )
		pass->depthFunc = D3DCMP_EQUAL;
	else if( !stricmp( parameter , "less" ) )
		pass->depthFunc = D3DCMP_LESS;
	else if( !stricmp( parameter , "greater" ) )
		pass->depthFunc = D3DCMP_GREATER;
	else if( !stricmp( parameter , "lequal" ) )
		pass->depthFunc = D3DCMP_LESSEQUAL;
	else if( !stricmp( parameter , "gequal" ) )
		pass->depthFunc = D3DCMP_GREATEREQUAL;
	else if( !stricmp( parameter , "notequal" ) )
		pass->depthFunc = D3DCMP_NOTEQUAL;

	pass->flags |= SHADERPASS_DEPTHFUNC;

	g_script.SkipLine();
}

void CShaderMng::ShaderPassKeyFunc_alphafunc( shader_t *shader , shaderpass_t *pass )
{
	char *parameter;

	parameter = g_script.GetNextToken( false );
	if( !parameter[0] ) return;

	if     ( !stricmp( parameter , "ge128" ) )
	{
		pass->alphaFunc.func = D3DCMP_GREATEREQUAL;
		pass->alphaFunc.ref = 128;
	}
	else if( !stricmp( parameter , "lt128" ) )
	{
		pass->alphaFunc.func = D3DCMP_LESS;
		pass->alphaFunc.ref = 128;
	}
	else if( !stricmp( parameter , "gt0" ) )
	{
		pass->alphaFunc.func = D3DCMP_GREATER;
		pass->alphaFunc.ref = 0;
	}
	
	pass->flags |= SHADERPASS_ALPHAFUNC | SHADERPASS_NONDEPTHWRITE;

	g_script.SkipLine();
}


void CShaderMng::ShaderPassKeyFunc_tcmod( shader_t *shader , shaderpass_t *pass )
{
	char parameter[10][32];
	char *token;

	int numParms = 0;
	while( 1 )
	{
		token = g_script.GetNextToken( false );
		if( !token[ 0 ] ) break;
		strcpy( parameter[ numParms ] , token );
		numParms++;
	}

	if( pass->tcMod.numTcModFuncs >= MAX_SHADER_TCMOD ) return;
	tcmodfunc_t *tcModeFunc = &pass->tcMod.tcModFuncs[ pass->tcMod.numTcModFuncs ];

	if     ( !stricmp( parameter[0] , "rotate" ) )
	{
		if( numParms < 2 ) return;
		tcModeFunc->type = TCMOD_ROTATE;
		tcModeFunc->params[0] = (float) atof( parameter[1] );
		pass->tcMod.numTcModFuncs++;
		
		pass->flags |= SHADERPASS_TCMOD;
	}
	else if( !stricmp( parameter[0] , "scale" ) )
	{
		if( numParms < 3 ) return;
		tcModeFunc->type = TCMOD_SCALE;
		tcModeFunc->params[0] = (float) atof( parameter[1] );
		tcModeFunc->params[1] = (float) atof( parameter[2] );
		pass->tcMod.numTcModFuncs++;
		
		pass->flags |= SHADERPASS_TCMOD;
	}
	else if( !stricmp( parameter[0] , "scroll" ) )
	{
		if( numParms < 3 ) return;
		tcModeFunc->type = TCMOD_SCROLL;
		tcModeFunc->params[0] = (float) atof( parameter[1] );
		tcModeFunc->params[1] = (float) atof( parameter[2] );
		pass->tcMod.numTcModFuncs++;
		
		pass->flags |= SHADERPASS_TCMOD;
	}
	else if( !stricmp( parameter[0] , "stretch" ) )
	{
		if( numParms < 6 ) return;
		tcModeFunc->type = TCMOD_STRETCH;
		tcModeFunc->func.type = ParseWaveFunc( parameter[1] );
		if( tcModeFunc->func.type == WAVE_NONE )	return;
		tcModeFunc->func.base      = (float) atof( parameter[2] );
		tcModeFunc->func.amplitude = (float) atof( parameter[3] );
		tcModeFunc->func.phase     = (float) atof( parameter[4] );
		tcModeFunc->func.frequency = (float) atof( parameter[5] );
		pass->tcMod.numTcModFuncs++;

		pass->flags |= SHADERPASS_TCMOD;
	}
	else if( !stricmp( parameter[0] , "turb" ) )
	{
		if( numParms < 5 ) return;
		tcModeFunc->type = TCMOD_TURB;
		tcModeFunc->func.type = WAVE_NONE;
		tcModeFunc->func.base      = 1.0f;
		tcModeFunc->func.amplitude = (float) atof( parameter[2] );
		tcModeFunc->func.phase     = (float) atof( parameter[3] );
		tcModeFunc->func.frequency = (float) atof( parameter[4] );
		pass->tcMod.numTcModFuncs++;

		pass->flags |= SHADERPASS_TCMOD;
	}
}

void CShaderMng::ParseShaderPassToken( shader_t *shader , shaderpass_t *pass , char *token )
{
	if     ( !stricmp( token , "map" ) )
	{
		ShaderPassKeyFunc_map( shader , pass );
	}
	else if( !stricmp( token , "clampmap" ) )
	{
		ShaderPassKeyFunc_clampmap( shader , pass );
	}
	else if( !stricmp( token , "animmap" ) )
	{
		ShaderPassKeyFunc_animmap( shader , pass );
	}
	else if( !stricmp( token , "blendfunc" ) )
	{
		ShaderPassKeyFunc_blendfunc( shader , pass );
	}
	else if( !stricmp( token , "rgbgen" ) )
	{
		ShaderPassKeyFunc_rgbgen( shader , pass );
	}
	else if( !stricmp( token , "alphagen" ) )
	{
		ShaderPassKeyFunc_alphagen( shader , pass );
	}
	else if( !stricmp( token , "tcgen" ) )
	{
		ShaderPassKeyFunc_tcgen( shader , pass );
	}
	else if( !stricmp( token , "tcmod" ) )
	{
		ShaderPassKeyFunc_tcmod( shader , pass );
	}
	else if( !stricmp( token , "depthfunc" ) )
	{
		ShaderPassKeyFunc_depthfunc( shader , pass );
	}
	else if( !stricmp( token , "depthwrite" ) )
	{
		pass->flags |= SHADERPASS_DEPTHWRITE;
	}
	else if( !stricmp( token , "detail" ) )
	{
		pass->blend.src  = D3DBLEND_DESTCOLOR;
		pass->blend.dest = D3DBLEND_SRCCOLOR;
		pass->flags |= SHADERPASS_BLEND;
	}
	else if( !stricmp( token , "alphafunc" ) )
	{
		ShaderPassKeyFunc_alphafunc( shader , pass );
	}
	else
		g_script.SkipLine();
}

void CShaderMng::EvaluateTcMod( shaderpass_t *pass , float time )
{
	tcmod_t *tcMod = &pass->tcMod;
	matrix4x4_t mat;

	mat.Translate( 0.5f , 0.5f , 0.0f );

	for( int i = 0 ; i < tcMod->numTcModFuncs ; i++ )
	{
		EvaluateTcModFunc( &tcMod->tcModFuncs[i] , time );
		mat *= tcMod->tcModFuncs[i].mat;
	}

	mat.Translate( -0.5f , -0.5f , 0.0f );
	tcMod->tcMatrix = mat;
	tcMod->tcMatrix.zx = tcMod->tcMatrix.wx;
	tcMod->tcMatrix.zy = tcMod->tcMatrix.wy;
}

void CShaderMng::EvaluateTcModFunc( tcmodfunc_t *tcmodFunc, float asyncTime )
{
	tcmodFunc->mat.Identity();
	
	switch( tcmodFunc->type ) {
        case TCMOD_ROTATE:
            tcmodFunc->mat.RotateZ( tcmodFunc->params[0] * asyncTime );
            break; 
        case TCMOD_SCALE:
            tcmodFunc->mat.Scale( tcmodFunc->params[0] , tcmodFunc->params[1] , 1.0f );
            break;
        case TCMOD_SCROLL:
            tcmodFunc->mat.Translate( tcmodFunc->params[0] * asyncTime , tcmodFunc->params[1] * asyncTime , 0 );
            break;
        case TCMOD_STRETCH: {          
                float f = EvaluateFunc( &tcmodFunc->func , asyncTime );
                tcmodFunc->mat.Scale( f , f , 1.0f );
            }
            break;
        case TCMOD_TURB: {
                
                float x = ( tcmodFunc->func.phase + asyncTime ) * tcmodFunc->func.frequency;
                x -= (float) floor(x);
                float y1,y2;
                y1 = (float) sin( x * __TWOPI ) * tcmodFunc->func.amplitude + tcmodFunc->func.base;
                y2 = (float) sin( ( x + 0.25 ) * __TWOPI ) * tcmodFunc->func.amplitude + tcmodFunc->func.base;
                tcmodFunc->mat.Scale( y1 , y2 , 1.0f );
            }
            break;
        case TCMOD_TRANSFORM:
            break;
    }
}

float CShaderMng::EvaluateFunc( wavefunc_t *func, float asyncTime )
{
	float x, y;

    switch( func->type )
    {
	    case WAVE_SIN:
			x = asyncTime * func->frequency + func->phase;
			x = (float)sin( x * __TWOPI );
			y = func->base + ( func->amplitude * x );
			break;
	    
	    case WAVE_TRIANGLE:
			x = (float)fmod( asyncTime + ( func->phase / func->frequency ) , 1 / func->frequency ) * func->frequency;
			x *= 2.0f;
			if ( x > 1.0f )
				x = 2.0f - x;
			x -= 0.5f;
			x *= 2.0f;
			y = func->base + ( func->amplitude * x );
			break;

	    case WAVE_SQUARE:
			x = (float)fmod( asyncTime + ( func->phase / func->frequency ) , 1 / func->frequency ) * func->frequency;
			if ( x > 0.5f )
				x = 1.0f;
			else
				x = -1.0f;
			y = func->base + ( func->amplitude * x );
			break;

	    case WAVE_SAWTOOTH:
			x = (float)fmod( asyncTime + ( func->phase / func->frequency ) , 1 / func->frequency ) * func->frequency;
			y = func->base + ( func->amplitude * x );
			break;

	    case WAVE_INVSAWTOOTH:
			x = (float)fmod( asyncTime + ( func->phase / func->frequency ) , 1 / func->frequency ) * func->frequency;
			x = func->base + ( func->amplitude * x );
			y = 1.0f - x;
			break;

		default :
			return 0.0f;
    }

    return y;
}

float CShaderMng::EvaluateFunc( wavefunc_t *func , float offset , float asyncTime )
{
	float x, y;

    switch( func->type )
    {
	    case WAVE_SIN:
			x = asyncTime * func->frequency + func->phase + offset;
			x = (float)sin( x * __TWOPI );
			y = func->base + ( func->amplitude * x );
			break;
	    
	    case WAVE_TRIANGLE:
			x = (float)fmod( asyncTime + ( ( func->phase + offset ) / func->frequency ) , 1 / func->frequency ) * func->frequency;
			x *= 2.0f;
			if ( x > 1.0f )
				x = 2.0f - x;
			x -= 0.5f;
			x *= 2.0f;
			y = func->base + ( func->amplitude * x );
			break;

	    case WAVE_SQUARE:
			x = (float)fmod( asyncTime + ( ( func->phase + offset ) / func->frequency ) , 1 / func->frequency ) * func->frequency;
			if ( x > 0.5f )
				x = 1.0f;
			else
				x = -1.0f;
			y = func->base + ( func->amplitude * x );
			break;

	    case WAVE_SAWTOOTH:
			x = (float)fmod( asyncTime + ( ( func->phase + offset ) / func->frequency ) , 1 / func->frequency ) * func->frequency;
			y = func->base + ( func->amplitude * x );
			break;

	    case WAVE_INVSAWTOOTH:
			x = (float)fmod( asyncTime + ( ( func->phase + offset ) / func->frequency ) , 1 / func->frequency ) * func->frequency;
			x = func->base + ( func->amplitude * x );
			y = 1.0f - x;
			break;

		default :
			return 0.0f;
    }

    return y;
}

void CShaderMng::PrepareShader( int idxShader , DWORD frameFlag )
{
	shaderpass_t *pass;
	DWORD flags;
	DWORD genRGBColor = 0x00000000;
	DWORD genAlphaColor;
	float color;
	int   i;
	shader_t *shader;

	shader = &m_shaders[ idxShader ];
	if( shader->frameFlag == frameFlag ) return;
	shader->frameFlag = frameFlag;

	pass = &shader->passes[0];
	for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		flags = pass->flags;

		if( flags & SHADERPASS_MAP ){;}
		else if( flags & SHADERPASS_MAP_LIGHTMAP )
		{
			pass->texRef = g_texMng.GetWhiteImageRef();
		}
		else if( flags & SHADERPASS_MAP_ANIMMAP )
		{
			pass->texRef = pass->animMap->GetFrame( g_timer.GetAppTime() );
		}
		else if( flags & SHADERPASS_MAP_WHITEIMAGE )
		{
			pass->texRef = g_texMng.GetWhiteImageRef();
		}

		if( flags & SHADERPASS_RGBGEN )
		{
			if( pass->rgbGen.type == SHADERPASS_RGBGEN_WAVE )
			{
				color = EvaluateFunc( &pass->rgbGen.func ,  g_timer.GetAppTime() );
				color *= 255.0f;
				if( color < 0.0f ) color = 0.0f;
				else if( color > 255.0f ) color = 255.0f;

				genRGBColor = (DWORD) color;
				pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , genRGBColor , genRGBColor , genRGBColor );
			}
			else if( pass->rgbGen.type == SHADERPASS_RGBGEN_IDENTITY )
				pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
		}

		if( flags & SHADERPASS_ALPHAGEN )
		{
			if( pass->alphaGen.type == SHADERPASS_RGBGEN_WAVE )
			{
				color = EvaluateFunc( &pass->alphaGen.func ,  g_timer.GetAppTime() );
				color *= 255.0f;
				if( color < 0.0f ) color = 0.0f;
				else if( color > 255.0f ) color = 255.0f;

				genAlphaColor = (DWORD) color;
				pass->alphaGen.color = MAKE_COLOR_ARGB( genAlphaColor , genRGBColor , genRGBColor , genRGBColor );
			}
			else if( pass->alphaGen.type == SHADERPASS_RGBGEN_IDENTITY )
				pass->alphaGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
		}

		if( flags & SHADERPASS_TCMOD )
			EvaluateTcMod( pass , g_timer.GetAppTime() );
	}
}

void CShaderMng::PrepareShader( int idxShader , float time )
{
	shaderpass_t *pass;
	DWORD flags;
	DWORD genRGBColor = 0x00000000;
	DWORD genAlphaColor;
	float color;
	int   i;
	shader_t *shader;

	shader = &m_shaders[ idxShader ];
	shader->frameFlag -= 1;

	pass = &shader->passes[0];
	for( i = 0 ; i < shader->numPasses ; i++ , pass++ )
	{
		flags = pass->flags;

		if( flags & SHADERPASS_MAP ){;}
		else if( flags & SHADERPASS_MAP_LIGHTMAP )
		{
			pass->texRef = g_texMng.GetWhiteImageRef();
		}
		else if( flags & SHADERPASS_MAP_ANIMMAP )
		{
			pass->texRef = pass->animMap->GetFrame( time );
		}
		else if( flags & SHADERPASS_MAP_WHITEIMAGE )
		{
			pass->texRef = g_texMng.GetWhiteImageRef();
		}

		if( flags & SHADERPASS_RGBGEN )
		{
			if( pass->rgbGen.type == SHADERPASS_RGBGEN_WAVE )
			{
				color = EvaluateFunc( &pass->rgbGen.func ,  time );
				color *= 255.0f;
				if( color < 0.0f ) color = 0.0f;
				else if( color > 255.0f ) color = 255.0f;

				genRGBColor = (DWORD) color;
				pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , genRGBColor , genRGBColor , genRGBColor );
			}
			else if( pass->rgbGen.type == SHADERPASS_RGBGEN_IDENTITY )
				pass->rgbGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
		}

		if( flags & SHADERPASS_ALPHAGEN )
		{
			if( pass->alphaGen.type == SHADERPASS_RGBGEN_WAVE )
			{
				color = EvaluateFunc( &pass->alphaGen.func ,  time );
				color *= 255.0f;
				if( color < 0.0f ) color = 0.0f;
				else if( color > 255.0f ) color = 255.0f;

				genAlphaColor = (DWORD) color;
				pass->alphaGen.color = MAKE_COLOR_ARGB( genAlphaColor , genRGBColor , genRGBColor , genRGBColor );
			}
			else if( pass->alphaGen.type == SHADERPASS_RGBGEN_IDENTITY )
				pass->alphaGen.color = MAKE_COLOR_ARGB( 0xFF , 0xFF , 0xFF , 0xFF );
		}

		if( flags & SHADERPASS_TCMOD )
			EvaluateTcMod( pass , time );
	}
}

#define TCGEN_EFFECTIVE_DISTANCE   ( 500.0f * 500.0f )

void CShaderMng::PrepareTCGen( shader_t *shader , vec3_t pos )
{
	vec3_t diff;
	float  dist , ratio;

	shader->tcGenMatrix.Identity();

	VectorSubtract( diff, pos, g_camera.m_position );
	dist = VectorDotProduct( diff , diff );

	ratio = dist / TCGEN_EFFECTIVE_DISTANCE;
	if     ( ratio > 10.0f ) ratio = 1.9f;
	else if( ratio > 1.0f )  ratio = 0.9f + ratio * 0.1f;
	else if( ratio > 0.3f )  ratio = 0.12f + ratio * 0.6f;
	else if( ratio < 0.3f )  ratio = 0.3f;

	shader->tcGenMatrix.xx = ratio;
	shader->tcGenMatrix.yy = ratio;

	shader->tcGenMatrix.zx -= g_camera.m_angles[ YAW ] / 360.0f;
	shader->tcGenMatrix.zy -= g_camera.m_angles[ PITCH ] / 180.0f;
}