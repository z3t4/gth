



#include "../global_def.h"

#define	MAXTOUCH	32
typedef struct
{
	
	

	
	
	

	
	int			numtouch;
	

	vec3_t		viewangles;			
	float		viewheight;

	vec3_t		mins, maxs;			

	
	int			watertype;
	int			waterlevel;

	
	
	
} pmove_t;

#define	STEPSIZE	18





typedef struct
{
	vec3_t      origin;     
	vec3_t      velocity;   

	vec3_t      forward, right, up;
	float       frametime;


	int         groundsurfaceflags;
	plane_t    *groundplane;
	int         groundcontents;

	vec3_t      previous_origin;
	int         ladder;

	float       waterDepth;
} pml_t;

pmove_t		pm;
pml_t		pml;



float	pm_stopspeed = 100;
float	pm_maxspeed = 300;
float	pm_duckspeed = 100;
float	pm_accelerate = 10;
float	pm_airaccelerate = 0;
float	pm_wateraccelerate = 10;
float	pm_friction = 6;
float	pm_waterfriction = 1;
float	pm_waterspeed = 400;


vec3_t vec3_origin = {0,0,0};





CMove::CMove()
{
	CheckDuck();
}

CMove::~CMove()
{

}


#define PLAYER_EYE_HEIGHT 60.0f

void CMove::CatagorizePosition()
{
	vec3_t		point;
	int			cont;
	float       sample1;
	float       sample2;





	point[0] = pml.origin[0];
	point[1] = pml.origin[1];
	point[2] = pml.origin[2] + PLAYER_EYE_HEIGHT;

	



	pm.waterlevel = 0;
	pm.watertype = 0;

	sample2 = pm.viewheight - pm.mins[2];
	sample1 = sample2 / 2.0f;

	point[2] = pml.origin[2] + pm.mins[2] + 1;	
	cont = m_world->PointContents( point );

	if( cont & MASK_WATER )
	{
		pm.watertype = cont;
		pm.waterlevel = 1;
		point[2] = pml.origin[2] + pm.mins[2] + sample1;
		cont = m_world->PointContents( point );
		if( cont & MASK_WATER )
		{
			pm.waterlevel = 2;
			point[2] = pml.origin[2] + pm.mins[2] + sample2;
			cont = m_world->PointContents( point );
			if( cont & MASK_WATER )
				pm.waterlevel = 3;
		}
	}

}

void CMove::CheckDuck()
{
	pm.mins[0] = -16.0f;
	pm.mins[1] = -16.0f;

	pm.maxs[0] = 16.0f;
	pm.maxs[1] = 16.0f;

	pm.mins[2] = -24.0f;
	pm.maxs[2] = 32.0f;

	pm.viewheight = 22.0f;
}


int CMove::Pmove( vec3_t origin , vec3_t velocity , vec3_t angles , int checkContents )
{
	int      flag;

	checkContents = false;

	
	pm.numtouch = 0;
	VectorSet( pm.viewangles , angles[0] , angles[1] , 0.0f );
	pm.viewheight = 0;
	pm.watertype = 0;
	pm.waterlevel = 0;

	
	memset( &pml , 0 , sizeof( pml ) );

	
	VectorCopy( pml.origin , origin );
	VectorCopy( pml.velocity , velocity );
	
	
	VectorCopy( pml.previous_origin , origin );

	pml.frametime = m_timer->GetElapsedTime();

	
	

	
	

	flag = StepSlideMove( checkContents );

	if( flag & GTH_CONTENT_WATER_WALK_STEP )
	{
		g_chatManager->SetDFontColor( 0xffffffff );
		g_chatManager->DPrint( 256, 380, "WATER_STEP_WALK  depth : %f" , pml.waterDepth );
	}
	else if( flag & GTH_CONTENT_WATER_SWIM_STEP )
	{
		g_chatManager->SetDFontColor( 0xffffffff );
		g_chatManager->DPrint( 256, 380, "WATER_STEP_SWIM" );
	}
	else if( flag & GTH_CONTENT_LAVA )
	{
		g_chatManager->SetDFontColor( 0xffffffff );
		g_chatManager->DPrint( 256, 380, "LAVA_STEP_DIE" );
	}

	VectorCopy( origin , pml.origin );

	return flag;
}


int CMove::StepSlideMove( int checkContents )
{
	vec3_t   start_o, start_v;
	trace_t  trace;
	vec3_t   down , forward;
	int      flag;

	VectorCopy( start_o , pml.origin );
	VectorCopy( start_v , pml.velocity );

	
	start_o[2] += PLAYER_EYE_HEIGHT;
	start_v[2] = 0.001f;
	
	VectorAdd( forward , start_o , start_v );
	trace = m_world->Trace( start_o , pm.mins , pm.maxs , forward , MASK_PLAYERSOLID );

	if( trace.plane && trace.plane->normal[2] < 0.7f )
	{
		flag = COLLIDE_STARTSOLID;
		return flag;
	}

	if( trace.fraction < 0.2f )
	{
		flag = COLLIDE_STARTSOLID;
		return flag;
	}
	else if( trace.fraction < 1.0f )
	{
		VectorCopy( start_o , trace.endpos );
		flag = COLLIDE_FORWARD;
	}
	else
	{
		VectorCopy( start_o , trace.endpos );
		flag = COLLIDE_NONE;
	}

	VectorCopy( down , start_o );
	
	down[2] -= 2000.0f;
	start_o[2] += 50.0f;
	trace = m_world->Trace( start_o , pm.mins , pm.maxs , down , 0xffffffff );
	if( checkContents )
	{
		if( trace.contents & CONTENTS_WATER )
		{
			down[2]    = trace.endpos[2] - 64.0f;  
			start_o[2] = trace.endpos[2];
			trace = m_world->Trace( start_o , pm.mins , pm.maxs , down , MASK_PLAYERSOLID );
			if( trace.fraction != 1.0f )
			{
				pml.origin[0] = start_o[0];
				pml.origin[1] = start_o[1];
				pml.origin[2] = trace.endpos[2] + pm.mins[2];
				flag |= GTH_CONTENT_WATER_WALK_STEP;
				pml.waterDepth = 64.0f * trace.fraction;
			}
			else
			{
				pml.origin[0] = start_o[0];
				pml.origin[1] = start_o[1];
				pml.origin[2] = start_o[2] + pm.mins[2];
				flag |= GTH_CONTENT_WATER_SWIM_STEP;
			}
		}
		else if( trace.contents & CONTENTS_LAVA )
		{
			pml.origin[0] = start_o[0];
			pml.origin[1] = start_o[1];
			pml.origin[2] = trace.endpos[2] + pm.mins[2];
			flag |= GTH_CONTENT_LAVA;
		}
		else if( trace.fraction > 0.0f )
		{
			pml.origin[0] = start_o[0];
			pml.origin[1] = start_o[1];
			pml.origin[2] = trace.endpos[2] + pm.mins[2];
		}
	}
	else if( trace.fraction > 0.0f )
	{
		pml.origin[0] = start_o[0];
		pml.origin[1] = start_o[1];
		pml.origin[2] = trace.endpos[2] + pm.mins[2];
	}

	return flag;
}


float CMove::GetMoveSpeed( movetype_t type )
{
	float speed;

	switch( type )
	{
	case MOVE_TYPE_WALK :   
		       speed = 150.0f;
			   break;

	case MOVE_TYPE_JOG :   
		       speed = 200.0f;
			   break;

	case MOVE_TYPE_RUN :   
		       speed = 300.0f;
			   break;	         
	}

	return speed;
}

float CMove::GetHeight( vec3_t pos )
{
	vec3_t start , down;
	float height;
	trace_t trace;

	VectorCopy( start , pos );
	VectorCopy( down  , pos );
	
	height = pos[2];
	down[2] -= 2000.0f;
	start[2] += 100.0f; 
	trace = m_world->Trace( start , pm.mins , pm.maxs , down , MASK_PLAYERSOLID );
	if( trace.fraction > 0.0f )
		height = trace.endpos[2] + pm.mins[2];
	
	return height;
}

int CMove::GetHeight( vec3_t pos , int checkContents )
{
	vec3_t start , down;
	trace_t trace;
	int      flag;

	VectorCopy( start , pos );
	VectorCopy( down  , pos );
	
	flag = 0;
	down[2] -= 2000.0f;
	start[2] += 50.0f; 
	trace = m_world->Trace( start , pm.mins , pm.maxs , down , 0xffffffff );
	if( checkContents )
	{
		if( trace.contents & CONTENTS_WATER )
		{
			down[2]  = trace.endpos[2] - 64.0f;  
			start[2] = trace.endpos[2];
			trace = m_world->Trace( start , pm.mins , pm.maxs , down , MASK_PLAYERSOLID );
			if( trace.fraction != 1.0f )
			{
				pos[2] = trace.endpos[2] + pm.mins[2];
				flag = GTH_CONTENT_WATER_WALK_STEP;
				pml.waterDepth = 64.0f * trace.fraction;
			}
			else
			{
				pos[2] = start[2] + pm.mins[2];
				flag = GTH_CONTENT_WATER_SWIM_STEP;
			}
		}
		else if( trace.contents & CONTENTS_LAVA )
		{
			pos[2] = trace.endpos[2] + pm.mins[2];
			flag = GTH_CONTENT_LAVA;
		}
		else if( trace.fraction > 0.0f )
		{
			pos[2] = trace.endpos[2] + pm.mins[2];
		}
	}
	else if( trace.fraction > 0.0f )
	{
		pos[2] = trace.endpos[2] + pm.mins[2];
	}
	
	return flag;
}

int CMove::IsClick( vec3_t origin ,Fx_CHARACTER_t *character , int checkContents )
{
	vec3_t position, delta, dir;
	float speed;
	int flag=COLLIDE_NONE;

	switch ( g_pApp->m_myCharacter->event )
	{
	case GTH_EV_CHAR_WALK : 	
	case GTH_EV_CHAR_MOUSEWALK :
	case GTH_EV_CHAR_WALKRIGHT :
	case GTH_EV_CHAR_WALKLEFT :
		speed = SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_BACK :
	case GTH_EV_CHAR_BACKRIGHT :
	case GTH_EV_CHAR_BACKLEFT :
		speed = -1 * BACK_SCALE * SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_RUN :
	case GTH_EV_CHAR_MOUSERUN :
	case GTH_EV_CHAR_RUNLEFT :
	case GTH_EV_CHAR_RUNRIGHT :
		speed = RUN_SCALE * SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;


	case GTH_EV_CHAR_MOVERIGHT :
		speed = SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_MOVELEFT :
		speed = -1 * SPEED_CONSTANT * g_pApp->m_myCharacter->moveSpeed * g_timer.GetElapsedTime();
		break;

	case GTH_EV_CHAR_ATTACK :
		speed = 50;	
		break;

	default:
		speed = SPEED_CONSTANT * character->moveSpeed * g_timer.GetElapsedTime();
		break;
	}

	if (g_pApp->m_myCharacter->event == GTH_EV_CHAR_MOUSEWALK || g_pApp->m_myCharacter->event == GTH_EV_CHAR_MOUSERUN)
	{
		VectorClear( dir );
		dir[YAW] = g_pApp->m_myCharacter->mouseAngle;
		AngleToVector( dir , delta );
	}
	else
	{
		g_pApp->m_myCharacter->SetupVectors();
		g_pApp->m_myCharacter->GetFwdVector( delta );
		delta[2] = 0;
		VectorNormalize( delta );
	}

	VectorScale( delta , delta , speed );
	VectorCopy( position , origin );

	
	pm.numtouch = 0;
	VectorSet( pm.viewangles ,  g_pApp->m_myCharacter->angles[0] ,  g_pApp->m_myCharacter->angles[1] , 0.0f );
	pm.viewheight = 0;
	pm.watertype = 0;
	pm.waterlevel = 0;

	
	memset( &pml , 0 , sizeof( pml ) );

	
	VectorCopy( pml.origin , position );
	VectorCopy( pml.velocity , delta );
	
	
	VectorCopy( pml.previous_origin , position );

	pml.frametime = m_timer->GetElapsedTime();

	vec3_t   start_o, start_v;
	trace_t  trace;
	vec3_t  forward;

	VectorCopy( start_o , pml.origin );
	VectorCopy( start_v , pml.velocity );

	start_o[2] += PLAYER_EYE_HEIGHT;
	start_v[2] = 0.001f;
	
	VectorAdd( forward , start_o , start_v );
	trace = m_world->Trace( start_o , pm.mins , pm.maxs , forward , MASK_PLAYERSOLID );

	if(trace.allsolid && trace.startsolid )
	{
		flag = COLLIDE_STARTSOLID;
		return flag;
	}
	return flag;
}