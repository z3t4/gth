







extern	_XTextureManager	g_MainInterfaceTextureArchive;





_XWindow_IME::_XWindow_IME()
{
	m_pListBox = NULL;
}

_XWindow_IME::~_XWindow_IME()
{

}

BOOL _XWindow_IME::Initialize(void)
{
	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								
		{ 6, 24 },							
		{ 116, 128 },						
		0,									
		-1,									
		&g_MainInterfaceTextureArchive,		
		_T(" "),							
		14,									
		9,									
		_XLSTYLE_RIGHTSCROLLBAR,			

		
		{12, 128},
		12,
		22,
		150,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};

	m_pListBox = new _XListBox;
	m_pListBox->Create(listboxstructure);

	POINT coloffset = { 2, 2 };
	m_pListBox->SetColumnOffset(0, coloffset);
	m_pListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(0, 110 );
	m_pListBox->SetTrackHeight(14);
	m_pListBox->SetTrackBarColor( D3DCOLOR_ARGB(64,128,128,128) );
	m_pListBox->EnableSelectItem(FALSE);
	m_pListBox->SetAlwaysShowScrollBarMode( FALSE );

	_XVScrollBar* pScrollBar = m_pListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}

	InsertChildObject(m_pListBox);	
	
	return TRUE;
}

void _XWindow_IME::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_IME::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(this->m_ShowWindow)
	{
		DWORD blackcolor = D3DCOLOR_ARGB(255,0,0,0);
		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, 
						 m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, blackcolor );
		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, 
			m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(255,32,32,32) );
		_XDrawRectAngle( m_WindowPosition.x + 2, m_WindowPosition.y + 2, 
						 m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f, blackcolor );

		_XDrawLine2D( m_WindowPosition.x + 3, m_WindowPosition.y + 19, 
					  m_WindowPosition.x  + m_WindowSize.cx - 3, m_WindowPosition.y + 19, blackcolor,blackcolor );
	
		_XIMEContainer* pIMEContainer = _XIMEKERNEL.GetIMEContainer();

		g_XBaseFont.SetColor(_XSC_DEFAULT);
		g_XBaseFont.Puts( m_WindowPosition.x + 8, m_WindowPosition.y + 6, pIMEContainer->GetComText() );
		g_XBaseFont.Flush();
	}
}

BOOL _XWindow_IME::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
		
	_XWindow::Process(pfocusobject);
	
	return FALSE;
}

BOOL _XWindow_IME::CheckArea(void)
{
	if( !m_pMyManager ) return FALSE;

	if(CheckMousePosition())
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_IME::ShowWindow(BOOL show)
{	
	if( show ) CheckOutside();
	_XWindow::ShowWindow(show);
}
