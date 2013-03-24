// AdminLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "AdminLoginDlg.h"
#include "afxdialogex.h"

// CAdminLoginDlg dialog

IMPLEMENT_DYNAMIC(CAdminLoginDlg, CDialog)

CAdminLoginDlg::CAdminLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminLoginDlg::IDD, pParent)
{

	m_strAdminName = _T("admin");
	m_strAdminPasswd = _T("admin");
	m_strCompanyName = _T("");

}

CAdminLoginDlg::~CAdminLoginDlg()
{
}

void CAdminLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADMINNAME, m_strAdminName);
	DDX_Text(pDX, IDC_EDIT_ADMINPASSWD, m_strAdminPasswd);
	DDX_Text(pDX, IDC_EDIT_COMPANYNAME, m_strCompanyName);
	DDX_Check(pDX, IDC_CHECK_DEFAULTSET, m_bIsDefaultSet);

}


BEGIN_MESSAGE_MAP(CAdminLoginDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_DEFAULTSET, &CAdminLoginDlg::OnClickedCheckDefaultset)
	ON_BN_CLICKED(IDOK, &CAdminLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAdminLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAdminLoginDlg message handlers

//��������ΪĬ�ϡ��������򽫵�ǰ�Ĺ�˾������ΪĬ��
void CAdminLoginDlg::OnClickedCheckDefaultset()
{
	// TODO: Add your control notification handler code here
	if(BST_CHECKED==IsDlgButtonChecked(IDC_CHECK_DEFAULTSET))
	{
		GetDlgItem(IDC_EDIT_COMPANYNAME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_COMPANYNAME)->EnableWindow(TRUE);

	}
}


BOOL CAdminLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	try
	{
		//�������ݿ�
		m_dbAdmin.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
		if(!m_dbAdmin.IsOpen())
		{
			AfxMessageBox(_T("�����ݿ�ʧ�ܣ�"));
			return FALSE;
		}
		//��ȡ����Ա���ݿ�����
		CRecordset rs(&m_dbAdmin);
		rs.Open(CRecordset::snapshot,_T("select * from netdiskadmintb"));
		if(rs.IsBOF())
			return FALSE;
		//��������Ա���ݿ�
		while (!rs.IsEOF())
		{
			CString tmpval;
			rs.GetFieldValue((short)1,tmpval);
			m_strCompanyName=tmpval;
			rs.GetFieldValue((short)2,tmpval);
			m_strCmpAdminName=tmpval;
			rs.GetFieldValue((short)3,tmpval);
			m_strCmpAdminPasswd=tmpval;
			rs.GetFieldValue((short)4,tmpval);
			if(tmpval==_T("1"))
				m_bIsDefaultSet=TRUE;
			else
				m_bIsDefaultSet=FALSE;
			rs.MoveNext();
		}
		rs.Close();
	}
	catch (CDBException* pe)
	{
		pe->ReportError();
		pe->Delete();
	}
	//���ó�ʼ����˾�����Ƿ�Ĭ��
	if(m_bIsDefaultSet)
	{
		((CButton*)GetDlgItem(IDC_CHECK_DEFAULTSET))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(m_strCompanyName);
		GetDlgItem(IDC_EDIT_COMPANYNAME)->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_DEFAULTSET))->SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(m_strCompanyName);
		GetDlgItem(IDC_EDIT_COMPANYNAME)->EnableWindow(TRUE);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAdminLoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//�����û�������ȷ
	if(m_strAdminName!=m_strCmpAdminName)
	{
		AfxMessageBox(_T("�û����������������룡"));
		return;
	}
	//�����������
	if(m_strAdminPasswd!=m_strCmpAdminPasswd)
	{
		AfxMessageBox(_T("����������������룡"));
		return;
	}
	//����˾�����뵽�������ݿ���
	//try
	//{
	//	
	//	CString strSql;
	//	strSql.Format(_T("insert netdisk_department_tb value ('%s','%s','%s')"),m_strCompanyName,_T(""),_T(""));		
	//	m_dbAdmin.ExecuteSQL(strSql);
	//}
	//catch (CDBException* pe)
	//{
	//	pe->ReportError();
	//	pe->Delete();
	//}

	//�ر����ݿ�
	m_dbAdmin.Close();
	CDialog::OnOK();
}


void CAdminLoginDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	
	//�ر����ݿ�
	m_dbAdmin.Close();
	CDialog::OnCancel();
}
