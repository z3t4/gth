#ifndef _D3DDEVICE_H
#define _D3DDEVICE_H

#include <d3dx8.h>

#define GTH_NULL_FOG_TYPE   0
#define GTH_VERTEX_FOG_TYPE 1
#define GTH_PIXEL_FOG_TYPE  2

class CD3DDevice
{
public:
	enum { VS_FFP = 0,
		   VS_1_1,
		   VS_2_0,
		   VS_3_0 };

	CD3DDevice();

	LPDIRECT3DDEVICE8 GetD3DDevice() const { return m_pd3dDevice; }
	bool IsDevice() const { return m_pd3dDevice != NULL; }
	void SetD3DDevice(LPDIRECT3DDEVICE8 pd3dDevice);
	void ReleaseDevice();
	void ActivateDevice();
	void PassivateDevice();

	
	

	const D3DCAPS8& GetCaps() const   { return m_d3dCaps; }
	
	
	int GetVShaderVersion() const     { return m_VShaderVersion; }
	int GetFogType() const            { return m_FogType; }
	
	

	int GetScreenWidth() const        { return m_ScreenWidth; }
	int GetScreenHeight() const       { return m_ScreenHeight; }
	D3DFORMAT GetScreenFormat() const { return m_ScreenFormat; }
	float GetScreenAspect() const             { return m_ScreenAspect; }
	LPDIRECT3DSURFACE8 GetBackBuffer() const  { return m_pBackBuffer; }
	LPDIRECT3DSURFACE8 GetMainZStencilSurface() const { return m_pMainZStencilSurface; }

	void SetScreen(int width, int height, D3DFORMAT format) 
	{ 
		m_ScreenWidth  = width;
	    m_ScreenHeight = height;
		m_ScreenFormat = format;
		m_ScreenAspect = (float)width / (float)height;
	}

private:
	int			m_VShaderVersion;
	int			m_ScreenWidth;
	int			m_ScreenHeight;
	D3DFORMAT	m_ScreenFormat;
	float		m_ScreenAspect;

    D3DCAPS8    m_d3dCaps;                  
    LPDIRECT3DDEVICE8  m_pd3dDevice;        
	LPDIRECT3DSURFACE8 m_pBackBuffer;
	LPDIRECT3DSURFACE8 m_pMainZStencilSurface;
    DWORD              m_dwCreateFlags;     
	int                m_FogType;
};

extern CD3DDevice g_d3dDevice;

#endif
