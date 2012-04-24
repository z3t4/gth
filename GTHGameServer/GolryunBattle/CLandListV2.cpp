



#include "..\global.h"
#include "CLandListV2.h"
#include <windowsx.h>




CLandListV2::CLandListV2(const int Max)
{
	m_bOK=FALSE;
	m_pArray = NULL;
	m_iMaxNum=0;

	m_pArray = (int*)GlobalAllocPtr(GMEM_FIXED,sizeof(int)*Max);
	if(NULL == m_pArray) return;


	m_bOK=TRUE;

}

CLandListV2::~CLandListV2()
{
	if(NULL != m_pArray){ GlobalFreePtr(m_pArray); m_pArray=NULL; }
}



void CLandListV2::Push(const int Data)
{
	m_pArray[m_iMaxNum++]=Data;
}


int CLandListV2::Get(void)
{
	int idx = rand() % m_iMaxNum ;
	int data=0;
	while(TRUE){
		if(-1 != m_pArray[idx]){
			data = m_pArray[idx];
			m_pArray[idx]=-1;
			return data;
		}
		idx++;
		if(idx >= m_iMaxNum) idx=0;
	}
}




