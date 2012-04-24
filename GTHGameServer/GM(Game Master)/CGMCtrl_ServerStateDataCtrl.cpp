



#include "..\global.h"
#include "CGMCtrl_ServerStateDataCtrl.h"




CGMCtrl_ServerStateDataCtrl::CGMCtrl_ServerStateDataCtrl()
{
	m_bOK=FALSE;

	memset(&m_State,0x00,sizeof(struct tagState));

	m_State.Flag.Ele.bPK=FALSE;
	m_State.Flag.Ele.bNProtectAUTH=FALSE;

	m_bOK=TRUE;

}





CGMCtrl_ServerStateDataCtrl::~CGMCtrl_ServerStateDataCtrl()
{

}





BOOL CGMCtrl_ServerStateDataCtrl::isOK(void) const
{
	return m_bOK;
}





BOOL CGMCtrl_ServerStateDataCtrl::isPK(void) const
{
	return m_State.Flag.Ele.bPK;
}



void CGMCtrl_ServerStateDataCtrl::SaveODBC(void)
{
	g_DBGameServer->SaveServerStatus(m_State.Flag.Value);
}




BOOL CGMCtrl_ServerStateDataCtrl::isNProtectAUTH(void) const
{
	return m_State.Flag.Ele.bNProtectAUTH;
}




void CGMCtrl_ServerStateDataCtrl::SetNProtectAUTH(const BOOL in_bEnable)
{
	m_State.Flag.Ele.bNProtectAUTH= in_bEnable;

}




void CGMCtrl_ServerStateDataCtrl::SetPK(const BOOL bPK)
{
	m_State.Flag.Ele.bPK = bPK;
}





void CGMCtrl_ServerStateDataCtrl::ODBC_ProcessQueryResultQueue_QUERY_LOADSERVERSTATUS(const querySet_t& result)
{
	if(result.retCode < 0){
		return;
	}

	m_State.Flag.Value = result.ServerStatus.Value;
}





