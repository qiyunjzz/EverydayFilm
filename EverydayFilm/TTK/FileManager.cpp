#include "StdAfx.h"
#include "FileManager.h"
__int64 fullLength;
int FileNum;
DWORD WINAPI ErgodicPath( LPVOID lpParam )
{
	CFileManager *pWnd = (CFileManager *)lpParam;
	TCHAR tmp[MAX_PATH] = {0};
	::GetPrivateProfileString(_T("QVODTERMINAL"),_T("mediapath"),_T(""),tmp,MAX_PATH,pWnd->m_qvodIni);

	int max = ::GetPrivateProfileInt(_T("memory"),_T("maxedit"),5,pWnd->m_cfgPath);
	pWnd->ErgodicDir(tmp,fullLength,FileNum);
	__int64 a = (__int64)max*1024*1024*1024;
	if(fullLength>=a)
		::PostMessage(pWnd->GetHWND(),WM_MYMESSAGE,0,0);
	return 1;
}
//////////////thread proc////////////////////////////

CFileManager::CFileManager(void)
{
	m_pm = new CPaintManagerUI;
}

CFileManager::~CFileManager(void)
{
}

void CFileManager::Init()
{
	HANDLE hThread;
	DWORD ThreadId;
	hThread = CreateThread(NULL,
		0,
		ErgodicPath,
		this,
		0,
		&ThreadId
		);
	::CloseHandle(hThread);

	m_btnClose = static_cast<CButtonUI*>(m_pm->FindControl(_T("btnClose")));
	m_btnOpenpath = static_cast<CButtonUI*>(m_pm->FindControl(_T("openpath")));
	m_btnNomore = static_cast<CButtonUI*>(m_pm->FindControl(_T("nomore")));
	m_btnCancel = static_cast<CButtonUI*>(m_pm->FindControl(_T("cancel")));
	m_Label = static_cast<CLabelUI*>(m_pm->FindControl(_T("t2")));
	return ;
}

void CFileManager::Notify(TNotifyUI& msg)
{
	if( msg.sType == MST_WINDOWINIT ) 
	{
		OnPrepare();
	}
	else if(msg.sType == MST_CLICK)
	{
		if (msg.pSender == m_btnClose)
		{
			ShowWindow(false,false);
		}
		else if(msg.pSender == m_btnOpenpath)
		{
			TCHAR tmp[MAX_PATH] = {0};
			::GetPrivateProfileString(_T("QVODTERMINAL"),_T("mediapath"),_T(""),tmp,MAX_PATH,m_qvodIni);
			ShellExecute(NULL,_T("open"),tmp,NULL,NULL,SW_SHOWNORMAL);
		}
		else if(msg.pSender == m_btnNomore)
		{
			::WritePrivateProfileString(_T("memory"),_T("delstartup"),_T("0"),m_cfgPath);
			ShowWindow(false,false);
		}
		else if(msg.pSender == m_btnCancel)
		{
			ShowWindow(false,false);
		}
	}
}

LRESULT CFileManager::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_pm->Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create((LPCTSTR)skn,_T("filemanager.xml"), (UINT)0, NULL, m_pm);
	ASSERT(pRoot && "Failed to parse xml");
	m_pm->AttachDialog(pRoot);
	m_pm->AddNotifier(this);

	SIZE szInitWindowSize = m_pm->GetInitSize();
	if( szInitWindowSize.cx != 0 ) 
	{
		::SetWindowPos(*this, NULL, 0, 0, szInitWindowSize.cx, szInitWindowSize.cy, SWP_NOZORDER | SWP_NOMOVE);
	}

	Init();
	return 0;
}

LRESULT CFileManager::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CFileManager::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CFileManager::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CFileManager::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm->GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_pm->FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CFileManager::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm->GetRoundCorner();
	if (this == NULL)
	{
		return 0;
	}
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
	{
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn2, TRUE);
		::DeleteObject(hRgn2);
	}
	bHandled = FALSE;
	return 0;
}
LRESULT CFileManager::OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rtClient;
	POINTS pt	= MAKEPOINTS(lParam);
	int x = (int)(short) LOWORD(lParam);
	int y = (int)(short) HIWORD(lParam);
	::GetWindowRect(m_hWnd,&rtClient);
	return MoveWindow((int)(short) LOWORD(lParam),(int)(short) HIWORD(lParam),rtClient.right - rtClient.left,rtClient.bottom - rtClient.top,FALSE);;
}
LRESULT CFileManager::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CFileManager::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MOVE:			lRes = OnMoveWindow(uMsg,wParam,lParam);break;
	case WM_MYMESSAGE:      lRes = OnCheckOver(uMsg,wParam,lParam,bHandled);break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) 
		return lRes;

	if( m_pm->MessageHandler(uMsg, wParam, lParam, lRes) ) 
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CFileManager::ErgodicDir(CStdString path,__int64& FullSize,int& FileNum)
{
	std::stack<CStdString> find_file;//栈先进后出
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
	FullSize = 0;
	FileNum = 0;
    
    find_file.push(path);

    while(!find_file.empty())
    {
        CStdString find_path = find_file.top();        
        find_file.pop();
        
        //访问
        //std::string dir;
        //int index_of_slash=find_path.rfind('\\');
        //dir=find_path.substr(index_of_slash+1,find_path.length()-index_of_slash);
        //for(int i=0;i<find_file.size();i++)
        //    cout<<"   ";
        //cout << dir << endl;
        
        //查找子目录/文件
        find_path=find_path+_T("\\*.*");
        
        hFind=::FindFirstFile(find_path,&FindFileData);
        if(hFind == INVALID_HANDLE_VALUE)
            continue;
        do
        {
            //非目录直接输出，目录的话则压栈
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if(_tcscmp(FindFileData.cFileName,_T("."))==0||
                    _tcscmp(FindFileData.cFileName,_T(".."))==0)
                    continue;

				find_path=find_path.Left(find_path.ReverseFind('\\'));
                find_path=find_path+_T("\\")+FindFileData.cFileName;
                find_file.push(find_path);                 
            }
            else
            {
				//static __int64 all=0;
                //for(int i=0;i<=find_file.size();i++)
                //    cout<<"   ";
                //cout<<FindFileData.cFileName<<endl;
				FullSize+=(FindFileData.nFileSizeHigh * (MAXDWORD+1)) + FindFileData.nFileSizeLow;
				FileNum++;
            }
        }
        while(FindNextFile(hFind,&FindFileData));    
    }
}
LRESULT CFileManager::OnCheckOver(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::MoveWindow(m_hWnd,0,0,430,235,FALSE);
	CenterWindow();
	CStdString txt;
	int bigmin = fullLength/1024/1024;
	txt.Format(_T("您当前的缓存目录下共用 %d 个文件，大小约为 %d MB"),FileNum,bigmin);
	m_Label->SetText(txt);
	return 1;
}
