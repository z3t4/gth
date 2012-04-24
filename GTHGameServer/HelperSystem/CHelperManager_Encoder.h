



#if !defined(AFX_CHELPERMANAGER_ENCODER_H__AEE01AEB_29A6_4503_B077_6569AA0D154A__INCLUDED_)
#define AFX_CHELPERMANAGER_ENCODER_H__AEE01AEB_29A6_4503_B077_6569AA0D154A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



class CHelperManager_Encoder  
{	
	public:
		void SendMessage_Helper_StackOpen(playerCharacter_t* pPlayer);

	protected:
		void PC_SetSummonsInfo(playerCharacter_t* pPlayer, char* name, int worldIdx, vec3_t position);
		void PC_InitSummonsInfo(playerCharacter_t* pPlayer);
		void SendMessage_Invite_Request_toTaker( playerCharacter_t* pTaker,char* pHelperName);
		void SendMessage_Invite_Respond_toHelper ( playerCharacter_t* pPlayer, int errorCode);
		void SendMessage_Invite_Respond_toTaker( playerCharacter_t* pPlayer, int errorCode);
		
		void SendMessage_Added_toPlayer( 
			playerCharacter_t* pPlayer, char* name, int level, int job, int worldIdx );
		
		void SendMessage_Remove_Respond_toPlayer( 
			playerCharacter_t* pPlayer, char* name, int errorCode);
		
		void SendMessage_Remove_Notify_toRemovePlayer(playerCharacter_t* pPlayer,char* pName);
		void SendMessage_UpdatePointInfo_toPlayer(playerCharacter_t* pPlayer);
		void SendMessage_RequestSummons(playerCharacter_t* pPlayer,char* toName);
		void SendMessage_ReplySummons(int reply, char* toName, char* fromName);
		void SendMessage_Helper_Mypoint(playerCharacter_t* pPlayer);
		void SendMessage_Helper_Helppoint(playerCharacter_t* pPlayer);

		void SendMessage_Mypoint_toTaker(
			playerCharacter_t* pPlayer,char* pTakerName, char* pHelperName, int MyPoint);
		
		void SendMessage_Helppoint_toHelper(
			playerCharacter_t* pPlayer, char* pgiverName, int HelpPoint);

	public:
		CHelperManager_Encoder();
		virtual ~CHelperManager_Encoder();

};

#endif 
