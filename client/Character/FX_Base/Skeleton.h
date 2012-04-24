 
 
 
 
 
 
 
 
 
#ifndef	_SKELETON_H_F28HF98H30H4302H40_
#define _SKELETON_H_F28HF98H30H4302H40_

	 
typedef struct
{
	vec3_t		position;		 
	vec4_t		rotation;		 
	vec3_t		scale;			 
	vec4_t		scale_axis;		 
} TAG;

	 
typedef struct 
{
	char			strBoneName	[SIZE_BONENAME];		 

	float			fLength;						 
	int				iParentID;
	int				iNumChildBone;
	int				childBoneID	[MAX_CHILDBONE];
} BONE;

 
 
 
class FX_CSkel
{
	 
public:
	FX_CSkel	();
	~FX_CSkel	();

	 
public:
		 
	int			m_iNumBone;
	int			m_iNumRootBone;
	int			m_iRootBoneID [MAX_ROOTBONE_ID];
	BONE*		m_pBones;

	int			m_fileVersion;

		 
	INT			m_iTagCount;
	TAG			m_Tag [MAX_TAG];

		 
	char				m_strDesc [256];		
	FX_BONESTRUCT_ID	m_pBoneStID;

	 
public:
	bool		LoadData	(	TCHAR		*strFilename,	CFileMng	*fileMng	);
	VOID		Clear	();
	VOID		Cleanup	();

		 
	void		SetGroundTag (	const vec3_t	in_pos	);
};

#endif