 
 
 
 
 
 
 
 
 

 
 
 

#include "../../global_def.h"
#include "../Common.h"

 
 
 
FX_CSkelManager::FX_CSkelManager	()
{
	 
	memset	(	m_iBoneLookUp,
				-1,
				sizeof(int) * MAX_COUNT_SKEL * MAX_COUNT_BONE	);
}


 
 
 
bool	FX_CSkelManager::LoadSkelList	()
{
	m_pScript->Load	(	FILENAME_SKEL_LIST	);
	m_pScript->BeginParse();

	 
	m_iNumSkels	=	atoi ( m_pScript->GetNextToken() );

	for	(	int	index	=	0;
				index	<	m_iNumSkels;
				++index	)
	{
		strcpy	(	m_SkelBuffer [index].name,
					m_pScript->GetNextToken()		);
		m_SkelBuffer [index].id	=	atoi ( m_pScript->GetNextToken() );
		LoadSkel	(	m_SkelBuffer [index].name,
						m_SkelBuffer [index].id		);
	}

	return	true;
}


 
 
 
bool	FX_CSkelManager::LoadSkel(	char			*pszSkelName,
									unsigned int	&iSkelID		)
{
	SKELETON_t	*pSkel;

	pSkel	=	&m_Skels	[iSkelID];

	pSkel->skeleton.LoadData	(	pszSkelName,
									m_fileMng	);

	return	true;
}

 
 
 
void	FX_CSkelManager::Cleanup	()
{
	for	(	int	index	=	0;
				index	<	m_iNumSkels;
				++index		)
	{
		m_Skels [index]	.skeleton	.Cleanup();
	}

	return;
}


 
 
 
void	FX_CSkelManager::MakeLookupTable	()
{
	for	(	int	index	=	0;
				index	<	m_iNumSkels;
				++index		)
	{
		for	(	int	subindex	=	0;
					subindex	<	m_pBoneManager->m_iBoneCount;
					++subindex		)
		{
			m_iBoneLookUp [index][subindex]	=	GetLocalID	(	subindex,
																index	);
		}
	}	

	return;
}


 
 
 
int		FX_CSkelManager::GetLocalID	(	const int	iGlobalID,
										const int	iBoneID		)
{
	char		*pszBoneName;
	FX_CSkel	*pThisBone	=	&m_Skels [iBoneID] .skeleton;

	pszBoneName	=	m_pBoneManager->GetName (	iGlobalID	);

	for	(	int	index	=	0;
				index	<	pThisBone->m_iNumBone;
				++index		)
	{
		if	(	0	==	stricmp ( pszBoneName, pThisBone ->m_pBones [index].strBoneName	)	)
		{
			return	index;
		}
	}

	return	-1;
}
