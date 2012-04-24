



#if !defined(AFX_GIFTCARD_H__D41438A7_98EF_4FF1_A470_A247C36D6CBB__INCLUDED_)
#define AFX_GIFTCARD_H__D41438A7_98EF_4FF1_A470_A247C36D6CBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGiftCard  
{
public:








	enum enumCode{
		success=0,
		can_not_find,
		password_not_match,
		pre_use,
		unknown,
	};	

public:
	void ProcessGiftCard();

public:

	void GTH_ProcessMessage_Notify();		
	void GTH_ProcessMessage_Open_Window();	
	void GTH_SendMessage_Request_item(char* id, char* password);
	void GTH_ProcessMessage_Add_Item();		
	void GTH_ProcessMessage_Close();
	void GTH_SendMMessage_Close();

	CGiftCard();
	virtual ~CGiftCard();

};


#endif 
