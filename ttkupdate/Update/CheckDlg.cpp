// CheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "CheckDlg.h"
#include "EventID.h"




// 下载任务的线程函数
UINT __cdecl ThreadProcConnectionServer(LPVOID lpParameter)
{
	CCheckDlg *pCheck = (CCheckDlg*)lpParameter;
	ASSERT(pCheck);
	pCheck->ShowProcessCtrl();
	TRACE(_T("执行完毕\n"));
	return TRUE;
}

// CCheckDlg 对话框

IMPLEMENT_DYNAMIC(CCheckDlg, CDialog)

CCheckDlg::CCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckDlg::IDD, pParent)
{

}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CCheckDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CCheckDlg 消息处理程序

void CCheckDlg::OnBnClickedBtnCancel()
{
	GetParent()->SendMessage(WM_UPDATE_CANCEL,(WPARAM)pCheckThread);
}

BOOL CCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CProgressCtrl *pProcess = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_CONNECTION_SERVER);
	pProcess->SetRange(0,100);

	return TRUE; 
}


void CCheckDlg::ShowWindowEx(BOOL m_bShow)
{
	ShowWindow(m_bShow);
	if(!m_bShow) 	return;

	pCheckThread=NULL;
	pCheckThread=AfxBeginThread(ThreadProcConnectionServer,this);
}

void CCheckDlg::ShowProcessCtrl()
{
	CProgressCtrl *pProcess = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_CONNECTION_SERVER);
	for(int i=1;i<=100;i++)
	{
		pProcess->SetPos(i);
		Sleep(20);
	}
	GetParent()->SendMessage(WM_CHECK_NEXT_DOWNLOAD);
}