



#include "../global_def.h"
#include "..\Game\IFHelperWin.h"
#include "..\Game\HelperSystem.h"





CIFHelperWin::CIFHelperWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_HELPER_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	
	m_closeBtn = new CIFButtonCtl( this );
	m_SpawnBtn = new CIFButtonCtl(this);
	m_DeleteBtn = new CIFButtonCtl(this);
	m_ShutDownBtn = new CIFButtonCtl(this);
	
	m_onlineBar = new CIFControl( this );
	m_offlineBar = new CIFControl( this );
	
	m_nameLevelBar = new CIFControl( this );
	
	Enable( false );

	g_ifMng->AddFocus( this );
	SetWindowType( IF_HELPER_WINDOW_TYPE );

	m_totalOnlineHelperCount = 0;	
	m_totalHelperCount = 0;			
	m_Mode = -1;
	
	m_selectedHelperIdx = -1;
	memset( m_selectedHelperName, 0, NAMESTRING );

	
	m_SpawnBtn->Enable(FALSE);	
	
	
}

CIFHelperWin::~CIFHelperWin() { }

void CIFHelperWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_HELPER_WIN );
	
	
	m_textCtl[IT_HELPER_WIN_TITLE]->PrepareControl( IF_HELPER_WIN_TITLE_TEXT );
	m_textCtl[IT_HELPER_WIN_NAME]->PrepareControl( IF_HELPER_WIN_NAME_TEXT );
	m_textCtl[IT_HELPER_WIN_LEVEL]->PrepareControl( IF_HELPER_WIN_LEVEL_TEXT );	

	m_textCtl[IT_HELPER_WIN_SPAWN_BTN]->PrepareControl(IF_HELPER_WIN_SPAWN_TEXT);
	m_textCtl[IT_HELPER_WIN_DELETE_BTN]->PrepareControl(IF_HELPER_WIN_DELETE_TEXT);
	m_textCtl[IT_HELPER_WIN_SHUTDOWN_BTN]->PrepareControl(IF_HELPER_WIN_SHUTDOWN_TEXT);
	
	
	m_titleBar->PrepareControl( IF_HELPER_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_HELPER_WIN_TITLE_BAR_BOTTOM );
	
	
	m_closeBtn->PrepareControl( IF_HELPER_WIN_CLOSE_BTN_0 );
	m_SpawnBtn->PrepareControl(IF_HELPER_WIN_SPAWN_BTN_0);
	m_DeleteBtn->PrepareControl(IF_HELPER_WIN_DELETE_BTN_0);
	m_ShutDownBtn->PrepareControl(IF_HELPER_WIN_SHUTDOWN_BTN_0);

	m_onlineBar->PrepareControl( IF_HELPER_WIN_ONLINE_BAR );
	m_offlineBar->PrepareControl( IF_HELPER_WIN_OFFLINE_BAR );
	
	m_nameLevelBar->PrepareControl( IF_HELPER_WIN_NAMELEVEL_BAR );
}

void CIFHelperWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	
	m_closeBtn->Render();

	m_SpawnBtn->Render();
	m_DeleteBtn->Render();
	m_ShutDownBtn->Render();
	
	m_nameLevelBar->Render();
	
	int i, idx = 1;
	for( i = 0; i < MAX_TAKER_LIST; i ++ )
	{
		if( g_HelperSystem.m_HelperEntry[i].name[0] != 0 )
		{
			if( g_HelperSystem.m_HelperEntry[i].worldIdx == -1 || g_HelperSystem.m_HelperEntry[i].worldIdx == 255 )
			{
				m_offlineBar->m_info[0].clientY = m_textCtl[IT_HELPER_WIN_NAME]->GetYPos() + 15*idx;
				m_offlineBar->UpdateBuffer();
				m_offlineBar->Render();
			}
			else
			{
				m_onlineBar->m_info[0].clientY = m_textCtl[IT_HELPER_WIN_NAME]->GetYPos() + 15*idx;
				m_onlineBar->UpdateBuffer();
				m_onlineBar->Render();
			}
			
			idx++;
		}
	}
	
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_HELPER_WIN_TITLE]->Render();
		m_textCtl[IT_HELPER_WIN_NAME]->Render();
		m_textCtl[IT_HELPER_WIN_LEVEL]->Render();

		
		

		m_textCtl[IT_HELPER_WIN_DELETE_BTN]->Render();
		m_textCtl[IT_HELPER_WIN_SHUTDOWN_BTN]->Render();
		
		char szTemp[32+1];
		int x, y, xs, ys;
		idx = 1;
		
		y = m_textCtl[IT_HELPER_WIN_NAME]->GetYPos();
		xs = m_textCtl[IT_HELPER_WIN_NAME]->GetXSize();
		ys = m_textCtl[IT_HELPER_WIN_NAME]->GetYSize();
		
		for( i = 0; i < MAX_TAKER_LIST; i ++ )
		{
			if( g_HelperSystem.m_HelperEntry[i].name[0] != 0 )
			{
				if( m_selectedHelperIdx == idx)
					g_chatManager->SetColor( 0xff0000ff );
				else
				{
					if(g_HelperSystem.m_HelperEntry[i].bDeleteMe == TRUE)
						g_chatManager->SetColor(D3DCOLOR_ARGB(255, 255, 0, 0));
					else
						g_chatManager->SetColor( 0xffffffff );
				}	
				
				x = m_textCtl[IT_HELPER_WIN_NAME]->GetXPos();
				g_chatManager->DrawText( x, y + idx * 15, g_HelperSystem.m_HelperEntry[i].name );
				
				if( g_HelperSystem.m_HelperEntry[i].worldIdx != -1 && g_HelperSystem.m_HelperEntry[i].worldIdx != 255 )
				{
					x = m_textCtl[IT_HELPER_WIN_LEVEL]->GetXPos();
					_snprintf( szTemp, 32, "Lv.%d", g_HelperSystem.m_HelperEntry[i].level );
					szTemp[32]=NULL;
					g_chatManager->DrawText( x, y + idx * 15, szTemp );
				}
				idx++;
			}
		}
	}
	g_chatManager->EndFrame();
}

int CIFHelperWin::WindowFrame()
{
	char szTemp[256+1];
	
	
	if(m_Mode == 0)
	{
		
		_snprintf(szTemp, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,326));
		szTemp[256]=NULL;
		m_textCtl[IT_HELPER_WIN_TITLE]->SetText( szTemp );
		
		_snprintf(szTemp, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,327), m_totalOnlineHelperCount, m_totalHelperCount);
		szTemp[256]=NULL;
		m_textCtl[IT_HELPER_WIN_NAME]->SetText( szTemp );
		m_SpawnBtn->SetButtonState(IF_BTN_TYPE_ENABLE);
	}
	else
	{
		
		_snprintf(szTemp, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,328));
		szTemp[256]=NULL;
		m_textCtl[IT_HELPER_WIN_TITLE]->SetText( szTemp );
		
		_snprintf(szTemp, 256, g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,329), m_totalOnlineHelperCount, m_totalHelperCount);
		szTemp[256]=NULL;
		m_textCtl[IT_HELPER_WIN_NAME]->SetText( szTemp );
		m_SpawnBtn->SetButtonState(IF_BTN_TYPE_DISABLE);
	}

	_snprintf( szTemp, 256, "Lv" );
	szTemp[256]=NULL;
	m_textCtl[IT_HELPER_WIN_LEVEL]->SetText( szTemp );

	
	
	

	m_textCtl[IT_HELPER_WIN_SPAWN_BTN]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL2,301));
	m_textCtl[IT_HELPER_WIN_DELETE_BTN]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL,244));
	m_textCtl[IT_HELPER_WIN_SHUTDOWN_BTN]->SetText(g_LPACK.GetMassage(LPACK_TYPE_NORMAL,936));
	
	m_textCtl[IT_HELPER_WIN_NAME]->SetColor( 0xffffffff );
	m_textCtl[IT_HELPER_WIN_LEVEL]->SetColor( 0xffffffff );
	
	return( IF_CTL_NONE );
}

int CIFHelperWin::WindowProcess()
{
	char szTemp[256+1];
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}

	if(m_ShutDownBtn->ProcessButton() == IF_CTL_SELECT)
	{
		Enable(false);
		return IF_CTL_SELECT;
	}

	if(m_SpawnBtn->ProcessButton() == IF_CTL_SELECT)
	{
		g_HelperSystem.GTH_SendMessage_HelpSystem_SpawnTaker(m_selectedHelperName);
		return IF_CTL_SELECT;
	}
	
	if(m_DeleteBtn->ProcessButton() == IF_CTL_SELECT)
	{
		g_HelperSystem.GTH_SendMessage_HelpSystem_RemovePlayer(m_selectedHelperName);
		return IF_CTL_SELECT;
	}
	
	int mx, my;
	int x1, y1, xs, ys, i, idx;
	
	idx = 1;
	x1 = m_textCtl[IT_HELPER_WIN_NAME]->GetXPos();
	xs = m_textCtl[IT_HELPER_WIN_NAME]->GetXSize();
	ys = m_textCtl[IT_HELPER_WIN_NAME]->GetYSize();
	
	for( i=0; i < MAX_TAKER_LIST; i++ )
	{
		if( g_HelperSystem.m_HelperEntry[i].name[0] != '\0' )
		{
			y1 = m_textCtl[IT_HELPER_WIN_NAME]->GetYPos() + idx * 15;
			mx = g_input->GetPosX();
			my = g_input->GetPosY();
			mx = mx - x1;
			my = my - y1;
			
			if( mx < 0 || my < 0 || mx > xs || my > ys ) 
			{
			
				
				
			}
			else
			{
				if( g_input->GetLBDown() )
				{
					
					

					if( g_HelperSystem.m_HelperEntry[i].worldIdx >= 0)
					{
						m_selectedHelperIdx = idx;
						strncpy( m_selectedHelperName, g_HelperSystem.m_HelperEntry[i].name, NAMESTRING);
						m_selectedHelperName[NAMESTRING]=NULL;
					}
				}
				
				if( g_HelperSystem.m_HelperEntry[i].worldIdx != -1 && g_HelperSystem.m_HelperEntry[i].worldIdx != 255 )
				{
					g_ifMng->InitRolloverText();
					
					
					g_ifMng->AddRolloverText( g_LPACK.GetMessage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_PC_JOB, g_HelperSystem.m_HelperEntry[i].job ) );
					
					_snprintf( szTemp, 256, "(%s)", g_LPACK.GetMessage( 0, 723 ) );
					
					if( g_HelperSystem.m_HelperEntry[i].worldIdx == -1 || g_HelperSystem.m_HelperEntry[i].worldIdx == 255 )
						_snprintf( szTemp, 256, "(%s)", g_LPACK.GetMessage( 0, 723 ) );
					
					else
						_snprintf( szTemp, 256, "(%s)", g_cgv.worldTable[ g_HelperSystem.m_HelperEntry[i].worldIdx ].hanName );
					
					szTemp[256]=NULL;
					g_ifMng->AddRolloverText( szTemp );
					
					g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true );
					
				}
				
				break;
			}
			
			idx++;
		}
	}
	
	return IF_CTL_NONE;
}

int CIFHelperWin::WindowSelection()
{
	int ret=-1;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );
	
	return( IF_CTL_NONE );
}

void CIFHelperWin::AddHelper(HelperEntry* newHelper)
{
	for( int i=0; i < MAX_TAKER_LIST; i++ )
	{
		if(g_HelperSystem.m_HelperEntry[i].name[0] == 0)
		{
			g_HelperSystem.m_HelperEntry[i].job = newHelper->job;
			g_HelperSystem.m_HelperEntry[i].level = newHelper->level;
			strncpy(g_HelperSystem.m_HelperEntry[i].name, newHelper->name, NAMESTRING);
			g_HelperSystem.m_HelperEntry[i].name[NAMESTRING]=NULL;
			g_HelperSystem.m_HelperEntry[i].worldIdx = newHelper->worldIdx;
			
			m_totalHelperCount++;
			if(g_HelperSystem.m_HelperEntry[i].worldIdx != -1 && g_HelperSystem.m_HelperEntry[i].worldIdx != 255 )
			{
				m_totalOnlineHelperCount++;
			}
			break;
		}
	}
}

void CIFHelperWin::RemoveHelper(char *HelperName)
{
	for( int i=0; i < MAX_TAKER_LIST; i++ )
	{
		if(g_HelperSystem.m_HelperEntry[i].name[0] != 0 )
		{
			if( strcmp(g_HelperSystem.m_HelperEntry[i].name, HelperName) == 0 )
			{
				g_HelperSystem.m_HelperEntry[i].job = 0;
				g_HelperSystem.m_HelperEntry[i].level = 0;
				memset(g_HelperSystem.m_HelperEntry[i].name, 0, NAMESTRING );
				
				if(g_HelperSystem.m_HelperEntry[i].worldIdx != -1 && g_HelperSystem.m_HelperEntry[i].worldIdx != 255 )
					m_totalOnlineHelperCount--;
				
				g_HelperSystem.m_HelperEntry[i].worldIdx = -1;
				
				m_totalHelperCount--;
				break;
			}
		}
	}
}


void CIFHelperWin::UpdateHelper(HelperEntry* oldHelper)
{
	for( int i=0; i< MAX_TAKER_LIST; i++ )
	{
		if(g_HelperSystem.m_HelperEntry[i].name[0] != 0 )
		{
			if( strcmp(g_HelperSystem.m_HelperEntry[i].name, oldHelper->name) == 0 )
			{
				g_HelperSystem.m_HelperEntry[i].job = oldHelper->job;
				g_HelperSystem.m_HelperEntry[i].level = oldHelper->level;
				strncpy(g_HelperSystem.m_HelperEntry[i].name, oldHelper->name, NAMESTRING);
				g_HelperSystem.m_HelperEntry[i].name[NAMESTRING] = NULL;
					
				if(g_HelperSystem.m_HelperEntry[i].worldIdx != -1 && g_HelperSystem.m_HelperEntry[i].worldIdx != 255 )
				{
					if( oldHelper->worldIdx == -1 || oldHelper->worldIdx == 255 )
						m_totalOnlineHelperCount--;
				}
				else
				{
					if( oldHelper->worldIdx != -1 && oldHelper->worldIdx != 255 )
						m_totalOnlineHelperCount++;
				}
				g_HelperSystem.m_HelperEntry[i].worldIdx = oldHelper->worldIdx;
				break;
			}
		}
	}
}
