
#include "../global_def.h"

 

sCS_FUNC CS_FUNC[] =
{
	
	{ CS_IsMessageWating		,	0 },							
	{ CS_SendMessage			,	1 },							
	{ CS_SendMessageButton		,	2 },							
	{ CS_GetMessageSelect		,	0 },							
	{ CS_CloseMessage			,	0 },							
	{ CS_GetPcEvent				,   1 },							
	{ CS_SetQuestStep			,	2 },							
	{ CS_CheckQuestStep			,	1 },							
	{ CS_GetTutorialNum			,	1 },							
	{ CS_SetTutorialNum			,	1 },							
	{ CS_ChkOpenInvenWindow		,	0 },							
	{ CS_ChkEquipItem			,   0 },							
	{ CS_ChkMoveCamera			,   1 },							
	{ CS_IsSelectObject			,   0 },							
	{ CS_IsRequestNpcMenu		,   0 },							
	{ CS_EndTutorial			,	0 },							
	{ CS_IsOpenSkillWindow		,   0 },							
	{ CS_ChkRegSitDownIcon		,   0 },							
	{ CS_SEND_CHAT_MESSAGE		,   1 },							
	
};







int CS_IsMessageWating( void* pPc, int *pParam )
{
	ClientScript_t* pc = static_cast<ClientScript_t*>(pPc);
	if( pc->msgWaiting ) return( true );
	else return( false );

}








int CS_SendMessage( void* pPc, int *pParam )
{
	ClientScript_t* pc	= static_cast<ClientScript_t*>(pPc);
	int idx				= pParam[0];

	pc->msgWaiting		= true;
	pc->msgFlag			= true;

	pc->sendMsgIdx		= idx;
	pc->pcReplyIdx		= -1;
	pc->replyMsgIdxNumber = 0;

	
	g_ifMng->m_tutorialWin->InitHelp();

	return( true );
}







int CS_SendMessageButton( void* pPc, int *pParam )
{
	ClientScript_t *pc	= static_cast<ClientScript_t*>(pPc);
	int buttonType			= pParam[0];
	int idx					= pParam[1];

	if( buttonType + 1 > pc->replyMsgIdxNumber )
		pc->replyMsgIdxNumber = buttonType + 1;

	pc->replyMsgIdx[buttonType] = idx;

	return( true );
}







int CS_GetMessageSelect( void* pPc, int *pParam )
{
	ClientScript_t *pc	= static_cast<ClientScript_t*>(pPc);	

	if( pc->pcReplyIdx >= 0 )
	{
		
		pc->msgWaiting = false;
		return( pc->pcReplyIdx );
	}
	else return( -1 );
}







int CS_CloseMessage( void* pPc, int *pParam )
{
	ClientScript_t *pc	= static_cast<ClientScript_t*>(pPc);




	



	g_TutorialMng->SetCompleteTutorial(g_TutorialMng->m_NowTutorialNum);

	return( true );
}






int CS_GetPcEvent( void *pPc, int *pParam )
{
	ClientScript_t *pc	= static_cast<ClientScript_t*>(pPc);
	if (pParam[0] == g_pApp->m_myCharacter->event )
	{		
		pc->msgWaiting = false;
		return 1;
	}
	return 0;
}







int CS_GetTutorialNum( void *pPC, int *pParam)
{	
	return ( pParam[0]  == g_TutorialMng->m_NowTutorialNum );
}






int CS_SetTutorialNum( void *pPC, int *pParam)
{	
	
	return g_TutorialMng->m_NowTutorialNum = pParam[0];
}







int CS_ChkOpenInvenWindow( void *pPC, int *pParam)
{
	return g_ifMng->m_itemWin->State();
}






int CS_SetQuestStep( void *pPc, int *pParam )						
{
	ClientScript_t* pc	= static_cast< ClientScript_t* >( pPc );
	int questNo				= pParam[0];
	int questStep			= pParam[1];

	g_TutorialMng->ScriptVar [ questNo ] = questStep;
	pc->msgWaiting = FALSE;


	



	return TRUE;
}






int CS_CheckQuestStep( void *pPc, int *pParam )
{
	

	int questNo			= pParam[0];
	return g_TutorialMng->ScriptVar[ questNo ];
}






int CS_ChkEquipItem( void *pPc, int *pParam )
{	
	if ( g_cgv.myCharacterInfo->equipment[0] > -1|| g_cgv.myCharacterInfo->equipment[1] > -1 )		
		return 1;
	return 0;
}






int CS_ChkMoveCamera( void *pPc, int *pParam)
{		
	switch ( pParam[0] )
	{
	case 1:
		if ( g_input->KeyEvent( DIK_LEFT )  || g_input->KeyEvent( DIK_RIGHT ))
			return 1;
		break;
	case 2:
		if ( g_input->KeyEvent( DIK_UP ) || g_input->KeyEvent( DIK_DOWN ) ) 
			return 1;
		break;
	case 3:
		if ( g_input->KeyEvent( DIK_PRIOR ) || g_input->KeyEvent( DIK_NEXT ) ) 
			return 1;		
		break;
	case 4:		
		if ( g_input->m_isWheeled )
			return 1;
		break;
	case 5:
		if (g_input->GetMouseMode() == INPUT_MOUSE_SCROLL_MODE )
			return 1;
		break;	
	}
	return 0;
	
}






int CS_IsSelectObject(void *pPc, int *pParam)
{
	if ( g_cgv.pTargetChar == NULL ) return 0;
				
	else if ( g_cgv.pTargetChar->entityType == ENTITY_MONSTER )
		return 1;
	
	return 0;
	
}






int CS_IsRequestNpcMenu(void *pPc, int *pParam)
{	
	if ( g_cgv.clickEvent == GTH_CEV_CHAR_NPC_REQUEST)
		return 1;
	
	return 0;
}







int	CS_EndTutorial (void *pPc, int *pParam)
{

	g_TutorialMng->SetTotutirlMode(false);
	g_TutorialMng->SaveHistory("history.his");
	
	return 1;
}







int	CS_IsOpenSkillWindow (void *pPc, int *pParam)
{
	return g_ifMng->m_skillWin->State();
}







int CS_ChkRegSitDownIcon (void *pPc, int *pParam)
{
	for (int i = 0 ;i <MAX_NUMBER_OF_HOTKEY; i++ )		
	{
		int hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[i];
		if( hotIdx < 0 ) continue;
		
		if( g_cgv.myCharacterInfo->hotkeyType[i] == IF_HOTTYPE_ACTION)
		{
			if (g_actionTable[hotIdx].id == ACTION_SIT) return 1;
		}
	}
	return 0;
}







int CS_SEND_CHAT_MESSAGE(void *pPc, int *pParam)
{
	int idx = pParam[0];
	g_ifMng->m_dlgWin->AddChatMessage( g_LPACK.GetMassage(LPACK_TYPE_NORMAL,idx) );
	return 1;
}

