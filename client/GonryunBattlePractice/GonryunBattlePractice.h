



#if !defined(AFX_GONRYUNBATTLEPRACTICE_H__F18A9CE1_E566_49AB_A524_3B8724AE776B__INCLUDED_)
#define AFX_GONRYUNBATTLEPRACTICE_H__F18A9CE1_E566_49AB_A524_3B8724AE776B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum
{
	PCB_EFF_NONE,
	PCB_EFF_READY,
	PCB_EFF_WIN,
	PCB_EFF_DRAW,
	PCB_EFF_DEFEAT,
};

enum 
{
	PCB_NONE,
	PCB_SUGGEST,
	PCB_BATTING,
	PCB_READY,
	PCB_COUNTDOWN,
	PCB_START,
	PCB_END,
	PCB_MAX
};

enum enumGonryunBattleErrorCode	
{	
	success						= 1,	
	overflow_oneday_use_count	= 2,	
	missmatch_user_level		= 3,	
	not_enoughf_nak				= 4,	
	busy_GonryunBattlePractice	= 5,	
	overflow_user_number		= 6,	
	gonryun_NotUse				= 7,	
	differLevel					= 8,	
	busy_Suggest				= 9,	
	unknown						= 10,	
	Weight_Over					= 11,
	NotEnough_InventorySpace	= 12,
	gonryun_Lest_UseCount		= 13,
};


class CGonryunBattlePractice  
{
public:

   BOOL					m_bshow;
   DWORD				m_dwFPS;
   DWORD				m_dwTimes;
   short int			m_ieffectMode;	
   short int			m_ibattleMode;			
   short int			m_ibattleinfoMode;      
   short int			m_ieffectIndex;
   int					m_arrEffIdx;
   
   int					m_iLevelDiffer;
  
   int					m_myTradeSEtemp;
   int 					m_otherTradeSEtemp;
   
   int					m_imyTradetemp[MAX_TRADE_SIZE];	
   int					m_iotherTradetemp[MAX_TRADE_SIZE];

   int					m_iroyalty;
   int					m_ibattingRoyalty;
   item_t				m_itradeItemtemp[MAX_TRADE_SIZE];

   float				m_fAngle;
   struct LeaderInfo {
	   int pcidx;
	   char	Name[NAMESTRING+1];
   };

enum enumBattingErrorCode{

	ERROR_None = 0,
	ERROR_Weight_Over,
	ERROR_NotEnough_InventorySpace,
	ERROR_NotEnough_Nake,
	ERROR_Over_BattingNake,
	ERROR_InvalideUser,

};
	struct LeaderInfo m_leader;   
	
	
public:
	CGonryunBattlePractice();

	virtual ~CGonryunBattlePractice();

public :
	void	RecvDanBattle();

public :
	void	RecvErrorCode();
	void	RecvRequesterList();
	void	RecvBattleInfo();
	void	RecvBattleResult();
	void	RecvBattleStart();
	void	RecvSuggestResult();						

	void	RecvBattleReward();
	void	RecvRefreshData();							
	void	RecvGonryunFieldOut();
public :
	void	SendRequestBattle();
	void	SendSetStatus(BYTE bStatus);	
	void	SendSuggestBattle(int danIdx,LPSTR DanName);
	void	SendSuggestResult(BYTE bResult);
	void	SendChangeBattleMap();	
	void	SendInitChao();	
	void	SendCancelRequest();														
	void	SendGonryunBan();
	void    SendGonryunStart(int minLevel,int maxLevel);								
	void	SendMessagePosition(float pos_x,float pos_y, float pos_z);
	void	SendBanBatting(void);
	void	SendCancelBatting(void);
	void	SendDanTestMode(int mode);
	void	SendFailItemAdd(void);
	
	void	RecvGonryunEnd();	
	void	RecvGonryunDeleteUser();
	void	RecvOtherMyinfo();  
	void	RecvMyinfo();
	void	RequestBattleBatting(char *pUser);
	void	RequestBattleGenerate(char *pUser);  
	void    RecvBattleGenerate();
	void    RecvBattleBatting();
	void    SendSuggestResultGenerate(BYTE bresult);
	void	RecvRequestResultBatting(); 
	void	RecvRequestResultGenerate(); 
	void	RecvGonryunState();	
	void	RecvGonryunRewordField();
	void	RecvGonryunBattleInfo();
	void	RecvGonryunBattigReject();
	void	RecvGonryunGenerateReject();
	void	RecvBattleMsg();														
	void    RecvRequestGonryunResult();		
	void	RecvBattleNake();				
	void	RecvBattleAddUser(void);
	void	RecvDeleteUser(void);
	
	void    ProcessGonryunEffect();
    void	RecvProcessEffect();
	void	RecvBattleOpen(void);
	void	GTH_SendMessage_CheatGonryunCode(short int message);


};

#endif 
