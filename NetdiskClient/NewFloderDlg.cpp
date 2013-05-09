// NewFloderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "NewFloderDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

// CNewFloderDlg dialog

IMPLEMENT_DYNAMIC(CNewFloderDlg, CDialogEx)

CNewFloderDlg::CNewFloderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewFloderDlg::IDD, pParent)
{

	m_strNewFloderName = _T("");
}

CNewFloderDlg::~CNewFloderDlg()
{
}

void CNewFloderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_FLODERNAME, m_str);
	DDX_Text(pDX, IDC_EDIT_FLODERNAME, m_strNewFloderName);
}


BEGIN_MESSAGE_MAP(CNewFloderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OK, &CNewFloderDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CNewFloderDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CNewFloderDlg message handlers


void CNewFloderDlg::OnBnClickedBtnOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strNewFloderName == _T(""))
	{
		AfxMessageBox(_T("输入不能为空！"));
		return;
	}
	//发送创建文件夹的信息
	CString path=m_pMainDlg->m_strCurrentPath+_T("\\")+m_strNewFloderName;
	if(m_pMainDlg->m_Client.CreateNewFloder(path))
	{
		//更新客户端显示
		m_pMainDlg->OnBnClickedBtnRefresh();
	}

	CDialogEx::OnOK();
}


void CNewFloderDlg::OnBnClickedBtnCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
