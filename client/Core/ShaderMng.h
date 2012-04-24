



#if !defined(AFX_SHADERMNG_H__695B05D6_23DD_4B58_9867_AA85C5B19C62__INCLUDED_)
#define AFX_SHADERMNG_H__695B05D6_23DD_4B58_9867_AA85C5B19C62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "shader.h"

#define MAX_SIZE_SHADERS  4096  
#define MAX_SIZE_ANIMS    128
#define MAX_SIZE_ENV_SKYS 64

class CShaderMng  
{
public:
	         CShaderMng();
			 CShaderMng( int owner );
	virtual ~CShaderMng();

	int       m_owner;
	shader_t *m_shaders;
	int       m_numShaders;
	int       m_maxSizeShader;

	anim_t    m_animMaps[ MAX_SIZE_ANIMS ];
	envsky_t  m_envSkys[ MAX_SIZE_ENV_SKYS ];
	int       m_numAnimMaps;
	int       m_numEnvSkys;

	void  SetOwner ( int owner )        { m_owner = owner; }
	int   Init( int sizeShader );
	int   GetShaderCount( char *fName , DWORD size = 0 , BYTE *data = NULL );
	int   ReadShaderScript( char *fName , DWORD size = 0 , BYTE *data = NULL );
	int   SearchShader( char *name );
	shader_t *GetShader( int idx );
	shader_t *GetShader( char *name );
	int   MakeDefaultShader( char *name );
	void  InitShader( int idxShader );
	void  Release();
	void  Release( int idxShader );

	
	void  EvaluateTcMod( shaderpass_t *pass , float time );
	void  EvaluateTcModFunc( tcmodfunc_t *tcmodFunc, float asyncTime );
	float EvaluateFunc( wavefunc_t *func, float asyncTime );
	float EvaluateFunc( wavefunc_t *func , float offset , float asyncTime );
	void  PrepareShader( int idxShader , DWORD frameFlag );
	void  PrepareShader( int idxShader , float time );
	void  PrepareTCGen( shader_t *shader , vec3_t pos );

private:
	int  GetShaderTexRef( char *textureName );

	void ParseShaderPass( shader_t *shader );
	void ParseShaderToken( shader_t *shader , char *token );
	void ParseShaderPassToken( shader_t *shader , shaderpass_t *pass , char *token );
	shader_t *GetNewShader( char *name );

	
	wavetype_t ParseWaveFunc( char *param );

	
	void ShaderKeyFunc_skyparms( shader_t *shader );
	void ShaderKeyFunc_cull( shader_t *shader );
	void ShaderKeyFunc_deformvertexes( shader_t *shader );
	void ShaderKeyFunc_sort( shader_t *shader );
	void ShaderKeyFunc_surfaceparm( shader_t *shader );

	
	void ShaderPassKeyFunc_map( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_clampmap( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_animmap( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_blendfunc( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_rgbgen( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_alphagen( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_tcgen( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_depthfunc( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_alphafunc( shader_t *shader , shaderpass_t *pass );
	void ShaderPassKeyFunc_tcmod( shader_t *shader , shaderpass_t *pass );
};


extern CShaderMng g_shaderMng;

#endif 
