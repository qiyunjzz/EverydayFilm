// MessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "MessageDlg.h"


// CMessageDlg 对话框

IMPLEMENT_DYNAMIC(CMessageDlg, CDialog)

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{

}

CMessageDlg::~CMessageDlg()
{
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CMessageDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDOK, &CMessageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CMessageDlg::OnBnClickedOk2)
END_MESSAGE_MAP()


// CMessageDlg 消息处理程序

BOOL CMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(NULL, TRUE);			// 设置大图标
	SetIcon(NULL, FALSE);		// 设置小图标

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMessageDlg::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CMessageDlg::OnBnClickedOk()
{
	OnOK();
}

void CMessageDlg::OnBnClickedOk2()
{
	OnOK();
}
