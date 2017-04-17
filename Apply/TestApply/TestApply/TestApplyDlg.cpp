// TestApplyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestApply.h"
#include "TestApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	//handle='7758'|name='天天应用'|web='www.baidu.com'|icon='logo_16.ico'|iconlink='www.linkcon.com'|iconsavepath='\\apps\\200002017\\' 
	//|es='今天天气很好'|size='800,400' "  
	// 字符格式必须紧凑，不能有多余空格或tab 以及 ‘|’  
	// 句柄、程序名、导入网站、图标、 图标下载地址、图标保存目录（相对路径）、附加信息、程序大小


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestApplyDlg 对话框




CTestApplyDlg::CTestApplyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestApplyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestApplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_str);
}

BEGIN_MESSAGE_MAP(CTestApplyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestApplyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestApplyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestApplyDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestApplyDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestApplyDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestApplyDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTestApplyDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestApplyDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestApplyDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CTestApplyDlg 消息处理程序

BOOL CTestApplyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	strPath=GetExeModulePath();
	strPath.Append(_T("//Apply.exe"));

	m_str.SetWindowText(_T("webFuntion=fund handle=7765 name=脑筋急转弯 web=http://360.tuyaya.com/bd_jizhuanwan.php/ iconlink=http://www.zhidao001.com/home.gif es=脑筋急转弯 size=540,340"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestApplyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestApplyDlg::OnPaint()
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
HCURSOR CTestApplyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString CTestApplyDlg::GetExeModulePath(HINSTANCE hInstance)
{
	TCHAR szFilePath[MAX_PATH + 1]; 
	AfxGetInstanceHandle();
	GetModuleFileName(hInstance, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
	CString str_url = szFilePath;
	return str_url;
}


void CTestApplyDlg::OnBnClickedButton1()
{
	strVal.Empty();

	strVal=_T("handle=7758 name=快递查询 web=http://baidu.kuaidi100.com/ iconlink=http://www.zhidao001.com/home.gif es=快递查询，方便前往家 size=800,380");


	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton2()
{
	strVal.Empty();

	strVal=_T("handle=7759 name=电视直播 web=http://topic.pptv.com/partner/baiduapp/live/main/ iconlink=http://www.zhidao001.com/home.gif es=今天天气特别地很好 size=900,493");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton3()
{
	strVal.Empty();

	strVal=_T("handle=7760 name=凤凰视频 web=http://v.ifeng.com/app/ iconlink=http://www.zhidao001.com/home.gif es=凤凰卫视看世界真实新闻 size=940,630");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton4()
{
	strVal.Empty();

	strVal=_T("handle=7761 name=天气预报 web=http://shenghuo.360.cn/tq/yb iconlink=http://www.zhidao001.com/home.gif es=天气预报 size=400,600");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

//////////////////////////////////////////////////////////////
///////////
void CTestApplyDlg::OnBnClickedButton5()
{
	strVal.Empty();

	
	strVal=_T("handle=7762 name=在线地图 web=http://ditu.google.cn/maps?hl=zh-CN&tab=ll iconlink=http://www.zhidao001.com/home.gif es=在线地图 size=1380,746");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton6()
{
	strVal.Empty();

	
	strVal=_T("handle=7763 name=列车时刻表 web=http://lieche.58.com/app/search.html iconlink=http://www.zhidao001.com/home.gif es=列车时刻表 size=800,600");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton7()
{
	strVal.Empty();

	
	strVal=_T("handle=7764 name=股票行情 web=http://app.hexun.com/market/ iconlink=http://www.zhidao001.com/home.gif es=股票行情 size=770,622");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton8()
{
	strVal.Empty();

	
	strVal=_T("handle=7765 name=脑筋急转弯 web=http://360.tuyaya.com/bd_jizhuanwan.php/ iconlink=http://www.zhidao001.com/home.gif es=脑筋急转弯 size=540,340");

	ShellExecute(NULL,_T("open"),strPath,strVal,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strValue;
	m_str.GetWindowText(m_strValue);
	ShellExecute(NULL,_T("open"),strPath,m_strValue,NULL,SW_SHOWNORMAL);
}

void CTestApplyDlg::OnStnClickedStatic1()
{
	// TODO: 在此添加控件通知处理程序代码
}
