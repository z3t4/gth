 
 
 
 
 
 
 
 
 
#ifndef	_CHAR_LOOK_UP_MANAGER_H_JD09JE08H08
#define _CHAR_LOOK_UP_MANAGER_H_JD09JE08H08

 
 
 
class FX_CCharLookupMng
{
	 
public:
	FX_CCharLookupMng ()	{};
	~FX_CCharLookupMng()	{};

	Fx_CHAR_LOOKUP_t	m_charLookups[NUM_GEN_LOOKUPS];
#ifdef	_GTH_NEW_VERSION
	Fx_CHAR_LOOKUP_t	m_creatureLookups[NUM_GEN_LOOKUPS];
#endif

	Fx_MON_LOOKUP_t		m_monLookups [NUM_MON_LOOKUPS];

	Fx_ANIM_INFO_t		m_aniInfo[MAX_COUNT_ANIM];

	int			m_fileVersion;

	 
protected:
		 
	CFileMng			*m_fileMng;

	 
public:
		 
	void		SetFileManager	(	CFileMng		*fileMng	)		{	m_fileMng	=	fileMng;	}

	void	LoadTableInfo	();
	void	InitTableInfo	();
	void	SaveTableInfoFromFile( char* filename );
	void	LoadTableInfoFromFile( char* filename );
	void	LoadTableInfoFromBuffer( char* filename );

	int		GetBoneIDFromTable	(	const int	in_entityType,	const int	in_entityIdx	);
	int		GetItemAnimIDFromTable	(	const int	in_entityIdx,	const int	in_itemIdx,		const int	in_animIdx	);
	int		GetCommonAnimIDFromTable	(	const int	in_entityIdx,	const int	in_animIdx		);
	int		GetSkillAnimIDFromTable	(	const int	in_entityIdx,	const int	in_animIdx		);
	int		GetMonAnimIDFromTable	(	const int	in_entityIdx,	const int	in_animIdx		);
	int		GetAnimIDFromTable	(	const int	in_entityType,	const int	in_entityIdx,	const int	in_animType,
									const int	in_itemIdx,		const int	in_animIdx, const int	in_charID,
									const int	in_isCreature	=	false	);

	int		GetAnimTypeFromID	(	const int	in_entityType,	const int	in_entityIdx,	const int	in_animType,
									const int	in_itemIdx,		const int	in_globalAnimIdx	);
	int		GetIdleAnimation	(	const int	in_entityType,	const int	in_entityIdx,	const int	in_animType,	const int	in_itemIdx	);

#ifdef	_GTH_NEW_VERSION
	int		GetCreatureItemAnimIDFromTable	(	const int	in_entityIdx,	const int	in_itemIdx,	const int	in_animIdx	);
	int		GetCreatureCommonAnimIDFromTable	(	const int	in_entityIdx,	const int	in_animIdx	);
	int		GetCreatureSkillAnimIDFromTable	(	const int	in_entityIdx,	const int	in_animIdx		);
#endif
};


#endif