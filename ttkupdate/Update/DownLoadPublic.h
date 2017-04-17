#ifndef __DOWNLOADPUBLIC__H__
#define __DOWNLOADPUBLIC__H__
#pragma once
#include <Afxmt.h>
#include <afxtempl.h>
#include "SocketClient.h"


//缺省的重试次数
const UINT DEFAULT_RETRY_MAX	= 10; 

//线程下载 超时  结束下载
#define SLEEP_RETURN_Down(x)             {if ( ::WaitForSingleObject ( m_hEvtEndModule, x ) == WAIT_OBJECT_0 ) return DownloadEnd(FALSE);}


enum
{
	NOTIFY_TYPE_GOT_REMOTE_FILENAME,				// 取得远程站点文件名, 当被下载的文件被重定向时才发送该通知，lpNotifyData 为 LPCTSTR 类型的文件名字符串指针
	NOTIFY_TYPE_GOT_REMOTE_FILESIZE,				// 取得远程站点文件大小，lpNotifyData 为 int 类型的文件大小
	NOTIFY_TYPE_START_DOWNLOAD,						// 开始下载，lpNotifyData 无用
	NOTIFY_TYPE_END_DOWNLOAD,						// 结束下载，lpNotifyData 为 ENUM_DOWNLOAD_RESULT 类型的下载结果
	NOTIFY_TYPE_WILL_DOWNLOAD_SIZE,					// 本次需要下载的大小，lpNotifyData 为 int 类型的需要下载的大小
	NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE,				// 某线程已下载的数据大小，lpNotifyData 为 int 类型的已下载的大小
};


typedef struct tagDownloadNotifyPara     //定义下载通知参数
{
	int nIndex;
	UINT nNotityType;
	LPVOID lpNotifyData;
	LPVOID pDownloadMTR;
} DownloadNotifyPara;

typedef CArray<DownloadNotifyPara,DownloadNotifyPara&> g_ArrayDownloadNotifyPara;

// 消息通知回调函数
typedef void (*FUNC_DownloadNotify) ( DownloadNotifyPara *pDownloadNotifyPara, WPARAM wParam );
typedef void (*FUNC_SaveDownloadInfo) ( int nIndex, int nDownloadedSize, int nSimpleSaveSize, WPARAM wParam );

// 缺省端口号
#define DEFAULT_HTTP_PORT			80
#define DEFAULT_HTTPS_PORT			443
#define DEFAULT_FTP_PORT			21
#define DEFAULT_SOCKS_PORT			6815

typedef enum _DownloadResult   //下载结果
{
	ENUM_DOWNLOAD_RESULT_SUCCESS,
	ENUM_DOWNLOAD_RESULT_FAILED,
	ENUM_DOWNLOAD_RESULT_CANCEL,
} ENUM_DOWNLOAD_RESULT;


class CDownLoadPublic
{
public:
	CDownLoadPublic(void);
	~CDownLoadPublic(void);

	BOOL ThreadIsRunning();				//线程是否运行
	CString GetDownloadObjectFileName(CString *pstrExtensionName=NULL);  //得到下载对象文件名
	void ClearThreadHandle();					//清除线程句柄
	void ResetVar();							//重新设置Var
	int GetUndownloadBytes();					//得到未下载字节
	BOOL ThreadProcDownload();						//线程下载
	BOOL SetSaveFileName(LPCTSTR lpszSaveFileName);		//设置保存径路名
	int GetWillDownloadSize();							//得到将要下载字节
	void SetWillDownloadSize(int nWillDownloadSize);		//设置将要下载字节
	int GetDownloadedSize();								//得到下载大小
	void SetDownloadedSize(int nDownloadedSize);			//设置下载大小
	int GetTempSaveBytes();									//得到预保存字节
	void SetTempSaveBytes(int nTempSaveBytes);				//设置预保存字节
	CString GetRemoteFileName();							//得到远程文件名
	BOOL SetDownloadUrl(LPCTSTR lpszDownloadUrl);			//设置下载URL
	virtual BOOL Connect();									//是否连接
	BOOL GetRemoteSiteInfo();								//得到远程站点信息

	void SetAuthorization (LPCTSTR lpszUsername, LPCTSTR lpszPassword);		//设置授权
	void SetReferer(LPCTSTR lpszReferer);										//设置参考
	void SetUserAgent(LPCTSTR lpszUserAgent);									//设置用户代理
	void SetSaveDownloadInfoCallback(FUNC_SaveDownloadInfo Proc_SaveDownloadInfo, WPARAM wParam);  //设置保存对象信息回调函数
	virtual BOOL Download(				//下载
		int nWillDownloadStartPos,
		int nWillDownloadSize,
		int nDownloadedSize
		);
	BOOL IsSupportResume() ;   //是否支持恢复
	CString GetProtocolType() ;  //协议
	time_t GetTimeLastModified() ;   //得到最后修改时间
	int GetFileTotalSize() ;		//得到文件总大小
	CString GetUserName() ;			//得到用户名
	CString GetPassword() ;				//得到密码
	CString GetDownloadUrl() ;		//得到URL
	BOOL IsDownloadSuccess() ;		//下载是否成功
	HANDLE GetThreadHandle() ;				//得到线程句柄	
	int GetWillDownloadStartPos() ;	//得到将要开始下载位置
	CString GetServerName() ;				//得到服务器名
	void StopDownload();											//停止下载

	virtual BOOL GetRemoteSiteInfoPro();							//得到远程站点信息
	virtual BOOL DownloadOnce();									//是否曾经下载
	CString GetRefererFromURL();									//通过URL得到参考
	int SaveDataToFile(char *pData, int nSize);						//保存数据到文件
	virtual BOOL RecvDataAndSaveToFile(CSocketClient &SocketClient,char* szTailData=NULL, int nTailSize=0); //接受数据和保存到文件
	BOOL DownloadEnd(BOOL bRes);	

	BOOL OpenFileForSave();  //为保存打开文件

public:
	LPVOID m_pDownloadMTR;
	int m_nIndex;

public:
	CFile m_file;

	// 待下载URL
	CString		m_strDownloadUrl;
	CString		m_strSaveFileName;

	// 保存下载信息的回调函数
	FUNC_SaveDownloadInfo m_funcSaveDownloadInfo;
	WPARAM m_wSaveDownloadInfo_Param;

	// 是否支持断点续传
	BOOL		m_bSupportResume;
	HANDLE		m_hThread;

	// 文件以及下载大小
	int					m_nFileTotalSize;				// 文件总的大小，-1表示未知文件大小
	int					m_nWillDownloadStartPos;		// 要下载文件的开始位置
	int					m_nWillDownloadSize;			// 本次需要下载的大小，-1表示不知道文件大小，所以下载到服务器关闭连接为止
	CCriticalSection	m_csWillDownloadSize;		// 访问 m_nWillDownloadSize 变量的互斥锁
	int					m_nTempSaveBytes;				// 存放在临时缓冲中的字节数
	CCriticalSection	m_csTempSaveBytes;			// 访问 m_nTempSaveBytes 变量的互斥锁
	int					m_nDownloadedSize;				// 已下载的字节数，指完全写到文件中的字节数，不包含临时缓冲里的数据
	CCriticalSection	m_csDownloadedSize;			// 访问 m_nDownloadedSize 变量的互斥锁

	// 文件日期(远程文件的信息)
	CTime		m_TimeLastModified;

	// Referer
	CString		m_strReferer;
	CString		m_strCookieFlag;

	// UserAgent
	CString		m_strUserAgent;

	// 是否进行验证 : Request-Header: Authorization
	CString		m_strUsername;
	CString		m_strPassword;

	// 下载过程中所用的变量
	CString		m_strProtocolType;	// 所使用的传输协议：http、ftp等
	CString		m_strServer;
	CString		m_strObject;
	CString		m_strFileName;
	USHORT		m_nPort;

	BOOL m_bDownloadSuccess;

public:
	// 模块结束事件
	HANDLE m_hEvtEndModule;
	// 连接服务器的 Socket
	CSocketClient m_SocketClient;
};

int Base64Encode(LPCTSTR lpszEncoding, CString &strEncoded);   //基本64编码
int Base64Decode(LPCTSTR lpszDecoding, CString &strDecoded);	//基地64解码
BOOL ParseTrueURL(LPCTSTR lpszURL,CString& strServer,CString& strObject,USHORT& nPort, CString &csProtocolType);		//解析URL
void Set_DownloadNotify_Callback(FUNC_DownloadNotify Proc_DownloadNotify, WPARAM wParam);				//设置下载通知
void SetRetryTimes(int nRetryTimes);											//设置重试时间

#endif