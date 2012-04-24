#ifndef _DAN_H_																			
#define _DAN_H_


#define DAN_BATTLEMAP_NUM	20

typedef struct BATTLE_RECORD
{
	int No;
}*LPBATTLE_RECORD;

typedef struct REQUEST_RECORD
{
	int		No;
	int		ServerNo;
	int		ServerID;
	char	DanName[GUILDNAMESTRING];
	char	MasterName[NAMESTRING];

	int		danMemberNumber;					
	int		danBattleJoinNumber;				
	int		KillNumber;							

}*LPREQUEST_RECORD;

class CDanBattle
{
public :

	CDanBattle(){}
	~CDanBattle(){}

public :
	void	RecvDanBattle();

public :
	void	RecvErrorCode();
	void	RecvRequesterList();
	void	RecvSuggest();
	void	RecvBattleInfo();
	void	RecvBattleResult();
	void	RecvBattleStart();
	void	RecvSuggestResult();						
	void	RecvBattleDan();
	void	RecvBattleReward();
	void	RecvRefreshData();							

public :
	void	SendRequestBattle();
	void	SendSetStatus(BYTE bStatus);	
	void	SendSuggestBattle(int danIdx,LPSTR DanName);
	void	SendSuggestResult(BYTE bResult);
	void	SendChangeBattleMap();	
	void	SendInitChao();	
	void	SendCancelRequest();														

	
	void	SendBattleReady();
	void	RecvBattleReady();
	
	
	void	SendDanTestMode(int mode);
};

#endif