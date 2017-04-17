// DownLoadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "DownLoadDlg.h"
#include "EventID.h"

#include "MessageDlg.h"


IMPLEMENT_DYNAMIC(CDownLoadDlg, CDialog)

CDownLoadDlg::CDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadDlg::IDD, pParent)
{

}

CDownLoadDlg::~CDownLoadDlg()
{
	if(m_Font.m_hObject!=NULL)
		m_Font.DeleteObject();
}

void CDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDownLoadDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_NEXT, &CDownLoadDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CDownLoadDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(ID_BTN_TRY_AGAIN, &CDownLoadDlg::OnBnClickedBtnTryAgain)
	ON_BN_CLICKED(ID_BTN_LOOK_CAREFULLY, &CDownLoadDlg::OnBnClickedBtnLookCarefully)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CDownLoadDlg::OnBnClickedBtnNext()
{
	GetParent()->SendMessage(WM_DOWNLOAD_AT_ONCE);
}

void CDownLoadDlg::OnBnClickedBtnCancel()
{
	GetParent()->SendMessage(WM_UPDATE_CANCEL,10);
}

void CDownLoadDlg::OnBnClickedBtnTryAgain()
{
	GetParent()->SendMessage(WM_DOWNLOAD_TRY_AGAIN);
}

CProgressCtrl* CDownLoadDlg::GetProgressCtrlDown()
{		
	return (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_DOWN);
}

CListBox* CDownLoadDlg::GetListBoxDown()
{
	return (CListBox *)GetDlgItem(IDC_PROGRESS_CONNECTION_SERVER);
}

CButton* CDownLoadDlg::GetBtnAtOnce()
{
	return (CButton *)GetDlgItem(ID_BTN_NEXT);
}

CButton* CDownLoadDlg::GetBtnTryAggin()
{
	return (CButton *)GetDlgItem(ID_BTN_TRY_AGAIN);
}

CStatic *   CDownLoadDlg::GetStaticShow()
{
	return (CStatic *)GetDlgItem(IDC_STATIC_DOWNSHOW);
}

void CDownLoadDlg::OnBnClickedBtnLookCarefully()
{
	GetDlgItem(ID_BTN_LOOK_CAREFULLY)->ShowWindow(FALSE);
	GetDlgItem(IDC_PROGRESS_CONNECTION_SERVER)->ShowWindow(TRUE);
}

BOOL CDownLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_PROGRESS_CONNECTION_SERVER)->ShowWindow(FALSE);
	GetDlgItem(ID_BTN_LOOK_CAREFULLY)->ShowWindow(TRUE);

	GetDlgItem(ID_BTN_NEXT)->ShowWindow(TRUE);
	GetDlgItem(ID_BTN_TRY_AGAIN)->ShowWindow(FALSE);

	m_Font.CreateFont(12,0,0,0,FW_NORMAL/*FW_BOLD*/,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("宋体"));   
	return TRUE;
}

void CDownLoadDlg::OnPaint()
{
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

	m_strUpTip.Format(_T("发现有天天看最新%s，您是否需要立即升级？"),theApp.m_strFileVision);
	
	HFONT hOldFont = (HFONT) ::SelectObject(dcMem.m_hDC, m_Font);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.DrawText(m_strUpTip,CRect(4,10,rect.Width(),25),DT_EDITCONTROL|DT_WORDBREAK|DT_CENTER);
	::SelectObject(dcMem.m_hDC, hOldFont);	

	pWindowDC.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	ReleaseDC(&pWindowDC);
}
