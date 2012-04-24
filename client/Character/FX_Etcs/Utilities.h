 
 
 
 
 
 
 
 
 
#ifndef _UTILITIES_H_IJF02J023J03JH0AH0D9H3_
#define _UTILITIES_H_IJF02J023J03JH0AH0D9H3_

 
 
 
typedef struct
{
	char		strName [32];
	int			iIndex;
	char		szParentName [32];
} stTAGINFO;

typedef struct
{
	TCHAR		strName[32];
	INT			iIndex;
} stMESHINFO;

typedef struct
{
	char		name [32];
	int			index;
} BONEINFO_t;






#define	DXVectorSet3( a, b, c, d )		( (a).x = (b), (a).y = (c), (a).z = (d) )
#define	DXVectorSet4( a, b, c, d, e )	( (a).x = (b), (a).y = (c), (a).z = (d), (a).w = (e) )

 
 
 
class CMeshManager
{
public:
	 
	CMeshManager() {};
	virtual ~CMeshManager() {};

protected:
	 
	stMESHINFO*		m_aMeshInfo;
	INT				m_iMeshCount;

		 
	CScript			*m_pScript;

public:
	 
	bool			Load		(	TCHAR	*strFilename	);
	int				GetIndex	(	char	*pszMeshName	);
	INT				GetSize		();
	TCHAR*			GetName		(	const INT	iMeshIndex	);
	void			Cleanup		();

		 
	void			SetScript	(	CScript			*script		)		{	m_pScript	=	script;	}
};

 
 
 
class	FX_CBoneManager
{
public:
	 
	FX_CBoneManager()	{};
	~FX_CBoneManager()	{};

	 
public:
	BONEINFO_t		*m_BoneInfos;
	int				m_iBoneCount;

		 
	CScript			*m_pScript;

	 
public:
	bool			Load		(	char	*pszFileName	);
	int				GetIndex	(	char	*pszBoneName	);
	char*			GetName		(	const int	iBoneIndex	);
	void			Cleanup		();

		 
	void			SetScript	(	CScript			*script		)		{	m_pScript	=	script;	}
};

 
 
 
 
#define		FX_NUM_TRI	3600

class	FX_CMathTbl
{
	 
public:
	FX_CMathTbl ()	{};
	~FX_CMathTbl ()	{};

	 
public:
	float		m_sin [FX_NUM_TRI];
	float		m_cos [FX_NUM_TRI];

		 
	CFileMng			*m_fileMng;

	 
public:
	void			Load	();

		 
	void			SetFileManager	(	CFileMng		*fileMng	)		{	m_fileMng	=	fileMng;	}
};

 
 
 
 
class	FX_CCharUtil
{
	 
public:
	FX_CCharUtil () {};
	~FX_CCharUtil () {};

	 
public:
		 
	CScript		*m_pScript;
	CFileMng	*m_fileMng;
	FX_CMathTbl	*m_mathtbl;

	 
public:
		 
	void		SetFileManager	(	CFileMng		*fileMng	)		{	m_fileMng	=	fileMng;	}
	void		SetScript		(	CScript			*script		)		{	m_pScript	=	script;		}
	void		SetMathTbl		(	FX_CMathTbl		*mathtbl	)		{	m_mathtbl	=	mathtbl;	}

		  
	void		TransformBBox	(	const bboxf_t	&in_bound,	const float		in_yaw,	const float	in_roll,	const vec3_t	in_pos,	
									bboxf_t		&out_bound	);
	void		TransformBBoxByCamera (	const bboxf_t	&in_bound,			const float		in_yaw,	const float		in_camera_yaw,
										const float		in_camera_pitch,	const vec3_t	in_pos,	bboxf_t		&out_bound	);
	void		RemoveExtension (	char	*in_source,	char	*out_dest	);
	void		TransformVertexForBBox	(	const float	*in_vec,	const float	sinval,	const float	cosval,	float	*out_vec	);

	int			MakeAngleWithTime (	const float	in_time,		 
									const float	in_vel	);		 
};

 
#ifdef	_FXDEBUG
	#define	CHECKERROR(cond, message)	\
		if	(	cond	) {	ShowError	(	message,	__FILE__,	__LINE__	);	exit (0);	}
#endif

void	ShowError	(	const char		*pszText,	const char	*pszFileName,	const int	line	);
void	ShowDXError	(	const HRESULT	hr,			const char	*pszFileName,	const int	line	);
void	ShowMessage	(	const char		*pszTitle,	const char	*pszMessage,	const char	*pszFileName,	const int	line	);

void	GetDataFromBuffer	(	void	*out,	const BYTE	*buffer,	const int	size,	int		&count	);

void	BuildQuaternion (	const vec3_t	in_base,
							const vec3_t	in_vector,
							vec4_t			out_quat	);

inline void	Fx_Quat2Multiply	(	vec4_t			qResult,
									const vec4_t	q2,
									const vec4_t	q1		)
{
	qResult [0]	=	q1 [0]*q2 [3] + q1 [1]*q2 [2] - q1 [2]*q2 [1] + q1 [3]*q2 [0];
	qResult [1]	= -	q1 [0]*q2 [2] + q1 [1]*q2 [3] + q1 [2]*q2 [0] + q1 [3]*q2 [1];
	qResult [2]	=	q1 [0]*q2 [1] - q1 [1]*q2 [0] + q1 [2]*q2 [3] + q1 [3]*q2 [2];
	qResult [3]	= -	q1 [0]*q2 [0] - q1 [1]*q2 [1] - q1 [2]*q2 [2] +	q1 [3]*q2 [3];

	return;
};

inline void	Fx_Quat3Multiply	(	vec4_t	qResult,
									vec4_t	qBuffer,
									const vec4_t	q1,
									const vec4_t	q2,
									const vec4_t	q3		)
{
	Fx_Quat2Multiply	(	qBuffer,	q1,			q2		);
	Fx_Quat2Multiply	(	qResult,	qBuffer,	q3		);

	return;
}

void	FX_CatmullRomMulti	(	DIFFUSETEXVERTEX	*out_pt,			 
								const vec3_t		in_first,			 
								const vec3_t		in_firstctrl,		 
								const vec3_t		in_second,			 
								const vec3_t		in_secondctrl,		 
								int					in_numpoints	);

void	InverseRigidTransform	(	matrix4x4_t		*in_source,	matrix4x4_t		*out_dest	);
void	FastSlerp	(	D3DXQUATERNION	*out_quat,
						D3DXQUATERNION	*in_first,
						D3DXQUATERNION	*in_second,
						const float		in_disp		);

void	SaveCurrentCharacterState	(	Fx_CHARACTER_t		*in_char	);

#endif
