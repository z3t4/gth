



#if !defined(AFX_CGMCTRL_SERVERSTATEDATACTRL_H__69F41670_3750_4092_BDDB_ED407AA1E1E8__INCLUDED_)
#define AFX_CGMCTRL_SERVERSTATEDATACTRL_H__69F41670_3750_4092_BDDB_ED407AA1E1E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CGMCtrl_ServerStateDataCtrl  
{
	public:
		struct tagState{
			public:
				struct tagFlag{
					public:
						
						
						struct tagElement{
							unsigned int bPK:1;
							unsigned int bNProtectAUTH:1;
							unsigned int unused:30;
						};
					public:
						union{
							unsigned int Value;
							struct tagElement Ele;
						};
				};
			public:
				struct tagFlag Flag;
				
		};

	private:
		BOOL m_bOK;
		struct tagState m_State;

	public:
		BOOL isOK(void) const;
		void SaveODBC(void);

	
	public:
		BOOL isPK(void) const;
		void SetPK(const BOOL bPK);
		
	
	public:
		BOOL isNProtectAUTH(void) const;
		void SetNProtectAUTH(const BOOL in_bEnable);

	public:
		void ODBC_ProcessQueryResultQueue_QUERY_LOADSERVERSTATUS(const querySet_t& result);


	
	public:
		CGMCtrl_ServerStateDataCtrl();
		virtual ~CGMCtrl_ServerStateDataCtrl();

};

#endif 
