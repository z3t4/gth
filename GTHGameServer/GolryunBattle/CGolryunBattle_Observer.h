



#if !defined(AFX_GOLRYUNBATTLE_OBSERVER_H__BF86269C_7F76_4EC0_9986_C604C8DBDAE3__INCLUDED_)
#define AFX_GOLRYUNBATTLE_OBSERVER_H__BF86269C_7F76_4EC0_9986_C604C8DBDAE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CGolryunBattle_Observer  
{
	private:
		BOOL m_bOK;

		struct tagObserver{
			int  pcIdx;
			char name[NAMESTRING+1];
			char id[IDSTRING+1];
		};	
		std::map<int,struct tagObserver> m_mapRecord;
	public:
		static vec3_t m_position;		
		

	private:
		BOOL Init(void);
		playerCharacter_t* GetPlayerPointer(const struct tagObserver& Observer);

	public:
		BOOL isOK(void) const;	
		BOOL Insert(const playerCharacter_t* pPlayer);
		BOOL Delete(const playerCharacter_t* pPlayer);	

		void NET_SendUnreliableMessage(playerCharacter_t* pPlayer);
		void NET_SendUnreliableMessage(monsterCharacter_t* pMonster);
		

	
	public:
		CGolryunBattle_Observer();
		virtual ~CGolryunBattle_Observer();

};

#endif 
