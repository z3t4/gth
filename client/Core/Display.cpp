#include <d3dx8.h>
#include "display.h"
#include "d3ddevice.h"
#include "../common/d3dapp.h"
#include "../math/math.h"

CDisplay g_display;

CDisplay::CDisplay()
{
	m_gammaRampType = 1;
}

CDisplay::~CDisplay()
{
}

void CDisplay::SetupGamma()
{
	D3DGAMMARAMP ramp;
	for(int i = 0; i < 256; i++)
	{
		int x = fast_round(	(float)( ( m_DisplayParams.Contrast + 0.5 ) * 
									 pow( i/255.0, 1.0/m_DisplayParams.Gamma ) * 65535.0 +
									 ( m_DisplayParams.Brightness - 0.5 ) * 32768.0 - 
									 m_DisplayParams.Contrast * 32768.0 + 16384.0 ) );
		
		WORD val = x < 0 ? 0 : x<65535 ? x : 65535;

		ramp.red[i]   = val;
		ramp.green[i] = val;
		ramp.blue[i]  = val;
	}

	
#ifdef _DEBUG
	return;
#endif

	
	switch( m_gammaRampType )
	{
	case 1:
		g_d3dDevice.GetD3DDevice()->SetGammaRamp( D3DSGR_CALIBRATE, &ramp );
		break;
	case 2:
		{
			HDC hDC;
			hDC = GetDC( NULL );
			SetDeviceGammaRamp( hDC, &ramp );
			ReleaseDC( NULL, hDC );
		}
		break;
	default:
		g_d3dDevice.GetD3DDevice()->SetGammaRamp( D3DSGR_CALIBRATE, &ramp );
		break;
	}

}
