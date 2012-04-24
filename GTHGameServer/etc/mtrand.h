

#ifndef MTRAND_H
#define MTRAND_H

#ifdef _WIN32
#pragma once
#endif

#ifdef __cplusplus
extern "C"
{
#endif


void init_genrand(unsigned long s);









unsigned long genrand_int32(void);


long genrand_int31(void);


double genrand_real1(void);


double genrand_real2(void);


double genrand_real3(void);


double genrand_res53(void);



#ifdef __cplusplus
}
#endif

#endif 