



#include "../Global.h"
#include "HiCocoonCtrl.h"





CHiCocoonCtrl::CHiCocoonCtrl()
{
	m_bOK = FALSE;
	BOOL Result = FALSE;

	
	Result = m_DaumGameAuth.Init (
		_T("z6DuDLA1NcyXge3psD63Uu0o3GqpAzyBjm7pD0hNDYKefE37fNM1tVng34ejixgA") );

	if ( Result == FALSE)
		return;

	m_bOK = TRUE;

}

CHiCocoonCtrl::~CHiCocoonCtrl()
{

}

BOOL CHiCocoonCtrl::IsOK()
{
	return m_bOK;
}

BOOL CHiCocoonCtrl::SetAuth(char* auth)
{
	if ( auth == NULL)
		return FALSE;

	BOOL Result = FALSE;
	Result = m_DaumGameAuth.SetSource( auth );	
	return Result;

}

BOOL CHiCocoonCtrl::GetTimeExpired()
{
	return m_DaumGameAuth.IsTimeExpired();
}



CDaumGameAuth	CHiCocoonCtrl::GetDaumGameAuth()
{	
	return m_DaumGameAuth;
}




	
	
	

	
	