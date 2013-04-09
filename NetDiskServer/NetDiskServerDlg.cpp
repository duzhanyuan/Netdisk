

// NetDiskServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "NetDiskServerDlg.h"
#include "afxdialogex.h"
#include "SetServDlg.h"
#include "SetDeprtNameDlg.h"
#include "AdminLoginDlg.h"
#include "AddNewUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CNetDiskServerDlg dialog




CNetDiskServerDlg::CNetDiskServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetDiskServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//CNetDiskServerDlg::CNetDiskServerDlg(CString strCompyName)
//{
//	m_tcRoot=m_tcShowDeprt.InsertItem(strCompyName);
//}
void CNetDiskServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SHOWDEPARMENT, m_tcShowDeprt);
}

BEGIN_MESSAGE_MAP(CNetDiskServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBTN_SETSERV, &CNetDiskServerDlg::OnBnClickedMfcbtnSetserv)
	ON_BN_CLICKED(IDC_MFCBTN_QUIT, &CNetDiskServerDlg::OnBnClickedMfcbtnQuit)
	ON_BN_CLICKED(IDC_MFCBTN_ADDDEPARTMENT, &CNetDiskServerDlg::AddDepartment)
	ON_BN_CLICKED(IDC_MFCBTN_ADDUSER, &CNetDiskServerDlg::OnBtnAdduser)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SHOWDEPARMENT, &CNetDiskServerDlg::OnSelchangedTreeShowdeparment)
	ON_BN_CLICKED(IDC_MFCBTN_SERVCONTROL, &CNetDiskServerDlg::OnBnClickedMfcbtnServcontrol)
	ON_BN_CLICKED(IDC_MFCBTN_DELDEPRT, &CNetDiskServerDlg::OnBnClickedMfcbtnDeldeprt)
	ON_BN_CLICKED(IDC_MFCBTN_DELUSER, &CNetDiskServerDlg::OnBnClickedMfcbtnDeluser)

END_MESSAGE_MAP()


// CNetDiskServerDlg message handlers

BOOL CNetDiskServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_iServControl=0;
	//Ĭ�ϲ�����ť����ʹ��
	GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBTN_DELDEPRT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBTN_DELUSER)->EnableWindow(FALSE);
	//���ò�����ʾ��ͼ���
	m_tcShowDeprt.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	//m_tcRoot=m_tcShowDeprt.InsertItem(m_strCompyName);
	//�������ݿ�
	m_dbDepart.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
	if(!m_dbDepart.IsOpen())
	{
		AfxMessageBox(_T("���ݿ�����ʧ�ܣ�"));
		return FALSE;
	}

	//��ʾ������ͼ
	ShowDeprtDbInfo();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetDiskServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetDiskServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetDiskServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���÷�������IP��ַ�Ͷ˿ں�
void CNetDiskServerDlg::OnBnClickedMfcbtnSetserv()
{
	// TODO: Add your control notification handler code here
	CSetServDlg* pSetServerDlg=new CSetServDlg();
	if(pSetServerDlg->DoModal()==IDOK)
	{
		m_strServAddr=pSetServerDlg->m_strServAddr;
		m_iServPort=pSetServerDlg->m_iServPort;
	}

}

//�˳�����������
void CNetDiskServerDlg::OnBnClickedMfcbtnQuit()
{
	// TODO: Add your control notification handler code here
	//�ر����ݿ�
	m_dbDepart.Close();
	CDialog::OnCancel();
}

//�½������ļ���
void CNetDiskServerDlg::AddDepartment()
{

	// TODO: Add your control notification handler code here
	m_pAddNewDeprt=new CSetDeprtNameDlg();
	if(m_pAddNewDeprt->DoModal()==IDOK)
	{
		//m_tcShowDeprt.InsertItem(m_pAddNewDeprt->m_strDeprtName,NULL,NULL,m_tcRoot);
		//�����������ݿ���Ŀ
		try
		{
			CString strSql;
			strSql.Format(_T("insert netdisk_department_tb value ('%s',NULL)"),m_pAddNewDeprt->m_strDeprtName);
			m_dbDepart.ExecuteSQL(strSql);

			//��ʾ������ͼ
			ShowDeprtDbInfo();

		}
		catch(CDBException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
	}
}

//�����û�
void CNetDiskServerDlg::OnBtnAdduser()
{
	// TODO: Add your control notification handler code here
	//�����û��������û���Ϣ
	CAddNewUserDlg* pNewUser=new CAddNewUserDlg();
	if(IDOK==pNewUser->DoModal())
	{
		try
		{
			//ѡ�еĲ�������
			CString strSelDeprt=m_tcShowDeprt.GetItemText(m_hCurrent);
			CString strAddUser=pNewUser->m_strUserName;
			CString tmpSql;
			tmpSql.Format(_T("insert netdisk_department_tb value ('%s','%s')"),strSelDeprt,strAddUser);
			m_dbDepart.ExecuteSQL(tmpSql);

			//��ʾ������ͼ
			ShowDeprtDbInfo();
		}
		catch(CDBException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
	}
}

//������ͼ��ѡ�нڵ���¼�
void CNetDiskServerDlg::OnSelchangedTreeShowdeparment(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_hCurrent=m_tcShowDeprt.GetSelectedItem(); //��õ�ǰѡ�еĽڵ�
	CString strCurrent=m_tcShowDeprt.GetItemText(m_hCurrent);//���ѡ�еĽڵ���ַ���ֵ
	m_tcShowDeprt.SetItemState(m_hCurrent,TVIS_SELECTED,TVIS_SELECTED);//����ѡ�и���
	//ѡ�еĽڵ�Ϊ��˾����
	if(strCurrent==m_strCompyName)
	{
		GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(TRUE);
	}
	*pResult = 0;
}

//���������ư�ť����������ֹͣ������
void CNetDiskServerDlg::OnBnClickedMfcbtnServcontrol()
{
	// TODO: Add your control notification handler code here
	//����������
	if(m_iServControl==0)
	{
		GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_DELDEPRT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_DELUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_SERVCONTROL)->SetWindowText(_T("ֹͣ������"));
		m_iServControl=1;
		//tree control��Ĭ��ѡ�й�˾���ƣ���ʱ�޷�����û�
		m_tcShowDeprt.SelectItem(m_tcRoot);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);

		//����������
		if(!m_iocp.Start())
		{
			AfxMessageBox(_T("����������ʧ�ܣ�"));
			return;
		}
		return;
	}
	//ֹͣ������
	if(m_iServControl==1)
	{
		GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_DELDEPRT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_DELUSER)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBTN_SERVCONTROL)->SetWindowText(_T("����������"));
		m_iServControl=0;

		m_iocp.Stop();
		return;
	}

}


void CNetDiskServerDlg::OnBnClickedMfcbtnDeldeprt()
{
	// TODO: Add your control notification handler code here
}


void CNetDiskServerDlg::OnBnClickedMfcbtnDeluser()
{
	// TODO: Add your control notification handler code here
}

BOOL CNetDiskServerDlg::ShowDeprtDbInfo()
{
	//���tree control����ʾ�Ĳ�����Ϣ
	m_tcShowDeprt.DeleteAllItems();
	m_tcRoot=m_tcShowDeprt.InsertItem(m_strCompyName);

	try
	{
		//���������ݿ���Ϣ��ʾ��tree control�ؼ��ϣ���������ͼ
		CRecordset rs(&m_dbDepart);
		rs.Open(CRecordset::snapshot,_T("select * from netdisk_department_tb where user_name is null"));
		if(rs.IsBOF())
			return FALSE;
		CString tmpval;
		while(!rs.IsEOF())
		{
			
			rs.GetFieldValue((short)0,tmpval);
			m_tcShowDeprt.InsertItem(tmpval,NULL,NULL,m_tcRoot);
			UpdateData(FALSE);
			rs.MoveNext();
		}
		rs.Close();
		
		//��user��ʾ�ڲ�����ͼ��
		rs.Open(CRecordset::snapshot,_T("select * from netdisk_department_tb where user_name is not null"));
		if(rs.IsBOF())
			return FALSE;
		HTREEITEM hParentItem;
		while(!rs.IsEOF())
		{
			rs.GetFieldValue((short)0,tmpval);
			hParentItem=ReturnDepartment(tmpval);//��ȡ���׽ڵ�
			rs.GetFieldValue((short)1,tmpval);
			m_tcShowDeprt.InsertItem(tmpval,NULL,NULL,hParentItem);
			UpdateData(FALSE);
			rs.MoveNext();
		}
		rs.Close();
	}
	catch(CDBException* pe)
	{
		pe->ReportError();
		pe->Delete();
	}
	//����treeControl�ķ��Ϊչ��
	m_tcShowDeprt.Expand(m_tcRoot,TVE_EXPAND);	
	return TRUE;
}
HTREEITEM CNetDiskServerDlg::ReturnDepartment(CString DeprtName)
{
	if(!m_tcShowDeprt.ItemHasChildren(m_tcRoot))
		return NULL;
	HTREEITEM hChildItem=m_tcShowDeprt.GetChildItem(m_tcRoot);
	while (hChildItem!=NULL)
	{
		if(m_tcShowDeprt.GetItemText(hChildItem)==DeprtName)
			return hChildItem;
		hChildItem=m_tcShowDeprt.GetNextItem(hChildItem,TVGN_NEXT);
	}
	return NULL;
}

