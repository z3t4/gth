



#include "../global_def.h"






CCursor::CCursor()
{
	memset( &m_cursorPos , 0 , sizeof( cursorpos_t ) );
	m_touchableDist = 4000.0f;
	m_targetedMove = false;
}

CCursor::~CCursor()
{

}

void CCursor::SetupViewPort( int x , int y , int width , int height , float minZ , float maxZ )
{
	m_viewport.X = x;
	m_viewport.Y = y;
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MinZ = minZ;
	m_viewport.MaxZ = maxZ;
}

void CCursor::SetupMatrix( matrix4x4_t *inverse )
{
	for( int i = 0 ; i < 16 ; i++ )
		m_invTransformMat.m[i] = inverse->m[i];
}


void CCursor::ScreenToSpace()
{
	vec3_t nearPoint , farPoint;
	vec3_t mins , maxs;
	trace_t trace; 
	vec3_t dist;
	float halfWidth , halfHeight;
	float projectedX , projectedY;
	int point;

	halfWidth  = (float) m_viewport.Width / 2.0f;
	halfHeight = (float) m_viewport.Height / 2.0f;
	projectedX = ( (float) m_cursorPos.scrX - halfWidth ) / halfWidth;
	projectedY = ( (float) m_cursorPos.scrY - halfHeight ) / halfHeight;

	nearPoint[0] = projectedX;
	nearPoint[1] = -projectedY;
	nearPoint[2] = 0.0f;
	
	farPoint[0] = projectedX;
	farPoint[1] = -projectedY;
	farPoint[2] = 1.0f;

	

	
	
	m_invTransformMat.Transform( m_cursorPos.nearPoint , nearPoint );
	m_invTransformMat.Transform( m_cursorPos.farPoint  , farPoint );
	
	VectorCopy( nearPoint , m_cursorPos.nearPoint );
	VectorCopy( farPoint  , m_cursorPos.farPoint );

	VectorSubtract( dist , farPoint , nearPoint );
	VectorNormalize( dist );
	VectorMA( farPoint , nearPoint , dist , m_touchableDist );

	mins[0] = -10.0f;  mins[1] = -10.0f;  mins[2] = -10.0f;
	maxs[0] =  10.0f;  maxs[1] =  10.0f;  maxs[2] =  10.0f;
	trace = m_world->Trace( nearPoint , mins , maxs , farPoint , MASK_PLAYERSOLID );
	
	if( trace.fraction == 1.0f )
	{
		VectorCopy( m_cursorPos.farPoint , farPoint );
		m_cursorPos.touched = false;
	}
	else
	{
		VectorCopy( m_cursorPos.farPoint , trace.endpos );
		m_cursorPos.touched = true;
	}

	point = (int) m_cursorPos.farPoint[0];
	m_cursorPos.farPoint[0] = (float) point;
	point = (int) m_cursorPos.farPoint[1];
	m_cursorPos.farPoint[1] = (float) point;
	point = (int) m_cursorPos.farPoint[2];
	m_cursorPos.farPoint[2] = (float) point; 

	VectorSubtract( dist , m_cursorPos.farPoint , m_cursorPos.nearPoint );
	m_farSquareDist = VectorDotProduct( dist , dist );
}


int CCursor::Initialize()
{
	strcpy( m_pointer.itemName , "pointer" );
	VectorClear( m_pointer.origin );
	VectorClear( m_pointer.angles );
	m_pointer.idxEntity = -1;

	return true;
}

void CCursor::QuitPlayerTargetedMove()       
{ 
	if ( m_pointer.idxEntity >= 0 )
		m_effectMng ->Delete( m_pointer.idxEntity );
	m_pointer.idxEntity = -1;
	m_targetedMove = false;
}

void CCursor::SetPlayerTargetedMove()
{
	if( m_pointer.idxEntity >= 0 )
		m_effectMng->Delete( m_pointer.idxEntity );

	VectorCopy( m_pointer.origin , m_targetPos );
	m_pointer.idxEntity = m_effectMng->Add( GTH_EFFECT_CLASS_POINTER , 0 , m_pointer.origin );
	
	if( m_pointer.idxEntity < 0 )
		return;

	m_targetedMove = true; 
	m_adjustedAngle = false; 
}


void CCursor::SetPointerTargetPos( vec3_t target , vec3_t start )
{ 
	float max;

	VectorCopy( m_targetPos , target );
	VectorCopy( m_startPos , start );
	VectorSubtract( m_targetDir , m_targetPos , m_startPos );
	VectorNormalize( m_targetDir );
	
	max = fabs( m_targetDir[0] );
	m_whichDir = 0;
	if( fabs( m_targetDir[1] ) > max )
		m_whichDir = 1;
}

int CCursor::IsOverPointerTargetPos( vec3_t pos )
{
	if( m_targetDir[ m_whichDir ] > 0 )
	{
		if( pos[ m_whichDir ] - m_targetPos[ m_whichDir ] >= 0.0f )
			return true;
	}
	else
	{
		if( pos[ m_whichDir ] - m_targetPos[ m_whichDir ] <= 0.0f )
			return true;
	}

	return false;
}