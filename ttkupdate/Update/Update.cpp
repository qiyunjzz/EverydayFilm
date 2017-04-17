// Update.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Update.h"
#include<afxinet.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpdateApp

BEGIN_MESSAGE_MAP(CUpdateApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUpdateApp 构造

CUpdateApp::CUpdateApp()
{
	m_enDlState=DL_NORMAL;
	m_bDownComplete=FALSE;
}


// 唯一的一个 CUpdateApp 对象

CUpdateApp theApp;


// CUpdateApp 初始化

BOOL CUpdateApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if(!AfxSocketInit())
	{
		AfxMessageBox(_T("初始化出错！"));
		return FALSE;
	}

	AfxEnableControlContainer();

#ifdef _DEBUG
	AllocConsole();  
	freopen( "CONOUT$", "w+t", stdout );//用于调试
#else
#endif

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	if(!OnlyOneExe())
	{		return FALSE;	}

	OnInitFile();

	CString strUpdateFile;

	strUpdateFile.Format(_T("http://up.hao.kaikai001.com/new/up/newupdate.ini?%d"),GetTickCount());

	if(!OnGetUpdateFile(strUpdateFile)) return FALSE;
	////OnGetUpdateFile(strUpdateFile,m_strFile);

	//Read DownLoad Mode 正常 DL_NORMAL  提示 DL_CHECK  强制 DL_FORCE

	//command line start;
	LPTSTR  strCommand=GetCommandLine();
	int iCommand=0;

	 LPWSTR *strArgv =CommandLineToArgvW(strCommand,&iCommand);

	 CString str;

	 if (iCommand >= 2) 
	 {
		if(_tcscmp(strArgv[1],_T("slient"))==0)
		{
			if(!IsUpdateNow()) return FALSE;
			else
			{
				int iMode=OnGetRunMode();
				if(iMode==1) m_enDlState=DL_FORCE;
				else m_enDlState=DL_CHECK;

				if(m_enDlState==DL_CHECK)
				{
					CString strMge;
					strMge.Format(_T("发现有天天看新版%s，请点确认升级"),m_strFileVision);
					if(IDCANCEL==AfxMessageBox(strMge,MB_OKCANCEL|MB_TOPMOST))
						return FALSE;					
				}
			}
		}
		else
		{
			return FALSE;
		}
     }   
	 //command line end;

	CUpdateDlg dlg;
	m_pMainWnd = &dlg;

	
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}


BOOL CUpdateApp::OnlyOneExe()
{
	// Create Mutex
	HANDLE hMutex = CreateMutex(NULL, FALSE,_T("ExeEverydayUpdate"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return FALSE;
	}
	return TRUE;
}

void CUpdateApp::OnInitFile()
{
	//file
	m_strFilePath=GetExeModulePath();
	m_strUrl=_T("http://up.hao.kaikai001.com/new/up/");

	//DWORD  mt=GetTickCount();
	//m_strFileName.Format(_T("update%d.ini"),mt);
	m_strFileName=_T("update.ini");
	m_strFile=m_strFilePath+_T("\\")+m_strFileName;
}

int CUpdateApp::OnGetRunMode()
{
	return ::GetPrivateProfileInt(_T("RUN"),_T("MODE"),0,m_strFile);
}

int CUpdateApp::OnGetUpdateFile(CString strUrl)  
{
	if(strUrl.IsEmpty()) return FALSE;

	int downIndex=m_downControl.AddTask(strUrl.GetBuffer(strUrl.GetLength()),m_strFilePath.GetBuffer(m_strFilePath.GetLength()),m_strFileName.GetBuffer(m_strFileName.GetLength()),1,NULL,NULL,TRUE);
	strUrl.ReleaseBuffer();
	m_strFilePath.ReleaseBuffer();
	m_strFileName.ReleaseBuffer();

    int nTaskNum = m_downControl.GetTaskNum();
	if(nTaskNum < 1) 
	{
		MessageBox(NULL,_T("下载出错！ Download Error！"),NULL,NULL);
		return FALSE;
	}

	DWORD mStartTime=GetTickCount();
	BOOL downOne=TRUE;
	float f1=0;

	while(downOne)
	{
		int nFileTotaleSize = 0, nTotalDownloadedSize = 0, nTotalDownloadedSize_ThisTimes = 0, nDownloadElapsedTime = 0;

		Sleep(100);
		if(m_downControl.QueryState(downIndex,nFileTotaleSize,nTotalDownloadedSize,nTotalDownloadedSize_ThisTimes,nDownloadElapsedTime)==2)
		{
				float f = float(nTotalDownloadedSize*100.0/nFileTotaleSize);
				if((int)f1!=(int)f) 
				{
					f1=f;
					mStartTime=GetTickCount();
				}
				Sleep(100);
				downOne=FALSE;	
		}
		else if(m_downControl.QueryState(downIndex,nFileTotaleSize,nTotalDownloadedSize,nTotalDownloadedSize_ThisTimes,nDownloadElapsedTime)==1)
		{			
			if(nTotalDownloadedSize==nFileTotaleSize)	
			{
				Sleep(100);
				downOne=FALSE;
			}
		}				

	//	TRACE(_T("GetTickCount %d   mStartTime %d m_bDownError %d\n"),GetTickCount()/1000,((mStartTime+30800)/1000),m_downControl.m_pCurDownloadMTR->m_bDownError);
		if((GetTickCount()/1000)>=((mStartTime+30800)/1000))
		{
			if(m_downControl.m_pCurDownloadMTR||m_downControl.m_pCurDownloadMTR->m_bDownError)
			{
					 if(m_downControl.m_pCurDownloadMTR) m_downControl.StopTask(downIndex);
					 Sleep(500);
					 return FALSE;
			}
		}
	}
	return TRUE;
}

int CUpdateApp::ExitInstance()
{
	CFileStatus   status; 
	m_strFile.Replace(_T("\\"),_T("\\\\"));
	if(m_bDownComplete) OnRunExe();
    if(CFile::GetStatus(m_strFile,status)) 
	{
		DeleteFile(m_strFile);	
	}
	return CWinApp::ExitInstance();
}

BOOL CUpdateApp::OnRunExe()
{
	TCHAR strExe[MAX_PATH];
	::GetPrivateProfileString(_T("RUN"),_T("NAME"),_T(""),strExe,MAX_PATH,m_strFile);
	if(_tcslen(strExe)>0)
		ShellExecute(NULL,_T("open"),strExe,NULL,NULL,SW_SHOWNORMAL);
	ShellExecute(NULL,_T("FilmEveryday.exe"),strExe,NULL,NULL,SW_SHOWNORMAL);
	return TRUE;
}

BOOL CUpdateApp::GetFileExist(CString strFile)
{
	CFileStatus   status; 
	strFile.Replace(_T("\\"),_T("\\\\"));
     if(!CFile::GetStatus(strFile,status)) return FALSE;

	return TRUE;
}



CString CUpdateApp::GetExeVision(CString AppName)
{
	CString AppVersion;

	DWORD RessourceVersionInfoSize;
	DWORD JustAJunkVariabel;
	TCHAR* VersionInfoPtr;
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *TranslationPtr;
	TCHAR* InformationPtr;
	UINT  VersionInfoSize;
	TCHAR  VersionValue[255];

	RessourceVersionInfoSize=GetFileVersionInfoSize(AppName,&JustAJunkVariabel);
	if(0!=RessourceVersionInfoSize)
	{
		VersionInfoPtr=new TCHAR[RessourceVersionInfoSize];
		if(GetFileVersionInfo(AppName,0,RessourceVersionInfoSize,VersionInfoPtr))
		{
			if(!VerQueryValue(
				VersionInfoPtr,
				_T("VarFileInfo\\Translation"),
				(LPVOID*)&TranslationPtr,
				&VersionInfoSize))
			{
				delete[] VersionInfoPtr;
				return AppVersion;
			}
		}

		// retrieve File Description
		wsprintf(VersionValue,
			_T("\\StringFileInfo\\%04x%04x\\FileVersion"),
			TranslationPtr[0].wLanguage,
			TranslationPtr[0].wCodePage);

		if(!VerQueryValue(
			VersionInfoPtr,
			VersionValue,
			(LPVOID*)&InformationPtr,
			&VersionInfoSize))
		{
			delete[] VersionInfoPtr;
			return AppVersion;
		}
		if(_tcslen(InformationPtr)>0) //Not Null
		{
			AppVersion=CString(InformationPtr);
		}
		delete[] VersionInfoPtr;
	}
	return AppVersion;
}

BOOL CUpdateApp::CompareVision(CString strUpdate,CString strExe)
{
	strUpdate.Replace(_T("."),_T(","));
	strUpdate.Replace(_T(" "),_T(""));
	strExe.Replace(_T(" "),_T(""));

	TCHAR seps[] =_T(",");
	TCHAR *tokenU=NULL,*tokenE=NULL;
	TCHAR* Update[10],*Exe[10];

	tokenU = _tcstok( strUpdate.GetBuffer(strUpdate.GetLength()),seps);
	strUpdate.ReleaseBuffer();
	int i=0;
	while( tokenU != NULL )					//分割字符 '|'
	{
		Update[i]=tokenU;
		tokenU = _tcstok( NULL, seps );
		i++;
	}
	
	tokenE = _tcstok( strExe.GetBuffer(strExe.GetLength()),seps);
	strExe.ReleaseBuffer();
	int j=0;
	while( tokenE != NULL )					//分割字符 '|'
	{
		Exe[j]=tokenE;
		tokenE = _tcstok( NULL, seps );
		j++;
	}

	if(i>j||i!=j) return TRUE;

	if(_ttoi(Exe[0])<_ttoi(Update[0]))  //主版本号
	{
		return TRUE;
	}
	else if(_ttoi(Exe[0])>_ttoi(Update[0]))
	{
		return FALSE;
	}

	if(_ttoi(Exe[1])<_ttoi(Update[1]))  //副版本号
	{
		return TRUE;
	}
	else if(_ttoi(Exe[1])>_ttoi(Update[1])) 
	{
		return FALSE;
	}
	
	if(_ttoi(Exe[2])<_ttoi(Update[2]))  //月 版本号
	{
		return TRUE;
	}
	else if(_ttoi(Exe[2])>_ttoi(Update[2])) 
	{
		return FALSE;
	}

	if(_ttoi(Exe[3])<_ttoi(Update[3]))  //日 版本号
	{
		return TRUE;
	}
	else if(_ttoi(Exe[3])>_ttoi(Update[3]))
	{
		return FALSE;
	}

	return FALSE;
}

BOOL CUpdateApp::IsUpdateNow()
{
	m_strExePath=GetExeModulePath();
	m_strExePath.Append(_T("FilmEveryday.exe"));
	m_strExePath.Replace(_T("\\"),_T("\\\\"));

	CString strNowVision=GetExeVision(m_strExePath);	

	TCHAR strNewVision[255];
	::GetPrivateProfileString(_T("UPDATE"),_T("VERSION"),_T("Error"),strNewVision,255,m_strFile);
	
	if(_tcscmp(strNewVision,_T("Error"))==0) return FALSE;
	
	m_strFileVision=strNewVision;
	return CompareVision(strNewVision,strNowVision);
}

CString CUpdateApp::GetExeModulePath()
{
	TCHAR szFilePath[MAX_PATH + 1]; 
	AfxGetInstanceHandle();
	GetModuleFileName(NULL, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
	CString str_url = szFilePath;
	return str_url;
}