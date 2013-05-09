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
	m_strShareFloder=m_pMainDlg->m_strShareFloder;

	m_tcMoveFile.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_SINGLEEXPAND);
	m_MyDisk=m_tcMoveFile.InsertItem(_T("我的网盘"));
	m_ShareDisk=m_tcMoveFile.InsertItem(_T("共享文件夹"));

	//显示目录
	if(m_pMainDlg->m_Client.UpdateClientCatalog(m_pMainDlg->m_strUserLoginName))
	{
		CString m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowCatalog(m_MyDisk,m_strIndexInfo);

	}
	if(m_pMainDlg->m_Client.UpdateClientCatalog(m_pMainDlg->m_strShareFloder))
	{
		CString m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowCatalog(m_ShareDisk,m_strIndexInfo);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//更新目录显示
void CMoveFileDlg::ShowCatalog(HTREEITEM rootNode,CString indexInfo)
{
	//if(indexInfo==_T(""))
	//	return;
	//保存当前路径信息，防止改变
	//CString tmpCurrentPath=m_strCurrentPath;

	//清空根目录下的目录结构
	DeleteSubRoot(rootNode);

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
	//没有子文件夹
	if(totalNum == 0)
		return;
	HTREEITEM root=m_tcMoveFile.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(0)),rootNode);
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
			root=m_tcMoveFile.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(i)),rootNode);
			catalogRootArr->Add(root);
			flag=true;
		}
	}
	m_tcMoveFile.Expand(rootNode,TVE_EXPAND);
	m_tcMoveFile.SetImageList(&m_pMainDlg->m_cSysIcon.m_ImageSmallList,TVSIL_NORMAL); 

	//m_strCurrentPath=tmpCurrentPath;
	//SetSelectByFileListClick(m_strCurrentPath);
}

//递归删除根目录
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

//获取文件夹名
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

//获取子目录的根目录名称
CString CMoveFileDlg::getRootCatalogName(CString srcStr)
{
	if(srcStr==_T(""))
		return _T("");
	CString tmpName=GetCatalogName(srcStr);
	CString tmpPath,tmpStr=srcStr;
	int pos;
	//获取目录路径
	for(int i=0;i<4;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}
	pos=tmpStr.Find('|');
	tmpPath=tmpStr.Left(pos);

	//获取根目录名称
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

			if(m_pMainDlg->m_strCurrentPath == m_strMovePath || 0 == m_strMovePath.Find(srcPath))
			{
				AfxMessageBox(_T("当前选中的路径无效，请重新选择！"));
				return;
			}
			CString sendMsg=srcPath+_T("+")+desPath;
			//发送移动文件或目录信息
			m_pMainDlg->m_Client.MoveClientFile(sendMsg);
		}
	}
	//将当前路径设置为移动后的目录
	m_pMainDlg->m_strCurrentPath=m_strMovePath;
	//刷新客户端
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
	CArray<CString,CString&>* strPathArr=new CArray<CString,CString&>();

	//递归查找选中文件夹的路径
	while(tHit!=m_MyDisk && tHit !=m_ShareDisk)
	{
		strPathArr->Add(m_tcMoveFile.GetItemText(tHit));
		tHit=m_tcMoveFile.GetParentItem(tHit);
	}
	if(tHit == m_MyDisk)
		m_strMovePath=m_pMainDlg->m_strUserLoginName;
	if(tHit == m_ShareDisk)
		m_strMovePath=m_pMainDlg->m_strShareFloder;

	//获取选中的路径
	for(int i=strPathArr->GetCount()-1;i>=0;i--)
	{
		m_strMovePath=m_strMovePath+_T("\\")+strPathArr->GetAt(i);
	}

	*pResult = 0;
}
