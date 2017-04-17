#include "StdAfx.h"
#include "DownLoadMTR.h"
#include <io.h>
#include "PublicFunc.h"


void DownloadNotify(int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloadMTR);

CDownLoadMTR::CDownLoadMTR() : m_nThreadCount(DEFAULT_THREAD_COUNT)
, m_pDownloadPub_MTR(NULL)
, m_pDownloadPub_Info(NULL)
, m_pDownloadCellInfo(NULL)
, m_hThread(NULL)
, m_bForceDownload(FALSE)
, m_bFileExist(FALSE)
, m_bDownStart(FALSE)
, m_bDownError(FALSE)
, m_nTotalDownloadedSize_ThisTimes(0)
{
	memset(&m_BaseDownInfo,0,sizeof(BaseDownInfo));
	m_hEvtEndModule = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_dwDownloadStartTime = GetTickCount();
}

CDownLoadMTR::~CDownLoadMTR(void)
{
	StopDownload ();
}



// 设置下载的线程数
BOOL CDownLoadMTR::SetThreadCount(int nThreadCount)
{
	if (nThreadCount <= 0 || nThreadCount > MAX_DOWNLOAD_THREAD_COUNT)
	{
		TRACE(_T("Thread count %d is invalid. Rang [%d-%d]"), nThreadCount, 1, MAX_DOWNLOAD_THREAD_COUNT);
		return FALSE;
	}
	if (nThreadCount == m_nThreadCount)
		return TRUE;

	m_nThreadCount = nThreadCount;
	return TRUE;
}

// 下载任务的线程函数
DWORD WINAPI ThreadProcDownloadMTR(LPVOID lpParameter   // thread data
									)
{
	CDownLoadMTR *pDownloadMTR = (CDownLoadMTR*)lpParameter;
	ASSERT(pDownloadMTR);
	pDownloadMTR->ThreadProcDownloadMTR();
	TRACE(_T("下载任务的线程函数 执行完毕\n"));
	return TRUE;
}

BOOL CDownLoadMTR::ThreadProcDownloadMTR()
{
	// 启动多线程下载任务
	int nRet = StartMTRDownload();
	if (nRet == 2) return HandleDownloadFinished(ENUM_DOWNLOAD_RESULT_SUCCESS);
	if (nRet == 0) return HandleDownloadFinished(ENUM_DOWNLOAD_RESULT_FAILED);

	// 等待所有线程下载完成
	ENUM_DOWNLOAD_RESULT eDownloadResult = WaitForDownloadFinished ();
	if (eDownloadResult == ENUM_DOWNLOAD_RESULT_SUCCESS && !GetDownloadResult()) 
	{
		eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;
	}
	return HandleDownloadFinished(eDownloadResult);
}

BOOL CDownLoadMTR::StartDownload()
{
	return Download(m_strDownloadURL, m_strSavePath, m_strSaveOnlyFileName);
}

// 多线程断点续传下载一个文件
BOOL CDownLoadMTR::Download (
							 LPCTSTR lpszDownloadURL,
							 LPCTSTR lpszSavePath,
							 LPCTSTR lpszSaveOnlyFileName,
							 LPCTSTR lpszUsername/*=NULL*/,
							 LPCTSTR lpszPassword/*=NULL*/,
							 BOOL bForceDownload/*=FALSE*/		// 如果为 TRUE 表示强制性重新下载，以下载的部分将会被删除，FALSE 表示断点续传
							 )
{
	if (!HANDLE_IS_VALID(m_hEvtEndModule))
		return FALSE;
	if (!lpszSavePath || _tcslen(lpszSavePath) < 1)
		return FALSE;
	m_strSavePath = lpszSavePath;
	m_strSaveOnlyFileName = GET_SAFE_STRING(lpszSaveOnlyFileName);
	m_bForceDownload = bForceDownload;

	CString strServer, strObject;
	USHORT nPort = 0;
	if (!ParseTrueURL (lpszDownloadURL, strServer, strObject, nPort, m_strProtocolType))
	{
		TRACE(_T("Download URL [%s] invalid"), lpszDownloadURL);
		m_bDownError=TRUE;
		return FALSE;
	}
	m_strDownloadURL = lpszDownloadURL;

	// 创建取站点信息对象
	if (!(m_pDownloadPub_Info = CreateDownloadObject()))
	{
		TRACE(_T("Create download object failed"));
		m_bDownError=TRUE;
		return HandleDownloadFinished(ENUM_DOWNLOAD_RESULT_FAILED);
	}
	// 设置取站点信息对象的参数
	m_pDownloadPub_Info->SetAuthorization(lpszUsername, lpszPassword);
	m_pDownloadPub_Info->m_pDownloadMTR = this;
	m_bDownError=!m_pDownloadPub_Info->SetDownloadUrl(lpszDownloadURL);

	// 创建一个下载线程
	DWORD dwThreadId = 0;
	m_hThread = CreateThread(NULL,0,::ThreadProcDownloadMTR,LPVOID(this),0,&dwThreadId);
	if (!HANDLE_IS_VALID(m_hThread))
	{
		TRACE(_T("Create download thread failed"));
		m_bDownError=TRUE;
		return FALSE;
	}
	return TRUE;
}


//
// 创建下载对象
//
CDownLoadPublic* CDownLoadMTR::CreateDownloadObject ( int nCount/*=1*/ )
{
	if (nCount < 1) return NULL;
	CDownLoadPublic *pDownloadPub = NULL;
	if (m_strProtocolType.CompareNoCase (_T("http")) == 0)
	{
		pDownloadPub = (CDownLoadPublic*)new CDownLoadHTTP[nCount];
	}
	else if (m_strProtocolType.CompareNoCase (_T("ftp")) == 0)
	{
		pDownloadPub = (CDownLoadPublic*)new CDownLoadFTP[nCount];
	}
	else
		return NULL;

	return pDownloadPub;
}


// 删除下载对象
void CDownLoadMTR::DeleteDownloadObject (CDownLoadPublic *pDownloadPub )
{
	if ( m_strProtocolType.CompareNoCase (_T("http")) == 0 )
	{
		delete[] ((CDownLoadHTTP*)pDownloadPub);
	}
	else if (m_strProtocolType.CompareNoCase (_T("ftp")) == 0)
	{
		delete[] ((CDownLoadFTP*)pDownloadPub);
	}
	else delete[] pDownloadPub;
}

void Callback_SaveDownloadInfo ( int nIndex, int nDownloadedSize, int nSimpleSaveSize, WPARAM wParam )
{
	CDownLoadMTR *pDownloadMTR = (CDownLoadMTR*)wParam;
	ASSERT ( pDownloadMTR );
	pDownloadMTR->CallbackSaveDownloadInfo(nIndex, nDownloadedSize, nSimpleSaveSize);
}


void CDownLoadMTR::CallbackSaveDownloadInfo( int nIndex, int nDownloadedSize, int nSimpleSaveSize )
{
	if (nIndex >= 0 && nIndex < m_nThreadCount)
	{
		m_pDownloadCellInfo[nIndex].nDownloadedSize = nDownloadedSize;
		if (nDownloadedSize > 0)
		{
			m_csDownloadedData.Lock();
			m_nTotalDownloadedSize_ThisTimes += nSimpleSaveSize;
			m_csDownloadedData.Unlock();
		}
	}
}


// 创建多线程下载使用的对象和数据缓冲
BOOL CDownLoadMTR::CreateDownloadObjectAndDataMTR()
{
	DeleteDownloadObjectAndDataMTR();

	ASSERT(!m_pDownloadPub_MTR && m_pDownloadPub_Info);
	m_pDownloadPub_MTR = CreateDownloadObject(m_nThreadCount);
	// 设置多线程下载使用的对象的参数
	if (m_pDownloadPub_MTR)
	{
		for (int nIndex=0; nIndex<m_nThreadCount; nIndex++)
		{
			m_pDownloadPub_MTR[nIndex].m_nIndex = nIndex;
			m_pDownloadPub_MTR[nIndex].m_pDownloadMTR = this;
			m_pDownloadPub_MTR[nIndex].SetSaveDownloadInfoCallback(::Callback_SaveDownloadInfo, WPARAM(this) );
			m_pDownloadPub_MTR[nIndex].SetAuthorization(m_pDownloadPub_Info->GetUserName(), m_pDownloadPub_Info->GetPassword() );
			m_pDownloadPub_MTR[nIndex].SetDownloadUrl(m_strDownloadURL);
			if (!m_pDownloadPub_MTR[nIndex].SetSaveFileName (GetTempFilePath()))
				return FALSE;
		}
	}

	// 创建多线程下载使用的数据缓冲
	ASSERT (!m_pDownloadCellInfo);
	m_pDownloadCellInfo = new DownLoadCellInfo[m_nThreadCount];
	if (m_pDownloadCellInfo)
		memset(m_pDownloadCellInfo, 0, m_nThreadCount*sizeof(DownLoadCellInfo));

	if (m_pDownloadPub_MTR != NULL && m_pDownloadCellInfo != NULL)
		return TRUE;
	TRACE(_T("Create MTR download object or buffer failed"));
	return FALSE;
}

// 删除多线程下载使用的对象和数据缓冲
void CDownLoadMTR::DeleteDownloadObjectAndDataMTR()
{
	if (m_pDownloadPub_MTR)
	{
		DeleteDownloadObject(m_pDownloadPub_MTR);
		m_pDownloadPub_MTR = NULL;
	}
	if (m_pDownloadCellInfo)
	{
		delete[] m_pDownloadCellInfo;
		m_pDownloadCellInfo = NULL;
	}
}

// 删除取站点信息的下载对象
void CDownLoadMTR::DeleteDownloadObjectInfo()
{
	if (m_pDownloadPub_Info)
	{
		DeleteDownloadObject(m_pDownloadPub_Info);
		m_pDownloadPub_Info = NULL;
	}
}


// 启动多线程下载，返回 0 表示失败，1表示成功，2表示不用下载了，因为该文件已经下载过了
int CDownLoadMTR::StartMTRDownload()
{
	m_dwDownloadStartTime = GetTickCount();
	DownloadNotify(-1, NOTIFY_TYPE_START_DOWNLOAD, (LPVOID)NULL, this);
	// 先获取站点信息
	ASSERT(m_pDownloadPub_Info);
	if (!m_pDownloadPub_Info->GetRemoteSiteInfo())
		return 0;

	TRACE(_T("要下载的文件大小是： %d 字节\n"), m_pDownloadPub_Info->GetFileTotalSize());
	CString xstr;
	xstr.Format(_T("%d"),m_pDownloadPub_Info->GetFileTotalSize());

	StandardSaveFileName();
	CFileStatus fileStatus;

	if (m_bForceDownload)
	{
		// 需要重新下载
		::DeleteFile(m_strSavePathFileName);
		::DeleteFile(GetTempFilePath());
	}
	else
	{
		// 要保存的文件是否已经存在，且大小和创建时间一致，如果不是强制性下载，则不需要再下载了。
		if (CFile::GetStatus(m_strSavePathFileName,fileStatus))
		{
			if	(
				(
				fileStatus.m_mtime.GetTime() - m_pDownloadPub_Info->GetTimeLastModified() <=2 &&
				m_pDownloadPub_Info->GetTimeLastModified()-fileStatus.m_mtime.GetTime() <=2
				)
				&&
				fileStatus.m_size == m_pDownloadPub_Info->GetFileTotalSize()
				&&
				!m_bForceDownload
				)
			{
				m_bFileExist=TRUE;
				return 2;
			}
		}
	}

	BOOL bMustCreateNullFile = TRUE;
	// 读取下载信息，如果能读到说明上次下载尚未完成
	if (!m_bForceDownload && m_pDownloadPub_Info->IsSupportResume())
	{
		if (CFile::GetStatus(GetTempFilePath(),fileStatus) &&
			fileStatus.m_size == m_pDownloadPub_Info->GetFileTotalSize()+GetDownloadInfoWholeSize())
		{
			if (ReadDownloadInfo())
				bMustCreateNullFile = FALSE;
		}
	}

	if (bMustCreateNullFile)
	{
		int nFileSize = m_pDownloadPub_Info->GetFileTotalSize();
		int nTempFileSize = nFileSize+GetDownloadInfoWholeSize();
		if (nFileSize < 0 || !m_pDownloadPub_Info->IsSupportResume())
			nTempFileSize = 0;
		// 创建一个用来保存下载数据的空文件
		if (!CreateNullFile(GetTempFilePath(), nTempFileSize ))
		{
			m_bDownError=TRUE;
			return FALSE;
		}

	}

	// 分配下载任务
	if (!AssignDownloadTask())
	{
		TRACE(_T("Assign task failed"));
		m_bDownError=TRUE;
		return 0;
	}

	m_dwDownloadStartTime = GetTickCount();
	return 1;
}


// 得到临时数据保存的路径文件名
CString CDownLoadMTR::GetTempFilePath()
{
	ASSERT(!m_strSavePathFileName.IsEmpty());
	CString strTempFileName;
	strTempFileName.Format(_T("%s.lj"), m_strSavePathFileName);
	//::SetFileAttributes(strTempFileName,FILE_ATTRIBUTE_HIDDEN);
	return strTempFileName;
}


// 分配下载任务
BOOL CDownLoadMTR::AssignDownloadTask()
{
	ASSERT ( m_pDownloadPub_Info );
	if (!m_pDownloadPub_Info->IsSupportResume())
	{
		DeleteDownloadObjectAndDataMTR ();
		TRACE(_T("Site [%s] not support resume download"), m_pDownloadPub_Info->GetServerName());
	}
	// 文件大小未知，采用单线程
	if (m_pDownloadPub_Info->GetFileTotalSize () <= 0 || !m_pDownloadPub_Info->IsSupportResume())
	{
		if (m_nThreadCount != 1)
		{
			DeleteDownloadObjectAndDataMTR();
			SetThreadCount(1);
		}
	}

	if (!DownloadInfoIsValid() || !m_pDownloadPub_MTR || !m_pDownloadCellInfo)
	{
		if (!CreateDownloadObjectAndDataMTR())
			return FALSE;
	}

	ASSERT (m_pDownloadPub_MTR && m_pDownloadCellInfo);

	// 下载任务尚未分配
	if (!DownloadInfoIsValid())
	{
		int nWillDownloadSize = -1, nWillDownloadStartPos = 0, nNoAssignSize = 0;
		if (m_pDownloadPub_Info->GetFileTotalSize() > 0)
		{
			nWillDownloadSize = m_pDownloadPub_Info->GetFileTotalSize () / m_nThreadCount;
			// 均分后剩下的部分，让第一个线程来承担下载
			nNoAssignSize = m_pDownloadPub_Info->GetFileTotalSize () % m_nThreadCount;
		}

		TRACE(_T("任务分配如下：--------------------\n"));

		for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
		{
			m_pDownloadCellInfo[nIndex].nWillDownloadStartPos = nWillDownloadStartPos;
			m_pDownloadCellInfo[nIndex].nWillDownloadSize = nWillDownloadSize;
			if (nIndex == 0 && m_pDownloadPub_Info->GetFileTotalSize () > 0)
			{
				m_pDownloadCellInfo[nIndex].nWillDownloadSize += nNoAssignSize;
			}

			TRACE(_T("线程.%d 从 %d(0x%08x) 下载到 %d(0x%08x) 共 %d(0x%08x) 字节\n"), nIndex, 
				m_pDownloadCellInfo[nIndex].nWillDownloadStartPos, m_pDownloadCellInfo[nIndex].nWillDownloadStartPos,
				m_pDownloadCellInfo[nIndex].nWillDownloadStartPos+m_pDownloadCellInfo[nIndex].nWillDownloadSize,
				m_pDownloadCellInfo[nIndex].nWillDownloadStartPos+m_pDownloadCellInfo[nIndex].nWillDownloadSize,
				m_pDownloadCellInfo[nIndex].nWillDownloadSize, m_pDownloadCellInfo[nIndex].nWillDownloadSize);

			nWillDownloadStartPos += m_pDownloadCellInfo[nIndex].nWillDownloadSize;
		}
	}


	// 启动下载任务
	for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
	{
		if (!m_pDownloadPub_MTR[nIndex].Download(m_pDownloadCellInfo[nIndex].nWillDownloadStartPos,
			m_pDownloadCellInfo[nIndex].nWillDownloadSize, m_pDownloadCellInfo[nIndex].nDownloadedSize))
			return FALSE;
	}
	m_bDownStart=TRUE;
	m_BaseDownInfo.dwThreadCount = m_nThreadCount;
	return TRUE;
}


// 从下载信息文件中读取下载信息
BOOL CDownLoadMTR::ReadDownloadInfo()
{
	CString strTempFileName = GetTempFilePath();
	BOOL bRet = FALSE;
	CFile file;
	TRY
	{
		if (file.Open(strTempFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone ))
		{
			if ( file.Seek(-(int)sizeof(BaseDownInfo), CFile::end) == (int)(file.GetLength() - sizeof(BaseDownInfo)))
			{
				if ((UINT)file.Read ( &m_BaseDownInfo, sizeof(BaseDownInfo) ) == sizeof(BaseDownInfo))
				{
					if ((m_BaseDownInfo.dwThreadCount > 0 && m_BaseDownInfo.dwThreadCount <= MAX_DOWNLOAD_THREAD_COUNT)&&
						SetThreadCount ( m_BaseDownInfo.dwThreadCount ))
					{
						if (CreateDownloadObjectAndDataMTR())
						{
							if (file.Seek(-GetDownloadInfoWholeSize(), CFile::end ) == int(file.GetLength() - GetDownloadInfoWholeSize()))
							{
								if (file.Read(m_pDownloadCellInfo, sizeof(DownLoadCellInfo)*m_nThreadCount ) == sizeof(DownLoadCellInfo)*m_nThreadCount)
								{
									bRet = TRUE;
								}
								else
								{
									memset(m_pDownloadCellInfo, 0, sizeof(DownLoadCellInfo)*m_nThreadCount);
								}
							}
						}
					}
				}
			}
		}
	}
	CATCH(CFileException, e)
	{
		e->Delete ();
		bRet = FALSE;
	}
	END_CATCH

	if (HANDLE_IS_VALID(file.m_hFile))
		file.Close();

	return bRet;
}


BOOL CDownLoadMTR::SaveDownloadInfo ()
{
	if (!m_pDownloadPub_Info->IsSupportResume())
		return TRUE;
	CString strTempFileName = GetTempFilePath();
	BOOL bRet = FALSE;
	CFile file;
	TRY
	{
		if (file.Open(strTempFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone))
		{
			if (file.Seek ( -(int)sizeof(BaseDownInfo), CFile::end ) == (int)(file.GetLength() - sizeof(BaseDownInfo)))
			{
				file.Write ( &m_BaseDownInfo, sizeof(BaseDownInfo) );
				if (file.Seek ( -GetDownloadInfoWholeSize(), CFile::end ) == int(file.GetLength() - GetDownloadInfoWholeSize()))
				{
					file.Write(m_pDownloadCellInfo, m_nThreadCount*sizeof(DownLoadCellInfo));
					bRet = TRUE;
				}
			}
		}
	}
	CATCH(CFileException, e)
	{
		e->Delete();
		bRet = FALSE;
	}
	END_CATCH
	if (HANDLE_IS_VALID(file.m_hFile))
			file.Close();

	if (!bRet)
		TRACE(_T("Save download info failed. %s"), ErrorCodeFormatMessage(GetLastError()));

	return bRet;
}


BOOL CDownLoadMTR::HandleDownloadFinished(ENUM_DOWNLOAD_RESULT eDownloadResult)
{
	CString strTempFileName;
	CFileStatus fileStatus;
	BOOL bRet = FALSE;
	CFile file;

	if (eDownloadResult != ENUM_DOWNLOAD_RESULT_SUCCESS)
	{
		SaveDownloadInfo();
		goto Finished;
	}
	strTempFileName = GetTempFilePath();

	// 设置文件大小
	if (m_pDownloadPub_Info->IsSupportResume() && m_pDownloadPub_Info->GetFileTotalSize() > 0)
	{
		TRY
		{
			file.Open(strTempFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone);
			file.SetLength(m_pDownloadPub_Info->GetFileTotalSize ());
			bRet = TRUE;
		}
		CATCH(CFileException, e)
		{
			e->Delete();
			bRet = FALSE;
		}
		END_CATCH
			
		if (HANDLE_IS_VALID(file.m_hFile))
			file.Close();

		if (!bRet)
		{
			TRACE(_T("Set [%s] length failed"), strTempFileName);
			eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;
			goto Finished;
		}
	}

	if (_taccess(strTempFileName,04) == 0)
	{
		// 将文件改名
		bRet = FALSE;
		DeleteFile(m_strSavePathFileName);
		TRY
		{
			CFile::Rename(strTempFileName, m_strSavePathFileName);
			bRet = TRUE;
		}
		CATCH(CFileException, e)
		{
			e->Delete();
			bRet = FALSE;
		}
		END_CATCH

		if (!bRet)
		{
			TRACE(_T("Rename [%s] failed. %s"), strTempFileName, ErrorCodeFormatMessage(GetLastError()));
			eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;
			goto Finished;
		}

		// 设置文件属性，时间设置和服务器一致
		bRet = FALSE;
		if (CFile::GetStatus(m_strSavePathFileName,fileStatus))
		{
			fileStatus.m_mtime = m_pDownloadPub_Info->GetTimeLastModified();
			fileStatus.m_attribute = CFile::normal;
			CFile::SetStatus (m_strSavePathFileName, fileStatus);
			bRet = TRUE;
		}
		if ( !bRet )
		{
			TRACE(_T("Set file [%s] status failed. %s"), strTempFileName, ErrorCodeFormatMessage(GetLastError()));
			eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;
			goto Finished;
		}
	}

Finished:
	DownloadNotify(-1, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult, this);
	return bRet;
}


BOOL CDownLoadMTR::GetDownloadResult()
{
	for (int nIndex=0; nIndex<m_nThreadCount; nIndex++)
	{
		if (!m_pDownloadPub_MTR[nIndex].IsDownloadSuccess())
			return FALSE;
	}
	return TRUE;
}


// 下载信息是否有效
BOOL CDownLoadMTR::DownloadInfoIsValid()
{
	BOOL bValid = FALSE;
	int nIndex = 0;
	if (!m_pDownloadCellInfo) 
		goto Invalid;
	if (m_BaseDownInfo.dwThreadCount < 1 || m_BaseDownInfo.dwThreadCount > MAX_DOWNLOAD_THREAD_COUNT)
		goto Invalid;

	for (nIndex=0; nIndex<m_nThreadCount; nIndex++)
	{
		if (m_pDownloadCellInfo[nIndex].nWillDownloadSize > 0)
		{
			bValid = TRUE;
			break;
		}
	}
	if (!bValid) goto Invalid;

	return TRUE;

Invalid:
	if (m_pDownloadCellInfo)
		memset(m_pDownloadCellInfo, 0, m_nThreadCount*sizeof(DownLoadCellInfo));
	memset(&m_BaseDownInfo, 0, sizeof(BaseDownInfo));
	return FALSE;
}



// 找到剩余未下载的数量最大的那个对象编号
int CDownLoadMTR::GetUndownloadMaxBytes( int &nUndownloadBytes )
{
	nUndownloadBytes = 0;
	int nMaxIndex = -1;
	for (int nIndex=0; nIndex<m_nThreadCount; nIndex++)
	{
		int nTempBytes = m_pDownloadPub_MTR[nIndex].GetUndownloadBytes();
		if (nUndownloadBytes < nTempBytes)
		{
			nUndownloadBytes = nTempBytes;
			nMaxIndex = nIndex;
		}
	}
	return nMaxIndex;
}


// 编号为 nIndex 的对象调度任务，为下载任务最繁重的对象减轻负担
BOOL CDownLoadMTR::AttemperDownloadTask(int nIndex)
{
	ASSERT(m_pDownloadPub_MTR && m_pDownloadCellInfo);
	if (m_nThreadCount <= 1 || m_pDownloadCellInfo[nIndex].nWillDownloadSize == -1)
		return FALSE;
	int nUndownloadBytes = 0;
	int nIndex_Heavy = GetUndownloadMaxBytes(nUndownloadBytes);
	if (nIndex_Heavy == -1 || nIndex_Heavy == nIndex)
		return FALSE;
	if (m_pDownloadPub_MTR[nIndex_Heavy].ThreadIsRunning() && nUndownloadBytes < 100*1024)
		return FALSE;
	ASSERT(nIndex_Heavy >= 0 && nIndex_Heavy < m_nThreadCount);
	ASSERT(m_pDownloadPub_MTR[nIndex_Heavy].GetWillDownloadStartPos() == m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadStartPos);

	TRACE(_T("下载对象.%d 帮 %d (%s) 减轻负担\n"), nIndex, nIndex_Heavy, m_pDownloadPub_MTR[nIndex_Heavy].ThreadIsRunning()?"运行":"停止");


	// 给空闲下载对象分配新任务
	m_pDownloadCellInfo[nIndex].nWillDownloadSize = ( m_pDownloadPub_MTR[nIndex_Heavy].ThreadIsRunning()?(nUndownloadBytes/2) : nUndownloadBytes );
	m_pDownloadCellInfo[nIndex].nWillDownloadStartPos = m_pDownloadPub_MTR[nIndex_Heavy].GetWillDownloadStartPos() +
		m_pDownloadPub_MTR[nIndex_Heavy].GetWillDownloadSize() - m_pDownloadCellInfo[nIndex].nWillDownloadSize;
	m_pDownloadCellInfo[nIndex].nDownloadedSize = 0;

	TRACE(_T("空闲下载对象.%d 分配新任务： %d(0x%08x) - %d(0x%08x) 共 %d(0x%08x)\n"), nIndex, m_pDownloadCellInfo[nIndex].nWillDownloadStartPos, m_pDownloadCellInfo[nIndex].nWillDownloadStartPos,m_pDownloadCellInfo[nIndex].nWillDownloadStartPos + m_pDownloadCellInfo[nIndex].nWillDownloadSize, m_pDownloadCellInfo[nIndex].nWillDownloadStartPos + m_pDownloadCellInfo[nIndex].nWillDownloadSize,m_pDownloadCellInfo[nIndex].nWillDownloadSize, m_pDownloadCellInfo[nIndex].nWillDownloadSize);



	// 启动空闲下载对象的下载任务
	if (m_pDownloadCellInfo[nIndex].nWillDownloadSize == 0)
		return FALSE;
	m_pDownloadPub_MTR[nIndex].ResetVar();
	if (!m_pDownloadPub_MTR[nIndex].Download(m_pDownloadCellInfo[nIndex].nWillDownloadStartPos,
		m_pDownloadCellInfo[nIndex].nWillDownloadSize, m_pDownloadCellInfo[nIndex].nDownloadedSize))
		return FALSE;


	// 减轻繁忙下载对象的任务
	m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadSize -= m_pDownloadCellInfo[nIndex].nWillDownloadSize;
	m_pDownloadPub_MTR[nIndex_Heavy].SetWillDownloadSize(m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadSize);

	TRACE(_T("繁忙下载对象.%d 下载了 %d(0x%08x) 未完 %d(0x%08x) 字节，调整任务为： %d(0x%08x) - %d(0x%08x) 共 %d(0x%08x)\n"),
		nIndex_Heavy, m_pDownloadPub_MTR[nIndex_Heavy].GetDownloadedSize(), m_pDownloadPub_MTR[nIndex_Heavy].GetDownloadedSize(),
		nUndownloadBytes, nUndownloadBytes,
		m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadStartPos, m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadStartPos,
		m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadStartPos + m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadSize, m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadStartPos + m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadSize,
		m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadSize, m_pDownloadCellInfo[nIndex_Heavy].nWillDownloadSize);


	return TRUE;
}


// 等待下载结束
ENUM_DOWNLOAD_RESULT CDownLoadMTR::WaitForDownloadFinished()
{
	ASSERT(HANDLE_IS_VALID(m_hEvtEndModule));
	int nCount = m_nThreadCount + 1;
	ENUM_DOWNLOAD_RESULT eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;
	HANDLE *lpHandles = new HANDLE[nCount];
	if (!lpHandles) 
		goto End;
	while (TRUE)
	{
		nCount = 0;
		for (int nIndex=0; nIndex<m_nThreadCount; nIndex++)
		{
			HANDLE hThread = m_pDownloadPub_MTR[nIndex].GetThreadHandle();
			if (HANDLE_IS_VALID(hThread))
				lpHandles[nCount++] = hThread;
		}
		lpHandles[nCount++] = m_hEvtEndModule;

		if (nCount == 1)
		{
			if (GetTotalDownloadedSize() >= m_pDownloadPub_Info->GetFileTotalSize())
			{
				ASSERT(GetTotalDownloadedSize() == m_pDownloadPub_Info->GetFileTotalSize());
				eDownloadResult = ENUM_DOWNLOAD_RESULT_SUCCESS;
			}
			else
				eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;
			goto End;
		}
		int nRet = (int)WaitForMultipleObjects(nCount, lpHandles, FALSE, INFINITE ) - WAIT_OBJECT_0;
		// 某下载对象完成任务了
		if (nRet >= 0 && nRet < nCount-1)
		{
			int nIndex = FindIndexByThreadHandle(lpHandles[nRet]);
			if ((nIndex >= 0 && nIndex < m_nThreadCount ))
			{
				if (!m_pDownloadPub_MTR[nIndex].IsDownloadSuccess() ||
					!AttemperDownloadTask (nIndex))
				{
					m_pDownloadPub_MTR[nIndex].ClearThreadHandle();
				}
			}
			else
			{
				eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
				goto End;
			}
		}
		// 模块结束		
		else
		{
			eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
			goto End;
		}
	}

End:
	// 等待所有下载线程结束
	if (eDownloadResult != ENUM_DOWNLOAD_RESULT_SUCCESS)
	{
		nCount = 0;
		for (int nIndex=0; nIndex<m_nThreadCount; nIndex++)
		{
			HANDLE hThread = m_pDownloadPub_MTR[nIndex].GetThreadHandle();
			if (HANDLE_IS_VALID(hThread))
				lpHandles[nCount++] = hThread;
		}
		WaitForMultipleObjects(nCount, lpHandles, TRUE, 500*1000);
	}
	if (lpHandles) 
		delete[] lpHandles;
	return eDownloadResult;
}


int CDownLoadMTR::FindIndexByThreadHandle(HANDLE hThread)
{
	for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
	{
		HANDLE hThread_Temp = m_pDownloadPub_MTR[nIndex].GetThreadHandle();
		if (HANDLE_IS_VALID(hThread_Temp) && hThread_Temp == hThread)
			return nIndex;
	}
	return -1;
}

int CDownLoadMTR::GetDownloadInfoWholeSize()
{
	return ( sizeof(DownLoadCellInfo)*m_nThreadCount + sizeof(BaseDownInfo));
}


// 获取下载所消耗的时间（毫秒），可用来计算下载速度和推算剩余时间
DWORD CDownLoadMTR::GetDownloadElapsedTime()
{
 	return (GetTickCount()-m_dwDownloadStartTime);
}



// 停止下载。将所有下载线程关闭，将下载对象删除，文件关闭
void CDownLoadMTR::StopDownload()
{
	if (HANDLE_IS_VALID(m_hEvtEndModule))
	{
		::SetEvent(m_hEvtEndModule);
	}

	// 设置多线程下载使用的对象的参数
	if (m_pDownloadPub_MTR)
	{
		for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
		{
			m_pDownloadPub_MTR[nIndex].StopDownload();
		}
	}
	if (m_pDownloadPub_Info)
	{
		m_pDownloadPub_Info->StopDownload();
	}

	if (HANDLE_IS_VALID(m_hThread))
	{
		WaitForThreadEnd(m_hThread,100*1000);
		CLOSE_HANDLE(m_hThread)
	}

	DeleteDownloadObjectAndDataMTR();
	DeleteDownloadObjectInfo();

	CLOSE_HANDLE(m_hEvtEndModule);
}



void CDownLoadMTR::StandardSaveFileName()
{
	ASSERT(m_strSavePath.GetLength() > 0);

	ASSERT(m_strSavePath.GetAt(m_strSavePath.GetLength() - 1) ==_T('\\'));
	/*  
	StandardizationPathBuffer ( m_strSavePath.GetBuffer(MAX_PATH), MAX_PATH );
	m_strSavePath.ReleaseBuffer ();
	MakeSureDirectory ( m_strSavePath );
	*/

	TCHAR szOnlyFileName_NoExt_User[MAX_PATH] = {0};
	TCHAR szExtensionName_User[MAX_PATH] = {0};
	// 如果用户指定了新的保存文件名，就用新的。
	if (m_strSaveOnlyFileName.GetLength() > 0)
	{
		CString strFileNameByURL = GetLocalFileNameByURL (m_strDownloadURL);
		if (strFileNameByURL.CompareNoCase(m_strSaveOnlyFileName) != 0)
		{
			PartFileAndExtensionName(m_strSaveOnlyFileName, szOnlyFileName_NoExt_User, MAX_PATH, szExtensionName_User, MAX_PATH);
		}
	}

	CString strExtensionName_Remote;
	CString strFileName_Remote = m_pDownloadPub_Info->GetDownloadObjectFileName(&strExtensionName_Remote);

	if (_tcslen(szOnlyFileName_NoExt_User) > 0)
	{
		if (_tcslen(szExtensionName_User) < 1)
		{
			//#define STRNCPY_CS(sz,str) strncpy((char*)(sz),(str).GetBuffer((str).GetLength()),sizeof(sz))
			STRNCPY_CS(szExtensionName_User, strExtensionName_Remote);
			strExtensionName_Remote.ReleaseBuffer();
		}
		m_strSavePathFileName.Format (_T("%s%s.%s"), StandardizationFileForPathName(m_strSavePath,FALSE),
			StandardizationFileForPathName(szOnlyFileName_NoExt_User,TRUE), StandardizationFileForPathName(szExtensionName_User,TRUE) );
	}
	else
	{
		m_strSavePathFileName.Format (_T("%s%s"), StandardizationFileForPathName(m_strSavePath,FALSE), StandardizationFileForPathName(strFileName_Remote,TRUE));
	}
}


// 根据 URL 来获取本地保存的文件名
CString CDownLoadMTR::GetLocalFileNameByURL ( LPCTSTR lpszDownloadURL )
{
	if (!lpszDownloadURL || _tcslen(lpszDownloadURL) < 1)
		return _T("");
	TCHAR szOnlyPath[MAX_PATH] = {0};
	TCHAR szOnlyFileName[MAX_PATH] = {0};
	if (!PartFileAndPathByFullPath(lpszDownloadURL, szOnlyFileName, MAX_PATH, szOnlyPath, MAX_PATH))
		return _T("");
	return szOnlyFileName;
}


// 获取文件大小
int CDownLoadMTR::GetFileTotaleSize()
{
	if (!m_pDownloadPub_Info) return -1;
	return m_pDownloadPub_Info->GetFileTotalSize ();
}

// 获取已下载的字节数，包括以前下载的和本次下载的
int CDownLoadMTR::GetTotalDownloadedSize()
{
	if (!m_pDownloadPub_Info) return -1;
	int nTotalUndownloadBytes = 0;
	for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
	{
		nTotalUndownloadBytes += m_pDownloadPub_MTR[nIndex].GetUndownloadBytes();
	}
	int nFileSize = m_pDownloadPub_Info->GetFileTotalSize();
	if (nFileSize < 1) return -1;
	// 文件大小减去未完成的，就是已下载的
	TRACE(_T("%d %dBYTE"),nFileSize ,nTotalUndownloadBytes);
	return (nFileSize - nTotalUndownloadBytes);
}

int CDownLoadMTR::GetTotalDownloadedSize_ThisTimes()
{
	m_csDownloadedData.Lock();
	int nTotalDownloadedSize_ThisTimes = m_nTotalDownloadedSize_ThisTimes;
	m_csDownloadedData.Unlock();
	return nTotalDownloadedSize_ThisTimes;
}

