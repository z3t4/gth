



#if !defined(AFX_POSTFUNC_H__B30E5206_64CC_41DA_9772_80C94378B55E__INCLUDED_)
#define AFX_POSTFUNC_H__B30E5206_64CC_41DA_9772_80C94378B55E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "tagGCPacket.h"

class CPostFunc  
{
		
	
public:	
	void GTH_SendMessage_PostSystem_Send();
	int GTH_SendMessage_PostSystem_ReceiveItems();
	void GTH_SendMessage_PostSystem_State();
	void GTH_SendMessage_PostSystem_View();	
	void GTH_SendMessage_PostSystem_DeletePost();
	void GTH_SendMessage_PostSystem_ExitWindow();
	void GTH_SendMessage_PostSystem_WriteOpen(void);


	int	 GTH_SendMessage_MailBox_Open(int PostType, int PostIdx);
	int  GTH_SendMessage_MailBox_Item( int idx, int pos );
	int	 GTH_SendMessage_MailBox_Exit();
	bool GTH_SendMessage_MailBox_UndoInvenItem(void);
	
	
public:	
	
	void GTH_ProcessMessage_RecvPostSystem();
	void GTH_ProcessMessage_PostSystem_OpenWindow();	
	void GTH_ProcessMessage_PostSystem_WriteOpen(void);
	void GTH_ProcessMessage_PostSystem_Send();
	void GTH_ProcessMessage_PostSystem_ReceiveItems();
	void GTH_ProcessMessage_PostSystem_State();
	void GTH_ProcessMessage_PostSystem_View();
	void GTH_ProcessMessage_PostSystem_Arrive();
	void GTH_ProcessMessage_PostSystem_ExitWindow();
	void GTH_ProcessMessage_PostSystem_DeletePost();
	void GTH_ProcessMessage_PostSystem_SendComplete(void);

	
	void GTH_ProcessMessage_MailBox_UndoInvenItem();
	void GTH_ProcessMessage_MailBox_Open();	
	void GTH_ProcessMessage_RecvMailBox();
	int  GTH_ProcessMessage_MailBox_Item();
	void GTH_ProcessMessage_MailBox_Exit();
	

protected:
	inline void GTH_ProcessMessage_PostSystem_ReceiveItems_Success(void);	
	


public:
	CPostFunc();
	virtual ~CPostFunc();

	int		GetItemCount();
	bool	Safe_ItemBox(int index);
};

#endif 
