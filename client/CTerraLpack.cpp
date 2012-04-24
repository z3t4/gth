




#include "../global_def.h"
#include "CTerraLpack.h"





char* CTerraLpack::GetMessage(int Idx, int etc1, int etc2)
{
	if(Idx >= m_vpMessage.size()) return m_pUnknownMsg_RangeOverflow_Etc;
	if(etc1 >= GetVector(Idx)->size() ) return m_pUnknownMsg_RangeOverflow_String;

	std::string* pString = (std::string*)(*GetVector(Idx))[etc1];
	return (char*)pString->c_str();	
}





void CTerraLpack::Init()
{
	m_vpMessage.reserve(MaxTypeNum);	
	m_NowvpIdx = 0;	

	for (int i = 0; i < MaxTypeNum; i++){
		std::vector< void* >* pMessageVector = new std::vector< void* >;
		m_vpMessage.push_back(pMessageVector);
	}		
	
}





void CTerraLpack::Destroy()
{
	for (int i = 0; i < m_vpMessage.size(); i++ )
	{
		for (int p = 0; p < m_vpMessage[i]->size(); p++)
		{			
			std::string* pString = (std::string*)(*GetVector(i))[p];
			delete pString;
		}
		m_vpMessage[i]->clear();
		delete m_vpMessage[i];
	}	
	m_vpMessage.clear();

}






bool CTerraLpack::LoadFile(char* filename)
{
	BYTE*	pFileBuffer = NULL;
	BYTE*	ptempBuffer	= NULL;

	char	TokenBuffer[TEMPBUFFER_SIZE];
	int		LineSize[LPACK_MAX_LINESIZE];
	char	Token,tempToken;
	int		TokenBufferPos;
	char	SentenseOpen;	
	int		BufferPos;
	int		Checkwon;	
	int		RowCount;

	TokenBufferPos	= 0;
	SentenseOpen	= 0;
	Checkwon		= 0;
	BufferPos		= 0;
	RowCount		= 0;

	if (m_pFileMng != NULL)
		pFileBuffer = m_pFileMng->ReadFileFromPack(filename);
	
	if ( pFileBuffer == NULL ) return 0;

	ptempBuffer = pFileBuffer;

	memset ( TokenBuffer,0, sizeof( TokenBuffer ) );
	memset ( LineSize, 0, sizeof( LineSize ) );

	while (ptempBuffer[BufferPos] != NULL)
	{
		Token=ptempBuffer[BufferPos++];

		if( IsDBCSLeadByte( (BYTE)Token ) )
		{
			TokenBuffer[TokenBufferPos++]=Token;
			Token=ptempBuffer[BufferPos++];
			TokenBuffer[TokenBufferPos++]=Token;
			
			continue;
		}

		switch (Token)
		{
		case LPACK_SEMICOLON :										
			if (SentenseOpen)
			{
				TokenBuffer[TokenBufferPos++]=Token;
			}
			else
			{
				while (Token!=LPACK_LINEFEED && ptempBuffer[BufferPos]!=NULL)
				{
					Token=ptempBuffer[BufferPos++];
				}
			}
			break;

		case LPACK_COLON :											
			if (!SentenseOpen)
			{


				
			}
			else
			{
				TokenBuffer[TokenBufferPos++]=Token;
			}
			break;

		case LPACK_BACKSLASH :										
			if (SentenseOpen)										
			{
				tempToken=ptempBuffer[BufferPos++];

				if (tempToken=='\\')
					TokenBuffer[TokenBufferPos++]=' ';

				 
				else					
				if (tempToken=='n')									
					TokenBuffer[TokenBufferPos++]='\n';
			}
			break;
			
		case LPACK_RETURN :											
			break;

		case LPACK_LINEFEED :										
			if(0 == strcmp("[TERRA]", TokenBuffer))	
				m_LPackType = LPACK_TYPE_TERRA;

			else if(0 == strcmp ("[BILLING]", TokenBuffer))
				m_NowvpIdx = type_Billing;
			
			else if ( strcmp ("[HACK SHIELD]", TokenBuffer) == 0)			
				m_NowvpIdx = type_HackShield;			

			memset(TokenBuffer,0,TEMPBUFFER_SIZE);
			TokenBufferPos=0;
			
			break;

		case LPACK_QUOTATION :										
			if (!SentenseOpen )
			{
				SentenseOpen=1;										
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}
			else
			{
				SentenseOpen=0;										
				LineSize[RowCount++]=strlen(TokenBuffer);

				std::string* str = new std::string;
				(*str) = TokenBuffer;

				std::vector<void*>* pvector = GetVector(m_NowvpIdx);
				pvector->push_back((void*)str);
				
				memset(TokenBuffer, 0, TEMPBUFFER_SIZE);
				TokenBufferPos=0;
				Token=ptempBuffer[BufferPos++];

				while (Token!=LPACK_LINEFEED && ptempBuffer[BufferPos]!=NULL)
				{
					Token=ptempBuffer[BufferPos++];
				}
			}
			break;

		default :
			TokenBuffer[TokenBufferPos++]=Token;
			break;
		}
	}
	
	GTH_SAFE_FREE( pFileBuffer );
	return TRUE;
}