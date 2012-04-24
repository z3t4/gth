 
 
 
 
 
 
 
 
 
#ifndef _BONE_MANAGER_HF93H018H203
#define _BONE_MANAGER_HF93H018H203

 
 
 
typedef struct
{
	FX_CSkel		skeleton;
} SKELETON_t;


 
 
 
class FX_CSkelManager
{
	 
public:
	FX_CSkelManager();
	~FX_CSkelManager() {};

	 
public:
		 
	SKELETON_t		m_Skels		[MAX_COUNT_SKEL];
	int				m_iNumSkels;										 
	int				m_iBoneLookUp [MAX_COUNT_SKEL][MAX_COUNT_BONE];		 
		 

	Fx_SKEL_BUF_t	m_SkelBuffer [MAX_COUNT_SKEL];		 

		 
	FX_CBoneManager		*m_pBoneManager;
	
	CScript				*m_pScript;
	CFileMng			*m_fileMng;

	 
public:
	bool		LoadSkelList();
	void		Cleanup();
	int			GetLocalID	(	const int	iGlobalID,	const int	iBoneID	);
	void		MakeLookupTable();

		 
	void		SetBoneManager	(	FX_CBoneManager		*boneManager	)		{	m_pBoneManager	=	boneManager;	}

	void		SetScript		(	CScript			*script		)		{	m_pScript	=	script;		}
	void		SetFileManager	(	CFileMng		*fileMng	)		{	m_fileMng	=	fileMng;	}
	
private:
	bool		LoadSkel	(	char			*pszSkelName,
								unsigned int	&iSkelID		);
	
};

#endif