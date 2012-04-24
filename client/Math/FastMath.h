#ifndef _FASTMATH_H
#define _FASTMATH_H


#define __PI		(3.14159265358979323846264338327950288f)
#define __PID		(3.14159265358979323846264338327950288419716939937510)
#define __TWOPI     (__PI*2.0f)
#define __TWOPID    (__PID*2.0)
#define __HALFPI    (__PI*0.5f)
#define __HALFPID   (__PID*0.5)
#define __DEG2RAD	(__PI/180.0f)
#define __EPSILON   (1e-10f)
#define __EPSILOND  (1e-12)

#define deg2rad(x)	((x)*__DEG2RAD)


__forceinline
__declspec(naked) 
float __fastcall
fast_cos(float angle)
{
	__asm {
		fld		DWORD PTR [esp+4]
		fcos
		ret 4
	}
}

__forceinline
__declspec(naked)
float __fastcall
fast_sin(float angle)
{
	__asm {
		fld		DWORD PTR [esp+4]
		fsin
		ret 4
	}
}

inline int fast_round(float f)
{
	int i;
	__asm fld   [f]
	__asm fistp [i]
	return i;
}







inline static float __fastcall
fast_rsqrt(float x)
{
    static const float _0_47 = 0.47f;
    static const float _1_47 = 1.47f;
    unsigned long y;
    _asm {
        mov     eax, 07F000000h+03F800000h 
        sub     eax, x
        sar     eax, 1

        mov     y, eax                      
        fld     _0_47                       
        fmul    DWORD PTR x                 

        fld     DWORD PTR y
        fld     st(0)                       
        fmul    st(0), st(1)                

        fld     _1_47                       
        fxch    st(3)                       
        fmulp   st(1), st(0)                
        fsubp   st(2), st(0)                
        fmulp   st(1), st(0)                
        fstp    y
        and     y, 07FFFFFFFh   
    }

    float r = *(float*) &y;
    
	r = (3.0f - x * (r * r)) * r * 0.5f;
    return r;
}



#endif