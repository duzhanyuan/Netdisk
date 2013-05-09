// RecycleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "RecycleDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

// CRecycleDlg dialog

IMPLEMENT_DYNAMIC(CRecycleDlg, CDialogEx)

CRecycleDlg::CRecycleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecycleDlg::IDD, pParent)
{

	m_strSearchFile = _T("");
}

CRecycleDlg::~CRecycleDlg()
{
}

void CRecycleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearchFile);
	DDX_Control(pDX, IDC_LIST_RECYCLELIST, m_lcRecycleList);
	DDX_Control(pDX, IDC_MFCBTN_RESTORE, m_btnRestore);
}


BEGIN_MESSAGE_MAP(CRecycleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBTN_RESTORE, &CRecycleDlg::OnBnClickedMfcbtnRestore)
	ON_BN_CLICKED(IDC_MFCBTN_DELETE, &CRecycleDlg::OnBnClickedMfcbtnDelete)
	ON_BN_CLICKED(IDC_MFCBTN_CLEANUP, &CRecycleDlg::OnBnClickedMfcbtnCleanup)
	ON_BN_CLICKED(IDC_MFCBTN_REFRESH, &CRecycleDlg::OnBnClickedMfcbtnRefresh)
	ON_BN_CLICKED(IDC_MFCBTN_SEARCH, &CRecycleDlg::OnBnClickedMfcbtnSearch)
END_MESSAGE_MAP()


// CRecycleDlg message handlers


BOOL CRecycleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	//获取主窗口句柄
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();

	//初始化当前路径
	m_strCurrentPath = m_pMainDlg->m_strUserLoginName;

	//添加文件夹图标识别
	HICON hIcon;
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,1);
	m_pMainDlg->m_cSysIcon.m_ImageSmallList.Add(hIcon);
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,0);
	m_pMainDlg->m_cSysIcon.m_ImageLargeList.Add(hIcon);
	m_pMainDlg->m_cSysIcon.m_arExtName.Add(_T(""));



	m_lcRecycleList.InsertColumn(0,_T("文件名"),LVCFMT_CENTER,300);
	m_lcRecycleList.InsertColumn(1,_T("大小"),LVCFMT_CENTER,150);
	m_lcRecycleList.InsertColumn(2,_T("更新时间"),LVCFMT_CENTER,200);
	//初始化列表显示
	InitRecycleList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRecycleDlg::InitRecycleList()
{
	m_lcRecycleList.ModifyStyle(0,LVS_SHOWSELALWAYS|LVS_REPORT|LVS_SINGLESEL|LVSIL_NORMAL);
	m_lcRecycleList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);
	
	//更新列表显示
	if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
	}

	m_lcRecycleList.SetImageList(&m_pMainDlg->m_cSysIcon.m_ImageSmallList,LVSIL_SMALL);

}

//还原文件操作
void CRecycleDlg::OnBnClickedMfcbtnRestore()
{
	// TODO: Add your control notification handler code here
	int count=0;
	for(int i=0;i<m_lcRecycleList.GetItemCount();i++)
	{
		if(m_lcRecycleList.GetCheck(i))
			count++;
	}
	if(count == 0)
	{
		AfxMessageBox(_T("请选择要还原的文件或者目录"));
		return;
	}
	for(int i=0;i<count;i++)
	{
		if(m_lcRecycleList.GetCheck(i))
		{
			m_pMainDlg->m_Client.RecycleRestore(m_pMainDlg->m_strUserLoginName+_T("\\")+m_lcRecycleList.GetItemText(i,0));
		}
	}

	//更新列表显示
	if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
	}

	//更新主窗口的列表显示
	m_pMainDlg->OnBnClickedBtnRefresh();
}

//删除回收站记录
void CRecycleDlg::OnBnClickedMfcbtnDelete()
{
	// TODO: Add your control notification handler code here
	int count=0;
	for(int i=0;i<m_lcRecycleList.GetItemCount();i++)
	{
		if(m_lcRecycleList.GetCheck(i))
			count++;
	}
	if(count == 0)
	{
		AfxMessageBox(_T("请选择要还原的文件或者目录"));
		return;
	}

	//警告用户是否继续删除操作
	int nRes = AfxMessageBox(_T("确定删除所选文件？"),MB_YESNO|MB_ICONQUESTION);
	if(nRes == IDYES)
	{
		for(int i=0;i<m_lcRecycleList.GetItemCount();i++)
		{
			if(m_lcRecycleList.GetCheck(i))
			{
				m_pMainDlg->m_Client.RecycleDel(m_pMainDlg->m_strUserLoginName+_T("\\")+m_lcRecycleList.GetItemText(i,0));
			}
		}

		//更新列表显示
		if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
		}

	}

}

//清空回收站
void CRecycleDlg::OnBnClickedMfcbtnCleanup()
{
	// TODO: Add your control notification handler code here
	int count=0;
	for(int i=0;i<m_lcRecycleList.GetItemCount();i++)
	{
		count++;
	}
	if(count == 0)
	{
		return;
	}
	//警告用户是否继续清空操作
	int nRes = AfxMessageBox(_T("确定删除所有文件？"),MB_YESNO|MB_ICONQUESTION);
	if(nRes == IDYES)
	{
		m_pMainDlg->m_Client.RecycleClean(m_strCurrentPath);

		//更新列表显示
		if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
		}
	}
	
}

//刷新文件列表
void CRecycleDlg::OnBnClickedMfcbtnRefresh()
{
	// TODO: Add your control notification handler code here
	//更新列表显示
	if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
	}
}

//搜索回收站文件
void CRecycleDlg::OnBnClickedMfcbtnSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pMainDlg->m_Client.RecycleFindFile(m_strSearchFile))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pSearchFile=new CSearchFile();
		m_pSearchFile->ShowFindFileList(&m_lcRecycleList,m_strIndexInfo);
	}
}
