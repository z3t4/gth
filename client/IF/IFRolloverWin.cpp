



#include "../global_def.h"
#include "IFCommon.h"





CIFRolloverWin::CIFRolloverWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	m_win = new CIFControl( this );
	m_titleLeft = new CIFControl( this );
	m_titleRight = new CIFControl( this );
	m_titleCenter = new CIFControl( this );
	m_titleLeftSide = new CIFControl( this );
	m_titleRightSide = new CIFControl( this );
	m_titleBottom = new CIFControl( this );

	Enable( false );

	m_str[0] = 0;
}

CIFRolloverWin::~CIFRolloverWin()
{

}

void CIFRolloverWin::PrepareControl()
{
	m_win->PrepareControl( IF_ROLLOVER_WIN );
	m_titleLeft->PrepareControl( IF_ROLLOVER_WIN_TITLE_LEFT );
	m_titleRight->PrepareControl( IF_ROLLOVER_WIN_TITLE_RIGHT );
	m_titleCenter->PrepareControl( IF_ROLLOVER_WIN_TITLE_CENTER );
	m_titleLeftSide->PrepareControl( IF_ROLLOVER_SIDE );
	m_titleRightSide->PrepareControl( IF_ROLLOVER_SIDE );
	m_titleBottom->PrepareControl( IF_ROLLOVER_BOTTOM );
}

int CIFRolloverWin::WindowFrame()
{
	if( !State() ) return( IF_CTL_NONE );
	return IF_CTL_NONE;
}

void CIFRolloverWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	m_win->Render();
	m_titleLeft->Render();
	m_titleRight->Render();
	m_titleCenter->Render();
	m_titleLeftSide->Render();
	m_titleRightSide->Render();
	m_titleBottom->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->SetColor( 0xffffffff );
	g_chatManager->BeginFrame();
	{
		g_chatManager->DrawText( m_outputTextX, m_outputTextY, m_str, m_outputTextXSize, 13, m_saveArragneType );
	}
	g_chatManager->EndFrame();
}

int CIFRolloverWin::WindowSelection()
{
	return IF_CTL_NONE;
}

int CIFRolloverWin::WindowProcess()
{
	return IF_CTL_NONE;
}

void CIFRolloverWin::MakeRolloverWindow( int x, int y, char *str, int forcedWinFlag, int xsize, int ysize, int arrangeType )
{
	m_saveArragneType = arrangeType;

	Enable();

	m_win->Enable( false );
	m_titleLeft->Enable( false );
	m_titleRight->Enable( false );
	m_titleCenter->Enable( false );
	m_titleLeftSide->Enable( false );
	m_titleRightSide->Enable( false );
	m_titleBottom->Enable( false );

	int sx, sy, rolloverType = 0;
	int tx, ty, nsx, nsy;

	
	
	xsize = -1;
	
	g_chatManager->GetTextRect( xsize, 13, str, sx, sy );
	if( sx <= 0 || sy <= 0 ) return;
	
	
	if( sx < m_titleBottom->m_info[0].sizeX ) 
	{
		rolloverType = 1;
		if( forcedWinFlag ) sx = m_titleBottom->m_info[0].sizeX;
	}
	if( sx > xsize && xsize > 0 ) 
	{
		sx = xsize;
		sy = g_chatManager->GetTextRectLine( sx, 13, str );
	}
	if( !forcedWinFlag ) rolloverType = 1;

	
	tx = sx;
	ty = sy;

	if( x - sx < 0 ) x += 16;
	else x -= sx;

	if( y + sy > g_ifMng->m_clientHeight )
	{
		y -= sy;
	}

	sx += 10;
	sy += 10;

	x -= 5;
	y -= 5;
	
	if( rolloverType && !forcedWinFlag )
	{
		
		m_win->m_info[0].clientX = x;
		m_win->m_info[0].clientY = y;
		m_win->UpdateBuffer( sx, sy );
		m_win->Enable();
	}
	else
	{
		
		m_win->m_info[0].clientX = x;
		m_win->m_info[0].clientY = y;
		m_win->UpdateBuffer( sx, sy );
		
		
		m_titleLeft->m_info[0].clientX = x;
		m_titleLeft->m_info[0].clientY = y - m_titleLeft->m_info[0].sizeY;
		m_titleLeft->UpdateBuffer();
		
		
		m_titleCenter->m_info[0].clientX = m_titleLeft->m_info[0].clientX + m_titleLeft->m_info[0].sizeX;
		m_titleCenter->m_info[0].clientY = m_titleLeft->m_info[0].clientY;
		nsx = sx - m_titleLeft->m_info[0].sizeX - m_titleRight->m_info[0].sizeX;
		m_titleCenter->UpdateBuffer( nsx, m_titleCenter->m_info[0].sizeY );
		
		
		m_titleRight->m_info[0].clientX = m_titleCenter->m_info[0].clientX + nsx;
		m_titleRight->m_info[0].clientY = y - m_titleRight->m_info[0].sizeY;
		m_titleRight->UpdateBuffer();
		
		
		nsy = sy - m_titleBottom->m_info[0].sizeY;
		m_titleLeftSide->m_info[0].clientX = x;
		m_titleLeftSide->m_info[0].clientY = y;
		m_titleLeftSide->UpdateBuffer( m_titleLeftSide->m_info[0].sizeX, sy );
		
		
		m_titleRightSide->m_info[0].clientX = m_titleRight->m_info[0].clientX + m_titleRight->m_info[0].sizeX;
		m_titleRightSide->m_info[0].clientY = y;
		m_titleRightSide->UpdateBuffer( m_titleRightSide->m_info[0].sizeX, sy );
		
		
		m_titleBottom->m_info[0].clientX = x;
		m_titleBottom->m_info[0].clientY = y + sy;
		m_titleBottom->UpdateBuffer( sx, m_titleBottom->m_info[0].sizeY );
		
		m_win->Enable();
		m_titleLeft->Enable();
		m_titleRight->Enable();
		m_titleCenter->Enable();
		m_titleLeftSide->Enable();
		m_titleRightSide->Enable();
		m_titleBottom->Enable();
	}
	
	strcpy( m_str, str );
	m_outputTextX = x + 5;
	m_outputTextY = y + 5;
	m_outputTextXSize = tx;
	m_outputTextYSize = ty;
}



CIFGuildAuthoritySettingWin::CIFGuildAuthoritySettingWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	m_win = new CIFControl( this );
	m_titleLeft = new CIFControl( this );
	m_titleRight = new CIFControl( this );
	m_titleCenter = new CIFControl( this );
	m_titleLeftSide = new CIFControl( this );
	m_titleRightSide = new CIFControl( this );
	m_titleBottom = new CIFControl( this );

	m_joinBtn = new CIFButtonCtl( this ); 
	m_disorganizeBtn = new CIFButtonCtl( this ); 
	m_banishBtn = new CIFButtonCtl( this ); 
	m_setMasterBtn = new CIFButtonCtl( this ); 
	m_applyBloodPledgeBtn = new CIFButtonCtl( this ); 
	m_breakBloodPledgeBtn = new CIFButtonCtl( this ); 
	m_applyBloodPledgeBattleBtn = new CIFButtonCtl( this ); 
	m_requestPvPBtn = new CIFButtonCtl( this ); 
	m_modeSettingBtn = new CIFButtonCtl( this ); 
	m_resetPositionBtn = new CIFButtonCtl( this ); 

	Enable( false );

	m_str[0] = 0;
}

CIFGuildAuthoritySettingWin::~CIFGuildAuthoritySettingWin()
{

}

void CIFGuildAuthoritySettingWin::PrepareControl()
{
	m_win->PrepareControl( IF_ROLLOVER_WIN );
	m_titleLeft->PrepareControl( IF_ROLLOVER_WIN_TITLE_LEFT );
	m_titleRight->PrepareControl( IF_ROLLOVER_WIN_TITLE_RIGHT );
	m_titleCenter->PrepareControl( IF_ROLLOVER_WIN_TITLE_CENTER );
	m_titleLeftSide->PrepareControl( IF_ROLLOVER_SIDE );
	m_titleRightSide->PrepareControl( IF_ROLLOVER_SIDE );
	m_titleBottom->PrepareControl( IF_ROLLOVER_BOTTOM );

	
	
	
	
	
	
	
	
	
	
}

int CIFGuildAuthoritySettingWin::WindowFrame()
{
	if( !State() ) return( IF_CTL_NONE );
	return IF_CTL_NONE;
}

void CIFGuildAuthoritySettingWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	m_win->Render();
	m_titleLeft->Render();
	m_titleRight->Render();
	m_titleCenter->Render();
	m_titleLeftSide->Render();
	m_titleRightSide->Render();
	m_titleBottom->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->SetColor( 0xffffffff );
	g_chatManager->BeginFrame();
	{
		g_chatManager->DrawText( m_outputTextX, m_outputTextY, m_str, m_outputTextXSize, 13, m_saveArragneType );
	}
	g_chatManager->EndFrame();
}

int CIFGuildAuthoritySettingWin::WindowSelection()
{
	return IF_CTL_NONE;
}

int CIFGuildAuthoritySettingWin::WindowProcess()
{
	return IF_CTL_NONE;
}

void CIFGuildAuthoritySettingWin::MakeRolloverWindow( int x, int y, char *str, int forcedWinFlag, int xsize, int ysize, int arrangeType )
{
	m_saveArragneType = arrangeType;

	Enable();

	m_win->Enable( false );
	m_titleLeft->Enable( false );
	m_titleRight->Enable( false );
	m_titleCenter->Enable( false );
	m_titleLeftSide->Enable( false );
	m_titleRightSide->Enable( false );
	m_titleBottom->Enable( false );

	int sx, sy, rolloverType = 0;
	int tx, ty, nsx, nsy;

	
	
	xsize = -1;
	
	g_chatManager->GetTextRect( xsize, 13, str, sx, sy );
	if( sx <= 0 || sy <= 0 ) return;
	
	
	if( sx < m_titleBottom->m_info[0].sizeX ) 
	{
		rolloverType = 1;
		if( forcedWinFlag ) sx = m_titleBottom->m_info[0].sizeX;
	}
	if( sx > xsize && xsize > 0 ) 
	{
		sx = xsize;
		sy = g_chatManager->GetTextRectLine( sx, 13, str );
	}
	if( !forcedWinFlag ) rolloverType = 1;

	
	tx = sx;
	ty = sy;

	if( x - sx < 0 ) x += 16;
	else x -= sx;

	if( y + sy > g_ifMng->m_clientHeight )
	{
		y -= sy;
	}

	sx += 10;
	sy += 10;

	x -= 5;
	y -= 5;
	
	if( rolloverType && !forcedWinFlag )
	{
		
		m_win->m_info[0].clientX = x;
		m_win->m_info[0].clientY = y;
		m_win->UpdateBuffer( sx, sy );
		m_win->Enable();
	}
	else
	{
		
		m_win->m_info[0].clientX = x;
		m_win->m_info[0].clientY = y;
		m_win->UpdateBuffer( sx, sy );
		
		
		m_titleLeft->m_info[0].clientX = x;
		m_titleLeft->m_info[0].clientY = y - m_titleLeft->m_info[0].sizeY;
		m_titleLeft->UpdateBuffer();
		
		
		m_titleCenter->m_info[0].clientX = m_titleLeft->m_info[0].clientX + m_titleLeft->m_info[0].sizeX;
		m_titleCenter->m_info[0].clientY = m_titleLeft->m_info[0].clientY;
		nsx = sx - m_titleLeft->m_info[0].sizeX - m_titleRight->m_info[0].sizeX;
		m_titleCenter->UpdateBuffer( nsx, m_titleCenter->m_info[0].sizeY );
		
		
		m_titleRight->m_info[0].clientX = m_titleCenter->m_info[0].clientX + nsx;
		m_titleRight->m_info[0].clientY = y - m_titleRight->m_info[0].sizeY;
		m_titleRight->UpdateBuffer();
		
		
		nsy = sy - m_titleBottom->m_info[0].sizeY;
		m_titleLeftSide->m_info[0].clientX = x;
		m_titleLeftSide->m_info[0].clientY = y;
		m_titleLeftSide->UpdateBuffer( m_titleLeftSide->m_info[0].sizeX, sy );
		
		
		m_titleRightSide->m_info[0].clientX = m_titleRight->m_info[0].clientX + m_titleRight->m_info[0].sizeX;
		m_titleRightSide->m_info[0].clientY = y;
		m_titleRightSide->UpdateBuffer( m_titleRightSide->m_info[0].sizeX, sy );
		
		
		m_titleBottom->m_info[0].clientX = x;
		m_titleBottom->m_info[0].clientY = y + sy;
		m_titleBottom->UpdateBuffer( sx, m_titleBottom->m_info[0].sizeY );
		
		m_win->Enable();
		m_titleLeft->Enable();
		m_titleRight->Enable();
		m_titleCenter->Enable();
		m_titleLeftSide->Enable();
		m_titleRightSide->Enable();
		m_titleBottom->Enable();
	}
	
	strcpy( m_str, str );
	m_outputTextX = x + 5;
	m_outputTextY = y + 5;
	m_outputTextXSize = tx;
	m_outputTextYSize = ty;
}
