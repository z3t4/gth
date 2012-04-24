







#include "../global_def.h"
#include "../Terra ICT/GIFT CARD/tagGiftCardPacket.h"

CGiftCard::CGiftCard()
{

}

CGiftCard::~CGiftCard()
{

}








void CGiftCard::ProcessGiftCard()
{	
	enum tagGiftCardPacket::enumPacketID PacketId;
	PacketId = static_cast<enum tagGiftCardPacket::enumPacketID> (MSG_ReadShort());
	
	switch(PacketId)
	{	
	case tagGiftCardPacket::enumPacketID::SC_Open:				
		GTH_ProcessMessage_Open_Window();		
		break;

	case tagGiftCardPacket::enumPacketID::SC_Notify:		
		GTH_ProcessMessage_Notify();
		break;

	case tagGiftCardPacket::enumPacketID::SC_AddItem:	
		g_ifMng->m_itemWin->Enable();
		GTH_ProcessMessage_Add_Item();
		break;

	case tagGiftCardPacket::enumPacketID::SC_Close:		
		GTH_ProcessMessage_Close();
		break;	
	}
}







void CGiftCard::GTH_ProcessMessage_Notify()
{
	enum tagGiftCardPacket_Notify::enumCode code;
	code = static_cast<enum tagGiftCardPacket_Notify::enumCode>(MSG_ReadByte());

	switch(code)
	{
	case tagGiftCardPacket_Notify::enumCode::success:
		
		g_ifMng->AddChatMessage( "maya", GTHCOLOR_SYSMESSAGE,"Gift Card Registered!" );		
		break;


	case tagGiftCardPacket_Notify::enumCode::can_not_find:		
		g_ifMng->SetMessage("GiftCard",
		"Can't find Card Information");
		break;

	case tagGiftCardPacket_Notify::enumCode::password_not_match:		
		g_ifMng->SetMessage( "GiftCard",
		"Invaild password");
		break;

	case tagGiftCardPacket_Notify::enumCode::pre_use:		
		g_ifMng->SetMessage( "GiftCard",
		"Card already used");
		break;

	case tagGiftCardPacket_Notify::enumCode::unknown:				
	default:
		g_ifMng->SetMessage("GiftCard",
			"unknown");
	}

}







void CGiftCard::GTH_ProcessMessage_Open_Window()
{		

	g_ifMng->m_chatWin->Enable(FALSE);

	
	


	g_ifMng->m_GiftCardWin->InitGiftCardWin();
	g_ifMng->m_GiftCardWin->Enable();	
	g_ifMng->SetFocus(g_ifMng->m_GiftCardWin);
}






void CGiftCard::GTH_SendMessage_Request_item(char* id, char* password)
{	
	MSG_BeginWriting( &netMessage );
	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GIFTCARD );		
		MSG_WriteShort(&netMessage,	tagGiftCardPacket::enumPacketID::CS_Request);	
		MSG_WriteString(&netMessage,id);	
		MSG_WriteString(&netMessage,password);	
		
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);
}







void CGiftCard::GTH_ProcessMessage_Add_Item()
{
			
	item_t item;
	GTH_InitItem(&item);
	MSG_ReadItem(&item);
	
	
	int stackItemIdx = GTH_StackMyItem( &item );		

	if ( stackItemIdx >= 0 && stackItemIdx < MAX_NUMBER_OF_OWNITEM)	
	{	
		g_cgv.myCharacterInfo->item[stackItemIdx].durability += item.durability + 1;
		if( g_cgv.myCharacterInfo->item[stackItemIdx].durability + 1 >= MAX_COUNT_STACK_ITEM )
			g_cgv.myCharacterInfo->item[stackItemIdx].durability = MAX_COUNT_STACK_ITEM - 1;
	}
	else 
	{
		int inventoryPos = GTH_FindEmptyInventoryPos();
		
		assert(inventoryPos >= 0);
		assert(inventoryPos < MAX_INVENTORY_SIZE);			
		
		int itemIdx = GTH_AddMyItem( &item );
		g_cgv.myCharacterInfo->inventory[inventoryPos] = itemIdx;
	}

}








void CGiftCard::GTH_ProcessMessage_Close()
{	
	g_ifMng->m_GiftCardWin->Enable(false);

	g_chatManager->SetInputState( INPUT_INDEX_GIFT_CARD_ID, -1 );
	g_chatManager->SetInputState( INPUT_INDEX_GIFT_CARD_PASSWORD, -1 );
	g_chatManager->ChangeInputMessage( INPUT_INDEX_DIALOGBOX );

}






void CGiftCard::GTH_SendMMessage_Close()
{	
	MSG_BeginWriting( &netMessage );	
	MSG_Clear( &netMessage );
	{
		MSG_WriteByte(&netMessage,	EXTEND_SECOND);
		MSG_WriteShort(&netMessage, GIFTCARD );		
		MSG_WriteShort(&netMessage,	tagGiftCardPacket::enumPacketID::CS_Close);			
		NET_SendMessage(&gsSocket, &netMessage);
	}
	MSG_EndWriting(&netMessage);	
}
	