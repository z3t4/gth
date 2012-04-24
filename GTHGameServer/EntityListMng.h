



#if !defined(AFX_ENTITYLISTMNG_H__06F45DBA_2DB0_493D_88CC_0BEE5AAFB1D4__INCLUDED_)
#define AFX_ENTITYLISTMNG_H__06F45DBA_2DB0_493D_88CC_0BEE5AAFB1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define		MAX_NUMBER_OF_ENTITY_PCLIST		5000

typedef struct
{
	int			idx;					
	int			serverIdx;				
	int			idxOnServer;			
	int			characterID;			
	int			serverGroupID;			

	int			worldIdx;				
	int			job ;					
	int			level ;					

	char		name[NAMESTRING];		
	char		userID[NAMESTRING];		


} entityPCList_t;

typedef std::vector<entityPCList_t *> VECTOR_ENTITYPCLIST_STRUCT;

class CEntityListMng  
{
public:
	entityPCList_t				m_entityPCList[MAX_NUMBER_OF_ENTITY_PCLIST];
	
	VECTOR_ENTITYPCLIST_STRUCT	m_vEntityPCList;

	CEntityListMng();
	virtual ~CEntityListMng();

	int	FindPCList ( entityPCList_t * pl );
	int	FindPCList ( char *name, entityPCList_t * pl );
	
	int	FindEmptyPCList ()
	{
		for( int i = 0; i < MAX_NUMBER_OF_ENTITY_PCLIST; ++i )
		{
			if ( m_entityPCList[i].idx < 0 ) return i;
		}

		return -1;
	}

	void			MakePCList( playerCharacter_t *pc, entityPCList_t *pl );
	void			AddPCList( entityPCList_t *pl );
	void			DeletePCList( entityPCList_t *pl );
	void			UpdatePCList( entityPCList_t *pl );
	int				GetPCListNumber()						{	return( m_vEntityPCList.size() );	}
};

#endif 
