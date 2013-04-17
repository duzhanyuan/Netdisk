// DateShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "DateShowDlg.h"
#include "afxdialogex.h"


// CDateShowDlg dialog

IMPLEMENT_DYNAMIC(CDateShowDlg, CDialog)

CDateShowDlg::CDateShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateShowDlg::IDD, pParent)
{

}

CDateShowDlg::~CDateShowDlg()
{
}

void CDateShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHSHOW, m_monthShow);
}


BEGIN_MESSAGE_MAP(CDateShowDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDateShowDlg message handlers


void CDateShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(GetDlgItem(IDC_MONTHSHOW))
	{
		CRect rect;
		GetDlgItem(IDC_MONTHSHOW)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		rect.left=rect.left*cx/m_rect.Width();
		rect.right=rect.right*cx/m_rect.Width();
		rect.bottom=rect.bottom*cy/m_rect.Height();
		rect.top=rect.top*cy/m_rect.Height();
		GetDlgItem(IDC_MONTHSHOW)->MoveWindow(&rect);
	}
	GetClientRect(&m_rect);

	// TODO: Add your message handler code here
}


BOOL CDateShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetClientRect(&m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
