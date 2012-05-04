#define STRICT
#include <windows.h>
#include <basetsd.h>
#include <tchar.h>
#include <stdlib.h>
#include "global_def.h"
#include "resource.h"

#include "GTH.h"
#include "../game/GolryunBattle/GolryunBattle.h"

extern itementity_t *g_mouseSelectedItemObject;
extern Fx_CHARACTER_t *g_mouseSelectedCharacter;
extern int g_freeMove;
extern int g_volume;

int GTH::SetupView()
{
	POINT p;
	vec3_t rotate;
	int  delta;

	if( ( delta = g_input->GetWheelDelta() ) != 0 )	g_camera.Zoom( delta );
	
	g_input->GetCursorPos( p );
	if( g_input->GetMouseMode() == INPUT_MOUSE_SCROLL_MODE )
	{
		rotate[ PITCH ] = (float) p.y * SIZE_MOUSE_ROTATE * m_fElapsedTime;
		rotate[ YAW ] = -(float) p.x * SIZE_MOUSE_ROTATE * m_fElapsedTime;
		rotate[ ROLL ] = 0.0f;
		g_camera.RotateAngles( rotate );
	}
	return( true );
}

#define KEYBOARD_ROTATE_SIZE		15


int GTH::SetupViewWithKey()
{
	vec3_t rotate;
	int px = 0, py = 0;
	bool bRotate = false;

	if( g_cgv.state != CLIENT_GAME_PROCESS ) return( true );

	
	if( g_chatManager->bIsActiveCompWnd ) return( true );

	if( g_ifMng->m_boothWin->State() || 
		g_ifMng->m_mailEditWin->State() ||
		g_ifMng->m_guildWin->State() ||
	
		g_ifMng->m_mailBoxWin->State() ||

	
		g_ifMng->m_Item_ScrollWin->State() ||

	
		g_ifMng->m_worldChatWin->State() ) return ( true );

	if( ( g_chatManager->m_inputStr[0][0] == 0    || g_chatManager->m_inputStr[0][0] == '/'  || g_chatManager->m_inputStr[0][0] == '\"' ||
		  g_chatManager->m_inputStr[0][0] == '#'  || g_chatManager->m_inputStr[0][0] == '&'  || g_chatManager->m_inputStr[0][0] == '@' ) 
		   && g_chatManager->m_inputStr[0][1] == 0 )	bRotate = true;
	
	if( g_chatManager->m_inputStr[0][0] == '\'' )
	{
		char szTemp[CHATSTRING];

		if( g_ifMng->m_dlgWin->m_whisperName[0] == 0 ) 
			sprintf( szTemp, "'" );
		else 
			sprintf( szTemp, "'%s ", g_ifMng->m_dlgWin->m_whisperName );

		if( !stricmp( szTemp, g_chatManager->m_inputStr[0] ) )
			bRotate = true;
	}

	if( !bRotate )
		return ( true );

	
	if( g_input->KeyEvent( DIK_LEFT ) )		py = -KEYBOARD_ROTATE_SIZE;
	if( g_input->KeyEvent( DIK_RIGHT ) )	py = KEYBOARD_ROTATE_SIZE;
	
	if( g_input->KeyEvent( DIK_PRIOR ) )	px = -KEYBOARD_ROTATE_SIZE;
	if( g_input->KeyEvent( DIK_NEXT ) )		px = KEYBOARD_ROTATE_SIZE;
	if( g_input->KeyEvent( DIK_UP ) )		g_camera.Zoom( KEYBOARD_ROTATE_SIZE * 5 );
	if( g_input->KeyEvent( DIK_DOWN ) )		g_camera.Zoom( -KEYBOARD_ROTATE_SIZE * 5 );	
	
	
	
	rotate[ PITCH ] = (float) px * SIZE_MOUSE_ROTATE * m_fElapsedTime;
	rotate[ YAW ] = -(float) py * SIZE_MOUSE_ROTATE * m_fElapsedTime;
	rotate[ ROLL ] = 0.0f;
	g_camera.RotateAngles( rotate );

	return( true );
}

int GTH::SetupKeys()
{


	return( true );
}


float	g_attackTime = 0.0f;
float	g_realAttackTime = 0.0f;
float	g_attackTimeFrame = 0.0f;
float	g_realAttackTimeFrame = 0.0f;

void GTH::SetupCursor()
{

}

void GTH::SetupCursor_Neo()
{
	vec3_t touched;
	vec2_t mouse;
	vec2_t viewport;

	float passedTime;

	int changeTargetFlag = false;
	
	g_cgv.saveMenuTime -= g_timer.GetElapsedMilliTime();
	if( g_cgv.saveMenuTime < 0.0f )
	{
		g_cgv.saveMenuTime = -1.0f;
	}


	
	GTH_EpackProcess();
	g_SkillApp.GTH_SkillCastingProcess();
	g_SkillLogic.GTH_SkillCoolTimeProcess();


	


  
	GTH_ItemUseTimeProcess();	

	
	

	if( m_myCharacter->pvpMode != g_cgv.pvpMove )
	{
		g_cgv.pvpMove = m_myCharacter->pvpMode;
		if( g_cgv.pvpMove ) 
		{
			
		
			
		   
			if(g_GonryunBattlePractic.m_ibattleMode==PCB_BATTING
				|| g_GonryunBattlePractic.m_ibattleMode==PCB_START) {
				
				g_musicMng->ChangeBGMusic( "../BGMusic/thefight.mp3" );
			}
			
			else {
				g_musicMng->ChangeBGMusic( m_mapInfos[ 0 ].BGMusic );
			}
		
		}
		
		else 
		{
			
	
			g_musicMng->ChangeBGMusic( m_mapInfos[ 0 ].BGMusic );
		}
	}
	


	
	g_cgv.pTargetChar = g_GameTarget.GTH_GetTargetPointer( m_myCharacter );
	if( g_cgv.pTargetChar == NULL )
	{
		m_myCharacter->targetIdx = -1;
		m_myCharacter->targetType = -1;
		g_cgv.myCharacterInfo->targetIdx = -1;
		g_cgv.myCharacterInfo->targetType = -1;
	}

	
	
	if( g_cgv.resendTargeting && g_cgv.pTargetChar != NULL && m_myCharacter->atk_changeEvent )
	{
		passedTime = g_timer.GetAppMilliTime() - g_cgv.resendTime;
		if( passedTime > TARGETING_RESEND_DELAY )
		{
			g_cgv.resendTime = g_timer.GetAppMilliTime();
			

			{
				VectorCopy( touched, g_cgv.pTargetChar->position );
				if( !VectorCompare( touched, g_cgv.oldTargetPosition ) )	
				{								
					VectorCopy( g_cgv.oldTargetPosition, touched );
					
					g_cursor->SetPointerTargetPos( touched , m_myCharacter->position );
					g_cursor->SetPlayerTargetedMove();
					
					g_cgv.keyRun = true;
					g_cgv.resendFlag = true;
				}
			}
		}
	}
	
	switch( g_cgv.saveEvent )
	{
		
	case GTH_EV_CHAR_ATTACK :
		{

			
			if( g_cgv.pTargetChar != NULL )
			{
				
				if( (g_cgv.pTargetChar->curRA <= 0) ||
					(g_cgv.saveTargetIdx != g_cgv.pTargetChar->idxOnServer) ) 
					{
				
						m_myCharacter->event = GTH_EV_CHAR_IDLE;
						g_cgv.saveEvent = -1;
						g_cgv.resendTargeting = false;
						g_cgv.resendFlag = false;
						m_myCharacter->atk_resend = false;
					}
			}
			
		

			if( m_myCharacter->atk_resend )
			{






				









					
					if(g_cgv.pTargetChar==NULL)break;
					
					float atkDistance = VectorDistance( g_cgv.pTargetChar->position, m_myCharacter->position );
					float	attackLength = 0.0f;

					if( m_myCharacter->isTransform )
					{
						attackLength = FindTransformAttackLength(m_myCharacter->transformEntityIdx, m_myCharacter->transformType);
						if( attackLength == 0.0f )
							attackLength = (float)g_cgv.myCharacterInfo->calAttackLength;
					}
					else
					{
						attackLength = (float)g_cgv.myCharacterInfo->calAttackLength;
					}

					if( atkDistance < (float)attackLength * 1.5f )
					{
						m_myCharacter->event	= GTH_EV_CHAR_ATTACK;
						g_cgv.resendAttack		= true;
						g_cgv.resendFlag		= true;
						g_cgv.resendTargeting	= false;
						g_cgv.sendSkillFlag		= false;
						m_myCharacter->atk_resend = true;

					}

					
					
					else if(atkDistance > (float)attackLength * 1.5f)
					{
						m_myCharacter->event = GTH_EV_CHAR_IDLE;
						g_cgv.saveEvent = -1;
						g_cgv.resendTargeting = false;
						g_cgv.resendFlag = false;
						m_myCharacter->atk_resend = false;
					}
		
					
				}			
		
		}
		break;
	case GTH_EV_CHAR_SKILL :
		{
			
			
			if( m_myCharacter->atk_changeEvent )
			{
				
				
				
				if(g_cgv.Skill.iSelectSkillUseType == globalVariable_t::SELECT_ITEM)
				{
					if(!g_SkillLogic.GTH_SkillProcess(g_cgv.Skill.iSelectSkillUseType,g_cgv.Skill.iUseInvenPos))
					{
						g_SkillLogic.UnSelectSkill();
					}
				}
				else
				{
					if(!g_SkillLogic.GTH_SkillProcess(g_cgv.Skill.iSelectSkillUseType,g_cgv.saveSkillIdx))
					{
						g_SkillLogic.UnSelectSkill();
					}
				}
			}
			

			
			if( m_myCharacter->atk_resend )
			{
				m_myCharacter->event		= GTH_EV_CHAR_SKILL;
				g_cgv.resendAttack			= false;
				g_cgv.sendSkillFlag			= true;
				g_cgv.resendFlag			= true;
				g_cgv.resendTargeting		= false;
				m_myCharacter->atk_resend	= true;

			}
			
		}
		break;
	case GTH_EV_CHAR_BEFORE_TELEPORT:
		{
			if( m_myCharacter->atk_resend && g_cgv.sendTeleportFlag)
			{
				if( g_cgv.pTargetChar != NULL )
				{
					if( g_cgv.pTargetChar->curRA >= 0 )
					{
						GTH_SendEventMessage_Teleport();

						m_myCharacter->event		= GTH_EV_CHAR_TELEPORT;
						g_cgv.sendTeleportFlag		= false;
						g_cgv.resendFlag			= false;
						g_cgv.resendTargeting		= false;
						m_myCharacter->atk_resend	= false;
					}
				}
				
			}
		}
		break;
	
	
	
	default:
		{
			if( m_myCharacter->atk_resend )
			{
				if( g_cgv.pTargetChar == NULL ) break;
				if( g_cgv.saveTargetIdx != g_cgv.pTargetChar->idxOnServer )	break;
				if( m_myCharacter->targetIdx != g_cgv.pTargetChar->idxOnServer ) break;

				float atkDistance = VectorDistance( g_cgv.pTargetChar->position, m_myCharacter->position );
				if( atkDistance < (float)g_cgv.myCharacterInfo->calAttackLength * 1.5f )
				{
					GTH_SendEventMessage_Attack();
				}
				m_myCharacter->atk_resend	= false;
				g_cgv.resendAttack			= false;
				g_cgv.resendFlag			= false;
				g_cgv.resendTargeting		= false;


			}
		}
		break;
	}

	
	if( m_frameFlag && GTH_CanMove() )
	{
		Vector2Set( mouse , (float)g_input->GetPosX() , (float)g_input->GetPosY() );
		Vector2Set( viewport , (float)g_cursor->m_viewport.Width , (float)g_cursor->m_viewport.Height );

		
		


		g_mouseSelectedItemObject = g_itemMng->SearchCursorEntityNeo( g_camera.m_position, g_camera.m_angles, g_camera.GetTransformMatrix(), 
			g_cursor->GetFarSquareDistance(), mouse, viewport );

		g_mouseSelectedCharacter = g_charManager->SearchCursorEntityNeo2 ( g_camera.m_position , g_camera.m_angles , g_camera.GetTransformMatrix() , 
			g_itemMng->GetMinSquareDist() , mouse , viewport );
		
		g_cursor->SetScreenPos( g_input->GetPosX() , g_input->GetPosY() );
		g_cursor->SetupMatrix( g_camera.GetInverseTransformMatrix() );
		g_cursor->ScreenToSpace( );

		
		if( g_cgv.state != CLIENT_GAME_PROCESS ) return;

		
		
		
		SetMouseEvent(touched);
	}
}


void	GTH::SetMouseEvent(vec3_t	touched)
{
	if( g_input->GetLBUp() )
	{
		if( g_mouseSelectedItemObject == NULL && g_mouseSelectedCharacter == NULL )
		{
			
			if( g_cursor->IsTouched() )
			{
				
				if(g_pApp->m_myCharacter->m_bObserveMode)	return;
				

				
				g_cursor->GetTouchedPos( touched );


				
				int flag = g_move->IsClick(touched ,g_pApp->m_myCharacter,false);
				if(flag == COLLIDE_STARTSOLID)
				{
					g_SkillFile.DebugOutString("StepSlideMove close filed %d \n",flag);
					return;
				}
				g_SkillFile.DebugOutString("StepSlideMove open filed  %d \n",flag);		

				g_cursor->SetPointerTargetPos( touched, m_myCharacter->position );
				
				
				if ( VectorDistance(touched , m_myCharacter->position ) < 40.0f )
					return;
				
				
				g_cursor->SetPlayerTargetedMove();
				g_cgv.clickEvent = GTH_CEV_CHAR_IDLE;
				g_cgv.saveEvent = -1;
			}
		}
		
		else if( g_mouseSelectedItemObject != NULL )
		{
			g_cgv.clickEvent = GTH_CEV_CHAR_PICKUP_ITEM;
			VectorCopy( touched, g_mouseSelectedItemObject->origin );
			g_cgv.targetItemIdxOnServer = g_mouseSelectedItemObject->idxOnServer;
			VectorClear( g_cgv.oldTargetPosition );
			g_cursor->SetPointerTargetPos( touched , m_myCharacter->position );
			g_cursor->SetPlayerTargetedMove();
			g_cgv.saveEvent = -1;
		}
			 
		else if	(	(	g_mouseSelectedCharacter	!=	NULL	)	&&
					(	! g_mouseSelectedCharacter ->flag_outOfMySight	)	)

		{
			
			if( g_cgv.pTargetChar == NULL )
			{
				g_GameTarget.GTH_ChangeTarget( g_mouseSelectedCharacter );
			}
			
			else if( !( m_myCharacter->targetType == g_mouseSelectedCharacter->entityType && m_myCharacter->targetIdx == g_mouseSelectedCharacter->idxOnServer ) )
			{
				g_GameTarget.GTH_ChangeTarget( g_mouseSelectedCharacter );
			}
			
			else
			{
				if( m_myCharacter->isTransform && m_myCharacter->transformType == TRANSFORM_ETHEREAL )
				{
					g_ifMng->AddSysMessage( g_LPACK.GetMassage( 0, 6 ) );
					return;
				}

				VectorClear( g_cgv.oldTargetPosition );	
				VectorCopy( touched, g_cgv.pTargetChar->position );
				g_cursor->SetPointerTargetPos( touched, m_myCharacter->position );

				switch( g_cgv.pTargetChar->entityType )
				{
				case ENTITY_MONSTER :
					g_SkillApp.ActionProcess( ACTION_ATTACK );
					break;
				case ENTITY_NPC :
					if( g_cgv.pTargetChar->scriptFlag ) g_cgv.clickEvent = GTH_CEV_CHAR_NPC_REQUEST;
					break;
				case ENTITY_PC :
					
					if( !g_AttackProc.IsSaftTargeting() ) 
					{
						g_cgv.clickEvent = GTH_CEV_CHAR_PC_REQUEST;
					}
					else 
					{
						
						AddTargetList( g_cgv.pTargetChar->name );
						g_SkillApp.ActionProcess( ACTION_ATTACK );
					}
					break;
				}
			}

			
			
			
			if( g_input->KeyEvent( DIK_LALT ) )
			{
				g_ifMng->m_dlgWin->SetWhisperFromTarget();
			}

		}
	}
}





























































































































































































































































































void GTH::GetKeyState()
{
	g_ifMng->ProcessIFKeyState();







	
	SetupViewWithKey();
}



HRESULT GTH::FreeMove()
{
    return S_OK;
}
