#ifndef __DOWNLOADFTP__H__
#define __DOWNLOADFTP__H__
#pragma once
#include "DownLoadPublic.h"

class CDownLoadFTP :
	public CDownLoadPublic
{
public:
	CDownLoadFTP(void);
	~CDownLoadFTP(void);

public:
	BOOL Connect ();			//连接
	void GetFileTimeInfo(CString strFileTime1, CString strFileTime2, CString strFileTime3);			//得到文件时间信息
	BOOL CreateFTPDataConnect(CSocketClient &SocketClient);					//建立FTP协议数据连接
	void ParseFileInfoStr(CString &strFileInfoStr);						//分析文件信息
	virtual BOOL GetRemoteSiteInfoPro();					//得到远程站点信息服务
	virtual BOOL DownloadOnce();							//是否曾经下载
};

#endif