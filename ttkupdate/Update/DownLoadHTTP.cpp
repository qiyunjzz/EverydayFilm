#include "StdAfx.h"
#include "DownLoadHTTP.h"
#include "PublicFunc.h"

void DownloadNotify(int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR);

CDownLoadHTTP::CDownLoadHTTP(void)
{
}

CDownLoadHTTP::~CDownLoadHTTP(void)
{
}


BOOL CDownLoadHTTP::DownloadOnce()
{
	// 不需要下载了
	int nWillDownloadSize = GetWillDownloadSize();				// 本次应该下载的字节数
	int nDownloadedSize = GetDownloadedSize();				// 已下载字节数
	if (nWillDownloadSize > 0 && nDownloadedSize >= nWillDownloadSize)
		return DownloadEnd(TRUE);

	if (!CDownLoadPublic::DownloadOnce())
		return DownloadEnd(FALSE);

	char szTailData[NET_BUFFER_SIZE] = {0};
	int nTailSize = sizeof(szTailData);
	if (!RequestHttpData(TRUE, szTailData,&nTailSize))
		return DownloadEnd(FALSE);
	// 从HTTP服务器中读取数据，并保存到文件中
	return DownloadEnd(RecvDataAndSaveToFile(m_SocketClient,szTailData,nTailSize));
}


BOOL CDownLoadHTTP::RequestHttpData(BOOL bGet, char *szTailData/*=NULL*/, int *pnTailSize/*=NULL*/ )
{
	int nTailSizeTemp = 0;
	BOOL bRetryRequest = TRUE;
	while (bRetryRequest)
	{
		CString strReq = GetRequestStr(bGet);
		CString strResponse;
		nTailSizeTemp = pnTailSize?(*pnTailSize):0;
		if (!SendRequest(strReq, strResponse, szTailData, &nTailSizeTemp))
			return FALSE;

		CString strReferer_Old = m_strReferer;
		CString strDownloadUrl_Old = m_strDownloadUrl;
		CString strServer_Old = m_strServer;
		CString strObject_Old = m_strObject;
		USHORT nPort_Old = m_nPort;
		CString strProtocolType_Old = m_strProtocolType;
		if (!ParseResponseString(strResponse, bRetryRequest))
		{
			if (!m_strCookieFlag.IsEmpty())
			{
				m_strCookieFlag.Empty();
				return FALSE;
			}
			m_strReferer = strReferer_Old;
			m_strDownloadUrl = strDownloadUrl_Old;
			m_strServer = strServer_Old;
			m_strObject = strObject_Old;
			m_nPort = nPort_Old;
			m_strProtocolType = strProtocolType_Old;
			m_strCookieFlag =_T("Flag=UUIISPoweredByUUSoft");
			bRetryRequest = TRUE;
		}
	}
	if (pnTailSize)
		*pnTailSize = nTailSizeTemp;
	return TRUE;
}



// 获取远程站点信息，如：是否支持断点续传、要下载的文件大小和创建时间等
BOOL CDownLoadHTTP::GetRemoteSiteInfoPro()
{
	BOOL bRet = FALSE;
	if (!CDownLoadPublic::GetRemoteSiteInfoPro())
		goto finished;

	if (!RequestHttpData(TRUE))
		goto finished;

	bRet = TRUE;

finished:
	return bRet;
}


CString CDownLoadHTTP::GetRequestStr(BOOL bGet)
{
	CString strVerb;
	if(bGet)
		strVerb = _T("GET ");
	else
		strVerb = _T("HEAD ");

	CString strReq, strAuth, strRange;
	strReq  = strVerb  + m_strObject +_T(" HTTP/1.1\r\n");

	if (!m_strUsername.IsEmpty ())
	{
		strAuth = _T("");
		Base64Encode(m_strUsername +_T(":") + m_strPassword, strAuth);
		strReq +=_T("Authorization: Basic ") + strAuth +_T("\r\n");
	}

	CString strPort;
	if (m_nPort != DEFAULT_HTTP_PORT)
		strPort.Format (_T(":%d"), m_nPort);
	strReq +=_T("Host: ")+ m_strServer + strPort +_T("\r\n");
	strReq +=_T("Accept: */*\r\n");
	strReq +=_T("Pragma: no-cache\r\n"); 
	strReq +=_T("Cache-Control: no-cache\r\n");
	strReq +=_T("User-Agent: "+m_strUserAgent+ _T("\r\n"));
	if(m_strReferer.IsEmpty())
	{
		m_strReferer = GetRefererFromURL ();
	}
	strReq +=_T("Referer: ")+m_strReferer+_T("\r\n");
	strReq +=_T("Connection: close\r\n");
	if (!m_strCookieFlag.IsEmpty())
	{
		strReq +=_T("Cookie: ")+ m_strCookieFlag +_T("\r\n");
	}

	// 指定要下载的文件范围
	CString strEndPos;
	int nWillDownloadStartPos = GetWillDownloadStartPos();	// 开始位置
	int nWillDownloadSize = GetWillDownloadSize();				// 本次应该下载的字节数
	int nDownloadedSize = GetDownloadedSize();				// 已下载字节数
	if (nWillDownloadSize > 0)
		strEndPos.Format(_T("%d"), nWillDownloadStartPos+nWillDownloadSize-1);
	ASSERT (nWillDownloadSize < 0 || nDownloadedSize < nWillDownloadSize);
	strRange.Format(_T("Range: bytes=%d-%s\r\n"), nWillDownloadStartPos+nDownloadedSize, strEndPos);

	strReq+=strRange;
	//strReq.Append(strRange);
	TRACE(_T("%s"),strReq);
	strReq += _T("\r\n");

	return strReq;
}



// 向服务器提交请求，并得到返回字符串
BOOL CDownLoadHTTP::SendRequest(LPCTSTR lpszReq, CString &strResponse, char *szTailData/*=NULL*/, int *pnTailSize/*=NULL*/ )
{
	USES_CONVERSION;
	m_SocketClient.Disconnect();
	if (!Connect ()) 
		return FALSE;
	if (!m_SocketClient.SendString (lpszReq))
	{
		return FALSE;
	}

	for (int i = 0; ; i++)
	{

		char szRecvBuf[NET_BUFFER_SIZE] = {0};
		TRACE(_T("%d"),sizeof(szRecvBuf));
		int nReadSize = m_SocketClient.Receive(szRecvBuf, sizeof(szRecvBuf));
		//_tcscpy(szRecvBuf,A2T(strnet));

		if (nReadSize <= 0)			return FALSE;

		strResponse += szRecvBuf;
		char *p = strstr(szRecvBuf,"\r\n\r\n");
		if (p)
		{
			if (szTailData && pnTailSize && *pnTailSize > 0)
			{
				p += 4;
				int nOtioseSize = nReadSize - int( p - szRecvBuf );
				*pnTailSize = MIN ( nOtioseSize, *pnTailSize );
				memcpy(szTailData, p, *pnTailSize);
			}
#ifdef _DEBUG
			int nPos = strResponse.Find (_T("\r\n\r\n"), 0);
			CString strDump;
			if (nPos >= 0)
				strDump = strResponse.Left(nPos);
			else 
				strDump = strResponse;
#endif
			break;
		}
	}
	return TRUE;
}



DWORD CDownLoadHTTP::GetResponseCode(CString strLineText)
{
	strLineText.MakeLower ();
	ASSERT(strLineText.Find (_T("http/"), 0) >= 0);
	int nPos = strLineText.Find (_T(" "), 0);
	if (nPos < 0) return 0;
	CString strCode = strLineText.Mid(nPos + 1);
	strCode.TrimLeft(); 
	strCode.TrimRight();
	nPos = strCode.Find (_T(" "), 0);
	if (nPos < 0) 
		nPos = strCode.GetLength() - 1;
	strCode = strCode.Left (nPos);

	return (DWORD)_ttoi(strCode);
}


BOOL CDownLoadHTTP::ParseResponseString ( CString strResponseString, OUT BOOL &bRetryRequest )
{
	bRetryRequest = FALSE;
	// 获取返回代码
	CString strOneLine = GetOneLine (strResponseString);
	strResponseString.ReleaseBuffer();
	DWORD dwResponseCode = GetResponseCode (strOneLine);

	if (dwResponseCode < 1)		return FALSE;

	int nPos = 0;
	// 请求文件被重定向
	if(dwResponseCode >= 300 && dwResponseCode < 400)
	{
		bRetryRequest = TRUE;
		// 得到请求文件新的URL
		CString strRedirectFileName = FindAfterFlagString(_T("location:"), strResponseString);

		// 设置 Referer
		m_strReferer = GetRefererFromURL();

		// 重定向到其他的服务器
		nPos = strRedirectFileName.Find(_T("://"));
		if (nPos >= 0)
		{
			m_strDownloadUrl = strRedirectFileName;
			// 检验要下载的URL是否有效
			if (!ParseTrueURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort, m_strProtocolType))			return FALSE;

			return TRUE;
		}

		// 重定向到本服务器的其他地方
		strRedirectFileName.Replace(_T("\\"),_T("/"));
		// 重定向于根目录
		if(strRedirectFileName[0] ==_T('/'))
		{
			m_strObject = strRedirectFileName;
			CString str = GetDownloadObjectFileName();
			DownloadNotify(-1, NOTIFY_TYPE_GOT_REMOTE_FILENAME, (LPVOID)(str.GetBuffer(str.GetLength())), m_pDownloadMTR);
			str.ReleaseBuffer();
			return TRUE;
		}

		// 定向于相对当前目录
		int nParentDirCount = 0;
		nPos = strRedirectFileName.Find (_T("../"));
		while (nPos >= 0)
		{
			strRedirectFileName = strRedirectFileName.Mid(nPos+3);
			nParentDirCount++;
			nPos = strRedirectFileName.Find(_T("../"));
		}
		for (int i = 0; i <= nParentDirCount; i++)
		{
			nPos = m_strDownloadUrl.ReverseFind(_T('/'));
			if (nPos != -1)
				m_strDownloadUrl = m_strDownloadUrl.Left(nPos);
		}
		if (strRedirectFileName.Find (_T("./"), 0) == 0)
			strRedirectFileName.Delete(0, 2);
		m_strDownloadUrl = m_strDownloadUrl+_T("/")+strRedirectFileName;

		return ParseTrueURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort, m_strProtocolType);
	}
	// 请求被成功接收、理解和接受
	else if(dwResponseCode >= 200 && dwResponseCode < 300)
	{
		if (m_nIndex == -1)	// 主线程才需要获取文件大小的信息
		{
			// 获取 Content-Length
			CString strDownFileLen = FindAfterFlagString(_T("content-length:"), strResponseString);
			m_nFileTotalSize = (int)_ttoi(strDownFileLen.GetBuffer(strDownFileLen.GetLength()));
			strDownFileLen.ReleaseBuffer();
			DownloadNotify (-1, NOTIFY_TYPE_GOT_REMOTE_FILESIZE, (LPVOID)m_nFileTotalSize, m_pDownloadMTR);
			int nWillDownloadStartPos = GetWillDownloadStartPos();	// 开始位置
			int nWillDownloadSize = GetWillDownloadSize();				// 本次应该下载的字节数
			int nDownloadedSize = GetDownloadedSize();				// 已下载字节数
			if (m_nFileTotalSize > 0 && nWillDownloadSize-nDownloadedSize > m_nFileTotalSize)
				SetWillDownloadSize(m_nFileTotalSize-nDownloadedSize);
		}

		// 获取服务器文件的最后修改时间
		CString strModifiedTime = FindAfterFlagString(_T("last-modified:"), strResponseString);
		if (!strModifiedTime.IsEmpty())
		{
			m_TimeLastModified = ConvertHttpTimeString(strModifiedTime);
		}

		if (dwResponseCode == 206)	// 支持断点续传
		{
			m_bSupportResume = TRUE;
		}
		else							// 不支持断点续传
		{
			m_bSupportResume = FALSE;
		}
		return TRUE;
	}

	return FALSE;
}


CString CDownLoadHTTP::FindAfterFlagString(LPCTSTR lpszFoundStr, CString strOrg)
{
	ASSERT (lpszFoundStr && _tcslen(lpszFoundStr) > 0);
	CString strReturing, strFoundStr = GET_SAFE_STRING(lpszFoundStr);
	strFoundStr.MakeLower();

	TCHAR *pTmp = new TCHAR[strOrg.GetLength()+1];
	_tcscpy(pTmp,strOrg.GetBuffer(strOrg.GetLength()));
	strOrg.ReleaseBuffer();
	MytoLower(pTmp);
	CString strOrgLower(pTmp);
	int nPos = strOrgLower.Find(strFoundStr );
	if (nPos < 0) return _T("");
	strReturing = strOrg.Mid(nPos + strFoundStr.GetLength());
	nPos = strReturing.Find(_T("\r\n"));
	if (nPos < 0) return _T("");
	strReturing = strReturing.Left(nPos);
	strReturing.TrimLeft();
	strReturing.TrimRight();

	delete pTmp;

	return strReturing;
}


// 将 HTTP 服务器表示的时间转换为 CTime 格式，如：Wed, 16 May 2007 14:29:53 GMT
CTime CDownLoadHTTP::ConvertHttpTimeString(CString strTimeGMT)
{
	CString strYear, strMonth, strDay, strTime;
	CTime tReturning = -1;
	int nPos = strTimeGMT.Find(_T(","),0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strTimeGMT = strTimeGMT.Mid(nPos+1);
	strTimeGMT.TrimLeft();
	strTimeGMT.TrimRight();

	// 日
	nPos = strTimeGMT.Find (_T(" "), 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strDay = strTimeGMT.Left(nPos);
	strTimeGMT = strTimeGMT.Mid(nPos + 1);
	strTimeGMT.TrimLeft();
	strTimeGMT.TrimRight();

	// 月
	nPos = strTimeGMT.Find (_T(" "), 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strMonth = strTimeGMT.Left(nPos);
	int nMonth = GetMouthByShortStr(strMonth);
	ASSERT(nMonth >= 1 && nMonth <= 12);
	strMonth.Format (_T("%02d"), nMonth);
	strTimeGMT = strTimeGMT.Mid(nPos + 1);
	strTimeGMT.TrimLeft(); 
	strTimeGMT.TrimRight();

	// 年
	nPos = strTimeGMT.Find (_T(" "), 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strYear = strTimeGMT.Left(nPos);
	strTimeGMT = strTimeGMT.Mid(nPos + 1);
	strTimeGMT.TrimLeft(); 
	strTimeGMT.TrimRight();

	// 时间
	nPos = strTimeGMT.Find (_T(" "), 0);
	if (nPos < 0 || nPos >= strTimeGMT.GetLength()-1)
		return tReturning;
	strTime = strTimeGMT.Left(nPos);
	strTimeGMT = strTimeGMT.Mid (nPos + 1);

	CString strFileTimeInfo;
	strFileTimeInfo.Format(_T("%s-%s-%s %s"), strYear, strMonth, strDay, strTime);
	ConvertStrToCTime(strFileTimeInfo.GetBuffer(strFileTimeInfo.GetLength()), tReturning);
	strFileTimeInfo.ReleaseBuffer();
	return tReturning;
}
