



#if !defined(AFX_CGOLRYUNBATTLE_DATA_H__16B835DA_93EA_4963_8562_6BE794844F8D__INCLUDED_)
#define AFX_CGOLRYUNBATTLE_DATA_H__16B835DA_93EA_4963_8562_6BE794844F8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 









#include "tagGolryunBattle.h"
#include "tagGolryunBattlePacket.h"

class CGolryunBattle_Data  
{
	public:
		struct tagFighter{
			int  pcIdx;
			char id[IDSTRING+1];
			char name[NAMESTRING+1];
			int  FightNum;
		};

		

		struct tagMatch{
			public:
				enum enumCorner{
					None=0,
					Red,
					Blue,
				};

			public:
				struct tagFighter RedCornerFighter;
				struct tagFighter BlueCornerFighter;				
				enum enumCorner   WinnerCorner;
				BOOL bComplete;
		};

		struct tagTournament{
			public:
				struct tagTicket{
					int   iTotalNak;
					float fShareRate;
				};	

			public:
				BOOL  bComplete;
				int   iTournamentGUID;
				int   iPlayLevelGroupIdx;
				struct tagMatch   Match[tagGolryunBattle::Max_Num_Match_Per_Tournament];
				struct tagFighter FirstWinner;
				struct tagFighter SecondWinner;
				struct tagTicket  TwoGyun;
		};

		struct tagEntranceTicket{
			int iMaxAllocGUID;
			int	LotteryGUIDList[tagGolryunBattle::Max_Num_Prize_List_Per_Battle]; 
		};


	private:
		BOOL						m_bOK;

	public:
		int							m_iBattleGUID;
		struct tagEntranceTicket	m_EntranceTicket;
		struct tagTournament		m_Tournament[tagGolryunBattle::Max_Num_Tournament_Per_Battle];				

	public:
		BOOL						isOK(void) const;
		BOOL						InitBattle();		
		struct tagTournament*       GetTournamentPointer(const int Idx);
		
	public:
		CGolryunBattle_Data();
		virtual ~CGolryunBattle_Data();

};

#endif 
