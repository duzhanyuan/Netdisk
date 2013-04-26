// CatalogShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "CatalogShowDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

// CCatalogShowDlg dialog

IMPLEMENT_DYNAMIC(CCatalogShowDlg, CDialog)

CCatalogShowDlg::CCatalogShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCatalogShowDlg::IDD, pParent)
{
}

CCatalogShowDlg::~CCatalogShowDlg()
{
}

void CCatalogShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CATALOGSHOW, m_CatalogShow);
}


BEGIN_MESSAGE_MAP(CCatalogShowDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CATALOGSHOW, &CCatalogShowDlg::OnTvnSelchangedTreeCatalogshow)
END_MESSAGE_MAP()


// CCatalogShowDlg message handlers


BOOL CCatalogShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetClientRect(&m_rect);
	// TODO:  Add extra initialization here
	m_CatalogShow.ModifyStyle(0,TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_SINGLEEXPAND);
	m_MyDisk=m_CatalogShow.InsertItem(_T("我的网盘"));
	m_ShareDisk=m_CatalogShow.InsertItem(_T("共享文件夹"));
	m_iMyDiskCount=1;

	m_MainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();
	//初始化当前默认路径
	m_strUserLoginName=((CNetdiskClientApp*)AfxGetApp())->m_loginName;
	m_Client=m_MainDlg->m_Client;


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCatalogShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(GetDlgItem(IDC_TREE_CATALOGSHOW))
	{
		CRect rect;
		GetDlgItem(IDC_TREE_CATALOGSHOW)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		rect.left=rect.left*cx/m_rect.Width();
		rect.right=rect.right*cx/m_rect.Width();
		rect.bottom=rect.bottom*cy/m_rect.Height();
		rect.top=rect.top*cy/m_rect.Height();
		GetDlgItem(IDC_TREE_CATALOGSHOW)->MoveWindow(&rect);
	}
	GetClientRect(&m_rect);
	// TODO: Add your message handler code here
}

//更新目录显示
void CCatalogShowDlg::UpdateCatalogShow(CString indexInfo)
{
	//if(indexInfo==_T(""))
	//	return;
	//保存当前路径信息，防止改变
	CString tmpCurrentPath=m_strCurrentPath;

	//清空根目录下的目录结构
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
	HTREEITEM root=m_CatalogShow.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(0)),m_MyDisk);
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
				root=m_CatalogShow.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(i)),catalogRootArr->GetAt(j));
				m_iMyDiskCount++;
				catalogRootArr->Add(root);
				flag=true;
				break;
			}
		}
		if(!flag)
		{
			root=m_CatalogShow.InsertItem(GetCatalogName(oneIndexInfoArr->GetAt(i)),m_MyDisk);
			m_iMyDiskCount++;			
			catalogRootArr->Add(root);
			flag=true;
		}
	}
	m_CatalogShow.Expand(m_MyDisk,TVE_EXPAND);
	m_CatalogShow.SetImageList(&m_MainDlg->m_cSysIcon.m_ImageSmallList,TVSIL_NORMAL); 
	m_strCurrentPath=tmpCurrentPath;
	SetSelectByFileListClick(m_strCurrentPath);
}
//递归删除根目录下的目录结构
BOOL CCatalogShowDlg::DeleteSubRoot(HTREEITEM hItem)
{
	if(!m_CatalogShow.ItemHasChildren(hItem)) 
		return   FALSE;     
	HTREEITEM hChild = m_CatalogShow.GetChildItem(hItem); 
	do   
	{   
		if(m_CatalogShow.ItemHasChildren(hChild)) {   
			DeleteSubRoot(hChild);   
		}
		else { 
			HTREEITEM hChild1 = m_CatalogShow.GetNextSiblingItem(hChild);
			m_CatalogShow.DeleteItem(hChild);
			hChild = hChild1;
		}  
	}while(hChild != NULL);
	return TRUE;
}
//获取子目录的根目录名称
CString CCatalogShowDlg::getRootCatalogName(CString srcStr)
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
//读取索引记录并保存在链表中
//bool CCatalogShowDlg::readCatalogIndex(CCatalogIndex* catalogHead,CString srcStr)
//{
//	CString tmpStr=srcStr;
//	int pos;
//	CString oneIndexInfo=GetOneIndexInfo(tmpStr,pos);
//	tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
//
//	while(oneIndexInfo[0]!=_T('D'))
//	{
//		oneIndexInfo=GetOneIndexInfo(tmpStr,pos);
//		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
//	}
//
//	CString catalogName=GetCatalogName(oneIndexInfo);
//	int subCatNum=NumOfSubCatalog(oneIndexInfo);
//
//	CCatalogIndex* newCatalogIndex=new CCatalogIndex();
//	newCatalogIndex->m_strName=catalogName;
//	newCatalogIndex->m_subCatalog=catalogHead->m_subCatalog;
//	catalogHead->m_subCatalog=newCatalogIndex;
//
//	while(subCatNum)
//	{
//		readCatalogIndex(catalogHead->m_subCatalog,tmpStr);
//		subCatNum--;
//	}
//	return true;
//}

////递归插入子文件夹节点
//HTREEITEM CCatalogShowDlg::RecurInsertCatalogInfo(HTREEITEM parentRoot,CString catalogName)
//{
//	if(parentRoot == NULL)
//		return NULL;
//
//	while(parentRoot!=NULL)
//	{
//		m_CatalogShow.InsertItem(catalogName,parentRoot);
//		
//	}
//}

//获取一条完整的文件或文件夹索引记录
CString CCatalogShowDlg::GetOneIndexInfo(CString srcStr,int &iPos)
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
CString CCatalogShowDlg::GetCatalogName(CString srcStr)
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

//获取子文件夹的数目
int CCatalogShowDlg::NumOfSubCatalog(CString srcStr)
{
	CString tmpStr,numStr=0;
	if(srcStr==_T(""))
		return 0;

	int pos=srcStr.Find('|');
	tmpStr=srcStr.Right(srcStr.GetLength()-pos-1);

	pos=tmpStr.Find('|');
	numStr=tmpStr.Left(pos);

	return _ttoi(numStr);
}

//获取子文件数目
int CCatalogShowDlg::NumOfSubFile(CString srcStr)
{
	CString tmpStr,numStr=0;
	if(srcStr==_T(""))
		return 0;

	int pos=srcStr.Find('|');
	tmpStr=srcStr.Right(srcStr.GetLength()-pos-1);
	pos=tmpStr.Find('|');
	tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);

	pos=tmpStr.Find('|');
	numStr=tmpStr.Left(pos);

	return _ttoi(numStr);
}

//处理目录点击事件
void CCatalogShowDlg::OnTvnSelchangedTreeCatalogshow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM tHit=m_CatalogShow.GetSelectedItem();

	CArray<CString,CString&>* nodeNameArr=new CArray<CString,CString&>();
	m_strCurrentPath=m_strUserLoginName;
	if(tHit!=m_MyDisk)
	{
		HTREEITEM tmpItem=tHit;
		m_CatalogShow.Select(tmpItem, TVGN_CARET);
		m_CatalogShow.Select(tmpItem, TVGN_DROPHILITE);

		while(tmpItem!=m_MyDisk)
		{
			nodeNameArr->Add(m_CatalogShow.GetItemText(tmpItem));
			tmpItem=m_CatalogShow.GetParentItem(tmpItem);
		}

		for(int i=nodeNameArr->GetCount()-1;i>=0;i--)
		{
			m_strCurrentPath=m_strCurrentPath+_T("\\")+nodeNameArr->GetAt(i);
		}

		//将本次操作路径添加到历史路径数组中
		HistroyPath* tmpHistroyPath=new HistroyPath();
		tmpHistroyPath->path=m_strCurrentPath;
		tmpHistroyPath->next=NULL;
		m_MainDlg->m_pHisPathTail->next=tmpHistroyPath;
		m_MainDlg->m_pHisPathTail=tmpHistroyPath;
	}
	else
	{
		m_CatalogShow.Select(m_MyDisk, TVGN_CARET);
		m_CatalogShow.Select(m_MyDisk, TVGN_DROPHILITE);
	}
	if(m_Client.GetCatalogInfo(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_MainDlg->ShowFileList(m_strIndexInfo);
	}
	
	//更新到主窗口的路径显示中去
	m_MainDlg->m_strCurrentPath=m_strCurrentPath;
	m_MainDlg->UpdateData(FALSE);



	//GetDlgItem(IDC_EDIT_CURRENTPATH,(HWND*)(AfxGetMainWnd()->m_hWnd))->SetWindowText(m_strCurrentPath);
	*pResult = 0;
}

//双击文件列表时，目录同步显示该文件夹
void CCatalogShowDlg::SetSelectByFileListClick(CString path)
{
	HTREEITEM tmpRoot=m_CatalogShow.GetChildItem(m_MyDisk);
	CString tmpPath=m_strUserLoginName;
	if(path!=tmpPath)
	{
		int pos;
		while(tmpRoot!=NULL)
		{
			tmpPath=tmpPath+_T("\\")+m_CatalogShow.GetItemText(tmpRoot);
			if(tmpPath!=path)
			{
				pos=path.Find(tmpPath+_T("\\"));
				if(pos==0)
					tmpRoot=m_CatalogShow.GetChildItem(tmpRoot);
				else
				{
					pos=tmpPath.ReverseFind('\\');
					tmpPath=tmpPath.Left(pos);
					tmpRoot=m_CatalogShow.GetNextItem(tmpRoot,TVGN_NEXT);
				}
			}
			else
				break;
		}
	}
	else
		tmpRoot=m_MyDisk;
	m_CatalogShow.Select(tmpRoot, TVGN_CARET);
	m_CatalogShow.Select(tmpRoot, TVGN_DROPHILITE);
	//设置展开属性
	m_CatalogShow.Expand(tmpRoot,TVE_EXPAND);

}
