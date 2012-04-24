



#if !defined(AFX_IFCHECKBOX_H__7646E663_667D_44E6_94E9_B0025FA22EB7__INCLUDED_)
#define AFX_IFCHECKBOX_H__7646E663_667D_44E6_94E9_B0025FA22EB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CIFCheckBox : public CIFControl  
{
public:
	bool		m_bChecked;

	CIFCheckBox( CIFControlNode *pParentControl = NULL );
	virtual ~CIFCheckBox();

	virtual void		PrepareControl( int id, float diffX = 0.0f, float diffY = 0.0f );
	virtual void		UpdateControl();




	int			ProcessCheckBox();

	inline void		SetCheck(bool bCheck)		{ m_bChecked = bCheck; if( m_bChecked ) ChangeControlSprite( 1 ); else ChangeControlSprite( 0 ); }
	inline bool		GetCheck()					{ return m_bChecked; }
};

#endif 
