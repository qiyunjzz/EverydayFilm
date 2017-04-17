#include "StdAfx.h"
#include "SocketClient.h"
#include "PublicFunc.h"

#pragma comment(lib, "wSock32.lib")

#define		_afxSockThreadState			AfxGetModuleThreadState()
#define		_AFX_SOCK_THREAD_STATE		AFX_MODULE_THREAD_STATE
#define		WM_SOCKET_NOTIFY			0x0373   
#define		WM_SOCKET_DEAD				0x0374 

CSocketClient::CSocketClient(void):m_hEvtEndModule(NULL),m_bConnected(FALSE),m_nIndex(-1)
{
}

CSocketClient::~CSocketClient(void)
{
}

BOOL CSocketClient::IsConnected() //是否连接
{
	return m_bConnected;
}  


CString  CSocketClient::GetDigitStrAtHead(LPCTSTR lpszStr)		//得到数字报头
{
	if (!lpszStr) return _T("");
	CString csStr = lpszStr;
	csStr.TrimLeft();
	csStr.TrimRight();
	CString csDigitStr;
	for (int i = 0; isdigit((int)csStr[i]); i++)
	{
		csDigitStr += csStr[i];
	}
	return csDigitStr;
}

// return :	> 0	-回应代码  =0 -未读到数据  =-1 -网络出错
int  CSocketClient::GetResponse (CString *pstrResponseStr, BOOL bBlock)   //得到响应
{
	if (pstrResponseStr) *pstrResponseStr =_T("");
	ASSERT (m_hSocket != INVALID_SOCKET);
	CString strOneLine = GetOneLine(m_strResponseHistoryString);
	m_strResponseHistoryString.ReleaseBuffer();
	if (strOneLine.IsEmpty())
	{
		TCHAR szRecvBuf[NET_BUFFER_SIZE] = {0};
		char strnet[NET_BUFFER_SIZE]={0};
		int nRet = Receive(strnet,sizeof(strnet),bBlock);

		if (nRet <= 0)
		{
			if (nRet < 0)
				TRACE(_T("(%d) Receive response data failed"),m_nIndex);
			return nRet;
		}
		m_strResponseHistoryString += szRecvBuf;
		strOneLine = GetOneLine (m_strResponseHistoryString);
		m_strResponseHistoryString.ReleaseBuffer();
		if (strOneLine.IsEmpty()) return -1;
	}

	if (pstrResponseStr) 
		*pstrResponseStr = strOneLine;
	CString strRetCode = GetDigitStrAtHead(strOneLine);

	TRACE(_T("(%d) FTP server response: %s"),m_nIndex,strOneLine);
	return _ttoi(strRetCode);
}

BOOL  CSocketClient::GetResponse (int nVerifyCode,CString *pstrResponseStr)
{
	CString strResponseStr;
	int nResponseCode = GetResponse ( &strResponseStr );
	if (pstrResponseStr) *pstrResponseStr = strResponseStr;
	if (nResponseCode != nVerifyCode)
	{
		TRACE(_T("Receive error response code : %s"),strResponseStr);
		return FALSE;
	}
	return TRUE;
}

CString CSocketClient::GetResponseHistoryString()	//等到历史响应
{
	return m_strResponseHistoryString; 
}

void CSocketClient::ClearResponseHistoryString()		//清楚历史响应
{
	m_strResponseHistoryString.Empty();
}



BOOL CSocketClient::Connect(LPCTSTR lpszHost, USHORT nPort)
{
	if (!lpszHost || _tcslen(lpszHost) <= 0 || nPort < 1)
		return FALSE;

	TRACE(_T("(%d) Connecting to [%s:%d] ..."), m_nIndex, lpszHost, nPort);
	Close ();
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("AfxSocketInit() failed!"));
		return FALSE;
	}
	if (!Create())
		return FALSE;
	m_bConnected = CSocket::Connect(lpszHost, nPort);
	if (!m_bConnected)
		TRACE(_T("(%d) Connect to [%s:%d] failed"), m_nIndex, lpszHost, nPort);
	return m_bConnected;
}

void CSocketClient::Disconnect()
{
	CancelBlockingCall();
	Close();
	m_bConnected = FALSE;
}

// return :(>= 0 - 收到的字节数)  (-1 -	失败)

int CSocketClient::Receive(char* pBuf, int nSize, BOOL bBlock)
{
	if (!pBuf || nSize < 0) return -1;

	int nReadSize = 0;
	if (bBlock)
	{
		nReadSize = CSocket::Receive(pBuf, nSize);

		if (nReadSize <= 0) 
			nReadSize = -1;
	}
	else
	{
		nReadSize = CAsyncSocket::Receive(pBuf, nSize);

		if (nReadSize < 0)
		{
			if (WSAEWOULDBLOCK  == GetLastError ())
				nReadSize = 0;
			else
				nReadSize = -1;
		}
	}
	if (nReadSize == -1)
	{
		m_bConnected = FALSE;
	}
	return nReadSize;
}


// 从类似 "(192,168,0,2,4,31)" 字符串中得到IP地址和端口号
BOOL CSocketClient::GetIPAndPortByPasvString(LPCTSTR lpszPasvString, OUT CString &strIP, OUT USHORT &nPort)
{
	if (!lpszPasvString ) return FALSE;
	const TCHAR *p = _tcschr(lpszPasvString,_T('('));
	if (!p) return FALSE;
	CString strPasvStr = p+1, strTemp;
	int nPosStart = 0, nPosEnd = 0;
	int nMultiple = 0, nMantissa = 0;
	for (int i = 0; ; i++)
	{
		nPosEnd = strPasvStr.Find (_T(","),nPosStart);
		if (nPosEnd < 0)
		{
			if (i == 5)
			{
				nPosEnd = strPasvStr.Find(_T(")"), nPosStart);
				strTemp = strPasvStr.Mid(nPosStart, nPosEnd-nPosStart);
				nMantissa = _ttoi(strTemp);
				break;
			}
			else 
				return FALSE;
		}
		strTemp = strPasvStr.Mid(nPosStart, nPosEnd-nPosStart);
		strTemp.TrimLeft(); strTemp.TrimRight();
		if (i < 4)
		{
			if (!strIP.IsEmpty ()) strIP +=_T(".");
			strIP += strTemp;
		}
		else if (i == 4)
		{
			nMultiple = _ttoi (strTemp);
		}
		else 
			return FALSE;
		nPosStart = nPosEnd + 1;
	}
	nPort = nMultiple*256 + nMantissa;
	return TRUE;
}

void CSocketClient::SetEventOfEndModule(HANDLE hEvtEndModule)
{
	m_hEvtEndModule = hEvtEndModule;
	ASSERT(m_hEvtEndModule && m_hEvtEndModule != INVALID_HANDLE_VALUE);
}


BOOL CSocketClient::Send(char *pData, int nSize)
{
	ASSERT(m_hEvtEndModule && m_hEvtEndModule != INVALID_HANDLE_VALUE);
	ASSERT(m_hSocket != INVALID_SOCKET);
	if (!pData || nSize < 1) return TRUE;

	int nRemainBytes = nSize;
	int nSentTotalBytes = 0;
	int nSendFailedCount = 0;
	while (nRemainBytes > 0)
	{
		int nSentBytes = CSocket::Send(pData+nSentTotalBytes,nRemainBytes);
		if (nSentBytes < 0)
		{
			nSendFailedCount++;
			if (nSendFailedCount > 10)
			{
				TRACE(_T("(%d) Send net data block failed"), m_nIndex);
				m_bConnected = FALSE;
				return FALSE;
			}
			else
			{
				SLEEP_RETURN(100);
			}
		}
		else
		{
			nRemainBytes -= nSentBytes;
			nSentTotalBytes += nSentBytes;
			nSendFailedCount = 0;
		}
	}
	return TRUE;
}

BOOL CSocketClient::SendString(LPCTSTR lpszData, ...)
{
	USES_CONVERSION;
	// 格式化
	char szDataBuf[NET_BUFFER_SIZE] = {0};
	va_list  va;
	va_start(va,lpszData);
	_vsnprintf(szDataBuf,sizeof(szDataBuf)-1,/*(const char*)*/T2A(lpszData),va);
	va_end(va);

	TRACE(_T("(%d) Send string : \r\n------------------------>>>\r\n%s------------------------>>>\r\n"),	m_nIndex, szDataBuf);
	return Send(szDataBuf, strlen(szDataBuf));
}


// 改写这个虚函数，可以在调用 CancelBlockingCall() 函数时立即终止连接
BOOL CSocketClient::ConnectHelper(const SOCKADDR *lpSockAddr, int nSockAddrLen)
{
	if (!CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen))
	{
		if (GetLastError() == WSAEWOULDBLOCK)
		{
			while (PumpMessagesMy(FD_CONNECT))
			{
				if (m_nConnectError != -1)
				{
					WSASetLastError(m_nConnectError);
					return (m_nConnectError == 0);
				}
			}
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CSocketClient::PumpMessagesMy(UINT uStopFlag)
{
	//相同的插座，最好不要在多个地方封锁。
	ASSERT(m_pbBlocking == NULL);

	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;

	ASSERT(pState->m_hSocketWindow != NULL);

	BOOL bBlocking = TRUE;
	m_pbBlocking = &bBlocking;
	CWinThread* pThread = AfxGetThread();

	//这是不是在Winsock感超时，但更多的	像一个WM_KICKIDLE保持消息抽水活着

	UINT nTimerID = ::SetTimer(pState->m_hSocketWindow, 1, m_nTimeOut, NULL);

	if (nTimerID == 0)
		AfxThrowResourceException();

	BOOL bPeek = TRUE;

	while (bBlocking)
	{
		TRY
		{
			MSG msg;
			if ( ::PeekMessage(&msg, pState->m_hSocketWindow,
				WM_SOCKET_NOTIFY, WM_SOCKET_DEAD, PM_REMOVE) )
			{
				if (msg.message == WM_SOCKET_NOTIFY && (SOCKET)msg.wParam == m_hSocket)
				{
					if (WSAGETSELECTEVENT(msg.lParam) == FD_CLOSE)
					{
						break;
					}
					if (WSAGETSELECTEVENT(msg.lParam) == uStopFlag)
					{
						if (uStopFlag == FD_CONNECT)
							m_nConnectError = WSAGETSELECTERROR(msg.lParam);
						break;
					}
				}
				if (msg.wParam != 0 || msg.lParam != 0)
					CSocket::AuxQueueAdd(msg.message, msg.wParam, msg.lParam);

				bPeek = TRUE;
			}
			else if ( ::PeekMessage(&msg, pState->m_hSocketWindow,
				WM_TIMER, WM_TIMER, PM_REMOVE) )
			{
				break;
			}

			if (bPeek && ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (OnMessagePending())
				{
					// allow user-interface updates
					pThread->OnIdle(-1);
				}
				else
				{
					bPeek = FALSE;
				}
			}
			else
			{
				// no work to do -- allow CPU to sleep
				SLEEP_RETURN ( 100 );
				bPeek = TRUE;
			}
		}
		CATCH_ALL(e)
		{
			TRACE(_T("Error: caught exception in PumpMessage - continuing.\n"));
			e->Delete ();
			bPeek = TRUE;
		}
		END_CATCH_ALL
	}

	::KillTimer(pState->m_hSocketWindow, nTimerID);

	if (!bBlocking)
	{
		WSASetLastError(WSAEINTR);
		return FALSE;
	}
	m_pbBlocking = NULL;

	::PostMessage(pState->m_hSocketWindow, WM_SOCKET_NOTIFY, 0, 0);

	return TRUE;

}