// RecycleFileHistriyPath.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "RecycleFileHistriyPath.h"


// CRecycleFileHistriyPath

CRecycleFileHistriyPath::CRecycleFileHistriyPath()
{


}
CRecycleFileHistriyPath::CRecycleFileHistriyPath(CString userName)
{
	m_strIndexPath=_T("E:\\企业网盘目录\\回收站索引目录\\");
	m_strHistroyRootPath=_T("");
	m_strCurrentRootPath=_T("");
	m_strFileName.Format(_T("%s_file_hisPath.txt"),userName);
	m_pFilePathArr=new CArray<CString,CString&>();
}

CRecycleFileHistriyPath::~CRecycleFileHistriyPath()
{
}
//增加一条记录
BOOL CRecycleFileHistriyPath::AddIndex(CString curPath,CString hisIndex)
{
	CStdioFile stdioFile;
	CString path=m_strIndexPath+m_strFileName;
	CString tmpStr=curPath+_T("|")+hisIndex;
	if(stdioFile.Open(path,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		int n=stdioFile.SeekToEnd();
		if(n != 0)
			stdioFile.WriteString(_T("\n"));
		stdioFile.WriteString(tmpStr);
		stdioFile.Close();
	}
	else
		return FALSE;
	
	return TRUE;
}

//删除一条记录
BOOL CRecycleFileHistriyPath::DeleteIndex(CString curPath,CString &hisPath)
{
	CStdioFile fileRead;
	CStdioFile fileWrite;
	CString path=m_strIndexPath+m_strFileName;
	
	CString tmpStr;
	if(fileRead.Open(path,CFile::modeRead))
	{
		while(fileRead.ReadString(tmpStr))
		{
			int pos= tmpStr.Find('|');
			int len=curPath.GetLength();
			if((0 == tmpStr.Find(curPath)) && ( len == pos))
			{
				hisPath=tmpStr.Right(tmpStr.GetLength()-pos-1);
				continue;
			}
			m_pFilePathArr->Add(tmpStr);
		}
		fileRead.Close();
	}
	else
		return FALSE;
	if(fileWrite.Open(path,CFile::modeCreate|CFile::modeWrite))
	{
		for(int i=0;i<m_pFilePathArr->GetCount();i++)
		{
			fileWrite.WriteString(m_pFilePathArr->GetAt(i));
			fileWrite.WriteString(_T("\n"));
		}
		fileWrite.Close();
	}
	else
		return FALSE;
	return TRUE;
}
//清空索引文件
void CRecycleFileHistriyPath::CleanIndexFile()
{
	CString path=m_strIndexPath+m_strFileName;
	DeleteFile(path);

}
// CRecycleFileHistriyPath member functions
