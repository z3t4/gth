

#ifndef __IJL_H__
#define __IJL_H__


#if defined( __cplusplus )
extern "C" {
#endif


#ifndef IJL_ALL_WARNINGS
#if defined (_WIN32)
#if _MSC_VER >= 1000


#pragma warning(disable : 4201)

#pragma warning(disable : 4214)

#pragma warning(disable : 4514)

#pragma warning(disable : 4115)

#endif 
#endif 
#endif 


#if defined (_WIN32)
#define IJL_STDCALL __stdcall
#else
#define IJL_STDCALL
#endif



#if defined (_WIN32)
#pragma pack (8)
#else
#endif





#define JBUFSIZE    4096


#if defined (_WIN32)
#define IJL_INT64  __int64
#else
#define IJL_INT64  long long
#endif

#define IJL_UINT64 unsigned IJL_INT64


#ifndef IJLAPI
  #ifdef IJL_MSEXPORTS
    #define IJLAPI(type,name,arg) \
      extern __declspec(dllimport) type IJL_STDCALL name arg
  #else
    #define IJLAPI(type,name,arg) \
      extern type IJL_STDCALL name arg
  #endif
#endif



#define IJL_DIB_ALIGN (sizeof(int) - 1)

#define IJL_DIB_UWIDTH(width,nchannels) \
  ((width) * (nchannels))

#define IJL_DIB_AWIDTH(width,nchannels) \
  ( ((IJL_DIB_UWIDTH(width,nchannels) + IJL_DIB_ALIGN) & (~IJL_DIB_ALIGN)) )

#define IJL_DIB_PAD_BYTES(width,nchannels) \
  ( IJL_DIB_AWIDTH(width,nchannels) - IJL_DIB_UWIDTH(width,nchannels) )


#define IJL_DIB_SCALE_SIZE(jpgsize,scale) \
  ( ((jpgsize) + (scale) - 1) / (scale) )






typedef struct _IJLibVersion
{
  int         major;
  int         minor;
  int         build;
  const char* Name;
  const char* Version;
  const char* InternalVersion;
  const char* BuildDate;
  const char* CallConv;

} IJLibVersion;




typedef struct _IJL_RECT
{
  long  left;
  long  top;
  long  right;
  long  bottom;

} IJL_RECT;




typedef void* IJL_HANDLE;




typedef enum _IJLIOTYPE
{
  IJL_SETUP                   = -1,


  
  
  IJL_JFILE_READPARAMS        =  0,
  IJL_JBUFF_READPARAMS        =  1,

  
  IJL_JFILE_READWHOLEIMAGE    =  2,
  IJL_JBUFF_READWHOLEIMAGE    =  3,

  
  IJL_JFILE_READHEADER        =  4,
  IJL_JBUFF_READHEADER        =  5,

  
  IJL_JFILE_READENTROPY       =  6,
  IJL_JBUFF_READENTROPY       =  7,

  
  IJL_JFILE_WRITEWHOLEIMAGE   =  8,
  IJL_JBUFF_WRITEWHOLEIMAGE   =  9,

  
  IJL_JFILE_WRITEHEADER       = 10,
  IJL_JBUFF_WRITEHEADER       = 11,

  
  IJL_JFILE_WRITEENTROPY      = 12,
  IJL_JBUFF_WRITEENTROPY      = 13,


  

  
  IJL_JFILE_READONEHALF       = 14,
  IJL_JBUFF_READONEHALF       = 15,

  
  IJL_JFILE_READONEQUARTER    = 16,
  IJL_JBUFF_READONEQUARTER    = 17,

  
  IJL_JFILE_READONEEIGHTH     = 18,
  IJL_JBUFF_READONEEIGHTH     = 19,

  
  IJL_JFILE_READTHUMBNAIL     = 20,
  IJL_JBUFF_READTHUMBNAIL     = 21

} IJLIOTYPE;




typedef enum _IJL_COLOR
{
  IJL_RGB         = 1,    
  IJL_BGR         = 2,    
  IJL_YCBCR       = 3,    
                          
  IJL_G           = 4,    
  IJL_RGBA_FPX    = 5,    
                          
  IJL_YCBCRA_FPX  = 6,    
                          

  IJL_OTHER       = 255   
                          
                          

} IJL_COLOR;




typedef enum _IJL_JPGSUBSAMPLING
{
  IJL_NONE    = 0,    
                      
  IJL_411     = 1,    
  IJL_422     = 2,    

  IJL_4114    = 3,    
  IJL_4224    = 4     

} IJL_JPGSUBSAMPLING;




typedef IJL_JPGSUBSAMPLING IJL_DIBSUBSAMPLING;




typedef struct _HUFFMAN_TABLE
{
  int             huff_class;
  int             ident;
  unsigned int    huffelem[256];
  unsigned short  huffval[256];
  unsigned short  mincode[17];
  short           maxcode[18];
  unsigned short  valptr[17];

} HUFFMAN_TABLE;




typedef struct _JPEGHuffTable
{
  unsigned char* bits;
  unsigned char* vals;
  unsigned char  hclass;
  unsigned char  ident;

} JPEGHuffTable;




typedef struct _QUANT_TABLE
{
  int    precision;
  int    ident;
  short* elements;
  short  elarray [84];

} QUANT_TABLE;




typedef struct _JPEGQuantTable
{
  unsigned char* quantizer;
  unsigned char  ident;

} JPEGQuantTable;




typedef struct _FRAME_COMPONENT
{
  int ident;
  int hsampling;
  int vsampling;
  int quant_sel;

} FRAME_COMPONENT;




typedef struct _FRAME
{
  int              precision;
  int              width;
  int              height;
  int              MCUheight;
  int              MCUwidth;
  int              max_hsampling;
  int              max_vsampling;
  int              ncomps;
  int              horMCU;
  long             totalMCU;
  FRAME_COMPONENT* comps;
  int              restart_interv;
  int              SeenAllDCScans;
  int              SeenAllACScans;

} FRAME;




typedef struct _SCAN_COMPONENT
{
  int            comp;
  int            hsampling;
  int            vsampling;
  HUFFMAN_TABLE* dc_table;
  HUFFMAN_TABLE* ac_table;
  QUANT_TABLE*   quant_table;

} SCAN_COMPONENT;




typedef struct _SCAN
{
  int             ncomps;
  int             gray_scale;
  int             start_spec;
  int             end_spec;
  int             approx_high;
  int             approx_low;
  unsigned int    restart_interv;
  int             curxMCU;
  int             curyMCU;
  int             dc_diff[4];
  SCAN_COMPONENT* comps;

} SCAN;




typedef enum _DCTTYPE
{
  IJL_AAN = 0,
  IJL_IPP = 1

} DCTTYPE;




typedef enum _UPSAMPLING_TYPE
{
  IJL_BOX_FILTER      = 0,
  IJL_TRIANGLE_FILTER = 1

} UPSAMPLING_TYPE;




typedef struct _SAMPLING_STATE
{
  short* top_row;
  short* cur_row;
  short* bottom_row;
  short* last_row;
  int    cur_row_number;

} SAMPLING_STATE;




typedef enum _PROCESSOR_TYPE
{
  IJL_OTHER_PROC            = 0,
  IJL_PENTIUM_PROC          = 1,
  IJL_PENTIUM_PRO_PROC      = 2,
  IJL_PENTIUM_PROC_MMX_TECH = 3,
  IJL_PENTIUM_II_PROC       = 4,
  IJL_PENTIUM_III_PROC      = 5,
  IJL_PENTIUM_4_PROC        = 6,
  IJL_NEW_PROCESSOR         = 7

} PROCESSOR_TYPE;



typedef struct _RAW_DATA_TYPES_STATE
{
  int data_type;
  unsigned short* raw_ptrs[4];

} RAW_DATA_TYPES_STATE;




typedef struct _ENTROPYSTRUCT
{
  unsigned int   offset;
  int            dcval1;
  int            dcval2;
  int            dcval3;
  int            dcval4;
  IJL_UINT64     bit_buffer_64;
  int            bitbuf_bits_valid;
  unsigned char  unread_marker;

} ENTROPYSTRUCT;




typedef struct _STATE
{
  
  IJL_UINT64     bit_buffer_64;
  void*          ctx;
  int            bitbuf_bits_valid;

  
  unsigned char* cur_entropy_ptr;
  unsigned char* start_entropy_ptr;
  unsigned char* end_entropy_ptr;
  int            entropy_bytes_processed;
  int            entropy_buf_maxsize;
  int            entropy_bytes_left;
  int            Prog_EndOfBlock_Run;

  
  unsigned char* DIB_ptr;

  
  unsigned char  unread_marker;
  PROCESSOR_TYPE processor_type;
  int            cur_scan_comp;
  IJL_HANDLE     file;
  unsigned char  JPGBuffer [JBUFSIZE];

} STATE;




typedef enum _FAST_MCU_PROCESSING_TYPE
{
  IJL_NO_CC_OR_US                   = 0,

  IJL_111_YCBCR_111_RGB             = 1,
  IJL_111_YCBCR_111_BGR             = 2,

  IJL_411_YCBCR_111_RGB             = 3,
  IJL_411_YCBCR_111_BGR             = 4,

  IJL_422_YCBCR_111_RGB             = 5,
  IJL_422_YCBCR_111_BGR             = 6,

  IJL_111_YCBCR_1111_RGBA_FPX       = 7,
  IJL_411_YCBCR_1111_RGBA_FPX       = 8,
  IJL_422_YCBCR_1111_RGBA_FPX       = 9,

  IJL_1111_YCBCRA_FPX_1111_RGBA_FPX = 10,
  IJL_4114_YCBCRA_FPX_1111_RGBA_FPX = 11,
  IJL_4224_YCBCRA_FPX_1111_RGBA_FPX = 12,

  IJL_111_RGB_1111_RGBA_FPX         = 13,

  IJL_1111_RGBA_FPX_1111_RGBA_FPX   = 14,

  IJL_111_OTHER_111_OTHER           = 15,
  IJL_411_OTHER_111_OTHER           = 16,
  IJL_422_OTHER_111_OTHER           = 17,

  IJL_YCBYCR_YCBCR                  = 18,
  IJL_YCBCR_YCBYCR                  = 19,

  IJL_1_G_1_G                       = 20

} FAST_MCU_PROCESSING_TYPE;




typedef struct _JPEG_PROPERTIES
{
  
  IJLIOTYPE                iotype;               
  IJL_RECT                 roi;                  
  DCTTYPE                  dcttype;              
  FAST_MCU_PROCESSING_TYPE fast_processing;      
  int                      interrupt;            

  
  unsigned char*           DIBBytes;             
  int                      DIBWidth;             
  int                      DIBHeight;            
  int                      DIBPadBytes;          
  int                      DIBChannels;          
  IJL_COLOR                DIBColor;             
  IJL_DIBSUBSAMPLING       DIBSubsampling;       
  int                      DIBLineBytes;         

  
  const char*              JPGFile;              
  unsigned char*           JPGBytes;             
  int                      JPGSizeBytes;         
  int                      JPGWidth;             
  int                      JPGHeight;            
  int                      JPGChannels;          
  IJL_COLOR                JPGColor;             
  IJL_JPGSUBSAMPLING       JPGSubsampling;       
  int                      JPGThumbWidth;        
  int                      JPGThumbHeight;       

  
  int                      cconversion_reqd;     
  int                      upsampling_reqd;      
  int                      jquality;             
  int                      jinterleaveType;      
  int                      numxMCUs;             
  int                      numyMCUs;             

  
  int                      nqtables;
  int                      maxquantindex;
  int                      nhuffActables;
  int                      nhuffDctables;
  int                      maxhuffindex;

  QUANT_TABLE              jFmtQuant[4];
  HUFFMAN_TABLE            jFmtAcHuffman[4];
  HUFFMAN_TABLE            jFmtDcHuffman[4];

  short*                   jEncFmtQuant[4];
  HUFFMAN_TABLE*           jEncFmtAcHuffman[4];
  HUFFMAN_TABLE*           jEncFmtDcHuffman[4];

  
  int                      use_external_qtables;
  int                      use_external_htables;

  JPEGQuantTable           rawquanttables[4];
  JPEGHuffTable            rawhufftables[8];
  char                     HuffIdentifierAC[4];
  char                     HuffIdentifierDC[4];

  
  FRAME                    jframe;
  int                      needframe;

  
  SCAN*                    jscan;

  
  STATE                    state;
  int                      SawAdobeMarker;
  int                      AdobeXform;

  
  ENTROPYSTRUCT*           rowoffsets;

  
  unsigned char*           MCUBuf;
  unsigned char            tMCUBuf[720*2];

  
  PROCESSOR_TYPE           processor_type;

  RAW_DATA_TYPES_STATE*    raw_coefs;

  
  int                      progressive_found;
  short*                   coef_buffer;

  
  UPSAMPLING_TYPE          upsampling_type;
  SAMPLING_STATE*          sampling_state_ptr;

  
  unsigned short           AdobeVersion;         
  unsigned short           AdobeFlags0;          
  unsigned short           AdobeFlags1;          

  
  int                      jfif_app0_detected;
  unsigned short           jfif_app0_version;    
  unsigned char            jfif_app0_units;      
  unsigned short           jfif_app0_Xdensity;   
  unsigned short           jfif_app0_Ydensity;   

  
  char*                    jpeg_comment;         
  unsigned short           jpeg_comment_size;    

} JPEG_PROPERTIES;




typedef struct _JPEG_CORE_PROPERTIES
{
  int                UseJPEGPROPERTIES;         

  
  unsigned char*     DIBBytes;                  
  int                DIBWidth;                  
  int                DIBHeight;                 
  int                DIBPadBytes;               
  int                DIBChannels;               
  IJL_COLOR          DIBColor;                  
  IJL_DIBSUBSAMPLING DIBSubsampling;            

  
  const char*        JPGFile;                   
  unsigned char*     JPGBytes;                  
  int                JPGSizeBytes;              
  int                JPGWidth;                  
  int                JPGHeight;                 
  int                JPGChannels;               
  IJL_COLOR          JPGColor;                  
  IJL_JPGSUBSAMPLING JPGSubsampling;            
  int                JPGThumbWidth;             
  int                JPGThumbHeight;            

  
  int                cconversion_reqd;          
  int                upsampling_reqd;           
  int                jquality;                  

  
  JPEG_PROPERTIES    jprops;

} JPEG_CORE_PROPERTIES;




typedef enum _IJLERR
{
  
  IJL_OK                          = 0,
  IJL_INTERRUPT_OK                = 1,
  IJL_ROI_OK                      = 2,

  
  IJL_EXCEPTION_DETECTED          =  -1,
  IJL_INVALID_ENCODER             =  -2,
  IJL_UNSUPPORTED_SUBSAMPLING     =  -3,
  IJL_UNSUPPORTED_BYTES_PER_PIXEL =  -4,
  IJL_MEMORY_ERROR                =  -5,
  IJL_BAD_HUFFMAN_TABLE           =  -6,
  IJL_BAD_QUANT_TABLE             =  -7,
  IJL_INVALID_JPEG_PROPERTIES     =  -8,
  IJL_ERR_FILECLOSE               =  -9,
  IJL_INVALID_FILENAME            = -10,
  IJL_ERROR_EOF                   = -11,
  IJL_PROG_NOT_SUPPORTED          = -12,
  IJL_ERR_NOT_JPEG                = -13,
  IJL_ERR_COMP                    = -14,
  IJL_ERR_SOF                     = -15,
  IJL_ERR_DNL                     = -16,
  IJL_ERR_NO_HUF                  = -17,
  IJL_ERR_NO_QUAN                 = -18,
  IJL_ERR_NO_FRAME                = -19,
  IJL_ERR_MULT_FRAME              = -20,
  IJL_ERR_DATA                    = -21,
  IJL_ERR_NO_IMAGE                = -22,
  IJL_FILE_ERROR                  = -23,
  IJL_INTERNAL_ERROR              = -24,
  IJL_BAD_RST_MARKER              = -25,
  IJL_THUMBNAIL_DIB_TOO_SMALL     = -26,
  IJL_THUMBNAIL_DIB_WRONG_COLOR   = -27,
  IJL_BUFFER_TOO_SMALL            = -28,
  IJL_UNSUPPORTED_FRAME           = -29,
  IJL_ERR_COM_BUFFER              = -30,
  IJL_RESERVED                    = -99

} IJLERR;









IJLAPI(IJLERR, ijlInit, ( JPEG_CORE_PROPERTIES* jcprops ));




IJLAPI(IJLERR, ijlFree, ( JPEG_CORE_PROPERTIES* jcprops ));




IJLAPI(IJLERR, ijlRead, ( JPEG_CORE_PROPERTIES* jcprops, IJLIOTYPE iotype ));




IJLAPI(IJLERR, ijlWrite, ( JPEG_CORE_PROPERTIES* jcprops, IJLIOTYPE iotype ));




IJLAPI(const IJLibVersion*, ijlGetLibVersion, (void));




IJLAPI(const char*, ijlErrorStr, (IJLERR code));




#if defined( __cplusplus )
}
#endif

#endif  
