



#if !defined(AFX_ITEM_SCROLL_PROC_H__7514679D_1C5E_402F_A0C0_E40BDB0F3E26__INCLUDED_)
#define AFX_ITEM_SCROLL_PROC_H__7514679D_1C5E_402F_A0C0_E40BDB0F3E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


class CItem_Scroll_Proc  
{
public:
	enum enumPacketID{

		
		CC_REQUEST_PC_SUMMON=0x0001,
		GSC_REQUEST_PC_SUMMON,
		CC_REPLY_PC_SUMMON,
		GSC_REPLY_PC_SUMMON,

		
		GSC_ERROR_MES,
		GSC_STATE_MES,

		CC_ERROR_MES,
		CC_STATE_MES,

		
		CC_REQUEST_PC_MOVE_SUMMON,
		GSC_REQUEST_PC_MOVE_SUMMON,
		CC_REPLY_PC_MOVE_SUMMON,
		GSC_REPLY_PC_MOVE_SUMMON,

		CC_REDAY_PC_MOVE_SUMMON,
		GSC_REPLY_REDAY_PC_MOVE_SUMMON,

	};


	enum enumErrorCode
	{
		ERROR_NULL_TARGET_PLAYER=1,
		ERROR_ITEM_NOT_HAVE,
		ERROR_INVALID_ITEM,
		ERROR_NOT_SUMMON_ITEM,
		ERROR_NULL_REQUEST_PLAYER,
		
		ERROR_TARGET_PLAYER_NOT_USE_AREA,
		ERROR_REQUEST_PLAYER_NOT_USE_AREA,

		ERROR_NOT_USE_ME_TO_ME,

		ERROR_NOT_MOVE_SUMMON_ITEM,
	};

	enum enumStateCode
	{
		STATE_SEND_REQUEST	=1,

		STATE_TARGET_PLAYER_REFUSE,
		STATE_TARGET_PLAYER_OK,

		STATE_MSGBOX_POPUP_SUCC,
		STATE_MSGBOX_POPUP_FAIL,		

		STATE_TARGET_PLAYER_MOVE_REFUSE,
		STATE_TARGET_PLAYER_MOVE_OK,
	};

	enum enumTargetPlayerReply
	{
		REPLY_OK=1,
		REPLY_REFUSE,
	};

public:
	CItem_Scroll_Proc();	
	virtual ~CItem_Scroll_Proc();

	void		Decoder(playerCharacter_t* pPlayer);
	bool     	CheckInvalidArea(playerCharacter_t* pFromPlayer);
	int			GSDecoder();

	
	int			ClientRequest_PC_Summon(playerCharacter_t* pPlayer);
	int			Client_Reply_PC_Summon(playerCharacter_t* pPlayer);
	int			Client_State_Mes(playerCharacter_t*  pPlayer);
	
	int			OtherServer_CleintRequest_PC_Summon();
	int			Otherserver_from_Reply_Pc_Summon();
	
	int			GsErrorMes();
	int			GsStateMes();	

	
	
	int			ClientRequest_PC_Move_Summon(playerCharacter_t* pPlayer);
	int			OtherServer_CleintRequest_PC_Move_Summon();

	int			Client_Reply_PC_Move_Summon(playerCharacter_t* pPlayer);
	
	
	int			Otherserver_from_Reply_Pc_Move_Summon();
	int			Client_Error_Mes(playerCharacter_t*  pPlayer);
	int			Client_Ready_PC_Move_Summon(playerCharacter_t* pPlayer);

	int			ReplySummons(playerCharacter_t *pc);
};

#endif 
