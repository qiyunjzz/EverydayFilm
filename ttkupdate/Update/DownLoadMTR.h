#ifndef __DOWNLOADMTR__H__
#define __DOWNLOADMTR__H__  
#pragma once // MTR - MultiThread Resume : 多线程断点续传
#include "DownloadFtp.h"
#include "DownloadHttp.h"
#include <Afxmt.h>


#define NOTE_DownloadMTR  _T("FTP HTTP 开始下载")

#define DEFAULT_THREAD_COUNT		1			// 默认下载一个文件所用的线程数
#define MAX_DOWNLOAD_THREAD_COUNT	16			// 下载一个文件最大使用的线程数

// 单个对象的下载信息
typedef struct tagDownLoadCellInfo
{
	int nWillDownloadStartPos;			// 要下载文件的开始位置
	int nWillDownloadSize;				// 本次需要下载的大小，-1表示一直下载到文件尾
	DWORD nDownloadedSize;				// 该线程已下载的大小
}DownLoadCellInfo;

// 下载信息
typedef struct tagBaseDownInfo
{
	DWORD dwThreadCount;				// 该文件由多少个线程在下载
}BaseDownInfo;


class CDownLoadMTR
{
public:
	CDownLoadMTR(void);
	~CDownLoadMTR(void);

	int GetTotalDownloadedSize_ThisTimes();			//当前时间 得到下载总大小
	int GetTotalDownloadedSize();				//得到下载总大小
	int GetFileTotaleSize();					//得到文件总大小
	static CString GetLocalFileNameByURL(LPCTSTR lpszDownloadURL);		//通过url得到本地文件名
	void StopDownload();						//停止下载
	DWORD GetDownloadElapsedTime();				//下载过去时间
	BOOL StartDownload();					    // 模拟断点续传
	BOOL Download (								//下载
		LPCTSTR lpszDownloadURL,
		LPCTSTR lpszSavePath,
		LPCTSTR lpszSaveOnlyFileName,
		LPCTSTR lpszUsername=NULL,
		LPCTSTR lpszPassword=NULL,
		BOOL bForceDownload=FALSE
		);
	BOOL SetThreadCount (int nThreadCount);					//设置线程个数

	void CallbackSaveDownloadInfo(int nIndex, int nDownloadedSize, int nSimpleSaveSize);  //调用保存下载信息
	BOOL ThreadProcDownloadMTR();				//线程下载

public:

	void StandardSaveFileName();					//标准保存文件名
	int GetDownloadInfoWholeSize();					//得到下载信息大小
	int FindIndexByThreadHandle(HANDLE hThread);     //通过线程找到序号
	ENUM_DOWNLOAD_RESULT WaitForDownloadFinished();		//等待下载完成
	BOOL GetDownloadResult();				//得到下载结果
	BOOL AttemperDownloadTask(int nIndex);  //调度下载任务
	int GetUndownloadMaxBytes(int &nUndownloadBytes);			//非断点下载最大字节
	BOOL HandleDownloadFinished (ENUM_DOWNLOAD_RESULT eDownloadResult);		//处理下载完成
	BOOL SaveDownloadInfo();				//保存下载信息
	BOOL AssignDownloadTask();				//分配下载任务
	BOOL DownloadInfoIsValid();				//下载信息是有效的
	BOOL ReadDownloadInfo();				//读取下载信息
	CString GetTempFilePath();				//得到文件路径
	int StartMTRDownload();					//开始 MTR 下载
	void DeleteDownloadObjectAndDataMTR();		//删除下载对象和MTR数据
	void DeleteDownloadObjectInfo();				//删除下载对象信息
	BOOL CreateDownloadObjectAndDataMTR();				//建立下载对象和MTR数据
	CDownLoadPublic* CreateDownloadObject(int nCount=1);		//建立下载对象
	void DeleteDownloadObject(CDownLoadPublic *pDownloadPub);		//删除下载对象

public:

	CString m_strSavePath, m_strSaveOnlyFileName, m_strSavePathFileName, m_strDownloadURL;	//下载路劲 下载文件名 保存路径名 下载URL
	CString m_strProtocolType;				//协议类型
	int m_nThreadCount;							// 线程数
	CDownLoadPublic *m_pDownloadPub_Info;			// 取站点信息对象
	CDownLoadPublic	*m_pDownloadPub_MTR;			// 多线程下载对象
	BaseDownInfo	m_BaseDownInfo;				// 下载基本信息，线程数等
	DownLoadCellInfo *m_pDownloadCellInfo;	// 各个下载对象的参数
	HANDLE m_hEvtEndModule;			// 模块结束事件

	int m_nTotalDownloadedSize_ThisTimes;		// 表示这次启动下载任务以来总共下载的字节数
	CCriticalSection m_csDownloadedData;			//临界区

	BOOL m_bForceDownload;							//是否重新下载？
	HANDLE m_hThread;								//线程句柄
	DWORD m_dwDownloadStartTime;					//下载开始时间

	BOOL m_bFileExist,m_bDownStart,m_bDownError;
};

#endif