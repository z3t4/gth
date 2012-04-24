



#if !defined(AFX_CTERRABILLINGCTRL_COMMUNICATIONCTRL_H__62234542_B22F_4A33_A2EF_4D23000EC4FB__INCLUDED_)
#define AFX_CTERRABILLINGCTRL_COMMUNICATIONCTRL_H__62234542_B22F_4A33_A2EF_4D23000EC4FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 



class CTerraBillingCtrl_CommunicationCtrl  
{
	private:
		BOOL    m_bOK;		
		SOCKET	m_sock;
		unsigned long  m_addr;
		unsigned short m_port;

		struct tagThreadVar{
			BOOL bLive;
			BOOL bReqExit;			
		};
		struct tagThreadVar m_ThreadVar;

	public:
		enum enumStep{
			step_disconnect_try=0,
			step_disconnect,
			step_connect_try,
			step_connect,
		};
	private:
		enum enumStep m_Step;



	private:
		inline BOOL SocketCreate(void);
		inline void SocketDestory(void);
		inline BOOL Connect(void);		
		inline void Disconnect(void);		

	
	private:
		static unsigned int __stdcall ThreadFunction(void* pV);
		int  RealThreadFunction(void);			

	public:
		BOOL isOK(void) const;				
		BOOL Start(void);
		BOOL Send(const struct tagAddBillPacket& pPacket);
		BOOL TryConnect(const unsigned long addr,const unsigned short port);
		BOOL TryDisconnect(void);		
		enum enumStep GetStep(void);


	
public:
	CTerraBillingCtrl_CommunicationCtrl();
	virtual ~CTerraBillingCtrl_CommunicationCtrl();

};

#endif 
