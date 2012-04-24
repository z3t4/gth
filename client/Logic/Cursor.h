



#if !defined(AFX_CURSOR_H__67ED35DD_7BEB_4B61_B508_A0AB38237FB0__INCLUDED_)
#define AFX_CURSOR_H__67ED35DD_7BEB_4B61_B508_A0AB38237FB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

typedef struct
{
	int     scrX;
	int     scrY;
	vec3_t  nearPoint;
	vec3_t  farPoint;
	int     touched;
} cursorpos_t;

#define MAX_SIZE_ITEM_NAME 32

typedef struct
{
	char    itemName[ MAX_SIZE_ITEM_NAME ];
	vec3_t  origin;
	vec3_t  angles;
	int     idxEntity;
} pointerMDL_t;

class CCursor  
{
public:

	matrix4x4_t   m_invTransformMat;
	
	viewport_t    m_viewport;

	cursorpos_t   m_cursorPos;
	float         m_touchableDist;

	CBSP        *m_world;   
	CEffectMng  *m_effectMng; 
	
	pointerMDL_t m_pointer;
	vec3_t       m_targetPos;
	vec3_t       m_startPos;
	vec3_t       m_targetDir;
	int          m_targetedMove;
	int          m_whichDir;
	int          m_adjustedAngle;
	float        m_farSquareDist;

	CCursor();
	virtual ~CCursor();

	void  SetWorldModel( CBSP *world )            { m_world = world; }
	void  SetEffectMng( CEffectMng *effectMng )   { m_effectMng = effectMng; }

	void  SetScreenPos( int x , int y )  { m_cursorPos.scrX = x; m_cursorPos.scrY = y; }
	int   IsTouched()                    { return m_cursorPos.touched; }
	void  GetTouchedPos( vec3_t pos )    { VectorCopy( pos , m_cursorPos.farPoint ); }
	void  GetTargetPos( vec3_t pos )     { VectorCopy( pos , m_targetPos ); }
	int   IsPlayerTargetedMove()         { return m_targetedMove; }
	int   IsAdjustedPlayerAngle()        { return m_adjustedAngle; }
	void  SetPlayerAngleAdjusted()       { m_adjustedAngle = true; }
	float GetFarSquareDistance()         { return m_farSquareDist; }

	void  QuitPlayerTargetedMove();
	void  SetPlayerTargetedMove();

	void  SetupViewPort( int x , int y , int width , int height , float minZ , float maxZ );
	void  SetupMatrix( matrix4x4_t *inverse );
	void  ScreenToSpace();

	int   Initialize();
	void  SetPointerTargetPos( vec3_t target , vec3_t start );
	int   IsOverPointerTargetPos( vec3_t pos );

};

#endif 
