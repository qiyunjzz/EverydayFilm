// FailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "FailDlg.h"
#include "EventID.h"


// CFailDlg 对话框

IMPLEMENT_DYNAMIC(CFailDlg, CDialog)

CFailDlg::CFailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFailDlg::IDD, pParent)
{

}

CFailDlg::~CFailDlg()
{
	if(m_Font.m_hObject!=NULL)
		m_Font.DeleteObject();
}

void CFailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFailDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_OK, &CFailDlg::OnBnClickedBtnOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CFailDlg 消息处理程序

void CFailDlg::OnBnClickedBtnOk()
{
	GetParent()->SendMessage(WM_DOWNLOAD_COMPLETE);
}

BOOL CFailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_linkSite.SetLink(TRUE);
	m_linkSite.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,ID_STATIC_SITE);
	m_linkSite.SetURL(_T("http://www.ttkvod.com"));
	m_linkSite.SetWindowText(_T("http://www.ttkvod.com"));
	m_linkSite.SetColours(RGB(0,0,255),RGB(0,0,255),RGB(0,0,125));
	
	m_Font.CreateFont(15,0,0,0,/*FW_NORMAL*/FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("宋体"));   
	return TRUE;
}

void CFailDlg::OnPaint()
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

	HFONT hOldFont = (HFONT) ::SelectObject(dcMem.m_hDC, m_Font);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.DrawText(_T("升级失败，请手动下载新版！"),CRect(4,55,rect.Width(),70),DT_EDITCONTROL|DT_WORDBREAK|DT_CENTER);
	::SelectObject(dcMem.m_hDC, hOldFont);
	
	m_linkSite.SetBkGnd(&dcMem);

	pWindowDC.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	ReleaseDC(&pWindowDC);
	
	m_linkSite.MoveWindow(98,158,208,13);
}
