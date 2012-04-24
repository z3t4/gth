



#include "../global_def.h"
#include "IFCommon.h"





CIFHyperLink::CIFHyperLink( CIFControlNode *pParentControl ) : CIFControlNode( pParentControl )
{
	m_enable = true;
	
	m_rootLinkNode = NULL;
	m_pointLinkIndex = -1;

	m_startLine = 0;
	m_maxLineCount = 0;
	m_viewLineCount = 0;

	m_str[0] = 0;
}

CIFHyperLink::~CIFHyperLink()
{
	ClearAllLinkNode();
}

void CIFHyperLink::PrepareControl( int id )
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
	
	m_info.clientX = g_ifMng->m_iPos[id].clientX + (int)px;
	m_info.clientY = g_ifMng->m_iPos[id].clientY + (int)py;
}

void CIFHyperLink::UpdateControl()
{
	float px, py;
	int pID = -1, cID;

	px = 0.0f;
	py = 0.0f;

	cID = m_info.ID;
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

int CIFHyperLink::Selection()
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



int CIFHyperLink::PointInLink()
{
	if( m_rootLinkNode == NULL)
	{
		m_pointLinkIndex = -1;
		return -1;
	}

	linkNode_t *node = m_rootLinkNode;
	int mx, my;
	int linkIndex = 0;

	mx = g_input->GetPosX() - m_info.clientX;
	my = g_input->GetPosY() - m_info.clientY + m_startLine*13;

	while( node != NULL )
	{
		if( node->top >= m_startLine*13 ) 
		{
			if( node->left <= mx && node->right >= mx && node->top <= my && node->bottom >= my )
			{
				m_pointLinkIndex = linkIndex;

				if ( g_input->GetLBUp() )
					return node->linkNumber;
				else
					return -1;
			}

			linkIndex++;
		}
		
		node = node->next;
	}

	m_pointLinkIndex = -1;
	return -1;
}



void CIFHyperLink::AddLinkNode( linkNode_t *node )
{
	linkNode_t *newNode = new linkNode_t;
	linkNode_t *tempNode = m_rootLinkNode;

	newNode->next = NULL;

	memcpy( &newNode->left, &node->left, sizeof(int)*4 );
	newNode->linkNumber = node->linkNumber;

	if( m_rootLinkNode == NULL )
	{
		m_rootLinkNode = newNode;
		return;
	}
	else
	{
		while( tempNode->next != NULL )
			tempNode = tempNode->next;
		
		tempNode->next = newNode;
	}	
}



void CIFHyperLink::ClearAllLinkNode()
{
	if( m_rootLinkNode == NULL) return;

	linkNode_t *node = m_rootLinkNode;
	linkNode_t *next = NULL;

	while( node != NULL )
	{
		next = node->next;
		delete node;
		node = next;
	}

	m_rootLinkNode = NULL;
}



void CIFHyperLink::SetText( char *str )
{
	int texIdx;
	int tx1, tx2, ty1, ty2;
	int rx = 0, ry = 0;

	bool bLinkStart = false;
	linkNode_t node;
	
	wchar_t uniCode;
	unsigned char aniCode[3];
	
	char buffer[4]; 
	bool bDoubleByte = false;

	int xs = m_info.sizeX;
	int ys = m_info.sizeY;
	
	strcpy( m_str, str );

	
	ClearAllLinkNode();

	
	m_startLine = 0;
	m_maxLineCount = 0;
	m_viewLineCount = 0;

	if ( *str == NULL )
		return;

	m_maxLineCount = 1;

	while( *str )
	{		
		memset( aniCode, 0, sizeof( aniCode ) );
		
		memset( buffer, 0, sizeof( buffer ) );

		aniCode[0] = *str;

		tx1 = ty1 = tx2 = ty2 = 0;
		texIdx = -1;

		if( aniCode[0] == '&' )
		{
			aniCode[1] = * ++str;

			if ( aniCode[1] == 's' ) 
			{
				
				memset( &node, 0, sizeof(node) );
				node.linkNumber = -1;

				
				node.left = rx;
				node.top = ry;
				
				bLinkStart = true;
			}
			else if ( aniCode[1] == 'e' ) 
			{
				
				node.right = rx;
				node.bottom = ry + 13;

				if( bLinkStart && node.linkNumber != -1 )
				{
					bLinkStart = false;

					
					AddLinkNode( &node );
				}

				
				memset( &node, 0, sizeof(node) );
				node.linkNumber = -1;
			}
			else if ( aniCode[1] == 'l' ) 
			{
				
				buffer[0] = * ++str;
				buffer[1] = * ++str;
				buffer[2] = * ++str;
				buffer[3] = 0;

				if( bLinkStart )
				{
					node.linkNumber = atoi( buffer );
				}
			}

			

			else if( aniCode[1] == 'n' )
			{
				if (bLinkStart)
				{
					node.right = xs;
					node.bottom = ry + 13;

					
					AddLinkNode( &node );

					bLinkStart = false;
				}

				m_maxLineCount++;

				ry += 13;
				rx = 0;
			}
		}
		
		else if( aniCode[0] == ' ' )
		{
			tx1 = 0;
			tx2 = 6;
		}
		
		else 
		{
			
			if( IsDBCSLeadByte( (BYTE)aniCode[0] ) )
			{
				aniCode[1] = * ++str;
				aniCode[2] = 0;

				bDoubleByte = true;
			}
			else
			{
				aniCode[1] = 0;
			}
			
			MultiByteToWideChar( CP_ACP, MB_COMPOSITE, (char*)aniCode, -1, &uniCode, 1 );

			
			
			texIdx = g_chatManager->FindTexIdx( uniCode );
		}
		if( texIdx >= 0 )
		{
			tx1 = g_chatManager->GetHanTexCoord(texIdx)->tx1;
			ty1 = g_chatManager->GetHanTexCoord(texIdx)->ty1;
			tx2 = g_chatManager->GetHanTexCoord(texIdx)->tx2;
			ty2 = g_chatManager->GetHanTexCoord(texIdx)->ty2;
		}

		str ++;
		
		rx += tx2 - tx1;

		if( rx > xs )
		{
			
			if (bLinkStart)
			{
				node.right = xs;
				node.bottom = ry + 13;

				AddLinkNode( &node );

				
				bLinkStart = false;
			}

			
			if (bDoubleByte)
				str -= 2;
			else
				str --;

			m_maxLineCount++;

			rx = 0;
			ry += 13;
		}
		
		if( ry <= ys )
		{
			m_viewLineCount = m_maxLineCount;
		}
	}

	
}

void CIFHyperLink::SetColor( DWORD color )
{
	m_color = color;
}

void CIFHyperLink::Render( int renderType )
{
	if( !State() ) return;

	if( m_str[0] == 0 )  return;

	int x, y, xs, ys;

	x = m_info.clientX;
	y = m_info.clientY;
	xs = m_info.sizeX;
	ys = m_info.sizeY;

	g_chatManager->SetColor( m_color );
	g_chatManager->SetColor( 0xffffffff );
	g_chatManager->DrawTextRectLineScroll( x, y, m_str, xs, ys, m_startLine, m_pointLinkIndex );

}
