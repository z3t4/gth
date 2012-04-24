#include "../global_def.h"

CInterfacePhysics::CInterfacePhysics()
{
	for( int i = 0; i < MAX_INTERFACE_PHYSICS_INDEX; i ++ )
	{
		memset( &m_value[i], 0, sizeof( interfacePhysics_t ) );
		memset( &m_value2D[i], 0, sizeof( interfacePhysics2D_t ) );
	}
}

CInterfacePhysics::~CInterfacePhysics()
{
}

void CInterfacePhysics::SetInterfacePhysics( int idx, float sp, float ep, float vs, float ac, float st, int ncol )
{
	memset( &m_value[idx], 0, sizeof( interfacePhysics_t ) );

	interfacePhysics_t	*pValue = &m_value[idx];

	pValue->startPosition	= sp;
	pValue->currentPosition	= sp;

	pValue->endPosition		= ep;

	pValue->startVelocity	= vs;
	pValue->acceleration	= ac;

	pValue->startMilliTime	= st;
	pValue->currentMilliTime = st;

	pValue->restitution		= 0.3f;
	pValue->numCollision	= 0;
	pValue->maxCollision	= ncol;

	pValue->moving			= true;
}

void CInterfacePhysics::SetInterfacePhysics2D( int idx, float spx, float spy, float epx, float epy, float vs, float ac, float st, int ncol )
{
	float tanValue;

	memset( &m_value2D[idx], 0, sizeof( interfacePhysics2D_t ) );

	interfacePhysics2D_t	*pValue = &m_value2D[idx];

	pValue->startPositionX		= spx;
	pValue->currentPositionX	= spx;

	pValue->startPositionY		= spy;
	pValue->currentPositionY	= spy;

	pValue->endPositionX		= epx;
	pValue->endPositionY		= epy;

	pValue->startVelocity	= vs;
	pValue->acceleration	= ac;

	pValue->startMilliTime	= st;
	pValue->currentMilliTime = st;

	pValue->restitution		= 0.3f;
	pValue->numCollision	= 0;
	pValue->maxCollision	= ncol;

	pValue->moving			= true;

	tanValue = ( epy - spy ) / ( epx - spx );

	
	pValue->angle = atanf( tanValue );
}

void CInterfacePhysics::SetRestitution( int idx, float res )
{
	interfacePhysics_t	*pValue = &m_value[idx];
	pValue->restitution			= res;
}

void CInterfacePhysics::SetRestitution2D( int idx, float res )
{
	interfacePhysics2D_t	*pValue = &m_value2D[idx];
	pValue->restitution			= res;
}


int CInterfacePhysics::GetCurrentPosition( int idx, float& pos )
{
	interfacePhysics_t	*pValue = &m_value[idx];
	int ret = 1;

	pos = pValue->currentPosition;

	
	if( !pValue->moving ) return( 0 );
	
	int	direction;
	float curTime = g_timer.GetAppMilliTime();
	float dt = curTime - pValue->currentMilliTime;
	
	
	pValue->currentVelocity += pValue->acceleration * dt;

	pValue->currentPosition += pValue->currentVelocity * dt;
	
	if( pValue->acceleration > 0.0f ) direction = 1;
	else direction = 0;
	
	

	
	if( ( pValue->currentPosition > pValue->endPosition && direction == 1  ) )
	{
		pValue->currentPosition = pValue->endPosition;
		pValue->currentVelocity = -pValue->currentVelocity * pValue->restitution;
		pValue->numCollision ++;
		ret = 2;
		if( pValue->numCollision > pValue->maxCollision ) 
		{
			pValue->currentPosition = pValue->endPosition;
			pValue->moving = 0;
			ret = 0;
		}
	}
	
	if(	( pValue->currentPosition < pValue->endPosition && direction == 0  ) )
	{
		pValue->currentPosition = pValue->endPosition;
		pValue->currentVelocity = -pValue->currentVelocity * pValue->restitution;
		pValue->numCollision ++;
		ret = 2;
		if( pValue->numCollision > pValue->maxCollision ) 
		{
			pValue->currentPosition = pValue->endPosition;
			pValue->moving = 0;
			ret = 0;
		}
	}
	pos = pValue->currentPosition;
	pValue->currentMilliTime = curTime;
	return( ret );
}

int CInterfacePhysics::GetCurrentPosition2D( int idx, float& posx, float &posy )
{
	interfacePhysics2D_t	*pValue = &m_value2D[idx];
	float xv, yv, xa, ya;

	int ret = 1;

	posx = pValue->currentPositionX;
	posy = pValue->currentPositionY;

	
	if( !pValue->moving ) return( 0 );
	
	int	directionX, directionY;

	float curTime = g_timer.GetAppMilliTime();
	float dt = curTime - pValue->currentMilliTime;
	
	

	pValue->currentVelocity += pValue->acceleration * dt;

	xv = pValue->currentVelocity * cosf( pValue->angle );
	yv = pValue->currentVelocity * sinf( pValue->angle );

	xa = pValue->acceleration * cosf( pValue->angle );
	ya = pValue->acceleration * sinf( pValue->angle );

	pValue->currentPositionX += xv * dt;
	pValue->currentPositionY += yv * dt;
	
	if( xa > 0.0f ) directionX = 1;
	else directionX = 0;
	if( ya > 0.0f ) directionY = 1;
	else directionY = 0;
	
	

	
	if( ( pValue->currentPositionX > pValue->endPositionX && directionX == 1  ) )
	{
		if( ( pValue->currentPositionY > pValue->endPositionY && directionY == 1  ) || 
			( pValue->currentPositionY < pValue->endPositionY && directionY == 0  ) )
		{
			
			pValue->currentPositionX = pValue->endPositionX;
			pValue->currentPositionY = pValue->endPositionY;
			
			
			pValue->currentVelocity = -pValue->currentVelocity * pValue->restitution;
			pValue->numCollision ++;
			ret = 2;
			if( pValue->numCollision > pValue->maxCollision ) 
			{
				pValue->currentPositionX = pValue->endPositionX;
				pValue->currentPositionY = pValue->endPositionY;
				pValue->moving = 0;
				ret = 0;
			}
		}
	}
	
	if(	( pValue->currentPositionX < pValue->endPositionX && directionX == 0  ) )
	{
		if( ( pValue->currentPositionY > pValue->endPositionY && directionY == 1  ) || 
			( pValue->currentPositionY < pValue->endPositionY && directionY == 0  ) )
			
		{
			pValue->currentPositionX = pValue->endPositionX;
			pValue->currentPositionY = pValue->endPositionY;
			
			pValue->currentVelocity = -pValue->currentVelocity * pValue->restitution;
			pValue->numCollision ++;
			ret = 2;
			if( pValue->numCollision > pValue->maxCollision ) 
			{
				pValue->currentPositionX = pValue->endPositionX;
				pValue->currentPositionY = pValue->endPositionY;
				
				pValue->moving = 0;
				ret = 0;
			}
		}
	}
	posx = pValue->currentPositionX;
	posy = pValue->currentPositionY;

	pValue->currentMilliTime = curTime;
	return( ret );
}


int CInterfacePhysics::GetCurrentPosition( int idx, float& pos, float elTime )
{
	interfacePhysics_t	*pValue = &m_value[idx];
	int ret = 1;

	pos = pValue->currentPosition;

	
	if( !pValue->moving ) return( 0 );
	
	int	direction;
	float curTime = elTime;
	float dt = curTime - pValue->currentMilliTime;
	
	
	pValue->currentVelocity += pValue->acceleration * dt;

	pValue->currentPosition += pValue->currentVelocity * dt;
	
	if( pValue->acceleration > 0.0f ) direction = 1;
	else direction = 0;
	
	

	
	if( ( pValue->currentPosition > pValue->endPosition && direction == 1  ) )
	{
		pValue->currentPosition = pValue->endPosition;
		pValue->currentVelocity = -pValue->currentVelocity * pValue->restitution;
		pValue->numCollision ++;
		ret = 2;
		if( pValue->numCollision > pValue->maxCollision ) 
		{
			pValue->currentPosition = pValue->endPosition;
			pValue->moving = 0;
			ret = 0;
		}
	}
	
	if(	( pValue->currentPosition < pValue->endPosition && direction == 0  ) )
	{
		pValue->currentPosition = pValue->endPosition;
		pValue->currentVelocity = -pValue->currentVelocity * pValue->restitution;
		pValue->numCollision ++;
		ret = 2;
		if( pValue->numCollision > pValue->maxCollision ) 
		{
			pValue->currentPosition = pValue->endPosition;
			pValue->moving = 0;
			ret = 0;
		}
	}
	pos = pValue->currentPosition;
	pValue->currentMilliTime = curTime;
	return( ret );
}
