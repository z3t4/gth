 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
 
FX_CCharShaderMng::FX_CCharShaderMng ()
{
		 
	memset	(	m_shaderRefs,	0,	sizeof(m_shaderRefs)	);
	m_frameFlag			=	0;
}

FX_CCharShaderMng::~FX_CCharShaderMng ()
{
}

 
 
 
bool	FX_CCharShaderMng::LoadTextureList	()
{
	int		result;

	result	=	m_pScript ->Load	(	FILENAME_TEX_LIST	);
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load texture list file! Check filename!"	)
#endif

	m_pScript ->BeginParse ();

	m_iNumTexture	=	atoi ( m_pScript ->GetNextToken() );

	for	(	int	index	=	0;	\
				index	<	m_iNumTexture;	\
				++index	)	 
	{
		strcpy	(	m_TexBuffer [index].name,
					m_pScript ->GetNextToken ()	);
	}

	if	(	g_cgv.currentWorldIdx	<	0	)
	{
		PrepareCharShaderLogin ();
	}
	else
	{
		PrepareCharShaderCommon ();
	}

	return	true;
}

 
 
 
 
bool	FX_CCharShaderMng::LoadEffectTextureList ()
{
	int		result;

	result	=	m_pScript ->Load	(	FILENAME_EFFECT_TEX_LIST	);
#ifdef	_FXDEBUG
	CHECKERROR (	! result,	"Failed to load effect-texture list file! Check filename!"	)
#endif

	m_pScript ->BeginParse ();

	m_numEffectTexture	=	atoi ( m_pScript ->GetNextToken() );

	for	(	int	index	=	0;	\
				index	<	m_numEffectTexture;	\
				++index	)	 
	{
		strcpy	(	m_effectTexName [index],
					m_pScript ->GetNextToken ()	);
	}

	PrepareEffectShader ();

	return	true;
}

 
 
 
 
int		FX_CCharShaderMng::PrepareCharShaderLogin ()
{
	int		i,	idxShader;
	shader_t			*shader;
	CHAR_SHADERREF_t	*shaderRef;
	char	shaderName [256];
	
	for	(	i	=	0;	\
			i	<	m_iNumTexture;	\
			i++		)
	{
		if	(	strstr ( m_TexBuffer [i].name, "skin/monster/mol/" )	)
		{
		}
		else if	(	strstr ( m_TexBuffer [i].name, "skin/monster" )	||
					strstr ( m_TexBuffer [i].name, "skin/npc" )	)
		{
			continue;
		}

		m_pCharUtil ->RemoveExtension (	m_TexBuffer [i].name,	shaderName	);

		shaderRef	=	&m_shaderRefs [i];
		strcpy	(	shaderRef ->name,	shaderName	);

		idxShader	=	m_shaderMng ->SearchShader ( shaderName );
		if (	idxShader	<	0	) 
		{
			shaderRef ->shader			=	m_pTexMng ->Add	(	shaderName,
																OWNER_TYPE_CHARACTER	);
			shaderRef ->surfaceFlags	=	0;
			shaderRef ->isShader		=	false;
			continue;
		}

		m_shaderMng ->InitShader ( idxShader );
		
		shader	=	m_shaderMng ->GetShader ( idxShader );
		
		shaderRef ->shader			=	idxShader;
		shaderRef ->surfaceFlags	=	shader ->flags;
		shaderRef ->isShader		=	true;

	}

	return true;
}

 
 
 
 
int		FX_CCharShaderMng::PrepareCharShaderCommon ()
{
	int		index,	subindex;
	int		idxShader;
	shader_t			*shader;
	CHAR_SHADERREF_t	*shaderRef;
	char	shaderName [256];
	int		monsterIdx	=	-1;
	bool	flag		=	false;
	int		numMonsterKind			=	g_pApp ->m_mapInfos [0].monsterKind;
	unsigned short	*monsterKind	=	g_pApp ->m_mapInfos [0].monsters;
	
	for	(	index	=	0;	\
			index	<	m_iNumTexture;	\
			index++		)
	{
		if	(	strstr ( m_TexBuffer [index].name, "skin/monster" )	||
				strstr ( m_TexBuffer [index].name, "skin/npc" )	)
		{
			monsterIdx	+=	1;
			bool	found	=	false;
			for	(	subindex	=	0;	\
					subindex	<	numMonsterKind;	\
					++subindex	)
			{
				if	(	monsterIdx	==	g_monsterTable [(int)monsterKind [subindex]].modelID	)
				{
					found	=	true;
					break;
				}
			}

			if	(	! found	)	continue;
		}

		m_pCharUtil ->RemoveExtension (	m_TexBuffer [index].name,	shaderName	);

		shaderRef	=	&m_shaderRefs [index];
		strcpy	(	shaderRef ->name,	shaderName	);

		idxShader	=	m_shaderMng ->SearchShader ( shaderName );
		if (	idxShader	<	0	) 
		{
			shaderRef ->shader			=	m_pTexMng ->Add	(	shaderName,
																OWNER_TYPE_CHARACTER	);
			shaderRef ->surfaceFlags	=	0;
			shaderRef ->isShader		=	false;
			continue;
		}

		m_shaderMng ->InitShader ( idxShader );
		
		shader	=	m_shaderMng ->GetShader ( idxShader );
		
		shaderRef ->shader			=	idxShader;
		shaderRef ->surfaceFlags	=	shader ->flags;
		shaderRef ->isShader		=	true;

	}

	return true;
}

 
 
 
 
int		FX_CCharShaderMng::PrepareEffectShader ()
{
	int		index,	idxShader;
	shader_t			*shader;
	CHAR_SHADERREF_t	*shaderRef;
	char	shaderName [256];
	
	for	(	index	=	0;	\
			index	<	m_numEffectTexture;	\
			index++		)
	{
		m_pCharUtil ->RemoveExtension (	m_effectTexName [index],	shaderName	);

		shaderRef	=	&m_effectShaderRefs [index];
#ifdef	_FXDEBUG
		strcpy	(	shaderRef ->name,	shaderName	);
#endif

		idxShader	=	m_shaderMng ->SearchShader ( shaderName );
		if (	idxShader	<	0	) 
		{
			shaderRef ->shader			=	m_pTexMng ->Add	(	shaderName,
																OWNER_TYPE_CHARACTER	);
			shaderRef ->surfaceFlags	=	0;
			shaderRef ->isShader		=	false;
			continue;
		}

		m_shaderMng ->InitShader ( idxShader );
		
		shader	=	m_shaderMng ->GetShader ( idxShader );
		
		shaderRef ->shader			=	idxShader;
		shaderRef ->surfaceFlags	=	shader ->flags;
		shaderRef ->isShader		=	true;
	}

	return true;
}

 
 
 
 
int		FX_CCharShaderMng::SearchShaderRef (	char	*in_name	)
{
	int		index;
	for	(	index	=	0;	\
			index	<	m_iNumTexture;	\
			++index		)
	{
		if	(	stricmp	( in_name, m_TexBuffer [index].name )	==	0	)
			return	index;
	}

	return	0;
}