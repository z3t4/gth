



#include "../Global.h"
#include "CCSVFileCtrl.h"
#include <windowsx.h>




CCSVFileCtrl::CCSVFileCtrl(const char* szpFileName)
{
	m_bOK=FALSE;
	m_pFileBuffer = NULL;
	strncpy(m_szFileName,szpFileName,_MAX_PATH);
	m_szFileName[_MAX_PATH]=NULL;
	m_iFileBytes=0;
	m_pCurPos = NULL;
	m_pEndPos =  NULL;

	if(FALSE == LoadFile()) return;

	m_bOK=TRUE;
}






CCSVFileCtrl::~CCSVFileCtrl()
{
	if(NULL != m_pFileBuffer){ GlobalFreePtr(m_pFileBuffer); m_pFileBuffer=NULL; }

}





BOOL CCSVFileCtrl::isOK(void) const
{
	return m_bOK;
}




const unsigned char* CCSVFileCtrl::NextToken(void)
{
	
	if(m_pCurPos >= m_pEndPos) return NULL;
	int bytes=0;
	
    
	while(TRUE){
		if ( m_pCurPos >= m_pEndPos ) break;
		if(*m_pCurPos == '\n'){	m_pCurPos++; continue;}
		if((*m_pCurPos == ',') || (*m_pCurPos == '\r')){ m_pCurPos++; break; }


		if(TRUE == IsDBCSLeadByte((BYTE)*m_pCurPos)){
			bytes+=1; m_Token[bytes-1] = *m_pCurPos++;
			bytes+=1; m_Token[bytes-1] = *m_pCurPos++;			
		}else{
			bytes+=1; m_Token[bytes-1] = *m_pCurPos++;			
		}
	}

	if(bytes <= 0) return NULL;
	m_Token[bytes]=NULL;
	return (const unsigned char*)m_Token;	
}





BOOL CCSVFileCtrl::LoadFile(void)
{	
	FILE *fp = NULL;
	fp = fopen( m_szFileName, "rb" );
	if(NULL == fp) return FALSE;

	fseek(fp,0,SEEK_END);
	m_iFileBytes = ftell(fp);
	m_pFileBuffer = (unsigned char*)GlobalAllocPtr(GMEM_FIXED,sizeof(unsigned char) * m_iFileBytes);
	if(NULL == m_pFileBuffer) return FALSE;


	fseek(fp,0,SEEK_SET);
	fread(m_pFileBuffer,m_iFileBytes,1,fp);
	if(NULL != fp){ fclose(fp); fp=NULL; }

	m_pCurPos = m_pFileBuffer;
	m_pEndPos = m_pFileBuffer + m_iFileBytes;
	return TRUE;	
}


