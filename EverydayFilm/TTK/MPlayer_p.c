

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "MPlayer_h.h"

#define TYPE_FORMAT_STRING_SIZE   77                                
#define PROC_FORMAT_STRING_SIZE   2245                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MPlayer_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MPlayer_MIDL_TYPE_FORMAT_STRING;

typedef struct _MPlayer_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MPlayer_MIDL_PROC_FORMAT_STRING;

typedef struct _MPlayer_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } MPlayer_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MPlayer_MIDL_TYPE_FORMAT_STRING MPlayer__MIDL_TypeFormatString;
extern const MPlayer_MIDL_PROC_FORMAT_STRING MPlayer__MIDL_ProcFormatString;
extern const MPlayer_MIDL_EXPR_FORMAT_STRING MPlayer__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMMPlayer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IMMPlayer_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MPlayer_MIDL_PROC_FORMAT_STRING MPlayer__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure put_BorderStyle */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter style */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_BorderStyle */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x24 ),	/* 36 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pstyle */

/* 60 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Appearance */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x6 ),	/* 6 */
/* 84 */	NdrFcShort( 0x8 ),	/* 8 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nAppearance */

/* 96 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Appearance */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x22 ),	/* 34 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pnAppearance */

/* 132 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_FullScreen */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xb ),	/* 11 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x24 ),	/* 36 */
/* 158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 168 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_FullScreen */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xc ),	/* 12 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_URL */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xd ),	/* 13 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x8 ),	/* 8 */
/* 230 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 232 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 234 */	NdrFcShort( 0x1 ),	/* 1 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 240 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_URL */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xe ),	/* 14 */
/* 260 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 266 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 268 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x1 ),	/* 1 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 276 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Play */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0xf ),	/* 15 */
/* 296 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 302 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 312 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Pause */

/* 318 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 320 */	NdrFcLong( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0x10 ),	/* 16 */
/* 326 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 328 */	NdrFcShort( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x8 ),	/* 8 */
/* 332 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 334 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 340 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 342 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 344 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 346 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Stop */

/* 348 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 350 */	NdrFcLong( 0x0 ),	/* 0 */
/* 354 */	NdrFcShort( 0x11 ),	/* 17 */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 362 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 364 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 366 */	NdrFcShort( 0x0 ),	/* 0 */
/* 368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 372 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 374 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 376 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CurrentPosition */

/* 378 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 380 */	NdrFcLong( 0x0 ),	/* 0 */
/* 384 */	NdrFcShort( 0x12 ),	/* 18 */
/* 386 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 390 */	NdrFcShort( 0x24 ),	/* 36 */
/* 392 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 394 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 398 */	NdrFcShort( 0x0 ),	/* 0 */
/* 400 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 402 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 404 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 408 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 410 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_CurrentPosition */

/* 414 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 416 */	NdrFcLong( 0x0 ),	/* 0 */
/* 420 */	NdrFcShort( 0x13 ),	/* 19 */
/* 422 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 424 */	NdrFcShort( 0x8 ),	/* 8 */
/* 426 */	NdrFcShort( 0x8 ),	/* 8 */
/* 428 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 430 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 438 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 440 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 446 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Volume */

/* 450 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x14 ),	/* 20 */
/* 458 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 460 */	NdrFcShort( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0x24 ),	/* 36 */
/* 464 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 466 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 474 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 476 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 480 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 482 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Volume */

/* 486 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 488 */	NdrFcLong( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x15 ),	/* 21 */
/* 494 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 496 */	NdrFcShort( 0x8 ),	/* 8 */
/* 498 */	NdrFcShort( 0x8 ),	/* 8 */
/* 500 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 502 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 508 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 510 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 512 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 516 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 518 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_VideoFrame */

/* 522 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 528 */	NdrFcShort( 0x16 ),	/* 22 */
/* 530 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0x24 ),	/* 36 */
/* 536 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 538 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 542 */	NdrFcShort( 0x0 ),	/* 0 */
/* 544 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 546 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 548 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 552 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 554 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 556 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_VideoFrame */

/* 558 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 560 */	NdrFcLong( 0x0 ),	/* 0 */
/* 564 */	NdrFcShort( 0x17 ),	/* 23 */
/* 566 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 568 */	NdrFcShort( 0x8 ),	/* 8 */
/* 570 */	NdrFcShort( 0x8 ),	/* 8 */
/* 572 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 574 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 584 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 588 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 590 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 592 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RendererType */

/* 594 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 596 */	NdrFcLong( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x18 ),	/* 24 */
/* 602 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x24 ),	/* 36 */
/* 608 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 610 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 618 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 620 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 622 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 626 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RendererType */

/* 630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x19 ),	/* 25 */
/* 638 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 640 */	NdrFcShort( 0x8 ),	/* 8 */
/* 642 */	NdrFcShort( 0x8 ),	/* 8 */
/* 644 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 646 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 654 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 656 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 662 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Balance */

/* 666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x1a ),	/* 26 */
/* 674 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x24 ),	/* 36 */
/* 680 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 682 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 690 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 692 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 694 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 696 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 698 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Balance */

/* 702 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 704 */	NdrFcLong( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x1b ),	/* 27 */
/* 710 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 714 */	NdrFcShort( 0x8 ),	/* 8 */
/* 716 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 718 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 726 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 728 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 732 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 734 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Duration */

/* 738 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 740 */	NdrFcLong( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x1c ),	/* 28 */
/* 746 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x24 ),	/* 36 */
/* 752 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 754 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 760 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter length */

/* 762 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 764 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 770 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Mute */

/* 774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x1d ),	/* 29 */
/* 782 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x24 ),	/* 36 */
/* 788 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 790 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 798 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 804 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 806 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Mute */

/* 810 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 812 */	NdrFcLong( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x1e ),	/* 30 */
/* 818 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 822 */	NdrFcShort( 0x8 ),	/* 8 */
/* 824 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 826 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 830 */	NdrFcShort( 0x0 ),	/* 0 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 834 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 836 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 838 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 840 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 842 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 844 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_State */

/* 846 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 848 */	NdrFcLong( 0x0 ),	/* 0 */
/* 852 */	NdrFcShort( 0x1f ),	/* 31 */
/* 854 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x24 ),	/* 36 */
/* 860 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 862 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter state */

/* 870 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 872 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 874 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 876 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 878 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 880 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_id */

/* 882 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 884 */	NdrFcLong( 0x0 ),	/* 0 */
/* 888 */	NdrFcShort( 0x20 ),	/* 32 */
/* 890 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 892 */	NdrFcShort( 0x19 ),	/* 25 */
/* 894 */	NdrFcShort( 0x8 ),	/* 8 */
/* 896 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 898 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 904 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 906 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 908 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 910 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Return value */

/* 912 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 914 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_net */

/* 918 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 920 */	NdrFcLong( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x21 ),	/* 33 */
/* 926 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 928 */	NdrFcShort( 0x19 ),	/* 25 */
/* 930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 932 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 934 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 938 */	NdrFcShort( 0x0 ),	/* 0 */
/* 940 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter inNet */

/* 942 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 944 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 946 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Return value */

/* 948 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 950 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 952 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_FileLength */

/* 954 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 956 */	NdrFcLong( 0x0 ),	/* 0 */
/* 960 */	NdrFcShort( 0x22 ),	/* 34 */
/* 962 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 964 */	NdrFcShort( 0x0 ),	/* 0 */
/* 966 */	NdrFcShort( 0x2c ),	/* 44 */
/* 968 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 970 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 976 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 978 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 980 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 982 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Return value */

/* 984 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 986 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 988 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_FileLength */

/* 990 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 992 */	NdrFcLong( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0x23 ),	/* 35 */
/* 998 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1000 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1004 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1006 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1014 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1016 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1018 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Return value */

/* 1020 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1022 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1024 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowAd */

/* 1026 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1028 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1032 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1034 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1036 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1040 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1042 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1048 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1050 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1052 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1054 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure HideAd */

/* 1056 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1058 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1062 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1064 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1068 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1070 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1072 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1078 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1080 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1082 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1084 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateNetSource */

/* 1086 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1088 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x26 ),	/* 38 */
/* 1094 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1098 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1100 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1102 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1112 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CanConnect */

/* 1116 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1118 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0x27 ),	/* 39 */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1128 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1130 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1132 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bCanConn */

/* 1140 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1142 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1148 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RendererModule */

/* 1152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1158 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1160 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1164 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1166 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1168 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1176 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1178 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1180 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1182 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1184 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RendererModule */

/* 1188 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1190 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1194 */	NdrFcShort( 0x29 ),	/* 41 */
/* 1196 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1198 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1200 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1202 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1204 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1212 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1214 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1216 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1220 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_hue */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0x2a ),	/* 42 */
/* 1232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1238 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1240 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1248 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1250 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1252 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1256 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_hue */

/* 1260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1266 */	NdrFcShort( 0x2b ),	/* 43 */
/* 1268 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1274 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1276 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1282 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1284 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1286 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1288 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1292 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_gama */

/* 1296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1302 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1304 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1308 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1310 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1312 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1318 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1320 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1322 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1324 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1328 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_gama */

/* 1332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1338 */	NdrFcShort( 0x2d ),	/* 45 */
/* 1340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1342 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1346 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1348 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1356 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1360 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_contrast */

/* 1368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x2e ),	/* 46 */
/* 1376 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1382 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1384 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1392 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1394 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1396 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1398 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1400 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1402 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_contrast */

/* 1404 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1406 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1410 */	NdrFcShort( 0x2f ),	/* 47 */
/* 1412 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1416 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1418 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1420 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1422 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1426 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1428 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1430 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1432 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1434 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1436 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_lightness */

/* 1440 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1442 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1446 */	NdrFcShort( 0x30 ),	/* 48 */
/* 1448 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1452 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1454 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1456 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1460 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1462 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1464 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1466 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1468 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1470 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1472 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_lightness */

/* 1476 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1478 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1482 */	NdrFcShort( 0x31 ),	/* 49 */
/* 1484 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1488 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1490 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1492 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1498 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1500 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1502 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1504 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1506 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1508 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1510 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_saturation */

/* 1512 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1514 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1518 */	NdrFcShort( 0x32 ),	/* 50 */
/* 1520 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1524 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1526 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1528 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1534 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1536 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1538 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1542 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1544 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1546 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_saturation */

/* 1548 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1550 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1554 */	NdrFcShort( 0x33 ),	/* 51 */
/* 1556 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1558 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1560 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1562 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1564 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1570 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1572 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1574 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1578 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1580 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1582 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSourceSize */

/* 1584 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1586 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1590 */	NdrFcShort( 0x34 ),	/* 52 */
/* 1592 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1594 */	NdrFcShort( 0x38 ),	/* 56 */
/* 1596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1598 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1600 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1606 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cx */

/* 1608 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 1610 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter cy */

/* 1614 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 1616 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1622 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetHWND */

/* 1626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1632 */	NdrFcShort( 0x35 ),	/* 53 */
/* 1634 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1636 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1638 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1640 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1642 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1648 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hWnd */

/* 1650 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 1652 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1654 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1656 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1658 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1660 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_bMini */

/* 1662 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1664 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1668 */	NdrFcShort( 0x36 ),	/* 54 */
/* 1670 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1674 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1676 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1678 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1684 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1686 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1688 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1692 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1694 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1696 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_bMini */

/* 1698 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1700 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1704 */	NdrFcShort( 0x37 ),	/* 55 */
/* 1706 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1708 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1710 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1712 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1714 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1720 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1722 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1724 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1728 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1730 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1732 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Max */

/* 1734 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1736 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1740 */	NdrFcShort( 0x38 ),	/* 56 */
/* 1742 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1744 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1746 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1748 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1750 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1756 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1758 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1760 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1762 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1764 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1766 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1768 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_DownNO */

/* 1770 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1772 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1776 */	NdrFcShort( 0x39 ),	/* 57 */
/* 1778 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1780 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1782 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1784 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1786 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1792 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1794 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1796 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1798 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1800 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1802 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1804 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_PlayNO */

/* 1806 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1808 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1812 */	NdrFcShort( 0x3a ),	/* 58 */
/* 1814 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1818 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1820 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1822 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1826 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1828 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1830 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1832 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1836 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1838 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1840 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_TimeStr */

/* 1842 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1844 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1848 */	NdrFcShort( 0x3b ),	/* 59 */
/* 1850 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1854 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1856 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1858 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1862 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1864 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1866 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1868 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1870 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 1872 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1874 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1876 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enable */

/* 1878 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1880 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1884 */	NdrFcShort( 0x3c ),	/* 60 */
/* 1886 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1888 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1890 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1892 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1894 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1900 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1902 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1904 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1906 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1908 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1910 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1912 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_tsuyoku */

/* 1914 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1916 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1920 */	NdrFcShort( 0x3d ),	/* 61 */
/* 1922 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1926 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1928 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1930 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1936 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1938 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1940 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1944 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1946 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_tsuyoku */

/* 1950 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1952 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1956 */	NdrFcShort( 0x3e ),	/* 62 */
/* 1958 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1960 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1964 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1966 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1972 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1974 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1976 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1978 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1980 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1982 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPic */

/* 1986 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1988 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0x3f ),	/* 63 */
/* 1994 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2000 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2002 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2006 */	NdrFcShort( 0xa ),	/* 10 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter SafePath */

/* 2010 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2012 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2014 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Parameter StrFormat */

/* 2016 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2018 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2020 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 2022 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2024 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_image */

/* 2028 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2030 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2034 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2036 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2040 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2042 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2044 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2050 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2052 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2054 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2056 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2058 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2060 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_image */

/* 2064 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2066 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2070 */	NdrFcShort( 0x41 ),	/* 65 */
/* 2072 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2074 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2076 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2078 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2080 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2088 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2090 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2092 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2094 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2096 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2098 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_adTime */

/* 2100 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2102 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2106 */	NdrFcShort( 0x42 ),	/* 66 */
/* 2108 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2112 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2114 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2116 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2122 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2124 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2126 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2132 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_adTime */

/* 2136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2142 */	NdrFcShort( 0x43 ),	/* 67 */
/* 2144 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2150 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2152 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2160 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2162 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2166 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2168 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_speed */

/* 2172 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2174 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2178 */	NdrFcShort( 0x44 ),	/* 68 */
/* 2180 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2184 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2186 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2188 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2190 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2194 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2196 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2198 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2200 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Return value */

/* 2202 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2204 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2206 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_speed */

/* 2208 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2210 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2214 */	NdrFcShort( 0x45 ),	/* 69 */
/* 2216 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2222 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2224 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2228 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2230 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2232 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2234 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2236 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 2238 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2240 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2242 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MPlayer_MIDL_TYPE_FORMAT_STRING MPlayer__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  8 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 12 */	NdrFcShort( 0x1c ),	/* Offset= 28 (40) */
/* 14 */	
			0x13, 0x0,	/* FC_OP */
/* 16 */	NdrFcShort( 0xe ),	/* Offset= 14 (30) */
/* 18 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 20 */	NdrFcShort( 0x2 ),	/* 2 */
/* 22 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 24 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 26 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 28 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 30 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 32 */	NdrFcShort( 0x8 ),	/* 8 */
/* 34 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (18) */
/* 36 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 38 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 40 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 42 */	NdrFcShort( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x4 ),	/* 4 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0xffde ),	/* Offset= -34 (14) */
/* 50 */	
			0x12, 0x0,	/* FC_UP */
/* 52 */	NdrFcShort( 0xffea ),	/* Offset= -22 (30) */
/* 54 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x4 ),	/* 4 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (50) */
/* 64 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 66 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 68 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 70 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 72 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 74 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IMMPlayer, ver. 0.0,
   GUID={0x98D2729B,0xD43C,0x4426,{0xB3,0xB5,0xD9,0x9C,0xE2,0x5F,0xE0,0x9D}} */

#pragma code_seg(".orpc")
static const unsigned short IMMPlayer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    144,
    180,
    216,
    252,
    288,
    318,
    348,
    378,
    414,
    450,
    486,
    522,
    558,
    594,
    630,
    666,
    702,
    738,
    774,
    810,
    846,
    882,
    918,
    954,
    990,
    1026,
    1056,
    1086,
    1116,
    1152,
    1188,
    1224,
    1260,
    1296,
    1332,
    1368,
    1404,
    1440,
    1476,
    1512,
    1548,
    1584,
    1626,
    1662,
    1698,
    1734,
    1770,
    1806,
    1842,
    1878,
    1914,
    1950,
    1986,
    2028,
    2064,
    2100,
    2136,
    2172,
    2208
    };

static const MIDL_STUBLESS_PROXY_INFO IMMPlayer_ProxyInfo =
    {
    &Object_StubDesc,
    MPlayer__MIDL_ProcFormatString.Format,
    &IMMPlayer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IMMPlayer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MPlayer__MIDL_ProcFormatString.Format,
    &IMMPlayer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(70) _IMMPlayerProxyVtbl = 
{
    &IMMPlayer_ProxyInfo,
    &IID_IMMPlayer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_BorderStyle */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_BorderStyle */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_Appearance */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_Appearance */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_FullScreen */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_FullScreen */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_URL */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_URL */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::Play */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::Pause */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::Stop */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_CurrentPosition */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_CurrentPosition */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_Volume */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_Volume */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_VideoFrame */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_VideoFrame */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_RendererType */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_RendererType */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_Balance */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_Balance */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_Duration */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_Mute */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_Mute */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_State */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_id */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_net */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_FileLength */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_FileLength */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::ShowAd */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::HideAd */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::CreateNetSource */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::CanConnect */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_RendererModule */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_RendererModule */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_hue */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_hue */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_gama */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_gama */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_contrast */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_contrast */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_lightness */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_lightness */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_saturation */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_saturation */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::GetSourceSize */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::GetHWND */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_bMini */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_bMini */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_Max */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_DownNO */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_PlayNO */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_TimeStr */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_Enable */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_tsuyoku */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_tsuyoku */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::GetPic */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_image */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_image */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_adTime */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_adTime */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::get_speed */ ,
    (void *) (INT_PTR) -1 /* IMMPlayer::put_speed */
};


static const PRPC_STUB_FUNCTION IMMPlayer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMMPlayerStubVtbl =
{
    &IID_IMMPlayer,
    &IMMPlayer_ServerInfo,
    70,
    &IMMPlayer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    MPlayer__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _MPlayer_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IMMPlayerProxyVtbl,
    0
};

const CInterfaceStubVtbl * _MPlayer_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IMMPlayerStubVtbl,
    0
};

PCInterfaceName const _MPlayer_InterfaceNamesList[] = 
{
    "IMMPlayer",
    0
};

const IID *  _MPlayer_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _MPlayer_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _MPlayer, pIID, n)

int __stdcall _MPlayer_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_MPlayer_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo MPlayer_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _MPlayer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _MPlayer_StubVtblList,
    (const PCInterfaceName * ) & _MPlayer_InterfaceNamesList,
    (const IID ** ) & _MPlayer_BaseIIDList,
    & _MPlayer_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

