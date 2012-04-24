



#if !defined(AFX_CTERRALPACK_H__FCE6D227_ED0E_40DB_8F68_1CE7C7D24C29__INCLUDED_)
#define AFX_CTERRALPACK_H__FCE6D227_ED0E_40DB_8F68_1CE7C7D24C29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 




class CTerraLpack : public CLPack
{
	public:
		enum enumMessageType{
			type_Billing=0,
			type_HackShield,
			MaxTypeNum,
		};	

		enum{
			LPACK_MAX_LINESIZE = 3000,
		};

	private:		
		std::vector<std::vector< void* >* >	m_vpMessage;		
		int			    m_NowvpIdx;
		
	public:
		bool			LoadFile(char* filename);
		char*			GetMessage(int Idx, int etc1, int etc2 = 0);		
		
	public:
		std::vector<void*>*  GetVector(int idx){
			return m_vpMessage[idx];			
		}

	public:
		void			Init();
		void			Destroy();		

	
	public:
		CTerraLpack(char *filename, CFileMng* pFileMng)
		{	
			Init();
			m_pFileMng	= pFileMng;	
			LoadFile(filename);	
		}		
		CTerraLpack(){};
	~CTerraLpack(){};	


};

#endif 
