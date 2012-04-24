



#if !defined(AFX_WORLDCHANGEFUNC_H__D74A2D37_A071_4A90_9D38_FB3272873661__INCLUDED_)
#define AFX_WORLDCHANGEFUNC_H__D74A2D37_A071_4A90_9D38_FB3272873661__INCLUDED_

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


void	GTH_SendMessage_Request_WorldChangeUseItem(WorldChangeSystem *pWorldChangeSystem);
void	GTH_ProcessMessage_Reply_WorldChangeUseItem();


void	GTH_SendMessage_Request_WorldChange(WorldChangeSystem *pWorldChangeSystem);
void	GTH_ProcessMessage_Reply_WorldChange();



void	GTH_ProcessMessage_ClearWorldChageMode();


void	InitWorldChangeSystem();
int		IsAllowWorldChange_ForPcEvent();

void	SetWorldChangeMode(WorldChangeSystem::enumWorldChangeSystemMode mode);


#endif 
