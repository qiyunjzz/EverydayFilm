// Apply.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Apply.h"
#include "ApplyDlg.h"
#include "CRC.H"

#include "shlwapi.h" 
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplyApp

BEGIN_MESSAGE_MAP(CApplyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CApplyApp 构造

CApplyApp::CApplyApp()
{
	m_strIcon.Empty();

	TCHAR strPath[MAX_PATH];
	::GetModuleFileName(NULL,strPath,MAX_PATH);
	strURL=_T("res://");			// 定位文件中的HTML资源
	strURL+=strPath;
	strURL += _T("/HTML_LOADING");

	m_sCommandLine.m_bShield = FALSE;
}

CApplyApp theApp;


BOOL CApplyApp::InitInstance()
{

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	if(!Parse_Command())	return FALSE;

	if(m_sCommandLine.m_strClassHandle.IsEmpty())
	{
		m_sCommandLine.m_strClassHandle=_T("EverydayApply");
	}

	if(!OnlyOneExe())
	{
		HWND hWndPrev=::GetWindow(::GetDesktopWindow(),GW_CHILD);    
		
		while(::IsWindow(hWndPrev))
		{
			if(::GetProp(hWndPrev,m_sCommandLine.m_strClassHandle))
			{
				if(::IsIconic(hWndPrev))	::SendMessage(hWndPrev,WM_SYSCOMMAND,SC_RESTORE,0);
				else 	::ShowWindow(hWndPrev,SW_SHOW);
				::SetForegroundWindow(hWndPrev);//将应用程序的主窗口激活。
				return FALSE;  //退出实例。
			}
			//继续寻找下一个窗口。
			hWndPrev = ::GetWindow(hWndPrev,GW_HWNDNEXT); 
		}
		return FALSE;	
	}

#ifdef _DEBUG
#else
	LPBYTE pData = NULL;
	DWORD dwSize = 0;

	TCHAR* strExepath = new TCHAR[MAX_PATH + 1]; 
	GetModuleFileName(NULL, strExepath, MAX_PATH); 
	(_tcsrchr(strExepath, _T('\\')))[1] = 0;//删除文件名，只获得路径
	_tcscat(strExepath,_T("Codecs\\Source.dll"));

	HANDLE hFile = ::CreateFile(strExepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
		FILE_ATTRIBUTE_NORMAL, NULL);
	delete strExepath;
	if( hFile == INVALID_HANDLE_VALUE ) return NULL;
	dwSize = ::GetFileSize(hFile, NULL);
	if( dwSize == 0 ) return NULL;
	DWORD dwRead = 0;
	pData = new BYTE[ dwSize ];
	::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
	::CloseHandle( hFile );
	ULONG longCrc=CRC((CHAR*)pData,dwSize); // Creates a CRC from a string buffer
	delete pData;
	if(0xAD72C126!=longCrc) return FALSE;
#endif

	if(	m_sCommandLine.m_strName.IsEmpty()      ||
		m_sCommandLine.m_strWeb.IsEmpty()) return FALSE;

	if(m_sCommandLine.m_szSize.cx<=0||m_sCommandLine.m_szSize.cy<=0) return FALSE;

	if(m_sCommandLine.m_strEs.IsEmpty())
		m_sCommandLine.m_strEs=_T("天天看应用中心");

	//if(m_sCommandLine.m_strAdUrl.IsEmpty())
	//	m_sCommandLine.m_strAdUrl = _T("about:blank");


	CString strIconName=m_sCommandLine.m_strIconLink;
	int iPosName=strIconName.ReverseFind(_T('/'));
	strIconName.Delete(0,iPosName+1);
	m_strIcon=GetExeModulePath();
	m_strIcon.Append(_T("\\icons\\")+m_sCommandLine.m_strClassHandle+_T("\\")+strIconName);

	if(!IsFileExist(m_strIcon))
	{
		if(!JudgeDirectoryAndCreateMulDirectory(m_strIcon)) m_strIcon.Empty();; 

		if(!m_strIcon.IsEmpty()&&!OnGetUpdateFile(m_sCommandLine.m_strIconLink,m_strIcon)) m_strIcon.Empty();
	}

	CString strSkin=GetExeModulePath()+_T("\\Skin\\apply_default.sk");
	if(!IsFileExist(strSkin)) AfxMessageBox(_T("皮肤不存在"));

	m_renderEngine.SetSkin(strSkin);
	pShowWnd.CreatePlayWnd();
	m_pMainWnd=&pShowWnd;
	pShowWnd.ShowWindow(TRUE);

	//CApplyDlg dlg;
	//m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//}
	return TRUE;
}

BOOL CApplyApp::OnlyOneExe()
{
	// Create Mutex
	if(m_sCommandLine.m_strClassHandle.IsEmpty()) return TRUE;
	HANDLE hMutex = CreateMutex(NULL, FALSE,m_sCommandLine.m_strClassHandle);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return FALSE;
	}
	return TRUE;
}


int CApplyApp::OnGetUpdateFile(CString szURL, CString Filename)
{
    CInternetSession session;
    CInternetFile* file = NULL;

	session.SetOption(   INTERNET_OPTION_CONNECT_TIMEOUT,   200   );     //设置超时时间 
    session.SetOption(   INTERNET_OPTION_CONNECT_RETRIES,   0);     //设置重试次数
    try
    {
        file = (CInternetFile*)session.OpenURL(szURL); 
    }
    catch (CInternetException* m_pException)
    {
        file = NULL; 
        m_pException->Delete();
        return FALSE;
    }

    BOOL bRet = FALSE;
	ULONGLONG fLen=0;  
	try
	{
		fLen=file->SeekToEnd();//fLen是文件的正确长度   
	}
	catch(CInternetException * m_pException)
	{
		if(file)
		{
			file->Close();
			delete file;
		}
		return FALSE;
	}
	try
	{
		file->SeekToBegin();
	}
	catch(CInternetException *m_pException)
	{
		if(file)
		{
			file->Close();
			delete file;
		}
		return FALSE;
	}
	   
    CStdioFile dataStore;
    if(dataStore.Open(Filename,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite | CFile::typeBinary))
    {
        if (file)
        {
			UINT pos=0;
            UINT somelen = 0;
            BYTE somecode[2048];
            while((somelen = file->Read(somecode, 2048)) > 0)
            {
                dataStore.Write(somecode, somelen);
            }
            bRet = TRUE;
        }
        dataStore.Close();
    }
    if(file)
    {
        file->Close();
        delete file;
    }

    return bRet;
}

BOOL CApplyApp::IsFileExist(CString strFile)
{
	CFileStatus   status; 
	strFile.Replace(_T("\\"),_T("\\\\"));
     if(!CFile::GetStatus(strFile,status)) return FALSE;

	return TRUE;
}

CString CApplyApp::GetExeModulePath(HINSTANCE hInstance)
{
	TCHAR szFilePath[MAX_PATH + 1]; 
	GetModuleFileName(hInstance, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
	CString str_url = szFilePath;
	return str_url;
}

BOOL CApplyApp::JudgeDirectoryAndCreateMulDirectory(CString strPath)
{
	if(PathIsDirectory(strPath))  return TRUE;

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


BOOL CApplyApp::Parse_Command()
{
	//command line start;
	LPTSTR  strCommand=GetCommandLine();
	int iCommand=0;

	 LPWSTR *strArgv =CommandLineToArgvW(strCommand,&iCommand);
	
	if (iCommand < 2)  return FALSE;

	for(int i=1;i<iCommand;i++)
	{
		Parse_String(strArgv[i]);
	}

 	// 	Parse_String(_T("handle=18"));
		//Parse_String(_T("name=豆瓣电台"));
		//Parse_String(_T("web=http://cdn.ttkvod.com/123/ttkvodtv2012/live.html"));
		//Parse_String(_T("iconlink=http://192.168.1.110:82/ico/dbdt.ico"));
		//Parse_String(_T("es=豆瓣电台"));
		//Parse_String(_T("size=888,666"));
		//Parse_String(_T("Scroll=TRUE"));
		//Parse_String(_T("Shield=TRUE"));
		//Parse_String(_T("AdUrl=http://192.168.1.110:81/test.php"));

	return TRUE;
}

void CApplyApp::StringToSize(CString strVal)
{
	int i;
	if(strVal.IsEmpty()) return;

	if((i=strVal.Find(_T(',')))==-1) return;

	m_sCommandLine.m_szSize.cx=_ttoi(strVal.Left(i))+4;
	strVal.Delete(0,i+1);

	m_sCommandLine.m_szSize.cy=_ttoi(strVal)+60;
}

void CApplyApp::Parse_String(LPCTSTR pstrVal)
{
    CString strVal = pstrVal;
	CString sItem;
    CString sValue;

	if(strVal.IsEmpty()) return;

	int iPos=strVal.Find(_T('='));
	sItem=strVal.Left(iPos);
	strVal.Delete(0,iPos+1);
	sValue=strVal;


	if( sItem == _T("handle")) 
	{
		m_sCommandLine.m_strClassHandle = sValue;
    }
    else if( sItem == _T("name") ) 
	{
		m_sCommandLine.m_strName = sValue+_T("|天天看应用中心");
    }
	else if( sItem == _T("web") ) 
	{
		m_sCommandLine.m_strWeb = sValue;
    }
	else if( sItem == _T("iconlink") ) 
	{
		m_sCommandLine.m_strIconLink = sValue;
    }
	else if( sItem == _T("es") ) 
	{
		m_sCommandLine.m_strEs = sValue;
	}
    else if( sItem == _T("size") ) 
    {
		StringToSize(sValue); 
    }
	else if( sItem == _T("Scroll") )
	{
		if(_tcscmp(sValue,_T("TRUE"))==0||_tcscmp(sValue,_T("true"))==0) 
			m_sCommandLine.m_bScroll=TRUE;
	}
	else if( sItem == _T("Shield"))
	{
		if(_tcscmp(sValue,_T("TRUE"))==0||_tcscmp(sValue,_T("true"))==0) 
			m_sCommandLine.m_bShield=TRUE;	
	}
	else if( sItem == _T("AdUrl"))
	{
		m_sCommandLine.m_strAdUrl = sValue;
	}		
}
