

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Jan 19 16:14:06 2012
 */
/* Compiler settings for .\MPlayer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IMMPlayer,0x98D2729B,0xD43C,0x4426,0xB3,0xB5,0xD9,0x9C,0xE2,0x5F,0xE0,0x9D);


MIDL_DEFINE_GUID(IID, LIBID_MPlayerLib,0xFB0E97B5,0x1F16,0x468E,0xB3,0x32,0x4F,0x37,0x86,0xF6,0x1F,0xB5);


MIDL_DEFINE_GUID(IID, DIID__IMMPlayerEvents,0x420F66C6,0x0B80,0x4F53,0xAB,0x96,0x7C,0xA6,0xCA,0x57,0xDB,0x81);


MIDL_DEFINE_GUID(CLSID, CLSID_MMPlayer,0xF3D03D98,0xE1F1,0x4F78,0xA4,0x96,0xE2,0x2B,0xE0,0xA4,0xBF,0x83);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



