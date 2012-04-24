



#if !defined(AFX_POSTFUNC_H__947B49AA_99FC_46A5_B041_60462EE5D219__INCLUDED_)
#define AFX_POSTFUNC_H__947B49AA_99FC_46A5_B041_60462EE5D219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



#include "tagGCPacket.h"


class CPostFunc  
{
	private:
		BOOL InsertCheck_atINVENTORY(
				const playerCharacter_t*           pPlayer,
				const CPostManager::PostPackage_t* pPostPack);	
	
public:
	



	
	void	GTH_ProcessMessage_PostSystem();
	void	GTH_ProcessMessage_RequestPostList();	
	void	GTH_ProcessMessage_PostSend();	
	void	GTH_ProcessMessage_PostWinExit();
	void	GTH_ProcessMessage_PostDelete();

	void	GTH_ProcessMessage_PostState();
	void    GTH_ProcessMessage_PostWriteOpen(void);



	
	
	void	GTH_ProcessMessage_MailBox();
	void	GTH_ProcessMessage_MailBox_Item();
	void	GTH_ProcessMessage_MailBox_Open();

	void	GTH_ProcessMessage_MailBox_CheckAllowReceiveItems();	

	void	GTH_ProcessMessage_MailBox_ReceiveItems(playerCharacter_t* pPlayer);	
	
	void GTH_ProcessMessage_MailBox_AddItemToPlayer(
			playerCharacter_t* pPlayer,
			const int DeletePostPackageIdx,
			const BOOL bDelete);


	void	GTH_ProcessMessage_MailBox_Exit();

	void    GTH_ProcessMessage_MailBox_UndoInvenItem(playerCharacter_t* pc);



	static void GTH_SendMessage_PostList(
			playerCharacter_t* pc,
			const enum tagGCPacket_POSTSYSTEM_OPEN::enumCode code);

	void    GTH_SendMessage_MailBox_UndoInvenItem(playerCharacter_t*, int reply, int errorIdx);

	void	GTH_SendMessage_PostSend(playerCharacter_t*,  int reply, int errorIdx);

	
	void	GTH_SendMessage_MailBox_Exit( playerCharacter_t* pc );
	void	GTH_SendMessage_MailBox_Open(playerCharacter_t* pc, int reply, int error, int PostIdx);

	
	void GTH_SendMessage_MailBox_Item( 
			playerCharacter_t* pc, 
			const CPostManager::PostPackage_t* pPostPack,
			int reply, 
			int error, 
			int isOwner, 
			int pos, 
			int itemTableIdx );

	void	GTH_SendMessage_MailBox_ReceiveItems(playerCharacter_t* pc, int reply, int error);	
	void	GTH_SendMessage_PostDelete(playerCharacter_t* pc, int reply, int error);
	static void	GTH_SendMessage_MailBox_Cnt(playerCharacter_t* pc,const int iUnReadedNum,const int iReadedNum);

	void	GTH_SendMessage_PostWriteOpen(playerCharacter_t* pc, int reply, int error);
	void	GTH_SendMessage_PostState(playerCharacter_t* pc, int reply, int error);
	
	
	
	
	void GTH_SendMessage_PostSendComplete(playerCharacter_t* pc,int reply,  int error);
	





	
public:
	CPostFunc();
	virtual ~CPostFunc();

};

#endif 
