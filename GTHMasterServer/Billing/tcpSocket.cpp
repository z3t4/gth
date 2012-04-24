








#include "..\\global.h"

extern CLogSystem *g_logSystem;







tcpSocket::tcpSocket ()
: m_sock (INVALID_SOCKET),
  m_run (true)
{
}

tcpSocket::~tcpSocket ()
{
	Close () ;
}



void tcpSocket::Close ()
{
	m_run = false ;

	if ( INVALID_SOCKET != m_sock )
	{
		closesocket ( m_sock ) ;
		m_sock = INVALID_SOCKET ;
	}
}



bool tcpSocket::Create ()
{
	
	m_sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP) ;
	if ( INVALID_SOCKET == m_sock )
	{
		return false ;
	}

	return true ;
}



bool tcpSocket::Connect (unsigned long addr, unsigned short port)
{
	if ( INVALID_SOCKET == m_sock )
	{
		return false ;
	}

	CInternetAddress servAddr (addr, port) ;

	int result = connect ( m_sock, (sockaddr*)&servAddr, sizeof (servAddr ) ) ;
	if ( SOCKET_ERROR == result )
	{
		return false ;
	}

	
	m_addr = addr ;
	m_port = port ;

	return true ;
}



int tcpSocket::Run ()
{
	int ret ;
	PACKET_GAME packetGame ;

	while (m_run)
	{
		ret = recv ( m_sock, (char*)&packetGame, sizeof (packetGame), 0 ) ;
		if ( SOCKET_ERROR == ret )
		{
			g_logSystem->Write ( "tcpSocket::Run (), Socket error: %d",
										WSAGetLastError () ) ;
			break ;
		}

		if (!m_recv.Input ((unsigned char*)&packetGame, ret))
		{
			g_logSystem->Write ( "tcpSocket::Run(), "
				"tcpSocket recv buffer overflow" ) ;
		}
	}

	return 0 ;
}

bool tcpSocket::Send (PACKET_GAME * pPacket)
{
	if ( !m_run ) return false ;

	int ret = send ( m_sock, (char*) pPacket, sizeof (PACKET_GAME), 0 ) ;
	if ( SOCKET_ERROR == ret )
	{
		g_logSystem->Write ( "tcpSocket::Send (), "
			"socket error: %d\nRetry connection...", WSAGetLastError () );

		Close () ;		
		Wait (200) ;	

		m_recv.Clear();	

		Create () ;		
		if ( !Connect (m_addr, m_port) )
		{
			g_logSystem->Write ( "tcpSocket::Send (), "
				"Fail to connect server: %d", WSAGetLastError () ) ;

			return false ;
		}

		
		Start () ;

		
		ret = send ( m_sock, (char*) pPacket, sizeof (PACKET_GAME), 0 ) ;
		if ( SOCKET_ERROR == ret )
		{
			g_logSystem->Write ( "tcpSocket::Send (), "
				"Socket error: %d", WSAGetLastError () );

			return false ;
		}
	}

	return true ;
}







CInternetAddress::CInternetAddress (unsigned long address, unsigned short port)
{
	sin_family = AF_INET ;
	sin_port = htons (port) ;
	sin_addr.s_addr = address ;
}







CRecvBuffer::CRecvBuffer (int packetCount)
: m_bufferSize (packetCount * sizeof (PACKET_GAME)),
  m_init (true)
{
	m_pBuffer = new unsigned char [m_bufferSize] ;

	if ( NULL == m_pBuffer )
	{
		m_init = false ;
		RemoveBuffer () ;
	}

	Clear () ;
}



CRecvBuffer::~CRecvBuffer ()
{
	RemoveBuffer () ;
}

void CRecvBuffer::RemoveBuffer ()
{
	if ( NULL != m_pBuffer )
	{
		delete [] m_pBuffer ;
		m_pBuffer = NULL ;
	}
}



void CRecvBuffer::Clear ()
{
	m_dataSize = 0 ;
	m_write = 0 ;
	m_read = 0 ;
}



bool CRecvBuffer::Input (unsigned char *pData, int size)
{
	if ( size + m_dataSize > m_bufferSize || !m_init)
	{
		return false ;
	}

	CCriticalSection::Owner lock (m_cs) ;

	
	if ( m_write + size >= m_bufferSize )
	{
		int part1 = m_bufferSize - m_write ;
		int part2 = size - part1 ;

		memcpy (&m_pBuffer[m_write], pData, part1) ;
		memcpy (m_pBuffer, &pData[part1], part2) ;

		m_write = part2 ;
	}
	else
	{
		memcpy (&m_pBuffer[m_write], pData, size) ;
		m_write += size ;
	}

	m_dataSize += size ;
	return true ;
}



bool CRecvBuffer::Get (PACKET_GAME *packetGame)
{
	if ( sizeof (PACKET_GAME) > m_dataSize )
	{
		return false ;
	}

	CCriticalSection::Owner lock (m_cs) ;

	if ( m_bufferSize - m_read > sizeof (PACKET_GAME) )
	{
		memcpy ( packetGame, &m_pBuffer[m_read], sizeof (PACKET_GAME) ) ;
		m_read += sizeof (PACKET_GAME) ;
	}
	
	else
	{
		int part1 = m_bufferSize - m_read ;
		int part2 = sizeof (PACKET_GAME) - part1 ;

		memcpy ( packetGame, &m_pBuffer[m_read], part1 ) ;
		memcpy ( &packetGame[part1], m_pBuffer, part2 ) ;
	}

	m_dataSize -= sizeof (PACKET_GAME) ;
	return true ;
}


