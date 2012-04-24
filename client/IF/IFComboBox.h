



#if !defined(AFX_IFCOMBOBOX_H__E1B67981_9D88_4FC9_A72B_3A7CB607A9C0__INCLUDED_)
#define AFX_IFCOMBOBOX_H__E1B67981_9D88_4FC9_A72B_3A7CB607A9C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#define MAX_NUM_OF_SEL_STRING		32
#define COMBO_EXPANSION_HEIGHT		17


class CIFComboBox : public CIFControl
{
public:

	
	CIFControl			*m_combo;
	CIFControl			*m_comboExpansion;
	CIFButtonCtl		*m_comboBtn;

	int					m_state;
	int					m_bExpansion;		
	int					m_size;
	
	
	chatMessage			m_string[MAX_NUM_OF_SEL_STRING];
	int					m_numString;
	int					m_curSel;
	int					m_tempCurSel;

	
	int					m_strColor[MAX_NUM_OF_SEL_STRING];
	
	DWORD				m_itemData[MAX_NUM_OF_SEL_STRING];

	CIFComboBox( CIFControlNode *pParentControl = NULL );
	virtual ~CIFComboBox();

	int			State()								{ return( m_state ); }
	void		Enable( int state = true )			{ m_state = state; }
	
	void		SetDevice( LPDIRECT3DDEVICE8 pD3DDevice );
	void		SetTexManager( CD3DTextureMng *texMng );

	virtual	void		PrepareControl( int comboID, int size = 100 );
	virtual void		UpdateControl();

	void		PrepareBuffer();
	void		UpdateBuffer();

	void		Release();
	void		Render();

	int			ProcessComboBox();

	int			AddString( char* str , int color = 0 );
	int			ReadString( char* str, int sel );
	int			ReadCurrentString( char* str );

	int			SetCurSel( int sel );
	int			GetCurSel();
	
	void		ClearString();
	
	void		SetItemData( int sel, DWORD data );
	DWORD		GetItemData( int sel );
};

#endif 
