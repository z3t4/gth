



#if !defined(AFX_RESMANAGER_H__53529D1A_A3FC_4AB4_975D_4E5B098E1D7B__INCLUDED_)
#define AFX_RESMANAGER_H__53529D1A_A3FC_4AB4_975D_4E5B098E1D7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#pragma warning( disable : 4786 )
#pragma warning( disable : 4251 )	

#include <ctime>
#include <map>
#include <stack>


enum ResourceType
{
	RESOURCE_TYPE_NORMAL,
	RESOURCE_TYPE_MODEL,
	RESOURCE_TYPE_TEXTURE,
	RESOURCE_TYPE_SOUND,

	NUM_RESOURCE_TYPE
};

#define RES_INIT_CREATE				0x00000000
#define RES_INIT_NOTCREATE			0x00000001

#define RHANDLE						unsigned int
#define INVALID_RHANDLE				0xFFFFFFFF

#define IS_INVALID_RHANDLE(_rh)		((_rh == INVALID_RHANDLE) ? true : false)
#define IS_VALID_RHANDLE(_rh)		((_rh == INVALID_RHANDLE) ? false : true)

class BaseResource 
{
public:
	
	enum PriorityType
	{
		RES_LOW_PRIORITY = 0,
		RES_MED_PRIORITY,
		RES_HIGH_PRIORITY
	};

	BaseResource()			{  Clear();  }
	virtual ~BaseResource()	{  Destroy();  }

	virtual void			Clear();
	virtual bool			Create( char *fname, int iSize )	{  return false;  }
	virtual bool			Register( char *fname, int iSize )	{  return false;  }
	virtual void			Destroy()	{}

	virtual bool			Recreate() = 0;
	virtual void			Dispose() = 0;

	virtual size_t			GetSize() = 0;
	virtual bool			IsDisposed() = 0;

	inline void				SetPriority( PriorityType priority )		{  m_Priority = priority;  }
	inline PriorityType		GetPriority()								{  return m_Priority;  }

	inline void				SetReferenceCount( unsigned int nCount )	{  m_nRefCount = nCount;  }
	inline unsigned int		GetReferenceCount()							{  return m_nRefCount;  }
	inline bool				IsLocked()									{  return (m_nRefCount > 0) ? true : false;  }

	inline void				SetLastAccess( time_t LastAccess )			{  m_LastAccess = LastAccess;  }
	inline time_t			GetLastAccess()								{  return m_LastAccess;  }

	virtual bool operator < (BaseResource& container);

protected:

	PriorityType	m_Priority;
	unsigned int	m_nRefCount;
	time_t			m_LastAccess;

};

template <class T>
class ptr_less
{
public:
	inline bool operator ()(T left, T right)
	{  return ((*left) < (*right));  }
};

template <class T>
class ptr_greater
{
public:
	inline bool operator ()(T left, T right)
	{  return !((*left) < (*right));  }
};


typedef std::map<RHANDLE, BaseResource*>		ResMap;
typedef ResMap::iterator						ResMapItor;
typedef ResMap::value_type						ResMapPair;

class CResManager  
{
public:
	CResManager();
	virtual ~CResManager();

	void		Clear();
	
	bool		Create( unsigned int nMaxSize );
	void		Destroy();

	
	

	bool		SetMaximumMemory( size_t nMem );
	size_t		GetMaximumMemory()	{  return m_nMaximumMemory;  }
	size_t		GetUsedMemory()		{  return m_nCurrentUsedMemory;  }

	
	
	inline void				GotoBegin()				{  m_CurrentResource = m_ResourceMap.begin(); }
	inline BaseResource*	GetCurrentResource()	{  return (*m_CurrentResource).second;  }
	inline bool				GotoNext()				{  m_CurrentResource++;  return IsValid();  }
	inline bool				IsValid()				{  return (m_CurrentResource != m_ResourceMap.end()) ? true : false;  }


	
	
	bool			ReserveMemory( size_t nMem );

	bool			InsertResource( RHANDLE* rhUniqueID, BaseResource* pResource );
	bool			InsertResource( RHANDLE rhUniqueID, BaseResource* pResource );
	
	bool			RegisterResource( RHANDLE rhUniqueID, BaseResource* pResource );

	bool			RemoveResource( BaseResource* pResource );
	bool			RemoveResource( RHANDLE rhUniqueID );

	bool			DestroyResource( BaseResource* pResource );
	bool			DestroyResource( RHANDLE rhUniqueID );

	BaseResource*	GetResource( RHANDLE rhUniqueID );
	BaseResource*	Lock( RHANDLE rhUniqueID );

	int				Unlock( RHANDLE rhUniqueID );
	int				Unlock( BaseResource* pResource );
	RHANDLE			FindResourceHandle( BaseResource* pResource );

	ResourceType	GetResourceType()						{	return m_resourceType;				}
	inline void		SetResourceType( ResourceType type )	{	m_resourceType		=	type;		}

protected:

	inline void		AddMemory( size_t nMem )		{  m_nCurrentUsedMemory += nMem;  }
	inline void		RemoveMemory( size_t nMem )		{  m_nCurrentUsedMemory -= nMem;  }
	unsigned int	GetNextResHandle()				{  return --m_rhNextResHandle;  }

	bool CheckForOverallocation();

protected:

	RHANDLE			m_rhNextResHandle;
	size_t			m_nCurrentUsedMemory;
	size_t			m_nMaximumMemory;
	bool			m_bResourceReserved;
	ResMapItor		m_CurrentResource;
	ResMap			m_ResourceMap;

	ResourceType	m_resourceType;
};

#endif 
