



#ifndef _FMOD_H_
#define _FMOD_H_





#if (!defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(_WIN32_WCE) && !defined(_XBOX)) || (defined(__GNUC__) && defined(WIN32))
    #ifndef __cdecl
        #define __cdecl
    #endif
    #ifndef __stdcall
        #define __stdcall
    #endif
#endif 

#if defined(_WIN32_WCE)
    #define F_API _cdecl
    #define F_CALLBACKAPI _cdecl
#else
    #define F_API __stdcall
    #define F_CALLBACKAPI __stdcall
#endif

#ifdef DLL_EXPORTS
    #define DLL_API __declspec(dllexport)
#else
    #if defined(__LCC__) || defined(__MINGW32__) || defined(__CYGWIN32__)
        #define DLL_API F_API
    #else
        #define DLL_API
    #endif 
#endif 

#define FMOD_VERSION    3.70f


typedef struct FSOUND_SAMPLE    FSOUND_SAMPLE;
typedef struct FSOUND_STREAM    FSOUND_STREAM;
typedef struct FSOUND_DSPUNIT   FSOUND_DSPUNIT;
typedef struct FSOUND_SYNCPOINT FSOUND_SYNCPOINT;
typedef struct FMUSIC_MODULE    FMUSIC_MODULE;


typedef void *      (F_CALLBACKAPI *FSOUND_DSPCALLBACK)     (void *originalbuffer, void *newbuffer, int length, int param);
typedef signed char (F_CALLBACKAPI *FSOUND_STREAMCALLBACK)  (FSOUND_STREAM *stream, void *buff, int len, int param);
typedef void        (F_CALLBACKAPI *FMUSIC_CALLBACK)        (FMUSIC_MODULE *mod, unsigned char param);

typedef unsigned int(F_CALLBACKAPI *FSOUND_OPENCALLBACK)    (const char *name);
typedef void        (F_CALLBACKAPI *FSOUND_CLOSECALLBACK)   (unsigned int handle);
typedef int         (F_CALLBACKAPI *FSOUND_READCALLBACK)    (void *buffer, int size, unsigned int handle);
typedef int         (F_CALLBACKAPI *FSOUND_SEEKCALLBACK)    (unsigned int handle, int pos, signed char mode);
typedef int         (F_CALLBACKAPI *FSOUND_TELLCALLBACK)    (unsigned int handle);

typedef void *      (F_CALLBACKAPI *FSOUND_ALLOCCALLBACK)   (unsigned int size);
typedef void *      (F_CALLBACKAPI *FSOUND_REALLOCCALLBACK) (void *ptr, unsigned int size);
typedef void        (F_CALLBACKAPI *FSOUND_FREECALLBACK)    (void *ptr);

typedef signed char (F_CALLBACKAPI *FSOUND_METADATACALLBACK)(char *name, char *value, int userdata);



enum FMOD_ERRORS 
{
    FMOD_ERR_NONE,             
    FMOD_ERR_BUSY,             
    FMOD_ERR_UNINITIALIZED,    
    FMOD_ERR_INIT,             
    FMOD_ERR_ALLOCATED,        
    FMOD_ERR_PLAY,             
    FMOD_ERR_OUTPUT_FORMAT,    
    FMOD_ERR_COOPERATIVELEVEL, 
    FMOD_ERR_CREATEBUFFER,     
    FMOD_ERR_FILE_NOTFOUND,    
    FMOD_ERR_FILE_FORMAT,      
    FMOD_ERR_FILE_BAD,         
    FMOD_ERR_MEMORY,           
    FMOD_ERR_VERSION,          
    FMOD_ERR_INVALID_PARAM,    
    FMOD_ERR_NO_EAX,           
    FMOD_ERR_CHANNEL_ALLOC,    
    FMOD_ERR_RECORD,           
    FMOD_ERR_MEDIAPLAYER,      
    FMOD_ERR_CDDEVICE          
};



enum FSOUND_OUTPUTTYPES
{
    FSOUND_OUTPUT_NOSOUND,    
    FSOUND_OUTPUT_WINMM,      
    FSOUND_OUTPUT_DSOUND,     
    FSOUND_OUTPUT_A3D,        

    FSOUND_OUTPUT_OSS,        
    FSOUND_OUTPUT_ESD,        
    FSOUND_OUTPUT_ALSA,       

    FSOUND_OUTPUT_ASIO,       
    FSOUND_OUTPUT_XBOX,       
    FSOUND_OUTPUT_PS2,        
    FSOUND_OUTPUT_MAC,        
    FSOUND_OUTPUT_GC,         

    FSOUND_OUTPUT_NOSOUND_NONREALTIME   
};



enum FSOUND_MIXERTYPES
{
    FSOUND_MIXER_AUTODETECT,        
    FSOUND_MIXER_BLENDMODE,         
    FSOUND_MIXER_MMXP5,             
    FSOUND_MIXER_MMXP6,             

    FSOUND_MIXER_QUALITY_AUTODETECT,
    FSOUND_MIXER_QUALITY_FPU,       
    FSOUND_MIXER_QUALITY_MMXP5,     
    FSOUND_MIXER_QUALITY_MMXP6,     

    FSOUND_MIXER_MONO,              
    FSOUND_MIXER_QUALITY_MONO,      

    FSOUND_MIXER_MAX
};



enum FMUSIC_TYPES
{
    FMUSIC_TYPE_NONE,       
    FMUSIC_TYPE_MOD,        
    FMUSIC_TYPE_S3M,        
    FMUSIC_TYPE_XM,         
    FMUSIC_TYPE_IT,         
    FMUSIC_TYPE_MIDI,       
    FMUSIC_TYPE_FSB         
};



#define FSOUND_DSP_DEFAULTPRIORITY_CLEARUNIT        0       
#define FSOUND_DSP_DEFAULTPRIORITY_SFXUNIT          100     
#define FSOUND_DSP_DEFAULTPRIORITY_MUSICUNIT        200     
#define FSOUND_DSP_DEFAULTPRIORITY_USER             300     
#define FSOUND_DSP_DEFAULTPRIORITY_FFTUNIT          900     
#define FSOUND_DSP_DEFAULTPRIORITY_CLIPANDCOPYUNIT  1000    




#define FSOUND_CAPS_HARDWARE                0x1     
#define FSOUND_CAPS_EAX2                    0x2     
#define FSOUND_CAPS_EAX3                    0x10    




#define FSOUND_LOOP_OFF      0x00000001  
#define FSOUND_LOOP_NORMAL   0x00000002  
#define FSOUND_LOOP_BIDI     0x00000004  
#define FSOUND_8BITS         0x00000008  
#define FSOUND_16BITS        0x00000010  
#define FSOUND_MONO          0x00000020  
#define FSOUND_STEREO        0x00000040  
#define FSOUND_UNSIGNED      0x00000080  
#define FSOUND_SIGNED        0x00000100  
#define FSOUND_DELTA         0x00000200  
#define FSOUND_IT214         0x00000400  
#define FSOUND_IT215         0x00000800  
#define FSOUND_HW3D          0x00001000  
#define FSOUND_2D            0x00002000  
#define FSOUND_STREAMABLE    0x00004000  
#define FSOUND_LOADMEMORY    0x00008000  
#define FSOUND_LOADRAW       0x00010000  
#define FSOUND_MPEGACCURATE  0x00020000  
#define FSOUND_FORCEMONO     0x00040000  
#define FSOUND_HW2D          0x00080000  
#define FSOUND_ENABLEFX      0x00100000  
#define FSOUND_MPEGHALFRATE  0x00200000  
#define FSOUND_IMAADPCM      0x00400000  
#define FSOUND_VAG           0x00800000  
#define FSOUND_NONBLOCKING   0x01000000  
#define FSOUND_GCADPCM       0x02000000  
#define FSOUND_MULTICHANNEL  0x04000000  
#define FSOUND_USECORE0      0x08000000  
#define FSOUND_USECORE1      0x10000000  
#define FSOUND_LOADMEMORYIOP 0x20000000  
#define FSOUND_STREAM_NET    0x80000000  

#define FSOUND_NORMAL       (FSOUND_16BITS | FSOUND_SIGNED | FSOUND_MONO)      





#define FSOUND_CD_PLAYCONTINUOUS    0   
#define FSOUND_CD_PLAYONCE          1   
#define FSOUND_CD_PLAYLOOPED        2   
#define FSOUND_CD_PLAYRANDOM        3   




#define FSOUND_FREE             -1      
#define FSOUND_UNMANAGED        -2      
#define FSOUND_ALL              -3      
#define FSOUND_STEREOPAN        -1      
#define FSOUND_SYSTEMCHANNEL    -1000   
#define FSOUND_SYSTEMSAMPLE     -1000   





typedef struct _FSOUND_REVERB_PROPERTIES 
{                                   
    unsigned int Environment;            
    float        EnvSize;                
    float        EnvDiffusion;           
    int          Room;                   
    int          RoomHF;                 
    int          RoomLF;                 
    float        DecayTime;              
    float        DecayHFRatio;           
    float        DecayLFRatio;           
    int          Reflections;            
    float        ReflectionsDelay;       
    float        ReflectionsPan[3];      
    int          Reverb;                 
    float        ReverbDelay;            
    float        ReverbPan[3];           
    float        EchoTime;               
    float        EchoDepth;              
    float        ModulationTime;         
    float        ModulationDepth;        
    float        AirAbsorptionHF;        
    float        HFReference;            
    float        LFReference;            
    float        RoomRolloffFactor;      
    float        Diffusion;              
    float        Density;                
    unsigned int Flags;                  
} FSOUND_REVERB_PROPERTIES;



#define FSOUND_REVERB_FLAGS_DECAYTIMESCALE        0x00000001 
#define FSOUND_REVERB_FLAGS_REFLECTIONSSCALE      0x00000002 
#define FSOUND_REVERB_FLAGS_REFLECTIONSDELAYSCALE 0x00000004 
#define FSOUND_REVERB_FLAGS_REVERBSCALE           0x00000008 
#define FSOUND_REVERB_FLAGS_REVERBDELAYSCALE      0x00000010 
#define FSOUND_REVERB_FLAGS_DECAYHFLIMIT          0x00000020 
#define FSOUND_REVERB_FLAGS_ECHOTIMESCALE         0x00000040 
#define FSOUND_REVERB_FLAGS_MODULATIONTIMESCALE   0x00000080 
#define FSOUND_REVERB_FLAGS_CORE0                 0x00000100 
#define FSOUND_REVERB_FLAGS_CORE1                 0x00000200 
#define FSOUND_REVERB_FLAGS_DEFAULT              (FSOUND_REVERB_FLAGS_DECAYTIMESCALE |        \
                                                  FSOUND_REVERB_FLAGS_REFLECTIONSSCALE |      \
                                                  FSOUND_REVERB_FLAGS_REFLECTIONSDELAYSCALE | \
                                                  FSOUND_REVERB_FLAGS_REVERBSCALE |           \
                                                  FSOUND_REVERB_FLAGS_REVERBDELAYSCALE |      \
                                                  FSOUND_REVERB_FLAGS_DECAYHFLIMIT |          \
                                                  FSOUND_REVERB_FLAGS_CORE0 |                 \
                                                  FSOUND_REVERB_FLAGS_CORE1 )







#define FSOUND_PRESET_OFF              {0,	7.5f,	1.00f, -10000, -10000, 0,   1.00f,  1.00f, 1.0f,  -2602, 0.007f, { 0.0f,0.0f,0.0f },   200, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,   0.0f,   0.0f, 0x33f }
#define FSOUND_PRESET_GENERIC          {0,	7.5f,	1.00f, -1000,  -100,   0,   1.49f,  0.83f, 1.0f,  -2602, 0.007f, { 0.0f,0.0f,0.0f },   200, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_PADDEDCELL       {1,	1.4f,	1.00f, -1000,  -6000,  0,   0.17f,  0.10f, 1.0f,  -1204, 0.001f, { 0.0f,0.0f,0.0f },   207, 0.002f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_ROOM 	           {2,	1.9f,	1.00f, -1000,  -454,   0,   0.40f,  0.83f, 1.0f,  -1646, 0.002f, { 0.0f,0.0f,0.0f },    53, 0.003f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_BATHROOM 	       {3,	1.4f,	1.00f, -1000,  -1200,  0,   1.49f,  0.54f, 1.0f,   -370, 0.007f, { 0.0f,0.0f,0.0f },  1030, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f,  60.0f, 0x3f }
#define FSOUND_PRESET_LIVINGROOM       {4,	2.5f,	1.00f, -1000,  -6000,  0,   0.50f,  0.10f, 1.0f,  -1376, 0.003f, { 0.0f,0.0f,0.0f }, -1104, 0.004f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_STONEROOM        {5,	11.6f,	1.00f, -1000,  -300,   0,   2.31f,  0.64f, 1.0f,   -711, 0.012f, { 0.0f,0.0f,0.0f },    83, 0.017f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_AUDITORIUM       {6,	21.6f,	1.00f, -1000,  -476,   0,   4.32f,  0.59f, 1.0f,   -789, 0.020f, { 0.0f,0.0f,0.0f },  -289, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CONCERTHALL      {7,	19.6f,	1.00f, -1000,  -500,   0,   3.92f,  0.70f, 1.0f,  -1230, 0.020f, { 0.0f,0.0f,0.0f },    -2, 0.029f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CAVE             {8,	14.6f,	1.00f, -1000,  0,      0,   2.91f,  1.30f, 1.0f,   -602, 0.015f, { 0.0f,0.0f,0.0f },  -302, 0.022f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_ARENA            {9,	36.2f,	1.00f, -1000,  -698,   0,   7.24f,  0.33f, 1.0f,  -1166, 0.020f, { 0.0f,0.0f,0.0f },    16, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_HANGAR           {10,	50.3f,	1.00f, -1000,  -1000,  0,   10.05f, 0.23f, 1.0f,   -602, 0.020f, { 0.0f,0.0f,0.0f },   198, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CARPETTEDHALLWAY {11,	1.9f,	1.00f, -1000,  -4000,  0,   0.30f,  0.10f, 1.0f,  -1831, 0.002f, { 0.0f,0.0f,0.0f }, -1630, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_HALLWAY          {12,	1.8f,	1.00f, -1000,  -300,   0,   1.49f,  0.59f, 1.0f,  -1219, 0.007f, { 0.0f,0.0f,0.0f },   441, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_STONECORRIDOR    {13,	13.5f,	1.00f, -1000,  -237,   0,   2.70f,  0.79f, 1.0f,  -1214, 0.013f, { 0.0f,0.0f,0.0f },   395, 0.020f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_ALLEY 	       {14,	7.5f,	0.30f, -1000,  -270,   0,   1.49f,  0.86f, 1.0f,  -1204, 0.007f, { 0.0f,0.0f,0.0f },    -4, 0.011f, { 0.0f,0.0f,0.0f }, 0.125f, 0.95f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_FOREST 	       {15,	38.0f,	0.30f, -1000,  -3300,  0,   1.49f,  0.54f, 1.0f,  -2560, 0.162f, { 0.0f,0.0f,0.0f },  -229, 0.088f, { 0.0f,0.0f,0.0f }, 0.125f, 1.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  79.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_CITY             {16,	7.5f,	0.50f, -1000,  -800,   0,   1.49f,  0.67f, 1.0f,  -2273, 0.007f, { 0.0f,0.0f,0.0f }, -1691, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  50.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_MOUNTAINS        {17,	100.0f, 0.27f, -1000,  -2500,  0,   1.49f,  0.21f, 1.0f,  -2780, 0.300f, { 0.0f,0.0f,0.0f }, -1434, 0.100f, { 0.0f,0.0f,0.0f }, 0.250f, 1.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  27.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_QUARRY           {18,	17.5f,	1.00f, -1000,  -1000,  0,   1.49f,  0.83f, 1.0f, -10000, 0.061f, { 0.0f,0.0f,0.0f },   500, 0.025f, { 0.0f,0.0f,0.0f }, 0.125f, 0.70f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_PLAIN            {19,	42.5f,	0.21f, -1000,  -2000,  0,   1.49f,  0.50f, 1.0f,  -2466, 0.179f, { 0.0f,0.0f,0.0f }, -1926, 0.100f, { 0.0f,0.0f,0.0f }, 0.250f, 1.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  21.0f, 100.0f, 0x3f }
#define FSOUND_PRESET_PARKINGLOT       {20,	8.3f,	1.00f, -1000,  0,      0,   1.65f,  1.50f, 1.0f,  -1363, 0.008f, { 0.0f,0.0f,0.0f }, -1153, 0.012f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_SEWERPIPE        {21,	1.7f,	0.80f, -1000,  -1000,  0,   2.81f,  0.14f, 1.0f,    429, 0.014f, { 0.0f,0.0f,0.0f },  1023, 0.021f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 0.000f, -5.0f, 5000.0f, 250.0f, 0.0f,  80.0f,  60.0f, 0x3f }
#define FSOUND_PRESET_UNDERWATER       {22,	1.8f,	1.00f, -1000,  -4000,  0,   1.49f,  0.10f, 1.0f,   -449, 0.007f, { 0.0f,0.0f,0.0f },  1700, 0.011f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 1.18f, 0.348f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x3f }



#define FSOUND_PRESET_DRUGGED          {23,	1.9f,	0.50f, -1000,  0,      0,   8.39f,  1.39f, 1.0f,  -115,  0.002f, { 0.0f,0.0f,0.0f },   985, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 0.25f, 1.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_DIZZY            {24,	1.8f,	0.60f, -1000,  -400,   0,   17.23f, 0.56f, 1.0f,  -1713, 0.020f, { 0.0f,0.0f,0.0f },  -613, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 1.00f, 0.81f, 0.310f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }
#define FSOUND_PRESET_PSYCHOTIC        {25,	1.0f,	0.50f, -1000,  -151,   0,   7.56f,  0.91f, 1.0f,  -626,  0.020f, { 0.0f,0.0f,0.0f },   774, 0.030f, { 0.0f,0.0f,0.0f }, 0.250f, 0.00f, 4.00f, 1.000f, -5.0f, 5000.0f, 250.0f, 0.0f, 100.0f, 100.0f, 0x1f }



#define FSOUND_PRESET_PS2_ROOM         {1,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_STUDIO_A     {2,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_STUDIO_B     {3,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_STUDIO_C     {4,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_HALL         {5,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_SPACE        {6,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_ECHO         {7,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_DELAY        {8,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }
#define FSOUND_PRESET_PS2_PIPE         {9,	0,	    0,         0,  0,      0,   0.0f,   0.0f,  0.0f,     0,  0.000f, { 0.0f,0.0f,0.0f },     0, 0.000f, { 0.0f,0.0f,0.0f }, 0.000f, 0.00f, 0.00f, 0.000f,  0.0f, 0000.0f,   0.0f, 0.0f,   0.0f,   0.0f, 0x31f }





typedef struct _FSOUND_REVERB_CHANNELPROPERTIES 
{                                   
    int    Direct;                              
    int    DirectHF;                            
    int    Room;                                
    int    RoomHF;                              
    int    Obstruction;                         
    float  ObstructionLFRatio;                  
    int    Occlusion;                           
    float  OcclusionLFRatio;                    
    float  OcclusionRoomRatio;                  
    float  OcclusionDirectRatio;                
    int    Exclusion;                           
    float  ExclusionLFRatio;                    
    int    OutsideVolumeHF;                     
    float  DopplerFactor;                       
    float  RolloffFactor;                       
    float  RoomRolloffFactor;                   
    float  AirAbsorptionFactor;                 
    int    Flags;                               
} FSOUND_REVERB_CHANNELPROPERTIES;



#define FSOUND_REVERB_CHANNELFLAGS_DIRECTHFAUTO  0x00000001 
#define FSOUND_REVERB_CHANNELFLAGS_ROOMAUTO      0x00000002 
#define FSOUND_REVERB_CHANNELFLAGS_ROOMHFAUTO    0x00000004 
#define FSOUND_REVERB_CHANNELFLAGS_DEFAULT       (FSOUND_REVERB_CHANNELFLAGS_DIRECTHFAUTO |   \
                                                  FSOUND_REVERB_CHANNELFLAGS_ROOMAUTO|        \
                                                  FSOUND_REVERB_CHANNELFLAGS_ROOMHFAUTO)




enum FSOUND_FX_MODES
{
    FSOUND_FX_CHORUS,
    FSOUND_FX_COMPRESSOR,
    FSOUND_FX_DISTORTION,
    FSOUND_FX_ECHO,
    FSOUND_FX_FLANGER,
    FSOUND_FX_GARGLE,
    FSOUND_FX_I3DL2REVERB,
    FSOUND_FX_PARAMEQ,
    FSOUND_FX_WAVES_REVERB,

    FSOUND_FX_MAX
};


enum FSOUND_SPEAKERMODES
{
    FSOUND_SPEAKERMODE_DOLBYDIGITAL,  
    FSOUND_SPEAKERMODE_HEADPHONES,    
    FSOUND_SPEAKERMODE_MONO,          
    FSOUND_SPEAKERMODE_QUAD,          
    FSOUND_SPEAKERMODE_STEREO,        
    FSOUND_SPEAKERMODE_SURROUND,      
    FSOUND_SPEAKERMODE_DTS,           
    FSOUND_SPEAKERMODE_PROLOGIC2      
};



#define FSOUND_INIT_USEDEFAULTMIDISYNTH     0x0001    
#define FSOUND_INIT_GLOBALFOCUS             0x0002    
#define FSOUND_INIT_ENABLEOUTPUTFX          0x0004    
#define FSOUND_INIT_ACCURATEVULEVELS        0x0008    
#define FSOUND_INIT_PS2_DISABLECORE0REVERB  0x0010    
#define FSOUND_INIT_PS2_DISABLECORE1REVERB  0x0020    
#define FSOUND_INIT_PS2_SWAPDMACORES        0x0040    
#define FSOUND_INIT_DONTLATENCYADJUST       0x0080    
#define FSOUND_INIT_GC_INITLIBS             0x0100    
#define FSOUND_INIT_STREAM_FROM_MAIN_THREAD 0x0200    




enum FSOUND_STREAM_NET_STATUS
{
    FSOUND_STREAM_NET_NOTCONNECTED = 0,     
    FSOUND_STREAM_NET_CONNECTING,           
    FSOUND_STREAM_NET_BUFFERING,            
    FSOUND_STREAM_NET_READY,                
    FSOUND_STREAM_NET_ERROR                 
};



enum FSOUND_TAGFIELD_TYPE
{
    FSOUND_TAGFIELD_VORBISCOMMENT = 0,      
    FSOUND_TAGFIELD_ID3V1,                  
    FSOUND_TAGFIELD_ID3V2,                  
    FSOUND_TAGFIELD_SHOUTCAST,              
    FSOUND_TAGFIELD_ICECAST,                
    FSOUND_TAGFIELD_ASF                     
};



#define FSOUND_PROTOCOL_SHOUTCAST   0x00000001
#define FSOUND_PROTOCOL_ICECAST     0x00000002
#define FSOUND_PROTOCOL_HTTP        0x00000004
#define FSOUND_FORMAT_MPEG          0x00010000
#define FSOUND_FORMAT_OGGVORBIS     0x00020000







#ifdef __cplusplus
extern "C" {
#endif







DLL_API signed char     F_API FSOUND_SetOutput(int outputtype);
DLL_API signed char     F_API FSOUND_SetDriver(int driver);
DLL_API signed char     F_API FSOUND_SetMixer(int mixer);
DLL_API signed char     F_API FSOUND_SetBufferSize(int len_ms);
DLL_API signed char     F_API FSOUND_SetHWND(void *hwnd);
DLL_API signed char     F_API FSOUND_SetMinHardwareChannels(int min);
DLL_API signed char     F_API FSOUND_SetMaxHardwareChannels(int max);
DLL_API signed char     F_API FSOUND_SetMemorySystem(void *pool, 
                                                     int poollen, 
                                                     FSOUND_ALLOCCALLBACK   useralloc,
                                                     FSOUND_REALLOCCALLBACK userrealloc,
                                                     FSOUND_FREECALLBACK    userfree);


DLL_API signed char     F_API FSOUND_Init(int mixrate, int maxsoftwarechannels, unsigned int flags);
DLL_API void            F_API FSOUND_Close();



DLL_API void            F_API FSOUND_Update();   

DLL_API void            F_API FSOUND_SetSpeakerMode(unsigned int speakermode);
DLL_API void            F_API FSOUND_SetSFXMasterVolume(int volume);
DLL_API void            F_API FSOUND_SetPanSeperation(float pansep);
DLL_API void            F_API FSOUND_File_SetCallbacks(FSOUND_OPENCALLBACK  useropen,
                                                       FSOUND_CLOSECALLBACK userclose,
                                                       FSOUND_READCALLBACK  userread,
                                                       FSOUND_SEEKCALLBACK  userseek,
                                                       FSOUND_TELLCALLBACK  usertell);



DLL_API int             F_API FSOUND_GetError();
DLL_API float           F_API FSOUND_GetVersion();
DLL_API int             F_API FSOUND_GetOutput();
DLL_API void *          F_API FSOUND_GetOutputHandle();
DLL_API int             F_API FSOUND_GetDriver();
DLL_API int             F_API FSOUND_GetMixer();
DLL_API int             F_API FSOUND_GetNumDrivers();
DLL_API const char *    F_API FSOUND_GetDriverName(int id);
DLL_API signed char     F_API FSOUND_GetDriverCaps(int id, unsigned int *caps);
DLL_API int             F_API FSOUND_GetOutputRate();
DLL_API int             F_API FSOUND_GetMaxChannels();
DLL_API int             F_API FSOUND_GetMaxSamples();
DLL_API int             F_API FSOUND_GetSFXMasterVolume();
DLL_API int             F_API FSOUND_GetNumHardwareChannels();
DLL_API int             F_API FSOUND_GetChannelsPlaying();
DLL_API float           F_API FSOUND_GetCPUUsage();
DLL_API void            F_API FSOUND_GetMemoryStats(unsigned int *currentalloced, unsigned int *maxalloced);







DLL_API FSOUND_SAMPLE * F_API FSOUND_Sample_Load(int index, const char *name_or_data, unsigned int mode, int offset, int length);
DLL_API FSOUND_SAMPLE * F_API FSOUND_Sample_Alloc(int index, int length, unsigned int mode, int deffreq, int defvol, int defpan, int defpri);
DLL_API void            F_API FSOUND_Sample_Free(FSOUND_SAMPLE *sptr);
DLL_API signed char     F_API FSOUND_Sample_Upload(FSOUND_SAMPLE *sptr, void *srcdata, unsigned int mode);
DLL_API signed char     F_API FSOUND_Sample_Lock(FSOUND_SAMPLE *sptr, int offset, int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2);
DLL_API signed char     F_API FSOUND_Sample_Unlock(FSOUND_SAMPLE *sptr, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2);



DLL_API signed char     F_API FSOUND_Sample_SetMode(FSOUND_SAMPLE *sptr, unsigned int mode);
DLL_API signed char     F_API FSOUND_Sample_SetLoopPoints(FSOUND_SAMPLE *sptr, int loopstart, int loopend);
DLL_API signed char     F_API FSOUND_Sample_SetDefaults(FSOUND_SAMPLE *sptr, int deffreq, int defvol, int defpan, int defpri);
DLL_API signed char     F_API FSOUND_Sample_SetMinMaxDistance(FSOUND_SAMPLE *sptr, float min, float max);
DLL_API signed char     F_API FSOUND_Sample_SetMaxPlaybacks(FSOUND_SAMPLE *sptr, int max);



DLL_API FSOUND_SAMPLE * F_API FSOUND_Sample_Get(int sampno);
DLL_API const char *    F_API FSOUND_Sample_GetName(FSOUND_SAMPLE *sptr);
DLL_API unsigned int    F_API FSOUND_Sample_GetLength(FSOUND_SAMPLE *sptr);
DLL_API signed char     F_API FSOUND_Sample_GetLoopPoints(FSOUND_SAMPLE *sptr, int *loopstart, int *loopend);
DLL_API signed char     F_API FSOUND_Sample_GetDefaults(FSOUND_SAMPLE *sptr, int *deffreq, int *defvol, int *defpan, int *defpri);
DLL_API unsigned int    F_API FSOUND_Sample_GetMode(FSOUND_SAMPLE *sptr);
  






DLL_API int             F_API FSOUND_PlaySound(int channel, FSOUND_SAMPLE *sptr);
DLL_API int             F_API FSOUND_PlaySoundEx(int channel, FSOUND_SAMPLE *sptr, FSOUND_DSPUNIT *dsp, signed char startpaused);
DLL_API signed char     F_API FSOUND_StopSound(int channel);



DLL_API signed char     F_API FSOUND_SetFrequency(int channel, int freq);
DLL_API signed char     F_API FSOUND_SetVolume(int channel, int vol);
DLL_API signed char     F_API FSOUND_SetVolumeAbsolute(int channel, int vol);
DLL_API signed char     F_API FSOUND_SetPan(int channel, int pan);
DLL_API signed char     F_API FSOUND_SetSurround(int channel, signed char surround);
DLL_API signed char     F_API FSOUND_SetMute(int channel, signed char mute);
DLL_API signed char     F_API FSOUND_SetPriority(int channel, int priority);
DLL_API signed char     F_API FSOUND_SetReserved(int channel, signed char reserved);
DLL_API signed char     F_API FSOUND_SetPaused(int channel, signed char paused);
DLL_API signed char     F_API FSOUND_SetLoopMode(int channel, unsigned int loopmode);
DLL_API signed char     F_API FSOUND_SetCurrentPosition(int channel, unsigned int offset);



DLL_API signed char     F_API FSOUND_IsPlaying(int channel);
DLL_API int             F_API FSOUND_GetFrequency(int channel);
DLL_API int             F_API FSOUND_GetVolume(int channel);
DLL_API int             F_API FSOUND_GetPan(int channel);
DLL_API signed char     F_API FSOUND_GetSurround(int channel);
DLL_API signed char     F_API FSOUND_GetMute(int channel);
DLL_API int             F_API FSOUND_GetPriority(int channel);
DLL_API signed char     F_API FSOUND_GetReserved(int channel);
DLL_API signed char     F_API FSOUND_GetPaused(int channel);
DLL_API unsigned int    F_API FSOUND_GetLoopMode(int channel);
DLL_API unsigned int    F_API FSOUND_GetCurrentPosition(int channel);
DLL_API FSOUND_SAMPLE * F_API FSOUND_GetCurrentSample(int channel);
DLL_API signed char     F_API FSOUND_GetCurrentLevels(int channel, float *l, float *r);
DLL_API int             F_API FSOUND_GetNumSubChannels(int channel);
DLL_API int             F_API FSOUND_GetSubChannel(int channel, int subchannel);








DLL_API int             F_API FSOUND_FX_Enable(int channel, unsigned int fx);    
DLL_API signed char     F_API FSOUND_FX_Disable(int channel);    

DLL_API signed char     F_API FSOUND_FX_SetChorus(int fxid, float WetDryMix, float Depth, float Feedback, float Frequency, int Waveform, float Delay, int Phase);
DLL_API signed char     F_API FSOUND_FX_SetCompressor(int fxid, float Gain, float Attack, float Release, float Threshold, float Ratio, float Predelay);
DLL_API signed char     F_API FSOUND_FX_SetDistortion(int fxid, float Gain, float Edge, float PostEQCenterFrequency, float PostEQBandwidth, float PreLowpassCutoff);
DLL_API signed char     F_API FSOUND_FX_SetEcho(int fxid, float WetDryMix, float Feedback, float LeftDelay, float RightDelay, int PanDelay);
DLL_API signed char     F_API FSOUND_FX_SetFlanger(int fxid, float WetDryMix, float Depth, float Feedback, float Frequency, int Waveform, float Delay, int Phase);
DLL_API signed char     F_API FSOUND_FX_SetGargle(int fxid, int RateHz, int WaveShape);
DLL_API signed char     F_API FSOUND_FX_SetI3DL2Reverb(int fxid, int Room, int RoomHF, float RoomRolloffFactor, float DecayTime, float DecayHFRatio, int Reflections, float ReflectionsDelay, int Reverb, float ReverbDelay, float Diffusion, float Density, float HFReference);
DLL_API signed char     F_API FSOUND_FX_SetParamEQ(int fxid, float Center, float Bandwidth, float Gain);
DLL_API signed char     F_API FSOUND_FX_SetWavesReverb(int fxid, float InGain, float ReverbMix, float ReverbTime, float HighFreqRTRatio);  
 




DLL_API void            F_API FSOUND_3D_SetDopplerFactor(float scale);
DLL_API void            F_API FSOUND_3D_SetDistanceFactor(float scale);
DLL_API void            F_API FSOUND_3D_SetRolloffFactor(float scale);
DLL_API signed char     F_API FSOUND_3D_SetAttributes(int channel, const float *pos, const float *vel);
DLL_API signed char     F_API FSOUND_3D_GetAttributes(int channel, float *pos, float *vel);

DLL_API void            F_API FSOUND_3D_Listener_SetCurrent(int current, int numlisteners);  
DLL_API void            F_API FSOUND_3D_Listener_SetAttributes(const float *pos, const float *vel, float fx, float fy, float fz, float tx, float ty, float tz);
DLL_API void            F_API FSOUND_3D_Listener_GetAttributes(float *pos, float *vel, float *fx, float *fy, float *fz, float *tx, float *ty, float *tz);
 






DLL_API signed char        F_API FSOUND_Stream_SetBufferSize(int ms);      
                           
DLL_API FSOUND_STREAM *    F_API FSOUND_Stream_Open(const char *name_or_data, unsigned int mode, int offset, int length);
DLL_API FSOUND_STREAM *    F_API FSOUND_Stream_Create(FSOUND_STREAMCALLBACK callback, int length, unsigned int mode, int samplerate, int userdata);
DLL_API signed char        F_API FSOUND_Stream_Close(FSOUND_STREAM *stream);
                           
DLL_API int                F_API FSOUND_Stream_Play(int channel, FSOUND_STREAM *stream);
DLL_API int                F_API FSOUND_Stream_PlayEx(int channel, FSOUND_STREAM *stream, FSOUND_DSPUNIT *dsp, signed char startpaused);
DLL_API signed char        F_API FSOUND_Stream_Stop(FSOUND_STREAM *stream);
                           
DLL_API signed char        F_API FSOUND_Stream_SetPosition(FSOUND_STREAM *stream, unsigned int position);
DLL_API unsigned int       F_API FSOUND_Stream_GetPosition(FSOUND_STREAM *stream);
DLL_API signed char        F_API FSOUND_Stream_SetTime(FSOUND_STREAM *stream, int ms);
DLL_API int                F_API FSOUND_Stream_GetTime(FSOUND_STREAM *stream);
DLL_API int                F_API FSOUND_Stream_GetLength(FSOUND_STREAM *stream);
DLL_API int                F_API FSOUND_Stream_GetLengthMs(FSOUND_STREAM *stream);
                           
DLL_API signed char        F_API FSOUND_Stream_SetMode(FSOUND_STREAM *stream, unsigned int mode);
DLL_API unsigned int       F_API FSOUND_Stream_GetMode(FSOUND_STREAM *stream);
DLL_API signed char        F_API FSOUND_Stream_SetLoopPoints(FSOUND_STREAM *stream, unsigned int loopstartpcm, unsigned int loopendpcm);
DLL_API signed char        F_API FSOUND_Stream_SetLoopCount(FSOUND_STREAM *stream, int count);
DLL_API int                F_API FSOUND_Stream_GetOpenState(FSOUND_STREAM *stream);                
DLL_API FSOUND_SAMPLE *    F_API FSOUND_Stream_GetSample(FSOUND_STREAM *stream);
DLL_API FSOUND_DSPUNIT *   F_API FSOUND_Stream_CreateDSP(FSOUND_STREAM *stream, FSOUND_DSPCALLBACK callback, int priority, int param);
                           
DLL_API signed char        F_API FSOUND_Stream_SetEndCallback(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
DLL_API signed char        F_API FSOUND_Stream_SetSyncCallback(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);

DLL_API FSOUND_SYNCPOINT * F_API FSOUND_Stream_AddSyncPoint(FSOUND_STREAM *stream, unsigned int pcmoffset, const char *name);
DLL_API signed char        F_API FSOUND_Stream_DeleteSyncPoint(FSOUND_SYNCPOINT *point);
DLL_API int                F_API FSOUND_Stream_GetNumSyncPoints(FSOUND_STREAM *stream);
DLL_API FSOUND_SYNCPOINT * F_API FSOUND_Stream_GetSyncPoint(FSOUND_STREAM *stream, int index);
DLL_API char *             F_API FSOUND_Stream_GetSyncPointInfo(FSOUND_SYNCPOINT *point, unsigned int *pcmoffset);

DLL_API signed char        F_API FSOUND_Stream_SetSubStream(FSOUND_STREAM *stream, int index);     
DLL_API int                F_API FSOUND_Stream_GetNumSubStreams(FSOUND_STREAM *stream);            
DLL_API signed char        F_API FSOUND_Stream_SetSubStreamSentence(FSOUND_STREAM *stream, const int *sentencelist, int numitems);

DLL_API signed char        F_API FSOUND_Stream_GetNumTagFields(FSOUND_STREAM *stream, int *num);
DLL_API signed char        F_API FSOUND_Stream_GetTagField(FSOUND_STREAM *stream, int num, int *type, char **name, void **value, int *length);
DLL_API signed char        F_API FSOUND_Stream_FindTagField(FSOUND_STREAM *stream, int type, const char *name, void **value, int *length);



DLL_API signed char        F_API FSOUND_Stream_Net_SetProxy(const char *proxy);
DLL_API char *             F_API FSOUND_Stream_Net_GetLastServerStatus();
DLL_API signed char        F_API FSOUND_Stream_Net_SetBufferProperties(int buffersize, int prebuffer_percent, int rebuffer_percent);
DLL_API signed char        F_API FSOUND_Stream_Net_GetBufferProperties(int *buffersize, int *prebuffer_percent, int *rebuffer_percent);
DLL_API signed char        F_API FSOUND_Stream_Net_SetMetadataCallback(FSOUND_STREAM *stream, FSOUND_METADATACALLBACK callback, int userdata);
DLL_API signed char        F_API FSOUND_Stream_Net_GetStatus(FSOUND_STREAM *stream, int *status, int *bufferpercentused, int *bitrate, unsigned int *flags);







DLL_API signed char     F_API FSOUND_CD_Play(char drive, int track);
DLL_API void            F_API FSOUND_CD_SetPlayMode(char drive, signed char mode);
DLL_API signed char     F_API FSOUND_CD_Stop(char drive);
DLL_API signed char     F_API FSOUND_CD_SetPaused(char drive, signed char paused);
DLL_API signed char     F_API FSOUND_CD_SetVolume(char drive, int volume);
DLL_API signed char     F_API FSOUND_CD_SetTrackTime(char drive, unsigned int ms);
DLL_API signed char     F_API FSOUND_CD_Eject(char drive);

DLL_API signed char     F_API FSOUND_CD_GetPaused(char drive);
DLL_API int             F_API FSOUND_CD_GetTrack(char drive);
DLL_API int             F_API FSOUND_CD_GetNumTracks(char drive);
DLL_API int             F_API FSOUND_CD_GetVolume(char drive);
DLL_API int             F_API FSOUND_CD_GetTrackLength(char drive, int track); 
DLL_API int             F_API FSOUND_CD_GetTrackTime(char drive);







DLL_API FSOUND_DSPUNIT *F_API FSOUND_DSP_Create(FSOUND_DSPCALLBACK callback, int priority, int param);
DLL_API void            F_API FSOUND_DSP_Free(FSOUND_DSPUNIT *unit);
DLL_API void            F_API FSOUND_DSP_SetPriority(FSOUND_DSPUNIT *unit, int priority);
DLL_API int             F_API FSOUND_DSP_GetPriority(FSOUND_DSPUNIT *unit);
DLL_API void            F_API FSOUND_DSP_SetActive(FSOUND_DSPUNIT *unit, signed char active);
DLL_API signed char     F_API FSOUND_DSP_GetActive(FSOUND_DSPUNIT *unit);



DLL_API FSOUND_DSPUNIT *F_API FSOUND_DSP_GetClearUnit();
DLL_API FSOUND_DSPUNIT *F_API FSOUND_DSP_GetSFXUnit();
DLL_API FSOUND_DSPUNIT *F_API FSOUND_DSP_GetMusicUnit();
DLL_API FSOUND_DSPUNIT *F_API FSOUND_DSP_GetFFTUnit();
DLL_API FSOUND_DSPUNIT *F_API FSOUND_DSP_GetClipAndCopyUnit();



DLL_API signed char     F_API FSOUND_DSP_MixBuffers(void *destbuffer, void *srcbuffer, int len, int freq, int vol, int pan, unsigned int mode);
DLL_API void            F_API FSOUND_DSP_ClearMixBuffer();
DLL_API int             F_API FSOUND_DSP_GetBufferLength();      
DLL_API int             F_API FSOUND_DSP_GetBufferLengthTotal(); 
DLL_API float *         F_API FSOUND_DSP_GetSpectrum();          







DLL_API signed char     F_API FSOUND_Reverb_SetProperties(const FSOUND_REVERB_PROPERTIES *prop);
DLL_API signed char     F_API FSOUND_Reverb_GetProperties(FSOUND_REVERB_PROPERTIES *prop);
DLL_API signed char     F_API FSOUND_Reverb_SetChannelProperties(int channel, const FSOUND_REVERB_CHANNELPROPERTIES *prop);
DLL_API signed char     F_API FSOUND_Reverb_GetChannelProperties(int channel, FSOUND_REVERB_CHANNELPROPERTIES *prop);







DLL_API signed char     F_API FSOUND_Record_SetDriver(int outputtype);
DLL_API int             F_API FSOUND_Record_GetNumDrivers();
DLL_API const char *    F_API FSOUND_Record_GetDriverName(int id);
DLL_API int             F_API FSOUND_Record_GetDriver();



DLL_API signed char     F_API FSOUND_Record_StartSample(FSOUND_SAMPLE *sptr, signed char loop);
DLL_API signed char     F_API FSOUND_Record_Stop();
DLL_API int             F_API FSOUND_Record_GetPosition();  







DLL_API FMUSIC_MODULE * F_API FMUSIC_LoadSong(const char *name);
DLL_API FMUSIC_MODULE * F_API FMUSIC_LoadSongEx(const char *name_or_data, int offset, int length, unsigned int mode, const int *samplelist, int samplelistnum);
DLL_API int             F_API FMUSIC_GetOpenState(FMUSIC_MODULE *mod);
DLL_API signed char     F_API FMUSIC_FreeSong(FMUSIC_MODULE *mod);
DLL_API signed char     F_API FMUSIC_PlaySong(FMUSIC_MODULE *mod);
DLL_API signed char     F_API FMUSIC_StopSong(FMUSIC_MODULE *mod);
DLL_API void            F_API FMUSIC_StopAllSongs();

DLL_API signed char     F_API FMUSIC_SetZxxCallback(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback);
DLL_API signed char     F_API FMUSIC_SetRowCallback(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback, int rowstep);
DLL_API signed char     F_API FMUSIC_SetOrderCallback(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback, int orderstep);
DLL_API signed char     F_API FMUSIC_SetInstCallback(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback, int instrument);

DLL_API signed char     F_API FMUSIC_SetSample(FMUSIC_MODULE *mod, int sampno, FSOUND_SAMPLE *sptr);
DLL_API signed char     F_API FMUSIC_SetUserData(FMUSIC_MODULE *mod, unsigned int userdata);
DLL_API signed char     F_API FMUSIC_OptimizeChannels(FMUSIC_MODULE *mod, int maxchannels, int minvolume);



DLL_API signed char     F_API FMUSIC_SetReverb(signed char reverb);             
DLL_API signed char     F_API FMUSIC_SetLooping(FMUSIC_MODULE *mod, signed char looping);
DLL_API signed char     F_API FMUSIC_SetOrder(FMUSIC_MODULE *mod, int order);
DLL_API signed char     F_API FMUSIC_SetPaused(FMUSIC_MODULE *mod, signed char pause);
DLL_API signed char     F_API FMUSIC_SetMasterVolume(FMUSIC_MODULE *mod, int volume);
DLL_API signed char     F_API FMUSIC_SetMasterSpeed(FMUSIC_MODULE *mode, float speed);
DLL_API signed char     F_API FMUSIC_SetPanSeperation(FMUSIC_MODULE *mod, float pansep);
 


DLL_API const char *    F_API FMUSIC_GetName(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetType(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetNumOrders(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetNumPatterns(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetNumInstruments(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetNumSamples(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetNumChannels(FMUSIC_MODULE *mod);
DLL_API FSOUND_SAMPLE * F_API FMUSIC_GetSample(FMUSIC_MODULE *mod, int sampno);
DLL_API int             F_API FMUSIC_GetPatternLength(FMUSIC_MODULE *mod, int orderno);
 


DLL_API signed char     F_API FMUSIC_IsFinished(FMUSIC_MODULE *mod);
DLL_API signed char     F_API FMUSIC_IsPlaying(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetMasterVolume(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetGlobalVolume(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetOrder(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetPattern(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetSpeed(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetBPM(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetRow(FMUSIC_MODULE *mod);
DLL_API signed char     F_API FMUSIC_GetPaused(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetTime(FMUSIC_MODULE *mod);
DLL_API int             F_API FMUSIC_GetRealChannel(FMUSIC_MODULE *mod, int modchannel);
DLL_API unsigned int    F_API FMUSIC_GetUserData(FMUSIC_MODULE *mod);

#ifdef __cplusplus
}
#endif

#endif
