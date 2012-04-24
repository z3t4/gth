



#if !defined(AFX_IFCONTROL_H__BEC6D037_7B63_4ED2_A130_A1DC6D97D33B__INCLUDED_)
#define AFX_IFCONTROL_H__BEC6D037_7B63_4ED2_A130_A1DC6D97D33B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFControl	: public CIFControlNode  
{
public:
	
	IFControlInfo_t			m_info[MAX_COUNT_IF_CONTROL_SPRITE];
	int						m_click;
	int						m_current;		
	int						m_old;			
	int						m_enable;		
	
	bool					m_animFlag;		

	CD3DBuffer				*m_vexbuf;		
	LPDIRECT3DDEVICE8		m_pd3dDevice;
	CD3DTextureMng			*m_texMng;


	DWORD					m_fadeColor;
	int						m_fadeFlag;
	int						m_fadeState;
	float					m_fadeTime;
	float					m_stayTime;
	float					m_fadeStart;

	float					m_animTime;
	float					m_animCurTime;
	int						m_numAnimFrame;

	CIFControl( CIFControlNode *pParentControl = NULL );
	virtual ~CIFControl();

	void		Enable( int state = true )					{ m_enable = state; }
	int			ToggleState()								{ m_enable = !m_enable; return( m_enable ); }
	int			State()										{ return( m_enable ); }

	virtual void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice );
	virtual void		SetTexManager( CD3DTextureMng *texMng );
	virtual	void		Release();
	virtual	void		UpdateControl();
	virtual	void		PrepareBuffer();
	virtual	void		UpdateBuffer();
	virtual void		PrepareControl(int id, int idx = 0, float diffX = 0.0f, float diffY = 0.0f );
	virtual int			Selection();

	virtual int			GetXPos()			{ return( m_info[0].clientX ); }
	virtual int			GetYPos()			{ return( m_info[0].clientY ); }
	virtual int			GetXSize()			{ return( m_info[0].sizeX ); }
	virtual int			GetYSize()			{ return( m_info[0].sizeY ); }

	void		PrepareAnimControl(int id, int numFrame, float animTime = 500.0f );

	void		ChangeControlSprite( int idx );
	void		ChangeControlTexInfo( int id, int idx = 0 );

	void		UpdateAnimControl();
	
	void		UpdateStopControl();

	void		UpdateBuffer( int zx, int zy );
	void		UpdateRotateBuffer( float fAngle );
	
	
	void		Update3DBuffer( float z, float w );

	virtual	void	Render();
	void			RenderFade();
	void			SetFade( float fadetime, float staytime );
};

#endif 
