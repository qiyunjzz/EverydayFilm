

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Nov 24 23:35:51 2011
 */
/* Compiler settings for .\MPlayer.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MPlayer_i_h__
#define __MPlayer_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMMPlayer_FWD_DEFINED__
#define __IMMPlayer_FWD_DEFINED__
typedef interface IMMPlayer IMMPlayer;
#endif 	/* __IMMPlayer_FWD_DEFINED__ */


#ifndef ___IMMPlayerEvents_FWD_DEFINED__
#define ___IMMPlayerEvents_FWD_DEFINED__
typedef interface _IMMPlayerEvents _IMMPlayerEvents;
#endif 	/* ___IMMPlayerEvents_FWD_DEFINED__ */


#ifndef __MMPlayer_FWD_DEFINED__
#define __MMPlayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class MMPlayer MMPlayer;
#else
typedef struct MMPlayer MMPlayer;
#endif /* __cplusplus */

#endif 	/* __MMPlayer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMMPlayer_INTERFACE_DEFINED__
#define __IMMPlayer_INTERFACE_DEFINED__

/* interface IMMPlayer */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMMPlayer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("98D2729B-D43C-4426-B3B5-D99CE25FE09D")
    IMMPlayer : public IDispatch
    {
    public:
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_BorderStyle( 
            /* [in] */ long style) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_BorderStyle( 
            /* [retval][out] */ long *pstyle) = 0;
        
        virtual /* [id][requestedit][bindable][propput] */ HRESULT STDMETHODCALLTYPE put_Appearance( 
            /* [in] */ short nAppearance) = 0;
        
        virtual /* [id][requestedit][bindable][propget] */ HRESULT STDMETHODCALLTYPE get_Appearance( 
            /* [retval][out] */ short *pnAppearance) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FullScreen( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FullScreen( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_URL( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_URL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Play( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentPosition( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentPosition( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Volume( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Volume( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VideoFrame( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_VideoFrame( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RendererType( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RendererType( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Balance( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Balance( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_Duration( 
            /* [retval][out] */ LONG *length) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mute( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mute( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_State( 
            /* [retval][out] */ LONG *state) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE put_id( 
            /* [in] */ CHAR *id) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE put_net( 
            /* [in] */ CHAR *inNet) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileLength( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FileLength( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowAd( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HideAd( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNetSource( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CanConnect( 
            /* [retval][out] */ LONG *bCanConn) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RendererModule( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RendererModule( 
            /* [in] */ SHORT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_hue( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_hue( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_gama( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_gama( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_contrast( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_contrast( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_lightness( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_lightness( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_saturation( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_saturation( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSourceSize( 
            LONG *cx,
            LONG *cy) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetHWND( 
            LONG *hWnd) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_bMini( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_bMini( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Max( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DownNO( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PlayNO( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TimeStr( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enable( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_tsuyoku( 
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_tsuyoku( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPic( 
            /* [in] */ BSTR SafePath,
            /* [in] */ BSTR StrFormat) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMMPlayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMMPlayer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMMPlayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMMPlayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMMPlayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMMPlayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMMPlayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMMPlayer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BorderStyle )( 
            IMMPlayer * This,
            /* [in] */ long style);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BorderStyle )( 
            IMMPlayer * This,
            /* [retval][out] */ long *pstyle);
        
        /* [id][requestedit][bindable][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Appearance )( 
            IMMPlayer * This,
            /* [in] */ short nAppearance);
        
        /* [id][requestedit][bindable][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Appearance )( 
            IMMPlayer * This,
            /* [retval][out] */ short *pnAppearance);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FullScreen )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FullScreen )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IMMPlayer * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IMMPlayer * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IMMPlayer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IMMPlayer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IMMPlayer * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentPosition )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentPosition )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Volume )( 
            IMMPlayer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Volume )( 
            IMMPlayer * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoFrame )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_VideoFrame )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RendererType )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RendererType )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Balance )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Balance )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *length);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mute )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Mute )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *get_State )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *state);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *put_id )( 
            IMMPlayer * This,
            /* [in] */ CHAR *id);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *put_net )( 
            IMMPlayer * This,
            /* [in] */ CHAR *inNet);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileLength )( 
            IMMPlayer * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FileLength )( 
            IMMPlayer * This,
            /* [in] */ LONGLONG newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowAd )( 
            IMMPlayer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HideAd )( 
            IMMPlayer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNetSource )( 
            IMMPlayer * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CanConnect )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *bCanConn);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RendererModule )( 
            IMMPlayer * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RendererModule )( 
            IMMPlayer * This,
            /* [in] */ SHORT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_hue )( 
            IMMPlayer * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_hue )( 
            IMMPlayer * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_gama )( 
            IMMPlayer * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_gama )( 
            IMMPlayer * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_contrast )( 
            IMMPlayer * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_contrast )( 
            IMMPlayer * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_lightness )( 
            IMMPlayer * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_lightness )( 
            IMMPlayer * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_saturation )( 
            IMMPlayer * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_saturation )( 
            IMMPlayer * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSourceSize )( 
            IMMPlayer * This,
            LONG *cx,
            LONG *cy);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetHWND )( 
            IMMPlayer * This,
            LONG *hWnd);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_bMini )( 
            IMMPlayer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_bMini )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Max )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DownNO )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PlayNO )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TimeStr )( 
            IMMPlayer * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enable )( 
            IMMPlayer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_tsuyoku )( 
            IMMPlayer * This,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_tsuyoku )( 
            IMMPlayer * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPic )( 
            IMMPlayer * This,
            /* [in] */ BSTR SafePath,
            /* [in] */ BSTR StrFormat);
        
        END_INTERFACE
    } IMMPlayerVtbl;

    interface IMMPlayer
    {
        CONST_VTBL struct IMMPlayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMMPlayer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMMPlayer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMMPlayer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMMPlayer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMMPlayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMMPlayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMMPlayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMMPlayer_put_BorderStyle(This,style)	\
    ( (This)->lpVtbl -> put_BorderStyle(This,style) ) 

#define IMMPlayer_get_BorderStyle(This,pstyle)	\
    ( (This)->lpVtbl -> get_BorderStyle(This,pstyle) ) 

#define IMMPlayer_put_Appearance(This,nAppearance)	\
    ( (This)->lpVtbl -> put_Appearance(This,nAppearance) ) 

#define IMMPlayer_get_Appearance(This,pnAppearance)	\
    ( (This)->lpVtbl -> get_Appearance(This,pnAppearance) ) 

#define IMMPlayer_get_FullScreen(This,pVal)	\
    ( (This)->lpVtbl -> get_FullScreen(This,pVal) ) 

#define IMMPlayer_put_FullScreen(This,newVal)	\
    ( (This)->lpVtbl -> put_FullScreen(This,newVal) ) 

#define IMMPlayer_get_URL(This,pVal)	\
    ( (This)->lpVtbl -> get_URL(This,pVal) ) 

#define IMMPlayer_put_URL(This,newVal)	\
    ( (This)->lpVtbl -> put_URL(This,newVal) ) 

#define IMMPlayer_Play(This)	\
    ( (This)->lpVtbl -> Play(This) ) 

#define IMMPlayer_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define IMMPlayer_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IMMPlayer_get_CurrentPosition(This,pVal)	\
    ( (This)->lpVtbl -> get_CurrentPosition(This,pVal) ) 

#define IMMPlayer_put_CurrentPosition(This,newVal)	\
    ( (This)->lpVtbl -> put_CurrentPosition(This,newVal) ) 

#define IMMPlayer_get_Volume(This,pVal)	\
    ( (This)->lpVtbl -> get_Volume(This,pVal) ) 

#define IMMPlayer_put_Volume(This,newVal)	\
    ( (This)->lpVtbl -> put_Volume(This,newVal) ) 

#define IMMPlayer_get_VideoFrame(This,pVal)	\
    ( (This)->lpVtbl -> get_VideoFrame(This,pVal) ) 

#define IMMPlayer_put_VideoFrame(This,newVal)	\
    ( (This)->lpVtbl -> put_VideoFrame(This,newVal) ) 

#define IMMPlayer_get_RendererType(This,pVal)	\
    ( (This)->lpVtbl -> get_RendererType(This,pVal) ) 

#define IMMPlayer_put_RendererType(This,newVal)	\
    ( (This)->lpVtbl -> put_RendererType(This,newVal) ) 

#define IMMPlayer_get_Balance(This,pVal)	\
    ( (This)->lpVtbl -> get_Balance(This,pVal) ) 

#define IMMPlayer_put_Balance(This,newVal)	\
    ( (This)->lpVtbl -> put_Balance(This,newVal) ) 

#define IMMPlayer_get_Duration(This,length)	\
    ( (This)->lpVtbl -> get_Duration(This,length) ) 

#define IMMPlayer_get_Mute(This,pVal)	\
    ( (This)->lpVtbl -> get_Mute(This,pVal) ) 

#define IMMPlayer_put_Mute(This,newVal)	\
    ( (This)->lpVtbl -> put_Mute(This,newVal) ) 

#define IMMPlayer_get_State(This,state)	\
    ( (This)->lpVtbl -> get_State(This,state) ) 

#define IMMPlayer_put_id(This,id)	\
    ( (This)->lpVtbl -> put_id(This,id) ) 

#define IMMPlayer_put_net(This,inNet)	\
    ( (This)->lpVtbl -> put_net(This,inNet) ) 

#define IMMPlayer_get_FileLength(This,pVal)	\
    ( (This)->lpVtbl -> get_FileLength(This,pVal) ) 

#define IMMPlayer_put_FileLength(This,newVal)	\
    ( (This)->lpVtbl -> put_FileLength(This,newVal) ) 

#define IMMPlayer_ShowAd(This)	\
    ( (This)->lpVtbl -> ShowAd(This) ) 

#define IMMPlayer_HideAd(This)	\
    ( (This)->lpVtbl -> HideAd(This) ) 

#define IMMPlayer_CreateNetSource(This)	\
    ( (This)->lpVtbl -> CreateNetSource(This) ) 

#define IMMPlayer_CanConnect(This,bCanConn)	\
    ( (This)->lpVtbl -> CanConnect(This,bCanConn) ) 

#define IMMPlayer_get_RendererModule(This,pVal)	\
    ( (This)->lpVtbl -> get_RendererModule(This,pVal) ) 

#define IMMPlayer_put_RendererModule(This,newVal)	\
    ( (This)->lpVtbl -> put_RendererModule(This,newVal) ) 

#define IMMPlayer_get_hue(This,pVal)	\
    ( (This)->lpVtbl -> get_hue(This,pVal) ) 

#define IMMPlayer_put_hue(This,newVal)	\
    ( (This)->lpVtbl -> put_hue(This,newVal) ) 

#define IMMPlayer_get_gama(This,pVal)	\
    ( (This)->lpVtbl -> get_gama(This,pVal) ) 

#define IMMPlayer_put_gama(This,newVal)	\
    ( (This)->lpVtbl -> put_gama(This,newVal) ) 

#define IMMPlayer_get_contrast(This,pVal)	\
    ( (This)->lpVtbl -> get_contrast(This,pVal) ) 

#define IMMPlayer_put_contrast(This,newVal)	\
    ( (This)->lpVtbl -> put_contrast(This,newVal) ) 

#define IMMPlayer_get_lightness(This,pVal)	\
    ( (This)->lpVtbl -> get_lightness(This,pVal) ) 

#define IMMPlayer_put_lightness(This,newVal)	\
    ( (This)->lpVtbl -> put_lightness(This,newVal) ) 

#define IMMPlayer_get_saturation(This,pVal)	\
    ( (This)->lpVtbl -> get_saturation(This,pVal) ) 

#define IMMPlayer_put_saturation(This,newVal)	\
    ( (This)->lpVtbl -> put_saturation(This,newVal) ) 

#define IMMPlayer_GetSourceSize(This,cx,cy)	\
    ( (This)->lpVtbl -> GetSourceSize(This,cx,cy) ) 

#define IMMPlayer_GetHWND(This,hWnd)	\
    ( (This)->lpVtbl -> GetHWND(This,hWnd) ) 

#define IMMPlayer_get_bMini(This,pVal)	\
    ( (This)->lpVtbl -> get_bMini(This,pVal) ) 

#define IMMPlayer_put_bMini(This,newVal)	\
    ( (This)->lpVtbl -> put_bMini(This,newVal) ) 

#define IMMPlayer_put_Max(This,newVal)	\
    ( (This)->lpVtbl -> put_Max(This,newVal) ) 

#define IMMPlayer_put_DownNO(This,newVal)	\
    ( (This)->lpVtbl -> put_DownNO(This,newVal) ) 

#define IMMPlayer_put_PlayNO(This,newVal)	\
    ( (This)->lpVtbl -> put_PlayNO(This,newVal) ) 

#define IMMPlayer_put_TimeStr(This,newVal)	\
    ( (This)->lpVtbl -> put_TimeStr(This,newVal) ) 

#define IMMPlayer_put_Enable(This,newVal)	\
    ( (This)->lpVtbl -> put_Enable(This,newVal) ) 

#define IMMPlayer_get_tsuyoku(This,pVal)	\
    ( (This)->lpVtbl -> get_tsuyoku(This,pVal) ) 

#define IMMPlayer_put_tsuyoku(This,newVal)	\
    ( (This)->lpVtbl -> put_tsuyoku(This,newVal) ) 

#define IMMPlayer_GetPic(This,SafePath,StrFormat)	\
    ( (This)->lpVtbl -> GetPic(This,SafePath,StrFormat) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMMPlayer_INTERFACE_DEFINED__ */



#ifndef __MPlayerLib_LIBRARY_DEFINED__
#define __MPlayerLib_LIBRARY_DEFINED__

/* library MPlayerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MPlayerLib;

#ifndef ___IMMPlayerEvents_DISPINTERFACE_DEFINED__
#define ___IMMPlayerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IMMPlayerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IMMPlayerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("420F66C6-0B80-4F53-AB96-7CA6CA57DB81")
    _IMMPlayerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IMMPlayerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IMMPlayerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IMMPlayerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IMMPlayerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IMMPlayerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IMMPlayerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IMMPlayerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IMMPlayerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IMMPlayerEventsVtbl;

    interface _IMMPlayerEvents
    {
        CONST_VTBL struct _IMMPlayerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IMMPlayerEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IMMPlayerEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IMMPlayerEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IMMPlayerEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IMMPlayerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IMMPlayerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IMMPlayerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IMMPlayerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_MMPlayer;

#ifdef __cplusplus

class DECLSPEC_UUID("F3D03D98-E1F1-4F78-A496-E22BE0A4BF83")
MMPlayer;
#endif
#endif /* __MPlayerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


