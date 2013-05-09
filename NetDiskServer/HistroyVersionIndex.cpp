// HistroyVersionIndex.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "HistroyVersionIndex.h"
#include "md5.h"
#include <iostream>
using std::ios;
// CHistroyVersionIndex

CHistroyVersionIndex::CHistroyVersionIndex()
{
	m_strHisVerRootIndexPath=_T("E:\\企业网盘目录\\文件历史版本索引目录\\");
	m_strHisVerRootPath=_T("E:\\企业网盘目录\\文件历史版本\\");
	m_strDiskRootPath=_T("E:\\企业网盘目录\\网盘用户\\");
}

CHistroyVersionIndex::~CHistroyVersionIndex()
{
}

//更新某个文件的历史版本索引信息
void CHistroyVersionIndex::UpdateIndex(CString userName,CString folderName)
{
	CString FloderPath=m_strHisVerRootPath+userName+_T("\\")+folderName;

	//打开索引文件
	CStdioFile openFile;
	CString indexFileName=folderName+_T(".txt");
	CString indexPath=m_strHisVerRootIndexPath+userName+_T("\\")+indexFileName;
	openFile.Open(indexPath,CFile::modeCreate|CFile::modeWrite);

	//写入索引信息
	CFileFind finder;
	CString s_name,s_time,s_size,s_md5,s_path;
	CTime file_time;
	CString strTmp;
	char c_path[100];
	BOOL ret=finder.FindFile(FloderPath+_T("\\*.*"));

	while(ret)
	{
		ret=finder.FindNextFile();
		if(finder.IsDots())
			continue;
		if(finder.IsDirectory())
			continue;
		else
		{
			//获取文件信息
			s_name=finder.GetFileName();
			finder.GetCreationTime(file_time);
			s_time = file_time.Format(_T("%Y/%m/%d %H:%M:%S"));
			_tprintf_s(_T("%s\n"), (LPCTSTR) s_time);
			ULONGLONG fileSize=finder.GetLength();
			if(fileSize<1024)
				s_size.Format(_T("%.1lf B"),(double)fileSize);
			else if(fileSize>=1024&&fileSize<(1024*1024))
				s_size.Format(_T("%.1lf KB"),(double)fileSize/1024.0f);
			else
				s_size.Format(_T("%.1lf MB"),(double)fileSize/(1024*1024.0f));
			s_path=finder.GetFilePath();
			int n=s_path.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,s_path,s_path.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,s_path,s_path.GetLength()+1,c_path,len+1,NULL,NULL);
			c_path[len]='\0';
			s_md5=MD5(ifstream(c_path, ios::binary)).toString().c_str();

			//写入索引信息
			openFile.WriteString(s_name+_T("|"));
			openFile.WriteString(s_size+_T("|"));
			openFile.WriteString(s_time+_T("|"));
			openFile.WriteString(s_path+_T("|"));
			openFile.WriteString(s_md5+_T(";"));
			openFile.WriteString(_T("\n"));
		}
	}
	finder.Close();
	openFile.Close();

}

//判断目录是否存在
bool CHistroyVersionIndex::IfFloderExist(CString path)
{
	WIN32_FIND_DATA wfd;
	bool reVal=false;
	HANDLE hFind=FindFirstFile(path,&wfd);
	if((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		reVal=true;
	FindClose(hFind);
	return reVal;
}

//获取当前文件的MD5码
CString CHistroyVersionIndex::GetCurrentMd5Code(CString filePath)
{
	//获取该文件的md5码
	CString strMd5;
	char c_path[1000];
	int n=filePath.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,filePath,filePath.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,filePath,filePath.GetLength()+1,c_path,len+1,NULL,NULL);
	c_path[len]='\0';
	strMd5=MD5(ifstream(c_path, ios::binary)).toString().c_str();
	return strMd5;
}

//判断该文件是否存在相同版本
bool CHistroyVersionIndex::IfFileVerExist(CString userName,CString filePath)
{
	
	CString strMd5=GetCurrentMd5Code(filePath);
	//通过索引文件查找是否有相同的md5码
	int pos=filePath.ReverseFind('\\');
	CString folderName=filePath.Right(filePath.GetLength()-pos-1);
	folderName.Replace('.','_');
	//更新索引
	UpdateIndex(userName,folderName);

	folderName = folderName +_T(".txt");

	CStdioFile openFile;
	CString Path=m_strHisVerRootIndexPath+userName+_T("\\")+folderName;
	CString tmpStr;
	if(openFile.Open(Path,CFile::modeRead))
	{
		while(openFile.ReadString(tmpStr))
		{
			pos=tmpStr.Find(strMd5);
			if(pos == -1)
				continue;
			if(pos+strMd5.GetLength()+1 == tmpStr.GetLength())
			{
				openFile.Close();
				return true;
			}
		}

		openFile.Close();
		return false;
	}
}

//获取索引信息
CString CHistroyVersionIndex::GetHisVerIndex(CString userName,CString fileName)
{
	//更新索引文件
	CString floderName=fileName;
	floderName.Replace('.','_');
	UpdateIndex(userName,floderName);

	CString indexInfo;
	CStdioFile openFile;
	CString strName=fileName;
	strName.Replace('.','_');
	strName = strName+_T(".txt");
	CString filePath=m_strHisVerRootIndexPath+userName+_T("\\")+strName;
	CString tmpStr;
	if(openFile.Open(filePath,CFile::modeRead))
	{
		while(openFile.ReadString(tmpStr))
		{
			indexInfo += tmpStr;
		}

		openFile.Close();
	}

	return indexInfo;
}

//获取最新版本号
int CHistroyVersionIndex::GetFileVersion(CString userName,CString folderName)
{
	CString tmpStr=folderName;
	tmpStr.Replace('.','_');
	CString path=m_strHisVerRootPath+userName+_T("\\")+tmpStr;
	CFileFind finder;
	BOOL ret=finder.FindFile(path+_T("\\*.*"));
	int verNo=1;
	while(ret)
	{
		ret=finder.FindNextFile();
		if(finder.IsDots())
			continue;
		if(finder.IsDirectory())
			continue;
		else
		{
			verNo++;
		}
	}
	finder.Close();
	return verNo;
}

//添加一个版本记录
void CHistroyVersionIndex::AddIndex(CString userName,CString srcPath)
{
	int pos=srcPath.ReverseFind('\\');
	CString folderName=srcPath.Right(srcPath.GetLength()-pos-1);
	folderName.Replace('.','_');
	int verNo=GetFileVersion(userName,folderName);
	pos=srcPath.ReverseFind('.');
	CString strPostFix=srcPath.Right(srcPath.GetLength()-pos-1);
	CString newFile;
	newFile.Format(_T("版本%d.%s"),verNo,strPostFix);
	//判断该文件的版本库文件夹是否存在
	CString FloderPath=m_strHisVerRootPath+userName+_T("\\")+folderName;
	if(!IfFloderExist(FloderPath))
	{
		CreateDirectory(FloderPath,NULL);
	}
	CopyFile(srcPath,FloderPath+_T("\\")+newFile,0);

	//更新索引
	UpdateIndex(userName,folderName);
}

//还原一个版本记录
void CHistroyVersionIndex::RestoreIndex(CString userName,CString strInfo)
{
	//获取目标路径
	int pos=strInfo.Find(':');
	CString desPath=m_strDiskRootPath+strInfo.Left(pos);
	CString srcFileName=strInfo.Right(strInfo.GetLength()-pos-1);
	//获取文件夹名称
	pos=desPath.ReverseFind('\\');
	CString floderName=desPath.Right(desPath.GetLength()-pos-1);
	floderName.Replace('.','_');
	//获取还原版本文件所在路径
	pos=desPath.Find('.');
	CString strPostFix=desPath.Right(desPath.GetLength()-pos);
	CString srcPath=m_strHisVerRootPath+userName+_T("\\")+floderName+_T("\\")+srcFileName+strPostFix;
	
	//将版本文件拷贝到恢复文件处
	CopyFile(srcPath,desPath,0);

	//更新索引文件
	UpdateIndex(userName,floderName);
}

//删除一个版本
void CHistroyVersionIndex::DelIndex(CString userName,CString strInfo)
{
	//获取文件夹名称
	int pos=strInfo.Find(':');
	CString floderName=strInfo.Left(pos);
	CString fileName=strInfo.Right(strInfo.GetLength()-pos-1);
	//获取后缀名
	pos=floderName.Find('.');
	CString strPostFix=floderName.Right(floderName.GetLength()-pos);
	floderName.Replace('.','_');
	CString path=m_strHisVerRootPath+userName+_T("\\")+floderName+_T("\\")+fileName+strPostFix;
	DeleteFile(path);
	//更新索引文件
	UpdateIndex(userName,floderName);

}
//清空所有版本
void CHistroyVersionIndex::ClearIndex(CString userName,CString strInfo)
{
	//获取文件夹名称
	CString floderName=strInfo;
	floderName.Replace('.','_');
	CString path=m_strHisVerRootPath+userName+_T("\\")+floderName;
	
	CFileFind finder;
	BOOL ret=finder.FindFile(path+_T("\\*.*"));

	while(ret)
	{
		ret=finder.FindNextFile();
		if(finder.IsDirectory())
			continue;
		if(finder.IsDots())
			continue;
		else
		{
			DeleteFile(finder.GetFilePath());
		}
	}
	RemoveDirectory(path);
	finder.Close();
	//更新索引文件
	UpdateIndex(userName,floderName);

}

// CHistroyVersionIndex member functions
