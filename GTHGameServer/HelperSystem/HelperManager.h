



#if !defined(AFX_HELPERMANAGER_H__FBD7FE93_AF5C_40EE_A0D0_6175CD1245FF__INCLUDED_)
#define AFX_HELPERMANAGER_H__FBD7FE93_AF5C_40EE_A0D0_6175CD1245FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "tagHelper.h"

#include "CHelperManager_Encoder.h"



class CHelperManager : public CHelperManager_Encoder  
{
public:
	
	enum  Memberstate { 

			HelpersLogin =	0,
			HelpersLogout=	1,
			HelpersUpdate=	2,
	};
	
	enum define_{
		USERCOUNT				= 0,
		RESPOND_NO				= 0,
		RESPOND_YES				= 1,
		LEVEL_FOUR				= 4,
		LEVEL_TEN				= 10,
		LEVEL_TWENTY			= 20,
		LEVEL_THIRTY			= 30,
		HELPER_STANDARD_LEVEL	= 31,
		MAX_HELPERS_LIST		= 5000,
		
	};
	
public:
	CHelperManager();
	virtual ~CHelperManager();
	
public:
	void ProcessMessage_MemberList_Request_List ();
	void ProcessMessage_MemberList_Request_Update ();
	void ProcessMessage_Reply_Helpers_List ();
	void ProcessMessage_Reply_Helpers_Update ();
	
	
public:
	void ProcessMessage_Helper();
	BOOL ProcessMessage_Helper_SS(const enum enumHelperPacket& SecoundPacket);
	BOOL ProcessMessage_Helper_CS(const enum enumHelperPacket& SecoundPacket);
	
public:
	
	void GiveHelpPointToUser(playerCharacter_t* pPlayer);
	struct tagHelper* GetHelperPtr(const int idx);
	
	public:
		
		void MemberList_Send_toPlayer( entityPCList_t* pPlayerList );
		void MemberListInfo_Send_toAllPlayer(entityPCList_t* pPlayerList, int iFstate );
		void MemberList_Request_List( playerCharacter_t* pPlayer);	
		void MemberList_Request_Update( playerCharacter_t* pPlayer, int iFstate );
		void MemberList_Request_List_OtherServer( playerCharacter_t* pPlayer );
		void MemberList_Request_Update_OtherServer( playerCharacter_t* pPlayer, int iFstate );
	private:
		
		void ProcessMessage_Invite_REQ_fromHELPER(playerCharacter_t* pPlayer);
		void ProcessMessage_Invite_RES_fromTAKER (playerCharacter_t* pPlayer);
		
		void ProcessMessage_CS_REMOVE_REQ_fromPlayer(playerCharacter_t* pPlayer);
		void ProcessMessage_Helper_GivePoint_Req_fromHelper(playerCharacter_t * pPlayer);
		void ProcessMessage_Recall_Req_fromHelper(playerCharacter_t* pPlayer);
		void ProcessMessage_Recall_Res_fromTaker(playerCharacter_t* pPlayer);
		void ProcessMessage_ServerRequestSummons();	
		void ProcessMessage_ServerReplySummons();
		void ProcessMessage_MyPointConfirm(playerCharacter_t* pPlayer);
		void ProcessMessage_HelpPointConfirm(playerCharacter_t* pPlayer);
		void ProcessMessage_Change_WepPoint(playerCharacter_t* pPlayer);
		void ProcessMessage_SC_REMOVE_NOTIFY_toPlayer();
		void ProcessMessage_MemberList_Req_fromPlayer(playerCharacter_t* pPlayer);
		
	private:
		
		void Add_toMemberList( playerCharacter_t* pPlayer, playerCharacter_t* pTarget );
		
		void Remove_toMemberList( playerCharacter_t* pPlayer, char* name );
		
		
		
	private:
		
		BOOL isvalidIndex(const int idx)const;
		BOOL isMyChild(const playerCharacter_t* pPlayer,const char* pChildName) const;
		
	private:

		struct tagHelper	m_tHelpersList[MAX_HELPERS_LIST];
		
};

inline BOOL CHelperManager::isvalidIndex(const int idx) const
{
	return ((0 <=idx) && (idx<MAX_HELPERS_LIST)) ? TRUE : FALSE;
}

#endif 
