



#include "../global_def.h"

#define LPACK_MAX_LINESIZE			3000


char*  CLPack::m_pUnknownMsg_Empty					= "empty";
char*  CLPack::m_pUnknownMsg_RangeOverflow_String	= "Range Overflow String Index";
char*  CLPack::m_pUnknownMsg_RangeOverflow_Quest	= "Range Overflow Quest Index";
char*  CLPack::m_pUnknownMsg_RangeOverflow_Etc		= "Range Overflow Etc Index";	
char*  CLPack::m_pUnknownMsg_RangeOverflow_Type		= "Range Overflow Type";






CLPack::CLPack()
{	
}

CLPack::~CLPack()
{

}






void CLPack::Init()
{
	m_pFileMng		= NULL;
	m_LPackType		= -1;

}







CLPack::CLPack(char *filename, CFileMng* pFileMng)
{
	Init();
	m_pFileMng	= pFileMng;	
	LoadFile(filename);	
	
}







int	CLPack::GetLpackType()
{
	return m_LPackType;
}








bool CLPack::IfLineFeed(char	*TokenBuffer)
{
	if(TokenBuffer[0] != '[') return false;

	if ( strcmp ("[NORMAL]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_NORMAL;
		return true;
	}

	if ( strcmp ("[NPC]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_NPC;
		return true;
	}

	if ( strcmp ("[QUEST]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_QUESTINFO;
		return true;
	}

	if ( strcmp ("[HELP]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_HELP;
		return true;
	}

	if ( strcmp ("[ANSWER]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_ANSWER;
		return true;
	}

	if ( strcmp ("[ETC]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_ETC;
		return true;
	}

	if ( strcmp ("[ABUSE]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_ABUSE;				
		return true;
	}			

	
	if ( strcmp ("[ABUSE2]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_ABUSE2;				
		return true;
	}
	

	
	if ( strcmp ("[SKILL_TABLE]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_SKILL;				
		return true;
	}
	
	
	if ( strcmp ("[TIP]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_TIP;				
		return true;
	}			

	if ( strcmp ("[TUTORIAL]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_TUTORIAL;
		return true;
	}

	if ( strcmp ("[NORMAL2]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_NORMAL2;
		return true;
	}
	
	
	if ( strcmp ("[AUTOSCROLL]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_AUTOSCROLL;
		return true;
	}

	
	if ( strcmp ("[CHARACTER_LIST]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_CHARACTER_NAME;
		return true;
	}
	
	
	if ( strcmp ("[ITEM_TABLE]", TokenBuffer) == 0)
	{
		m_LPackType = LPACK_TYPE_ITEM;				
		return true;
	}
	
	

	return false;
}

bool CLPack::LoadFile(char *filename)
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


	int filesize = m_pFileMng->GetFileSizeFromPack(filename);
	
	if ( pFileBuffer == NULL ) return 0;

	ptempBuffer = pFileBuffer;

	memset ( TokenBuffer,0, sizeof( TokenBuffer ) );
	memset ( LineSize, 0, sizeof( LineSize ) );

	int Num=0,OldNum=0;
	while (ptempBuffer[BufferPos] != NULL && BufferPos < filesize)
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
				Num = atoi(TokenBuffer);


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

				
				else if (tempToken=='"')
					TokenBuffer[TokenBufferPos++]='"';
			}
			break;
			
		case LPACK_RETURN :											
			if(m_LPackType == LPACK_TYPE_SKILL || m_LPackType == LPACK_TYPE_ITEM )
			{
				if(TokenBuffer[0] == '/')
				{
					memset(TokenBuffer,0,TEMPBUFFER_SIZE);
					TokenBufferPos=0;
				}
			}
			break;

		case LPACK_LINEFEED :										
			
			if ( IfLineFeed(TokenBuffer))
			{
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}
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

				std::string * str = new std::string;
				(*str) = TokenBuffer;					
				
				if(m_LPackType == LPACK_TYPE_SKILL)
				{
#ifdef SKILL_TEST_VERSION
					if(OldNum/1000 == Num/1000)
					{
						if(abs(OldNum - Num) != 1)
						{
							MessageBox(NULL,TokenBuffer,"Lpack Line Error",MB_OK);
						}
					}					
#endif
					OldNum = Num;
				 	m_strMapLPackData.insert(CMap_LpackData::value_type(Num, str));
				
				}
				else if(m_LPackType == LPACK_TYPE_ITEM)
				{	
				 	m_strMapLPackData.insert(CMap_LpackData::value_type(Num, str));
				}
				else
				{
					m_vLPackData.push_back(str);					
				}

				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
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









void CLPack::Destroy()
{		
	for (int i = 0; i <m_vLPackData.size(); i ++)
	{				
		std::string* pString = m_vLPackData[i];
		SAFE_DELETE (pString);
	}
	m_vLPackData.clear();

	CMap_LpackData::iterator iter;
	for(iter = m_strMapLPackData.begin();iter != m_strMapLPackData.end(); iter ++)
	{
		string* pString = (string *) iter->second;
		SAFE_DELETE(pString);
	}
	m_strMapLPackData.clear();
}











bool CLPack_Quest::LoadFile(char* filename)
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

	int		QuestNum	= 0;
	int		StepNum		= 0;
	bool	QuestName	= 0;

	TokenBufferPos		= 0;
	SentenseOpen		= 0;
	Checkwon			= 0;
	BufferPos			= 0;

	int		str_cnt=0;

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
				if (!strcmp(TokenBuffer, "NAME"))				
					QuestName = 1;				

				if (!strcmp(TokenBuffer, "QUESTNUM") )
				{
					memset(TokenBuffer,0,TEMPBUFFER_SIZE);
					TokenBufferPos=0;
					
					while (ptempBuffer[BufferPos]!=NULL && Token != LPACK_LINEFEED)
					{									
						Token=ptempBuffer[BufferPos++];
						if (Token != 13 ) TokenBuffer[TokenBufferPos++]=Token;
					}					

					QuestNum = atoi (TokenBuffer);
				}
				
				if (!strcmp(TokenBuffer, "STEP"))				
				{
					memset(TokenBuffer,0,TEMPBUFFER_SIZE);
					TokenBufferPos=0;
					
					while (ptempBuffer[BufferPos]!=NULL && Token != 44)
					{									
						Token=ptempBuffer[BufferPos++];
						if (Token != 13 ) TokenBuffer[TokenBufferPos++]=Token;
					}										
					StepNum = atoi (TokenBuffer);					
				}
				
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
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
			if ( strcmp ("[NORMAL]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_NORMAL;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			if ( strcmp ("[NPC]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_NPC;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			if ( strcmp ("[QUEST]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_QUESTINFO;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			if ( strcmp ("[HELP]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_HELP;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			if ( strcmp ("[ANSWER]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_ANSWER;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			if ( strcmp ("[TIP]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_TIP;				
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}			

			if ( strcmp ("[TUTOTIAL]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_TUTORIAL;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			if ( strcmp ("[NORMAL2]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_NORMAL2;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

			
			if ( strcmp ("[AUTOSCROLL]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_AUTOSCROLL;
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;
			}

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

				std::string * str = new std::string;
				(*str) = TokenBuffer;
				
				
				if ( QuestName ) 
				{
					
					m_vLPackData.push_back(str);					
				
					QuestName = 0;
				}
				else
				{
					
					sStepMessage* StepMessage = new sStepMessage;
					StepMessage->step = StepNum;
					StepMessage->Message = (void*)str;

					if ( m_vQuestMessage.size() <= QuestNum )
					{
						sQuestMessage* QuestMessage = new sQuestMessage;
						QuestMessage->QuestNum = QuestNum;						
						m_vQuestMessage.push_back(QuestMessage);
						QuestMessage->v_StepMessage.push_back(StepMessage);		
						
					}
					else					
						m_vQuestMessage[QuestNum]->v_StepMessage.push_back(StepMessage);					

				}
				memset(TokenBuffer,0,TEMPBUFFER_SIZE);
				TokenBufferPos=0;				
				Token=ptempBuffer[BufferPos++];

				while (Token!=LPACK_LINEFEED && ptempBuffer[BufferPos]!=NULL)
				{
					Token=ptempBuffer[BufferPos++];
				}
			}
			break;

		default :			
			{
				if (!SentenseOpen )
				{
					if ( Token != 32 && Token !=9 )
						TokenBuffer[TokenBufferPos++]=Token;
				}
				else 
					if ( Token != 9) TokenBuffer[TokenBufferPos++]=Token;
			}break;
		}
	}

	 
	GTH_SAFE_FREE( pFileBuffer );

	return TRUE;
	
}






void CLPack_Quest::Destroy()
{
	CLPack::Destroy();

	for (int i = 0; i < m_vQuestMessage.size(); i ++)
	{
		for (int p = 0; p < m_vQuestMessage[i]->v_StepMessage.size(); p++)
		{
			std::string* pString = NULL;
			pString = (std::string*)m_vQuestMessage[i]->v_StepMessage[p]->Message;;

			SAFE_DELETE (pString);
			SAFE_DELETE(m_vQuestMessage[i]->v_StepMessage[p]);	

			m_vQuestMessage[i]->v_StepMessage[p] = NULL;			
		}		
		m_vQuestMessage[i]->v_StepMessage.clear();
		delete m_vQuestMessage[i];
		m_vQuestMessage[i] = NULL;
	}
	m_vQuestMessage.clear();	

}







void CLPack_Quest::Init()
{
	CLPack::Init();
	m_LastQuestMessageIdx	= 0;
	

}







void CLPack_Etc::Init()
{
	m_vpMessage.reserve(TYPE_COUNT);	
	m_NowvpIdx = 0;	

	for (int i = 0; i < TYPE_COUNT; i++)
	{
		std::vector< void* >* pMessageVector = new std::vector< void* >;
		m_vpMessage.push_back(pMessageVector);
	}		
	
}






void CLPack_Etc::Destroy()
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






bool CLPack_Etc::LoadFile(char* filename)
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
			if ( strcmp ("[NORMAL]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_NORMAL;				
			}
			else if ( strcmp ("[NPC]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_NPC;
			}
			else if ( strcmp ("[QUEST]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_QUESTINFO;
			}
			else if ( strcmp ("[HELP]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_HELP;
			}
			else if ( strcmp ("[ANSWER]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_ANSWER;
			}
			else if ( strcmp ("[ETC]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_ETC;				
			}
			else if ( strcmp ("[ABUSE]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_ABUSE;				
			}			
			
			else if ( strcmp ("[ABUSE2]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_ABUSE2;
			}			
			

			
			else if ( strcmp ("[SKILL_TABLE]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_SKILL;
			}			
			

			else if ( strcmp ("[TIP]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_TIP;				
			}			
			else if ( strcmp ("[TUTOTIAL]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_TUTORIAL;
			}
			else if ( strcmp ("[NORMAL2]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_NORMAL2;
			}
			
			else if ( strcmp ("[AUTOSCROLL]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_AUTOSCROLL;
			}

			
			else if ( strcmp ("[SKILL_ITEM]", TokenBuffer) == 0)
			{
				m_LPackType = LPACK_TYPE_ITEM;
			}			
			
			
			
			else if ( strcmp ("[GEN_ATTRIBUTE]", TokenBuffer) == 0)			
				m_NowvpIdx = LPACK_ETCTYPE_GEN_ATTRIBUTE;
			
			else if ( strcmp ("[PC_JOB]", TokenBuffer) == 0)			
				m_NowvpIdx = LPACK_ETCTYPE_PC_JOB;

			else if ( strcmp ("[WEAPON_TYPE]", TokenBuffer) == 0)			
				m_NowvpIdx = LPACK_ETCTYPE_WEAPON_TYPE;

			else if ( strcmp ("[RARITY]", TokenBuffer) == 0)			
				m_NowvpIdx = LPACK_ETCTYPE_RARITY;

			
			else if ( strcmp ("[PATCH]", TokenBuffer) == 0)			
				m_NowvpIdx = LPACK_ETCTYPE_PATCH;

			else if ( strcmp ("[GolrynBattle]", TokenBuffer) == 0)			
				m_NowvpIdx = LPACK_ETCTYPE_GOLRYUN_BATTLE;

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






int CLPack::GetMessageCount()
{
	return m_vLPackData.size();
}

