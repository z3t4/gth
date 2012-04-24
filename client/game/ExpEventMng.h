

#ifndef _EXPEVENTMNG_H_
#define	_EXPEVENTMNG_H_


class CExpRateMng
{
private :
	enum enumProtocal
	{
		CC_BROAD_CASE_SET_EVENT		= 0x0001,
		SS_BROAD_CASE_SET_EVENT		= 0x0002,
		SS_SET_EVENT				= 0x0003,

		SS_REQUEST_EVENT			= 0x0004,
		SS_REPLY_EVENT				= 0x0005,
		
		CC_SET_ACTIVE_SYSTEM		= 0x0006,
		SS_SET_ACTIVE_SYSTEM		= 0x0007,

		CC_REQUEST_EVENT_STATUS		= 0X0008,
		GSC_REPLY_EVENT_STATUS		= 0X0009,
	};

public:

	enum enumEventType
	{
		EXP_RATE,
		ITEM_DROP_RATE,
		NAK_DROP_RATE,
		GENCAPABLITY_RATE,
		ALL_READ_LOGIC,
	};

public:
	void ProcessMessage();
	void SendMessage_SetEvent(enum enumEventType eventTYpe, float fvalue );
	void SendMessage_SetEventActive(BOOL Active = FALSE);
	void SendMessage_RequestEventStatus();

	void Processage_ReplyEventStatus();

public:
	CExpRateMng();
	~CExpRateMng();

};

#endif


