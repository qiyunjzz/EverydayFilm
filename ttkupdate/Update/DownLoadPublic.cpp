#include "StdAfx.h"
#include "DownLoadPublic.h"
#include "PublicFunc.h"
#include "DownLoadMTR.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

// 下载数据保存的临时缓冲大小
#define TEMP_SAVE_BUFFER_SIZE		(10*NET_BUFFER_SIZE)
// 当下载的数据达到这个数的时候才保存到文件中
#define WRITE_TEMP_SAVE_MIN_BYTES	(TEMP_SAVE_BUFFER_SIZE - 2*NET_BUFFER_SIZE)
void DownloadNotify ( int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR );
// 重试多少次

int g_nRetryTimes = 100;

void SetRetryTimes (int nRetryTimes)
{	g_nRetryTimes = nRetryTimes;}


// 消息通知的回调函数
FUNC_DownloadNotify g_funcDownloadNotify = NULL;
WPARAM g_wDownloadNotify_Param = NULL;

const TCHAR* g_szBase64TAB = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

CDownLoadPublic::CDownLoadPublic(void)
{
	m_hThread					=		NULL;
	m_TimeLastModified			=		-1;
	m_strDownloadUrl			=		_T("");
	m_strSaveFileName			=		_T("");

	m_funcSaveDownloadInfo		=		NULL;
	m_wSaveDownloadInfo_Param	=		NULL;

	m_bSupportResume			=		FALSE;
	m_nFileTotalSize			=		-1;
	m_strReferer				=		_T("");
	m_strUserAgent				=		_T("HttpDown/2.0");
	m_strUsername				=		_T("");
	m_strPassword				=		_T("");
	m_strProtocolType			=		"http";
	m_strServer					=		_T("");
	m_strObject					=		_T("");
	m_strFileName				=		_T("");
	m_nPort						=		DEFAULT_HTTP_PORT ;
	m_nIndex					=		-1;
	m_strCookieFlag				=		_T("");
	m_hEvtEndModule				=		::CreateEvent ( NULL, TRUE, FALSE, NULL );
	m_pDownloadMTR				=		NULL;
	ResetVar();
}

CDownLoadPublic::~CDownLoadPublic(void)
{
	StopDownload();
	ClearThreadHandle();
}

void CDownLoadPublic::ResetVar()
{
	ClearThreadHandle();
	m_nWillDownloadStartPos		=		0;
	m_nWillDownloadSize			=		-1;
	m_nDownloadedSize			=		0;
	m_nTempSaveBytes			=		0;
	m_bDownloadSuccess			=		FALSE;
}

void CDownLoadPublic::StopDownload ()
{
	m_SocketClient.CancelBlockingCall();
	if (HANDLE_IS_VALID(m_hEvtEndModule))
	{
		::SetEvent(m_hEvtEndModule);
		WaitForThreadEnd(m_hThread,30*1000);
		CLOSE_HANDLE(m_hEvtEndModule);
		m_hEvtEndModule = NULL;
		ClearThreadHandle();
	}
}

// 设置认证信息
void CDownLoadPublic::SetAuthorization ( LPCTSTR lpszUsername, LPCTSTR lpszPassword )
{
	if( lpszUsername != NULL )
	{
		m_strUsername	= GET_SAFE_STRING(lpszUsername);
		m_strPassword	= GET_SAFE_STRING(lpszPassword);
	}
	else
	{
		m_strUsername	= _T("");
		m_strPassword	= _T("");
	}
}

// 设置Referer
void CDownLoadPublic::SetReferer(LPCTSTR lpszReferer)
{
	if(lpszReferer != NULL)
		m_strReferer = lpszReferer;
	else
		m_strReferer = _T("");
}

// 设置UserAgent
void CDownLoadPublic::SetUserAgent(LPCTSTR lpszUserAgent)
{
	m_strUserAgent = lpszUserAgent;	
	if(m_strUserAgent.IsEmpty())
		m_strUserAgent = _T("HttpDown/2.0");
}


// 设置保存下载信息回调函数
void CDownLoadPublic::SetSaveDownloadInfoCallback(FUNC_SaveDownloadInfo funcSaveDownloadInfo, WPARAM wParam)
{
	m_funcSaveDownloadInfo = funcSaveDownloadInfo;
	m_wSaveDownloadInfo_Param = wParam;
}

// 下载任务的线程函数
DWORD WINAPI ThreadProcDownload(LPVOID lpParameter)  // thread data								 
{
	CDownLoadPublic *pDownloadPub = (CDownLoadPublic*)lpParameter;
	ASSERT(pDownloadPub);
	return pDownloadPub->ThreadProcDownload();
}

BOOL CDownLoadPublic::ThreadProcDownload()
{
	BOOL bRet = FALSE;
	for (int i = 0; i < g_nRetryTimes; i++)
	{
		if (DownloadOnce())
		{
			bRet = TRUE;
			goto finished;
		}
		SLEEP_RETURN_Down(5*1000);
	}

finished:
	TRACE("(%d) 线程结束 %s", m_nIndex, bRet?"成功":"失败");

	return DownloadEnd(bRet);
}


BOOL CDownLoadPublic::DownloadOnce()
{
	// 打开文件
	if (!OpenFileForSave()) 
		return FALSE;

	// 连接到服务器
	if (!m_SocketClient.IsConnected())
	{
		if (!Connect())
			return FALSE;
	}
	return TRUE;
}


// 创建线程下载文件
BOOL CDownLoadPublic::Download (int nWillDownloadStartPos,		// 要下载文件的开始位置
							   int nWillDownloadSize,			// 本次需要下载的大小，-1表示一直下载到文件尾
							   int nDownloadedSize)				// 已下载的字节数，指完全写到文件中的字节数						 
{
	if (nWillDownloadSize == 0) return TRUE;
	// 设置下载参数
	m_nWillDownloadStartPos	= nWillDownloadStartPos;
	SetWillDownloadSize(nWillDownloadSize);
	if (m_nFileTotalSize > 0 && GetWillDownloadSize() > m_nFileTotalSize)
	{
		SetWillDownloadSize(m_nFileTotalSize);
	}
	SetDownloadedSize(nDownloadedSize);

	// 创建一个下载线程
	DWORD dwThreadId = 0;
	m_hThread = CreateThread(NULL,0,::ThreadProcDownload, LPVOID(this), 0, &dwThreadId);
	if (!HANDLE_IS_VALID(m_hThread))
	{
		TRACE(_T("(%d) Create download thread failed"),m_nIndex);
		return FALSE;
	}
	return TRUE;
}

BOOL CDownLoadPublic::IsSupportResume()  //是否支持恢复
{ 
	return m_bSupportResume;
} 

CString CDownLoadPublic::GetProtocolType()  //协议
{
	return m_strProtocolType;
}
	
time_t CDownLoadPublic::GetTimeLastModified()    //得到最后修改时间
{
	return m_TimeLastModified.GetTime();
}
	
int CDownLoadPublic::GetFileTotalSize() 			//得到文件总大小
{
	return m_nFileTotalSize;
}

CString CDownLoadPublic::GetUserName()				//得到用户名
{ 
	return m_strUsername;
}

CString CDownLoadPublic::GetPassword()				//得到密码
{
	return m_strPassword; 
}

CString CDownLoadPublic::GetDownloadUrl() 		//得到URL
{
	return m_strDownloadUrl;
}
	
BOOL CDownLoadPublic::IsDownloadSuccess()		//下载是否成功
{ 
	return m_bDownloadSuccess; 
}
	
HANDLE CDownLoadPublic::GetThreadHandle() 				//得到线程句柄	
{ 
	return m_hThread; 
}
	
int CDownLoadPublic::GetWillDownloadStartPos() 	//得到将要开始下载位置
{
	return m_nWillDownloadStartPos;
}
	
CString CDownLoadPublic::GetServerName() 					//得到服务器名
{ 
	return m_strServer; 
}

// 下载结束
BOOL CDownLoadPublic::DownloadEnd(BOOL bRes)
{
	m_bDownloadSuccess = bRes;
	m_SocketClient.Disconnect();
	if (HANDLE_IS_VALID(m_file.m_hFile))
	{
		m_file.Close();
	}
	TRACE (_T("(%d) %s 结束一次下载\n"), m_nIndex, bRes?_T("成功"):_T("!!! 失败 !!!"));

	return bRes;
}


BOOL CDownLoadPublic::Connect()
{
	if (!HANDLE_IS_VALID(m_hEvtEndModule))
		return FALSE;

	if (m_strServer.IsEmpty())
	{
		TRACE(_T("(%d) Please set download URL"), m_nIndex);
		return FALSE;
	}
	m_SocketClient.SetEventOfEndModule ( m_hEvtEndModule );
	m_SocketClient.m_nIndex = m_nIndex;
	// 连接到服务器
	if (!m_SocketClient.Connect(m_strServer, m_nPort))
		return FALSE;

	TRACE(_T("(%d) Connect to server [%s:%d] successfully!"), m_nIndex, m_strServer, m_nPort);
	return TRUE;
}

BOOL CDownLoadPublic::SetDownloadUrl(LPCTSTR lpszDownloadUrl)
{
	if ( !lpszDownloadUrl )
	{
		return FALSE;
	}
	m_strDownloadUrl = lpszDownloadUrl;
	// 检验要下载的URL是否为空
	m_strDownloadUrl.TrimLeft();
	m_strDownloadUrl.TrimRight();
	if(m_strDownloadUrl.IsEmpty())
	{
		return FALSE;
	}
	// 检验要下载的URL是否有效
	if (!ParseTrueURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort, m_strProtocolType))
	{
		TRACE(_T("Failed to parse the URL: %s\n"), m_strDownloadUrl);
		return FALSE;
	}
	return TRUE;
}

// 从服务器接收数据并保存到文件中
BOOL CDownLoadPublic::RecvDataAndSaveToFile(CSocketClient &SocketClient,char *pTailData/*=NULL*/, int nTailSize/*=0*/)
{
	int nDownloadedSize = GetDownloadedSize();
	if (pTailData && nTailSize > 0)
	{
		nDownloadedSize = SaveDataToFile(pTailData, nTailSize);
		if (nDownloadedSize < 0)
		{
			return FALSE;
		}
	}

	char szRecvBuf[NET_BUFFER_SIZE] = {0}, *szTempSaveBuf = new char[TEMP_SAVE_BUFFER_SIZE];
	if (!szTempSaveBuf)
	{
		TRACE(_T("(%d) Allocate memory failed"), m_nIndex);
		return FALSE;
	}
	m_nTempSaveBytes = 0;

	while (TRUE)
	{
		BOOL bDownloadFinished = FALSE;
		int nReadSize = 0;
		int nTempSaveBytes = GetTempSaveBytes ();
		int nRecvTotalBytes = nDownloadedSize+nTempSaveBytes;	// 保存在文件中的字节数加上临时缓冲中的字节数就是总共接收到字节数
		int nWillDownloadSize = GetWillDownloadSize ();
		if(
			// 从字节数判断，本次下载已经完成了。
			(nWillDownloadSize > 0 && nRecvTotalBytes >= nWillDownloadSize)
			||
			// 模块结束事件有信号，要结束了。
			(::WaitForSingleObject(m_hEvtEndModule, 1) == WAIT_OBJECT_0)
			)
		{
			bDownloadFinished = TRUE;
		}
		else
		{
			int nRecvBytesThisTimes = sizeof(szRecvBuf);
			if (nWillDownloadSize > 0)
				nRecvBytesThisTimes = nWillDownloadSize - nRecvTotalBytes;
			ASSERT (nRecvBytesThisTimes >= 0);
			nRecvBytesThisTimes = MIN (nRecvBytesThisTimes, sizeof(szRecvBuf));


			nReadSize = SocketClient.Receive(szRecvBuf, nRecvBytesThisTimes);
			// 读不到数据了，所以认为下载已经完成
			if (nReadSize <= 0)
			{
				if (nWillDownloadSize <= 0)
				{
					bDownloadFinished = TRUE;
				}
			}
			else
			{
								//TRACE (_T("对象.%d, 收到 %d 字节，我的任务是 %d (0x%08x)字节\n"),
								//	m_nIndex, nReadSize, nWillDownloadSize , nWillDownloadSize );	//w
								//CString str;
								//str.Format(_T("对象.%d, 收到 %d 字节，我的任务是 %d (0x%08x)字节\n"),
								//	m_nIndex, nReadSize, nWillDownloadSize , nWillDownloadSize );
								//MessageBox(NULL,str,NULL,NULL);
			}
		}

		// 先将数据保存到临时缓冲中
		if (nReadSize > 0)
		{
			nReadSize = MIN(nReadSize, TEMP_SAVE_BUFFER_SIZE-nTempSaveBytes);
			memcpy (szTempSaveBuf+nTempSaveBytes, szRecvBuf, nReadSize);
			nTempSaveBytes += nReadSize;
			ASSERT (nTempSaveBytes < TEMP_SAVE_BUFFER_SIZE);
		}
		// 当下载已完成或者收到的数据超过一定数量时才保存到文件中
		if (bDownloadFinished || nTempSaveBytes >= WRITE_TEMP_SAVE_MIN_BYTES)
		{
			// 保存文件失败，下载也应该终止
			nDownloadedSize = SaveDataToFile(szTempSaveBuf, nTempSaveBytes);
			if (nDownloadedSize < 0)
			{
				break;
			}
			nTempSaveBytes = 0;

			CDownLoadMTR* pDownLoadMtp=(CDownLoadMTR*)m_pDownloadMTR;   //保存线程下载的信息
			pDownLoadMtp->SaveDownloadInfo();
		}
		SetTempSaveBytes(nTempSaveBytes);

		if (bDownloadFinished)
		{
			ASSERT((GetWillDownloadSize () > 0 && nDownloadedSize >= GetWillDownloadSize ()) || (nReadSize <= 0 && GetWillDownloadSize () <= 0) ||
				(::WaitForSingleObject ( m_hEvtEndModule, 1 ) == WAIT_OBJECT_0));
			break;
		}
	}

	if (szTempSaveBuf) delete[] szTempSaveBuf;
	szTempSaveBuf = NULL;

	BOOL bRes = FALSE;
	int nWillDownloadSize = GetWillDownloadSize ();
	if (nWillDownloadSize != -1)
	{
		if (nDownloadedSize >= nWillDownloadSize)
		{
			bRes = TRUE;
		}
	}
	else if (nDownloadedSize > 0)
	{
		bRes = TRUE;
	}
	return bRes;
}


int CDownLoadPublic::SaveDataToFile(char *pData, int nSize)
{
	ASSERT ( HANDLE_IS_VALID ( m_file.m_hFile ) );
	if ( !pData || nSize < 0 ) return -1;

	int nDownloadedSize = -1;
	// 保存下载的数据
	ASSERT (HANDLE_IS_VALID(m_file.m_hFile));
	BOOL bRet = TRUE;
	TRY
	{
		m_file.Write (pData, nSize);
	}
	CATCH(CFileException, e)
	{
		e->Delete ();
		bRet = FALSE;
	}
	END_CATCH

	if (!bRet)
	{
		TRACE(_T("(%d) Write data to local file [%s] failed"), m_nIndex, m_file.GetFileName());
	}
	else
	{
		nDownloadedSize = GetDownloadedSize();
		//		TRACE ( "对象.%d, 保存 %d(0x%08x) 字节，偏移 %d(0x%08x) 字节， 共 %d(0x%08x) 字节\n",
		//			m_nIndex, size, size, m_nWillDownloadStartPos+nDownloadedSize, m_nWillDownloadStartPos+nDownloadedSize,
		//			nDownloadedSize + size, nDownloadedSize + size );	//w
		nDownloadedSize += nSize;
		SetDownloadedSize(nDownloadedSize);
		if (m_funcSaveDownloadInfo)
			m_funcSaveDownloadInfo(m_nIndex, nDownloadedSize, nSize, m_wSaveDownloadInfo_Param);
	}
	if (!bRet) return -1;
	return nDownloadedSize;
}

BOOL CDownLoadPublic::GetRemoteSiteInfoPro()
{
	return TRUE;
}

BOOL CDownLoadPublic::GetRemoteSiteInfo ()
{
	for (int i = 0; i < g_nRetryTimes; i++)
	{
		if (GetRemoteSiteInfoPro())
		{
			m_SocketClient.Disconnect();
			return TRUE;
		}
		SLEEP_RETURN_Down (5*1000);
	}
	m_SocketClient.Disconnect();
	return FALSE;
}

CString CDownLoadPublic::GetRemoteFileName()
{
	int nPos = m_strObject.ReverseFind ('/');
	if ( nPos <= 0) return m_strObject;
	return m_strObject.Mid(nPos+1);
}

void CDownLoadPublic::SetDownloadedSize(int nDownloadedSize)
{
	m_csDownloadedSize.Lock();
	m_nDownloadedSize = nDownloadedSize;
	m_csDownloadedSize.Unlock();
	DownloadNotify(m_nIndex,NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE,(LPVOID)nDownloadedSize, m_pDownloadMTR);
}

int CDownLoadPublic::GetDownloadedSize()
{
	int nDownloadedSize = 0;
	m_csDownloadedSize.Lock ();
	nDownloadedSize = m_nDownloadedSize;
	m_csDownloadedSize.Unlock ();

	return nDownloadedSize;
}

void CDownLoadPublic::SetTempSaveBytes(int nTempSaveBytes)
{
	m_csTempSaveBytes.Lock ();
	m_nTempSaveBytes = nTempSaveBytes;
	m_csTempSaveBytes.Unlock ();
}

int CDownLoadPublic::GetTempSaveBytes()
{
	int nTempSaveBytes = 0;
	m_csTempSaveBytes.Lock ();
	nTempSaveBytes = m_nTempSaveBytes;
	m_csTempSaveBytes.Unlock ();

	return nTempSaveBytes;
}

void CDownLoadPublic::SetWillDownloadSize(int nWillDownloadSize)
{
	m_csWillDownloadSize.Lock();
	m_nWillDownloadSize = nWillDownloadSize;
	m_csWillDownloadSize.Unlock();
	DownloadNotify(m_nIndex, NOTIFY_TYPE_WILL_DOWNLOAD_SIZE, (LPVOID)m_nWillDownloadSize, m_pDownloadMTR);
}

int CDownLoadPublic::GetWillDownloadSize()
{
	int nWillDownloadSize = 0;
	m_csWillDownloadSize.Lock ();
	nWillDownloadSize = m_nWillDownloadSize;
	m_csWillDownloadSize.Unlock ();
	return nWillDownloadSize;
}

BOOL CDownLoadPublic::SetSaveFileName(LPCTSTR lpszSaveFileName)
{
	if (!lpszSaveFileName) return FALSE;
	m_strSaveFileName = lpszSaveFileName;
	if (m_strSaveFileName.IsEmpty())
		return FALSE;
	return TRUE;
}

//
// 获取尚未下载的字节数，写到文件中的和临时缓冲里的都算是已经下载的
//
int CDownLoadPublic::GetUndownloadBytes()
{
	TRACE(_T("%d  %d  %d"),GetWillDownloadSize(),GetDownloadedSize(),GetTempSaveBytes());
	// 总共需要下载的字节数减去已经下载的字节数
	return GetWillDownloadSize()-(GetDownloadedSize()+GetTempSaveBytes());
}

BOOL CDownLoadPublic::OpenFileForSave()
{
	ASSERT (!m_strSaveFileName.IsEmpty());
	// 打开本地文件
	if (HANDLE_IS_VALID(m_file.m_hFile))
	{
		m_file.Close();
	}
	BOOL bRet = FALSE;
	TRY
	{
		if (m_file.Open(m_strSaveFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone))
		{
			int nWillDownloadStartPos = GetWillDownloadStartPos();
			nWillDownloadStartPos += GetDownloadedSize();
			if (m_file.Seek (nWillDownloadStartPos, CFile::begin) == nWillDownloadStartPos)
				bRet = TRUE;
		}
	}
	CATCH(CFileException, e)
	{
		e->Delete();
		bRet = FALSE;
	}
	END_CATCH

	if ( !bRet )
	{
		TRACE(_T("(%d) Open file [%s] failed. %s"), m_nIndex, m_strSaveFileName, ::ErrorCodeFormatMessage(GetLastError()));
	}
	return bRet;
}

void CDownLoadPublic::ClearThreadHandle()
{
	CLOSE_HANDLE(m_hThread);
}



//
// 获取下载对象的文件名（带扩展名的）
//
CString CDownLoadPublic::GetDownloadObjectFileName( CString *pstrExtensionName )
{
	ASSERT (!m_strObject.IsEmpty());
	CString strOnlyPath, strOnlyFileName, strExtensionName;
	if (!PartPathAndFileAndExtensionName(m_strObject, &strOnlyPath, &strOnlyFileName, &strExtensionName ))
		return _T("");
	if (pstrExtensionName) *pstrExtensionName = strExtensionName;
	if (!strExtensionName.IsEmpty())
	{
		strOnlyFileName += ".";
		strOnlyFileName += strExtensionName;
	}
	return strOnlyFileName;
}

CString CDownLoadPublic::GetRefererFromURL()
{
	int nPos = m_strDownloadUrl.ReverseFind (_T('/'));
	if (nPos < 0) return _T("");
	return m_strDownloadUrl.Left(nPos);
}

BOOL CDownLoadPublic::ThreadIsRunning()
{
	if (!HANDLE_IS_VALID(m_hThread))
		return FALSE;
	return (WaitForSingleObject(m_hThread, 0) != WAIT_OBJECT_0);
}

//////////////////////////////////////////////////////////////////////////////
// BASE64编码
int Base64Encode(LPCTSTR lpszEncoding, CString &strEncoded)
{
	UINT m_nBase64Mask[]= { 0, 1, 3, 7, 15, 31, 63, 127, 255 };
	int nDigit;
	int nNumBits = 6;
	int nIndex = 0;
	int nInputSize;

	strEncoded = _T( "" );
	if(lpszEncoding == NULL)
		return 0;

	if((nInputSize = lstrlen(lpszEncoding)) == 0)
		return 0;

	int nBitsRemaining = 0;
	long lBitsStorage	=0;
	long lScratch		=0;
	int nBits;
	UCHAR c;

	while(nNumBits > 0)
	{
		while((nBitsRemaining < nNumBits) && (nIndex < nInputSize)) 
		{
			c = lpszEncoding[nIndex++];
			lBitsStorage <<= 8;
			lBitsStorage |= (c&0xff);
			nBitsRemaining += 8;
		}
		if(nBitsRemaining < nNumBits) 
		{
			lScratch = lBitsStorage << (nNumBits - nBitsRemaining);
			nBits    = nBitsRemaining;
			nBitsRemaining = 0;
		}	 
		else 
		{
			lScratch = lBitsStorage >> (nBitsRemaining - nNumBits);
			nBits	 = nNumBits;
			nBitsRemaining -= nNumBits;
		}
		nDigit = (int)(lScratch & m_nBase64Mask[nNumBits]);
		nNumBits = nBits;
		if( nNumBits <=0 )
			break;

		strEncoded += g_szBase64TAB[nDigit];
	}
	// Pad with '=' as per RFC 1521
	while(strEncoded.GetLength() % 4 != 0)
		strEncoded += '=';

	return strEncoded.GetLength();

}

// STATIC BASE64解码
int Base64Decode(LPCTSTR lpszDecoding, CString &strDecoded)
{
	int nIndex =0;
	int nDigit;
	int nDecode[ 256 ];
	int nSize;
	int nNumBits = 6;
	int i;

	if(lpszDecoding == NULL)
		return 0;

	if((nSize = lstrlen(lpszDecoding)) == 0)
		return 0;

	// Build Decode Table
	for(i = 0; i < 256; i++) 
		nDecode[i] = -2; // Illegal digit
	for(i=0; i < 64; i++)
	{
		nDecode[g_szBase64TAB[i]] = i;
		nDecode['='] = -1; 
	}

	// Clear the output buffer
	strDecoded = _T("");
	long lBitsStorage  =0;
	int nBitsRemaining = 0;
	int nScratch = 0;
	UCHAR c;

	// Decode the Input
	for(nIndex = 0, i = 0; nIndex < nSize; nIndex++)
	{
		c = lpszDecoding[nIndex];

		// 忽略所有不合法的字符
		if( c> 0x7F)
			continue;

		nDigit = nDecode[c];
		if(nDigit >= 0) 
		{
			lBitsStorage = (lBitsStorage << nNumBits) | (nDigit & 0x3F);
			nBitsRemaining += nNumBits;
			while(nBitsRemaining > 7) 
			{
				nScratch = lBitsStorage >> (nBitsRemaining - 8);
				strDecoded += (wchar_t)(nScratch & 0xFF);
				i++;
				nBitsRemaining -= 8;
			}
		}
	}	

	return strDecoded.GetLength();
}


// 从URL里面拆分出Server、Object、协议类型等信息，其中 Object 里的值是区分大小写的，否则有些网站可能会下载不了
BOOL ParseTrueURL(LPCTSTR lpszURL, CString &strServer, CString &strObject,USHORT& nPort, CString &strProtocolType)
{
	if (!lpszURL || _tcslen(lpszURL) < 1) return FALSE;
	CString strURL_Lower(lpszURL);
	strURL_Lower.TrimLeft();
	strURL_Lower.TrimRight();
	strURL_Lower.Replace(_T("\\"),_T("/"));
	CString strURL = strURL_Lower;
	strURL_Lower.MakeLower();

	// 清除数据
	strServer = _T("");
	strObject = _T("");
	nPort	  = 0;

	int nPos = strURL_Lower.Find(_T("://"));
	if(nPos == -1)
	{
		strURL_Lower.Insert(0,_T("http://"));
		strURL.Insert(0,_T("http://"));
		nPos = 4;
	}
	strProtocolType = strURL_Lower.Left(nPos);

	strURL_Lower = strURL_Lower.Mid(strProtocolType.GetLength()+3);
	strURL = strURL.Mid(strProtocolType.GetLength()+3);
	nPos = strURL_Lower.Find(_T('/'));
	if (nPos == -1)
		return FALSE;

	strObject = strURL.Mid(nPos);
	CString strServerAndPort = strURL_Lower.Left(nPos);

	// 查找是否有端口号，站点服务器域名一般用小写
	nPos = strServerAndPort.Find(_T(":"));
	if(nPos == -1)
	{
		strServer = strServerAndPort;
		nPort	  = DEFAULT_HTTP_PORT;
		if (strProtocolType == _T("ftp"))
			nPort = DEFAULT_FTP_PORT;
	}
	else
	{
		strServer = strServerAndPort.Left(nPos);
		CString str = strServerAndPort.Mid(nPos+1);
		nPort = (USHORT)_ttoi(str.GetBuffer(str.GetLength()));
		str.ReleaseBuffer();
	}
	return TRUE;
}


// 设置通知回调函数
void SetDownloadNotifyCallback( FUNC_DownloadNotify Proc_DownloadNotify, WPARAM wParam )
{
	g_funcDownloadNotify = Proc_DownloadNotify;
	g_wDownloadNotify_Param = wParam;
}

void DownloadNotify( int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR )
{
	ASSERT (pDownloadMTR);
	DownloadNotifyPara DownloadNotifyPar = {0};
	DownloadNotifyPar.nIndex = nIndex;
	DownloadNotifyPar.nNotityType = nNotityType;
	DownloadNotifyPar.lpNotifyData = lpNotifyData;
	DownloadNotifyPar.pDownloadMTR = pDownloadMTR;

	if (g_funcDownloadNotify)
		g_funcDownloadNotify(&DownloadNotifyPar, g_wDownloadNotify_Param);
}
