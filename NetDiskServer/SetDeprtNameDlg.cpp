// SetDeprtNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "SetDeprtNameDlg.h"
#include "afxdialogex.h"
#include "NetDiskServerDlg.h"

// CSetDeprtNameDlg dialog

IMPLEMENT_DYNAMIC(CSetDeprtNameDlg, CDialog)

CSetDeprtNameDlg::CSetDeprtNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDeprtNameDlg::IDD, pParent)
{

	m_strDeprtName = _T("");
}

CSetDeprtNameDlg::~CSetDeprtNameDlg()
{
}

void CSetDeprtNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEPARTMENTNAME, m_strDeprtName);
}


BEGIN_MESSAGE_MAP(CSetDeprtNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetDeprtNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDeprtNameDlg message handlers


void CSetDeprtNameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strDeprtName!=_T(""))
	{
		CDatabase db;
		//判断数据库中是否存在相同的部门名称
		try
		{
			db.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
			if(!db.IsOpen())
			{
				AfxMessageBox(_T("数据库打开失败！"));
				return;
			}
			CRecordset rs(&db);
			CString tmpSql,tmpnum;
			tmpSql.Format(_T("select count(*) as num from netdisk_department_tb where deprt_name='%s'"),m_strDeprtName);
			rs.Open(CRecordset::snapshot,tmpSql);
			if(rs.IsBOF())
				return;
			rs.GetFieldValue((short)0,tmpnum);
			if(tmpnum!=_T("0"))
			{
				AfxMessageBox(_T("名称已存在，请重新输入！"));
				return;
			}
			rs.Close();
		}
		catch(CDBException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
		db.Close();
		CDialog::OnOK();
	}
	else
	{
		MessageBox(_T("输入名称为空，请重新输入！"),_T("Warning"));
	}

}
