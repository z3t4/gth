



#include "../global_def.h"
#include "IFCommon.h"





CIFCheckBox::CIFCheckBox( CIFControlNode *pParentControl ) : CIFControl( pParentControl )
{
	m_bChecked = false;
}

CIFCheckBox::~CIFCheckBox()
{

}

void CIFCheckBox::PrepareControl( int id, float diffX, float diffY )
{
	CIFControl::PrepareControl( id, 0, diffX, diffY );
	CIFControl::PrepareControl( id + 1, 1, diffX, diffY );
}

void CIFCheckBox::UpdateControl()
{
	int tempsprIdx = m_current;

	for( int i = 0; i < 2; i ++ )
	{
		m_current = i;
		CIFControl::UpdateControl();
	}
	m_current = tempsprIdx;

	CIFControlNode::UpdateControl();
}


int CIFCheckBox::ProcessCheckBox()
{
	if( !State() ) return( IF_CTL_NONE );


	switch( Selection() )
	{
		case IF_LBDN :
			return IF_CTL_NONE;
		case IF_LBUP :
		{
			m_bChecked = !m_bChecked;

			if( m_bChecked )
				ChangeControlSprite( 1 );
			else
				ChangeControlSprite( 0 );
			
			return IF_CTL_SELECT;
		}
		default :				
			return IF_CTL_NONE;
	}

	return 0;
}

