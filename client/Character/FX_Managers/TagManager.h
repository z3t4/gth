 
 
 
 
 
 
 
 
 

 
 
 
#ifndef	_FX_TAG_MANAGER_H_SDILKJF408GY9435
#define _FX_TAG_MANAGER_H_SDILKJF408GY9435

 
 
 
#define		FILENAME_TAGSCRIPT_LIST		FILENAME_BASE	TEXT ("TagScriptList.fxd")

#define		FX_TS_MAX_ATK_LVL		3
#define		FX_TS_MAX_DEF_LVL		3
#define		FX_TS_MAX_MGK_LVL		3

#define		FX_MAX_TAGSCRIPT		100

typedef enum
{
	SIN_FUNC	=	0,
	COS_FUNC,
	POS_FIXED
}	FX_TS_FUNCTYPE;

typedef struct
{
	FX_TS_FUNCTYPE	func;
	short			scale;
	float			angle_mul;
}	FX_TS_FUNC;

typedef enum
{
	ALWAYS_LOOK_TGT	=	0,
	ALWAYS_LOOK_FWD,
	ALWAYS_LOOK_CENTER,
	FREE_LOOK
}	FX_TS_LOOK;

typedef struct
{
	char		name [64];
	int			model;
	vec3_t		pos;
	float		velocity;
	FX_TS_FUNC	x_func;
	FX_TS_FUNC	y_func;
	FX_TS_FUNC	z_func;
	char		x_rotation;
	char		x_delay;
	char		y_rotation;
	char		y_delay;
	char		z_rotation;
	char		z_delay;
	FX_TS_LOOK	look;
	int			effect_atk [FX_TS_MAX_ATK_LVL];
	int			effect_def [FX_TS_MAX_DEF_LVL];
	int			effect_mgk [FX_TS_MAX_MGK_LVL];
}	FX_TAGSCRIPT_t;

 
 
 
class CTagManager
{
public:
	 
	CTagManager();
	virtual ~CTagManager();

protected:
	 
	stTAGINFO*		m_aTagInfo;
	INT				m_iTagCount;

		 
	CScript			*m_pScript;

	int				m_numTagScript;
	FX_TAGSCRIPT_t	m_tagScript [FX_MAX_TAGSCRIPT];
	FX_CMathTbl		*m_pMathTbl;

public:
	 
	bool			Load	(	TCHAR	*strFilename	);
	int				GetIndex	(	char	*pszMeshName	);
	char*			GetParentName	(	const int	iTagIndex	);
	INT				GetSize	();
	TCHAR*			GetName	(	const INT	iTagIndex	);
	void			Cleanup	();

	int				LoadTagScript ();
	int				ParseScriptToken (	FX_TAGSCRIPT_t	*in_script,
										char			*in_token	);
	char*			ParseFuncScript (	FX_TS_FUNC	*in_func	);
	char*			ParseSubFuncScript (	FX_TS_FUNC	*in_func,	const bool	in_isMinus	);
	int				ScriptSearch (	char	*in_name	);
	FX_TAGSCRIPT_t*	GetScript (	int		in_scriptID	)		{	return	&m_tagScript [in_scriptID];	}
	float			GetDisplacement (	const FX_TS_FUNCTYPE	in_functype,	const int	in_angle	);
	void			SetMathTbl	(	FX_CMathTbl		*pMathTbl	)		{	m_pMathTbl	=	pMathTbl;	}

		 
	void			SetScript	(	CScript			*script		)		{	m_pScript	=	script;	}
};

#endif