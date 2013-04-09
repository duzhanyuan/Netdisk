// AddNewUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "AddNewUserDlg.h"
#include "afxdialogex.h"
#include "NetDiskServerDlg.h"

// CAddNewUserDlg dialog

IMPLEMENT_DYNAMIC(CAddNewUserDlg, CDialog)

CAddNewUserDlg::CAddNewUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddNewUserDlg::IDD, pParent)
{

	m_strUserName = _T("");
	m_strUserServAddr = _T("127.0.0.1");
	m_iUserServPort = 3306;
	m_strUserFileSavePath = _T("");
	m_strUserPasswd = _T("");
	m_strComfPwd = _T("");
}

CAddNewUserDlg::~CAddNewUserDlg()
{
}

void CAddNewUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_USERSERVADDRSET, m_strUserServAddr);
	DDX_Text(pDX, IDC_EDIT_USERSERVPORTSET, m_iUserServPort);
	DDX_Text(pDX, IDC_EDITUSERFILESAVEPATH, m_strUserFileSavePath);
	DDX_Text(pDX, IDC_EDITUSERPASSWD, m_strUserPasswd);
	DDX_Text(pDX, IDC_CONFORMPWD, m_strComfPwd);
}


BEGIN_MESSAGE_MAP(CAddNewUserDlg, CDialog)
	ON_BN_CLICKED(IDC_MFCBTN_SHOWPATH, &CAddNewUserDlg::OnBtnShowpath)
	ON_BN_CLICKED(IDOK, &CAddNewUserDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddNewUserDlg message handlers

//ѡ���û����ļ����λ��
void CAddNewUserDlg::OnBtnShowpath()
{
	// TODO: Add your control notification handler code here
	TCHAR   szPath[MAX_PATH]={0};
	LPITEMIDLIST   pitem;
	BROWSEINFO   info;
	::ZeroMemory(&info,sizeof(info));
	info.hwndOwner=this->m_hWnd;  
	info.lpszTitle=_T("ѡ����λ��");
	info.ulFlags=BIF_NEWDIALOGSTYLE ;//��ӡ��½��ļ��С��Ի���
	//info.pszDisplayName = szPath;
	//��ʾ�ļ������
	if(pitem=::SHBrowseForFolder(&info))
	{
		::SHGetPathFromIDList(pitem,szPath);
		GetDlgItem(IDC_EDITUSERFILESAVEPATH)->SetWindowText(szPath);
		//m_strUserFileSavePath=szPath;
		//UpdateData(FALSE);

	} 
	
}


void CAddNewUserDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(!m_strUserName||!m_strUserPasswd||!m_strComfPwd||!m_strUserFileSavePath||!m_strUserServAddr||!m_iUserServPort)
	{
		AfxMessageBox(_T("������Ϣ�����������������룡"));
		return;
	}
	//���Ӳ������ݿ⣬�ж��û����Ƿ���ڣ�ʹ�ò�������+�û�����֤
	CDatabase db;
	CString tmpSql,numstr;
	try
	{
		db.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
		if(!db.IsOpen())
		{
			AfxMessageBox(_T("���ݿ��ʧ�ܣ�"));
			return;
		}
		
		CNetDiskServerDlg* pMain=((CNetDiskServerDlg*)AfxGetMainWnd());
		CString strCurDeprt=pMain->m_tcShowDeprt.GetItemText(pMain->m_hCurrent);
		tmpSql.Format(_T("select count(*) as num from netdisk_department_tb where deprt_name='%s' and user_name='%s'"),strCurDeprt,m_strUserName);
		CRecordset rs(&db);
		rs.Open(CRecordset::snapshot,tmpSql);
		if(rs.IsBOF())
			return;
		rs.GetFieldValue((short)0,numstr);
		if(numstr!=_T("0"))
		{
			AfxMessageBox(_T("�û����Ѵ��ڣ����������룡"));
			return;
		}
		rs.Close();

	}
	catch(CDBException* pe)
	{
		pe->ReportError();
		pe->Delete();
	}
	//db.Close();
	//�ж����������Ƿ�����
	if(m_strComfPwd!=m_strUserPasswd)
	{
		AfxMessageBox(_T("�����������벻һ�£����������룡"));    
		return;
	}
	try
	{
		
		tmpSql.Format(_T("insert netdisk_user_tb value ('%s','%d','%s','%s','%s')"),m_strUserServAddr,m_iUserServPort,m_strUserName,m_strUserPasswd,m_strUserFileSavePath);
		db.ExecuteSQL(tmpSql);
	}
	catch(CDBException* pe)
	{
		pe->ReportError();
		pe->Delete();
	}
	db.Close();
	CDialog::OnOK();
}


BOOL CAddNewUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
