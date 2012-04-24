#ifndef _IFPHYSICS_GHR038EDFR_
#define _IFPHYSICS_GHR038EDFR_

#define		MAX_INTERFACE_PHYSICS_INDEX		5

typedef struct
{
	float			startMilliTime;
	float			startPosition;
	float			endPosition;
	float			acceleration;
	float			startVelocity;

	float			currentVelocity;
	float			currentPosition;
	float			currentMilliTime;

	float			restitution;

	int				maxCollision;
	int				numCollision;

	int				moving;
} interfacePhysics_t;

typedef struct
{
	float			startMilliTime;
	float			startPositionX;
	float			endPositionX;

	float			startPositionY;
	float			endPositionY;

	float			acceleration;

	float			startVelocity;

	float			currentVelocity;
	float			currentPositionX;
	float			currentPositionY;

	float			currentMilliTime;

	float			restitution;

	float			angle;

	int				maxCollision;
	int				numCollision;

	int				moving;
} interfacePhysics2D_t;


class CInterfacePhysics
{
public :

	CInterfacePhysics();
	~CInterfacePhysics();

	interfacePhysics_t			m_value[MAX_INTERFACE_PHYSICS_INDEX];
	interfacePhysics2D_t		m_value2D[MAX_INTERFACE_PHYSICS_INDEX];

	void		SetInterfacePhysics( int idx, float sp, float ep, float vs, float ac, float st, int ncol );
	
	void		SetInterfacePhysics2D( int idx, float spx, float spy, float epx, float epy, float vs, float ac, float st, int ncol );

	void		SetRestitution( int idx, float res );
	int			GetCurrentPosition( int idx, float& pos );
	int			GetDeltaPosition( int idx, float& pos, float dt );
	int			GetCurrentPosition( int idx, float& pos, float elTime );

	void		SetRestitution2D( int idx, float res );
	int			GetCurrentPosition2D( int idx, float& posx, float& posy );
	int			GetDeltaPosition2D( int idx, float& posx, float& posy, float dt );

};


#endif 