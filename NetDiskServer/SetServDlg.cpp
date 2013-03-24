// SetServDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "SetServDlg.h"
#include "afxdialogex.h"


// CSetServDlg dialog

IMPLEMENT_DYNAMIC(CSetServDlg, CDialogEx)

CSetServDlg::CSetServDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetServDlg::IDD, pParent)
{

	m_strServAddr = _T("");
	m_iServPort = 0;
}

CSetServDlg::~CSetServDlg()
{
}

void CSetServDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVADDR, m_strServAddr);
	DDX_Text(pDX, IDC_EDIT_SERVPORT, m_iServPort);
}


BEGIN_MESSAGE_MAP(CSetServDlg, CDialogEx)
END_MESSAGE_MAP()


// CSetServDlg message handlers


BOOL CSetServDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//��ʼ����������ַ���˿ں�
	m_strServAddr=_T("127.0.0.1");
	m_iServPort=80;
	//��ʼ����ʾ��ַ�Ͷ˿ں�
	GetDlgItem(IDC_EDIT_SERVADDR)->SetWindowText(m_strServAddr);
	CString tmpStr;
	tmpStr.Format(_T("%d"),m_iServPort);
	GetDlgItem(IDC_EDIT_SERVPORT)->SetWindowText(tmpStr);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

