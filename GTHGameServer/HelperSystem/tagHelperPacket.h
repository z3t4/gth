

#ifndef _TAGHELPERPACKET_H_
#define _TAGHELPERPACKET_H_

enum enumHelperPacket
{	
		CS_INVITE_Req_fromHELPER					= 0,				
		SC_INVITE_Req_toTAKER						= 1,				
		CS_INVITE_Res_fromTAKER						= 2,				
		SC_INVITE_Res_toHELPER						= 3,				
		SC_INVITE_Res_toTAKER						= 4,
		CS_REMOVE_Req_fromPlayer					= 5,				
		SC_REMOVE_Res_toPlayer						= 6,				
		SC_REMOVE_NOTIFY_toPlayer					= 7,				
		SC_SEND_MEMBERLIST_toAllPlayer				= 8,
		SC_SEND_ADD_toPlayer						= 9,				
		SC_LIST_Res_toServer						= 10,
		SC_LISTINFO_toAllPlayer						= 11,
		CS_GIVEPOINT_Req_fromHelper					= 12,
		SC_UPDATE_POINTINFO_SEND_toPlayer			= 13,
		CS_SPAWN_Req_fromHelper						= 14,
		CS_SPAWN_Res_fromTaker						= 15,
		SC_SPAWN_Req_toTaker						= 16,
		SS_SPAWN_Req_fromServer						= 17,
		SC_SPAWN_Res_toTaker						= 18,    
		SS_SPAWN_Res_fromServer						= 19,
		CS_CONFIRM_MYPOINT_Req_fromPlayer			= 20,
		SC_CONFIRM_MYPOINT_Res_toPlayer				= 21,
		CS_CONFIRM_HELPPOINT_Req_fromPlayer			= 22,
		SC_CONFIRM_HELPPOINT_Res_toPlayer			= 23,
		CS_CONFIRM_WEPPOINT_Req_fromPlayer			= 24,
		SC_CONFIRM_WEPPOINT_Res_toPlayer			= 25,
		SC_STACK_OPEN_Req_toPlayer					= 26,
		CS_MemBerList_Req_fromPlayer				= 27,
		SC_SENDMYPOINT_toAllPlayer					= 28,
		SC_SENDHELPPOINT_toHelper					= 29,
		SS_REPLYMemberList_toServer					= 30,
		SS_REQUESTMemberList_toServer				= 31,
		SS_REPLY_UPDATE_MemberList_toServer			= 32,
		SS_REQUEST_UPDATE_MemberList_toServer		= 33,
		SS_REMOVE_NOTIFY_toPlayer					= 34,	
};


struct tagHelperPacket_SC_INVITE_RES_toHELPER
{
	enum enumCode{
		Success_RegistrationRequest					= 0,
		Fail_RegistrationRequest_invalidUser 		= 1,
		Fail_RegistrationRequest_invalidMode		= 2,
		Fail_RegistrationRequest_invalidTakersCount	= 3,
		Fail_RegistrationRequest_invalidWorldIndex	= 4,
		invalidBusy									= 5,
		Fail_RegistrationRequest_invaliChild		= 6,
		Fail_RegistrationReject						= 7,	
		Ready_Registration							= 8,
		invalidHelpersCount							= 9,
		Fail_RemoveRequest							= 10,
		fail_invite_becourse_sameIndex				= 11,
		fail_invite_lowlevel						= 12,
		Fail_RegistrationRequest_fail_invite_higt	= 13,
		Success_RemoveRequest						= 14,
		Fail_RegistrationRequest_fail_invite_low	= 15,
		Fail_RegistrationRequest_invalidHelperCount	= 16,
		Fail_RegistrationRequest_invalidTakerMode	= 17,
		
	};

};


#endif