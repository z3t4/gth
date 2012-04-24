



#if !defined(AFX_HELPERSYSTEM_H__33CDCCE3_CA51_4521_A551_CE49C3B0DF59__INCLUDED_)
#define AFX_HELPERSYSTEM_H__33CDCCE3_CA51_4521_A551_CE49C3B0DF59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum
{
	MAX_TAKER_LIST	= 10,
};

struct HelperEntry  
{
	int		level ;
	int		job ;
	int		worldIdx ;
	char	name[NAMESTRING+1];
	bool	bDeleteMe;

};



struct PointInfo
{
	int		iMyPoint;		
	int		iHelpPoint;		
	int		iTotalHelpPoint;
	int		iWebPoint;
};



class CHelperSystem  
{
public:
	enum enumHelperPacket
	{	
		CS_INVITE_Req_fromHELPER			= 0,		
		SC_INVITE_Req_toTAKER				= 1,		
		CS_INVITE_Res_fromTAKER				= 2,		
		SC_INVITE_Res_toHELPER				= 3,		
		SC_INVITE_Res_toTAKER				= 4,		
			
		CS_REMOVE_Req_fromPlayer			= 5,		
		SC_REMOVE_Res_toPlayer				= 6,		
		SC_REMOVE_NOTIFY_toPlayer			= 7,		
		SC_SEND_MEMBERLIST_toAllPlayer			= 8,		
		SC_SEND_ADD_toPlayer				= 9,		
			
		SC_LIST_Res_toServer				= 10,		
		SC_LISTINFO_toAllPlayer				= 11,		
			
		CS_GIVEPOINT_Req_toHelper			= 12,		
			
		SC_UPDATE_POINTINFO_SEND_toPlayer	= 13,		
			
		CS_SPAWN_Req_fromHelper				= 14,		
			
		CS_SPAWN_Res_fromTaker				= 15,		
			
		SC_SPAWN_Req_toTaker				= 16,		
		SC_SPAWN_Req_toServer				= 17,		
			
		SC_SPAWN_Res_toTaker				= 18,		
		SC_SPAWN_Res_toServer				= 19,
			
		CS_CONFIRM_MYPOINT_Req_fromPlayer	= 20,
		SC_CONFIRM_MYPOINT_Res_toPlayer		= 21,
			
		CS_CONFIRM_HELPPOINT_Req_fromPlayer	= 22,
		SC_CONFIRM_HELPPOINT_Res_toPlayer	= 23,
			
		CS_CONFIRM_WEPPOINT_Req_fromPlayer	= 24,
		SC_CONFIRM_WEPPOINT_Res_toPlayer	= 25,
			
		SC_STACK_OPEN_Req_toPlayer			= 26,		
		CS_MemBerList_Req_fromPlayer		= 27,
		
		SC_SENDMYPOINT_toAllPlayer			= 28,
		SC_SENDHELPPOINT_toHelper			= 29,
	};
	
	int					m_iGivePoint;
	struct HelperEntry	m_HelperEntry[MAX_TAKER_LIST];
	struct PointInfo	m_MyPointInfo;

	CHelperSystem();
	virtual ~CHelperSystem();
	
	void GTH_ProcessMessage_RecvHelpSystem(void);
	void GTH_ProcessMessage_HelpSystem_NeedToHelper(void);
	void GTH_ProcessMessage_HelpSystem_DoYouWant(void);
	void GTH_ProcessMessage_HelpSystem_RequstComplete(void);
	void GTH_ProcessMessage_HelpSystem_RemovePlayer(void);
	void GTH_ProcessMessage_HelpSystem_NotifytoRemovePlayer(void);
	void GTH_ProcessMessage_HelpSystem_AddToPlayer(void);
	void GTH_ProcessMessage_HelpSystem_UpDateListAll(void);
	void GTH_ProcessMessage_HelpSystem_UpDateList(void);
	void GTH_ProcessMessage_HelpSystem_UpDatePointInfo(void);
	void GTH_ProcessMessage_HelpSystem_SpawnTaker(void);
	void GTH_ProcessMessage_HelpSystem_Portal(void);
	void GTH_ProcessMessage_HelpSystem_DisplayMyPoint(void);
	void GTH_ProcessMessage_HelpSystem_DisplayHelpPoint(void);
	void GTH_ProcessMessage_HelpSystem_DisplayWebPoint(void);
	void GTH_ProcessMessage_HelpSystem_StackOpen(void);
	void GTH_ProcessMessage_HelpSystem_GivePointResultToTaker(void);
	void GTH_ProcessMessage_HelpSystem_GivePointResultToHelper(void);

	
	void GTH_SendMessage_HelpSystem_DoYouWant(void);
	void GTH_SendMessage_HelpSystem_AnswerYesOrNo(int Answer);
	void GTH_SendMessage_HelpSystem_RemovePlayer(char *DeleteHelperName);
	void GTH_SendMessage_HelpSystem_GivePointToHelper(int GivePoint, char *HelperName);
	void GTH_SendMessage_HelpSystem_SpawnTaker(char *SpawnHelperName);
	void GTH_SendMessage_HelpSystem_SpawnMeAnswer(int YesOrNo);
	void GTH_SendMessage_HelpSystem_DisplayMyPoint(void);
	void GTH_SendMessage_HelpSystem_DisplayHelpPoint(void);
	void GTH_SendMessage_HelpSystem_DisplayWebPoint(void);
	void GTH_SendMessage_HelpSystem_GiveMeListAll(void);
	BOOL IsChild(char* Name);
};

#endif 
