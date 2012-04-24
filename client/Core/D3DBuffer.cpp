#include "d3dbuffer.h"
#include "core_def.h"
#include "d3ddevice.h"

CD3DBuffer::CD3DBuffer()
{
	m_pD3DVertexBuffer = NULL;
	m_pD3DIndexBuffer = NULL;
	
	
}

CD3DBuffer::~CD3DBuffer()
{
    GTH_SAFE_RELEASE( m_pD3DVertexBuffer );
	GTH_SAFE_RELEASE( m_pD3DIndexBuffer );
}

void CD3DBuffer::Release()
{
    GTH_SAFE_RELEASE( m_pD3DVertexBuffer );
	GTH_SAFE_RELEASE( m_pD3DIndexBuffer );
}

int CD3DBuffer::Create( int size, DWORD format, DWORD bufType )
{
	m_type      = bufType;
	m_format    = format;
	m_maxSize   = size;
	m_usedSize  = 0;
	m_ptrBuffer = NULL;
	m_base      = 0;

    GTH_SAFE_RELEASE( m_pD3DVertexBuffer );
	GTH_SAFE_RELEASE( m_pD3DIndexBuffer );
    
	if( bufType & GTH_VERTEX_BUFFER )
		return CreateVertexBuffer( size , format , bufType );
	else if( bufType & GTH_INDEX_BUFFER )
		return CreateIndexBuffer( size , format , bufType );

	return false;
}

int CD3DBuffer::CreateIndexBuffer( int size, DWORD format, DWORD bufType )
{
    int offset = 0;
	DWORD usageFlag = 0;
	D3DFORMAT d3dFormat = D3DFMT_INDEX16;

    if( format & GTH_FORMAT_INDEX16 )
    {
        d3dFormat = D3DFMT_INDEX16;
        offset = sizeof( WORD );
    }
    else if( format & GTH_FORMAT_INDEX32 )
    {
        d3dFormat = D3DFMT_INDEX32;
        offset = sizeof( DWORD );
    }
	
    m_strideSize = offset;

	if( bufType & GTH_DYNAMIC_BUFFER )
		usageFlag = D3DUSAGE_DYNAMIC;

	if( bufType & GTH_WRITEONLY )
		usageFlag |= D3DUSAGE_WRITEONLY;

    if( FAILED( g_d3dDevice.GetD3DDevice()->CreateIndexBuffer( m_strideSize * m_maxSize , usageFlag 
		                                        , d3dFormat, D3DPOOL_DEFAULT , &m_pD3DIndexBuffer ) ) )
	{
        GTH_ERROR( GTH_ERROR_PRINT , "could not create index buffer" );
		return false;
	}

    m_isLocked = false;
	return true;
}

int CD3DBuffer::CreateVertexBuffer( int size, DWORD format, DWORD bufType )
{
    int offset = 0;
	DWORD usageFlag = 0;
	DWORD d3dFormat = 0;

    if( format & GTH_FORMAT_XYZ )
    {
        d3dFormat = D3DFVF_XYZ;
		m_offsetVertex = 0;
        offset += sizeof( float ) * 3;
    }
    else if( format & GTH_FORMAT_XYZRHW )
    {
        d3dFormat = D3DFVF_XYZRHW;
		m_offsetVertex = 0;
        offset += sizeof( float ) * 4;
    }
	
    if( format & GTH_FORMAT_NORMAL )
    {     
        d3dFormat |= D3DFVF_NORMAL;
        m_offsetNormal = offset;
        offset += sizeof( float ) * 3;        
    }

    if( format & GTH_FORMAT_DIFFUSE )
    {
        d3dFormat |= D3DFVF_DIFFUSE;
        m_offsetDiffuse = offset;
        offset += sizeof( DWORD );
    }

    if( format & GTH_FORMAT_SPECULAR )
    {
        d3dFormat |= D3DFVF_SPECULAR;
        m_offsetSpecular = offset;
        offset += sizeof( DWORD );
    }

    if( format & GTH_FORMAT_TEXVERTEXFLAG )
    {
        int nTexVertex = (format & ~GTH_FORMAT_TEXVERTEXFLAG ) >> 8;
        
        switch( nTexVertex )
        {
        case 0:
            d3dFormat |= D3DFVF_TEX0;
            break;
        case 1:
            d3dFormat |= D3DFVF_TEX1;            
            break;
        case 2:
            d3dFormat |= D3DFVF_TEX2;            
            break;
        case 3:
            d3dFormat |= D3DFVF_TEX3;            
            break;
        case 4:
            d3dFormat |= D3DFVF_TEX4;            
            break;
        case 5:
            d3dFormat |= D3DFVF_TEX5;            
            break;
        case 6:
            d3dFormat |= D3DFVF_TEX6;            
            break;
        case 7:
            d3dFormat |= D3DFVF_TEX7;            
            break;
        case 8:
            d3dFormat |= D3DFVF_TEX8;            
            break;
        default:
            GTH_ERROR( GTH_ERROR_PRINT , "vertex buffer couldn't have more than 8 texture coord" );
            break;
        }

        m_offsetTexVertex = offset;
        m_strideTexVertex = sizeof( float ) * 2;

        offset += m_strideTexVertex * nTexVertex;
    }
    
    m_strideSize = offset;

	if( bufType & GTH_DYNAMIC_BUFFER )
		usageFlag = D3DUSAGE_DYNAMIC;

	if( bufType & GTH_WRITEONLY )
		usageFlag |= D3DUSAGE_WRITEONLY;

    if( FAILED( g_d3dDevice.GetD3DDevice()->CreateVertexBuffer( m_strideSize * m_maxSize , usageFlag 
		                                        , d3dFormat, D3DPOOL_DEFAULT , &m_pD3DVertexBuffer ) ) )
	{
        GTH_ERROR( GTH_ERROR_PRINT , "could not create vertex buffer" );
		return false;
	}

    m_isLocked = false;
	return true;
}


void CD3DBuffer::Discard()
{
	m_usedSize = m_maxSize;
}

BYTE *CD3DBuffer::Lock( const int sizeToLock )
{
	if( m_type & GTH_VERTEX_BUFFER )
		return LockVertexBuffer( sizeToLock );
	else if( m_type & GTH_INDEX_BUFFER )
		return LockIndexBuffer( sizeToLock );

	return NULL;
}

int CD3DBuffer::CheckFreeSize( int sizeToLock )
{
	if( m_usedSize + sizeToLock >= m_maxSize )
		return false;
	
	return true;
}

inline BYTE *CD3DBuffer::LockVertexBuffer( const int sizeToLock )
{
	HRESULT ret;
	
	if( m_usedSize + sizeToLock >= m_maxSize )
		m_usedSize = m_maxSize;
	
	if( sizeToLock == 0 )
	{
		if( m_type & GTH_DYNAMIC_BUFFER )
			ret = m_pD3DVertexBuffer->Lock( 0, 0, (BYTE**) &m_ptrBuffer, D3DLOCK_DISCARD );
		else
			ret = m_pD3DVertexBuffer->Lock( 0, 0, (BYTE**) &m_ptrBuffer, NULL );
		
		m_base = 0;
		m_usedSize = m_maxSize;
	}
	else if( m_usedSize + sizeToLock >= m_maxSize )
	{
		if( m_type & GTH_DYNAMIC_BUFFER )
			ret = m_pD3DVertexBuffer->Lock( 0, m_strideSize * sizeToLock, (BYTE**) &m_ptrBuffer, D3DLOCK_DISCARD );
		else
			ret = m_pD3DVertexBuffer->Lock( 0, m_strideSize * sizeToLock, (BYTE**) &m_ptrBuffer, NULL );

		m_base = 0;
		m_usedSize = sizeToLock;
	}
	else
	{
		if( m_type & GTH_DYNAMIC_BUFFER )
			ret = m_pD3DVertexBuffer->Lock( m_strideSize * m_usedSize, m_strideSize * sizeToLock, (BYTE**) &m_ptrBuffer, D3DLOCK_NOOVERWRITE );
		else
			ret = m_pD3DVertexBuffer->Lock( m_strideSize * m_usedSize, m_strideSize * sizeToLock, (BYTE**) &m_ptrBuffer, NULL );

		m_base = m_usedSize;
		m_usedSize += sizeToLock;
	}
	
	if( ret != D3D_OK )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not Lock vertex buffer" );
	}
	
	m_isLocked = true ;
	return m_ptrBuffer;
	
}

inline BYTE *CD3DBuffer::LockIndexBuffer( const int sizeToLock )
{
	HRESULT ret;

	if( m_usedSize + sizeToLock >= m_maxSize )
		m_usedSize = m_maxSize;

	if( sizeToLock == 0 )
	{
		if( m_type & GTH_DYNAMIC_BUFFER )
			ret = m_pD3DIndexBuffer->Lock( 0, 0, (BYTE**) &m_ptrBuffer, D3DLOCK_DISCARD );
		else
			ret = m_pD3DIndexBuffer->Lock( 0, 0, (BYTE**) &m_ptrBuffer, NULL );
		m_base = 0;
		m_usedSize = m_maxSize;
	}
	else if( m_usedSize + sizeToLock >= m_maxSize )
	{
		if( m_type & GTH_DYNAMIC_BUFFER )
			ret = m_pD3DIndexBuffer->Lock( 0, m_strideSize * sizeToLock, (BYTE**) &m_ptrBuffer
			                             , D3DLOCK_DISCARD );
		else
			ret = m_pD3DIndexBuffer->Lock( 0, m_strideSize * sizeToLock, (BYTE**) &m_ptrBuffer
			                             , NULL );
		m_base = 0;
		m_usedSize = sizeToLock;
	}
	else
	{
		if( m_type & GTH_DYNAMIC_BUFFER )
			ret = m_pD3DIndexBuffer->Lock( m_strideSize * m_usedSize, m_strideSize * sizeToLock 
			                            , (BYTE**) &m_ptrBuffer, D3DLOCK_NOOVERWRITE );
		else
			ret = m_pD3DIndexBuffer->Lock( m_strideSize * m_usedSize, m_strideSize * sizeToLock 
			                            , (BYTE**) &m_ptrBuffer, NULL );
		m_base = m_usedSize;
		m_usedSize += sizeToLock;
	}

	if( ret != D3D_OK )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not Lock index buffer" );
	}

    m_isLocked = true;
    return m_ptrBuffer;
}

void CD3DBuffer::Unlock()
{
	HRESULT ret;

	if( m_type & GTH_VERTEX_BUFFER )
		ret = m_pD3DVertexBuffer->Unlock();
	else if( m_type & GTH_INDEX_BUFFER )
		ret = m_pD3DIndexBuffer->Unlock();

    if( ret != D3D_OK )
	{
		GTH_ERROR( GTH_ERROR_PRINT , "could not UnLock buffer" );
	}

    m_isLocked = false;
    m_ptrBuffer = NULL;
}

