#include "netlib.h"
#include "queue.h"
#include <time.h>

extern i3sizebuf_t		netMessage;
extern i3sizebuf_t		recvMessage;

extern int				msgReadCount;
extern int				msgBadRead;
extern int				readSizePerSecond;

extern CRITICAL_SECTION	spCrit;




void MSG_BeginWriting(i3sizebuf_t *sb)
{
	EnterCriticalSection( &spCrit );
}
void MSG_EndWriting(i3sizebuf_t *sb)
{
	LeaveCriticalSection( &spCrit );
}
void MSG_Clear( i3sizebuf_t *sb )
{
	SZ_Clear( sb );
}

 

void MSG_WriteChar (i3sizebuf_t *sb, int c)
{
	byte    *buf;
	
	buf = SZ_GetSpace (sb, 1);
	buf[0] = c;
}

void MSG_WriteByte (i3sizebuf_t *sb, int c)
{
	byte    *buf;

	buf = SZ_GetSpace (sb, 1);
	buf[0] = c;
}

void MSG_WriteShort (i3sizebuf_t *sb, int c)
{
	byte    *buf;
	
	buf = SZ_GetSpace (sb, 2);
	buf[0] = c&0xff;
	buf[1] = c>>8;
}

void MSG_WriteLong (i3sizebuf_t *sb, int c)
{
	byte    *buf;
	
	buf = SZ_GetSpace (sb, 4);
	buf[0] = c&0xff;
	buf[1] = (c>>8)&0xff;
	buf[2] = (c>>16)&0xff;
	buf[3] = c>>24;
}

void MSG_Write64Int(i3sizebuf_t *sb,const __int64 in_n64Value)
{
	byte* pBuffer=NULL;
	
	pBuffer = SZ_GetSpace (sb, 8);
	pBuffer[0] = (in_n64Value>>0)&0xff;
	pBuffer[1] = (in_n64Value>>8)&0xff;
	pBuffer[2] = (in_n64Value>>16)&0xff;
	pBuffer[3] = (in_n64Value>>24)&0xff;
	pBuffer[4] = (in_n64Value>>32)&0xff;
	pBuffer[5] = (in_n64Value>>40)&0xff;
	pBuffer[6] = (in_n64Value>>48)&0xff;
	pBuffer[7] = (in_n64Value>>56)&0xff;
}

void MSG_WriteString (i3sizebuf_t *sb, char *s)
{
	if (!s)
		SZ_Write (sb, "", 1);
	else
		SZ_Write (sb, s, strlen(s)+1);
}


void MSG_WriteFloat (i3sizebuf_t *sb, float f)
{
	union
	{
		float   f;
		int     l;
	} dat;
	
	
	dat.f = f;
	
	SZ_Write (sb, &dat.l, 4);
}

void MSG_WriteAngle (i3sizebuf_t *sb, float f)
{
	MSG_WriteByte (sb, ((int)f*256/360) & 255);
}

void MSG_WriteVector(i3sizebuf_t *sb, vec3_t v)
{
	MSG_WriteFloat(sb, v[0]);
	MSG_WriteFloat(sb, v[1]);
	MSG_WriteFloat(sb, v[2]);
}





void MSG_ReadVector(vec3_t v)
{
	readSizePerSecond += 16;

	v[0] = MSG_ReadFloat();
	v[1] = MSG_ReadFloat();
	v[2] = MSG_ReadFloat();
}

void MSG_BeginReading (void)
{
	msgReadCount = 0;
	msgBadRead = false;
}

int MSG_ReadChar (void)
{
	readSizePerSecond += 1;

	int     c;
	
	if (msgReadCount+1 > recvMessage.cursize)
	{
		msgBadRead = true;
		return -1;
	}
		
	c = (signed char)recvMessage.data[msgReadCount];
	msgReadCount++;
	
	return c;
}

int MSG_ReadByte (void)
{
	readSizePerSecond += 1;
	int     c;
	
	if (msgReadCount+1 > recvMessage.cursize)
	{
		msgBadRead = true;
		return -1;
	}
		
	c = (unsigned char)recvMessage.data[msgReadCount];
	msgReadCount++;
	
	return c;
}

int MSG_ReadShort (void)
{
	readSizePerSecond += 2;
	int     c;
	
	if (msgReadCount+2 > recvMessage.cursize)
	{
		msgBadRead = true;
		return -1;
	}
		
	c = (short)(recvMessage.data[msgReadCount]
	+ (recvMessage.data[msgReadCount+1]<<8));
	
	msgReadCount += 2;
	
	return c;
}

int MSG_ReadLong (void)
{
	readSizePerSecond += 4;
	int     c;
	
	if (msgReadCount+4 > recvMessage.cursize)
	{
		msgBadRead = true;
		return -1;
	}
		
	c = recvMessage.data[msgReadCount]
	+ (recvMessage.data[msgReadCount+1]<<8)
	+ (recvMessage.data[msgReadCount+2]<<16)
	+ (recvMessage.data[msgReadCount+3]<<24);
	
	msgReadCount += 4;
	
	return c;
}

__int64 MSG_Read64Int (void)
{
	readSizePerSecond += 8;
	__int64     c;
	
	if (msgReadCount+8 > recvMessage.cursize)
	{
		msgBadRead = true;
		return -1;
	}
		
	c = recvMessage.data[msgReadCount]
	+ ((__int64)(recvMessage.data[msgReadCount+1])<<8)
	+ ((__int64)(recvMessage.data[msgReadCount+2])<<16)
	+ ((__int64)(recvMessage.data[msgReadCount+3])<<24)
	+ ((__int64)(recvMessage.data[msgReadCount+4])<<32)
	+ ((__int64)(recvMessage.data[msgReadCount+5])<<40)
	+ ((__int64)(recvMessage.data[msgReadCount+6])<<48)
	+ ((__int64)(recvMessage.data[msgReadCount+7])<<56);
	
	msgReadCount += 8;
	
	return c;
}

char *MSG_ReadString (void)
{
	static char     string[2048];
	int             l,c;
	
	l = 0;
	do
	{
		c = MSG_ReadChar ();
		if (c == -1 || c == 0)
			break;
		string[l] = c;
		l++;
	} while (l < sizeof(string)-1);
	
	string[l] = 0;

	readSizePerSecond += l;
	
	return string;
}


float MSG_ReadFloat (void)
{
	readSizePerSecond += 4;
	union
	{
		byte    b[4];
		float   f;
		int     l;
	} dat;
	
	dat.b[0] =      recvMessage.data[msgReadCount];
	dat.b[1] =      recvMessage.data[msgReadCount+1];
	dat.b[2] =      recvMessage.data[msgReadCount+2];
	dat.b[3] =      recvMessage.data[msgReadCount+3];
	msgReadCount += 4;
	
	return dat.f;   
}


float MSG_ReadAngle (void)
{
	readSizePerSecond += 1;
	return float( MSG_ReadByte() * (360.0/256) );
}





void SZ_Clear (i3sizebuf_t *buf)
{
	buf->cursize = 0;
	buf->overwriting = 0;
}

unsigned char *SZ_GetSpace (i3sizebuf_t *buf, int length)
{
	unsigned char    *data;
	
	if (buf->cursize + length > buf->maxsize)
	{
		buf->overflowed = true;
		SZ_Clear (buf); 
	}

	data = buf->data + buf->cursize;
	buf->cursize += length;
	
	return data;
}

void SZ_Write (i3sizebuf_t *buf, void *data, int length)
{
	memcpy (SZ_GetSpace(buf,length),data,length);         
}

void SZ_Alloc (i3sizebuf_t *buf, int startsize)
{
	buf->maxsize = startsize;
	buf->cursize = 0;
	buf->overwriting = 0;
}

void SZ_Free (i3sizebuf_t *buf)
{
	buf->cursize = 0;
	buf->overwriting = 0;
}

void SZ_Load (i3sizebuf_t *buf, void *data)
{
	memcpy (data, buf->data, buf->cursize);         
}

