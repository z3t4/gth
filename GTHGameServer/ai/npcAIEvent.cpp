#include "../global.h"

void NPC_InitAI( playerCharacter_t* pc, int npcIdx )
{
	monsterCharacter_t* npc = &g_monster[npcIdx];
	if( npc->entityType != ENTITY_NPC ) return;
	pc->npcIdx = npcIdx;
	
	pc->npcAi.aiType = npc->npcAi.aiType;
	memcpy( &pc->npcAi.aiData, &npc->npcAi.aiData, sizeof( pc->npcAi.aiData ) );
	memcpy( &pc->npcAi.aiVariable, &npc->npcAi.aiVariable, sizeof( pc->npcAi.aiVariable ) );
	memcpy( &pc->npcAi.aiProcedureAddr, &npc->npcAi.aiProcedureAddr, sizeof( pc->npcAi.aiProcedureAddr ) );
	
	
	pc->npcAi.aiEvent = AI_NPC_RUN;
	pc->npcAi.saveTime = 0;
	pc->npcAi.thinkTime = 0;
	pc->npcAi.aiAddr = 0;
	
	pc->npcAi.callProcedureAddr = 0;
	pc->npcAi.isCall = false;
	
	pc->npcAi.pcReplyIdx = -1;

	pc->npcAi.msgWaiting = false;
	pc->npcAi.msgFlag = false;
	pc->npcAi.sendMsgIdx = 0;
	pc->npcAi.replyMsgIdxNumber = 0;
	pc->npcAi.replyMsgIdx[0] = 0;
	pc->npcAi.replyMsgIdx[1] = 0;
	pc->npcAi.replyMsgIdx[2] = 0;
	pc->npcAi.replyMsgIdx[3] = 0;
	pc->npcAi.replyMsgIdx[4] = 0;
	pc->npcAi.globalData[0] = 0;
	pc->npcAi.globalData[1] = 0;
	pc->npcAi.globalData[2] = 0;
	pc->npcAi.globalData[3] = 0;
	pc->npcAi.globalData[4] = 0;

	
	GTH_AddNoticePC( pc, ENTITY_NPC, npcIdx, BUSY_STATE_NPC );
}

void NPC_SendEvent( playerCharacter_t *pc )
{
	if( !pc->npcAi.msgFlag || pc->npcIdx == -1 ) return;
	GTH_SendMessage_NpcMessage( pc );

	pc->npcAi.msgFlag = false;
	pc->npcAi.replyMsgIdxNumber = 0;

}

int NPC_AddNoticePC( monsterCharacter_t* monster, playerCharacter_t* pc )
{
	int noticeIdx = monster->npc.noticePcNumber;

	if( noticeIdx >= monster->npc.maxNoticePcNumber ) return( false );

	monster->event = GTH_EV_CHAR_IDLE;
	monster->ai.aiEvent = AI_IDLE;

	for( int i = 0; i < noticeIdx; i ++ )
	{
		if( monster->npc.noticePcIdx[i] < 0 )
			break;
	}

	monster->npc.noticePcIdx[i] = pc->idx;
	if( i == noticeIdx ) monster->npc.noticePcNumber ++;

	return( true );
}

int NPC_DeleteNoticePC( monsterCharacter_t* monster, playerCharacter_t* pc )
{
	int noticeIdx = monster->npc.noticePcNumber;
	int i;

	GTH_DeleteNoticePC( pc );
	pc->npcIdx = -1;

	if( noticeIdx < 0 ) return( false );
	for( i = 0; i < noticeIdx; i ++ )
	{
		if( monster->npc.noticePcIdx[i] == pc->idx )
		{
			monster->npc.noticePcIdx[i] = -1;
			break;
		}
	}
	NPC_CheckNoticePC( monster );
	return( true );
}

int NPC_CheckNoticePC( monsterCharacter_t* monster )
{
	int noticeIdx = monster->npc.noticePcNumber;
	int i, n = 0, pcIdx;

	for( i = 0; i < MAX_NUMBER_OF_NPC_NOTICE_PC; i ++ )
	{
		pcIdx = monster->npc.noticePcIdx[i];

		if( pcIdx < 0 || pcIdx >= MAX_PCS )
		{
			monster->npc.noticePcIdx[i] = -1;
			continue;
		}

		if( g_pc[pcIdx].active ) 
			n ++;
		else 
			monster->npc.noticePcIdx[i] = -1;
	}
	monster->npc.noticePcNumber = n;
	return( true );
}
