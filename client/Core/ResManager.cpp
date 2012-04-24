



#include "ResManager.h"
#include <queue>

using namespace std;


void BaseResource::Clear()
{
	m_Priority		=	RES_MED_PRIORITY;
	m_nRefCount		=	0;
	m_LastAccess	=	0;
}

bool BaseResource::operator < ( BaseResource& container )
{
	
	if( GetPriority() < container.GetPriority() ) return true;
	else if( GetPriority() > container.GetPriority() ) return false;
	else
	{
		
		if( m_LastAccess < container.GetLastAccess() ) return true;
		else if( m_LastAccess > container.GetLastAccess() ) return false;
		else
		{
			
			if( GetSize() < container.GetSize() ) return true;
			else return false;
		}
	}
	return false;
}





CResManager::CResManager()
{
	Clear();
}

CResManager::~CResManager()
{

}

void CResManager::Clear()
{
	m_ResourceMap.clear();

	m_rhNextResHandle		=	INVALID_RHANDLE;
	m_nCurrentUsedMemory	=	0;
	m_nMaximumMemory		=	0;
	m_bResourceReserved		=	false;
	m_CurrentResource		=	m_ResourceMap.end();
}


bool CResManager::Create( unsigned int nMaxSize )
{
	Clear();

	SetMaximumMemory( nMaxSize );
	return true;
}


void CResManager::Destroy()
{
	for( ResMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor )
	{
		if( ((*itor).second) == NULL ) continue;
		if( !((*itor).second)->IsLocked() )
		{
			delete ((*itor).second);
		}
	}
	m_ResourceMap.clear();
	Clear();
}

bool CResManager::SetMaximumMemory( size_t nMem )
{
	m_nMaximumMemory = nMem;
	return CheckForOverallocation();
}


bool CResManager::ReserveMemory( size_t nMem )
{
	AddMemory( nMem );
	if( !CheckForOverallocation() ) return false;
	
	m_bResourceReserved = true;
	return true;
}


bool CResManager::InsertResource( RHANDLE* rhUniqueID, BaseResource* pResource )
{
	*rhUniqueID = GetNextResHandle();
	m_ResourceMap.insert( ResMapPair( *rhUniqueID, pResource ) );
	
	if( !m_bResourceReserved )
	{
		AddMemory( pResource->GetSize() );
		if( !CheckForOverallocation() ) return false;
	}
	else m_bResourceReserved = false;
	
	return true;
}


bool CResManager::InsertResource( RHANDLE rhUniqueID, BaseResource* pResource )
{
	ResMapItor itor = m_ResourceMap.find( rhUniqueID );
	
	if( itor != m_ResourceMap.end() ) return false;
	m_ResourceMap.insert( ResMapPair( rhUniqueID, pResource ) );
	
	if( !m_bResourceReserved )
	{
		AddMemory( pResource->GetSize() );
		if( !CheckForOverallocation() ) return false;
	}
	else m_bResourceReserved = false;

	return true;
}


bool CResManager::RegisterResource( RHANDLE rhUniqueID, BaseResource* pResource )
{
	ResMapItor itor = m_ResourceMap.find( rhUniqueID );
	
	if( itor != m_ResourceMap.end() ) return false;
	m_ResourceMap.insert( ResMapPair( rhUniqueID, pResource ) );
	
	return true;
}

bool CResManager::RemoveResource( RHANDLE rhUniqueID )
{
	ResMapItor itor = m_ResourceMap.find( rhUniqueID );
	
	if( itor == m_ResourceMap.end() ) return false;
	if( ((*itor).second)->IsLocked() ) return false;
	RemoveMemory( ((*itor).second)->GetSize() );

	m_ResourceMap.erase( itor );

	return true;
}

bool CResManager::RemoveResource( BaseResource* pResource )
{
	ResMapItor itor;
	
	for( itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor )
	{
		if(itor->second == pResource) break;
	}

	if( itor == m_ResourceMap.end() ) return false;
	if( itor->second->IsLocked() ) return false;
	RemoveMemory( pResource->GetSize() );

	m_ResourceMap.erase( itor );

	return true;
}


bool CResManager::DestroyResource( BaseResource* pResource )
{
	if( !RemoveResource( pResource ) ) return false;
	delete pResource;
	return true;
}

bool CResManager::DestroyResource( RHANDLE rhUniqueID )
{
	BaseResource* pResource = GetResource( rhUniqueID );
	if( !RemoveResource(rhUniqueID ) ) return false;
	delete pResource;
	return true;
}

BaseResource* CResManager::GetResource( RHANDLE rhUniqueID )
{
	ResMapItor itor = m_ResourceMap.find( rhUniqueID );

	bool	bGetSuccess	=	true;

	if( itor == m_ResourceMap.end() ) 
		return NULL;
	
	itor->second->SetLastAccess( time(0) );
	if( itor->second->IsDisposed() )
	{
		itor->second->Recreate();
		AddMemory( itor->second->GetSize() );

		Lock( rhUniqueID );
		
		bGetSuccess	=	CheckForOverallocation();

		Unlock( rhUniqueID );
	}

	if( !bGetSuccess ) return NULL;

	return itor->second;
}

BaseResource* CResManager::Lock( RHANDLE rhUniqueID )
{
	ResMapItor itor = m_ResourceMap.find( rhUniqueID );
	if( itor == m_ResourceMap.end() ) return NULL;
	
	itor->second->SetReferenceCount( itor->second->GetReferenceCount() + 1 );

	if( itor->second->IsDisposed() )
	{
		itor->second->Recreate();
		AddMemory( itor->second->GetSize() );
		CheckForOverallocation();
	}

	return itor->second;
}


int CResManager::Unlock( RHANDLE rhUniqueID )
{
	ResMapItor itor = m_ResourceMap.find( rhUniqueID );
	if( itor == m_ResourceMap.end() ) return -1;
	
	if( itor->second->GetReferenceCount() > 0 )
		itor->second->SetReferenceCount( itor->second->GetReferenceCount() - 1 );

	return itor->second->GetReferenceCount();
}

int CResManager::Unlock( BaseResource* pResource )
{
	RHANDLE rhResource = FindResourceHandle( pResource );
	if IS_INVALID_RHANDLE(rhResource) 
		return -1;
	return Unlock(rhResource);
}


RHANDLE CResManager::FindResourceHandle( BaseResource* pResource )
{
	ResMapItor itor;

	for( itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor )
	{
		if( itor->second == pResource )
			break;
	}
	if( itor == m_ResourceMap.end() )
		return INVALID_RHANDLE;
	return itor->first;
}


bool CResManager::CheckForOverallocation()
{
	if( m_nCurrentUsedMemory > m_nMaximumMemory )
	{
		int iMemToPurge = m_nCurrentUsedMemory - m_nMaximumMemory;

		priority_queue<BaseResource*, vector<BaseResource*>, ptr_greater<BaseResource*> > PriQueue;

		for( ResMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor )
		{
			if( !itor->second->IsDisposed() && !itor->second->IsLocked() )
				PriQueue.push( itor->second );
		}

		while( ( !PriQueue.empty() ) && ( m_nCurrentUsedMemory > m_nMaximumMemory ) )
		{
			unsigned int nDisposalSize = PriQueue.top()->GetSize();
			PriQueue.top()->Dispose();

			if( PriQueue.top()->IsDisposed() ) RemoveMemory(nDisposalSize);
			PriQueue.pop();
		}

		if( PriQueue.empty() && ( m_nCurrentUsedMemory > m_nMaximumMemory ) ) 
			return false;
	}
	return true;
}
