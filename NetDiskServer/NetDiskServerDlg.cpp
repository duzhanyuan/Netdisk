

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
	//默认操作按钮不可使用
	GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBTN_DELDEPRT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MFCBTN_DELUSER)->EnableWindow(FALSE);
	//设置部门显示视图风格
	m_tcShowDeprt.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	//m_tcRoot=m_tcShowDeprt.InsertItem(m_strCompyName);
	//连接数据库
	m_dbDepart.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
	if(!m_dbDepart.IsOpen())
	{
		AfxMessageBox(_T("数据库连接失败！"));
		return FALSE;
	}

	//显示部门视图
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


//设置服务器的IP地址和端口号
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

//退出服务器设置
void CNetDiskServerDlg::OnBnClickedMfcbtnQuit()
{
	// TODO: Add your control notification handler code here
	//关闭数据库
	m_dbDepart.Close();
	CDialog::OnCancel();
}

//新建部门文件夹
void CNetDiskServerDlg::AddDepartment()
{

	// TODO: Add your control notification handler code here
	m_pAddNewDeprt=new CSetDeprtNameDlg();
	if(m_pAddNewDeprt->DoModal()==IDOK)
	{
		//m_tcShowDeprt.InsertItem(m_pAddNewDeprt->m_strDeprtName,NULL,NULL,m_tcRoot);
		//创建部门数据库条目
		try
		{
			CString strSql;
			strSql.Format(_T("insert netdisk_department_tb value ('%s',NULL)"),m_pAddNewDeprt->m_strDeprtName);
			m_dbDepart.ExecuteSQL(strSql);

			//显示部门视图
			ShowDeprtDbInfo();

		}
		catch(CDBException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
	}
}

//增加用户
void CNetDiskServerDlg::OnBtnAdduser()
{
	// TODO: Add your control notification handler code here
	//新增用户，设置用户信息
	CAddNewUserDlg* pNewUser=new CAddNewUserDlg();
	if(IDOK==pNewUser->DoModal())
	{
		try
		{
			//选中的部门名称
			CString strSelDeprt=m_tcShowDeprt.GetItemText(m_hCurrent);
			CString strAddUser=pNewUser->m_strUserName;
			CString tmpSql;
			tmpSql.Format(_T("insert netdisk_department_tb value ('%s','%s')"),strSelDeprt,strAddUser);
			m_dbDepart.ExecuteSQL(tmpSql);

			//显示部门视图
			ShowDeprtDbInfo();
		}
		catch(CDBException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
	}
}

//部门视图中选中节点的事件
void CNetDiskServerDlg::OnSelchangedTreeShowdeparment(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_hCurrent=m_tcShowDeprt.GetSelectedItem(); //获得当前选中的节点
	CString strCurrent=m_tcShowDeprt.GetItemText(m_hCurrent);//获得选中的节点的字符串值
	m_tcShowDeprt.SetItemState(m_hCurrent,TVIS_SELECTED,TVIS_SELECTED);//设置选中高亮
	//选中的节点为公司名称
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

//服务器控制按钮，启动或者停止服务器
void CNetDiskServerDlg::OnBnClickedMfcbtnServcontrol()
{
	// TODO: Add your control notification handler code here
	//启动服务器
	if(m_iServControl==0)
	{
		GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_DELDEPRT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_DELUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBTN_SERVCONTROL)->SetWindowText(_T("停止服务器"));
		m_iServControl=1;
		//tree control中默认选中公司名称，此时无法添加用户
		m_tcShowDeprt.SelectItem(m_tcRoot);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);

		//启动服务器
		if(!m_iocp.Start())
		{
			AfxMessageBox(_T("启动服务器失败！"));
			return;
		}
		return;
	}
	//停止服务器
	if(m_iServControl==1)
	{
		GetDlgItem(IDC_MFCBTN_ADDDEPARTMENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_ADDUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_DELDEPRT)->EnableWindow(FALSE);
		GetDlgItem(IDC_MFCBTN_DELUSER)->EnableWindow(FALSE);

		GetDlgItem(IDC_MFCBTN_SERVCONTROL)->SetWindowText(_T("启动服务器"));
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
	//清空tree control上显示的部门信息
	m_tcShowDeprt.DeleteAllItems();
	m_tcRoot=m_tcShowDeprt.InsertItem(m_strCompyName);

	try
	{
		//将部门数据库信息显示在tree control控件上，即部门视图
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
		
		//将user显示在部门视图中
		rs.Open(CRecordset::snapshot,_T("select * from netdisk_department_tb where user_name is not null"));
		if(rs.IsBOF())
			return FALSE;
		HTREEITEM hParentItem;
		while(!rs.IsEOF())
		{
			rs.GetFieldValue((short)0,tmpval);
			hParentItem=ReturnDepartment(tmpval);//获取父亲节点
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
	//设置treeControl的风格为展开
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

