// UpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "UpdateDlg.h"
#include "EventID.h"
#include "ExitMessageDlg.h"
#include "MessageDlg.h"
#include "shlwapi.h" 
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib,"Version.lib");

//#define _CRTDBG_MAP_ALLOC 
//#include<stdlib.h> 
//
//#include<crtdbg.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_THREAD_STOP 0x0427
#define MAX_ALLSECTIONS 2048
#define MAX_SECTION  128
#define DL_NORMAL 0
#define DL_FORCE 1
#define DL_CHECK 2

static int giCount=0;

UINT __cdecl DownLoadFileThread(LPVOID pParam)
{

	CUpdateDlg* pDlg = (CUpdateDlg*)pParam;
	CStringArray* strSectionName = &(pDlg->m_strSectionName);

	CButton *pBtnAtOnce=pDlg->m_dlgDownLoad.GetBtnAtOnce();
	CButton *pBtnTryAgain=pDlg->m_dlgDownLoad.GetBtnTryAggin();

	pBtnAtOnce->EnableWindow(FALSE);
	pBtnTryAgain->EnableWindow(FALSE);

	CString strPath=pDlg->m_strFileBuffer;
	CString strCompletedFile=pDlg->m_strCompletedFileIni;

	CDownLoadControl m_downControl;

	BOOL bContinue=FALSE;
	//分析是否重下

	TCHAR strDownedVision[255];
	::GetPrivateProfileString(_T("COMPLETEDFILE"),_T("VERSION"),_T("-1"),strDownedVision,255,strCompletedFile);	

	if(_tcscmp(strDownedVision,_T("-1"))==0||_tcscmp(strDownedVision,theApp.m_strFileVision)!=0) 
	{
		//重新下载
		::WritePrivateProfileString(_T("COMPLETEDFILE"),_T("VERSION"),theApp.m_strFileVision,strCompletedFile);

		if(PathIsDirectory(strPath))  
		{
			pDlg->DeleteBufferDirectory(strPath);
		}
		CreateDirectory(strPath,NULL);
	}
	else if(_tcscmp(strDownedVision,theApp.m_strFileVision)==0)
	{
		//继续下载
		bContinue=TRUE;
	}

	CString strText;
	strText.Format(_T("共有%d个文件要升级......"),strSectionName->GetCount()-2);
	CListBox *pList=pDlg->m_dlgDownLoad.GetListBoxDown();
	pList->ResetContent();
	pList->AddString(strText);
	CStatic* pStatic=pDlg->m_dlgDownLoad.GetStaticShow();

	CProgressCtrl* pProgress=pDlg->m_dlgDownLoad.GetProgressCtrlDown();

	for(int i=1;i<strSectionName->GetCount();i++)
	{
		CString Name = strSectionName->GetAt(i);

		if(Name.Compare(_T("RUN"))==0)			continue;

		//获得当前要下载的文件的路径
		TCHAR strChildPath[MAX_PATH]=_T("");
		CString strChildDirectory;
		::GetPrivateProfileString(Name.GetBuffer(Name.GetLength()),_T("PATH"),_T(""),strChildPath,MAX_PATH,pDlg->m_strFile);
		Name.ReleaseBuffer();

		strChildDirectory=strPath+_T("\\")+strChildPath;
		strChildDirectory.Replace(_T("\\"),_T("\\\\"));

		if(bContinue)
		{
			CString strFilePath=strChildDirectory;
			strFilePath.Append(Name);
			
			if(theApp.GetFileExist(strFilePath))
			{
				strText =_T("已下载:")+Name;
				pList->AddString(strText);
				pStatic->SetWindowText(strText);		
				pDlg->m_vectorFileName.push_back(strFilePath);
				continue;
			}		
		}

		strText =_T("正在下载:")+Name;
		pList->AddString(strText);
		pStatic->SetWindowText(strText);

		if(!PathIsDirectory(strChildDirectory))  
		{
			if(CreateDirectory(strChildDirectory,NULL)==FALSE)
			{
				pList->AddString(_T("对不起，建立失败！")); 					 //退出程序
				pStatic->SetWindowText(_T("对不起，建立失败！"));
				Sleep(500);
				MessageBox(NULL,_T("建立失败！请重试！"),NULL,NULL);
				pBtnTryAgain->ShowWindow(SW_SHOW);
				pBtnAtOnce->ShowWindow(SW_HIDE);
				pBtnTryAgain->EnableWindow(TRUE);
				giCount++;
				if(giCount>=2) 	pDlg->SendMessage(WM_DOWNLOAD_FAIRL);
				return 0;			
			}
		}

		CString URL = pDlg->m_strUrl+Name;

		//先保存在临时文件夹内
		pProgress->SetPos(0);
		int downIndex=m_downControl.AddTask(URL.GetBuffer(URL.GetLength()),strChildDirectory.GetBuffer(strChildDirectory.GetLength()),Name.GetBuffer(Name.GetLength()),1);	
		m_downControl.SelTask(downIndex);
		URL.ReleaseBuffer();
		strChildDirectory.ReleaseBuffer();
		Name.ReleaseBuffer();


		int nTaskNum = m_downControl.GetTaskNum();
		if(nTaskNum < 1) 
		{
			MessageBox(NULL,_T("下载出错！请重试！"),NULL,NULL);
			pBtnTryAgain->ShowWindow(SW_SHOW);
			pBtnAtOnce->ShowWindow(SW_HIDE);
			pBtnTryAgain->EnableWindow(TRUE);
			giCount++;
			if(giCount>=2) 	pDlg->SendMessage(WM_DOWNLOAD_FAIRL);
			return 0;
		}
		
		DWORD mStartTime=GetTickCount();
		BOOL downOne=TRUE;
		
		while(downOne)
		{
			int nFileTotaleSize = 0, nTotalDownloadedSize = 0, nTotalDownloadedSize_ThisTimes = 0, nDownloadElapsedTime = 0;

			Sleep(100);
			if(m_downControl.QueryState(downIndex,nFileTotaleSize,nTotalDownloadedSize,nTotalDownloadedSize_ThisTimes,nDownloadElapsedTime)==2)
			{
				pProgress->SetPos(100);
				Sleep(100);
				downOne=FALSE;					
			}
			else if(m_downControl.QueryState(downIndex,nFileTotaleSize,nTotalDownloadedSize,nTotalDownloadedSize_ThisTimes,nDownloadElapsedTime)==1)
			{			
				float f = float(nTotalDownloadedSize*100.0/nFileTotaleSize);
				float f1 =	pProgress->GetPos();
				if((int)f1!=(int)f) 	mStartTime=GetTickCount();
				pProgress->SetPos(f);
				if(f==100)			downOne=FALSE;
			}				

//			printf("GetTickCount %d   mStartTime %d m_bDownError %d\n",GetTickCount()/1000,((mStartTime+15800)/1000),m_downControl.m_pCurDownloadMTR->m_bDownError);
			if((GetTickCount()/1000)>=((mStartTime+15800)/1000))
			{
				if(m_downControl.m_pCurDownloadMTR||m_downControl.m_pCurDownloadMTR->m_bDownError)
				{
					pList->AddString(_T("对不起，文件下载失败！")); 					 //退出程序
					pStatic->SetWindowText(_T("对不起，文件下载失败！"));
					if(m_downControl.m_pCurDownloadMTR) m_downControl.StopTask(downIndex);
					Sleep(500);
					MessageBox(NULL,_T("下载出错！请重试！"),NULL,NULL);
					pBtnTryAgain->ShowWindow(SW_SHOW);
					pBtnAtOnce->ShowWindow(SW_HIDE);
					pBtnTryAgain->EnableWindow(TRUE);
					giCount++;
					if(giCount>=2) 	pDlg->SendMessage(WM_DOWNLOAD_FAIRL);
				}
				return 0;
			}

			if(!downOne)
			{
				pDlg->m_vectorFileName.push_back(strChildDirectory+Name);
				pList->AddString(_T("下载完成"));
			}
		}
	}
	pList->AddString(_T("正在初始化....！"));

	BOOL bMove=pDlg->MoveUpdateFile();

	pStatic->SetWindowText(_T("升级完成！"));

	if(bMove)
	{
		pDlg->SendMessage(WM_DOWNLOAD_NEXT_COMPLETE);
		pDlg->DeleteBufferDirectory(strPath);
		theApp.m_bDownComplete=TRUE;
		::WritePrivateProfileString(_T("COMPLETEDFILE"),_T("VERSION"),_T("-1"),strCompletedFile);
	}
	else
	{
		pDlg->SendMessage(WM_DOWNLOAD_FAIRL);
	}

	return 0;
}



// CUpdateDlg 对话框
CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_vectorFileName.clear();
	pDownFileThread=NULL;

	m_strCompletedFileIni.Empty();
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_MESSAGE(WM_DOWNLOAD_FAIRL,OnDownLoadFail)
	ON_MESSAGE(WM_DOWNLOAD_TRY_AGAIN,OnDownLoadTryAgain)
	ON_MESSAGE(WM_DOWNLOAD_AT_ONCE,OnDownLoadAtOnce)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE,OnDownLoadComplete)
	ON_MESSAGE(WM_DOWNLOAD_NEXT_COMPLETE,OnDownLoadNextComplete)
	ON_MESSAGE(WM_CHECK_NEXT_DOWNLOAD,OnCheckNextDownLoad)
	ON_MESSAGE(WM_INTRODUCTION_NEXT,OnCheckDlgShow)
	ON_MESSAGE(WM_UPDATE_CANCEL,OnExitDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////////////////////
////////
typedef   BOOL   (FAR   PASCAL   *   FUNCTransparent)(   
											HWND   hwnd,                       //   handle   to   the   layered   window   
											COLORREF   crKey,             //   specifies   the   color   key   
											BYTE   bAlpha,                   //   value   for   the   blend   function   
											DWORD   dwFlags                 //   action   
											);   

void CUpdateDlg::SetTrans(int itrans)
{
	HMODULE   hModule   =   GetModuleHandle(_T("user32.dll"));   
    FUNCTransparent   SetLayeredWindowAttributes;   
    SetLayeredWindowAttributes= (FUNCTransparent)GetProcAddress(hModule,"SetLayeredWindowAttributes");      
     //  设置分层扩展标记   
    SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|0x80000L);   
    //   70%   alpha   
    SetLayeredWindowAttributes(GetSafeHwnd(),   0,   (255   *   itrans)   /   100,   0x2); 

}

////////////////////////////////////////////////////////////////////////////////////////////////
///////
void CUpdateDlg::OnInit()
{
	m_downIndex=0;
	m_strSectionName.RemoveAll();
	//file
	TCHAR path[MAX_PATH]={0};
	::GetModuleFileName(NULL,path,MAX_PATH);

	m_strFilePath=theApp.m_strFilePath;
	m_strFileName=theApp.m_strFileName;
	m_strUrl=theApp.m_strUrl;
	m_strFile=theApp.m_strFile;
	m_strCompletedFileIni=theApp.GetExeModulePath();
	m_strCompletedFileIni.Append(_T("CompletedFile.ini"));

	m_strFileBuffer=theApp.GetExeModulePath();
	m_strFileBuffer.Append(_T("Buffer"));

	//Image
	m_bmpBk.LoadBitmap(IDB_BITMAP_BK);
	m_dlgIntroduction.Create(IDD_DIALOG_INTRODUCTION,this);
//	m_dlgCheck.Create(IDD_DIALOG_CHECK,this);
	m_dlgDownLoad.Create(IDD_DIALOG_DOWNLOAD,this);
	m_dlgNew.Create(IDD_DIALOG_NEW,this);
	m_dlgComplete.Create(IDD_DIALOG_COMPLETE,this);
	m_dlgFail.Create(IDD_DIALOG_DOWNLOAD_FAIL,this);
	
}


void CUpdateDlg::OnMoveControl()
{
	CRect rect;
	GetClientRect(&rect);

	m_dlgIntroduction.MoveWindow(163,0,rect.Width()-163,rect.Height());
//	m_dlgCheck.MoveWindow(163,0,rect.Width()-163,rect.Height());
	m_dlgDownLoad.MoveWindow(163,0,rect.Width()-163,rect.Height());
	m_dlgNew.MoveWindow(rect);
	m_dlgComplete.MoveWindow(163,0,rect.Width()-163,rect.Height());
	m_dlgFail.MoveWindow(163,0,rect.Width()-163,rect.Height());
}

BOOL CUpdateDlg::GetFileExist(CString strFile)
{
	CFileStatus   status; 
	strFile.Replace(_T("\\"),_T("\\\\"));
     if(!CFile::GetStatus(strFile,status)) return FALSE;

	return TRUE;
}

BOOL CUpdateDlg::GetUpdateSection()
{
	if(!GetFileExist(m_strFile)) return FALSE;

	int   i;   
    int   iPos=0;   
    int   iMaxCount; 
    TCHAR   chSectionNames[MAX_ALLSECTIONS]={0};   //总的提出来的字符串 
    TCHAR   chSection[MAX_SECTION]={0};   //存放一个段名。 
    
	GetPrivateProfileSectionNames(chSectionNames,MAX_ALLSECTIONS,m_strFile); 

    //以下循环，截断到两个连续的0 
    for(i=0;i <MAX_ALLSECTIONS;i++) 
	{ 
        if(chSectionNames[i]==0) 
        if(chSectionNames[i]==chSectionNames[i+1])  break; 
	} 

    iMaxCount=i+1;   //要多一个0号元素。即找出全部字符串的结束部分。 

    for(i=0;i <iMaxCount;i++) 
	{ 
       chSection[iPos++]=chSectionNames[i]; 
       if(chSectionNames[i]==0) 
	   {   
           m_strSectionName.Add(chSection); 
           memset(chSection,0,i); 
           iPos=0; 
		}
	} 
    return TRUE; 
}

BOOL CUpdateDlg::MoveUpdateFile()
{
	if(!CloseExe()) 
	{
		if(theApp.m_enDlState!=DL_FORCE)
		{
			CMessageDlg m_dlgMessage;
			INT_PTR nResponse=m_dlgMessage.DoModal();
			CloseExe();
		}  
	}

  Sleep(100);
   for(vector<CString>::iterator it = m_vectorFileName.begin(); it < m_vectorFileName.end(); ++it)
   {
	   CString strPath=*it;
	   strPath.Replace(_T("\\\\Buffer"),_T(""));

	   if(CopyFile(*it,strPath,FALSE)==FALSE)
	   {	
		  if(ERROR_PATH_NOT_FOUND == GetLastError())
		  {   
			  if(!PathIsDirectory(strPath))  
				  CreateMulDirectory(strPath);
			  if(CopyFile(*it,strPath,FALSE)==FALSE)
				  return FALSE;	  
		  }
	   }
   }
    Sleep(300);
	return TRUE;
}

BOOL CUpdateDlg::CreateMulDirectory(CString strPath)
{
	CString newPath=strPath;
	newPath.Replace(_T("\\\\"),_T("\\"));
	int iPos=newPath.ReverseFind(_T('\\'));
	newPath=newPath.Left(iPos);
	newPath.Append(_T("\\"));

	if((PathIsDirectory(newPath)))  return true; 
    
	BOOL   bRet   =   false; 
    TCHAR   _szTmp[_MAX_DIR],_szDir[_MAX_PATH]; 
    int  nLen=0, idx ; 

	_tcscpy(_szDir,newPath); 
	nLen =_tcslen(_szDir); 

	if(_szDir[nLen-1]==_T('/'))         _szDir[nLen-1]=_T('\\'); 

	nLen =_tcslen(_szDir); 
	memset(_szTmp,0,_MAX_DIR); 
    TCHAR _str[2]; 

	for(idx=0;idx<nLen;idx++) 
	{ 
        if(_szDir[idx]!=_T('\\')) 
        { 
           _str[0]=_szDir[idx]; 
		   _str[1]=_T(''); 
		   _tcscat(_szTmp,   _str); 
		}
		else 
		{ 
			_tcscat(_szTmp,_T("\\\\")); 
		    if(!PathIsDirectory(_szTmp))  
				bRet=CreateDirectory(_szTmp,NULL);	
		}         
	}
	return bRet;
}

BOOL CUpdateDlg::DeleteBufferDirectory(CString strPath)
{
    CFileFind tempFind; 
    TCHAR sTempFileFind[_MAX_PATH] ;

    _stprintf(sTempFileFind,_T("%s\\*.*"),strPath); 
    BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
    while (IsFinded) 
    { 
      IsFinded = tempFind.FindNextFile(); 

        if (!tempFind.IsDots()) 
        { 
            TCHAR sFoundFileName[200]; 
            _tcscpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 
			tempFind.GetFileName().ReleaseBuffer();

            if (tempFind.IsDirectory()) 
            { 
                TCHAR sTempDir[200]; 
                _stprintf(sTempDir,_T("%s\\%s"),strPath,sFoundFileName); 
				DeleteBufferDirectory(sTempDir); 
            } 
            else 
            { 
                TCHAR sTempFileName[200]; 
                _stprintf(sTempFileName,_T("%s\\%s"),strPath,sFoundFileName); 
                DeleteFile(sTempFileName); 
            } 
        } 
    } 

    tempFind.Close(); 
    if(!RemoveDirectory(strPath))       return FALSE; 
    return TRUE; 
}


CString CUpdateDlg::GetExeVision(CString AppName)
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

BOOL CUpdateDlg::CompareVision(CString strUpdate,CString strExe)
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

	for(int x=0;x<i;x++)
	{
		
		if(_ttoi(Update[x])>_ttoi(Exe[x]))
			return TRUE;	
	}
	return FALSE;
}

BOOL CUpdateDlg::IsUpdateNow()
{
	m_strExePath=theApp.GetExeModulePath();;
	m_strExePath.Append(_T("FilmEveryday.exe"));
	m_strExePath.Replace(_T("\\"),_T("\\\\"));

	CString strNowVision=GetExeVision(m_strExePath);	

	TCHAR strNewVision[255];
	::GetPrivateProfileString(_T("UPDATE"),_T("VERSION"),_T("Error"),strNewVision,255,m_strFile);

	return CompareVision(strNewVision,strNowVision);
}


DWORD CUpdateDlg::FindProcessID(CString szExe)
{
	HANDLE handle=::CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
	PROCESSENTRY32 Info;
	Info.dwSize = sizeof(PROCESSENTRY32);
	if(::Process32First(handle,&Info))
	{
		do{
			CString ss=Info.szExeFile;
			if(!ss.CompareNoCase(szExe))
			{
				::CloseHandle(handle);
				return Info.th32ProcessID;
			}
		}
		while(::Process32Next(handle,&Info));	
		::CloseHandle(handle);
	}
	return -1;
}

BOOL CUpdateDlg::CloseExe()
{
	DWORD idExe1 = FindProcessID(_T("FilmEveryday.exe"));
	DWORD idExe2 = FindProcessID(_T("QvodTerminal.exe"));
	DWORD idExe3 = FindProcessID(_T("FilmAcc.exe"));

	if(theApp.m_enDlState==DL_FORCE)
	{
		AfxMessageBox(_T("天天看已升级到最新版，立即运行新版本！"),MB_OK|MB_TOPMOST);
	}

	if(idExe1!=-1||idExe2!=-1||idExe3!=-1)
	{
		if(!LookUpProcessPriviege())
			return FALSE;

		if(idExe1!=-1)
		{
			HANDLE ProcessHandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,idExe1);
			if(ProcessHandle)TerminateProcess(ProcessHandle,0);
		}

		if(idExe2!=-1)
		{
			HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,idExe2);
			if(ProcessHandle)TerminateProcess(ProcessHandle,0);
		}

		if(idExe3!=-1)
		{
			HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,idExe3);
			if(ProcessHandle)TerminateProcess(ProcessHandle,0);
		}

	}
	return TRUE;
}

BOOL CUpdateDlg::LookUpProcessPriviege()
{
  TOKEN_PRIVILEGES tkp; 
  HANDLE hToken; 
  if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
    return FALSE; 
  LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid);  tkp.PrivilegeCount = 1; 
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
  return TRUE;
}



LRESULT CUpdateDlg::OnExitDlg(WPARAM wParam,LPARAM lParam)
{
	CExitMessageDlg m_dlg;

	CWinThread* pThread=NULL;

	if(wParam!=0)
	{
		if(wParam==10&&pDownFileThread!=NULL) 	
		{
			pThread=pDownFileThread;
		}
		else if(wParam!=10)
		{		
			pThread=(CWinThread*)wParam;
		}
		if(pThread!=NULL)  pThread->SuspendThread();
	}
	INT_PTR nResponse =m_dlg.DoModal();

	if (nResponse == IDOK)
	{
		if(pThread!=NULL) 
		{
			DWORD dwCode;
			GetExitCodeThread(pDownFileThread->m_hThread, &dwCode);
			TerminateThread(pDownFileThread->m_hThread, dwCode);
			CloseHandle(pDownFileThread->m_hThread);
			pThread=NULL;
			pDownFileThread=NULL;
		}

		PostMessage(WM_CLOSE);
	}
	else if (nResponse == IDCANCEL)
	{
		if(pThread!=NULL) pThread->ResumeThread();
	}
	return TRUE;
}

LRESULT CUpdateDlg::OnCheckDlgShow(WPARAM wParam,LPARAM lParam)
{
	if(!GetFileExist(m_strFile))
	{
		if(m_dlgIntroduction.IsWindowVisible())	m_dlgIntroduction.ShowWindow(FALSE);
		m_dlgFail.ShowWindow(TRUE);		
		return FALSE;
	}

	if(m_dlgIntroduction.IsWindowVisible())		m_dlgIntroduction.ShowWindow(FALSE);

	if(IsUpdateNow())	
		m_dlgDownLoad.ShowWindow(TRUE);
	else		
		m_dlgNew.ShowWindow(TRUE);

	//if(m_dlgIntroduction.IsWindowVisible())
	//	m_dlgIntroduction.ShowWindow(FALSE);
	//	m_dlgDownLoad.ShowWindow(TRUE);


	return TRUE;
}

LRESULT CUpdateDlg::OnCheckNextDownLoad(WPARAM wParam,LPARAM lParam)
{
	//if(m_dlgCheck.IsWindowVisible())	m_dlgCheck.ShowWindow(FALSE);

	//if(IsUpdateNow())
	//	m_dlgDownLoad.ShowWindow(TRUE);
	//else
	//	m_dlgNew.ShowWindow(TRUE);
	return TRUE;
}

LRESULT CUpdateDlg::OnDownLoadNextComplete(WPARAM wParam,LPARAM lParam)
{
	if(theApp.m_enDlState==DL_FORCE) PostMessage(WM_DOWNLOAD_COMPLETE);   //强制升级 后自动关闭
	if(m_dlgDownLoad.IsWindowVisible())	m_dlgDownLoad.ShowWindow(FALSE);
	m_dlgComplete.ShowWindow(TRUE);
	return TRUE;
}

LRESULT CUpdateDlg::OnDownLoadTryAgain(WPARAM wParam,LPARAM lParam)
{
	CButton *pBtnTryAgain=m_dlgDownLoad.GetBtnTryAggin();
	pDownFileThread=NULL;
	
	pDownFileThread=AfxBeginThread(DownLoadFileThread,this);

	if(pDownFileThread==NULL) return FALSE;	
	return TRUE;
}

LRESULT CUpdateDlg::OnDownLoadAtOnce(WPARAM wParam,LPARAM lParam)
{
	pDownFileThread=AfxBeginThread(DownLoadFileThread,this);
	if(pDownFileThread==NULL) return FALSE;
	return TRUE;
}

LRESULT CUpdateDlg::OnDownLoadComplete(WPARAM wParam,LPARAM lParam)
{
	PostMessage(WM_CLOSE);
	return TRUE;
}

LRESULT CUpdateDlg::OnDownLoadFail(WPARAM wParam,LPARAM lParam)
{
	if(theApp.m_enDlState==DL_FORCE) PostMessage(WM_DOWNLOAD_COMPLETE);   //强制升级 后自动关闭
	if(m_dlgDownLoad.IsWindowVisible())	m_dlgDownLoad.ShowWindow(FALSE);
	m_dlgFail.ShowWindow(TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////

// CUpdateDlg 消息处理程序

BOOL CUpdateDlg::OnInitDialog()
{
	::SetProp(this->GetSafeHwnd(),_T("ExeEverydayUpdate"),AfxGetApp()->m_hInstance);
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	OnInit();
	GetUpdateSection();

	if(theApp.m_enDlState==DL_NORMAL)
	{
		if(theApp.IsUpdateNow()) m_dlgDownLoad.ShowWindow(TRUE);
		else m_dlgNew.ShowWindow(TRUE);
		//m_dlgIntroduction.ShowWindow(TRUE);	
	}
	else if(theApp.m_enDlState==DL_CHECK)
	{
		if(theApp.IsUpdateNow()) m_dlgDownLoad.ShowWindow(TRUE);
		else m_dlgNew.ShowWindow(TRUE);
	}
	else if(theApp.m_enDlState==DL_FORCE)
	{
		SetTrans(0);
		//删除WS_EX_APPWINDOW扩展风格
		ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
		SetTimer(WM_DOWNLOAD_MODE_FORCE,5000,NULL);
	}
	return TRUE;  
}


void CUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		OnMoveControl();
		CPaintDC pWindowDC(this);	
		CDC dcMem;
	    CBitmap Bmp,dcBmp;
		HGDIOBJ pOldBmp;

		CRect rect,rcClient;
	    GetWindowRect(&rect);
		GetClientRect(&rcClient);


		dcMem.CreateCompatibleDC(&pWindowDC);
		dcBmp.CreateCompatibleBitmap(&pWindowDC,rect.Width(),rect.Height());

		pOldBmp=dcMem.SelectObject(dcBmp);
		dcMem.BitBlt(0,0,rect.Width(),rect.Height(),&pWindowDC,0,0,SRCCOPY);


		{
			CDC MemDC;
			CBitmap *pOldBmp=NULL;
			BITMAP	bmpxy;

			MemDC.CreateCompatibleDC(&dcMem);
			MemDC.SelectObject(m_bmpBk);
			m_bmpBk.GetObject(sizeof(BITMAP),&bmpxy);
			pOldBmp = MemDC.SelectObject(&m_bmpBk);

			dcMem.BitBlt(0, 0, bmpxy.bmWidth,bmpxy.bmHeight, &MemDC,0, 0, SRCCOPY);	

			MemDC.SelectObject(pOldBmp);
			MemDC.DeleteDC();	
		}


		pWindowDC.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldBmp);
		dcMem.DeleteDC();
		ReleaseDC(&pWindowDC);
	}
}

HCURSOR CUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUpdateDlg::OnDestroy()
{
	if(theApp.m_bDownComplete&&PathIsDirectory(m_strFileBuffer))	DeleteBufferDirectory(m_strFileBuffer);
	CDialog::OnDestroy();
}

void CUpdateDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==WM_DOWNLOAD_MODE_FORCE)
	{
		KillTimer(WM_DOWNLOAD_MODE_FORCE);
		this->SendMessage(WM_DOWNLOAD_AT_ONCE);		
	}

	CDialog::OnTimer(nIDEvent);
}
