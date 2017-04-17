// CompleteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "CompleteDlg.h"
#include "EventID.h"


// CCompleteDlg 对话框

IMPLEMENT_DYNAMIC(CCompleteDlg, CDialog)

CCompleteDlg::CCompleteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompleteDlg::IDD, pParent)
{

}

CCompleteDlg::~CCompleteDlg()
{
	if(m_Font.m_hObject!=NULL)
		m_Font.DeleteObject();
}

void CCompleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCompleteDlg, CDialog)
	ON_BN_CLICKED(ID_BTN_OK, &CCompleteDlg::OnBnClickedBtnOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CCompleteDlg::OnBnClickedBtnOk()
{
	GetParent()->SendMessage(WM_DOWNLOAD_COMPLETE);
}

void CCompleteDlg::OnPaint()
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
	dcMem.DrawText(_T("升级成功，欢迎体验！"),CRect(4,55,rect.Width(),70),DT_EDITCONTROL|DT_WORDBREAK|DT_CENTER);
	::SelectObject(dcMem.m_hDC, hOldFont);
	
	m_linkSite.SetBkGnd(&dcMem);

	pWindowDC.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	ReleaseDC(&pWindowDC);
	
	m_linkSite.MoveWindow(20,158,208,13);
}

BOOL CCompleteDlg::OnInitDialog()
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
