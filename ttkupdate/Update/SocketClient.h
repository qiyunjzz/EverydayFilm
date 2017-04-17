#ifndef __CSOCKETCLIENT__H__
#define __CSOCKETCLIENT__H__
#pragma once
#include "afxsock.h"

#define NET_BUFFER_SIZE			2048		// 默认缓冲大小


class CSocketClient :
	public CSocket
{
public:
	CSocketClient(void);
	~CSocketClient(void);

	BOOL IsConnected();  //是否连接
	CString GetDigitStrAtHead ( LPCTSTR lpszStr );		//得到数字报头
	int GetResponse ( CString *pcsResponseStr=NULL, BOOL bBlock=TRUE );   //得到响应
	BOOL GetResponse ( int nVerifyCode, CString *pcsResponseStr=NULL );
	CString GetResponseHistoryString();							//等到历史响应
	void ClearResponseHistoryString();							//清楚历史响应

public:
	BOOL Connect ( LPCTSTR lpszHost, USHORT nPort );			//连接
	void Disconnect();								//结束连接
	int Receive (char *pBuf, int nSize, BOOL bBlock=TRUE );		//接受数据
	BOOL GetIPAndPortByPasvString(LPCTSTR lpszPasvString, OUT CString &csIP, OUT USHORT &nPort );	//得到IP和端口
	void SetEventOfEndModule ( HANDLE hEvtEndModule );			//设置结束事件
	BOOL Send ( char *pData, int nSize );						//发送数据
	BOOL SendString(LPCTSTR lpszData, ... );					//发送


public:
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);		//连接帮助
	BOOL PumpMessagesMy(UINT uStopFlag);	//我的消息表


private:
	CString m_strResponseHistoryString;		//历史响应
	HANDLE m_hEvtEndModule;					//事件句柄
	BOOL m_bConnected;						//连接判断

public:
	int m_nIndex;
};
#endif