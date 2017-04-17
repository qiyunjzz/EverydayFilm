// NewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Update.h"
#include "NewDlg.h"


// CNewDlg 对话框
#define ID_STATIC_SITE 2000

IMPLEMENT_DYNAMIC(CNewDlg, CDialog)

CNewDlg::CNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDlg::IDD, pParent)
{

}

CNewDlg::~CNewDlg()
{
}

void CNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_OK, &CNewDlg::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// CNewDlg 消息处理程序

BOOL CNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bmpBk.LoadBitmap(IDB_BITMAP_NEW);

	//m_linkSite.SetLink(TRUE);
	//m_linkSite.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,ID_STATIC_SITE);
	//m_linkSite.SetURL(_T("http://bbs.ttkvod.com"));
	//m_linkSite.SetWindowText(_T("http://bbs.ttkvod.com"));
	//m_linkSite.SetColours(RGB(0,0,255),RGB(0,0,255),RGB(0,0,125));	

	return TRUE; 
}

void CNewDlg::OnPaint()
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


	if(m_bmpBk.m_hObject!=NULL)
	{
		CDC MemDC;
		CBitmap *pOldBmp=NULL;
		BITMAP	bmpxy;

		MemDC.CreateCompatibleDC(&dcMem);
		MemDC.SelectObject(m_bmpBk);
		m_bmpBk.GetObject(sizeof(BITMAP),&bmpxy);
		pOldBmp = MemDC.SelectObject(&m_bmpBk);

		dcMem.BitBlt(0, 0, bmpxy.bmWidth,bmpxy.bmHeight, &MemDC,0, 0, SRCCOPY);	

		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();	
	}

	pWindowDC.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBmp);
	dcMem.DeleteDC();
	ReleaseDC(&pWindowDC);

	this->GetDlgItem(ID_BTN_OK)->MoveWindow(rect.Width()-109,rect.Height()-40,88,25);
}

void CNewDlg::OnBnClickedBtnOk()
{
	GetParent()->SendMessage(WM_CLOSE);
}
