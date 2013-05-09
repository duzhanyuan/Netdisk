// SearchFile.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "SearchFile.h"
#include "NetdiskClientDlg.h"

// CSearchFile

CSearchFile::CSearchFile()
{
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();
	m_strCurrentPath=m_pMainDlg->m_strCurrentPath;
	m_pMainDlg->m_pSearchFile=this;

}

CSearchFile::~CSearchFile()
{
}

void CSearchFile::SendSearchStr(CString searStr)
{
	if(m_pMainDlg->m_Client.FindFileByStr(searStr))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowFindFileList(&m_pMainDlg->m_lcFileShow,m_strIndexInfo);
	}
}

void CSearchFile::ShowFindFileList(CListCtrl* pListCtrl,CString indexInfo)
{
	pListCtrl->DeleteAllItems();

	CArray<CString,CString&>* pIndexInfoArr=new CArray<CString,CString&>();
	CString tmpStr=indexInfo;
	int pos=tmpStr.Find(';');
	while(pos != -1)
	{
		pIndexInfoArr->Add(tmpStr.Left(pos));
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
		pos=tmpStr.Find(';');
	}

	int count=pIndexInfoArr->GetCount();
	if(count == 0)
		return ;
	int nType;
	CString strName,strSize,strTime;
	m_pMapFilePath=new CMap<CString,LPCTSTR,CString,LPCTSTR>();
	for(int i=0;i<count;i++)
	{
		strName=m_pMainDlg->getFileListItemName(pIndexInfoArr->GetAt(i));
		strSize=m_pMainDlg->getFileListItemSize(pIndexInfoArr->GetAt(i));
		strTime=m_pMainDlg->getFileListItemEditTime(pIndexInfoArr->GetAt(i));
		m_pMapFilePath->SetAt(strName+_T("|")+strTime,getFindFilePath(pIndexInfoArr->GetAt(i)));

		if(strSize == _T(""))
			nType=0;
		else
			nType=m_pMainDlg->m_cSysIcon.GetExtIconID(strName);

		//插入数据
		LVITEM lvi;
		CString strItem;

		lvi.mask = TVIF_TEXT|TVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText =(LPTSTR)(LPCTSTR)strName;
		lvi.iImage = nType;		
		pListCtrl->InsertItem(&lvi);

		lvi.iSubItem =1;
		lvi.pszText = (LPTSTR)(LPCTSTR)strSize;
		pListCtrl->SetItem(&lvi);

		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
		pListCtrl->SetItem(&lvi);

	}
}

//获取索引中的路径信息
CString CSearchFile::getFindFilePath(CString indexInfo)
{
	CString tmpStr=indexInfo;
	int pos;
	//文件夹索引信息
	if(indexInfo[0] == 'D')
	{
		for(int i=0;i<4;i++)
		{
			pos=tmpStr.Find('|');
			tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
		}
		pos=tmpStr.Find('|');
		return tmpStr.Left(pos);
	}
	//文件索引信息
	if(indexInfo[0] == 'F')
	{
		for(int i=0;i<2;i++)
		{
			pos=tmpStr.Find('|');
			tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
		}
		pos=tmpStr.Find('|');
		return tmpStr.Left(pos);
	}
	else
		return _T("");
}
// CSearchFile member functions
