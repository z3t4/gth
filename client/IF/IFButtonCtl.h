



#if !defined(AFX_IFBUTTONCTL_H__3A58F660_6CBD_4730_A4D0_2C3C7B02FBE0__INCLUDED_)
#define AFX_IFBUTTONCTL_H__3A58F660_6CBD_4730_A4D0_2C3C7B02FBE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFButtonCtl : public CIFControl
{
public:

	int			m_btnState;
	int			m_saveButtonTime;
	char		m_str[256];
	DWORD		m_color;

	int			m_flickerMode;
	int			m_flickerModeBackup;
	float		m_flickerCurTime;
	int			m_spriteNo;

	
	
	static		m_focusedControlID;

	CIFButtonCtl( CIFControlNode *pParentControl = NULL );
	virtual ~CIFButtonCtl();

	virtual	void		PrepareControl( int id );
	virtual	void		UpdateControl();

	void		UpdateClientPosition( int x, int y );

	int			ProcessButton();
	void		SetButtonState( int state );
	int			GetButtonState()				{ return( m_btnState ); }
	
	void		RenderTextBtn();
	void		SetButtonText( char *str, DWORD color = 0xffffffff );

	void		SetFlickerMode( bool mode );
};



#endif 
