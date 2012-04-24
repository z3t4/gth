 
 
 
 
 
 
 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
 
CTagManager::CTagManager ()
{
	m_numTagScript	=	0;
}

CTagManager::~CTagManager ()
{
}

 
 
 
bool CTagManager::Load	(	TCHAR*	strFileName	)
{
	m_pScript->Load	(	strFileName	);
	m_pScript->BeginParse	();

	 
	if	(	RESOURCE_VERSION_VALUE	>	100	)
	{
		char	version [MAX_PATH];
		strcpy ( version, m_pScript ->GetNextToken () );
		if	(	! strncmp ( version, "VERSION", 7 )	)
		{
			if	(	strncmp ( &version [7], RESOURCE_VERSION_TEXT, 3 )	)
			{
				 
			}
		}
		else
		{
			 
		}
	}

	m_iTagCount	=	atoi ( m_pScript->GetNextToken() );

	m_aTagInfo	=	new stTAGINFO[m_iTagCount];

	for	(	int	index	=	0;
				index	<	m_iTagCount;
				++index 
		)
	{
		strcpy	(	m_aTagInfo [index]	.strName,
					m_pScript->GetNextToken()		);
		strcpy	(	m_aTagInfo [index]	.szParentName,
					m_pScript->GetNextToken()		);
		m_aTagInfo [index]	.iIndex		=	atoi ( m_pScript->GetNextToken() );
	}

	char	*pszTemp;
	pszTemp	=	m_pScript->GetNextToken();

	if	(	stricmp ( pszTemp, "EOF" )	!=	0	)
	{
		ShowError ( "Tag count doesn't match the actual data!", __FILE__, __LINE__ );
		return	false;
	}

	return	true;
}

 
 
 
int	CTagManager::GetIndex(	char	*pszMeshName	)
{
	for	(	int	index	=	0;
				index	<	m_iTagCount;
				++index		)
	{
		if	(	_stricmp	(	pszMeshName,
								m_aTagInfo [index]	.strName	)	==	0
			)
		{
			return	m_aTagInfo [index] .iIndex;
		}
	}

	ShowError	(	_T	("Failed to find matching tag! Please check tag info file!"),
					__FILE__,
					__LINE__	);
	return	-1;
}

 
 
 
char*	CTagManager::GetParentName	(	const int	iTagIndex	)
{
	if	(	(	iTagIndex	<	0			)		|| 
			(	iTagIndex	>=	m_iTagCount	)
		)	 
	{
		char	message [256];
		sprintf ( message,	"Tag index value is invalid!. Tag Index : %d\n",	iTagIndex );
		ShowError	(	message,
						__FILE__, 
						__LINE__	);
		return	NULL;
	}

	for	(	int	index	=	0; 
				index	<	m_iTagCount;
				++index		)
	{
		if	(	iTagIndex	==	m_aTagInfo [index]	.iIndex		)
			return	m_aTagInfo [index]	.szParentName;
	}

	ShowError	(	_T	("There's no matching tag! Please check tag info file!"),
					__FILE__, 
					__LINE__	);
	return	NULL;
}


 
 
 
INT CTagManager::GetSize()
{
	return m_iTagCount;
}


 
 
 
TCHAR* CTagManager::GetName	(	const int	iTagIndex	)
{
	if	(	(	iTagIndex	<	0			)		||
			(	iTagIndex	>=	m_iTagCount	)
		)	 
	{
		char	message [256];
		sprintf ( message,	"Tag index value is invalid!. Tag Index : %d\n",	iTagIndex );
		ShowError	(	message,
						__FILE__, 
						__LINE__	);
		return	NULL;
	}

	for	(	int	index	=	0;
				index	<	m_iTagCount; 
				++index		)
	{
		if	(	iTagIndex	==	m_aTagInfo [index]	.iIndex		)
			return	m_aTagInfo [index]	.strName;
	}

	ShowError	(	_T	("There's no matching tag! Please check tag info file!"), 
					__FILE__, 
					__LINE__	);
	return	NULL;
}


 
 
 
VOID	CTagManager::Cleanup	()
{
	GTH_SAFE_DELETE_ARRAY ( m_aTagInfo );

	return;
}

 
 
 
 
int		CTagManager::LoadTagScript	()
{
	char	*token;
	FX_TAGSCRIPT_t	*thisScript;

	if (	! m_pScript ->Load ( FILENAME_TAGSCRIPT_LIST	)	)
		return	-1;

	m_pScript->BeginParse	();

	m_numTagScript	=	0;

	while ( 1 )
	{
		token	=	m_pScript ->GetNextToken ( true );
		if (	! token [0]	)	break;

		if (	token [0]	!=	'{'	)	continue;

		if (	m_numTagScript	>=	FX_MAX_TAGSCRIPT	)
		{
			ShowError (	"TAG SCRIPT OVERFLOW!",	__FILE__,	__LINE__	);
			break;
		}

		thisScript	=	&m_tagScript [m_numTagScript++];
		memset (	thisScript,	0,	sizeof (FX_TAGSCRIPT_t)	);

		while ( 1 )
		{
			token	=	m_pScript ->GetNextToken ( true );
			if (	! token [0]	)	break;

			if (	token [0]	==	'}'	)    break;
			else
			{
			    if	(	ParseScriptToken (	thisScript,	token	)	==	-1	)
					return	-1;
			}
		}
	}

	return	true;
}

 
 
 
 
int		CTagManager::ParseScriptToken (	FX_TAGSCRIPT_t	*in_script,
										char			*in_token	)
{
	char	*token;
	int		index;
	int		count;

	if	(	0	==	_stricmp (	in_token,	TEXT ("name")	)	)
	{
		strcpy	(	in_script ->name,	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("model")	)	)
	{
		in_script ->model	=	atoi (	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("pos")	)	)
	{
		in_script ->pos [0]	=	(float)atof (	m_pScript ->GetNextToken ( true )	);
		in_script ->pos [1]	=	(float)atof (	m_pScript ->GetNextToken ( true )	);
		in_script ->pos [2]	=	(float)atof (	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("velocity")	)	)
	{
		in_script ->velocity	=	(float)atof (	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("x_func")	)	)
	{
		FX_TS_FUNC	*thisFunc	=	&in_script ->x_func;
		token	=	ParseFuncScript (	thisFunc	);
		if	(	token	)	
			ParseScriptToken (	in_script,	token	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("y_func")	)	)
	{
		FX_TS_FUNC	*thisFunc	=	&in_script ->y_func;
		token	=	ParseFuncScript (	thisFunc	);
		if	(	token	)	
			ParseScriptToken (	in_script,	token	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("z_func")	)	)
	{
		FX_TS_FUNC	*thisFunc	=	&in_script ->z_func;
		token	=	ParseFuncScript (	thisFunc	);
		if	(	token	)	
			ParseScriptToken (	in_script,	token	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("x_rotation")	)	)
	{
		in_script ->x_rotation	=	(char)atoi (	m_pScript ->GetNextToken ( true )	);
		in_script ->x_delay		=	(char)atoi (	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("y_rotation")	)	)
	{
		in_script ->y_rotation	=	(char)atoi (	m_pScript ->GetNextToken ( true )	);
		in_script ->y_delay		=	(char)atoi (	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("z_rotation")	)	)
	{
		in_script ->z_rotation	=	(char)atoi (	m_pScript ->GetNextToken ( true )	);
		in_script ->z_delay		=	(char)atoi (	m_pScript ->GetNextToken ( true )	);
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("look")	)	)
	{
		token	=	m_pScript ->GetNextToken ( true );
		if	(	0	==	_stricmp (	token,	TEXT ("always_look_tgt")	)	)
			in_script ->look	=	ALWAYS_LOOK_TGT;
		else if (	0	==	_stricmp (	token,	TEXT ("always_look_fwd")	)	)
			in_script ->look	=	ALWAYS_LOOK_FWD;
		else if (	0	==	_stricmp (	token,	TEXT ("always_look_center")	)	)
			in_script ->look	=	ALWAYS_LOOK_CENTER;
		else if (	0	==	_stricmp (	token,	TEXT ("free_look")	)	)
			in_script ->look	=	FREE_LOOK;
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("atk_effect")	)	)
	{
		count	=	atoi (	m_pScript ->GetNextToken ( true )	);
		if	(	count	>	FX_TS_MAX_ATK_LVL	)
		{
			ShowError (	"ATTACK EFFECT OVERFLOW!",	__FILE__,	__LINE__	);
			return	-1;
		}
		for	(	index	=	0;	\
				index	<	count;	\
				++index	)
		{
			in_script ->effect_atk [index]	=	atoi (	m_pScript ->GetNextToken ( true )	);
		}
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("def_effect")	)	)
	{
		count	=	atoi (	m_pScript ->GetNextToken ( true )	);
		if	(	count	>	FX_TS_MAX_DEF_LVL	)
		{
			ShowError (	"DEFENSE EFFECT OVERFLOW!",	__FILE__,	__LINE__	);
			return	-1;
		}
		for	(	index	=	0;	\
				index	<	count;	\
				++index	)
		{
			in_script ->effect_def [index]	=	atoi (	m_pScript ->GetNextToken ( true )	);
		}
	}
	else if (	0	==	_stricmp (	in_token,	TEXT ("mgk_effect")	)	)
	{
		count	=	atoi (	m_pScript ->GetNextToken ( true )	);
		if	(	count	>	FX_TS_MAX_MGK_LVL	)
		{
			ShowError (	"MAGIC EFFECT OVERFLOW!",	__FILE__,	__LINE__	);
			return	-1;
		}
		for	(	index	=	0;	\
				index	<	count;	\
				++index	)
		{
			in_script ->effect_mgk [index]	=	atoi (	m_pScript ->GetNextToken ( true )	);
		}
	}

	return	true;
}

 
 
 
 
char*	CTagManager::ParseFuncScript (	FX_TS_FUNC	*in_func	)
{
	char	*token;

	token	=	m_pScript ->GetNextToken ( true );

	if	(	0	==	_stricmp (	token,	TEXT ("sin_func")	)	)
	{
		in_func ->func	=	SIN_FUNC;
		token	=	ParseSubFuncScript (	in_func,	false	);
		if	(	token	)
			return	token;
	}
	else if (	0	==	_stricmp (	token,	TEXT ("minus_sin_func")	)	)
	{
		in_func ->func	=	SIN_FUNC;
		token	=	ParseSubFuncScript (	in_func,	true	);
		if	(	token	)
			return	token;
	}
	else if (	0	==	_stricmp (	token,	TEXT ("cos_func")	)	)
	{
		in_func ->func	=	COS_FUNC;
		token	=	ParseSubFuncScript (	in_func,	false	);
		if	(	token	)
			return	token;
	}
	else if (	0	==	_stricmp (	token,	TEXT ("minus_cos_func")	)	)
	{
		in_func ->func	=	COS_FUNC;
		token	=	ParseSubFuncScript (	in_func,	true	);
		if	(	token	)
			return	token;
	}
	else if (	0	==	_stricmp (	token,	TEXT ("fixed")	)	)
	{
		in_func ->func	=	POS_FIXED;
	}

	return	NULL;
}

 
 
 
 
char*	CTagManager::ParseSubFuncScript (	FX_TS_FUNC	*in_func,
											const bool	in_isMinus	)
{
	char	*token;

	in_func ->scale	=	atoi (	m_pScript ->GetNextToken ( true )	);

	if	(	in_isMinus	)
		in_func ->scale	=	-in_func ->scale;

	token	=	m_pScript ->GetNextToken ( true );

	if	(	0	==	_stricmp (	token,	TEXT ("angle_mul")	)	)
	{
		in_func ->angle_mul	=	(float)atof (	m_pScript ->GetNextToken ( true )	);
	}
	else
	{
		in_func ->angle_mul	=	1.0f;
		return	token;
	}

	return	NULL;
}

 
 
 
 
int		CTagManager::ScriptSearch (	char	*in_name	)
{
	for	(	int	index	=	0;	\
				index	<	m_numTagScript;	\
				++index	)
	{
		if	(	stricmp	( m_tagScript [index].name, in_name	)	==	0	)
			return	index;
	}

	ShowError	(	TEXT ("THERE'S NO MATCHING SCRIPT!"),	__FILE__,	__LINE__	);
	return	-1;
}

 
 
 
 
float	CTagManager::GetDisplacement (	const FX_TS_FUNCTYPE	in_functype,
										const int				in_angle	)
{
	switch ( in_functype )
	{
	case	POS_FIXED:
		return	0.0f;
	case	SIN_FUNC:
		return	m_pMathTbl ->m_sin [in_angle];
	case	COS_FUNC:
		return	m_pMathTbl ->m_cos [in_angle];
	}

	return	NULL;
}

