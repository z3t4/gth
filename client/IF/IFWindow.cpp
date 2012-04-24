



#include "../global_def.h"
#include "IFCommon.h"





#define IF_WINDOW_ATTACH_SIZE	10

CIFWindow::CIFWindow( CIFControlNode *pParentControl ) : CIFControl( pParentControl )
{
	m_movingFlag = false;
	m_minimize = false;
	
	
	m_bBeforeState = false;
}

CIFWindow::~CIFWindow()
{

}

void CIFWindow::UpdateMoving( int dx, int dy )
{
	int cID = m_info[0].ID, i;
	int nWindowXPos, nWindowYPos;
	
	int windowXPos[64], windowYPos[64];
	int tx, ty, mx = 9999, my = 9999, attachXIdx = -1, attachYIdx = -1;
	int attachXType, attachYType;

	if( !g_input->GetLBKeep() )
	{
		EndMoving();
	}

	if( !m_movingFlag ) return;

	
	if( dx < 0 ) attachXType = 0;
	else attachXType = 1;

	if( dy < 0 ) attachYType = 0;
	else attachYType = 1;

	
	int a1, a2, b1, b2;

	nWindowXPos = 0;
	nWindowYPos = 0;
	for( i = 0; i < NUM_IF_WINDOW_TYPE; i ++ )
	{
		if( g_ifMng->m_winControl[i] == this ) continue;
		if( !g_ifMng->m_winControl[i]->State() ) continue;

		
		a1 = g_ifMng->m_iPos[cID].clientY;
		a2 = g_ifMng->m_iPos[cID].clientY + g_ifMng->m_iPos[cID].sizeX;
		b1 = g_ifMng->m_winControl[i]->m_info[0].clientY;
		b2 = g_ifMng->m_winControl[i]->m_info[0].clientY + g_ifMng->m_winControl[i]->m_info[0].sizeY;

		if( !( a1 > b2 || a2 < b1 ) )
		{
			if( attachXType ) windowXPos[nWindowXPos++] = g_ifMng->m_winControl[i]->m_info[0].clientX;
			else windowXPos[nWindowXPos++] = g_ifMng->m_winControl[i]->m_info[0].clientX + g_ifMng->m_winControl[i]->m_info[0].sizeX;
		}

		
		a1 = g_ifMng->m_iPos[cID].clientX;
		a2 = g_ifMng->m_iPos[cID].clientX + g_ifMng->m_iPos[cID].sizeX;
		b1 = g_ifMng->m_winControl[i]->m_info[0].clientX;
		b2 = g_ifMng->m_winControl[i]->m_info[0].clientX + g_ifMng->m_winControl[i]->m_info[0].sizeX;

		if( !( a1 > b2 || a2 < b1 ) )
		{
			if( !attachYType ) windowYPos[nWindowYPos++] = g_ifMng->m_winControl[i]->m_info[0].clientY;
			else windowYPos[nWindowYPos++] = g_ifMng->m_winControl[i]->m_info[0].clientY + g_ifMng->m_winControl[i]->m_info[0].sizeY;
		}
	}
	windowXPos[nWindowXPos++] = 0;
	windowXPos[nWindowXPos++] = g_ifMng->m_clientWidth;
	windowYPos[nWindowYPos++] = 0;
	windowYPos[nWindowYPos++] = g_ifMng->m_clientHeight;


	
	if( abs( dx ) < IF_WINDOW_ATTACH_SIZE && dx != 0 )
	{
		if( attachXType == 0 ) tx = g_ifMng->m_iPos[cID].clientX + dx;
		else tx = g_ifMng->m_iPos[cID].clientX + g_ifMng->m_iPos[cID].sizeX + dx;

		for( i = 0; i < nWindowXPos; i ++ )
		{
			if( abs( windowXPos[i] - tx ) < IF_WINDOW_ATTACH_SIZE && abs( windowXPos[i] - tx ) < mx ) 
			{
				mx = abs( windowXPos[i] - tx );
				attachXIdx = i;
			}
		}
		if( attachXIdx < 0 ) 
		{
			g_ifMng->m_iPos[cID].clientX = g_ifMng->m_iPos[cID].clientX + dx;
		}
		else 
		{
			if( attachXType == 0 ) 
			{
				g_ifMng->m_iPos[cID].clientX = windowXPos[attachXIdx];
			}
			else 
			{
				g_ifMng->m_iPos[cID].clientX = windowXPos[attachXIdx] - m_info[0].sizeX;
			}
		}
	}
	else
	{
		g_ifMng->m_iPos[cID].clientX += dx;
	}

	
	if( abs( dy ) < IF_WINDOW_ATTACH_SIZE && dy != 0 )
	{
		if( attachYType == 0 ) ty = g_ifMng->m_iPos[cID].clientY + dy;
		else ty = g_ifMng->m_iPos[cID].clientY + g_ifMng->m_iPos[cID].sizeY + dy;

		for( i = 0; i < nWindowYPos; i ++ )
		{
			if( abs( windowYPos[i] - ty ) < IF_WINDOW_ATTACH_SIZE && abs( windowYPos[i] - ty ) < my ) 
			{
				my = abs( windowYPos[i] - ty );
				attachYIdx = i;
			}
		}
		if( attachYIdx < 0 ) 
		{
			g_ifMng->m_iPos[cID].clientY = g_ifMng->m_iPos[cID].clientY + dy;
		}
		else 
		{
			if( attachYType == 0 ) 
			{
				g_ifMng->m_iPos[cID].clientY = windowYPos[attachYIdx];
			}
			else 
			{
				g_ifMng->m_iPos[cID].clientY = windowYPos[attachYIdx] - m_info[0].sizeY;
			}
		}
	}
	else
	{
		g_ifMng->m_iPos[cID].clientY += dy;
	}

	m_info[0].clientX = g_ifMng->m_iPos[cID].clientX;
	m_info[0].clientY = g_ifMng->m_iPos[cID].clientY;
}
