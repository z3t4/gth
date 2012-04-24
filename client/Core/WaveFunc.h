

#ifndef _WAVEFUNC_H
#define _WAVEFUNC_H

typedef enum 
{
	WAVE_NONE           = 0,
    WAVE_SIN            = 1,
	WAVE_TRIANGLE       = 2,
	WAVE_SQUARE         = 3,
	WAVE_SAWTOOTH       = 4,
	WAVE_INVSAWTOOTH    = 5,
	WAVE_NOISE          = 6
} wavetype_t;

typedef struct 
{
  
    wavetype_t type;
    float base;
    float amplitude;
    float phase;
    float frequency;

} wavefunc_t;

#endif