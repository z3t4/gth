



#include "..\global.h"
#include "CGolryunBattle_Observer.h"
#pragma warning(disable:4786)



vec3_t CGolryunBattle_Observer::m_position={14396.0f,3685.0f,3252.0f};

#include "..\Tools\CTools.h"
extern CTools* gcpTools;




CGolryunBattle_Observer::CGolryunBattle_Observer()
{
	m_bOK=FALSE;

	if(FALSE == Init()) return;

	m_bOK=TRUE;

}




CGolryunBattle_Observer::~CGolryunBattle_Observer()
{
	m_mapRecord.clear();
}





BOOL CGolryunBattle_Observer::isOK(void) const
{
	return m_bOK; 
}






BOOL CGolryunBattle_Observer::Delete(const playerCharacter_t* pPlayer)
{
	std::map<int,struct tagObserver>::iterator itor;	
	itor = m_mapRecord.find( pPlayer->idx );

	if ( itor != m_mapRecord.end())
	{
		m_mapRecord.erase(itor);		
		return TRUE;
	}

	return FALSE;
}





BOOL CGolryunBattle_Observer::Insert(const playerCharacter_t* pPlayer)
{
	struct tagObserver Observer;
	memset(&Observer,0x00,sizeof(struct tagObserver));

	Observer.pcIdx = pPlayer->idx;
	strncpy(Observer.name,pPlayer->name,NAMESTRING);
	Observer.name[NAMESTRING]=NULL;
	strncpy(Observer.id,pPlayer->userID,IDSTRING);
	Observer.id[IDSTRING]=NULL;	

	pair<std::map<int,struct tagObserver>::iterator,BOOL> ret;
	ret=m_mapRecord.insert(std::map<int,struct tagObserver>::value_type(pPlayer->idx,Observer));	
	if(FALSE == ret.second) return FALSE;

	return TRUE;
}




BOOL CGolryunBattle_Observer::Init(void)
{
	m_mapRecord.clear();
	return TRUE;
}






playerCharacter_t* CGolryunBattle_Observer::GetPlayerPointer(const struct tagObserver& Observer)
{
	playerCharacter_t* pPlayer=gcpTools->GetPlayerRecordPointer(Observer.pcIdx);
	if(NULL == pPlayer) return NULL;

	if(0 != strcmp(pPlayer->name,Observer.name)) return NULL;
	if(0 != strcmp(pPlayer->userID,Observer.id)) return NULL;
	return pPlayer;
}







void CGolryunBattle_Observer::NET_SendUnreliableMessage(monsterCharacter_t* pMonster)
{
	if(pMonster->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index) return;		
	int distance = GTH_GetDistance2D(m_position,pMonster->position);
	if (distance > MAX_SEEINGDISTANCE) return;

	std::map<int,struct tagObserver>::iterator itor;
	playerCharacter_t* pTempPlayer=NULL;
	for(itor=m_mapRecord.begin(); itor != m_mapRecord.end(); itor++){
		pTempPlayer=GetPlayerPointer(itor->second);
		if(NULL == pTempPlayer) continue;
		

		::NET_SendUnreliableMessage(&pTempPlayer->sock, &netMessage);
	}


}





void CGolryunBattle_Observer::NET_SendUnreliableMessage(playerCharacter_t* pPlayer)
{
	if(pPlayer->worldIdx != tagGolryunBattle::Golryun_Battle_Map_Index) return;		
	int distance = GTH_GetDistance2D(m_position,pPlayer->position);
	if (distance > MAX_SEEINGDISTANCE) return;

	std::map<int,struct tagObserver>::iterator itor;
	playerCharacter_t* pTempPlayer=NULL;
	for(itor=m_mapRecord.begin(); itor != m_mapRecord.end(); itor++){
		pTempPlayer=GetPlayerPointer(itor->second);
		if(NULL == pTempPlayer) continue;
		if(pPlayer == pTempPlayer) continue;

		::NET_SendUnreliableMessage(&pTempPlayer->sock, &netMessage);
	}
}







