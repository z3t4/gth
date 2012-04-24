#include "../global_def.h"																






CPostManager::CPostManager()
{
	m_OpenPostIdx = -1;

}

CPostManager::~CPostManager(){}







void CPostManager::AddPostPackage(const PostPackage_t& postpackage)
{		
	m_PostEntity.push_back(postpackage);

}






bool CPostManager::DeletePostPackage(int postidx)
{
	for (int Postcount = 0; Postcount < m_PostEntity.size(); Postcount++)
	{
		if ( m_PostEntity[Postcount].iPostIdx == postidx )
		{
			m_PostEntity.erase(&m_PostEntity[Postcount]);
			return 1;
		}
	}
	return 0;	
}







void CPostManager::InitPostSystem()
{	
	m_PostEntity.erase(m_PostEntity.begin(), m_PostEntity.end());		
	m_PostSystemState	= POSTSYSTEM_READMODE;
	m_pOpenPost			= NULL;
	m_OpenPostIdx		= -1;
}



void CPostManager::InverseState()
{
	if(m_PostSystemState == POSTSYSTEM_READMODE)
		m_PostSystemState = POSTSYSTEM_WRITEMODE;
	else 
		m_PostSystemState = POSTSYSTEM_READMODE;
}


int CPostManager::GetState()
{
	return m_PostSystemState;
}







int CPostManager::GetPostCount()
{
	return m_PostEntity.size();
}






CPostManager::PostPackage_t*	CPostManager::GetPostPackage(int Idx)
{
	



	if ( m_PostEntity.size() <= Idx )
		return NULL;
	
	return & m_PostEntity[Idx];
}







void CPostManager::SetOpenPost(int idx)
{	
	m_pOpenPost = GetPostPackage(idx);
	m_OpenPostIdx = idx;
}







CPostManager::PostPackage_t* CPostManager::GetOpenPost()
{	
	return m_pOpenPost ;
}







bool CPostManager::IsValidPost(int Idx)
{
	if ( &m_PostEntity[Idx] == NULL)
		return 0;

	if ( m_PostEntity[Idx].iPostIdx < 0)
		return 0;	

	if ( strlen(m_PostEntity[Idx].szFromName) == 0 )
		return 0;

	if ( strlen(m_PostEntity[Idx].szSendPostTime) == 0 )
		return 0;

	return 1;	
}

int CPostManager::GetCheckCount()
{
	int Count = 0;
	int MaxPostCount = GetPostCount();	
	
	CPostManager::PostPackage_t* pPostPackage = NULL;
	
	for (int CheckIdx = 0; CheckIdx < MaxPostCount; CheckIdx ++)
	{
		pPostPackage = GetPostPackage(CheckIdx);
		if ( pPostPackage == NULL)
			continue;
		
		if ( pPostPackage->bCheck )
			Count++;
	}
	return Count;	
}



BOOL CPostManager::InitPackage(PostPackage_t* pPostPackage)
{
	if ( pPostPackage == NULL )	
		return FALSE;	

	memset(pPostPackage, 0, sizeof(PostPackage_t));		

	pPostPackage->bActive = 0;
	pPostPackage->bCheck = 0;
	pPostPackage->iPostIdx = -1;
	pPostPackage->iRemainDays = 0;
	pPostPackage->ItemNumber = 0;

	for (int ItemIdx = 0; ItemIdx< CPostManager::MAX_ITEMCOUNT_IN_POSTPACKAGE; ItemIdx++)
		pPostPackage->Items[ItemIdx].itemTableIdx = -1;

	pPostPackage->MailState = CPostManager::enumPostPackageState::POSTPACKAGE_UNCHECK;
	pPostPackage->Nak= 0;
	pPostPackage->PostSendType = CPostManager::enumPostSendType::POST_SENDTYPE_CHAR;	

	return TRUE;
}


void CPostManager::SetPostSystem(enumPostPackageMode mode)
{
	m_PostSystemState = mode;
}
