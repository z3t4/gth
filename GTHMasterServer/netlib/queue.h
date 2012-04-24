#define SENDING_QUEUE_SIZE		3000
#define RECEIVING_QUEUE_SIZE	3000

typedef struct
{
	int socket;
	int len;
	i3packet_t packet;
	sockaddr_in addr;
} i3packetqueue_t;


class CSendingQueue
{
private:
	int m_first;
	int m_last;
	int m_size;
	int m_overflow;

	i3packetqueue_t m_queue[SENDING_QUEUE_SIZE];

public:
	int m_count;
	int m_maxCount;

	CSendingQueue();
	~CSendingQueue();

	int Push (int socket, byte *buf, int len, struct sockaddr_in *addr);
	int Pop (int *socket, byte *buf, int *len, struct sockaddr_in *addr);
	void Clear ();
};


class CReceivingQueue
{
private:
	int m_first;
	int m_last;
	int m_size;
	int m_overflow;

	i3packetqueue_t m_queue[RECEIVING_QUEUE_SIZE];

public:
	int m_count;
	int m_maxCount;

	CReceivingQueue();
	~CReceivingQueue();

	int Push (int socket, byte *buf, int len, struct sockaddr_in *addr);
	int Pop (int *socket, byte *buf, int *len, struct sockaddr_in *addr);
	void Clear ();
};

