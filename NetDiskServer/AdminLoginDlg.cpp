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

//单击“设为默认”，若是则将当前的公司名称设为默认
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
		//连接数据库
		m_dbAdmin.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
		if(!m_dbAdmin.IsOpen())
		{
			AfxMessageBox(_T("打开数据库失败！"));
			return FALSE;
		}
		//获取管理员数据库数据
		CRecordset rs(&m_dbAdmin);
		rs.Open(CRecordset::snapshot,_T("select * from netdiskadmintb"));
		if(rs.IsBOF())
			return FALSE;
		//遍历管理员数据库
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
	//设置初始化公司名称是否默认
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
	//输入用户名不正确
	if(m_strAdminName!=m_strCmpAdminName)
	{
		AfxMessageBox(_T("用户名错误，请重新输入！"));
		return;
	}
	//输入密码错误
	if(m_strAdminPasswd!=m_strCmpAdminPasswd)
	{
		AfxMessageBox(_T("密码错误，请重新输入！"));
		return;
	}
	//将公司名插入到部门数据库中
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

	//关闭数据库
	m_dbAdmin.Close();
	CDialog::OnOK();
}


void CAdminLoginDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	
	//关闭数据库
	m_dbAdmin.Close();
	CDialog::OnCancel();
}
