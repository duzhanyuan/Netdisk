// MoveFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "MoveFileDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"


// CMoveFileDlg dialog

IMPLEMENT_DYNAMIC(CMoveFileDlg, CDialogEx)

CMoveFileDlg::CMoveFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoveFileDlg::IDD, pParent)
{
	m_strMovePath=_T("");
}

CMoveFileDlg::~CMoveFileDlg()
{
}

void CMoveFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MOVEFILE, m_tcMoveFile);
}


BEGIN_MESSAGE_MAP(CMoveFileDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MOVEOK, &CMoveFileDlg::OnBnClickedBtnMoveok)
	ON_BN_CLICKED(IDC_BTN_MOVECANCEL, &CMoveFileDlg::OnBnClickedBtnMovecancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MOVEFILE, &CMoveFileDlg::OnTvnSelchangedTreeMovefile)
END_MESSAGE_MAP()


// CMoveFileDlg message handlers


BOOL CMoveFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  Add extra initialization here
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();

	m_tcMoveFile.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_SINGLEEXPAND);
	m_MyDisk=m_tcMoveFile.InsertItem(_T("�ҵ�����"));

	//��ʾĿ¼
	if(m_pMainDlg->m_Client.UpdateClientCatalog())
	{
		CString m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowCatalog(m_strIndexInfo);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//����Ŀ¼��ʾ
void CMoveFileDlg::ShowCatalog(CString indexInfo)
{
	//if(indexInfo==_T(""))
	//	return;
	//���浱ǰ·����Ϣ����ֹ�ı�
	//CString tmpCurrentPath=m_strCurrentPath;

	//��ո�Ŀ¼�µ�Ŀ¼�ṹ
	DeleteSubRoot(m_MyDisk);

	CArray<CString,CString&> *oneIndexInfoArr=new CArray<CString,CString&>();
	//CArray<CString,CString&> *insertedIndexArr=new CArray<CString,CString&>();
	CArray<HTREEITEM,HTREEITEM&> *catalogRootArr=new CArray<HTREEITEM,HTREEITEM&>();
	//CArray<int,int&> *catalogNum=new CArray<int,int&>();

	CString oneIndexInfo;
	CString tmpStr ;
	int pos,totalNum;

	/*pos=indexInfo.Find('+');
	tmpStr=indexInfo.Left(pos);
	if(_ttoi(tmpStr)==EMPTYCATALOG)
	return;
	else
	tmpStr=indexInfo.Right(indexInfo.GetLength()-pos-1);
	catalogRootArr->Add(m_MyDisk);*/
	tmpStr=indexInfo;
	while(tmpStr.GetLength()>0)
	{
		oneIndexInfo=GetOneIndexInfo(tmpStr,pos);
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);

		if(oneIndexInfo[0]==_T('D'))
		{
			oneIndexInfoArr->Add(oneIndexInfo);

		}
	
	}

	totalNum=oneIndexInfoArr->GetCount();
	HTREEITEM root=m_tcMoveFile.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(0)),m_MyDisk);
	catalogRootArr->Add(root);
	bool flag;

	for(int i=1;i<totalNum;i++)
	{
		flag=false;
		CString parPath=getRootCatalogName(oneIndexInfoArr->GetAt(i));

		for(int j=0;j<i;j++)
		{
			pos=oneIndexInfoArr->GetAt(j).Find(parPath);
			if(-1!=pos&&oneIndexInfoArr->GetAt(j)[pos-1]==_T('|')&&oneIndexInfoArr->GetAt(j)[pos+parPath.GetLength()]==_T('|'))
			{
				root=m_tcMoveFile.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(i)),catalogRootArr->GetAt(j));
				catalogRootArr->Add(root);
				flag=true;
				break;
			}
		}
		if(!flag)
		{
			root=m_tcMoveFile.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(i)),m_MyDisk);
			catalogRootArr->Add(root);
			flag=true;
		}
	}
	m_tcMoveFile.Expand(m_MyDisk,TVE_EXPAND);
	//m_strCurrentPath=tmpCurrentPath;
	//SetSelectByFileListClick(m_strCurrentPath);
}

//�ݹ�ɾ����Ŀ¼
BOOL CMoveFileDlg::DeleteSubRoot(HTREEITEM hItem)
{
	if(!m_tcMoveFile.ItemHasChildren(hItem)) 
		return   FALSE;     
	HTREEITEM hChild = m_tcMoveFile.GetChildItem(hItem); 
	do   
	{   
		if(m_tcMoveFile.ItemHasChildren(hChild)) {   
			DeleteSubRoot(hChild);   
		}
		else { 
			HTREEITEM hChild1 = m_tcMoveFile.GetNextSiblingItem(hChild);
			m_tcMoveFile.DeleteItem(hChild);
			hChild = hChild1;
		}  
	}while(hChild != NULL);
	return TRUE;
}

CString CMoveFileDlg::GetOneIndexInfo(CString srcStr,int &iPos)
{
	CString tmpStr;
	if(srcStr==_T(""))
		return _T("");

	tmpStr=srcStr;
	int pos=tmpStr.Find(';');
	iPos=pos;

	return tmpStr.Left(pos);
}

//��ȡ�ļ�����
CString CMoveFileDlg::GetCatalogName(CString srcStr)
{
	CString tmpStr;
	if(srcStr==_T(""))
		return _T("");

	int pos=0;
	tmpStr=srcStr;
	for(int i=0;i<3;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}

	pos=tmpStr.Find('|');
	return tmpStr.Left(pos);
}

//��ȡ��Ŀ¼�ĸ�Ŀ¼����
CString CMoveFileDlg::getRootCatalogName(CString srcStr)
{
	if(srcStr==_T(""))
		return _T("");
	CString tmpName=GetCatalogName(srcStr);
	CString tmpPath,tmpStr=srcStr;
	int pos;
	//��ȡĿ¼·��
	for(int i=0;i<4;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}
	pos=tmpStr.Find('|');
	tmpPath=tmpStr.Left(pos);

	//��ȡ��Ŀ¼����
	pos=tmpPath.ReverseFind('\\');
	return tmpPath.Left(pos);

}


void CMoveFileDlg::OnBnClickedBtnMoveok()
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(m_pMainDlg->m_lcFileShow.GetCheck(i))
		{
			CString srcPath=m_pMainDlg->m_strCurrentPath+_T("\\")+m_pMainDlg->m_lcFileShow.GetItemText(i,0);
			CString desPath=m_strMovePath+_T("\\")+m_pMainDlg->m_lcFileShow.GetItemText(i,0);
			CString sendMsg=srcPath+_T("+")+desPath;
			//�����ƶ��ļ���Ŀ¼��Ϣ
			m_pMainDlg->m_Client.MoveClientFile(sendMsg);
		}
	}
	//����ǰ·������Ϊ�ƶ����Ŀ¼
	m_pMainDlg->m_strCurrentPath=m_strMovePath;
	//ˢ�¿ͻ���
	m_pMainDlg->OnBnClickedBtnRefresh();
	CDialogEx::OnOK();
}
	

void CMoveFileDlg::OnBnClickedBtnMovecancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CMoveFileDlg::OnTvnSelchangedTreeMovefile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM tHit=m_tcMoveFile.GetSelectedItem();
	m_strMovePath=m_pMainDlg->m_strUserLoginName;
	CArray<CString,CString&>* strPathArr=new CArray<CString,CString&>();

	//�ݹ����ѡ���ļ��е�·��
	while(tHit!=m_MyDisk)
	{
		strPathArr->Add(m_tcMoveFile.GetItemText(tHit));
		tHit=m_tcMoveFile.GetParentItem(tHit);
	}

	//��ȡѡ�е�·��
	for(int i=strPathArr->GetCount()-1;i>=0;i--)
	{
		m_strMovePath=m_strMovePath+_T("\\")+strPathArr->GetAt(i);
	}

	*pResult = 0;
}
