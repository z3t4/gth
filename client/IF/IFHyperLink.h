



#if !defined(AFX_IFHYPERLINK_H__53D8FDFF_2E56_4ECA_83E7_ABAB4E9F2CE4__INCLUDED_)
#define AFX_IFHYPERLINK_H__53D8FDFF_2E56_4ECA_83E7_ABAB4E9F2CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

typedef struct _linkNode {
	int left;
	int top;
	int right;
	int bottom;
	int linkNumber;

	_linkNode *next;
} linkNode_t;

class CIFHyperLink : public CIFControlNode  
{
public:

	IFControlInfo_t			m_info;
	char					m_str[4096];
	DWORD					m_color;

	int						m_enable;

	linkNode_t				*m_rootLinkNode;
	int						m_pointLinkIndex;
	int						m_startLine;
	int						m_maxLineCount;
	int						m_viewLineCount;

	CIFHyperLink( CIFControlNode *pParentControl = NULL );
	virtual ~CIFHyperLink();

	void		PrepareControl( int id );
	void		UpdateControl();



	int			Selection();

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

	void		AddLinkNode( linkNode_t *node );
	void		ClearAllLinkNode();

	int			PointInLink();
};

#endif 
