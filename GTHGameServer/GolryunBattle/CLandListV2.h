



#if !defined(AFX_CLANDLISTV2_H__452E36DD_3DC2_4755_971B_6D12EFCE3860__INCLUDED_)
#define AFX_CLANDLISTV2_H__452E36DD_3DC2_4755_971B_6D12EFCE3860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CLandListV2  
{
	private:
		BOOL m_bOK;
		int* m_pArray;
		int  m_iMaxNum;
	public:
		BOOL isOK(void) const;
		void Push(const int Data);

	public:
		CLandListV2();
		virtual ~CLandListV2();

};

#endif 
