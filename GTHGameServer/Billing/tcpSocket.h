








#pragma once

#include "CriticalSection.h"
#include "Billing.h"
#include "Thread.h"









class CRecvBuffer
{
public:
	CRecvBuffer (int packetCount = 512) ;
	~CRecvBuffer () ;

	void	Clear () ;

	bool	Input (unsigned char * data, int size) ;
	bool	Get (PACKET_GAME * packetGame) ;

	size_t	GetDataSize () { return m_dataSize ; }

private:
	const int		m_bufferSize ;
	unsigned char * m_pBuffer ;

	int		m_dataSize ;
	int		m_write ;
	int		m_read ;
	bool	m_init ;

	CCriticalSection m_cs ;

	
	void	RemoveBuffer () ;
};





class CInternetAddress : public SOCKADDR_IN
{
public:
	explicit CInternetAddress (unsigned long address, unsigned short port) ;
} ;





class tcpSocket : public CThread
{
public:
	tcpSocket () ;
	virtual ~tcpSocket () ;

	bool	Create () ;
	bool	Connect (unsigned long addr, unsigned short port) ;
	bool	Send ( PACKET_GAME * pPacket ) ;

	void	Close () ;
	void	SetStatus(BOOL bStatus);													
	BOOL	GetStatus();

	virtual int Run () ;		

	
	CRecvBuffer	m_recv ;
	CRecvBuffer	m_send ;

private:
	BOOL	m_run ;
	SOCKET	m_sock ;

	unsigned long	m_addr ;
	unsigned short	m_port ;
} ;


