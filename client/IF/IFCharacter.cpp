



#include "../global_def.h"
#include "IFCommon.h"
#include "../game/GolryunBattle/GolryunBattle.h"










#define LimitStatusLevel_BeforeAwaken	29
#define LimitStatusLevel_FirstAwaken	69
#define LimitStatusLevel_LastAwaken		99

int PC_LIMIT_STATUS_BEFORE_AWAKEN[8][4]	=
{
	
	{	80,		44,		70,		62	}, 
	{	55,		59,		87,		55	}, 
	{	53,		87,		64,		52	}, 
	{	75,		45,		59,		73	}, 
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
};
int PC_LIMIT_STATUS_FIRST_AWAKEN[8][4]	=
{
	
	{	140,	96,    131,	  124	},
	{	107,   121,	   147,	  116	},
	{	105,   147,	   125,	  114	},
	{	133,	97,	   127,	  137	},
	{	0,		 0,		 0,		0	},
	{	0,		 0,		 0,		0	},
	{	0,		 0,		 0,		0	},
	{	0,		 0,		 0,		0	},
};
int PC_LIMIT_STATUS_LAST_AWAKEN[8][4]	=
{
	
	{	216,	131,	174,	167	},
	{	142,	164,	223,	159	},
	{	140,	223,	168,	157	},
	{	212,	132,	169,	181	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
	{	0,		0,		0,		0	},
};




inline int CheckLimitStatus( int level, int pcJob, int status, int statusType )
{
	int		*limitStatus;	

	
	if( level <= LimitStatusLevel_BeforeAwaken ) limitStatus = PC_LIMIT_STATUS_BEFORE_AWAKEN[pcJob];
	else if( level <= LimitStatusLevel_FirstAwaken ) limitStatus = PC_LIMIT_STATUS_FIRST_AWAKEN[pcJob];
	else if( level <= LimitStatusLevel_LastAwaken ) limitStatus = PC_LIMIT_STATUS_LAST_AWAKEN[pcJob];

	


	if( status >= limitStatus[statusType] ) return false;

	return true;
}


CIFCharWin::CIFCharWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_CHAR_WIN; i ++ )
	{
		m_textCtl[i] = new CIFText( this );
	}
	
	for( i = 0; i < MAX_COUNT_IF_CHAR_WIN_LINE; i ++ )
		m_winLine[i] = new CIFControl( this );
	
	
	m_titleBar = new CIFControl( this );
	m_titleBarBottom = new CIFControl( this );
	m_genMarkSelection[0] = new CIFControl( this );
	m_genMarkSelection[1] = new CIFControl( this );
	m_genMarkSelection[2] = new CIFControl( this );
	m_genMarkSelection[3] = new CIFControl( this );
	m_genMark[0] = new CIFControl( this );
	m_genMark[1] = new CIFControl( this );
	m_genMark[2] = new CIFControl( this );
	m_genMark[3] = new CIFControl( this );
	m_genAttBar = new CIFControl( this );
	m_atkAttBar = new CIFControl( this );
	m_statAttBar = new CIFControl( this );
	
	
	m_raBar = new CIFBarCtl( this );
	m_saBar = new CIFBarCtl( this );
	m_expBar = new CIFBarCtl( this );
	m_genexpBar = new CIFBarCtl( this );

	
	m_closeBtn = new CIFButtonCtl( this );
	m_statsBtn[0] = new CIFButtonCtl( this );
	m_statsBtn[1] = new CIFButtonCtl( this );
	m_statsBtn[2] = new CIFButtonCtl( this );
	m_statsBtn[3] = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_CHAR_WINDOW_TYPE );
}

CIFCharWin::~CIFCharWin()
{
	
}
	
void CIFCharWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_CHAR_WIN );

	m_textCtl[IT_CHAR_WIN_TITLE]->PrepareControl( IF_CHAR_WIN_TITLE_TEXT );
	m_textCtl[IT_CHAR_WIN_NAME_U]->PrepareControl( IF_CHAR_WIN_NAME_TEXT_U );
	m_textCtl[IT_CHAR_WIN_AGE]->PrepareControl( IF_CHAR_WIN_AGE_TEXT );
	m_textCtl[IT_CHAR_WIN_AGE_U]->PrepareControl( IF_CHAR_WIN_AGE_TEXT_U );
	m_textCtl[IT_CHAR_WIN_LEVEL]->PrepareControl( IF_CHAR_WIN_LEVEL_TEXT );
	m_textCtl[IT_CHAR_WIN_LEVEL_U]->PrepareControl( IF_CHAR_WIN_LEVEL_TEXT_U );
	m_textCtl[IT_CHAR_WIN_GENLEVEL]->PrepareControl( IF_CHAR_WIN_GENLEVEL_TEXT );
	m_textCtl[IT_CHAR_WIN_GENLEVEL_U]->PrepareControl( IF_CHAR_WIN_GENLEVEL_TEXT_U );
	m_textCtl[IT_CHAR_WIN_FAME]->PrepareControl( IF_CHAR_WIN_FAME_TEXT );
	m_textCtl[IT_CHAR_WIN_FAME_U]->PrepareControl( IF_CHAR_WIN_FAME_TEXT_U );
	m_textCtl[IT_CHAR_WIN_RA]->PrepareControl( IF_CHAR_WIN_RA_TEXT );
	m_textCtl[IT_CHAR_WIN_SA]->PrepareControl( IF_CHAR_WIN_SA_TEXT );
	m_textCtl[IT_CHAR_WIN_RA_U]->PrepareControl( IF_CHAR_WIN_RA_TEXT_U );
	m_textCtl[IT_CHAR_WIN_SA_U]->PrepareControl( IF_CHAR_WIN_SA_TEXT_U );
	m_textCtl[IT_CHAR_WIN_EXP]->PrepareControl( IF_CHAR_WIN_EXP_TEXT );
	m_textCtl[IT_CHAR_WIN_GENEXP]->PrepareControl( IF_CHAR_WIN_GENEXP_TEXT );
	m_textCtl[IT_CHAR_WIN_EXP_U]->PrepareControl( IF_CHAR_WIN_EXP_TEXT_U );
	m_textCtl[IT_CHAR_WIN_GENEXP_U]->PrepareControl( IF_CHAR_WIN_GENEXP_TEXT_U );
	m_textCtl[IT_CHAR_WIN_GEN_ATT]->PrepareControl( IF_CHAR_WIN_GEN_ATT_TEXT );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_0]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_0 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_1]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_1 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_2]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_2 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_3]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_3 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_0]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_U_0 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_1]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_U_1 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_2]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_U_2 );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_3]->PrepareControl( IF_CHAR_WIN_GEN_GRADE_TEXT_U_3 );
	m_textCtl[IT_CHAR_WIN_ATK_ATT]->PrepareControl( IF_CHAR_WIN_ATK_ATT_TEXT );
	m_textCtl[IT_CHAR_WIN_ATK]->PrepareControl( IF_CHAR_WIN_ATK_TEXT );
	m_textCtl[IT_CHAR_WIN_ATK_U]->PrepareControl( IF_CHAR_WIN_ATK_TEXT_U );
	m_textCtl[IT_CHAR_WIN_MAG]->PrepareControl( IF_CHAR_WIN_MAG_TEXT );
	m_textCtl[IT_CHAR_WIN_MAG_U]->PrepareControl( IF_CHAR_WIN_MAG_TEXT_U );
	m_textCtl[IT_CHAR_WIN_DEF]->PrepareControl( IF_CHAR_WIN_DEF_TEXT );
	m_textCtl[IT_CHAR_WIN_DEF_U]->PrepareControl( IF_CHAR_WIN_DEF_TEXT_U );
	m_textCtl[IT_CHAR_WIN_PRES]->PrepareControl( IF_CHAR_WIN_PRES_TEXT );
	m_textCtl[IT_CHAR_WIN_PRES_U]->PrepareControl( IF_CHAR_WIN_PRES_TEXT_U );
	m_textCtl[IT_CHAR_WIN_NRES]->PrepareControl( IF_CHAR_WIN_NRES_TEXT );
	m_textCtl[IT_CHAR_WIN_NRES_U]->PrepareControl( IF_CHAR_WIN_NRES_TEXT_U );
	m_textCtl[IT_CHAR_WIN_ATR]->PrepareControl( IF_CHAR_WIN_ATR_TEXT );
	m_textCtl[IT_CHAR_WIN_ATR_U]->PrepareControl( IF_CHAR_WIN_ATR_TEXT_U );
	m_textCtl[IT_CHAR_WIN_AVO]->PrepareControl( IF_CHAR_WIN_AVO_TEXT );
	m_textCtl[IT_CHAR_WIN_AVO_U]->PrepareControl( IF_CHAR_WIN_AVO_TEXT_U );
	m_textCtl[IT_CHAR_WIN_ATS]->PrepareControl( IF_CHAR_WIN_ATS_TEXT );
	m_textCtl[IT_CHAR_WIN_ATS_U]->PrepareControl( IF_CHAR_WIN_ATS_TEXT_U );
	m_textCtl[IT_CHAR_WIN_VEL]->PrepareControl( IF_CHAR_WIN_VEL_TEXT );
	m_textCtl[IT_CHAR_WIN_VEL_U]->PrepareControl( IF_CHAR_WIN_VEL_TEXT_U );
	m_textCtl[IT_CHAR_WIN_CRI]->PrepareControl( IF_CHAR_WIN_CRI_TEXT );
	m_textCtl[IT_CHAR_WIN_CRI_U]->PrepareControl( IF_CHAR_WIN_CRI_TEXT_U );
	m_textCtl[IT_CHAR_WIN_STAT]->PrepareControl( IF_CHAR_WIN_STAT_TEXT );
	m_textCtl[IT_CHAR_WIN_STAT_POINT]->PrepareControl( IF_CHAR_WIN_STAT_POINT_TEXT );
	m_textCtl[IT_CHAR_WIN_STAT_POINT_U]->PrepareControl( IF_CHAR_WIN_STAT_POINT_TEXT_U );
	m_textCtl[IT_CHAR_WIN_FOR_POINT]->PrepareControl( IF_CHAR_WIN_FOR_POINT_TEXT );
	m_textCtl[IT_CHAR_WIN_FOR_POINT_U]->PrepareControl( IF_CHAR_WIN_FOR_POINT_TEXT_U );
	m_textCtl[IT_CHAR_WIN_AGI_POINT]->PrepareControl( IF_CHAR_WIN_AGI_POINT_TEXT );
	m_textCtl[IT_CHAR_WIN_AGI_POINT_U]->PrepareControl( IF_CHAR_WIN_AGI_POINT_TEXT_U );
	m_textCtl[IT_CHAR_WIN_SOU_POINT]->PrepareControl( IF_CHAR_WIN_SOU_POINT_TEXT );
	m_textCtl[IT_CHAR_WIN_SOU_POINT_U]->PrepareControl( IF_CHAR_WIN_SOU_POINT_TEXT_U );
	m_textCtl[IT_CHAR_WIN_VIT_POINT]->PrepareControl( IF_CHAR_WIN_VIT_POINT_TEXT );
	m_textCtl[IT_CHAR_WIN_VIT_POINT_U]->PrepareControl( IF_CHAR_WIN_VIT_POINT_TEXT_U );		

	
	m_titleBar->PrepareControl( IF_CHAR_WIN_TITLE_BAR );
	m_titleBarBottom->PrepareControl( IF_CHAR_WIN_TITLE_BAR_BOTTOM );
	m_genMarkSelection[0]->PrepareControl( IF_CHAR_WIN_GEN_MARK_SELECT_0 );
	m_genMarkSelection[1]->PrepareControl( IF_CHAR_WIN_GEN_MARK_SELECT_1 );
	m_genMarkSelection[2]->PrepareControl( IF_CHAR_WIN_GEN_MARK_SELECT_2 );
	m_genMarkSelection[3]->PrepareControl( IF_CHAR_WIN_GEN_MARK_SELECT_3 );
	m_genMark[0]->PrepareControl( IF_CHAR_WIN_PGEN_MARK_0_0 );
	m_genMark[1]->PrepareControl( IF_CHAR_WIN_PGEN_MARK_1_0 );
	m_genMark[2]->PrepareControl( IF_CHAR_WIN_PGEN_MARK_2_0 );
	m_genMark[3]->PrepareControl( IF_CHAR_WIN_PGEN_MARK_3_0 );
	m_genAttBar->PrepareControl( IF_CHAR_WIN_GEN_ATT_BAR );
	m_atkAttBar->PrepareControl( IF_CHAR_WIN_ATK_ATT_BAR );
	m_statAttBar->PrepareControl( IF_CHAR_WIN_STAT_BAR );	
	
	for( int i = 0; i < MAX_COUNT_IF_CHAR_WIN_LINE; i ++ )
		m_winLine[i]->PrepareControl( IF_CHAR_WIN_RLINE_NAME + i );
	
	
	m_raBar->PrepareControl( IF_CHAR_WIN_RA_BAR, IF_CHAR_WIN_RA_BAR_EMPTY );
	m_saBar->PrepareControl( IF_CHAR_WIN_SA_BAR, IF_CHAR_WIN_SA_BAR_EMPTY );
	m_expBar->PrepareControl( IF_CHAR_WIN_EXP_BAR, IF_CHAR_WIN_EXP_BAR_EMPTY );
	m_genexpBar->PrepareControl( IF_CHAR_WIN_GENEXP_BAR, IF_CHAR_WIN_GENEXP_BAR_EMPTY );

	
	m_closeBtn->PrepareControl( IF_CHAR_WIN_CLOSE_BTN_0 );
	m_statsBtn[0]->PrepareControl( IF_CHAR_WIN_FOR_BTN_0 );
	m_statsBtn[1]->PrepareControl( IF_CHAR_WIN_SOU_BTN_0 );
	m_statsBtn[2]->PrepareControl( IF_CHAR_WIN_AGI_BTN_0 );
	m_statsBtn[3]->PrepareControl( IF_CHAR_WIN_VIT_BTN_0 );
}

int	CIFCharWin::WindowFrame()
{	

	char szTemp[256];

	int expLevel = g_cgv.myCharacterInfo->level-1;
	int genExpLevel = g_cgv.myCharacterInfo->genLevel-1;
	__int64 curExp, nextExp;
	__int64 curGenExp, nextGenExp;
	float expRate, genExpRate;

	
	if( g_expTable[expLevel].totalexp < 0 )
	{
		curExp = 1;
		nextExp = 1;
	}
	else
	{
		curExp = g_expTable[expLevel].totalexp - g_cgv.myCharacterInfo->exp;
		nextExp = g_expTable[expLevel].exp;
	}

	if( g_expTable[genExpLevel].totalGenExp < 0 )
	{
		curGenExp = 1;
		nextGenExp = 1;
	}
	else
	{
		curGenExp = g_expTable[genExpLevel].totalGenExp - g_cgv.myCharacterInfo->genExp;
		nextGenExp = g_expTable[genExpLevel].genExp;
	}




	
	
	float fCurentExp = curExp/1000.0f;
	float fNextExp = nextExp/1000.0f;

	float fCurentGenExp = curGenExp/1000.0f;
	float fNextGenExp = nextGenExp/1000.0f;

	if( nextExp > 0 ) expRate = ( 1.0f - ( fCurentExp / fNextExp ) ) * 100.0f;
	if( nextGenExp > 0 ) genExpRate = ( 1.0f - ( fCurentGenExp / fNextGenExp ) ) * 100.0f;
	

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 457 ) );
	m_textCtl[IT_CHAR_WIN_TITLE]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_TITLE]->SetColor( 0xffffffff );

	
	sprintf( szTemp, "%s", g_cgv.myCharacterInfo->name );
	m_textCtl[IT_CHAR_WIN_NAME_U]->SetText( szTemp );
	sprintf( szTemp, g_LPACK.GetMassage( 0, 458 ) );
	m_textCtl[IT_CHAR_WIN_AGE]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->age / 3600 );
	m_textCtl[IT_CHAR_WIN_AGE_U]->SetText( szTemp );

	sprintf( szTemp, "LV." );
	m_textCtl[IT_CHAR_WIN_LEVEL]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->level );
	m_textCtl[IT_CHAR_WIN_LEVEL_U]->SetText( szTemp );
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 459 ) );
	m_textCtl[IT_CHAR_WIN_GENLEVEL]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->genLevel );
	m_textCtl[IT_CHAR_WIN_GENLEVEL_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 460 ) );
	m_textCtl[IT_CHAR_WIN_FAME]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->rankPoint );
	m_textCtl[IT_CHAR_WIN_FAME_U]->SetText( szTemp );

	sprintf( szTemp, "RA" );
	m_textCtl[IT_CHAR_WIN_RA]->SetText( szTemp );
	sprintf( szTemp, "SA" );
	m_textCtl[IT_CHAR_WIN_SA]->SetText( szTemp );
	sprintf( szTemp, "%d/%d", g_cgv.myCharacterInfo->curRA, g_cgv.myCharacterInfo->calMaxRA );
	m_textCtl[IT_CHAR_WIN_RA_U]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_RA_U]->SetColor( 0xffffffff );
	sprintf( szTemp, "%d/%d", g_cgv.myCharacterInfo->curSA, g_cgv.myCharacterInfo->calMaxSA );
	m_textCtl[IT_CHAR_WIN_SA_U]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_SA_U]->SetColor( 0xffffffff );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 461 ) );
	m_textCtl[IT_CHAR_WIN_EXP]->SetText( szTemp );
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 462 ) );
	m_textCtl[IT_CHAR_WIN_GENEXP]->SetText( szTemp );

	sprintf( szTemp, "%.2f%%", expRate );
	m_textCtl[IT_CHAR_WIN_EXP_U]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_EXP_U]->SetColor( 0xffffffff );
	sprintf( szTemp, "%.2f%%", genExpRate );
	m_textCtl[IT_CHAR_WIN_GENEXP_U]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_GENEXP_U]->SetColor( 0xffffffff );
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 463 ) );
	m_textCtl[IT_CHAR_WIN_GEN_ATT]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_GEN_ATT]->SetColor( 0xffffffff );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_0]->SetText( szTemp );
	sprintf( szTemp, g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_1]->SetText( szTemp );
	sprintf( szTemp, g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_2]->SetText( szTemp );
	sprintf( szTemp, g_LPACK.GetMassage( 0, 464 ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_3]->SetText( szTemp );

	
	
	
	int pcClass		=	g_cgv.myCharacterInfo->pcClass;
	int pcJob		=	g_cgv.myCharacterInfo->pcJob;

	if( !CheckAvailableGenCapability( g_cgv.myCharacterInfo, J_MENTALITY ) ) sprintf( szTemp, g_LPACK.GetMassage( 0, 465 ) );
	else strcpy( szTemp, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->genCapability[0] ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_0]->SetText( szTemp );

	if( !CheckAvailableGenCapability( g_cgv.myCharacterInfo, J_WIND ) ) sprintf( szTemp, g_LPACK.GetMassage( 0, 465 ) );
	else strcpy( szTemp, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->genCapability[1] ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_1]->SetText( szTemp );

	if( !CheckAvailableGenCapability( g_cgv.myCharacterInfo, J_WATER ) ) sprintf( szTemp, g_LPACK.GetMassage( 0, 465 ) );
	else strcpy( szTemp, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->genCapability[2] ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_2]->SetText( szTemp );

	if( !CheckAvailableGenCapability( g_cgv.myCharacterInfo, J_EARTH ) ) sprintf( szTemp, g_LPACK.GetMassage( 0, 465 ) );
	else strcpy( szTemp, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->genCapability[3] ) );
	m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_3]->SetText( szTemp );

	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 466 ) );
	m_textCtl[IT_CHAR_WIN_ATK_ATT]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_ATK_ATT]->SetColor( 0xffffffff );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 467 ) );
	m_textCtl[IT_CHAR_WIN_ATK]->SetText( szTemp );
	sprintf( szTemp, "%d~%d", g_cgv.myCharacterInfo->calPhyMinDmg, g_cgv.myCharacterInfo->calPhyMaxDmg );
	m_textCtl[IT_CHAR_WIN_ATK_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 468 ) );
	m_textCtl[IT_CHAR_WIN_MAG]->SetText( szTemp );
	sprintf( szTemp, "%d~%d", g_cgv.myCharacterInfo->calMagMinDmg, g_cgv.myCharacterInfo->calMagMaxDmg );
	m_textCtl[IT_CHAR_WIN_MAG_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 469 ) );
	m_textCtl[IT_CHAR_WIN_DEF]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calDefensePower );
	m_textCtl[IT_CHAR_WIN_DEF_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 470 ) );
	m_textCtl[IT_CHAR_WIN_PRES]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calPositiveRes );
	m_textCtl[IT_CHAR_WIN_PRES_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 471 ) );
	m_textCtl[IT_CHAR_WIN_NRES]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calNegativeRes );
	m_textCtl[IT_CHAR_WIN_NRES_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 472 ) );
	m_textCtl[IT_CHAR_WIN_ATR]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calHitPower );
	m_textCtl[IT_CHAR_WIN_ATR_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 473 ) );
	m_textCtl[IT_CHAR_WIN_AVO]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calAvoidPower );
	m_textCtl[IT_CHAR_WIN_AVO_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 474 ) );
	m_textCtl[IT_CHAR_WIN_ATS]->SetText( szTemp );
	if( g_cgv.myCharacterInfo->calAttackDelay < 500 )
	{
		sprintf( szTemp, "0" );
	}
	else
	{
		

		
		sprintf( szTemp, "%d", 2500 -  g_cgv.myCharacterInfo->calAttackDelay );
		
		
	}
	m_textCtl[IT_CHAR_WIN_ATS_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 475 ) );
	m_textCtl[IT_CHAR_WIN_VEL]->SetText( szTemp );
	sprintf( szTemp, "%d", (int)( g_cgv.myCharacterInfo->calVelocity * 100.0f ) );
	m_textCtl[IT_CHAR_WIN_VEL_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 476 ) );
	m_textCtl[IT_CHAR_WIN_CRI]->SetText( szTemp );
	sprintf( szTemp, "%d", (int)( g_cgv.myCharacterInfo->calCriticalRate * 100.0f ) );
	m_textCtl[IT_CHAR_WIN_CRI_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 477 ) );
	m_textCtl[IT_CHAR_WIN_STAT]->SetText( szTemp );
	m_textCtl[IT_CHAR_WIN_STAT]->SetColor( 0xffffffff );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 478 ) );
	m_textCtl[IT_CHAR_WIN_STAT_POINT]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->bonusStatusPoint );
	m_textCtl[IT_CHAR_WIN_STAT_POINT_U]->SetText( szTemp );

	
	
	m_textCtl[IT_CHAR_WIN_FOR_POINT_U]->SetColor( 0xffffffff );
	m_textCtl[IT_CHAR_WIN_SOU_POINT_U]->SetColor( 0xffffffff );
	m_textCtl[IT_CHAR_WIN_AGI_POINT_U]->SetColor( 0xffffffff );
	m_textCtl[IT_CHAR_WIN_VIT_POINT_U]->SetColor( 0xffffffff );

	if( g_cgv.myCharacterInfo->bonusStatusPoint > 0 )
	{
		m_statsBtn[0]->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_statsBtn[1]->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_statsBtn[2]->SetButtonState( IF_BTN_TYPE_ENABLE );
		m_statsBtn[3]->SetButtonState( IF_BTN_TYPE_ENABLE );
	}
	if( !CheckLimitStatus( g_cgv.myCharacterInfo->level, g_cgv.myCharacterInfo->pcClass, g_cgv.myCharacterInfo->calForce, STATUS_FORCE ) )
	{
		m_textCtl[IT_CHAR_WIN_FOR_POINT_U]->SetColor( 0xffffd700 );
		m_statsBtn[0]->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	if( !CheckLimitStatus( g_cgv.myCharacterInfo->level, g_cgv.myCharacterInfo->pcClass, g_cgv.myCharacterInfo->calSoulpower, STATUS_SOULPOWER ) )
	{
		m_textCtl[IT_CHAR_WIN_SOU_POINT_U]->SetColor( 0xffffd700 );
		m_statsBtn[1]->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	if( !CheckLimitStatus( g_cgv.myCharacterInfo->level, g_cgv.myCharacterInfo->pcClass, g_cgv.myCharacterInfo->calAgility, STATUS_AGILITY ) )
	{
		m_textCtl[IT_CHAR_WIN_AGI_POINT_U]->SetColor( 0xffffd700 );
		m_statsBtn[2]->SetButtonState( IF_BTN_TYPE_DISABLE );
	}
	if( !CheckLimitStatus( g_cgv.myCharacterInfo->level, g_cgv.myCharacterInfo->pcClass, g_cgv.myCharacterInfo->calVitality, STATUS_VITALITY ) )
	{
		m_textCtl[IT_CHAR_WIN_VIT_POINT_U]->SetColor( 0xffffd700 );
		m_statsBtn[3]->SetButtonState( IF_BTN_TYPE_DISABLE );
	}

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 479 ));
	m_textCtl[IT_CHAR_WIN_FOR_POINT]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calForce );
	m_textCtl[IT_CHAR_WIN_FOR_POINT_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 480 ) );
	m_textCtl[IT_CHAR_WIN_AGI_POINT]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calAgility );
	m_textCtl[IT_CHAR_WIN_AGI_POINT_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 481 ) );
	m_textCtl[IT_CHAR_WIN_SOU_POINT]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calSoulpower );
	m_textCtl[IT_CHAR_WIN_SOU_POINT_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 482 ) );
	m_textCtl[IT_CHAR_WIN_VIT_POINT]->SetText( szTemp );
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->calVitality );
	m_textCtl[IT_CHAR_WIN_VIT_POINT_U]->SetText( szTemp );

	m_expBar->UpdateBarState( (int)( expRate * 100.0f ), 10000 );
	m_genexpBar->UpdateBarState( (int)( genExpRate * 100.0f), 10000 );

	m_raBar->UpdateBarState( g_cgv.myCharacterInfo->curRA, g_cgv.myCharacterInfo->calMaxRA );
	m_saBar->UpdateBarState( g_cgv.myCharacterInfo->curSA, g_cgv.myCharacterInfo->calMaxSA );

	for( int i = 0; i < 4; i ++ )
	{
		m_genMarkSelection[i]->Enable( false );
	}
	m_genMarkSelection[g_cgv.myCharacterInfo->selectedGen]->Enable();

	for( i = 0; i < 4; i ++ )
	{
		
		
		if( CheckAvailableGenCapability( g_cgv.myCharacterInfo, i ) )
		{
			m_genMark[i]->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_0_0 + i * 2 );
		}
		else
		{
			m_genMark[i]->ChangeControlTexInfo( IF_CHAR_WIN_PGEN_MARK_0_0 + i * 2 + 1 );
		}
	}

	return( IF_CTL_NONE );
}

void CIFCharWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	
	CIFControl::Render();

	
	m_titleBar->Render();
	m_titleBarBottom->Render();
	m_genMarkSelection[0]->Render();
	m_genMarkSelection[1]->Render();
	m_genMarkSelection[2]->Render();
	m_genMarkSelection[3]->Render();
	m_genMark[0]->Render();
	m_genMark[1]->Render();
	m_genMark[2]->Render();
	m_genMark[3]->Render();
	m_genAttBar->Render();
	m_atkAttBar->Render();
	m_statAttBar->Render();
	for( int i = 0; i < MAX_COUNT_IF_CHAR_WIN_LINE; i ++ )
		m_winLine[i]->Render();

	
	m_raBar->Render();
	m_saBar->Render();
	m_expBar->Render();
	m_genexpBar->Render();

	
	m_closeBtn->Render();
	m_statsBtn[0]->Render();
	m_statsBtn[1]->Render();
	m_statsBtn[2]->Render();
	m_statsBtn[3]->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		
		m_textCtl[IT_CHAR_WIN_TITLE]->Render();
		m_textCtl[IT_CHAR_WIN_NAME_U]->Render();
		m_textCtl[IT_CHAR_WIN_AGE]->Render();
		m_textCtl[IT_CHAR_WIN_AGE_U]->Render();
		m_textCtl[IT_CHAR_WIN_LEVEL]->Render();
		m_textCtl[IT_CHAR_WIN_LEVEL_U]->Render();
		m_textCtl[IT_CHAR_WIN_GENLEVEL]->Render();
		m_textCtl[IT_CHAR_WIN_GENLEVEL_U]->Render();
		m_textCtl[IT_CHAR_WIN_FAME]->Render();
		m_textCtl[IT_CHAR_WIN_FAME_U]->Render();
		m_textCtl[IT_CHAR_WIN_RA]->Render();
		m_textCtl[IT_CHAR_WIN_SA]->Render();
		g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
		m_textCtl[IT_CHAR_WIN_RA_U]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_CHAR_WIN_SA_U]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_CHAR_WIN_EXP_U]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_CHAR_WIN_GENEXP_U]->Render( IF_TEXT_OUTPUT_CENTER );
		g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
		m_textCtl[IT_CHAR_WIN_EXP]->Render();
		m_textCtl[IT_CHAR_WIN_GENEXP]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_ATT]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_0]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_1]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_2]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_3]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_0]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_1]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_2]->Render();
		m_textCtl[IT_CHAR_WIN_GEN_GRADE_U_3]->Render();
		m_textCtl[IT_CHAR_WIN_ATK_ATT]->Render();
		m_textCtl[IT_CHAR_WIN_ATK]->Render();
		m_textCtl[IT_CHAR_WIN_ATK_U]->Render();
		m_textCtl[IT_CHAR_WIN_MAG]->Render();
		m_textCtl[IT_CHAR_WIN_MAG_U]->Render();
		m_textCtl[IT_CHAR_WIN_DEF]->Render();
		m_textCtl[IT_CHAR_WIN_DEF_U]->Render();
		m_textCtl[IT_CHAR_WIN_PRES]->Render();
		m_textCtl[IT_CHAR_WIN_PRES_U]->Render();
		m_textCtl[IT_CHAR_WIN_NRES]->Render();
		m_textCtl[IT_CHAR_WIN_NRES_U]->Render();
		m_textCtl[IT_CHAR_WIN_ATR]->Render();
		m_textCtl[IT_CHAR_WIN_ATR_U]->Render();
		m_textCtl[IT_CHAR_WIN_AVO]->Render();
		m_textCtl[IT_CHAR_WIN_AVO_U]->Render();
		m_textCtl[IT_CHAR_WIN_ATS]->Render();
		m_textCtl[IT_CHAR_WIN_ATS_U]->Render();
		m_textCtl[IT_CHAR_WIN_VEL]->Render();
		m_textCtl[IT_CHAR_WIN_VEL_U]->Render();
		m_textCtl[IT_CHAR_WIN_CRI]->Render();
		m_textCtl[IT_CHAR_WIN_CRI_U]->Render();
		m_textCtl[IT_CHAR_WIN_STAT]->Render();
		m_textCtl[IT_CHAR_WIN_STAT_POINT]->Render();
		m_textCtl[IT_CHAR_WIN_STAT_POINT_U]->Render();
		m_textCtl[IT_CHAR_WIN_FOR_POINT]->Render();
		m_textCtl[IT_CHAR_WIN_FOR_POINT_U]->Render();
		m_textCtl[IT_CHAR_WIN_AGI_POINT]->Render();
		m_textCtl[IT_CHAR_WIN_AGI_POINT_U]->Render();
		m_textCtl[IT_CHAR_WIN_SOU_POINT]->Render();
		m_textCtl[IT_CHAR_WIN_SOU_POINT_U]->Render();
		m_textCtl[IT_CHAR_WIN_VIT_POINT]->Render();
		m_textCtl[IT_CHAR_WIN_VIT_POINT_U]->Render();
	}
	g_chatManager->EndFrame();

}

int CIFCharWin::WindowSelection()
{
	int ret;
	
	if( !State() ) return( IF_CTL_NONE );

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFCharWin::WindowProcess()
{
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}
	if( m_statsBtn[0]->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestStatusUp( 0 );
	if( m_statsBtn[1]->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestStatusUp( 1 );
	if( m_statsBtn[2]->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestStatusUp( 2 );
	if( m_statsBtn[3]->ProcessButton() == IF_CTL_SELECT ) GTH_SendMessage_RequestStatusUp( 3 );

	for( int i = 0; i < 4; i ++ )
	{
		if( m_genMark[i]->Selection() == IF_LBUP )
		{
			
			
			if( CheckAvailableGenCapability( g_cgv.myCharacterInfo, i ) )
			{
				GTH_SendMessage_RequestGenChange( i );
			}
		}
	}
	return IF_CTL_NONE;
}












#define TARGET_NAME_VERY_EASY_COLOR		D3DCOLOR_ARGB( 255, 0, 255, 0 )
#define TARGET_NAME_EASY_COLOR			D3DCOLOR_ARGB( 255, 100, 255, 100 )
#define TARGET_NAME_NORMAL_COLOR		D3DCOLOR_ARGB( 255, 255, 255, 255 )
#define TARGET_NAME_HARD_COLOR			D3DCOLOR_ARGB( 255, 238, 249, 100 )
#define TARGET_NAME_VERY_HARD_COLOR		D3DCOLOR_ARGB( 255, 255, 255, 0 )

CIFNameWin::CIFNameWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	Enable( true );
	for( int i = 0; i < NUM_IT_NAME_WIN; i ++ )
	{
		m_textCtl[i] = new CIFText( this );
	}
	m_raBar = new CIFBarCtl( this );
	m_saBar = new CIFBarCtl( this );
	m_expBar = new CIFBarCtl( this );
	m_genexpBar = new CIFBarCtl( this );
	
	m_targetWin = new CIFControl( this );
	m_targetBar = new CIFBarCtl( this );
	m_castingBar = new CIFBarCtl( this );
	m_skillIconOutlineCtl = new CIFControl( this );

	
	m_destroyUpper = new CIFControl( this );
	m_destroyLower = new CIFControl( this );
	m_destroyBoots = new CIFControl( this );
	m_destroyGloves = new CIFControl( this );
	m_destroyProtector = new CIFControl( this );
	m_destroyWeapon = new CIFControl( this );
	
	m_leftSelectCharSphere = new CIFControl( this );
	m_rightSelectCharSphere = new CIFControl( this );

	m_targetWin->Enable( false );
	m_targetBar->Enable( false );
	m_castingBar->Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_NAME_WINDOW_TYPE );

	m_numAffectSkill = 0;
}

CIFNameWin::~CIFNameWin()
{

}

void CIFNameWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_NAME_WIN );
	
	
	m_textCtl[IT_NAME_WIN_LEVEL]->PrepareControl( IF_NAME_WIN_LEVEL_TEXT );
	m_textCtl[IT_NAME_WIN_LEVEL_U]->PrepareControl( IF_NAME_WIN_LEVEL_TEXT_U );
	m_textCtl[IT_NAME_WIN_NAME_U]->PrepareControl( IF_NAME_WIN_NAME_TEXT_U );
	m_textCtl[IT_NAME_WIN_RA_U]->PrepareControl( IF_NAME_WIN_RA_TEXT_U );
	m_textCtl[IT_NAME_WIN_SA_U]->PrepareControl( IF_NAME_WIN_SA_TEXT_U );
	m_textCtl[IT_NAME_WIN_GENLEVEL]->PrepareControl( IF_NAME_WIN_GENLEVEL_TEXT );
	m_textCtl[IT_NAME_WIN_GENLEVEL_U]->PrepareControl( IF_NAME_WIN_GENLEVEL_TEXT_U );
	m_textCtl[IT_NAME_WIN_EXP_TEXT_U]->PrepareControl( IF_NAME_WIN_EXP_TEXT_U );
	m_textCtl[IT_NAME_WIN_GENEXP_TEXT_U]->PrepareControl( IF_NAME_WIN_GENEXP_TEXT_U );
	m_textCtl[IT_TARGET_WIN_NAME_U]->PrepareControl( IF_TARGET_WIN_NAME_TEXT_U );

	
	m_raBar->PrepareControl( IF_NAME_WIN_RA_BAR );
	m_saBar->PrepareControl( IF_NAME_WIN_SA_BAR );
	m_expBar->PrepareControl( IF_NAME_WIN_EXP_BAR );
	m_genexpBar->PrepareControl( IF_NAME_WIN_GENEXP_BAR );
	
	m_targetWin->PrepareControl( IF_TARGET_WIN );
	m_targetBar->PrepareControl( IF_TARGET_WIN_RA_BAR );
	m_castingBar->PrepareControl( IF_NAME_WIN_SA_BAR );
	m_skillIconOutlineCtl->PrepareControl( IF_NAME_WIN_SKILLICON_OUTLINE );

	
	m_destroyUpper->PrepareControl( IF_DESTROY_UPPER );
	m_destroyLower->PrepareControl( IF_DESTROY_LOWER );
	m_destroyBoots->PrepareControl( IF_DESTROY_BOOTS );
	m_destroyGloves->PrepareControl( IF_DESTROY_GLOVES );
	m_destroyProtector->PrepareControl( IF_DESTROY_PROTECTOR );
	m_destroyWeapon->PrepareControl( IF_DESTROY_WEAPON );

	m_leftSelectCharSphere->PrepareControl( IF_SELECTCHAR_SPHERE );
	m_rightSelectCharSphere->PrepareControl( IF_SELECTCHAR_SPHERE );
	m_leftSelectCharSphere->Enable( false );
	m_rightSelectCharSphere->Enable( false );
}

int CIFNameWin::WindowFrame()
{
	int expLevel = g_cgv.myCharacterInfo->level-1;
	int genExpLevel = g_cgv.myCharacterInfo->genLevel-1;
	__int64 curExp, nextExp;
	__int64 curGenExp, nextGenExp;
	float expRate, genExpRate;

	curExp = g_expTable[expLevel].totalexp - g_cgv.myCharacterInfo->exp;
	nextExp = g_expTable[expLevel].exp;

	curGenExp = g_expTable[genExpLevel].totalGenExp - g_cgv.myCharacterInfo->genExp;
	nextGenExp = g_expTable[genExpLevel].genExp;




	
	
	float fCurentExp = curExp/1000.0f;
	float fNextExp = nextExp/1000.0f;

	float fCurentGenExp = curGenExp/1000.0f;
	float fNextGenExp = nextGenExp/1000.0f;

	if( nextExp > 0 ) expRate = ( 1.0f - ( fCurentExp / fNextExp ) ) * 100.0f;
	if( nextGenExp > 0 ) genExpRate = ( 1.0f - ( fCurentGenExp / fNextGenExp ) ) * 100.0f;

	g_cgv.myCharacterInfo->curRA = g_pApp->m_myCharacter->curRA;
	g_cgv.myCharacterInfo->curSA = g_pApp->m_myCharacter->curSA;
	g_cgv.myCharacterInfo->calMaxRA = g_pApp->m_myCharacter->calMaxRA;
	g_cgv.myCharacterInfo->calMaxSA = g_pApp->m_myCharacter->calMaxSA;


	


	
	
	
	char szTemp[256];
	
	sprintf( szTemp, "LV." );
	m_textCtl[IT_NAME_WIN_LEVEL]->SetText( szTemp );

	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->level );
	m_textCtl[IT_NAME_WIN_LEVEL_U]->SetText( szTemp );

	sprintf( szTemp, "%s", g_cgv.myCharacterInfo->name );
	m_textCtl[IT_NAME_WIN_NAME_U]->SetText( szTemp );

	sprintf( szTemp, "RA  %d/%d", g_cgv.myCharacterInfo->curRA, g_cgv.myCharacterInfo->calMaxRA );
	m_textCtl[IT_NAME_WIN_RA_U]->SetText( szTemp );		

	sprintf( szTemp, "SA  %d/%d", g_cgv.myCharacterInfo->curSA, g_cgv.myCharacterInfo->calMaxSA );
	m_textCtl[IT_NAME_WIN_SA_U]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 459 ) );
	m_textCtl[IT_NAME_WIN_GENLEVEL]->SetText( szTemp );
	
	sprintf( szTemp, "%d", g_cgv.myCharacterInfo->genLevel );
	m_textCtl[IT_NAME_WIN_GENLEVEL_U]->SetText( szTemp );
	
	sprintf( szTemp, "%.2f%%", expRate );
	m_textCtl[IT_NAME_WIN_EXP_TEXT_U]->SetText( szTemp );
	sprintf( szTemp, "%.2f%%", genExpRate );
	m_textCtl[IT_NAME_WIN_GENEXP_TEXT_U]->SetText( szTemp );	

	if( g_cgv.pTargetChar != NULL )
	{
		if( g_cgv.pTargetChar->entityType != ENTITY_MONSTER ) 
		{
			m_textCtl[IT_TARGET_WIN_NAME_U]->SetColor( 0xffffffff );


			g_ifMng->RenderChar_SetChrName(szTemp,g_cgv.pTargetChar);

			
				
			
		}
		else
		{
			int levelDiffer = g_monsterTable[g_cgv.pTargetChar->tableIdx].level - g_cgv.myCharacterInfo->level;
			if( levelDiffer <= -5 ) m_textCtl[IT_TARGET_WIN_NAME_U]->SetColor( TARGET_NAME_VERY_EASY_COLOR );
			else if( levelDiffer <= -1 ) m_textCtl[IT_TARGET_WIN_NAME_U]->SetColor( TARGET_NAME_EASY_COLOR );
			else if( levelDiffer == 0 ) m_textCtl[IT_TARGET_WIN_NAME_U]->SetColor( TARGET_NAME_NORMAL_COLOR );
			else if( levelDiffer >= 5 ) m_textCtl[IT_TARGET_WIN_NAME_U]->SetColor( TARGET_NAME_VERY_HARD_COLOR );
			else if( levelDiffer >= 1 ) m_textCtl[IT_TARGET_WIN_NAME_U]->SetColor( TARGET_NAME_HARD_COLOR );

			
			if( g_cgv.pTargetChar->summonValidTime > 0 && g_cgv.pTargetChar->stat_alive == MON_ALIVE_STATE_NORMAL)
				sprintf( szTemp, "%s (%dSec)", g_cgv.targetName, (int)(g_cgv.pTargetChar->summonValidTime/1000) );
			
			

			
			
			
			
			else
				sprintf( szTemp, "%s", g_cgv.targetName );
		}
		
		m_textCtl[IT_TARGET_WIN_NAME_U]->SetText( szTemp );
		m_targetWin->Enable();
		m_targetBar->Enable();
		m_targetBar->UpdateBarState( g_cgv.pTargetChar->curRA, g_cgv.pTargetChar->calMaxRA );

	}
	else
	{
		m_targetWin->Enable( false );
		m_targetBar->Enable( false );

	}

	m_raBar->UpdateBarState( g_cgv.myCharacterInfo->curRA, g_cgv.myCharacterInfo->calMaxRA );
	m_saBar->UpdateBarState( g_cgv.myCharacterInfo->curSA, g_cgv.myCharacterInfo->calMaxSA );

	m_expBar->UpdateBarState( (int)( expRate * 100.0f ), 10000 );
	m_genexpBar->UpdateBarState( (int)( genExpRate * 100.0f), 10000 );

	
	if( g_pApp->m_myCharacter->event == GTH_EV_CHAR_SKILLCASTING )
	{
		vec3_t position;
		int x, y;
		g_charManager->GetCharPanelScreenPos( g_pApp->m_myCharacter, position );
		if( g_cgv.myCharacterInfo->skillMaxCastingtime <= 0.0f ) 
		{
			m_castingBar->Enable( false );
		}
		else
		{
			m_castingBar->UpdateBarState( g_cgv.myCharacterInfo->skillCastingtime, g_cgv.myCharacterInfo->skillMaxCastingtime );
			
			x = (int)position[0];
			y = (int)position[1];
			x = x - m_castingBar->m_barSize / 2;
			y -= 50;
			m_castingBar->m_bar->m_info[0].clientX = x;
			m_castingBar->m_bar->m_info[0].clientY = y;

			m_castingBar->UpdateBuffer();
			m_castingBar->Enable();
		}
	}
	else
	{
		m_castingBar->Enable( false );
	}

	
	
	int itemIdx, nDestroy = 0;
	item_t *item;
	CIFControl *selectDestroy;

	m_destroyUpper->Enable( false );
	m_destroyLower->Enable( false );
	m_destroyBoots->Enable( false );
	m_destroyGloves->Enable( false );
	m_destroyProtector->Enable( false );
	m_destroyWeapon->Enable( false );

	for( int i = 0; i < 6; i ++ )
	{
		if( i == 0 ) { selectDestroy = m_destroyWeapon;		itemIdx = EQUIP_WEAPON_1 + g_cgv.myCharacterInfo->curWeapon; }
		if( i == 1 ) { selectDestroy = m_destroyUpper;		itemIdx = EQUIP_UPPER; }
		if( i == 2 ) { selectDestroy = m_destroyLower;		itemIdx = EQUIP_LOWER; }
		if( i == 3 ) { selectDestroy = m_destroyBoots;		itemIdx = EQUIP_BOOTS; }
		if( i == 4 ) { selectDestroy = m_destroyGloves;		itemIdx = EQUIP_GLOVES; }
		if( i == 5 ) { selectDestroy = m_destroyProtector;	itemIdx = EQUIP_BODY_PROTECTOR; }

		itemIdx =  g_cgv.myCharacterInfo->equipment[itemIdx];

		if( itemIdx >= 0 ) item = &g_cgv.myCharacterInfo->item[itemIdx];
		else continue;

		if( item->durability <= 10 ) 
		{
			selectDestroy->Enable( true );
			selectDestroy->m_info[0].clientX = nDestroy * 32;
			selectDestroy->m_info[0].clientY = m_info[0].sizeY;
			nDestroy ++;
		}
		else
		{
			selectDestroy->Enable( false );
		}
	}

	if( !State() ) return( IF_CTL_NONE );

	return( IF_CTL_NONE );
}

void CIFNameWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_raBar->Render();
	m_saBar->Render();
	m_expBar->Render();
	m_genexpBar->Render();

	m_targetWin->Render();
	m_targetBar->Render();

	m_castingBar->Render();

	
	m_destroyUpper->Render();
	m_destroyLower->Render();
	m_destroyBoots->Render();
	m_destroyGloves->Render();
	m_destroyProtector->Render();
	m_destroyWeapon->Render();

	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < g_cgv.syncCharacter[0].numStaticEffects; i ++ )
		{
			int cx, cy, skillTableIdx;
			cx = m_info[0].clientX + m_info[0].sizeX;
			cy = 0;
			cx += i * 38;

			skillTableIdx = g_cgv.syncCharacter[0].staticEffectIndex[i];

			g_chatManager->DPrint( 400, 50 + i * 12, "%.2f", m_affectTime[i] );

			m_affectTime[i] -= g_timer.GetElapsedMilliTime();

			
			if( m_affectTime[i] < 10000.0f )
			{
				if( !m_affectTwinkle[i] || m_affectTwinkleResetTime[i] == 3000.0f )
				{
					m_affectTwinkleResetTime[i] = 1000.0f; 
					m_affectTwinkleTime[i] = 1000.0f; 
				}
				m_affectTwinkle[i] = true;
			}
			
			else if( m_affectTime[i] < 30000.0f )
			{
				if( !m_affectTwinkle[i] )
				{
					m_affectTwinkleResetTime[i] = 3000.0f; 
					m_affectTwinkleTime[i] = 3000.0f; 
				}
				m_affectTwinkle[i] = true;
			}
			else
			{
				m_affectTwinkle[i] = false;
			}
			
			if( m_affectTwinkle[i] )
			{
				m_affectTwinkleTime[i] -= g_timer.GetElapsedMilliTime();
				if( m_affectTwinkleTime[i] < 0.0f ) m_affectTwinkleTime[i] = m_affectTwinkleResetTime[i];
				else if( m_affectTwinkleTime[i] < ( m_affectTwinkleResetTime[i] / 2 ) ) continue;
			}
			
			
			
			CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
			if(pSkillTable!=NULL) 
			{
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1 );
			}
		}
		
	}
	g_ifMng->m_iconCtl->EndFrame();

	for( int i = 0; i < g_cgv.syncCharacter[0].numStaticEffects; i ++ )
	{
		int cx, cy;
		cx = m_info[0].clientX + m_info[0].sizeX;
		cy = 0;
		cx += i * 38;

		if( m_affectTwinkle[i] )
		{
			if( m_affectTwinkleTime[i] < ( m_affectTwinkleResetTime[i] / 2 ) ) continue;
		}
		
		
		
		if( m_affectTime[i] < 60000.0f )
		{
			g_ifMng->m_iPos[ m_skillIconOutlineCtl->m_info[0].ID ].clientX = cx;
			g_ifMng->m_iPos[ m_skillIconOutlineCtl->m_info[0].ID ].clientY = cy;
			m_skillIconOutlineCtl->UpdateControl();
			m_skillIconOutlineCtl->UpdateBuffer();

			m_skillIconOutlineCtl->Render();
		}
	}

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_NAME_WIN_LEVEL]->Render();
		m_textCtl[IT_NAME_WIN_LEVEL]->GetXPos();
		m_textCtl[IT_NAME_WIN_LEVEL_U]->Render();
		m_textCtl[IT_NAME_WIN_NAME_U]->Render();

		


		
		m_textCtl[IT_NAME_WIN_RA_U]->Render( IF_TEXT_OUTPUT_NORMAL );
		m_textCtl[IT_NAME_WIN_SA_U]->Render( IF_TEXT_OUTPUT_NORMAL );
		

		m_textCtl[IT_NAME_WIN_GENLEVEL]->Render();
		m_textCtl[IT_NAME_WIN_GENLEVEL_U]->Render();
		m_textCtl[IT_NAME_WIN_EXP_TEXT_U]->Render( IF_TEXT_OUTPUT_CENTER );
		m_textCtl[IT_NAME_WIN_GENEXP_TEXT_U]->Render( IF_TEXT_OUTPUT_CENTER );		

		if( m_targetWin->State() ) m_textCtl[IT_TARGET_WIN_NAME_U]->Render( IF_TEXT_OUTPUT_CENTER );
	}
	g_chatManager->EndFrame();
}


int CIFNameWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFNameWin::WindowProcess()
{
	if( Selection() == IF_LBUP )
	{
		





		g_GameTarget.GTH_ChangeTarget( g_pApp->m_myCharacter );
	}
	return IF_CTL_NONE;
}

void CIFNameWin::AffectProcess( int affectNumber, int* affectIdx )
{
	if( affectIdx == NULL )
	{
		m_numAffectSkill = 0;
		return;
	}
	m_numAffectSkill = affectNumber;
	for( int i = 0; i < m_numAffectSkill; i ++ )
	{
		m_affectSkillIndex[i] = affectIdx[i];
	}
}








CIFHotWin::CIFHotWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_HOT_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );
	m_changeBtn = new CIFButtonCtl( this );

	Enable( true );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_HOT_WINDOW_TYPE );

	m_curPage = 0;
	m_clickedIdx = -1;
}

CIFHotWin::~CIFHotWin()
{

}

void CIFHotWin::PrepareControl()
{
	g_ifMng->m_iPos[IF_HOT_WIN].clientY = g_ifMng->m_iPos[IF_C_WIN].clientY - g_ifMng->m_iPos[IF_HOT_WIN].sizeY;
	CIFControl::PrepareControl( IF_HOT_WIN );

	m_textCtl[IT_HOT_WIN_PAGE_U]->PrepareControl( IF_HOT_WIN_PAGE_TEXT_U );
	m_textCtl[IT_HOT_WIN_ICON_U]->PrepareControl( IF_HOT_WIN_ICON_TEXT_U );
	
	m_prevBtn->PrepareControl( IF_HOT_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_HOT_WIN_NEXT_BTN_0 );
	m_changeBtn->PrepareControl( IF_HOT_WIN_CHANGE_BTN_0 );
}

int CIFHotWin::WindowFrame()
{
	char szTemp[256];

	sprintf( szTemp, "%d/4", m_curPage + 1 );
	m_textCtl[IT_HOT_WIN_PAGE_U]->SetText( szTemp );

	return IF_CTL_NONE;
}

void CIFHotWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_prevBtn->Render();
	m_nextBtn->Render();
	m_changeBtn->Render();

	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		int cx, cy, i, hotIdx, idx, data;
		
		
		float coolTime[MAX_NUMBER_OF_SKILL];
		for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
		{
			coolTime[i] = g_cgv.myCharacterInfo->skillCurCooltime[i] - g_timer.GetAbsoluteTime() * 1000.0f;
		}

		for( i = 0; i < MAX_COUNT_IF_HOT_KEY; i ++ )
		{
			idx = m_curPage * MAX_COUNT_IF_HOT_KEY + i;
			hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];

			if( hotIdx < 0 ) continue;
			cx = m_textCtl[IT_HOT_WIN_ICON_U]->GetXPos() + i * 33;
			cy = m_textCtl[IT_HOT_WIN_ICON_U]->GetYPos();

			switch( g_cgv.myCharacterInfo->hotkeyType[idx] )
			{
			case IF_HOTTYPE_SKILL :
				{
					

					data = g_cgv.myCharacterInfo->skill[hotIdx].tableIdx;
					int nIconID =0;

					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(data);
					if(pSkillTable!=NULL)
					{
						nIconID = pSkillTable->m_nIconID;
					}
					
					if( coolTime[hotIdx] > 0.0f )
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, nIconID + 1, 
							coolTime[hotIdx], g_cgv.myCharacterInfo->skillMaxCooltime[hotIdx] );
					}
					else
					{
						if( m_clickedIdx == i )
						{
							g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL,nIconID + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
						}
						else
						{
							g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL,nIconID + 1 );
						}
					}
				}
				break;
			case IF_HOTTYPE_ACTION :
				if( hotIdx == ACTION_EPACK_RA ) 
				{
					if( g_cgv.epackRADelayTime > 0.0f )
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1, 
							g_cgv.epackRADelayTime, g_cgv.epackRADelayMaxTime );
					}
					else
					{
						if( m_clickedIdx == i )
						{
							g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
						}
						else
						{
							g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1 );
						}
					}
				}
				else if( hotIdx == ACTION_EPACK_SA ) 
				{
					if( g_cgv.epackSADelayTime > 0.0f )
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1, 
							g_cgv.epackSADelayTime, g_cgv.epackSADelayMaxTime );
					}
					else
					{
						if( m_clickedIdx == i )
						{
							g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
						}
						else
						{
							g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1 );
						}
					}
				}
				else
				{
					if( m_clickedIdx == i )
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
					}
					else
					{
						g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[hotIdx].iconIdx + 1 );
					}
				}
				break;
			case IF_HOTTYPE_ITEM :
				break;
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_NORMAL );
	g_chatManager->BeginFrame();
	{
		int cx, cy, idx, hotIdx;
		char szTemp[16];
		
		m_textCtl[IT_HOT_WIN_PAGE_U]->Render( IF_TEXT_OUTPUT_CENTER );

		g_chatManager->SetOutputType( CHAT_OUTPUT_BORDER );
		g_chatManager->SetReverseSize( -1 );

		for( int i = 0; i < 8; i ++ )
		{
			cx = m_textCtl[IT_HOT_WIN_ICON_U]->GetXPos() + i * 33;
			cy = m_textCtl[IT_HOT_WIN_ICON_U]->GetYPos();

			idx = m_curPage * MAX_COUNT_IF_HOT_KEY + i;
			hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];

			sprintf( szTemp, "F%d", i + 1 );
			g_chatManager->TextOut( cx, cy, szTemp );

			if( hotIdx < 0 ) continue;
			
			if( g_cgv.myCharacterInfo->hotkeyType[idx] == IF_HOTTYPE_ACTION )
			{
				if( hotIdx == ACTION_EPACK_RA ) 
				{
					sprintf( szTemp, "%d", g_cgv.myCharacterInfo->curChargeRA );
					g_chatManager->Output( cx, cy + 19, szTemp, true );
				}
				else if( hotIdx == ACTION_EPACK_SA ) 
				{
					sprintf( szTemp, "%d", g_cgv.myCharacterInfo->curChargeSA );
					g_chatManager->Output( cx, cy + 19, szTemp, true );
				}
			}
		}
	}
	g_chatManager->EndFrame();
}
int CIFHotWin::WindowSelection()
{
	int ret;

	
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFHotWin::WindowProcess()
{
	m_clickedIdx = -1;

	

	
	
	if( g_input->KeyEvent( DIK_F1 ) ) 
	{
		
		m_clickedIdx = 0;
	}
	else if( g_input->KeyEvent( DIK_F2 ) ) 
	{
		
		m_clickedIdx = 1;
	}
	else if( g_input->KeyEvent( DIK_F3 ) ) 
	{
		
		m_clickedIdx = 2;
	}
	else if( g_input->KeyEvent( DIK_F4 ) ) 
	{
		
		m_clickedIdx = 3;
	}
	else if( g_input->KeyEvent( DIK_F5 ) ) 
	{
		
		m_clickedIdx = 4;
	}
	else if( g_input->KeyEvent( DIK_F6 ) ) 
	{
		
		m_clickedIdx = 5;
	}
	else if( g_input->KeyEvent( DIK_F7 ) ) 
	{
		
		m_clickedIdx = 6;
	}
	else if( g_input->KeyEvent( DIK_F8 ) ) 
		
	{
		m_clickedIdx = 7;	
	}

	
	int idx, hotIdx, data, pos;
	int mx, my;
	
	if( m_prevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_curPage --;
		if( m_curPage < 0 ) m_curPage = 0;
	}
	if( m_nextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		m_curPage ++;
		if( m_curPage > 3 ) m_curPage = 3;
	}
	
	
	
	if( m_changeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( !g_ifMng->m_helpWin->ToggleState() )
		{
			g_ifMng->SetFocus( g_ifMng->m_helpWin );
		}
	}

	mx = g_input->GetPosX() - m_textCtl[IT_HOT_WIN_ICON_U]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_HOT_WIN_ICON_U]->GetYPos();

	if( mx <= 0 || my <= 0 ) 
	{
		if( g_input->GetLBUp() ) g_input->ChangeCursor();
		return( IF_CTL_NONE );
	}
	if( mx >= 33 * MAX_COUNT_IF_HOT_KEY ||  my >= 32 ) 
	{
		if( g_input->GetLBUp() ) g_input->ChangeCursor();
		return( IF_CTL_NONE );
	}
	pos = mx / 33;

	if( pos < 0 || pos >= 8 ) 
	{
		if( g_input->GetLBUp() ) g_input->ChangeCursor();
		return( IF_CTL_NONE );
	}

	idx = m_curPage * MAX_COUNT_IF_HOT_KEY + pos;

	
	if( g_input->GetLBUp() )
	{
		
		if( g_input->GetCursorState( -1, MOUSE_CURSOR_TYPE ) )
		{
			hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];
			if( hotIdx < 0 ) return( IF_CTL_NONE );

			switch( g_cgv.myCharacterInfo->hotkeyType[idx] )
			{
			case IF_HOTTYPE_SKILL :
				{
					data = g_cgv.myCharacterInfo->skill[hotIdx].tableIdx;
					
					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(data);
					if(pSkillTable!=NULL)
					{
						g_input->ChangeCursor( pSkillTable->m_nIconID + 1, SKILL_CURSOR_TYPE, hotIdx );
					}
					
				}
				break;
			case IF_HOTTYPE_ACTION :
				g_input->ChangeCursor( g_actionTable[hotIdx].iconIdx + 1, ACTION_CURSOR_TYPE, g_actionTable[hotIdx].id );
				break;
				
			case IF_HOTTYPE_ITEM :
				break;
			}
			g_cgv.myCharacterInfo->hotkeyIdx[idx] = -1;
			g_cgv.myCharacterInfo->hotkeyType[idx] = 0;
			GTH_SendMessage_HotKey( idx );
		}
		
		else if( g_input->GetCursorState( -1, SKILL_CURSOR_TYPE ) )
		{
			hotIdx = g_input->GetCursorData();
			g_cgv.myCharacterInfo->hotkeyIdx[idx] = hotIdx;
			g_cgv.myCharacterInfo->hotkeyType[idx] = IF_HOTTYPE_SKILL;
			GTH_SendMessage_HotKey( idx );
			g_input->ChangeCursor();
		}
		
		else if( g_input->GetCursorState( -1, ACTION_CURSOR_TYPE ) )
		{
			hotIdx = g_input->GetCursorData();
			g_cgv.myCharacterInfo->hotkeyIdx[idx] = hotIdx;
			g_cgv.myCharacterInfo->hotkeyType[idx] = IF_HOTTYPE_ACTION;
			GTH_SendMessage_HotKey( idx );
			g_input->ChangeCursor();
		}
	}
	else if( g_input->GetRBUp() )
	{
		HotKeyProcess( pos );
	}
	else if( g_input->GetLBNone() )
	{
		hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];
		if( hotIdx < 0 ) return( IF_CTL_NONE );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeHotRollover( idx );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}
	else if( g_input->GetRBKeep() )
	{
		hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];
		if( hotIdx < 0 ) return( IF_CTL_NONE );
		m_clickedIdx = idx;
	}


	return IF_CTL_NONE;
}

void CIFHotWin::HotKeyProcess( int pos )
{
	int hotIdx;
	int idx;
	
	if(g_ifMng->m_autoQuestWin->m_iautoflag==AUTOQUESTSCROLL_START) return;
	idx = pos + m_curPage * MAX_COUNT_IF_HOT_KEY;

	
	hotIdx = g_cgv.myCharacterInfo->hotkeyIdx[idx];
	if( hotIdx < 0 ) return;
	
	switch( g_cgv.myCharacterInfo->hotkeyType[idx] )
	{
	
	case IF_HOTTYPE_SKILL :
		{	
			g_SkillFile.DebugOutString("Ω∫≈≥ «÷≈∞");

			
			
			Fx_CHARACTER_t *myChar = g_pApp->m_myCharacter;
			if( NULL != myChar && (myChar->ani_curSetType == ANIMTYPE_BYSKILL))
			{
				g_SkillFile.DebugOutString("Ω∫≈≥ «÷≈∞ Ω«∆–");
				break;
			}

			
			
			g_SkillFile.DebugOutString("IF_HOTTYPE_SKILL");

			if( 0 == g_SkillLogic.GTH_SkillProcess(globalVariable_t::SELECT_SKILL_HOTKEY, g_cgv.myCharacterInfo->hotkeyIdx[idx] ) )
			{
				g_SkillFile.DebugOutString("Ω∫≈≥ «÷≈∞ Ω«∆–");
			}
		}
		break;
	case IF_HOTTYPE_ACTION :
		g_SkillApp.ActionProcess( g_actionTable[hotIdx].id );
		break;
	case IF_HOTTYPE_ITEM :
		break;
	}
}












CIFSkillWin::CIFSkillWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_SKILL_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );

	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_tabBar = new CIFControl( this );
	m_tabBasicSkillLine = new CIFControl( this );
	m_tabGenSkillLine = new CIFControl( this );
	m_invenBar1 = new CIFControl( this );
	m_invenBar2 = new CIFControl( this );
	
	m_passivePrevBtn = new CIFButtonCtl( this );
	m_passiveNextBtn = new CIFButtonCtl( this );
	m_activePrevBtn = new CIFButtonCtl( this );
	m_activeNextBtn = new CIFButtonCtl( this );

	m_closeBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_SKILL_WINDOW_TYPE );

	m_curType = 0;
	m_curSelect = -1;
}

CIFSkillWin::~CIFSkillWin()
{

}

void CIFSkillWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_SKILL_WIN );

	m_textCtl[IT_SKILL_WIN_TITLE]->PrepareControl( IF_SKILL_WIN_TITLE_TEXT );
	m_textCtl[IT_SKILL_WIN_TAB_BASIC_SKILL]->PrepareControl( IF_SKILL_WIN_TAB_BASIC_SKILL_TEXT );
	m_textCtl[IT_SKILL_WIN_TAB_GEN_SKILL]->PrepareControl( IF_SKILL_WIN_TAB_GEN_SKILL_TEXT );
	m_textCtl[IT_SKILL_WIN_TAB_AWAKEN_SKILL]->PrepareControl( IF_SKILL_WIN_TAB_AWAKEN_SKILL_TEXT );
	m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->PrepareControl( IF_SKILL_WIN_INVEN_POS_0 );
	m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->PrepareControl( IF_SKILL_WIN_INVEN_POS_1 );

	m_titleBar->PrepareControl( IF_SKILL_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_SKILL_WIN_TITLE_BOTTOM );
	m_tabBar->PrepareControl( IF_SKILL_WIN_TAB_BAR );
	m_tabBasicSkillLine->PrepareControl( IF_SKILL_WIN_TAB_BASIC_SKILL_LINE );
	m_tabGenSkillLine->PrepareControl( IF_SKILL_WIN_TAB_GEN_SKILL_LINE );
	m_invenBar1->PrepareControl( IF_SKILL_WIN_INVEN_1 );
	m_invenBar2->PrepareControl( IF_SKILL_WIN_INVEN_2 );
	
	m_passivePrevBtn->PrepareControl( IF_SKILL_WIN_P_PREV_BTN_0 );
	m_passiveNextBtn->PrepareControl( IF_SKILL_WIN_P_NEXT_BTN_0 );
	m_activePrevBtn->PrepareControl( IF_SKILL_WIN_A_PREV_BTN_0 );
	m_activeNextBtn->PrepareControl( IF_SKILL_WIN_A_NEXT_BTN_0 );

	m_closeBtn->PrepareControl( IF_SKILL_WIN_CLOSE_BTN_0 );
}

int CIFSkillWin::WindowFrame()
{
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 531 ) );
	m_textCtl[IT_SKILL_WIN_TITLE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 532 ) );
	m_textCtl[IT_SKILL_WIN_TAB_BASIC_SKILL]->SetText( szTemp );
	
	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 533 ) );
	m_textCtl[IT_SKILL_WIN_TAB_GEN_SKILL]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 646 ) );
	m_textCtl[IT_SKILL_WIN_TAB_AWAKEN_SKILL]->SetText( szTemp );

	m_textCtl[IT_SKILL_WIN_TAB_BASIC_SKILL]->SetColor( 0xff808080 );
	m_textCtl[IT_SKILL_WIN_TAB_GEN_SKILL]->SetColor( 0xff808080 );
	m_textCtl[IT_SKILL_WIN_TAB_AWAKEN_SKILL]->SetColor( 0xff808080 );

	if( m_curType == 0 )
	{
		m_textCtl[IT_SKILL_WIN_TAB_BASIC_SKILL]->SetColor( 0xffffffff );
	}
	else if( m_curType == 1 )
	{
		m_textCtl[IT_SKILL_WIN_TAB_GEN_SKILL]->SetColor( 0xffffffff );
	}
	else if( m_curType == 2 )
	{
		m_textCtl[IT_SKILL_WIN_TAB_AWAKEN_SKILL]->SetColor( 0xffffffff );
	}

	return IF_CTL_NONE;
}

void CIFSkillWin::Render()
{
	if( !State() ) return;
	
	g_ifMng->SetRenderState();
	CIFControl::Render();

	m_titleBar->Render();
	m_titleBottom->Render();
	m_tabBar->Render();
	m_tabBasicSkillLine->Render();
	m_tabGenSkillLine->Render();
	m_invenBar1->Render();
	m_invenBar2->Render();
	
	m_passivePrevBtn->Render();
	m_passiveNextBtn->Render();

	m_activePrevBtn->Render();
	m_activeNextBtn->Render();

	m_closeBtn->Render();

	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		int cx, cy, i;
		switch( m_curType )
		{
			
		case 0 :
			{
				for( i = 0; i < NUM_ACTION_TYPE; i ++ )
				{
					
					if( i < 18 ) 
					{
						cx = m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->m_info.clientX + ( i % 6 ) * 34;
						cy = m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->m_info.clientY + ( i / 6 ) * 33;
					}
					else 
					{
						cx = m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->m_info.clientX + ( ( i - 18 ) % 6 ) * 34;
						cy = m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->m_info.clientY + ( ( i - 18 ) / 6 ) * 33;
					}
					g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, g_actionTable[i].iconIdx + 1 );
				}
			}
			break;
			
		case 1 :
			{
				
				int n = 0, isLearn, skillNumber, skillIdx;

				
				for( i = 0; i < m_numSortSkillIdxTable; i ++ )
				{
					skillIdx = m_sortSkillIdxTable[i];

					




					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillIdx);
					if(pSkillTable!=NULL)
					{
						if( pSkillTable->m_nId < 0 ) continue;
						if( !pSkillTable->m_nUsageType ) continue;
						if( pSkillTable->m_nJobType != g_cgv.myCharacterInfo->pcClass ) continue;
					}

					skillNumber = 0;
					for( int j = 0; j < MAX_NUMBER_OF_SKILL; j ++ )
					{
						if( g_cgv.myCharacterInfo->skillNumber == skillNumber ) break;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx < 0 ) continue;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx == skillIdx ) break;
						skillNumber ++;
					}
					m_genPassiveSkillIdxList[n].skillTableIdx = skillIdx;
					if( j == g_cgv.myCharacterInfo->skillNumber ) 
					{
						isLearn = false;
						m_genPassiveSkillIdxList[n].learnIdx = -1;
					}
					else 
					{
						isLearn = true;
						m_genPassiveSkillIdxList[n].learnIdx = j;
					}

					cx = m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->m_info.clientX + ( n % 6 ) * 34;
					cy = m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->m_info.clientY + ( n / 6 ) * 33;
					
				
					if(pSkillTable!=NULL)
					{
						if( isLearn ) g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1 );
						else g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
					}

					n ++;
				}
				m_numGenPassiveSkillIdxList = n;

				
				n = 0;
				for( i = 0; i < m_numSortSkillIdxTable; i ++ )
				{
					skillIdx = m_sortSkillIdxTable[i];

					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillIdx);
					if(pSkillTable!=NULL)
					{
						if( pSkillTable->m_nId < 0 ) continue;
						if( pSkillTable->m_nUsageType) continue;
						if( pSkillTable->m_nJobType != g_cgv.myCharacterInfo->pcClass ) continue;
					}

					skillNumber = 0;
					for( int j = 0; j < MAX_NUMBER_OF_SKILL; j ++ )
					{
						if( g_cgv.myCharacterInfo->skillNumber == skillNumber ) break;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx < 0 ) continue;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx == skillIdx ) break;
						skillNumber ++;
					}
					m_genActiveSkillIdxList[n].skillTableIdx = skillIdx;
					if( j == g_cgv.myCharacterInfo->skillNumber ) 
					{
						isLearn = false;
						m_genActiveSkillIdxList[n].learnIdx = -1;
					}
					else 
					{
						isLearn = true;
						m_genActiveSkillIdxList[n].learnIdx = j;
					}

					cx = m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->m_info.clientX + ( n % 6 ) * 34;
					cy = m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->m_info.clientY + ( n / 6 ) * 33;
					
				
					if(pSkillTable!=NULL)
					{
						if( isLearn ) g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1 );
						else g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
					}
					n ++;
				}
				m_numGenActiveSkillIdxList = n;
			}
			break;
			
		case 2 :
			{
#ifndef _GTH_CHEAT_VERSION
				if( g_cgv.myCharacterInfo->pcJob <= J_EARTH ) break;
#endif

				
				int n = 0, isLearn, skillNumber, skillIdx;

				
				for( i = 0; i < m_numSortSkillIdxTable; i ++ )
				{
					skillIdx = m_sortSkillIdxTable[i];

					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillIdx);
					if(pSkillTable!=NULL)
					{
						if( pSkillTable->m_nId < 0 ) continue;
						if( !pSkillTable->m_nUsageType ) continue;
						if( pSkillTable->m_nJobType != g_cgv.myCharacterInfo->pcJob ) continue;
					}

					skillNumber = 0;
					for( int j = 0; j < MAX_NUMBER_OF_SKILL; j ++ )
					{
						if( g_cgv.myCharacterInfo->skillNumber == skillNumber ) break;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx < 0 ) continue;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx == skillIdx ) break;
						skillNumber ++;
					}
					m_genPassiveSkillIdxList[n].skillTableIdx = skillIdx;
					if( j == g_cgv.myCharacterInfo->skillNumber ) 
					{
						isLearn = false;
						m_genPassiveSkillIdxList[n].learnIdx = -1;
					}
					else 
					{
						isLearn = true;
						m_genPassiveSkillIdxList[n].learnIdx = j;
					}

					cx = m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->m_info.clientX + ( n % 6 ) * 34;
					cy = m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->m_info.clientY + ( n / 6 ) * 33;
					
					
					if(pSkillTable!=NULL)
					{
						if( isLearn ) g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1 );
						else g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
					}

					n ++;
				}
				m_numGenPassiveSkillIdxList = n;

				
				n = 0;
				for( i = 0; i < m_numSortSkillIdxTable; i ++ )
				{
					skillIdx = m_sortSkillIdxTable[i];

					CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillIdx);
					if(pSkillTable!=NULL)
					{
						if( pSkillTable->m_nId < 0 ) continue;
						if( pSkillTable->m_nUsageType ) continue;
						if( pSkillTable->m_nJobType != g_cgv.myCharacterInfo->pcJob ) continue;
					}





					skillNumber = 0;
					for( int j = 0; j < MAX_NUMBER_OF_SKILL; j ++ )
					{
						if( g_cgv.myCharacterInfo->skillNumber == skillNumber ) break;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx < 0 ) continue;
						if( g_cgv.myCharacterInfo->skill[j].tableIdx == skillIdx ) break;
						skillNumber ++;
					}
					m_genActiveSkillIdxList[n].skillTableIdx = skillIdx;
					if( j == g_cgv.myCharacterInfo->skillNumber ) 
					{
						isLearn = false;
						m_genActiveSkillIdxList[n].learnIdx = -1;
					}
					else 
					{
						isLearn = true;
						m_genActiveSkillIdxList[n].learnIdx = j;
					}

					cx = m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->m_info.clientX + ( n % 6 ) * 34;
					cy = m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->m_info.clientY + ( n / 6 ) * 33;
					
				
					if(pSkillTable != NULL)
					{
						if( isLearn ) g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1 );
						else g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL, pSkillTable->m_nIconID + 1, 1.0f, 1.0f, DISALBE_ICON_DARK_COLOR );
					}
					
					

					n ++;
				}
				m_numGenActiveSkillIdxList = n;
			}
			break;
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_SKILL_WIN_TITLE]->Render();
		m_textCtl[IT_SKILL_WIN_TAB_BASIC_SKILL]->Render();
		m_textCtl[IT_SKILL_WIN_TAB_GEN_SKILL]->Render();
		m_textCtl[IT_SKILL_WIN_TAB_AWAKEN_SKILL]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFSkillWin::WindowSelection()
{
	int ret;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFSkillWin::WindowProcess()
{
	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		Enable( false );
		return IF_CTL_SELECT;
	}
	if( m_textCtl[IT_SKILL_WIN_TAB_BASIC_SKILL]->Selection() == IF_LBUP )
	{
		m_curType = 0;
		m_curPage = 0;
	}
	if( m_textCtl[IT_SKILL_WIN_TAB_GEN_SKILL]->Selection() == IF_LBUP )
	{
		m_curType = 1;
		m_curPage = 0;
	}
	if( m_textCtl[IT_SKILL_WIN_TAB_AWAKEN_SKILL]->Selection() == IF_LBUP )
	{
		m_curType = 2;
		m_curPage = 0;
	}
	

	
	int x, y, pos = -1, type = -1;
	int mx, my, temp;
	
	mx = g_input->GetPosX() - m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_SKILL_WIN_INVEN_POS_0]->GetYPos();

	if( mx >= 0 && my >= 0 && mx <= 34 * 6 &&  my <= 33 * 3 )
	{
		type = 0;
		temp = mx / 34;
		x = temp % 6;
		temp = my / 33;
		y = temp % 3;
		
		pos = y * 6 + x;
		
		if( pos < 0 || pos >= 18 )
		{
			pos = -1;
		}
	}

	mx = g_input->GetPosX() - m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_SKILL_WIN_INVEN_POS_1]->GetYPos();

	if( mx >= 0 && my >= 0 && mx <= 34 * 6 &&  my <= 33 * 4 )
	{
		type = 1;
		temp = mx / 34;
		x = temp % 6;
		temp = my / 33;
		y = temp % 4;
		
		pos = y * 6 + x;
		
		if( pos < 0 || pos >= 24 )
		{
			pos = -1;
		}
	}

	if( pos < 0 ) return IF_CTL_NONE;

	switch( m_curType )
	{
	
	case 0 :
		{

			
			
			pos = type * 18 + pos;
			if( pos > NUM_ACTION_TYPE ) return( IF_CTL_NONE );
			if( g_actionTable[pos].id < 0 ) return( IF_CTL_NONE );

			
			if( g_input->GetLBUp() ) 
			{
				
				if( g_cgv.myCharacterInfo->mouseInventory >= 0 ) return( IF_CTL_NONE );

				g_input->ChangeCursor( g_actionTable[pos].iconIdx + 1, ACTION_CURSOR_TYPE, g_actionTable[pos].id );
			}
			
			else if( g_input->GetRBUp() )
			{
				g_SkillApp.ActionProcess( g_actionTable[pos].id );
			}
			
			else if( g_input->GetLBNone() )
			{
				
				g_ifMng->InitRolloverText();
				g_ifMng->MakeActionRollover( g_actionTable[pos].id );
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
		break;
	
	case 1 :
		{
			int idx;
			int skillTableIdx, learnIdx;

			if( pos < 0 ) return( IF_CTL_NONE );

			if( !type ) 
			{
				idx = pos + m_curPage * 18;
				if( m_numGenPassiveSkillIdxList <= idx ) return( IF_CTL_NONE );
				skillTableIdx = m_genPassiveSkillIdxList[idx].skillTableIdx;
				learnIdx = m_genPassiveSkillIdxList[idx].learnIdx;
			}
			else 
			{
				idx = pos + m_curPage * 24;
				if( m_numGenActiveSkillIdxList <= idx ) return( IF_CTL_NONE );
				skillTableIdx = m_genActiveSkillIdxList[idx].skillTableIdx;
				learnIdx = m_genActiveSkillIdxList[idx].learnIdx;
			}


			
			
			if( g_input->GetLBUp() ) 
			{
				
				if( learnIdx < 0 ) return( IF_CTL_NONE );
				if( g_cgv.myCharacterInfo->mouseInventory >= 0 ) return( IF_CTL_NONE );

				
				CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
				if(pSkillTable!=NULL) 
				{
					if( pSkillTable->m_nUsageType ) return( IF_CTL_NONE );
					g_input->ChangeCursor( pSkillTable->m_nIconID + 1, SKILL_CURSOR_TYPE, learnIdx );
				}
				
				
			}
			
			else if( g_input->GetLBNone() )
			{
				
				g_ifMng->InitRolloverText();
				if( learnIdx >= 0 ) g_ifMng->MakeSkillRollover( learnIdx, 0 );
				else g_ifMng->MakeSkillRollover( skillTableIdx, 1 );

				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
	
	case 2 :
		{

#ifndef _GTH_CHEAT_VERSION
				if( g_cgv.myCharacterInfo->pcJob <= J_EARTH ) break;
#endif

			int idx;
			int skillTableIdx, learnIdx;

			if( pos < 0 ) return( IF_CTL_NONE );

			if( !type ) 
			{
				idx = pos + m_curPage * 18;
				if( m_numGenPassiveSkillIdxList <= idx ) return( IF_CTL_NONE );
				skillTableIdx = m_genPassiveSkillIdxList[idx].skillTableIdx;
				learnIdx = m_genPassiveSkillIdxList[idx].learnIdx;
			}
			else 
			{
				idx = pos + m_curPage * 24;
				if( m_numGenActiveSkillIdxList <= idx ) return( IF_CTL_NONE );
				skillTableIdx = m_genActiveSkillIdxList[idx].skillTableIdx;
				learnIdx = m_genActiveSkillIdxList[idx].learnIdx;
			}


			
			
			if( g_input->GetLBUp() ) 
			{
				
				if( learnIdx < 0 ) return( IF_CTL_NONE );
				if( g_cgv.myCharacterInfo->mouseInventory >= 0 ) return( IF_CTL_NONE );

				CSkilTable *pSkillTable = g_SkillTableDB.FindDB(skillTableIdx);
				if(pSkillTable!=NULL)
				{
					if( pSkillTable->m_nUsageType) return( IF_CTL_NONE );
					g_input->ChangeCursor( pSkillTable->m_nIconID + 1, SKILL_CURSOR_TYPE, learnIdx );
				}
			}
			
			else if( g_input->GetLBNone() )
			{
				
				g_ifMng->InitRolloverText();
				if( learnIdx >= 0 ) g_ifMng->MakeSkillRollover( learnIdx, 0 );
				else g_ifMng->MakeSkillRollover( skillTableIdx, 1 );

				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
			}
		}
		break;
	}

	return IF_CTL_NONE;
}

void CIFSkillWin::MakeSortSkillIdxTable()
{
	int i, j,  temp;
	int n = 0;

	
	for( i = 0; i < MAX_SKILL_NUMBER; i ++ )
	{
		CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(i);
		if(pSkillTable==NULL) continue;

		
		if( pSkillTable->m_nId < 0 ) continue;

		m_sortSkillIdxTable[n] = i;
		n ++;
	}

	m_numSortSkillIdxTable = n;

	for( i = 0; i < n; i ++ )
	{
		
		CSkilTable *pSkillTable0				= g_SkillTableDB.FindDB(i);
		if(pSkillTable0==NULL) continue;

		for( j = 0; j < n ; j ++ )
		{
			
			CSkilTable *pSkillTable1				= g_SkillTableDB.FindDB(j);
			if(pSkillTable1==NULL) continue;

			if( pSkillTable0->GetSkillRequire()->m_nGenLevel < pSkillTable1->GetSkillRequire()->m_nGenLevel )
			
			{
				temp = m_sortSkillIdxTable[i];
				m_sortSkillIdxTable[i] = m_sortSkillIdxTable[j];
				m_sortSkillIdxTable[j] = temp;
			}
		}
	}
}












CIFLearnWin::CIFLearnWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i = 0; i < NUM_IT_LEARN_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	


	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_listBar = new CIFControl( this );
	m_invenBar1 = new CIFControl( this );
	m_invenBar2 = new CIFControl( this );
	m_nameBar = new CIFControl( this );
	m_infoLine = new CIFControl( this );
	m_selectBox = new CIFControl( this );

	m_prevBtn = new CIFButtonCtl( this );
	m_nextBtn = new CIFButtonCtl( this );
	m_levelUpBtn = new CIFButtonCtl( this );
	m_closeBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_LEARN_WINDOW_TYPE );

	m_curPage = 0;
	m_selectedIdx =0;
    
	m_genType	  =0;
	m_numSkillInfo =0;
	memset(m_skillInfo,0,sizeof(learnSkillInfo_t)*MAX_NUMBER_OF_SKILL);
	
}

CIFLearnWin::~CIFLearnWin()
{

}

void CIFLearnWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_LEARN_WIN );

	m_textCtl[IT_LEARN_WIN_TITLE]->PrepareControl( IF_LEARN_WIN_TITLE_TEXT );
	m_textCtl[IT_LEARN_WIN_LIST]->PrepareControl( IF_LEARN_WIN_LIST_TEXT );
	m_textCtl[IT_LEARN_WIN_INVEN_POS]->PrepareControl( IF_LEARN_WIN_INVEN_POS );
	m_textCtl[IT_LEARN_WIN_NAME_U]->PrepareControl( IF_LEARN_WIN_NAME_TEXT_U );
	m_textCtl[IT_LEARN_WIN_INFO_U]->PrepareControl( IF_LEARN_WIN_INFO_TEXT_U );
	m_textCtl[IT_LEARN_WIN_LEVELUP]->PrepareControl( IF_LEARN_WIN_LEVELUP_TEXT );

	m_titleBar->PrepareControl( IF_LEARN_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_LEARN_WIN_TITLE_BOTTOM );
	m_listBar->PrepareControl( IF_LEARN_WIN_LIST_BAR );
	m_invenBar1->PrepareControl( IF_LEARN_WIN_INVEN_1 );
	m_invenBar2->PrepareControl( IF_LEARN_WIN_INVEN_2 );
	m_nameBar->PrepareControl( IF_LEARN_WIN_NAME_BAR );
	m_infoLine->PrepareControl( IF_LEARN_WIN_INFO_LINE );
	m_selectBox->PrepareControl( IF_LEARN_WIN_SELECT_BOX );

	m_prevBtn->PrepareControl( IF_LEARN_WIN_PREV_BTN_0 );
	m_nextBtn->PrepareControl( IF_LEARN_WIN_NEXT_BTN_0 );
	m_levelUpBtn->PrepareControl( IF_LEARN_WIN_LEVELUP_BTN_0 );
	m_closeBtn->PrepareControl( IF_LEARN_WIN_CLOSE_BTN_0 );
}

int CIFLearnWin::WindowFrame()
{
	
	char szTemp[256]={""};	
  	


	

	sprintf( szTemp, g_LPACK.GetMassage( 0, 499 ), g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, m_genType ) );
	m_textCtl[IT_LEARN_WIN_TITLE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 500 ) );
	m_textCtl[IT_LEARN_WIN_LIST]->SetText( szTemp );

	if( m_selectedIdx >= 0 )
	{
		
		int skillIdx=0, skillTableIdx=0;
		char szGenCap[128]={""},szReqGenCap[128]={""};
		

		skillIdx = m_skillInfo[m_selectedIdx].idx;
		skillTableIdx = g_cgv.myCharacterInfo->skill[skillIdx].tableIdx;


		
		
		sprintf( szTemp, "%s", g_SkillTableDB.GetSkillName(skillTableIdx));
		
		m_textCtl[IT_LEARN_WIN_NAME_U]->SetText( szTemp );
		
		
		
		strcpy( szGenCap, g_ifMng->GetNumberString( g_cgv.myCharacterInfo->genCapability[m_genType] ) );
		strcpy( szReqGenCap, g_ifMng->GetNumberString( m_skillInfo[m_selectedIdx].reqGenCapability ) );
		sprintf( szTemp, g_LPACK.GetMassage( 0, 501 ), szGenCap, szReqGenCap, m_skillInfo[m_selectedIdx].reqGenLevel );
		m_textCtl[IT_LEARN_WIN_INFO_U]->SetText( szTemp );

		m_textCtl[IT_LEARN_WIN_NAME_U]->Enable();
		m_textCtl[IT_LEARN_WIN_INFO_U]->Enable();
		
	}
	else
	{
		m_textCtl[IT_LEARN_WIN_NAME_U]->Enable( false );
		m_textCtl[IT_LEARN_WIN_INFO_U]->Enable( false );
	}

	

	sprintf( szTemp, g_LPACK.GetMassage( 0, 502 ) );
	m_textCtl[IT_LEARN_WIN_LEVELUP]->SetText( szTemp );

	return IF_CTL_NONE;
}

void CIFLearnWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_listBar->Render();
	m_invenBar1->Render();
	m_invenBar2->Render();
	m_nameBar->Render();
	m_infoLine->Render();

	m_prevBtn->Render();
	m_nextBtn->Render();
	m_levelUpBtn->Render();
	m_closeBtn->Render();
	
	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		
		for( int i = 0; i < m_numSkillInfo; i ++ )
		{
			int idx, skillIdx, cx, cy, skillTableIdx;
			idx = m_curPage * 36 + i;
			skillIdx = m_skillInfo[i].idx;

			skillTableIdx = g_cgv.myCharacterInfo->skill[skillIdx].tableIdx;
			cx = m_textCtl[IT_LEARN_WIN_INVEN_POS]->m_info.clientX + ( i % 6 ) * 34;
			cy = m_textCtl[IT_LEARN_WIN_INVEN_POS]->m_info.clientY + ( i / 6 ) * 33;

			int nIconID=0;
			CSkilTable *pSkillTable					= g_SkillTableDB.FindDB(skillTableIdx);
			if(pSkillTable!=NULL)
			{
				nIconID = pSkillTable->m_nIconID;
			}

			if( !m_skillInfo[i].enableLevelUp )
			{
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL,nIconID + 1, 1.0f, 1.0f, DISALBE_ICON_RED_COLOR );
			}
			else
			{
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_SKILL,nIconID + 1);
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();
	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_LEARN_WIN_TITLE]->Render();
		m_textCtl[IT_LEARN_WIN_LIST]->Render();
		m_textCtl[IT_LEARN_WIN_NAME_U]->Render();
		m_textCtl[IT_LEARN_WIN_INFO_U]->Render( IF_TEXT_OUTPUT_RECT );
		m_textCtl[IT_LEARN_WIN_LEVELUP]->Render();
	}
	g_chatManager->EndFrame();

	m_selectBox->Render();
}

int CIFLearnWin::WindowSelection()
{
	int ret;
	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFLearnWin::WindowProcess()
{
	char szTemp[256];
	int skillTableIdx;
	
	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_RequestQuitNpcChat();
		Enable( false );
	}
	if( m_levelUpBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_selectedIdx < 0 ) return( IF_CTL_NONE );
		if( !m_skillInfo[m_selectedIdx].enableLevelUp )
		{
			
			if( g_cgv.myCharacterInfo->genLevel < m_skillInfo[m_selectedIdx].reqGenLevel )
			{
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,413) );
			}
			else  
			{
				g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), g_LPACK.GetMassage(0,414) );
			}
		}
		else
		{
			char szReqGenCap[128];
			skillTableIdx = g_cgv.myCharacterInfo->skill[m_skillInfo[m_selectedIdx].idx].tableIdx;
			
			strcpy( szReqGenCap, g_ifMng->GetNumberString( m_skillInfo[m_selectedIdx].reqGenCapability ) );

			



	
			sprintf( szTemp, g_LPACK.GetMassage(0,417),g_SkillTableDB.GetSkillName(skillTableIdx), 
					m_skillInfo[m_selectedIdx].curLevel + 1, 
					g_LPACK.GetMassage( LPACK_TYPE_ETC, CLPack_Etc::LPACK_ETCTYPE_GEN_ATTRIBUTE, m_genType ), szReqGenCap );

			g_ifMng->SetMessage( g_LPACK.GetMassage(0,322), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_SKILL_LEVEL_UP );
		}
	}

	
	int x, y, pos;
	int mx, my, temp;
	
	mx = g_input->GetPosX() - m_textCtl[IT_LEARN_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_LEARN_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 8 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 8;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= 48 )
	{
		return( IF_CTL_NONE );
	}

	int idx;
	idx = m_curPage * 36 + pos;

	if( g_input->GetLBUp() )
	{
		if( idx >= m_numSkillInfo ) 
		{
			m_selectedIdx = -1;
		}
		else
		{
			int cx, cy;
			m_selectedIdx = idx;
			cx = g_ifMng->m_iPos[IF_LEARN_WIN_INVEN_POS].clientX;
			cy = g_ifMng->m_iPos[IF_LEARN_WIN_INVEN_POS].clientY;
			g_ifMng->m_iPos[IF_LEARN_WIN_SELECT_BOX].clientX = cx + ( m_selectedIdx % 6 * 34 );
			g_ifMng->m_iPos[IF_LEARN_WIN_SELECT_BOX].clientY = cy + ( m_selectedIdx / 6 * 33 );
		}
	}
	else if( g_input->GetLBNone() )
	{
		if( idx < m_numSkillInfo )
		{
			
			g_ifMng->InitRolloverText();
			g_ifMng->MakeSkillRollover( m_skillInfo[idx].idx, 0 );
			g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
		}
	}
	if( m_selectedIdx >= 0 ) m_selectBox->Enable( true );
	else m_selectBox->Enable( false );
	return IF_CTL_NONE;
}

void CIFLearnWin::MakeLearnSkillInfo()
{
	int i,  n = 0, skillTableIdx;
	int curLevel, gradeClass, limitLevel, valueGenFlag;
	m_numSkillInfo = 0;
	
	for( i = 0; i < MAX_NUMBER_OF_SKILL; i ++ )
	{
		if( g_cgv.myCharacterInfo->skill[i].tableIdx < 0 ) continue;
		
		
		if( !CheckAvailableGenCapability( g_cgv.myCharacterInfo, m_genType ) ) continue;

		skillTableIdx = g_cgv.myCharacterInfo->skill[i].tableIdx;

		




		m_skillInfo[m_numSkillInfo].idx = i;
		m_skillInfo[m_numSkillInfo].enableLevelUp = true;
		valueGenFlag = false;
		
		limitLevel = 1;
		gradeClass = 0;
		

	
	
			
		limitLevel = 1;
		gradeClass = 0;

		limitLevel = g_SkillDataGlobal.GetLimitlevel(skillTableIdx,m_genType);
		gradeClass = g_SkillDataGlobal.GetGradeClass(skillTableIdx,m_genType);
		
		curLevel = g_cgv.myCharacterInfo->skill[i].level.gen[m_genType];
		if( curLevel >= limitLevel )
		{
			continue;
		}
		m_skillInfo[m_numSkillInfo].curLevel = curLevel;
		
		m_skillInfo[m_numSkillInfo].reqGenCapability = 
			g_skillGradeClassTable[gradeClass].genCapability[curLevel];
		if( g_cgv.myCharacterInfo->genCapability[m_genType] < m_skillInfo[m_numSkillInfo].reqGenCapability || m_skillInfo[m_numSkillInfo].reqGenCapability <= 0 )
		{
			m_skillInfo[m_numSkillInfo].enableLevelUp = false;
		}
		
		m_skillInfo[m_numSkillInfo].reqGenLevel =
			g_skillGradeClassTable[gradeClass].limitGenLevel[curLevel];
		if( g_cgv.myCharacterInfo->genLevel < m_skillInfo[m_numSkillInfo].reqGenLevel )
		{
			m_skillInfo[m_numSkillInfo].enableLevelUp = false;
		}
		
		m_numSkillInfo ++;
	}
	m_selectedIdx = -1;
}

void CIFLearnWin::InitLearnWindow( int genType )
{
	m_genType = genType;
	MakeLearnSkillInfo();
}












CIFQuestWin::CIFQuestWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_QUEST_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_listBar = new CIFControl( this );
	m_selectBar = new CIFControl( this );
	m_itemBar = new CIFControl( this );
	m_listLine = new CIFControl( this );
	m_invenBar = new CIFControl( this );

	m_closeBtn = new CIFButtonCtl( this );
	m_stopBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_QUEST_WINDOW_TYPE );

	m_selectedIdx = -1;
}

CIFQuestWin::~CIFQuestWin()
{

}

void CIFQuestWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_QUEST_WIN );

	
	m_textCtl[IT_QUEST_WIN_TITLE]->PrepareControl( IF_QUEST_WIN_TITLE_TEXT );
	m_textCtl[IT_QUEST_WIN_LIST]->PrepareControl( IF_QUEST_WIN_LIST_TEXT );
	m_textCtl[IT_QUEST_WIN_LIST_0_U]->PrepareControl( IF_QUEST_WIN_LIST_0_TEXT_U );
	m_textCtl[IT_QUEST_WIN_LIST_1_U]->PrepareControl( IF_QUEST_WIN_LIST_1_TEXT_U );
	m_textCtl[IT_QUEST_WIN_LIST_2_U]->PrepareControl( IF_QUEST_WIN_LIST_2_TEXT_U );
	m_textCtl[IT_QUEST_WIN_LIST_3_U]->PrepareControl( IF_QUEST_WIN_LIST_3_TEXT_U );
	m_textCtl[IT_QUEST_WIN_LIST_4_U]->PrepareControl( IF_QUEST_WIN_LIST_4_TEXT_U );
	m_textCtl[IT_QUEST_WIN_ITEM]->PrepareControl( IF_QUEST_WIN_ITEM_TEXT );
	m_textCtl[IT_QUEST_WIN_INVEN_POS]->PrepareControl( IF_QUEST_WIN_INVEN_POS );
	
	m_titleBar->PrepareControl( IF_QUEST_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_QUEST_WIN_TITLE_BAR_BOTTOM );
	m_listBar->PrepareControl( IF_QUEST_WIN_LIST_BAR );
	m_selectBar->PrepareControl( IF_QUEST_WIN_SELECT_BAR );
	m_itemBar->PrepareControl( IF_QUEST_WIN_ITEM_BAR );
	m_listLine->PrepareControl( IF_QUEST_WIN_LIST_LINE );
	m_invenBar->PrepareControl( IF_QUEST_WIN_INVEN );

	m_closeBtn->PrepareControl( IF_QUEST_WIN_CLOSE_BTN_0 );
	m_stopBtn->PrepareControl( IF_QUEST_WIN_STOP_BTN_0 );

	m_selectBarSavePos = g_ifMng->m_iPos[IF_QUEST_WIN_SELECT_BAR].clientY;
}

int CIFQuestWin::WindowFrame()
{
	int i, n = 0;
	char szTemp[256];

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 528 ) );
	m_textCtl[IT_QUEST_WIN_TITLE]->SetText( szTemp );

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 529 ) );
	m_textCtl[IT_QUEST_WIN_LIST]->SetText( szTemp );

	m_textCtl[IT_QUEST_WIN_LIST_0_U]->Enable( false );
	m_textCtl[IT_QUEST_WIN_LIST_1_U]->Enable( false );
	m_textCtl[IT_QUEST_WIN_LIST_2_U]->Enable( false );
	m_textCtl[IT_QUEST_WIN_LIST_3_U]->Enable( false );
	m_textCtl[IT_QUEST_WIN_LIST_4_U]->Enable( false );

	m_selectBar->Enable( false );

	
	int bActiveSelectBar = 0;

	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		
		int step, questIdx;
		
		if( g_cgv.myCharacterInfo->questStatus[i].questIdx < 0 ) continue;

		questIdx = g_cgv.myCharacterInfo->questStatus[i].questIdx;
		step = g_cgv.myCharacterInfo->questVar[QUEST_SCROLL_INDEX].var.step;

		m_textCtl[IT_QUEST_WIN_LIST_0_U + n]->Enable( true );

		
		if ( questIdx == QUEST_SCROLL_INDEX )
		{	
			m_textCtl[IT_QUEST_WIN_LIST_0_U + n]->SetText( g_questscrollTable[g_cgv.myCharacterInfo->questStatus[i].dField[0]].name );
		}
		
		else if ( questIdx == AUTOQUESTSCROLL_ITEM_INDEX )
		{
			m_textCtl[IT_QUEST_WIN_LIST_0_U + n]->SetText( g_autoquestscrollTable[g_cgv.myCharacterInfo->questStatus[i].dField[0]].name );
		}
		else
			m_textCtl[IT_QUEST_WIN_LIST_0_U + n]->SetText( g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTNAME, questIdx ) );

		if( n == m_selectedIdx && step >= 0 )
		{
			int cx = m_info[0].clientX;
			int cy = m_info[0].clientY;
			m_selectBar->Enable();
			
			bActiveSelectBar = 1;
		}

		n ++;
	}

	
	if ( bActiveSelectBar == 0 )
	{
		m_selectedIdx = -1;
		m_selectedQuestIdx = -1;
	}

	
	sprintf( szTemp, g_LPACK.GetMassage( 0, 530 ) );
	m_textCtl[IT_QUEST_WIN_ITEM]->SetText( szTemp );

	return( IF_CTL_NONE );
}

void CIFQuestWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_listBar->Render();
	m_selectBar->Render();
	m_itemBar->Render();
	m_listLine->Render();
	m_invenBar->Render();

	m_closeBtn->Render();
	m_stopBtn->Render();

	
	g_ifMng->m_iconCtl->BeginFrame();
	{
		int cx, cy, n = 0;
		
		for( int i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
		{
			int itemTableIdx = g_cgv.myCharacterInfo->questInventory[i].item.itemIdx;
			int itemNumber = g_cgv.myCharacterInfo->questInventory[i].item.itemCount;

			if( itemTableIdx >= 0 && itemNumber > 0 )
			{
				cx = m_textCtl[IT_QUEST_WIN_INVEN_POS]->m_info.clientX + ( n % 6 ) * 34;
				cy = m_textCtl[IT_QUEST_WIN_INVEN_POS]->m_info.clientY + ( n / 6 ) * 33;

				
				g_ifMng->m_iconCtl->Draw( cx, cy, IF_ICON_ITEM, g_itemTable[itemTableIdx].iconItemID );

				n ++;
			}
		}
	}
	g_ifMng->m_iconCtl->EndFrame();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_QUEST_WIN_TITLE]->Render();
		m_textCtl[IT_QUEST_WIN_LIST]->Render();
		m_textCtl[IT_QUEST_WIN_LIST_0_U]->Render();
		m_textCtl[IT_QUEST_WIN_LIST_1_U]->Render();
		m_textCtl[IT_QUEST_WIN_LIST_2_U]->Render();
		m_textCtl[IT_QUEST_WIN_LIST_3_U]->Render();
		m_textCtl[IT_QUEST_WIN_LIST_4_U]->Render();
		m_textCtl[IT_QUEST_WIN_ITEM]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFQuestWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}



int CIFQuestWin::MakeRewardString( int idx, int value1, int value2, char *szTemp )
{
	switch (idx)
	{
	case QS_REWARD_EXP :
		strcpy( szTemp, g_ifMng->GetNumberString( value1 ) );
		strcat( szTemp, " " );
		strcat( szTemp, g_LPACK.GetMassage( 0, 461 ) );
		break;
	case QS_REWARD_GENEXP :
		strcpy( szTemp, g_ifMng->GetNumberString( value1 ) );
		strcat( szTemp, " " );
		strcat( szTemp, g_LPACK.GetMassage( 0, 462 ) );
		break;
	case QS_REWARD_GENCAP :
		strcpy( szTemp, g_ifMng->GetNumberString( value1 ) );
		strcat( szTemp, " " );
		strcat( szTemp, g_LPACK.GetMassage( 0, 740 ) );
		break;
	case QS_REWARD_NAK :
		strcpy( szTemp, g_ifMng->GetNumberString( value1 ) );
		strcat( szTemp, " " );
		strcat( szTemp, g_LPACK.GetMassage( 0, 485 ) );
		break;
	case QS_REWARD_ITEM :
		if ( value2 > 1 )
			sprintf( szTemp, "%s %d%s", g_itemTable[value1].name, value2, g_LPACK.GetMassage( 0, 760 ) );
		else
			sprintf( szTemp, "%s", g_itemTable[value1].name );
		break;
	case QS_REWARD_FAME:
		strcpy( szTemp, g_ifMng->GetNumberString( value1 ) );
		strcat( szTemp, " " );
		strcat( szTemp,g_LPACK.GetMassage( LPACK_TYPE_NORMAL2, 546 ) );
		break;

	default:
		strcpy( szTemp, "NULL" );
	}
	return 0;
}

int CIFQuestWin::WindowProcess()
{
	int mx, my, temp, x, y, pos;
	int x1, y1, xs, ys;
	int seletedIdx = -1;
	char szTemp[256];

	if( m_closeBtn->ProcessButton() == IF_CTL_SELECT ) Enable( false );

	
	if( m_stopBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_selectedIdx >= 0 && m_selectedQuestIdx >= 0 )
		{
			
			
			if ( m_selectedQuestIdx == QUEST_SCROLL_INDEX )
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 5 ), g_questscrollTable[g_cgv.myCharacterInfo->questStatus[m_selectedQuestStatusIdx].dField[0]].name );
			}
			
			else if ( m_selectedQuestIdx == AUTOQUESTSCROLL_ITEM_INDEX )
			{
				sprintf( szTemp, g_LPACK.GetMassage( 0, 5 ), g_autoquestscrollTable[g_cgv.myCharacterInfo->questStatus[m_selectedQuestStatusIdx].dField[0]].name );
			}
			else
				sprintf( szTemp, g_LPACK.GetMassage( 0, 5 ), g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTNAME, m_selectedQuestIdx ) );

			g_ifMng->SetMessage( g_LPACK.GetMassage( 0, 322 ), szTemp, MSG_BUTTONTYPE_OKCANCEL, IF_MESSAGE_STOP_QUEST );
			
			return IF_CTL_NONE;
		}

		m_selectedIdx = -1;
		m_selectedQuestIdx = -1;

		return( IF_CTL_NONE );
	}
	
	
	mx = g_input->GetPosX();
	my = g_input->GetPosY();
	x1 = m_textCtl[IT_QUEST_WIN_LIST_0_U]->GetXPos();
	y1 = m_textCtl[IT_QUEST_WIN_LIST_0_U]->GetYPos();
	xs = m_textCtl[IT_QUEST_WIN_LIST_0_U]->GetXSize();
	ys = g_cgv.myCharacterInfo->questNumber * 13;
	mx = mx - x1;
	my = my - y1;
	
	if( mx < 0 || my < 0 || mx > xs || my > ys ) 
		seletedIdx = -1;
	else
		seletedIdx = my / 13;
	
	
	int i, questNumber = -1, questIdx = 0, step;
	
	for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
	{
		if( g_cgv.myCharacterInfo->questStatus[i].questIdx < 0 ) 
			continue;

		questNumber ++;
		if( questNumber == m_selectedIdx ) 
		{
			m_selectedQuestIdx = g_cgv.myCharacterInfo->questStatus[i].questIdx;
			
			m_selectedQuestStatusIdx = i;
			break;
		}
	}
	
	if( seletedIdx >= 0 && seletedIdx < g_cgv.myCharacterInfo->questNumber )
	{
		questNumber = -1;
		questIdx = 0;

		for( i = 0; i < MAX_COUNT_QUEST_STATUS; i ++ )
		{
			if( g_cgv.myCharacterInfo->questStatus[i].questIdx < 0 ) 
				continue;

			questNumber ++;
			if( questNumber == seletedIdx ) 
			{
				questIdx = g_cgv.myCharacterInfo->questStatus[i].questIdx;
				step = g_cgv.myCharacterInfo->questVar[questIdx].var.step;
				break;
			}
		}
		if( g_input->GetLBUp() ) 
		{
			if( questNumber >= 0 ) 
			{
				m_selectedIdx = seletedIdx;
				m_selectBar->Enable();

				g_ifMng->m_iPos[IF_QUEST_WIN_SELECT_BAR].clientY = m_selectBarSavePos + m_selectedIdx * 13;
				m_selectBar->UpdateControl();
				m_selectBar->UpdateBuffer();
			}
			else 
			{
				m_selectedIdx = -1;
				m_selectBar->Enable( false );
			}
		}
		else if( g_input->GetLBNone() )
		{
			
			g_ifMng->InitRolloverText();

			
			if (questIdx == QUEST_SCROLL_INDEX)
			{
				char szReward[128];
				questStatus_t * questStatus;
				questStatus = &(g_cgv.myCharacterInfo->questStatus[i]);

				MakeRewardString( questStatus->dField[5], questStatus->dField[6], questStatus->dField[8], szReward );

				if ( questStatus->dField[1] < 0 )
					sprintf( szTemp, "Sorry for bug... Please cancel..." );
				else
					sprintf(	szTemp, g_LPACK.GetMassage( 0, 755 ), 
								g_monsterTable[	questStatus->dField[1] ].name,
								questStatus->dField[2],
								szReward,
								questStatus->dField[3],
								questStatus->dField[2]	);
				
				g_ifMng->AddRolloverText( g_questscrollTable[questStatus->dField[0]].name, 5 );
				g_ifMng->AddRolloverText( " ", 0 );
				g_ifMng->AddRolloverText( szTemp, 0 );
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, -1, -1, CHAT_ARRANGE_NONE );
			}
			
			else if (questIdx == AUTOQUESTSCROLL_ITEM_INDEX)
			{
				char szType[128], szTemp1[128], szTemp2[128];
				questStatus_t * questStatus;
				questStatus = &(g_cgv.myCharacterInfo->questStatus[i]);
				int Index = g_cgv.myCharacterInfo->questStatus[i].dField[0];			
								
				if ( questStatus->dField[1] < 0 )
					sprintf( szTemp, "Sorry for bug... Please cancel..." );
				else
				{				

					sprintf( szTemp, g_LPACK.GetMassage( 10, 1 ), 
						g_monsterTable[questStatus->dField[1]].name,
						questStatus->dField[2] );
					
					g_ifMng->AddRolloverText( g_autoquestscrollTable[questStatus->dField[0]].name, 5 );
					g_ifMng->AddRolloverText( " ", 0 );
					g_ifMng->AddRolloverText( szTemp, 0 );
				
					int exp = g_autoquestscrollTable[Index].data.reward.exp;	
					if(exp == -1)
						exp = 0;
					else
					{
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_EXP, exp, 0, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					int genExp = g_autoquestscrollTable[Index].data.reward.genExp;
					if(genExp == -1)
						genExp = 0;
					else
					{
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_GENEXP, genExp, 0, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					int genCap = g_autoquestscrollTable[Index].data.reward.genCap;
					if(genCap == -1)
						genCap = 0;
					else
					{
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_GENCAP, genCap, 0, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

				
					int fame = g_autoquestscrollTable[Index].data.reward.fame;
					if(fame == -1)
						fame = 0;
					else{
						g_cgv.myCharacterInfo->rankPoint += fame;
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_FAME, fame, 0, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
						
					}


					int nak = g_autoquestscrollTable[Index].data.reward.nak;
					if(nak == -1)
						nak = 0;
					else
					{
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_NAK, nak, 0, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					int itemIdx = g_autoquestscrollTable[Index].data.reward.itemIdx;
					int itemNo = g_autoquestscrollTable[Index].data.reward.itemNo;

					if(itemIdx != -1)
					{
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemIdx, itemNo, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}				

					
					if(g_autoquestscrollTable[Index].data.reward.regItemIndex0 !=-1)
					{
						int itemindex=g_autoquestscrollTable[Index].data.reward.regItemIndex0;
						int itemcount=g_autoquestscrollTable[Index].data.reward.regItemNo0;
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					if(g_autoquestscrollTable[Index].data.reward.regItemIndex1 !=-1)
					{
						int itemindex=g_autoquestscrollTable[Index].data.reward.regItemIndex1;
						int itemcount=g_autoquestscrollTable[Index].data.reward.regItemNo1;
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					if(g_autoquestscrollTable[Index].data.reward.regItemIndex2 !=-1)
					{
						int itemindex=g_autoquestscrollTable[Index].data.reward.regItemIndex2;
						int itemcount=g_autoquestscrollTable[Index].data.reward.regItemNo2;
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					if(g_autoquestscrollTable[Index].data.reward.regItemIndex3 !=-1)
					{
						int itemindex=g_autoquestscrollTable[Index].data.reward.regItemIndex3;
						int itemcount=g_autoquestscrollTable[Index].data.reward.regItemNo3;
						g_ifMng->m_questWin->MakeRewardString( QS_REWARD_ITEM, itemindex, itemcount, szType );
						sprintf( szTemp1, g_LPACK.GetMassage( 10, 2 ), szType );
						g_ifMng->AddRolloverText( szTemp1, 0 );
					}

					

					
					sprintf( szTemp2, g_LPACK.GetMassage( 10, 3 ), 
						questStatus->dField[3],
						questStatus->dField[2] );
					g_ifMng->AddRolloverText( szTemp2, 0 );
				}
						
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, -1, -1, CHAT_ARRANGE_NONE );			
			}
			else
			{
				g_ifMng->AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTNAME, questIdx ), 5 );
				g_ifMng->AddRolloverText( g_LPACK.GetMassage( LPACK_TYPE_QUESTINFO, CLPack_Quest::TYPE_QUESTSTEP, questIdx, step ), 0 );
				g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, -1, -1, CHAT_ARRANGE_NONE );
			}

			m_selectedQuestIdx = questIdx;
			
			m_selectedQuestStatusIdx = i;
		}
	}
	else
	{
		if( g_input->GetLBUp() ) m_selectedIdx = -1;
	}

	mx = g_input->GetPosX() - m_textCtl[IT_QUEST_WIN_INVEN_POS]->GetXPos();
	my = g_input->GetPosY() - m_textCtl[IT_QUEST_WIN_INVEN_POS]->GetYPos();

	if( mx <= 0 || my <= 0 ) return( IF_CTL_NONE );
	if( mx >= 34 * 6 ||  my >= 33 * 4 ) return( IF_CTL_NONE );

	temp = mx / 34;
	x = temp % 6;
	temp = my / 33;
	y = temp % 4;

	pos = y * 6 + x;

	
	if( pos < 0 || pos >= 24 )
	{
		return( IF_CTL_NONE );
	}

	int n = 0;
	
	for( i = 0; i < MAX_COUNT_QUEST_INVENTORY; i ++ )
	{
		int itemTableIdx = g_cgv.myCharacterInfo->questInventory[i].item.itemIdx;
		int itemNumber = g_cgv.myCharacterInfo->questInventory[i].item.itemCount;
		
		if( itemTableIdx >= 0 && itemNumber > 0 )
		{	
			if( n == pos ) break;
			n ++;
		}
	}
	if( i == MAX_COUNT_QUEST_INVENTORY ) return IF_CTL_NONE;

	if( g_cgv.myCharacterInfo->questInventory[i].item.itemIdx >= 0 &&
		g_cgv.myCharacterInfo->questInventory[i].item.itemCount > 0 )
	{
		item_t qItem;

		GTH_InitItem( &qItem );
		qItem.itemTableIdx = g_cgv.myCharacterInfo->questInventory[i].item.itemIdx;
		qItem.durability = g_cgv.myCharacterInfo->questInventory[i].item.itemCount - 1;
		strcpy( qItem.name, g_itemTable[qItem.itemTableIdx].name );

		g_ifMng->InitRolloverText();
		g_ifMng->MakeItemRollover( &qItem, true );
		g_ifMng->MakeRolloverText( g_input->GetPosX(), g_input->GetPosY(), true, 250 );
	}

	return IF_CTL_NONE;
}




CIFGuild_battle_info_win::CIFGuild_battle_info_win( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	for( int i=0; i<NUM_IT_GUILD_BATTLE_INFO_WIN; i++ )
		m_textCtl[i] = new CIFText( this );

	m_title				= new CIFControl( this );	



	Enable( false );
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GUILDARENA_INFO_WINDOW_TYPE );


	m_AteamPoint = 0;
	m_AGuildTotal = 0;	
	m_AJoinTotal = 0;

	m_BteamPoint = 0;
	m_AGuildTotal  =0;	
	m_AJoinTotal  =0;

	m_RemainTime  =0;
	
	
	memset(m_AteamName, NULL , sizeof (char) * NAMESTRING );
	memset(m_BteamName, NULL , sizeof (char) * NAMESTRING );
	
	

}
CIFGuild_battle_info_win::~CIFGuild_battle_info_win()
{

}
void CIFGuild_battle_info_win::PrepareControl()
{







	CIFControl::PrepareControl( IF_GUILD_BATTLE_INFO_WIN );




	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_TIME]->PrepareControl(IF_GUILD_BATTLE_TIME);

	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_ATEAM_NAME]->PrepareControl(IF_GUILD_BATTLE_ATEAMINFO_NAME);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_ATEAM_JOIN_INFO]->PrepareControl(IF_GUILD_BATTLE_ATEAMINFO_JOIN);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_ATEAM_KILL_INFO]->PrepareControl(IF_GUILD_BATTLE_ATEAMINFO_KILL);

	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_BTEAM_NAME]->PrepareControl(IF_GUILD_BATTLE_BTEAMINFO_NAME);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_BTEAM_JOIN_INFO]->PrepareControl(IF_GUILD_BATTLE_BTEAMINFO_JOIN);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_BTEAM_KILL_INFO]->PrepareControl(IF_GUILD_BATTLE_BTEAMINFO_KILL);


	

}
void CIFGuild_battle_info_win::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	



	for(int i =0; i < NUM_IT_GUILD_BATTLE_INFO_WIN; i ++)
		m_textCtl[i]->Render();
	
}

int	CIFGuild_battle_info_win::WindowFrame()
{

	
	








	if ( g_ifMng->m_guildArenaWin->m_ReadyBattleFlag != g_ifMng->m_guildArenaWin->STATE_BATTLE && 
		g_ifMng->m_guildArenaWin->m_ReadyBattleFlag != g_ifMng->m_guildArenaWin->STATE_READY )
		return 0;	
	
	char szTemp[256];
	
	if( m_RemainTime > 0.0f )	
		m_RemainTime -=  g_timer.GetElapsedMilliTime();
	else
		return 0;
	
	
	UINT RemainMin = m_RemainTime/60000;
	UINT RemainnSec = (m_RemainTime%60000) / 1000;	
	ShowLogInfo("RemainMin = %d, RemainSec = %d, danflag=%d", RemainMin, RemainnSec, g_ifMng->m_guildArenaWin->m_ReadyBattleFlag);
	
	
	UINT MaxCheckCount = 0;
	UINT MinCheckCount = 0;
	int NowEffectID = 0;
	
	if ( g_ifMng->m_guildArenaWin->m_ReadyBattleFlag == g_ifMng->m_guildArenaWin->STATE_BATTLE) 
	{	
		MaxCheckCount = 10;
		MinCheckCount = 0;
		NowEffectID = 64-(RemainnSec);
	}
	else
	{
		MaxCheckCount = 11;
		MinCheckCount = 1;
		NowEffectID = 64-(RemainnSec-1);
		
	}
	
	
	
	if ( (RemainnSec >= MinCheckCount && RemainnSec <= MaxCheckCount) && RemainMin == 0 )		
	{		
		if ( m_EffectFlag & (1 << NowEffectID) )
			return IF_CTL_NONE;		
		
		if ( NowEffectID == 64 && g_ifMng->m_guildArenaWin->m_ReadyBattleFlag 
			== g_ifMng->m_guildArenaWin->STATE_BATTLE) 
			return IF_CTL_NONE;
		
		g_effectMng->AddSelfEffect( GTH_EFFECT_CLASS_MODEL, (NowEffectID), 
			g_pApp->m_myCharacter->position,
			NULL, 	true, 
			g_pApp->m_myCharacter->entityType, 
			g_pApp->m_myCharacter->idxOnServer,
			0.0f, 0.0f, 0.0f, -1.0f,
			false );	
		
		
		if ( NowEffectID != 64)
			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_COUNT ), GTH_SOUND_PLAY_ONCE );
		else
			g_musicMng->PlaySample( g_soundMng->GetNormalSoundID( GTH_SOUND_ID_BATTLE_READYFIGHT ), GTH_SOUND_PLAY_ONCE );
		
		
		
		m_EffectFlag |= 1 << NowEffectID;
		ShowLogInfo("RemainMin = %d, RemainSec = %d", RemainMin, RemainnSec);
	}
	

	
	sprintf(szTemp, "%d(min) %d(sec)", RemainMin, RemainnSec);	
	
	

	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_TIME]->SetText(szTemp);

	sprintf( szTemp, "[%s]%s", m_AteamName,
		g_LPACK.GetMassage( 9, 45 ) );	
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_ATEAM_NAME]->SetText(szTemp);

	sprintf(szTemp, "%d/%d", m_AJoinTotal, m_AGuildTotal);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_ATEAM_JOIN_INFO]->SetText(szTemp);

	sprintf(szTemp, "%d", m_AteamPoint);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_ATEAM_KILL_INFO]->SetText(szTemp);	

	sprintf( szTemp, "[%s]%s", m_BteamName,
		g_LPACK.GetMassage( 9, 45 ) );	

	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_BTEAM_NAME]->SetText(szTemp);

	sprintf(szTemp, "%d/%d", m_BJoinTotal, m_BGuildTotal);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_BTEAM_JOIN_INFO]->SetText(szTemp);

	sprintf(szTemp, "%d", m_BteamPoint);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_BTEAM_KILL_INFO]->SetText(szTemp);
	
	return IF_CTL_NONE;

}
int	CIFGuild_battle_info_win::WindowProcess()
{
	
	return IF_CTL_NONE;
}
int	CIFGuild_battle_info_win::WindowSelection()
{
	return IF_CTL_NONE;
	
}


void CIFGuild_battle_info_win::SetBattleTeams(int atemidx, int bteamidx)
{
	memset(m_AteamName, NULL , sizeof (char) * NAMESTRING );
	memset(m_BteamName, NULL , sizeof (char) * NAMESTRING );

	
	strcpy(g_ifMng->m_guildArenaInfoWin->m_AteamName, 
		g_ifMng->m_guildArenaWin->m_szStandbyList[g_ifMng->m_guildArenaWin->m_RegAteamIdx]);
	strcpy(g_ifMng->m_guildArenaInfoWin->m_BteamName, 
		g_ifMng->m_guildArenaWin->m_szStandbyList[g_ifMng->m_guildArenaWin->m_RegBteamIdx]);	

	
	m_EffectFlag = 0;
}


void CIFGuild_battle_info_win::SetBattleRemainTime(DWORD remaintime)
{
	m_RemainTime = remaintime;
	m_EffectFlag = 0;
	
}













CIFSurgeryWin::CIFSurgeryWin( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{
	
	for( int i = 0; i < NUM_IT_SURGERY_WIN; i ++ )
		m_textCtl[i] = new CIFText( this );
	
	m_titleBar = new CIFControl( this );
	m_titleBottom = new CIFControl( this );
	m_faceIcon = new CIFControl( this );
	m_hairIcon = new CIFControl( this );
	m_faceBar = new CIFControl( this );
	m_hairBar = new CIFControl( this );

	m_facePrevBtn = new CIFButtonCtl( this );
	m_faceNextBtn = new CIFButtonCtl( this );
	m_hairPrevBtn = new CIFButtonCtl( this );
	m_hairNextBtn = new CIFButtonCtl( this );
	m_okBtn = new CIFButtonCtl( this );
	m_cancelBtn = new CIFButtonCtl( this );

	Enable( false );

	
	g_ifMng->AddFocus( this );
	SetWindowType( IF_SURGERY_WINDOW_TYPE );

	m_FaceCount = 0;
	m_HairCount = 0;
	m_maxFaceCount = 0;
	m_maxHairCount = 0;
	m_BackUpFaceIdx = 0;
	m_BackUpHairIdx = 0;
}

CIFSurgeryWin::~CIFSurgeryWin()
{

}

void CIFSurgeryWin::PrepareControl()
{
	CIFControl::PrepareControl( IF_SURGERY_WIN );

	
	m_textCtl[IT_SURGERY_WIN_FACE]->PrepareControl( IF_SURGERY_WIN_FACE_TEXT );
	m_textCtl[IT_SURGERY_WIN_HAIR]->PrepareControl( IF_SURGERY_WIN_HAIR_TEXT );
	m_textCtl[IT_SURGERY_WIN_FACE_U]->PrepareControl( IF_SURGERY_WIN_FACE_TEXT_U );
	m_textCtl[IT_SURGERY_WIN_HAIR_U]->PrepareControl( IF_SURGERY_WIN_HAIR_TEXT_U );
	m_textCtl[IT_SURGERY_WIN_OK]->PrepareControl( IF_SURGERY_WIN_OK_TEXT );
	m_textCtl[IT_SURGERY_WIN_CANCEL]->PrepareControl( IF_SURGERY_WIN_CANCEL_TEXT );

	m_titleBar->PrepareControl( IF_SURGERY_WIN_TITLE_BAR );
	m_titleBottom->PrepareControl( IF_SURGERY_WIN_TITLE_BAR_BOTTOM );
	m_faceIcon->PrepareControl( IF_SURGERY_WIN_FACE_ICON );
	m_hairIcon->PrepareControl( IF_SURGERY_WIN_HAIR_ICON );
	m_faceBar->PrepareControl( IF_SURGERY_WIN_FACE_BAR );
	m_hairBar->PrepareControl( IF_SURGERY_WIN_HAIR_BAR );

	m_facePrevBtn->PrepareControl( IF_SURGERY_WIN_FACE_PREV_BTN_0 );
	m_faceNextBtn->PrepareControl( IF_SURGERY_WIN_FACE_NEXT_BTN_0 );
	m_hairPrevBtn->PrepareControl( IF_SURGERY_WIN_HAIR_PREV_BTN_0 );
	m_hairNextBtn->PrepareControl( IF_SURGERY_WIN_HAIR_NEXT_BTN_0 );
	m_okBtn->PrepareControl( IF_SURGERY_WIN_OK_BTN_0 );
	m_cancelBtn->PrepareControl( IF_SURGERY_WIN_CANCEL_BTN_0 );

}

int CIFSurgeryWin::WindowFrame()
{
	char szTemp[256];

	




	sprintf( szTemp, "%d/%d", m_FaceCount+1, m_maxFaceCount );
	m_textCtl[IT_SURGERY_WIN_FACE_U]->SetText( szTemp );

	sprintf( szTemp, "%d/%d", m_HairCount+1, m_maxHairCount );
	m_textCtl[IT_SURGERY_WIN_HAIR_U]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 517 ) );
	m_textCtl[IT_SURGERY_WIN_FACE]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 518 ) );
	m_textCtl[IT_SURGERY_WIN_HAIR]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 549 ) );
	m_textCtl[IT_SURGERY_WIN_OK]->SetText( szTemp );

	sprintf( szTemp, g_LPACK.GetMassage( 0, 551 ) );
	m_textCtl[IT_SURGERY_WIN_CANCEL]->SetText( szTemp );

	return( IF_CTL_NONE );
}

void CIFSurgeryWin::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();
	CIFControl::Render();
	
	m_titleBar->Render();
	m_titleBottom->Render();
	m_faceIcon->Render();
	m_hairIcon->Render();
	m_faceBar->Render();
	m_hairBar->Render();

	m_facePrevBtn->Render();
	m_faceNextBtn->Render();
	m_hairPrevBtn->Render();
	m_hairNextBtn->Render();

	m_okBtn->Render();
	m_cancelBtn->Render();

	
	g_chatManager->SetOutputType( CHAT_OUTPUT_SHADOW );
	g_chatManager->BeginFrame();
	{
		m_textCtl[IT_SURGERY_WIN_FACE_U]->Render();
		m_textCtl[IT_SURGERY_WIN_HAIR_U]->Render();
		m_textCtl[IT_SURGERY_WIN_FACE]->Render();
		m_textCtl[IT_SURGERY_WIN_HAIR]->Render();
		m_textCtl[IT_SURGERY_WIN_OK]->Render();
		m_textCtl[IT_SURGERY_WIN_CANCEL]->Render();
	}
	g_chatManager->EndFrame();
}

int CIFSurgeryWin::WindowSelection()
{
	int ret;

	
	ret = m_titleBar->Selection();
	if( ret == IF_LBDN ) return( IF_CTL_BEGIN_MOVING );
	ret = Selection();
	if( ret == IF_LBDN ) return( IF_CTL_SELECT );

	return( IF_CTL_NONE );
}

int CIFSurgeryWin::WindowProcess()
{

	if( m_hairPrevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_HairCount <= 0 )
			m_HairCount = 0;
		else
			m_HairCount--;
		
		g_charManager->m_Characters[0].skinPart[0] = m_HairCount;
		g_camera.m_angles[ YAW ] = g_pApp->m_myCharacter->angles[ YAW ] + 180;
		g_camera.m_distance = -40;

		g_charManager->GenerateOtherCharacter(0);
		GTH_SendMessage_SkinPart(FALSE);
	}
	else if( m_hairNextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_HairCount >= m_maxHairCount-1 ) 
			m_HairCount = m_maxHairCount-1;
		else
			m_HairCount++;

		g_charManager->m_Characters[0].skinPart[0] = m_HairCount;
		g_camera.m_angles[ YAW ] = g_pApp->m_myCharacter->angles[ YAW ] + 180;
		g_camera.m_distance = -40;

		g_charManager->GenerateOtherCharacter(0);
		GTH_SendMessage_SkinPart(FALSE);
	}
	
	if( m_facePrevBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_FaceCount <= 0 )
			m_FaceCount = 0;
		else
			m_FaceCount--;

		g_charManager->m_Characters[0].skinPart[1] = m_FaceCount;

		g_camera.m_angles[ YAW ] = g_pApp->m_myCharacter->angles[ YAW ] + 180;
		g_camera.m_distance = -40;

		g_charManager->GenerateOtherCharacter(0);
		GTH_SendMessage_SkinPart(FALSE);
	}
	else if( m_faceNextBtn->ProcessButton() == IF_CTL_SELECT )
	{
		if( m_FaceCount >= m_maxFaceCount-1 ) 
			m_FaceCount = m_maxFaceCount-1;
		else
			m_FaceCount++;

		g_charManager->m_Characters[0].skinPart[1] = m_FaceCount;
		g_camera.m_angles[ YAW ] = g_pApp->m_myCharacter->angles[ YAW ] + 180;
		g_camera.m_distance = -40;

		g_charManager->GenerateOtherCharacter(0);
		GTH_SendMessage_SkinPart(FALSE);
	}

	if( m_okBtn->ProcessButton() == IF_CTL_SELECT )
	{
		GTH_SendMessage_Request_UsePotion(g_cgv.saveInventoryIdx);
		g_charManager->GenerateOtherCharacter(0);
		GTH_SendMessage_SkinPart(TRUE);
		Enable( false );
	}
	
	if( m_cancelBtn->ProcessButton() == IF_CTL_SELECT )
	{
		g_charManager->m_Characters[0].skinPart[0] = m_BackUpHairIdx;
		g_charManager->m_Characters[0].skinPart[1] = m_BackUpFaceIdx;
		g_charManager->GenerateOtherCharacter(0);
		GTH_SendMessage_SkinPart(FALSE);
		Enable( false );
	}

	return IF_CTL_NONE;
}




CIFGonryunBattleProgressInfo::CIFGonryunBattleProgressInfo( CIFControlNode *pParentControl ) : CIFWindow( pParentControl )
{

		m_textCtl[0] = new CIFText( this );

	m_title				= new CIFControl( this );	



	Enable( false );
	g_ifMng->AddFocus( this );
	SetWindowType( IF_GONRYUN_INFO_WINDOW_TYPE );


	m_AteamPoint = 0;
	m_AGuildTotal = 0;	
	m_AJoinTotal = 0;

	m_BteamPoint = 0;
	m_AGuildTotal  =0;	
	m_AJoinTotal  =0;

	m_RemainTime  =0;
	
	
	memset(m_AteamName, NULL , sizeof (char) * NAMESTRING );
	memset(m_BteamName, NULL , sizeof (char) * NAMESTRING );
	
	

}
CIFGonryunBattleProgressInfo::~CIFGonryunBattleProgressInfo()
{

}
void CIFGonryunBattleProgressInfo::PrepareControl()
{







	CIFControl::PrepareControl( IF_GUILD_BATTLE_INFO_WIN );




	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_TIME]->PrepareControl(IF_GUILD_BATTLE_TIME);










	

}
void CIFGonryunBattleProgressInfo::Render()
{
	if( !State() ) return;

	g_ifMng->SetRenderState();

	CIFControl::Render();
	





		m_textCtl[0]->Render();
}

int	CIFGonryunBattleProgressInfo::WindowFrame()
{
	char szTemp[256];
	
	if( m_RemainTime > 0.0f )	
		m_RemainTime -= g_timer.GetElapsedMilliTime();

	sprintf(szTemp, "%d(min) %d(sec)", m_RemainTime/60000, (m_RemainTime%60000) / 1000);
	m_textCtl[IT_GUILD_BATTLE_INFO_WIN_TIME]->SetText(szTemp);





	return IF_CTL_NONE;

}
int	CIFGonryunBattleProgressInfo::WindowProcess()
{
	
	return IF_CTL_NONE;
}
int	CIFGonryunBattleProgressInfo::WindowSelection()
{
	return IF_CTL_NONE;
	
}


void CIFGonryunBattleProgressInfo::SetBattleTeams(int atemidx, int bteamidx)
{
	memset(m_AteamName, NULL , sizeof (char) * NAMESTRING );
	memset(m_BteamName, NULL , sizeof (char) * NAMESTRING );

	




}

