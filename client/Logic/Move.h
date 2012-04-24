



#if !defined(AFX_MOVE_H__F84BF467_BA23_41C8_B137_06BDE4DFDDA8__INCLUDED_)
#define AFX_MOVE_H__F84BF467_BA23_41C8_B137_06BDE4DFDDA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

typedef enum
{
	MOVE_TYPE_WALK = 0,
	MOVE_TYPE_JOG ,
	MOVE_TYPE_RUN
} movetype_t;

typedef enum
{
	COLLIDE_STARTSOLID = 0 ,
	COLLIDE_FORWARD ,
	COLLIDE_NONE
} COLLIDETYPE;

#define COLLIDE_NONE                0x00000000
#define COLLIDE_STARTSOLID          0x00000001
#define COLLIDE_FORWARD             0x00000002

#define GTH_CONTENT_LAVA            0x00010000
#define GTH_CONTENT_WATER_WALK_STEP 0x00020000
#define GTH_CONTENT_WATER_SWIM_STEP 0x00040000

class CMove  
{
public:
	CBSP   *m_world;   
	CTimer *m_timer;   

	CMove();
	virtual ~CMove();

	void SetWorldModel( CBSP *world ) { m_world = world; }
	void SetTimer( CTimer *timer )    { m_timer = timer; }

	void   ClipVelocity( vec3_t in , vec3_t normal , vec3_t out , float overbounce );
	void   Friction();
	void   CatagorizePosition();
	void   CheckDuck();
	void   ClampAngles();
	int    Pmove( vec3_t origin , vec3_t velocity , vec3_t angles , int checkContents = false );
	int    StepSlideMove( int checkContents );
	float  GetMoveSpeed( movetype_t type );
	float  GetHeight( vec3_t pos );
	int    GetHeight( vec3_t pos , int checkContents );
	int	   IsClick( vec3_t origin ,Fx_CHARACTER_t *character , int checkContents );

};

#endif 
