



#if !defined(AFX_IFSPINCTL_H__BA1AA7DD_6A22_4A80_8547_39757EFC1710__INCLUDED_)
#define AFX_IFSPINCTL_H__BA1AA7DD_6A22_4A80_8547_39757EFC1710__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFSpinCtl : public CIFControlNode
{
private:

	bool			m_state;
	int				m_min;
	int				m_max;
	int				m_current;

	CIFButtonCtl	*m_upBtn;
	CIFButtonCtl	*m_downBtn;

public:

	bool		State()							{ return m_state; }
	void		Enable( bool state = true )		{ m_state = state; }

	void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice );
	void		SetTexManager( CD3DTextureMng *texMng );
	void		PrepareControl( int id );
	void		UpdateControl();
	void		PrepareBuffer();
	void		UpdateBuffer();
	void		Release();
	void		Render();

	int			ProcessControl();
	void		SetValueRange( int minValue, int maxValue );
	void		SetCurrentValueStart()			{ m_current = m_min; }
	void		SetCurrentValueEnd()			{ m_current = m_max; }
	int			GetCurrentValue()				{ return m_current; }
	
	CIFSpinCtl( CIFControlNode *pParentControl = NULL );
	virtual ~CIFSpinCtl();

};

#endif 
