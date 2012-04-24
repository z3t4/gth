 
 
 
 
 
 
 
 
 
#ifndef _ANIMATION_H_JKH2480HF27G0H90H02398
#define _ANIMATION_H_JKH2480HF27G0H90H02398

 
 
 
class FX_CAnim
{
	 
public:
	FX_CAnim	();
	~FX_CAnim	();

	 
public:
		 
	int			m_iNumTrack;					

	TRACK		*m_Tracks;						
	bboxf_t		*m_boundBox;

		 
	FX_CSkel	*m_pSkeleton;						

		 
	float		m_fCurrentTime;			 

	DWORD		m_dwNumVertices;		
	DWORD		m_dwNumIndices;			

		 
	TRACK		*m_ThisTrack;
	int			m_iCurFrame;
	int			m_iNextFrame;
	int			m_iReturnValue;
	int			m_iBoneID;

		 
	char				m_strDesc [256];		
	FX_BONESTRUCT_ID	m_pBoneStID;

	int			m_totalFrames;		 
	float		m_timePerFrame;		 
	float		m_framePerTime;
	float		m_totalTime;		 
	float		m_rcptotalTime;		 
	
	ANIM_PROPERTY_t	m_animProperty;

		 
	int			*m_eopframeIdx;
	int			*m_eoaframeIdx;
	float		m_eopframe;
	float		m_eoaframe;

		 
	float		m_initialHeight;

	int			m_fileVersion;

	 
public:
	bool		Clear	();								
	bool		LoadData(	TCHAR		*strFilename,
							CFileMng	*fileMng	);	
	void		Cleanup();								 

		 
	HRESULT		Interpolate	(	const KEYFRAME	*prevKeyframe,	const KEYFRAME	*nextKeyframe, const float	in_displacement,	KEYFRAME	*result		);
	void		MakeInterpolatedVertices	(	Fx_CHARACTER_t	*in_char,	const int	in_ended	);
	int			CheckKeyframeState		(	Fx_CHARACTER_t	*in_char,	const int	in_option	);
	void		StartNewAnim ();
	void		TransformKeyframes	(	Fx_CHARACTER_t	*in_char	);

	void		QuatInterpolate	(	vec4_t	out_quat,	const vec4_t	in_start,	const vec4_t	in_end,	const float	disp	);

	void		SetEOPFrame	();
	void		SetEOAFrame	();
	void		SetComboStart	(	Fx_CHARACTER_t*		in_char	);

	inline float	GetInitialHeight ()			{	return	m_Tracks [0].pKeyframe [0].vecOffset [2];	};
	inline float	GetInitialTall ()			{	return	( m_boundBox [0][5] - m_boundBox [0][2] );	};
			float	GetInitialRadius ();
};

#endif
 