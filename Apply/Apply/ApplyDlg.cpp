// ApplyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Apply.h"
#include "ApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CApplyDlg 对话框
/////////////////////////////////////////////////////////////////////////////////////////////
////////
typedef   BOOL   (FAR   PASCAL   *   FUNCTransparent)(   
											HWND   hwnd,                       //   handle   to   the   layered   window   
											COLORREF   crKey,             //   specifies   the   color   key   
											BYTE   bAlpha,                   //   value   for   the   blend   function   
											DWORD   dwFlags                 //   action   
											);   

void CApplyDlg::SetTrans(int itrans)
{
	HMODULE   hModule   =   GetModuleHandle(_T("user32.dll"));   
    FUNCTransparent   SetLayeredWindowAttributes;   
    SetLayeredWindowAttributes= (FUNCTransparent)GetProcAddress(hModule,"SetLayeredWindowAttributes");      
     //  设置分层扩展标记   
    SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|0x80000L);   
    //   70%   alpha   
    SetLayeredWindowAttributes(GetSafeHwnd(),   0,   (255   *   itrans)   /   100,   0x2); 

}



CApplyDlg::CApplyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApplyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon=(HICON)::LoadImage(0,_T("res\\Apply.ico"),IMAGE_ICON,   16,   16, LR_LOADFROMFILE );
}

void CApplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CApplyDlg::NewShowWnd()
{
	//m_renderEngine.SetSkin();
	CShowWnd* pShowWnd=new CShowWnd();
	pShowWnd->CreatePlayWnd();
	pShowWnd->ShowWindow(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////
BEGIN_MESSAGE_MAP(CApplyDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CApplyDlg 消息处理程序

BOOL CApplyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	SetTrans(0);
	//删除WS_EX_APPWINDOW扩展风格
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	NewShowWnd();
	NewShowWnd();
	NewShowWnd();
	NewShowWnd();

	SetTimer(100,500,NULL);
	return TRUE;  
}



void CApplyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CApplyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CApplyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nIDEvent==100)
	{
		this->ShowWindow(FALSE);
		KillTimer(100);
	
	}

	CDialog::OnTimer(nIDEvent);
}
