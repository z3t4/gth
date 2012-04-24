#include "netlib.h"
#include "queue.h"







CSendingQueue::CSendingQueue()
{
	m_first = 0;
	m_last = -1;
	m_count = 0;
	m_size = SENDING_QUEUE_SIZE;
	m_overflow = false;
	m_maxCount = 0;

	memset( m_queue , 0 , sizeof(i3packetqueue_t) * SENDING_QUEUE_SIZE );
}





CSendingQueue::~CSendingQueue()
{
}




int CSendingQueue::Push (int socket, byte *buf, int len, struct sockaddr_in *addr)
{
	if (m_count >= m_size) 
	{
		m_overflow = true;
		
		return 0;
	}
	if (m_last < m_size - 1)
		m_last ++;
	else 
		m_last = 0;

	m_queue[m_last].socket = socket;
	m_queue[m_last].len = len;
	memcpy( &m_queue[m_last].packet, buf, len );
	memcpy( &m_queue[m_last].addr, addr, sizeof(sockaddr_in) );




	m_count ++;
	if (m_count > m_maxCount) 
		m_maxCount = m_count;

	return 1;
}





int CSendingQueue::Pop (int *socket, byte *buf, int *len, struct sockaddr_in *addr)
{
	if (m_count <= 0)
	{
		
		return 0;
	}

	*socket = m_queue[m_first].socket;
	*len = m_queue[m_first].len;
	memcpy( buf, &m_queue[m_first].packet, m_queue[m_first].len );
	memcpy( addr, &m_queue[m_first].addr, sizeof(sockaddr_in) );

	if (m_first < m_size - 1)
		m_first ++;
	else
		m_first = 0;

	m_count --;

	return 1;
}




void CSendingQueue::Clear()
{
	CSendingQueue();
}







CReceivingQueue::CReceivingQueue()
{
	m_first = 0;
	m_last = -1;
	m_count = 0;
	m_size = RECEIVING_QUEUE_SIZE;
	m_overflow = false;
	m_maxCount = 0;

	memset( m_queue , 0 , sizeof(i3packetqueue_t) * RECEIVING_QUEUE_SIZE );
}



CReceivingQueue::~CReceivingQueue()
{
}




int CReceivingQueue::Push (int socket, byte *buf, int len, struct sockaddr_in *addr)
{
	if (m_count >= m_size) 
	{
		m_overflow = true;
		
		return 0;
	}
	if (m_last < m_size - 1)
		m_last ++;
	else 
		m_last = 0;

	m_queue[m_last].socket = socket;
	m_queue[m_last].len = len;
	memcpy( &m_queue[m_last].packet, buf, len );
	memcpy( &m_queue[m_last].addr, addr, sizeof(sockaddr_in) );

	m_count ++;
	if (m_count > m_maxCount) 
		m_maxCount = m_count;

	return 1;
}





int CReceivingQueue::Pop (int *socket, byte *buf, int *len, struct sockaddr_in *addr)
{
	if (m_count <= 0)
	{
		
		return 0;
	}

	*socket = m_queue[m_first].socket;
	*len = m_queue[m_first].len;
	memcpy( buf, &m_queue[m_first].packet, m_queue[m_first].len );
	memcpy( addr, &m_queue[m_first].addr, sizeof(sockaddr_in) );

	if (m_first < m_size - 1)
		m_first ++;
	else
		m_first = 0;

	m_count --;

	return 1;
}




void CReceivingQueue::Clear()
{
	CReceivingQueue();
}




