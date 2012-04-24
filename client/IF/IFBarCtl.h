



#if !defined(AFX_IFBARCTL_H__CF83FBFD_35C1_4108_8E5A_26315B0948AE__INCLUDED_)
#define AFX_IFBARCTL_H__CF83FBFD_35C1_4108_8E5A_26315B0948AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFBarCtl : public CIFControlNode  
{
public:

	int						m_state;

	int						m_curPos;
	int						m_maxPos;
	
	int						m_barID;
	int						m_emptyBarID;

	int						m_barSize;
	int						m_barPosX;

	CIFControl				*m_bar;
	CIFControl				*m_emptyBar;

	CIFBarCtl( CIFControlNode *pParentControl = NULL );
	virtual ~CIFBarCtl();

	int			State()								{ return( m_state ); }
	void		Enable( int state = true )			{ m_state = state; }
	
	virtual	void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice );
	virtual	void		SetTexManager( CD3DTextureMng *texMng );
	virtual	void		PrepareControl( int barID, int emptyBarID = -1 );
	virtual	void		UpdateControl();
	virtual	void		PrepareBuffer();
	virtual	void		UpdateBuffer();
	virtual	void		Release();

	virtual	void		Render();

	void		UpdateBarState( int curPos, int maxPos );
	void		UpdateBarState_Inverse( int curPos, int maxPos );
};

#endif 
