#pragma hdrstop

#include "d3ddevice.h"

CD3DDevice g_d3dDevice;

CD3DDevice::CD3DDevice()
{
	m_pd3dDevice  = NULL;
	m_pBackBuffer = NULL;
	m_pMainZStencilSurface = NULL;
	m_VShaderVersion      = VS_FFP;
}

void CD3DDevice::ActivateDevice()
{
}

void CD3DDevice::PassivateDevice()
{
	if(m_pBackBuffer)
	{
		m_pBackBuffer->Release();
		m_pBackBuffer = NULL;
	}
	if(m_pMainZStencilSurface)
	{
		m_pMainZStencilSurface->Release();
		m_pMainZStencilSurface = NULL;
	}
}

void CD3DDevice::ReleaseDevice()
{
	m_pd3dDevice = NULL;
}

void CD3DDevice::SetD3DDevice(LPDIRECT3DDEVICE8 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	if(!m_pd3dDevice)
	{
		memset(&m_d3dCaps, 0, sizeof(m_d3dCaps));
		m_VShaderVersion = VS_FFP;
		m_pMainZStencilSurface = NULL;
		return;
	}
	m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);

	
	DWORD vs = m_d3dCaps.VertexShaderVersion;
	if( vs >= D3DVS_VERSION(3,0) )
		m_VShaderVersion = VS_3_0;
	else if( vs >= D3DVS_VERSION(2,0) )
		m_VShaderVersion = VS_2_0;
	else if( vs >= D3DVS_VERSION(1,1) )
		m_VShaderVersion = VS_1_1;
	else
		m_VShaderVersion = VS_FFP;

	DWORD dwCaps = m_d3dCaps.RasterCaps;
    if((dwCaps & D3DPRASTERCAPS_FOGTABLE ) && ( dwCaps & D3DPRASTERCAPS_ZFOG ))
	{
        m_FogType = GTH_PIXEL_FOG_TYPE;
	}
    else if( dwCaps & D3DPRASTERCAPS_FOGVERTEX )
	{
        m_FogType = GTH_VERTEX_FOG_TYPE;
	}
	else
	{
		m_FogType = GTH_NULL_FOG_TYPE;
	}
}
