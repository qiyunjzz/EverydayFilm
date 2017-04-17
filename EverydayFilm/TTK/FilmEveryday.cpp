// FilmEveryday.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "FilmEveryday.h"
#include "MainWnd.h"
#include "CRC.h"

BOOL LookUpProcessPriviege()
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

BOOL Only1()
{
	HANDLE hMutex;
	// Create Mutex
	hMutex = CreateMutex(NULL, FALSE, _T("NewFilmeveryday"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		// 已经运行了
		return FALSE;
	}

	return TRUE;
}

BOOL IsFileExist(CString strFile)
{
	if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(strFile))
	{
		DWORD errcode = ::GetLastError();
		if(errcode==ERROR_FILE_NOT_FOUND)//文件没有找到执行下载操作
		{
			return FALSE;
		}	
	}
	return TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	CString cmdLine(lpCmdLine);
	if(!Only1())
	{
		HWND hWndPrev=::GetWindow(::GetDesktopWindow(),GW_CHILD);    
		while(::IsWindow(hWndPrev))
		{
			//判断窗口是否有我们预先设置的标记，如有，则是我们寻找的窗口，并将它激活。
			if(::GetProp(hWndPrev,_T("NewFilmEveryday")))
			{
				if(!cmdLine.IsEmpty()&&IsFileExist(cmdLine))
				{
					COPYDATASTRUCT cpd={0};
					cpd.dwData=0;
					cpd.cbData=cmdLine.GetLength()*2+2;
					if(cpd.cbData==0)	return FALSE;
					cpd.lpData=(void*)cmdLine.GetBuffer();
					::SendMessage(hWndPrev,WM_COPYDATA,0,(LPARAM)&cpd);
					cmdLine.ReleaseBuffer();					
				}
				else
				{
					COPYDATASTRUCT cpd={0};
					cpd.dwData=0;
					cpd.cbData=0;
					cpd.lpData=(void*)NULL;
					::SendMessage(hWndPrev,WM_COPYDATA,0,(LPARAM)&cpd);				
				}

				return FALSE;  //退出实例。
			}
			//继续寻找下一个窗口。
			hWndPrev = ::GetWindow(hWndPrev,GW_HWNDNEXT); 
		}
		return FALSE;
	}


	CPaintManagerUI::SetResourceInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CMainWnd* pFrame = new CMainWnd();
	if( pFrame == NULL ) return 0;

	pFrame->Create(NULL, _T("天天看高清影视"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 996, 660);
	pFrame->CenterWindow();
	pFrame->ShowWindow(SW_SHOW);
	::SetForegroundWindow(pFrame->GetHWND());//将应用程序的主窗口激活。

	int TopType = ::GetPrivateProfileInt(_T("memory"),_T("top"),3,pFrame->m_cfgPath);	
	BOOL bShow  = TRUE;

	if(TopType==1||TopType==2)
	{
		if(TopType==1&&!cmdLine.IsEmpty()&&IsFileExist(cmdLine))
		{
			::SetWindowPos(pFrame->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 置顶
			bShow = FALSE;
		}
		else if(TopType==2)
		{
			::SetWindowPos(pFrame->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 置顶	
			bShow = FALSE;
		}
	}
	
	if(bShow||TopType==3)									
	{
		::SetWindowPos(pFrame->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 置顶
		::SetWindowPos(pFrame->GetHWND(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // 取消置顶
	}

	pFrame->ShowWindow(SW_SHOW);

	if(!cmdLine.IsEmpty()&&IsFileExist(cmdLine))
	{
		int len=cmdLine.GetLength();
		cmdLine=cmdLine.Mid(1,len-2);
		pFrame->m_strComm=cmdLine;
	}

	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}