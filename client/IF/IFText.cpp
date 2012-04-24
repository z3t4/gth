



#include "../global_def.h"
#include "IFCommon.h"






CIFText::CIFText( CIFControlNode *pParentControl ) : CIFControlNode( pParentControl )
{
	m_str[0] = 0;
	m_color = 0xffffffff;
	m_enable = true;
}

CIFText::~CIFText()
{

}

void CIFText::PrepareControl( int id, float diffX, float diffY )
{
	float px, py;
	int pID = -1, cID;
	int tsizex, tsizey;

	m_info.ID = id;

	px = 0.0f;
	py = 0.0f;

	cID = id;
	pID = g_ifMng->m_iPos[cID].parentID;
	
	m_info.parentID = pID;

	tsizex = m_info.sizeX = g_ifMng->m_iPos[id].sizeX;
	tsizey = m_info.sizeY = g_ifMng->m_iPos[id].sizeY;
	
	while( pID >= 0 )
	{
		px += g_ifMng->m_iPos[pID].clientX;
		py += g_ifMng->m_iPos[pID].clientY;

		cID = pID;
		pID = g_ifMng->m_iPos[cID].parentID;
	}

	g_ifMng->m_iPos[id].clientX += diffX;
	g_ifMng->m_iPos[id].clientY += diffY;

	m_info.clientX = g_ifMng->m_iPos[id].clientX + (int)px;
	m_info.clientY = g_ifMng->m_iPos[id].clientY + (int)py;
}

void CIFText::UpdateControl()
{
	float px, py;
	int pID = -1, cID;

	px = 0.0f;
	py = 0.0f;

	cID = m_info.ID;

	if( cID < 0 )
	{
		return;
	}

	pID = g_ifMng->m_iPos[cID].parentID;
	
	
	while( pID >= 0 )
	{
		px += g_ifMng->m_iPos[pID].clientX;
		py += g_ifMng->m_iPos[pID].clientY;

		cID = pID;
		pID = g_ifMng->m_iPos[cID].parentID;
	}
	cID = m_info.ID;
	
	m_info.clientX = g_ifMng->m_iPos[cID].clientX + (int)px;
	m_info.clientY = g_ifMng->m_iPos[cID].clientY + (int)py;

	CIFControlNode::UpdateControl();
}

int CIFText::Selection()
{
	int x1, y1, x2, y2;
	int mx, my;
	
	x1 = m_info.clientX;
	y1 = m_info.clientY;
	x2 = m_info.clientX + m_info.sizeX;
	y2 = m_info.clientY + m_info.sizeY;
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();

	if( x1 <= mx && x2 >= mx && y1 <= my && y2 >= my )
	{
		if( g_input->GetLBDown() ) return( IF_LBDN );
		else if( g_input->GetRBDown() ) return( IF_RBDN );
		else if( g_input->GetLBUp() ) return( IF_LBUP );
		else if( g_input->GetRBUp() ) return( IF_RBUP );
		else if( g_input->GetLBKeep() ) return( IF_LBKEEP );
		else if( g_input->GetRBKeep() ) return( IF_RBKEEP );
		else return( IF_IN_POS );
	}
	
	return( IF_NOT_IN_POS );
}

void CIFText::SetText( char *str )
{
	if(NULL == str){ m_str[0]=NULL; return; }

	strncpy(m_str,str,maxbytes_str);
	m_str[maxbytes_str]=NULL;
	
	
}

void CIFText::SetColor( DWORD color )
{
	m_color = color;
}

void CIFText::Render( int renderType )
{
	if( !State() ) return;

	if( m_str[0] == 0 )  return;

	int x, y, xs, ys;

	x = m_info.clientX;
	y = m_info.clientY;
	xs = m_info.sizeX;
	ys = m_info.sizeY;

	g_chatManager->SetColor( m_color );

	switch( renderType )
	{
	case IF_TEXT_OUTPUT_NORMAL :
		g_chatManager->TextOut( x, y, m_str, CHAT_ARRANGE_NONE, xs );
		break;
	case IF_TEXT_OUTPUT_CENTER :
		g_chatManager->TextOut( x, y, m_str, CHAT_ARRANGE_CENTER, xs );
		break;
	case IF_TEXT_OUTPUT_RIGHT :
		g_chatManager->TextOut( x, y, m_str, CHAT_ARRANGE_RIGHT, xs );
		break;
	case IF_TEXT_OUTPUT_RECT :
		g_chatManager->DrawText( x, y, m_str, xs, 13 );
		break;
	case IF_TEXT_OUTPUT_MAIL :
		g_chatManager->DrawTextMail( x, y, m_str );
		break;
	}
}
