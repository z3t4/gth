



#ifndef _MANAGER_H_
#define _MANAGER_H_	


#include <List>


using namespace std;

template<typename T>
class CManager											
{
public:
	typedef list<T*>				ManagerList;		
	typedef ManagerList::iterator	ManagerItor;

	ManagerList						m_GarbageList;
	ManagerList						m_ManagerList;
	ManagerItor						m_iLoop;
	int								m_PointerSize;

protected :
	
	int								InitializeGarbage(int GarbageCount,int GarbageSize);

public:
	CManager();
	virtual ~CManager();

	int								Create(int GrabageCount,int GarbageSize);
	void							Destroy();

	T*								GetFromGarbage();
	void							PushToManager(T* pVoid);
	T*								PopFromManager();
	void							PushToGarbage(T*	 pVoid);

	T*								FindData(int Pos);

	T*								FindDataName(T *rT);

	void							ClearList();

	int								GetManagerListCount( void ) { return m_ManagerList.size(); }
	BOOL                            GetManagerListEmpty(void){return m_ManagerList.empty();}

};

template<typename T>
CManager<T>::CManager()
{
	m_PointerSize=0;
}

template<typename T>
CManager<T>::~CManager()
{
	Destroy();
}

template<typename T>
int CManager<T>::InitializeGarbage(int GarbageCount,int GarbageSize)
{



	T* pTemp=NULL;
	for (int i=0;i<GarbageCount;i++)
	{
	
	
	
		pTemp=new T;
		if (!pTemp)
			return FALSE;

		m_GarbageList.push_back(pTemp);
	}

	return TRUE;
}

template<typename T>
inline
int CManager<T>::Create(int GarbageCount,int GarbageSize)
{
	m_PointerSize=GarbageSize;
	
	if (!InitializeGarbage(GarbageCount,GarbageSize))
		return FALSE;

	return TRUE;
}

template<typename T>
void CManager<T>::Destroy()
{
	ManagerItor		iLoop;
	for( iLoop = m_ManagerList.begin(); iLoop != m_ManagerList.end(); iLoop++ )
	{
		
		delete (*iLoop);
		
	}

	for( iLoop = m_GarbageList.begin(); iLoop != m_GarbageList.end(); iLoop++ )
	{

		delete (*iLoop);
	
	}

	m_GarbageList.clear();
	m_ManagerList.clear();
}

template<typename T>
inline
T*	CManager<T>::GetFromGarbage()
{
	T* pVoid=NULL;
  
	if (m_GarbageList.empty())
	{
		
		
		
		pVoid=new T;

		return pVoid;

	}


	ManagerItor	itor = m_GarbageList.begin();
	pVoid	= (*itor);
	
	m_GarbageList.pop_front();

	return pVoid;
}

template<typename T>
inline
void CManager<T>::PushToManager(T* pVoid)
{
	m_ManagerList.push_back(pVoid);
}

template<typename T>
inline
T* CManager<T>::PopFromManager()
{
	T* pVoid=NULL;

	if (m_ManagerList.empty())
	{
		return NULL;
	}

	ManagerItor	itor = m_ManagerList.begin();
	pVoid	= (*itor);
	
	m_ManagerList.pop_front();

	return pVoid;
}
template<typename T>
inline
void CManager<T>::PushToGarbage(T* pVoid)
{
	m_GarbageList.push_back(pVoid);
}

template<typename T>
T* CManager<T>::FindData(int Pos)									
{
	ManagerItor		iLoop;
	int				LoopCount=0;

	for( iLoop=m_ManagerList.begin();iLoop!=m_ManagerList.end();iLoop++ )
	{
		if (LoopCount==Pos)
		{
			return (*iLoop);
		}
		LoopCount++;
	}

	return NULL;
}


template<typename T>

T*	CManager<T>::FindDataName(T *rT)
{
	ManagerItor		iLoop=NULL;
	iLoop= find_if(m_ManagerList.begin(), m_ManagerList.end(),bind2nd(Tfind_if(),rT)); 
	return (*iLoop) ;







}

template<typename T>
void CManager<T>::ClearList()
{
	ManagerItor		iLoop;
	int				LoopCount=0;
	T*				pVoid;

	for (iLoop=m_ManagerList.begin();iLoop!=m_ManagerList.end();)
	{
		pVoid=(*iLoop);
		m_ManagerList.erase(iLoop++);
		PushToGarbage(pVoid);
	}
}


#endif _MANAGER_H_
