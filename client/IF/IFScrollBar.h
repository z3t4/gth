



#if !defined(AFX_IFSCROLLBAR_H__85DD7A50_758C_4272_9444_715B1638DD47__INCLUDED_)
#define AFX_IFSCROLLBAR_H__85DD7A50_758C_4272_9444_715B1638DD47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFScrollBar : public CIFControlNode  
{
public:
	int					m_state;
	int					m_max;
	int					m_current;
	int					m_barType; 
	int					m_barOriginalPos;
	int					m_barPos;
	int					m_barSize;
	int					m_scrollBarDragFlag;

	CIFControl			*m_bgBar;
	CIFControl			*m_bar;
	CIFButtonCtl		*m_upBtn;
	CIFButtonCtl		*m_dnBtn;

	CIFScrollBar( CIFControlNode *pParentControl = NULL );
	virtual ~CIFScrollBar();

	int			State()								{ return( m_state ); }
	void		Enable( int state = true )			{ m_state = state; }
	
	void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice );
	void		SetTexManager( CD3DTextureMng *texMng );
	void		PrepareControl( int id );
	void		UpdateControl();

	void		PrepareBuffer();
	void		UpdateBuffer();

	void		Release();
	void		Render();

	int			ProcessScrollBar();

	inline void		SetScrollBarType(int type)			{ m_barType = type; }
	void		SetBarPosition(int max, int cur);
	void		SetBarPositionCurrent( int cur);
	void		SetBarPositionMax();
};

#endif 
