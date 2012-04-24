 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 

bool		g_bResult;
HRESULT		g_hr;
char		g_szTemp[512];

#define		param1			GLOBAL_VEC3_1
#define		param2			GLOBAL_VEC3_2
#define		param3			GLOBAL_VEC3_3
#define		param4			GLOBAL_VEC3_4

#define		g_nVector		GLOBAL_VEC3_1

 

 
 
 
bool CMeshManager::Load	(	TCHAR	*strFileName	)
{
	m_pScript->Load	(	strFileName	);
	m_pScript->BeginParse();

	m_iMeshCount	=	atoi ( m_pScript->GetNextToken() );

	m_aMeshInfo	=	new	stMESHINFO [m_iMeshCount];

	for	(	int	index	=	0; 
				index	<	m_iMeshCount; 
				++index		)
	{
		strcpy	(	m_aMeshInfo [index]	.strName,
					m_pScript->GetNextToken()		);
		m_aMeshInfo [index]	.iIndex		=	atoi ( m_pScript->GetNextToken() );
	}

	char		*pszTemp;
	pszTemp	=	m_pScript->GetNextToken();

	if	(	stricmp	( pszTemp, "EOF" )	!=	0	)
	{
		ShowError ( "Mesh count doesn't match the actual data!", __FILE__, __LINE__	);
		return	false;
	}

	return	true;
}

 
 
 
int	CMeshManager::GetIndex(	char	*pszMeshName	)
{
	for	(	int	index	=	0;
				index	<	m_iMeshCount;
				++index		)
	{
		if	(	_stricmp	(	pszMeshName,
								m_aMeshInfo [index]	.strName	)	==	0
			)
		{
			return	m_aMeshInfo [index] .iIndex;
		}
	}

	ShowError	(	_T	("Failed to find matching tag! Please check tag info file!"),
					__FILE__,
					__LINE__	);
	return	-1;
}


 
 
 
INT CMeshManager::GetSize	()
{
	return m_iMeshCount;
}


 
 
 
TCHAR* CMeshManager::GetName	(	const INT iMeshIndex	)
{
	if	(	(	iMeshIndex	<	0			)		|| 
			(	iMeshIndex	>=	m_iMeshCount)
		)	 
	{
		ShowError	(	_T	("Mesh index value is invalid!"), 
						__FILE__, 
						__LINE__	);
		return	NULL;
	}

	for	(	int	index	=	0; 
				index	<	m_iMeshCount;
				++index		)
	{
		if	(	iMeshIndex	==	m_aMeshInfo [index]	.iIndex		)
			return	m_aMeshInfo [index]	.strName;
	}

	ShowError	(	_T	("There's no matching Mesh! Please check Mesh info file!"), 
					__FILE__, 
					__LINE__	);
	return	NULL;
}


 
 
 
VOID	CMeshManager::Cleanup	()
{
	GTH_SAFE_DELETE_ARRAY ( m_aMeshInfo );

	return;
}

 

 
 
 
bool	FX_CBoneManager::Load	(	char	*pszFileName	)
{
	m_pScript ->Load	(	pszFileName	);
	m_pScript ->BeginParse ();

	m_iBoneCount	=	atoi ( m_pScript ->GetNextToken () );

	m_BoneInfos	=	new	BONEINFO_t [m_iBoneCount];

	for	(	int	index	=	0; 
				index	<	m_iBoneCount; 
				++index		)
	{
		strcpy	(	m_BoneInfos [index]	.name,
					m_pScript->GetNextToken()		);
		m_BoneInfos [index]	.index	=	atoi ( m_pScript->GetNextToken() );
	}

	char	*pszTemp;
	pszTemp	=	m_pScript->GetNextToken();

	if ( stricmp ( pszTemp,	"EOF" )	!=	0	)
	{
		ShowError ( "Mesh count doesn't match the actual data!", __FILE__, __LINE__	);
		return	false;
	}

	return	true;
}

 
 
 
int		FX_CBoneManager::GetIndex	(	char	*pszBoneName	)
{
	for	(	int	index	=	0;
				index	<	m_iBoneCount;
				++index		)
	{
		if	(	_stricmp	(	pszBoneName,
								m_BoneInfos [index]	.name	)	==	0
			)
		{
			return	m_BoneInfos [index] .index;
		}
	}

	ShowError	(	_T	("Failed to find matching tag! Please check tag info file!"),
					__FILE__,
					__LINE__	);
	return	-1;
}

 
 
 
char*	FX_CBoneManager::GetName	(	const int	iBoneIndex		)
{
	if	(	(	iBoneIndex	<	0			)		|| 
			(	iBoneIndex	>=	m_iBoneCount)
		)	 
	{
		ShowError	(	_T	("Mesh index value is invalid!"), 
						__FILE__, 
						__LINE__	);
		return	NULL;
	}

	for	(	int	index	=	0; 
				index	<	m_iBoneCount;
				++index		)
	{
		if	(	iBoneIndex	==	m_BoneInfos [index]	.index		)
			return	m_BoneInfos [index]	.name;
	}

	ShowError	(	_T	("There's no matching Mesh! Please check Mesh info file!"), 
					__FILE__, 
					__LINE__	);
	return	NULL;
}

 
 
 
void	FX_CBoneManager::Cleanup	()
{
	GTH_SAFE_DELETE_ARRAY ( m_BoneInfos );
}

 

 
 
 
 
void	FX_CMathTbl::Load	()
{
	BYTE	*pFileBuffer;
	int		offset	=	0,	num;

		 
	pFileBuffer	=	m_fileMng->ReadFileFromPack	(	FILENAME_SIN_TABLE	);

	GetDataFromBuffer	(	(void *)&num,	pFileBuffer,	sizeof (int),	offset	);

	if	(	num	!=	FX_NUM_TRI	)
		ShowError	(	TEXT ("FX_CMathTbl: array num & table num mismatch!"),
						__FILE__,	__LINE__	);

	GetDataFromBuffer	(	(void *)&m_sin,	pFileBuffer,	sizeof(float) * FX_NUM_TRI,	offset	);

	GTH_SAFE_FREE ( pFileBuffer );

		 
	offset	=	0;
	pFileBuffer	=	m_fileMng->ReadFileFromPack	(	FILENAME_COS_TABLE	);

	GetDataFromBuffer	(	(void *)&num,	pFileBuffer,	sizeof (int),	offset	);

	if	(	num	!=	FX_NUM_TRI	)
		ShowError	(	TEXT ("FX_CMathTbl: array num & table num mismatch!"),
						__FILE__,	__LINE__	);

	GetDataFromBuffer	(	(void *)&m_cos,	pFileBuffer,	sizeof(float) * FX_NUM_TRI,	offset	);

	GTH_SAFE_FREE ( pFileBuffer );

	return;
}

 

 
 
 
void	ShowError	(	const char	*pszText,
						const char	*pszFileName,
						const int	line	)
{
	TCHAR		szError [512];

	sprintf	(	szError,	"%s\nFile : %s\nLine : %d",
				pszText,	pszFileName,	line	);

	MessageBox	(	NULL,	szError,	"Error!",	MB_OK	);
}


 
 
 
void	ShowDXError	(	const HRESULT	hr,
						const char		*pszFileName,
						const int		line	)
{
	char	szDXError[256];
	char	szError[512];

	D3DXGetErrorString	(	hr,
							szDXError,
							256		);

	sprintf	(	szError,
				"%s\nFile : %s\nLine : %d",
				szDXError,	pszFileName,	line	);

	MessageBox	(	NULL,
					szError,
					"DX Error!",
					MB_OK	);
}


 
 
 
void	ShowMessage	(	const char	*pszTitle,
						const char	*pszMessage,
						const char	*pszFileName,
						const int	line	)
{
	TCHAR		szMessage[512];

	sprintf	(	szMessage,
				"%s\nFile : %s\nLine : %d",
				pszMessage,	pszFileName,	line	);

	MessageBox( NULL, szMessage, pszTitle, MB_OK );
}

 
 
 
void	GetDataFromBuffer	(	void		*out,
								const BYTE	*buffer,
								const int	size,
								int			&count		)
{
	memcpy	(	out,
				buffer + count,
				size	);
	count	+=	size;

	return;
}

 
 
 
 
void	FX_CCharUtil::TransformBBox (	const bboxf_t	&in_bound,
										const float		in_yaw,
										const float		in_roll,
										const vec3_t	in_pos,
										bboxf_t			&out_bound	)
{
	float	sinval;
	float	cosval;
	float	minx,	maxx,	miny,	maxy;
	float	minyy,	maxyy,	minz,	maxz;
	int		index;
	float	point_x	[4];
	float	outpt [8];

	if	(	in_yaw	!=	0.0f	)
	{
		 
		point_x [0]	=	in_bound [0];
		point_x [1]	=	in_bound [3];
		point_x [2]	=	in_bound [1];
		point_x [3]	=	in_bound [4];

		sinval	=	(float)sin (in_yaw);
		cosval	=	(float)cos (in_yaw);

		TransformVertexForBBox	(	point_x,	sinval,	cosval,	outpt		);

		minx	=	outpt [0];
		maxx	=	outpt [0];
		for	(	index	=	1;	\
				index	<	4;	\
				++index		)
		{
			if	(	minx	>	outpt [index]	)
				minx	=	outpt [index];
			if	(	maxx	<	outpt [index]	)
				maxx	=	outpt [index];
		}

		miny	=	outpt [4];
		maxy	=	outpt [4];
		for	(	index	=	5;	\
				index	<	8;	\
				++index		)
		{
			if	(	miny	>	outpt [index]	)
				miny	=	outpt [index];
			if	(	maxy	<	outpt [index]	)
				maxy	=	outpt [index];
		}
	}
	else
	{
		minx	=	in_bound [0];
		maxx	=	in_bound [3];
		miny	=	in_bound [1];
		maxy	=	in_bound [4];
	}
	
	if	(	in_roll	!=	0.0f	)
	{
		 
		point_x [0]	=	miny;
		point_x [1]	=	maxy;
		point_x [2]	=	in_bound [2];
		point_x [3]	=	in_bound [5];

		sinval	=	(float)sin (in_roll);
		cosval	=	(float)cos (in_roll);

		TransformVertexForBBox	(	point_x,	sinval,	cosval,	outpt		);

		minyy	=	outpt [0];
		maxyy	=	outpt [0];
		for	(	index	=	1;	\
				index	<	4;	\
				++index		)
		{
			if	(	minyy	>	outpt [index]	)
				minyy	=	outpt [index];
			if	(	maxyy	<	outpt [index]	)
				maxyy	=	outpt [index];
		}

		minz	=	outpt [4];
		maxz	=	outpt [4];
		for	(	index	=	5;	\
				index	<	8;	\
				++index		)
		{
			if	(	minz	>	outpt [index]	)
				minz	=	outpt [index];
			if	(	maxz	<	outpt [index]	)
				maxz	=	outpt [index];
		}
	}
	else
	{
		minyy	=	in_bound [1];
		maxyy	=	in_bound [4];
		minz	=	in_bound [2];
		maxz	=	in_bound [5];
	}
	
	out_bound [0]	=	minx + in_pos [0];
	out_bound [1]	=	miny + in_pos [1];
	out_bound [2]	=	minz + in_pos [2];

	out_bound [3]	=	maxx + in_pos [0];
	out_bound [4]	=	maxy + in_pos [1];
	out_bound [5]	=	maxz + in_pos [2];

	return;
}

 
 
 
 
void	FX_CCharUtil::TransformBBoxByCamera (	const bboxf_t	&in_bound,
												const float		in_yaw,
												const float		in_camera_yaw,
												const float		in_camera_pitch,
												const vec3_t	in_pos,
												bboxf_t			&out_bound	)
{
	float	sinval;
	float	cosval;
	float	minx,	maxx,	miny,	maxy;
	float	minyy,	maxyy,	minz,	maxz;
	int		index;
	float	point_x	[4];
	float	outpt [8];

	if	(	in_yaw	!=	0.0f	)
	{
		 
		point_x [0]	=	in_bound [0];
		point_x [1]	=	in_bound [3];
		point_x [2]	=	in_bound [1];
		point_x [3]	=	in_bound [4];

		sinval	=	(float)sin (in_yaw);
		cosval	=	(float)cos (in_yaw);

		TransformVertexForBBox	(	point_x,	sinval,	cosval,	outpt		);

		minx	=	outpt [0];
		maxx	=	outpt [0];
		for	(	index	=	1;	\
				index	<	4;	\
				++index		)
		{
			if	(	minx	>	outpt [index]	)
				minx	=	outpt [index];
			if	(	maxx	<	outpt [index]	)
				maxx	=	outpt [index];
		}

		miny	=	outpt [4];
		maxy	=	outpt [4];
		for	(	index	=	5;	\
				index	<	8;	\
				++index		)
		{
			if	(	miny	>	outpt [index]	)
				miny	=	outpt [index];
			if	(	maxy	<	outpt [index]	)
				maxy	=	outpt [index];
		}
	}
	else
	{
		minx	=	in_bound [0];
		maxx	=	in_bound [3];
		miny	=	in_bound [1];
		maxy	=	in_bound [4];
	}

	if	(	in_camera_yaw	!=	0.0f	)
	{
		 
		point_x [0]	=	minx;
		point_x [1]	=	maxx;
		point_x [2]	=	miny;
		point_x [3]	=	maxy;

		sinval	=	(float)sin (in_camera_yaw);
		cosval	=	(float)cos (in_camera_yaw);

		TransformVertexForBBox	(	point_x,	sinval,	cosval,	outpt		);

		minx	=	outpt [0];
		maxx	=	outpt [0];
		for	(	index	=	1;	\
				index	<	4;	\
				++index		)
		{
			if	(	minx	>	outpt [index]	)
				minx	=	outpt [index];
			if	(	maxx	<	outpt [index]	)
				maxx	=	outpt [index];
		}

		miny	=	outpt [4];
		maxy	=	outpt [4];
		for	(	index	=	5;	\
				index	<	8;	\
				++index		)
		{
			if	(	miny	>	outpt [index]	)
				miny	=	outpt [index];
			if	(	maxy	<	outpt [index]	)
				maxy	=	outpt [index];
		}
	}

	if	(	in_camera_pitch	!=	0.0f	)
	{
		 
		point_x [0]	=	miny;
		point_x [1]	=	maxy;
		point_x [2]	=	in_bound [2];
		point_x [3]	=	in_bound [5];

		sinval	=	(float)sin (in_camera_pitch);
		cosval	=	(float)cos (in_camera_pitch);

		TransformVertexForBBox	(	point_x,	sinval,	cosval,	outpt		);

		minyy	=	outpt [0];
		maxyy	=	outpt [0];
		for	(	index	=	1;	\
				index	<	4;	\
				++index		)
		{
			if	(	minyy	>	outpt [index]	)
				minyy	=	outpt [index];
			if	(	maxyy	<	outpt [index]	)
				maxyy	=	outpt [index];
		}

		minz	=	outpt [4];
		maxz	=	outpt [4];
		for	(	index	=	5;	\
				index	<	8;	\
				++index		)
		{
			if	(	minz	>	outpt [index]	)
				minz	=	outpt [index];
			if	(	maxz	<	outpt [index]	)
				maxz	=	outpt [index];
		}
	}
	else
	{
		minyy	=	in_bound [1];
		maxyy	=	in_bound [4];
		minz	=	in_bound [2];
		maxz	=	in_bound [5];
	}

	if	(	minyy	<	miny	)
		miny	=	minyy;
	if	(	maxyy	>	maxy	)
		maxy	=	maxyy;
	
	out_bound [0]	=	minx + in_pos [0];
	out_bound [1]	=	miny + in_pos [1];
	out_bound [2]	=	minz + in_pos [2];

	out_bound [3]	=	maxx + in_pos [0];
	out_bound [4]	=	maxy + in_pos [1];
	out_bound [5]	=	maxz + in_pos [2];

	return;
}

 
 
 
 
void	FX_CCharUtil::RemoveExtension (	char	*in_source,
										char	*out_dest	)
{
	char	*ch;
	int		pos;

	ch	=	strrchr (	in_source,	'.'	);
	pos	=	ch - in_source;

	strncpy (	out_dest,	in_source,	pos	);

	out_dest [pos]	=	'\0';

	return;
}

 
 
 
 
 
 
 
void	FX_CCharUtil::TransformVertexForBBox	(	const float	*in_vec,
													const float	sinval,
													const float	cosval,
													float	*out_vec	)
{
	float	a,	b,	c,	d;
	a	=	cosval * in_vec [0];
	b	=	cosval * in_vec [1];
	c	=	sinval * in_vec [2];
	d	=	sinval * in_vec [3];
	out_vec [0]	=	b - d;	
	out_vec [1]	=	a - d;	
	out_vec [2]	=	a - c;	
	out_vec [3]	=	b - c;	

	a	=	sinval * in_vec [0];
	b	=	sinval * in_vec [1];
	c	=	cosval * in_vec [2];
	d	=	cosval * in_vec [3];
	out_vec [4]	=	b + d;	
	out_vec [5]	=	a + d;	
	out_vec [6]	=	a + c;	
	out_vec [7]	=	b + c;	

	return;
}

 
 
 
 
int		FX_CCharUtil::MakeAngleWithTime (	const float	in_time,
											const float	in_vel	)
{
	return	(	int (in_time * in_vel * (float)3600)	%	3600	);
}

 
 
 
 
void	BuildQuaternion	(	const vec3_t	in_base,
							const vec3_t	in_vector,
							vec4_t			out_quat	)
{
	double		theta;
	double		s, c;
	const float	error	=	0.0001f;


	if	(	(in_vector [0] + 1.0f)	<	error	)
	{
		out_quat [0]	=	0.0f;
		out_quat [1]	=	1.0f;
		out_quat [2]	=	0.0f;
		out_quat [3]	=	0.0f;
	}
	else if	(	(1.0f - in_vector [0])	<	error	)
	{
		out_quat [0]	=	0.0f;
		out_quat [1]	=	0.0f;
		out_quat [2]	=	0.0f;
		out_quat [3]	=	1.0f;
	}
	else
	{
		VectorCrossProduct ( g_nVector,	in_base,	in_vector );
		D3DXVec3Normalize (	(D3DXVECTOR3 *)g_nVector,	(D3DXVECTOR3 *)g_nVector );

		theta		=	acos ( VectorDotProduct ( in_base, in_vector ) / VectorLength ( in_vector ) );	 

		s	=	sin ( theta / 2 );	 
		c	=	cos	( theta	/ 2 );

		out_quat [0]	=	(float)(s * g_nVector [0]);
		out_quat [1]	=	(float)(s * g_nVector [1]);
		out_quat [2]	=	(float)(s * g_nVector [2]);
		out_quat [3]	=	(float)c;

		D3DXQuaternionNormalize	( (D3DXQUATERNION *)out_quat,	(D3DXQUATERNION *)out_quat );
	}

	return;
}

 
 
 
 
void	FX_CatmullRomMulti	(	DIFFUSETEXVERTEX	*out_pt,			 
								const vec3_t		in_first,			 
								const vec3_t		in_firstctrl,		 
								const vec3_t		in_second,			 
								const vec3_t		in_secondctrl,		 
								int					in_numpoints	)
{
	float		disp	=	1.0f / (float)( in_numpoints + 1 );
	float		weight	=	0.0f,	weightsquare,	weighttriple;

	VectorScale ( param1,	in_first,	2.0f );
	VectorSubtract ( param2,	in_second,	in_firstctrl );
	param3 [0]	=	2.0f * in_firstctrl	[0]	-	5.0f * in_first [0]	+	4.0f * in_second [0]	-	in_secondctrl [0];
	param3 [1]	=	2.0f * in_firstctrl	[1]	-	5.0f * in_first [1]	+	4.0f * in_second [1]	-	in_secondctrl [1];
	param3 [2]	=	2.0f * in_firstctrl	[2]	-	5.0f * in_first [2]	+	4.0f * in_second [2]	-	in_secondctrl [2];

	param4 [0]	=	-in_firstctrl [0]	+	3.0f * in_first [0]	-	3.0f * in_second [0]	+	in_secondctrl [0];
	param4 [1]	=	-in_firstctrl [1]	+	3.0f * in_first [1]	-	3.0f * in_second [1]	+	in_secondctrl [1];
	param4 [2]	=	-in_firstctrl [2]	+	3.0f * in_first [2]	-	3.0f * in_second [2]	+	in_secondctrl [2];

	for	(	int	index	=	0;	\
				index	<	in_numpoints;	\
				++index	)
	{
		weight		+=	disp;
		weightsquare	=	weight * weight;
		weighttriple	=	weightsquare * weight;

		out_pt [2 * index].vecPos [0]	=	0.5f * (	param1 [0]	+	param2 [0] * weight	+	param3 [0] * weightsquare	+	param4 [0] * weighttriple	);
		out_pt [2 * index].vecPos [1]	=	0.5f * (	param1 [1]	+	param2 [1] * weight	+	param3 [1] * weightsquare	+	param4 [1] * weighttriple	);
		out_pt [2 * index].vecPos [2]	=	0.5f * (	param1 [2]	+	param2 [2] * weight	+	param3 [2] * weightsquare	+	param4 [2] * weighttriple	);
	}

	return;
}

 
 
 
 
void	InverseRigidTransform	(	matrix4x4_t		*in_source,	
									matrix4x4_t		*out_dest	)
{
	out_dest ->a00	=	in_source ->a00;
	out_dest ->a10	=	in_source ->a01;
	out_dest ->a20	=	in_source ->a02;
	out_dest ->a30	=	0.0f;
	out_dest ->a01	=	in_source ->a10;
	out_dest ->a11	=	in_source ->a11;
	out_dest ->a21	=	in_source ->a12;
	out_dest ->a31	=	0.0f;
	out_dest ->a02	=	in_source ->a20;
	out_dest ->a12	=	in_source ->a21;
	out_dest ->a22	=	in_source ->a22;
	out_dest ->a32	=	0.0f;
	out_dest ->a03	=	- ( in_source ->a03 * in_source ->a00	+	in_source ->a13 * in_source ->a10	+	in_source ->a23 * in_source ->a20 );
	out_dest ->a13	=	- ( in_source ->a03 * in_source ->a01	+	in_source ->a13 * in_source ->a11	+	in_source ->a23 * in_source ->a21 );
	out_dest ->a23	=	- ( in_source ->a03 * in_source ->a02	+	in_source ->a13 * in_source ->a12	+	in_source ->a23 * in_source ->a22 );
	out_dest ->a33	=	in_source ->a33;
}

 
 
 
 
void	FastSlerp	(	D3DXQUATERNION	*out_quat,
						D3DXQUATERNION	*in_first,
						D3DXQUATERNION	*in_second,
						const float		in_disp		)
{
	if	(	(	( in_first->x	-	in_second->x )	<	0.001f	)	&&
			(	( in_first->y	-	in_second->y )	<	0.001f	)	&&
			(	( in_first->z	-	in_second->z )	<	0.001f	)	&&
			(	( in_first->w	-	in_second->w )	<	0.001f	)	)
	{
		memcpy ( out_quat,	in_first, sizeof(D3DXQUATERNION) );
		return;
	}

	if	(	(	( in_first->x	+	in_second->x )	<	0.001f	)	&&
			(	( in_first->y	+	in_second->y )	<	0.001f	)	&&
			(	( in_first->z	+	in_second->z )	<	0.001f	)	&&
			(	( in_first->w	+	in_second->w )	<	0.001f	)	)
	{
		memcpy ( out_quat,	in_first, sizeof(D3DXQUATERNION) );
		return;
	}

	D3DXQuaternionNormalize ( in_first,		in_first );
	D3DXQuaternionNormalize ( in_second,	in_second );

	double	cosval	=	in_first ->x * in_second ->x	+	in_first ->y * in_second ->y	+	in_first ->z * in_second ->z	+	in_first ->w * in_second ->w;
	double	theta	=	acos ( cosval );

	char	temp [256];

	if	(	sin ( theta )	<	0.0001f	)
	{
		sprintf ( temp, "ERROR!: sin value is too small!,	cosval : %f",	cosval );
		ShowError	(	temp,	__FILE__,	__LINE__	);
	}

	double	invsin	=	1.0f / sin ( theta );
	double	a	=	sin ( theta * ( 1.0f - in_disp ) ) * invsin;
	double	b	=	sin ( theta * in_disp ) * invsin;

	out_quat ->x	=	a * in_first ->x	+	b * in_second ->x;
	out_quat ->y	=	a * in_first ->y	+	b * in_second ->y;
	out_quat ->z	=	a * in_first ->z	+	b * in_second ->z;
	out_quat ->w	=	a * in_first ->w	+	b * in_second ->w;

	D3DXQuaternionNormalize ( out_quat, out_quat );
}
