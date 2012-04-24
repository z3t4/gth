#include "../global.h"																






CPostManager::CPostManager()
{

}

CPostManager::~CPostManager()
{

}







CPostManager::PostPackage_t*	CPostManager::GetPostPackage(int Idx)
{




	if ( m_PostEntity.size() <= Idx )
		return NULL;
	
	return &m_PostEntity[Idx];
}







bool CPostManager::AddPostPackage(PostPackage_t* pPostPackage)
{
	if ( m_PostEntity.size() >= MAX_POSTPACKAGE_COUNT)
		return 0;

	m_PostEntity.push_back(*pPostPackage);
	return 1;
}











void CPostManager::SetPostSystem(enumPostSystemMode state)
{
	m_PostSystemState = state;
}


CPostManager::enumPostSystemMode	CPostManager::GetPostState()
{
	return m_PostSystemState;
}







int CPostManager::GetPostCount()
{
	return m_PostEntity.size();
}







bool CPostManager::IsValidPost(int idx)
{
	if ( &m_PostEntity[idx] == NULL )
		return 0;

	if ( m_PostEntity[idx].iPostIdx < 0 )	
		return 0;

	if ( strlen( m_PostEntity[idx].szFromName ) == 0 )
		return 0;

	if ( strlen( m_PostEntity[idx].szSendPostTime ) == 0 )
		return 0;
	
	return 1;

}






void CPostManager::InitPostSystem()
{
	
	m_PostEntity.erase(m_PostEntity.begin(), m_PostEntity.end());

	
	m_PostSystemState = POSTSYSTEM_NONE;
	m_pOpenPost		  = NULL;
	m_OpenPostIdx	  = -1;

}

void CPostManager::SetOpenPost(int idx)
{
	m_pOpenPost = GetPostPackage(idx);
	m_OpenPostIdx = idx;
}






CPostManager::PostPackage_t* CPostManager::GetOpenPost()
{
	return m_pOpenPost;
}


BOOL CPostManager::InitPost(PostPackage_t* pPostPackage)
{
	if ( pPostPackage == NULL)
		return FALSE;

	memset(pPostPackage, 0, sizeof(PostPackage_t));

	pPostPackage->bActive = 0;
	pPostPackage->bCheck = 0;
	pPostPackage->iPostIdx = -1;
	pPostPackage->iRemainDays = -1;
	pPostPackage->ItemNumber = 0;

	for (int itemIdx = 0; itemIdx < MAX_ITEMCOUNT_IN_POSTPACKAGE; itemIdx++){
		pPostPackage->Items[itemIdx].itemTableIdx = -1;	
		pPostPackage->ItemBox[itemIdx]=-1;
	}

	pPostPackage->MailState = enumPostPackageState::POSTPACKAGE_UNCHECK;
	pPostPackage->Nak = 0;

	pPostPackage->PostSendType = enumPostSendType::POST_SENDTYPE_CHAR;	


	return TRUE;
}

