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

	//��ȡ�����ھ��
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();

	//��ʼ����ǰ·��
	m_strCurrentPath = m_pMainDlg->m_strUserLoginName;

	//����ļ���ͼ��ʶ��
	HICON hIcon;
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,1);
	m_pMainDlg->m_cSysIcon.m_ImageSmallList.Add(hIcon);
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,0);
	m_pMainDlg->m_cSysIcon.m_ImageLargeList.Add(hIcon);
	m_pMainDlg->m_cSysIcon.m_arExtName.Add(_T(""));



	m_lcRecycleList.InsertColumn(0,_T("�ļ���"),LVCFMT_CENTER,300);
	m_lcRecycleList.InsertColumn(1,_T("��С"),LVCFMT_CENTER,150);
	m_lcRecycleList.InsertColumn(2,_T("����ʱ��"),LVCFMT_CENTER,200);
	//��ʼ���б���ʾ
	InitRecycleList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRecycleDlg::InitRecycleList()
{
	m_lcRecycleList.ModifyStyle(0,LVS_SHOWSELALWAYS|LVS_REPORT|LVS_SINGLESEL|LVSIL_NORMAL);
	m_lcRecycleList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);
	
	//�����б���ʾ
	if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
	}

	m_lcRecycleList.SetImageList(&m_pMainDlg->m_cSysIcon.m_ImageSmallList,LVSIL_SMALL);

}

//��ԭ�ļ�����
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
		AfxMessageBox(_T("��ѡ��Ҫ��ԭ���ļ�����Ŀ¼"));
		return;
	}
	for(int i=0;i<count;i++)
	{
		if(m_lcRecycleList.GetCheck(i))
		{
			m_pMainDlg->m_Client.RecycleRestore(m_pMainDlg->m_strUserLoginName+_T("\\")+m_lcRecycleList.GetItemText(i,0));
		}
	}

	//�����б���ʾ
	if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
	}

	//���������ڵ��б���ʾ
	m_pMainDlg->OnBnClickedBtnRefresh();
}

//ɾ������վ��¼
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
		AfxMessageBox(_T("��ѡ��Ҫ��ԭ���ļ�����Ŀ¼"));
		return;
	}

	//�����û��Ƿ����ɾ������
	int nRes = AfxMessageBox(_T("ȷ��ɾ����ѡ�ļ���"),MB_YESNO|MB_ICONQUESTION);
	if(nRes == IDYES)
	{
		for(int i=0;i<m_lcRecycleList.GetItemCount();i++)
		{
			if(m_lcRecycleList.GetCheck(i))
			{
				m_pMainDlg->m_Client.RecycleDel(m_pMainDlg->m_strUserLoginName+_T("\\")+m_lcRecycleList.GetItemText(i,0));
			}
		}

		//�����б���ʾ
		if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
		}

	}

}

//��ջ���վ
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
	//�����û��Ƿ������ղ���
	int nRes = AfxMessageBox(_T("ȷ��ɾ�������ļ���"),MB_YESNO|MB_ICONQUESTION);
	if(nRes == IDYES)
	{
		m_pMainDlg->m_Client.RecycleClean(m_strCurrentPath);

		//�����б���ʾ
		if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
		}
	}
	
}

//ˢ���ļ��б�
void CRecycleDlg::OnBnClickedMfcbtnRefresh()
{
	// TODO: Add your control notification handler code here
	//�����б���ʾ
	if(m_pMainDlg->m_Client.UpdateRecycleList(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pMainDlg->ShowFileList(&m_lcRecycleList,m_strIndexInfo);
	}
}

//��������վ�ļ�
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
