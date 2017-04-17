#ifndef __DOWNLOADHTTP__H__
#define __DOWNLOADHTTP__H__
#pragma once
#include "downloadpublic.h"

class CDownLoadHTTP :	//HTTP 协议下载类
	public CDownLoadPublic
{
public:
	CDownLoadHTTP(void);
	~CDownLoadHTTP(void);


public:
	BOOL RequestHttpData(BOOL bGet, char *szTailData=NULL, int *pnTailSize=NULL);	//请求HTTP数据
	CTime ConvertHttpTimeString(CString strTimeGMT );								//转换http 时间为字符
	CString FindAfterFlagString(LPCTSTR lpszFoundStr, CString strOrg);				//发现 后标致字符
	BOOL ParseResponseString(CString strResponseString, OUT BOOL &bRetryRequest);	//解析响应字符
	DWORD GetResponseCode(CString strLineText );									//得到响应代码
	BOOL SendRequest(LPCTSTR lpszReq, OUT CString &strResponse, char *szTailData=NULL, int *pnTailSize=NULL);		//发送请求
	CString GetRequestStr(BOOL bGet);					//得到请求字符

	virtual BOOL DownloadOnce();				//是否曾经下载
	virtual BOOL GetRemoteSiteInfoPro();			//得到远程站点信息



};

#endif