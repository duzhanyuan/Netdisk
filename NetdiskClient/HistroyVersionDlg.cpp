// HistroyVersionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "HistroyVersionDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"
// CHistroyVersionDlg dialog

IMPLEMENT_DYNAMIC(CHistroyVersionDlg, CDialogEx)

CHistroyVersionDlg::CHistroyVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistroyVersionDlg::IDD, pParent)
{
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();
}

CHistroyVersionDlg::~CHistroyVersionDlg()
{
}

void CHistroyVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HISTROYFILE, m_lcHistroyVersionFile);
}


BEGIN_MESSAGE_MAP(CHistroyVersionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RESTORE, &CHistroyVersionDlg::OnBnClickedBtnRestore)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CHistroyVersionDlg::OnBnClickedBtnRefresh)
	ON_NOTIFY(NM_CLICK, IDC_LIST_HISTROYFILE, &CHistroyVersionDlg::OnNMClickListHistroyfile)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CHistroyVersionDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_CLEAN, &CHistroyVersionDlg::OnBnClickedBtnClean)
END_MESSAGE_MAP()


// CHistroyVersionDlg message handlers


BOOL CHistroyVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lcHistroyVersionFile.InsertColumn(0,_T("文件名称"),LVCFMT_CENTER,225);
	m_lcHistroyVersionFile.InsertColumn(1,_T("大小"),LVCFMT_CENTER,100);
	m_lcHistroyVersionFile.InsertColumn(2,_T("时间"),LVCFMT_CENTER,100);
	m_lcHistroyVersionFile.InsertColumn(3,_T("版本号"),LVCFMT_CENTER,100);

	m_lcHistroyVersionFile.ModifyStyle(0,LVS_SHOWSELALWAYS|LVS_REPORT|LVS_SINGLESEL|LVSIL_NORMAL);
	m_lcHistroyVersionFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);
	m_lcHistroyVersionFile.SetImageList(&m_pMainDlg->m_cSysIcon.m_ImageSmallList,LVSIL_SMALL);
	//CHeaderCtrl* pHeadctrl=(CHeaderCtrl*)m_lcHistroyVersionFile.GetHeaderCtrl();
	//pHeadctrl->EnableWindow(FALSE);
	InitHisVerFileList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//恢复某个版本的文件
void CHistroyVersionDlg::OnBnClickedBtnRestore()
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<m_lcHistroyVersionFile.GetItemCount();i++)
	{
		if(m_lcHistroyVersionFile.GetCheck(i))
		{
			if(_T("当前版本")== m_lcHistroyVersionFile.GetItemText(i,3))
			{
				AfxMessageBox(_T("不能还原当前版本，请重新选择"));
				return;
			}
		}
	}

	CString filePath;
	for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(m_pMainDlg->m_lcFileShow.GetCheck(i))
		{
			CString fileName=m_pMainDlg->m_lcFileShow.GetItemText(i,0);
			filePath=m_pMainDlg->m_strCurrentPath+_T("\\")+fileName;
		}
	}
	for(int i=0; i< m_lcHistroyVersionFile.GetItemCount();i++)
	{
		if(m_lcHistroyVersionFile.GetCheck(i))
		{
			CString verName=m_lcHistroyVersionFile.GetItemText(i,3);
			CString sendMsg=filePath+_T(":")+verName;
			//发送还原历史版本的信息
			if(m_pMainDlg->m_Client.ReStoreHisVerFile(sendMsg))
			{
				InitHisVerFileList();
				//更新主客户端显示
				m_pMainDlg->OnBnClickedBtnRefresh();
			}
		}
	}
}

void CHistroyVersionDlg::InitHisVerFileList()
{
	for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(m_pMainDlg->m_lcFileShow.GetCheck(i))
		{
			CString fileName=m_pMainDlg->m_lcFileShow.GetItemText(i,0);
			CString path=m_pMainDlg->m_strCurrentPath+_T("\\")+fileName;
			//发送获取历史版本的信息
			if(m_pMainDlg->m_Client.GetHistroyVersionInfo(path))
			{
				m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
				ShowHisVerFileList(fileName,m_strIndexInfo);
			}
		}
	}
}

//显示历史版本信息
void CHistroyVersionDlg::ShowHisVerFileList(CString fileName,CString strInfo)
{
	m_lcHistroyVersionFile.DeleteAllItems();

	int pos=strInfo.ReverseFind(';');
	CString CurrentMd5Code=strInfo.Right(strInfo.GetLength()-pos-1);
	strInfo = strInfo.Left(pos+1);

	CArray<CString,CString&>* pIndexInfoArr=new CArray<CString,CString&>();
	pos=strInfo.Find(';');
	while(pos != -1)
	{
		pIndexInfoArr->Add(strInfo.Left(pos));
		strInfo = strInfo.Right(strInfo.GetLength()-pos-1);
		pos=strInfo.Find(';');
	}
	int count = pIndexInfoArr->GetCount();
	if(count == 0)
		return ;
	int nType;
	CString strSize,strTime,strVersion,strMd5Code;
	for(int i=0;i<count;i++)
	{
		strSize=getFileListItemSize(pIndexInfoArr->GetAt(i));
		strTime=getFileListItemEditTime(pIndexInfoArr->GetAt(i));
		strVersion=getFileListItemVersionNo(pIndexInfoArr->GetAt(i));
		strMd5Code=getFileListItemMd5Code(pIndexInfoArr->GetAt(i));
		if(CurrentMd5Code == strMd5Code)
			strVersion=_T("当前版本");

		nType=m_pMainDlg->m_cSysIcon.GetExtIconID(fileName);
		//插入数据
		LVITEM lvi;
		CString strItem;

		lvi.mask = TVIF_TEXT|TVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText =(LPTSTR)(LPCTSTR)fileName;
		lvi.iImage = nType;		
		m_lcHistroyVersionFile.InsertItem(&lvi);

		lvi.iSubItem =1;
		lvi.pszText = (LPTSTR)(LPCTSTR)strSize;
		m_lcHistroyVersionFile.SetItem(&lvi);

		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
		m_lcHistroyVersionFile.SetItem(&lvi);

		lvi.iSubItem =3;
		lvi.pszText = (LPTSTR)(LPCTSTR)strVersion;
		m_lcHistroyVersionFile.SetItem(&lvi);
	}

}
//获取MD5码信息
CString CHistroyVersionDlg::getFileListItemMd5Code(CString indexInfo)
{
	CString tmpStr=indexInfo;
	int pos=tmpStr.ReverseFind('|');
	tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);

	return tmpStr;
}
//获取版本号信息
CString CHistroyVersionDlg::getFileListItemVersionNo(CString indexInfo)
{
	int pos=indexInfo.Find('|');
	CString tmpStr=indexInfo.Left(pos);
	pos=tmpStr.Find('.');
	tmpStr=tmpStr.Left(pos);
	return tmpStr;
}

//获取文件时间信息
CString CHistroyVersionDlg::getFileListItemEditTime(CString indexInfo)
{
	CString tmpStr=indexInfo;
	int pos;
	for(int i=0;i<2;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}
	pos=tmpStr.Find('|');
	return tmpStr.Left(pos);
}
//获取文件大小信息
CString CHistroyVersionDlg::getFileListItemSize(CString indexInfo)
{
	CString tmpStr=indexInfo;
	int pos=tmpStr.Find('|');
	tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	pos=tmpStr.Find('|');

	return tmpStr.Left(pos);
}

//刷新显示
void CHistroyVersionDlg::OnBnClickedBtnRefresh()
{
	// TODO: Add your control notification handler code here
	InitHisVerFileList();
}

//设置一次只选中一个checkbox
void CHistroyVersionDlg::OnNMClickListHistroyfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int checkItem=pNMItemActivate->iItem;
	if(!m_lcHistroyVersionFile.GetCheck(checkItem))
	{
		for(int i=0; i<m_lcHistroyVersionFile.GetItemCount(); i++)
			m_lcHistroyVersionFile.SetCheck(i,0);
		m_lcHistroyVersionFile.SetCheck(checkItem,1);
	}
	*pResult = 0;
}

//删除一个版本的文件
void CHistroyVersionDlg::OnBnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	for(int i=0; i< m_lcHistroyVersionFile.GetItemCount();i++)
	{
		if(m_lcHistroyVersionFile.GetCheck(i))
		{
			CString fileName=m_lcHistroyVersionFile.GetItemText(i,0);
			CString verName=m_lcHistroyVersionFile.GetItemText(i,3);
			CString sendMsg=fileName+_T(":")+verName;
			//发送删除历史版本的信息
			if(m_pMainDlg->m_Client.DeleteHisVerFile(sendMsg))
			{
				InitHisVerFileList();
			}
		}
	}
}

//清空所有的版本
void CHistroyVersionDlg::OnBnClickedBtnClean()
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(m_pMainDlg->m_lcFileShow.GetCheck(i))
		{
			CString fileName=m_pMainDlg->m_lcFileShow.GetItemText(i,0);
			//发送获取历史版本的信息
			if(m_pMainDlg->m_Client.CleanHisVerFile(fileName))
			{
				InitHisVerFileList();
			}
		}
	}
}
