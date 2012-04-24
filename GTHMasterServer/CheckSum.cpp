












#include "Checksum.h"



void checksum::add(DWORD value)
{
 union { DWORD value; BYTE bytes[4]; } data;
 data.value = value;
 for(UINT i = 0; i < sizeof(data.bytes); i++)
    add(data.bytes[i]);
} 



void checksum::add(WORD value)
{
 union { DWORD value; BYTE bytes[2]; } data;
 data.value = value;
 for(UINT i = 0; i < sizeof(data.bytes); i++)
   add(data.bytes[i]);
} 



void checksum::add(BYTE value)
{
 BYTE cipher = (value ^ (r >> 8));
 r = (cipher + r) * c1 + c2;
 sum += cipher;
} 











void checksum::add(LPBYTE b, UINT length)
{
for(UINT i = 0; i < length; i++)
   add(b[i]);
} 