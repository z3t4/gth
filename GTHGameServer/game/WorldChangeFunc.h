



#if !defined(AFX_WORLDCHANGEFUNC_H__46A55A80_B2C5_42DE_9497_14028B1C573F__INCLUDED_)
#define AFX_WORLDCHANGEFUNC_H__46A55A80_B2C5_42DE_9497_14028B1C573F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum 
	{	GTH_REQUEST_USE_ITEM,
		GTH_REPLY_USE_ITEM,
	
		GTH_REQUEST_WORLD_CHANGE,
		GTH_REPLY_WORLD_CHANGE,

		
		GTH_CLEAR_WORLD_CHANGE_MODE,
	};	


void	GTH_WorldChangeSystem_Process();


void	GTH_SendMessage_Reply_WorldChangeUseItem(playerCharacter_t* pPc, int reply, int erroridx, int InvenPos);
void	GTH_ProcessMessage_Request_WorldChangeUseItem(playerCharacter_t* in_pPlayer);


void	GTH_SendMessage_Reply_WorldChange(playerCharacter_t* pPc, int reply, int erroridx, int InvenPos);
void	GTH_ProcessMessage_ReQuest_WorldChange();


void	GTH_SendMessage_ClearWorldChageMode(playerCharacter_t* pPc);

void	InitWorldChangeSystem();


#endif 
