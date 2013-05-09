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
	m_strHisVerRootIndexPath=_T("E:\\��ҵ����Ŀ¼\\�ļ���ʷ�汾����Ŀ¼\\");
	m_strHisVerRootPath=_T("E:\\��ҵ����Ŀ¼\\�ļ���ʷ�汾\\");
	m_strDiskRootPath=_T("E:\\��ҵ����Ŀ¼\\�����û�\\");
}

CHistroyVersionIndex::~CHistroyVersionIndex()
{
}

//����ĳ���ļ�����ʷ�汾������Ϣ
void CHistroyVersionIndex::UpdateIndex(CString userName,CString folderName)
{
	CString FloderPath=m_strHisVerRootPath+userName+_T("\\")+folderName;

	//�������ļ�
	CStdioFile openFile;
	CString indexFileName=folderName+_T(".txt");
	CString indexPath=m_strHisVerRootIndexPath+userName+_T("\\")+indexFileName;
	openFile.Open(indexPath,CFile::modeCreate|CFile::modeWrite);

	//д��������Ϣ
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
			//��ȡ�ļ���Ϣ
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

			//д��������Ϣ
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

//�ж�Ŀ¼�Ƿ����
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

//��ȡ��ǰ�ļ���MD5��
CString CHistroyVersionIndex::GetCurrentMd5Code(CString filePath)
{
	//��ȡ���ļ���md5��
	CString strMd5;
	char c_path[1000];
	int n=filePath.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,filePath,filePath.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,filePath,filePath.GetLength()+1,c_path,len+1,NULL,NULL);
	c_path[len]='\0';
	strMd5=MD5(ifstream(c_path, ios::binary)).toString().c_str();
	return strMd5;
}

//�жϸ��ļ��Ƿ������ͬ�汾
bool CHistroyVersionIndex::IfFileVerExist(CString userName,CString filePath)
{
	
	CString strMd5=GetCurrentMd5Code(filePath);
	//ͨ�������ļ������Ƿ�����ͬ��md5��
	int pos=filePath.ReverseFind('\\');
	CString folderName=filePath.Right(filePath.GetLength()-pos-1);
	folderName.Replace('.','_');
	//��������
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

//��ȡ������Ϣ
CString CHistroyVersionIndex::GetHisVerIndex(CString userName,CString fileName)
{
	//���������ļ�
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

//��ȡ���°汾��
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

//���һ���汾��¼
void CHistroyVersionIndex::AddIndex(CString userName,CString srcPath)
{
	int pos=srcPath.ReverseFind('\\');
	CString folderName=srcPath.Right(srcPath.GetLength()-pos-1);
	folderName.Replace('.','_');
	int verNo=GetFileVersion(userName,folderName);
	pos=srcPath.ReverseFind('.');
	CString strPostFix=srcPath.Right(srcPath.GetLength()-pos-1);
	CString newFile;
	newFile.Format(_T("�汾%d.%s"),verNo,strPostFix);
	//�жϸ��ļ��İ汾���ļ����Ƿ����
	CString FloderPath=m_strHisVerRootPath+userName+_T("\\")+folderName;
	if(!IfFloderExist(FloderPath))
	{
		CreateDirectory(FloderPath,NULL);
	}
	CopyFile(srcPath,FloderPath+_T("\\")+newFile,0);

	//��������
	UpdateIndex(userName,folderName);
}

//��ԭһ���汾��¼
void CHistroyVersionIndex::RestoreIndex(CString userName,CString strInfo)
{
	//��ȡĿ��·��
	int pos=strInfo.Find(':');
	CString desPath=m_strDiskRootPath+strInfo.Left(pos);
	CString srcFileName=strInfo.Right(strInfo.GetLength()-pos-1);
	//��ȡ�ļ�������
	pos=desPath.ReverseFind('\\');
	CString floderName=desPath.Right(desPath.GetLength()-pos-1);
	floderName.Replace('.','_');
	//��ȡ��ԭ�汾�ļ�����·��
	pos=desPath.Find('.');
	CString strPostFix=desPath.Right(desPath.GetLength()-pos);
	CString srcPath=m_strHisVerRootPath+userName+_T("\\")+floderName+_T("\\")+srcFileName+strPostFix;
	
	//���汾�ļ��������ָ��ļ���
	CopyFile(srcPath,desPath,0);

	//���������ļ�
	UpdateIndex(userName,floderName);
}

//ɾ��һ���汾
void CHistroyVersionIndex::DelIndex(CString userName,CString strInfo)
{
	//��ȡ�ļ�������
	int pos=strInfo.Find(':');
	CString floderName=strInfo.Left(pos);
	CString fileName=strInfo.Right(strInfo.GetLength()-pos-1);
	//��ȡ��׺��
	pos=floderName.Find('.');
	CString strPostFix=floderName.Right(floderName.GetLength()-pos);
	floderName.Replace('.','_');
	CString path=m_strHisVerRootPath+userName+_T("\\")+floderName+_T("\\")+fileName+strPostFix;
	DeleteFile(path);
	//���������ļ�
	UpdateIndex(userName,floderName);

}
//������а汾
void CHistroyVersionIndex::ClearIndex(CString userName,CString strInfo)
{
	//��ȡ�ļ�������
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
	//���������ļ�
	UpdateIndex(userName,floderName);

}

// CHistroyVersionIndex member functions
