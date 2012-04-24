#ifndef _D3DBUFFER_H
#define _D3DBUFFER_H
#include <d3dx8.h>

typedef enum
{
	GTH_VERTEX_BUFFER  = 1 << 0 ,
	GTH_INDEX_BUFFER   = 1 << 1 ,
	GTH_STATIC_BUFFER  = 1 << 2 ,
	GTH_DYNAMIC_BUFFER = 1 << 3 ,
	GTH_WRITEONLY      = 1 << 4 
} GTH_BUFFERTYPE;

typedef enum
{
	GTH_FORMAT_XYZ           = 1 << 0 ,
	GTH_FORMAT_XYZRHW        = 1 << 1 ,
	GTH_FORMAT_NORMAL        = 1 << 2 ,
	GTH_FORMAT_DIFFUSE       = 1 << 3 ,
	GTH_FORMAT_SPECULAR      = 1 << 4 ,
	GTH_FORMAT_TEXVERTEXFLAG = 1 << 16 
} GTH_VERTEXBUFFERFORMAT;

typedef enum
{
	GTH_FORMAT_INDEX16       = 1 << 0 ,
	GTH_FORMAT_INDEX32       = 1 << 1 
} GTH_INDEXBUFFERFORMAT;

#define GTH_FORMAT_TEXVERTEX( size )  ( GTH_FORMAT_TEXVERTEXFLAG | ( size << 8 ) )

class CD3DBuffer
{
public:

	      CD3DBuffer();
	     ~CD3DBuffer();

	void  Release();
	int   Create( int size, DWORD format, DWORD bufType );
	void  Discard();
	BYTE *Lock( const int sizeToLock = 0 );
	void  Unlock();
	int   CheckFreeSize( int sizeToLock );

	int   isLocked()      { return m_isLocked; }
	BYTE *GetBuffer()     { return m_ptrBuffer; }
	int   GetBase()       { return m_base; }
	int   GetStrideSize() { return m_strideSize; }

	LPDIRECT3DVERTEXBUFFER8 GetD3DVertexBuffer() { return m_pD3DVertexBuffer; }
	LPDIRECT3DINDEXBUFFER8  GetD3DIndexBuffer()  { return m_pD3DIndexBuffer; }

private:

	int   CreateIndexBuffer( int size, DWORD format, DWORD bufType );
	int   CreateVertexBuffer( int size, DWORD format, DWORD bufType );
	inline BYTE *LockVertexBuffer( const int sizeToLock );
	inline BYTE *LockIndexBuffer( const int sizeToLock );

	LPDIRECT3DVERTEXBUFFER8  m_pD3DVertexBuffer; 
	LPDIRECT3DINDEXBUFFER8   m_pD3DIndexBuffer;

	DWORD   m_type;
	DWORD   m_format;
	int     m_maxSize;
	int     m_usedSize;
	int     m_strideSize;
	int     m_base;

	BYTE   *m_ptrBuffer;
    int     m_isLocked;   

    unsigned long m_offsetVertex;   
    unsigned long m_strideVertex;   
    unsigned long m_offsetNormal;   
    unsigned long m_offsetDiffuse;  
    unsigned long m_offsetSpecular; 
    unsigned long m_offsetTexVertex; 
    unsigned long m_strideTexVertex; 
    int m_nTexVertex; 
    
};


#endif
