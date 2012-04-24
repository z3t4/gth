



#if !defined(AFX_IFTEXT_H__2C4A617B_D9FE_44FF_93F3_E577283C3CEB__INCLUDED_)
#define AFX_IFTEXT_H__2C4A617B_D9FE_44FF_93F3_E577283C3CEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFText  : public CIFControlNode
{
public:

	enum{
		maxbytes_str=256,
	};

	IFControlInfo_t			m_info;
	char					m_str[maxbytes_str+1];
	DWORD					m_color;

	int						m_enable;


	CIFText( CIFControlNode *pParentControl = NULL );
	
    
    CIFText(const CIFText &rtext):CIFControlNode(rtext), m_info(rtext.m_info),m_color(rtext.m_color),
		m_enable(rtext.m_enable)
	{
		strcpy(m_str,rtext.m_str);
	}
	
    CIFText& operator=( const CIFText &rParentControl)
	{
			
		if(this==&rParentControl) return *this;
		CIFControlNode::operator=(rParentControl);
		m_info=rParentControl.m_info;
		strcpy(m_str,rParentControl.m_str);
		m_color=rParentControl.m_color;
		m_enable=rParentControl.m_enable;
		return *this;
	}
	
	virtual ~CIFText();
	
	void		PrepareControl( int id, float diffX = 0.0f, float diffY = 0.0f );
	int			Selection();
	void		UpdateControl();

	void		SetText( char *str );
	void		SetColor( DWORD color );
	void		Render( int renderType = IF_TEXT_OUTPUT_NORMAL );

	int			GetXPos()			{ return( m_info.clientX ); }
	int			GetYPos()			{ return( m_info.clientY ); }
	int			GetXSize()			{ return( m_info.sizeX ); }
	int			GetYSize()			{ return( m_info.sizeY ); }

	void		Enable( int state = true )			{ m_enable = state; }
	int			ToggleState()						{ m_enable = !m_enable; return( m_enable ); }
	int			State()								{ return( m_enable ); }
};

#endif 
