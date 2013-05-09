// ServIndex.cpp : implementation file
//�������ļ�ϵͳ������

#include "stdafx.h"
#include "NetDiskServer.h"
#include "ServIndex.h"
#include "locale.h"
// CServIndex

CServIndex::CServIndex()
{
	m_strRootPath=_T("E:\\��ҵ����Ŀ¼\\�����û�\\");
	m_strIndexPath=_T("E:\\��ҵ����Ŀ¼\\�����û�����Ŀ¼\\");
	m_strRecyclePath=_T("E:\\��ҵ����Ŀ¼\\����վ\\");
	m_strRecycleIndexPath=_T("E:\\��ҵ����Ŀ¼\\����վ����Ŀ¼\\");
	m_catalogIndexHead=new CCatalogIndex();
	
}

CServIndex::~CServIndex()
{
	if(m_catalogIndexHead!=NULL)
		delete m_catalogIndexHead;
}

//��ȡ��Ŀ¼��
int CServIndex::getSubCatalogCount(CCatalogIndex* catalogHead)
{
	int count=0;
	CCatalogIndex* head;
	if(catalogHead!=NULL)
		head=catalogHead->m_subCatalog;
	else
		return count;


	while (head!=NULL)
	{
		count++;
		head=head->m_NextCatalog;
	}

	return count;
}

//��ȡͬ��Ŀ¼��
int CServIndex::getNextCatalogCount(CCatalogIndex* catalogHead)
{
	int count=0;
	CCatalogIndex* head=catalogHead;

	while(head !=NULL)
	{
		count++;
		head=head->m_NextCatalog;
	}
	return count;
}
//��ȡ���ļ���
int CServIndex::getSubFileCount(CFileIndex* fileHead)
{
	int count=0;

	CFileIndex* head=fileHead;

	while(NULL != head)
	{
		count++;
		head=head->m_Next;
	}

	return count;
}

//д��Ŀ¼������Ϣ
//void CServIndex::writeSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
//{
//	int count=getSubCatalogCount(catalogHead);
//	CArchive carchive(pcFile,CArchive::store);
//	carchive<<count;
//
//	while(catalogHead !=NULL)
//	{
//		carchive<<catalogHead->m_strName<<catalogHead->m_strPath<<catalogHead->m_strEditTime<<catalogHead->m_bIsShare<<catalogHead->m_strSharePath;
//		//writeSubCatalogInfo(catalogHead->m_subCatalog,pcFile);
//		catalogHead=catalogHead->m_NextCatalog;
//	}
//
//}

//д���ļ�������Ϣ
//void CServIndex::writeSubFileInfo(CFileIndex* fileIndex,CFile* pcFile)
//{
//	int count=getSubFileCount(fileIndex);
//	CArchive carchive(pcFile,CArchive::store);
//	carchive<<count;
//
//	while(fileIndex != NULL)
//	{
//		carchive<<fileIndex->m_strName<<fileIndex->m_strPath<<fileIndex->m_strEditTime<<fileIndex->m_bIsShare<<fileIndex->m_strSharePath;
//
//
//		fileIndex=fileIndex->m_Next;
//	}
//}

//д��ͬ��Ŀ¼��Ϣ
//void CServIndex::writeNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
//{
//	int count=getNextCatalogCount(catalogHead);
//	CArchive carchive(pcFile,CArchive::store);
//	carchive<<count;
//
//	while(catalogHead !=NULL)
//	{
//		carchive<<catalogHead->m_strName<<catalogHead->m_strPath<<catalogHead->m_strEditTime<<catalogHead->m_bIsShare<<catalogHead->m_strSharePath;
//		writeSubFileInfo(catalogHead->m_subFile,pcFile);
//		writeSubCatalogInfo(catalogHead->m_subCatalog,pcFile);
//
//		catalogHead=catalogHead->m_NextCatalog;
//	}
//}


//д���ļ�������Ϣ
void CServIndex::writeSubFileInfo(CFileIndex* fileHead,CStdioFile* pStdFile)
{
	//int count;
	//CString tmp;
	//		count=getSubFileCount(fileHead);
	//	tmp.Format(_T("%d"),count);
	//	pStdFile->WriteString(tmp+_T("-"));

	while(fileHead!=NULL)
	{
		pStdFile->WriteString(_T("F|"));//D������Ŀ¼��F�������ļ�

		pStdFile->WriteString(fileHead->m_strName+_T("|"));
		//pStdFile->WriteString(fileHead->m_strParentCatalogName+_T("|"));
		pStdFile->WriteString(fileHead->m_strPath+_T("|"));
		pStdFile->WriteString(fileHead->m_strEditTime+_T("|"));
		pStdFile->WriteString(fileHead->m_strFileSize+_T("|"));
		if(fileHead->m_bIsShare)
		{
			pStdFile->WriteString(_T("TRUE|"));
			pStdFile->WriteString(fileHead->m_strSharePath+_T(";"));
		}		
		else
		{
			pStdFile->WriteString(_T("FALSE|"));
			pStdFile->WriteString(_T("*;"));
		}
		pStdFile->WriteString(_T("\n"));
		fileHead=fileHead->m_Next;
	}

}
//void CServIndex::testWriteSub(CCatalogIndex* catalogHead,CStdioFile* pStdFile)
//{
//	int count=getSubCatalogCount(catalogHead);
//	//CString tmp;
//	//tmp.Format(_T("%d"),count);
//	//pStdFile->WriteString(tmp+_T("-"));
//	
//	while(catalogHead!=NULL)
//	{
//		pStdFile->WriteString(catalogHead->m_strName+_T("-"));
//		pStdFile->WriteString(catalogHead->m_strPath+_T("-"));
//		pStdFile->WriteString(catalogHead->m_strEditTime+_T("-"));
//		if(catalogHead->m_bIsShare)
//			pStdFile->WriteString(_T("TRUE-"));
//		else
//			pStdFile->WriteString(_T("FALSE-"));
//		pStdFile->WriteString(catalogHead->m_strSharePath+_T("-"));
//		pStdFile->WriteString(_T("\n"));
//		
//		testWriteSub(catalogHead->m_subCatalog,pStdFile);
//		testWriteFile(catalogHead->m_subFile,pStdFile);
//
//		catalogHead= catalogHead->m_subCatalog;
//	}
//
//}


//д��Ŀ¼��Ϣ
void CServIndex::writeNextCatalogInfo(CCatalogIndex* catalogHead,CStdioFile* pStdFile)
{
	CString tmp;
	int count;

	while(catalogHead!=NULL)
	{
		pStdFile->WriteString(_T("D|"));//D������Ŀ¼��F�������ļ�
		//д�����ļ�����
		count=getSubCatalogCount(catalogHead);
		tmp.Format(_T("%d"),count);
		pStdFile->WriteString(tmp+_T("|"));
		
		//д�����ļ�����
		count=getSubFileCount(catalogHead->m_subFile);
		tmp.Format(_T("%d"),count);
		pStdFile->WriteString(tmp+_T("|"));

		pStdFile->WriteString(catalogHead->m_strName+_T("|"));
		//pStdFile->WriteString(catalogHead->m_strParentCatalogName+_T("|"));
		pStdFile->WriteString(catalogHead->m_strPath+_T("|"));
		pStdFile->WriteString(catalogHead->m_strEditTime+_T("|"));
		if(catalogHead->m_bIsShare)
		{
			pStdFile->WriteString(_T("TRUE|"));
			pStdFile->WriteString(catalogHead->m_strSharePath+_T(";"));
		}
		else
		{
			pStdFile->WriteString(_T("FALSE|"));
			pStdFile->WriteString(_T("*;"));
		}
		pStdFile->WriteString(_T("\n"));

		writeNextCatalogInfo(catalogHead->m_subCatalog,pStdFile);
		writeSubFileInfo(catalogHead->m_subFile,pStdFile);
		catalogHead=catalogHead->m_NextCatalog;
	}

}
//��ȡ��Ŀ¼������Ϣ
void CServIndex::readSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
{
	int count=0;
	CCatalogIndex* head=catalogHead;
	CArchive carchive(pcFile,CArchive::load);
	carchive>>count;

	while(count--)
	{
		carchive>>head->m_strName>>head->m_strPath>>head->m_strEditTime>>head->m_bIsShare>>head->m_strSharePath;	
		//readSubFileInfo(head->m_subFile,pcFile);
		CCatalogIndex* newCatalogIndex=new CCatalogIndex();
		head->m_subCatalog=newCatalogIndex;
		head=newCatalogIndex;
	}
}

//��ȡ���ļ�������Ϣ
void CServIndex::readSubFileInfo(CFileIndex* fileHead,CFile* pcFile)
{
	int count=0;
	CFileIndex* head=fileHead;

	CArchive carchive(pcFile,CArchive::load);
	carchive>>count;

	while(count--)
	{
		carchive>>head->m_strName>>head->m_strPath>>head->m_strEditTime>>head->m_bIsShare>>head->m_strSharePath;
		CFileIndex* newFileIndex=new CFileIndex();
		head->m_Next=newFileIndex;
		head=newFileIndex;
	}
}

//��ȡͬ��Ŀ¼��Ϣ
void CServIndex::readNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile)
{
	int count=0;
	CCatalogIndex* head=catalogHead;

	CArchive carchive(pcFile,CArchive::load);
	carchive>>count;

	while(count--)
	{
		carchive>>head->m_strName>>head->m_strPath>>head->m_strEditTime>>head->m_bIsShare>>head->m_strSharePath;	
		readSubCatalogInfo(head->m_subCatalog,pcFile);
		readSubFileInfo(head->m_subFile,pcFile);
		CCatalogIndex* newCatalogIndex=new CCatalogIndex();
		head->m_NextCatalog=newCatalogIndex;
		head=newCatalogIndex;
	}
}

//�����������û������ļ�
BOOL CServIndex::UpdateIndex(CString rootPath,CString indexPath,CString rootName)
{
	//CString path=m_strIndexPath;
	CString path=indexPath;
	path+=rootName+_T(".txt");

	setlocale( LC_CTYPE, ("chs"));
	CStdioFile pStdioFile;
	if(pStdioFile.Open(path,CFile::modeCreate|CFile::modeWrite))
	{
		m_catalogIndexHead->m_NextCatalog=NULL;
		m_catalogIndexHead->m_subCatalog=NULL;
		m_catalogIndexHead->m_subFile=NULL;

		//����������Ϣ
		UpdateIndexList(rootPath+rootName,m_catalogIndexHead);
		writeNextCatalogInfo(m_catalogIndexHead->m_subCatalog,&pStdioFile);
		writeSubFileInfo(m_catalogIndexHead->m_subFile,&pStdioFile);
		//AfxMessageBox(_T("�����ļ������ɹ�!"));

		pStdioFile.Close();
		return TRUE;
	}
	//CFile file;
	//if(file.Open(path,CFile::modeCreate|CFile::modeWrite))
	//{
	//	//����������Ϣ
	//	UpdateIndex(m_strRootPath+rootName,m_catalogIndexHead);
	//	writeNextCatalogInfo(m_catalogIndexHead->m_subCatalog,&file);
	//	AfxMessageBox(_T("�����ļ������ɹ�!"));
	//	file.Close();
	//	return TRUE;
	//}

	return FALSE;
}

//�����û�Ŀ¼�����ļ�����
void CServIndex::UpdateIndexList(CString rootPath,CCatalogIndex* catalogIndexHead)
{
	BuildSubCatalogAndFileList(rootPath,catalogIndexHead);
	CCatalogIndex* tmp=catalogIndexHead->m_subCatalog;
	if(tmp == NULL)
		return ;
	RecurBuildCatalogList(tmp->m_strPath,tmp);
	/*while(tmp!=NULL)
	{
		BuildSubCatalogAndFileList(tmp->m_strPath,tmp);
		RecurBuildCatalogList(tmp->m_strPath,tmp);
		tmp=tmp->m_NextCatalog;
	}*/
}

//�ݹ齨����Ŀ¼����
void CServIndex::RecurBuildCatalogList(CString path,CCatalogIndex* rootIndex)
{
	CFileFind finder;
	BOOL isFound;
	CString strTmpDir;		//����ļ���·��
	CTime tmpTime;
	CString tmpStr;
	CCatalogIndex* tmpSubCatIndex=rootIndex->m_subCatalog;
	isFound=finder.FindFile(path+_T("\\*.*"));

	while(rootIndex!=NULL)
	{
		BuildSubCatalogAndFileList(rootIndex->m_strPath,rootIndex);
		if(rootIndex->m_subCatalog!=NULL )
			RecurBuildCatalogList(rootIndex->m_subCatalog->m_strPath,rootIndex->m_subCatalog);

		rootIndex=rootIndex->m_NextCatalog;
	}
	//BuildSubCatalogAndFileList(path,rootIndex);

	//while(isFound)
	//{
	//	isFound=finder.FindNextFile();
	//	if(finder.IsDots())
	//		continue;
	//	if(finder.IsDirectory())
	//	{
	//		strTmpDir=_T("");
	//		strTmpDir=finder.GetFilePath();

	//		if(IsDirecEmpty(strTmpDir))
	//			return;
	//		//�����һ��Ŀ¼�����ڵ�
	//		//CCatalogIndex* CatalogIndexItem=new CCatalogIndex();
	//		//CatalogIndexItem->m_strName=finder.GetFileName();
	//		//CatalogIndexItem->m_strPath=strTmpDir;
	//		//finder.GetLastAccessTime(tmpTime);
	//		//tmpStr = tmpTime.Format(_T("%c"));
	//		//_tprintf_s(_T("%s\n"), (LPCTSTR) tmpStr);
	//		//CatalogIndexItem->m_strEditTime=tmpStr;
	//		//CatalogIndexItem->m_bIsShare=FALSE;
	//		//CatalogIndexItem->m_strSharePath=_T("");
	//		//CatalogIndexItem->m_subFile=NULL;
	//		//CatalogIndexItem->m_NextCatalog=NULL;

	//		//CatalogIndexItem->m_subCatalog=rootIndex->m_subCatalog;
	//		//rootIndex->m_subCatalog=CatalogIndexItem;
	//		RecurBuildCatalogList(strTmpDir,rootIndex->m_subCatalog);
	//	}
	//}
	//rootIndex->m_subCatalog=tmpSubCatIndex;
}

//����ͬ��Ŀ¼�����ļ�������
void CServIndex::BuildSubCatalogAndFileList(CString rootPath,CCatalogIndex* rootIndex)
{
	CFileFind finder;
	BOOL isFounded;
	int iDirecCount=0;
	int iFileCount=0;
	//bool c_flag=false;
	//bool f_flag=false;
	CString strTmpDir;		//����ļ���·��
	CTime tmpTime;
	CString tmpStr;

	if(NULL == rootIndex)
		return ;
	if(IsDirecEmpty(rootPath))
		return ;
	//CCatalogIndex* tmpCatIndex=rootIndex->m_subCatalog;
	CCatalogIndex* tmpCatalogIndexHead=new CCatalogIndex();
	//CFileIndex* tmpFileIndex=rootIndex->m_subFile;
	CFileIndex* tmpFileIndexHead=new CFileIndex();
	

	isFounded=finder.FindFile(rootPath+_T("\\*.*"));

	while(isFounded)
	{
		isFounded=finder.FindNextFile();

		if(finder.IsDots())
			continue ;
		if(finder.IsDirectory())
		{
			iDirecCount++;
			strTmpDir=_T("");
			strTmpDir=finder.GetFilePath();
			//�����һ��Ŀ¼�����ڵ�
			CCatalogIndex* CatalogIndexItem=new CCatalogIndex();
			CatalogIndexItem->m_strName=finder.GetFileName();
			//CatalogIndexItem->m_strParentCatalogName=finder.GetRoot();
			CatalogIndexItem->m_strPath=strTmpDir;
			finder.GetLastWriteTime(tmpTime);
			tmpStr = tmpTime.Format(_T("%Y/%m/%d %H:%M:%S"));
			_tprintf_s(_T("%s\n"), (LPCTSTR) tmpStr);
			CatalogIndexItem->m_strEditTime=tmpStr;
			CatalogIndexItem->m_bIsShare=FALSE;
			CatalogIndexItem->m_strSharePath=_T("");
			CatalogIndexItem->m_subFile=NULL;
			CatalogIndexItem->m_NextCatalog=NULL;

			//���뵽��Ŀ¼����
			if(tmpCatalogIndexHead->m_strName==_T(""))
				tmpCatalogIndexHead=CatalogIndexItem;
			else
			{
				CatalogIndexItem->m_NextCatalog=tmpCatalogIndexHead->m_NextCatalog;
				tmpCatalogIndexHead->m_NextCatalog=CatalogIndexItem;
			}
			//CatalogIndexItem->m_NextCatalog=tmpCatIndex;
			//tmpCatIndex=CatalogIndexItem;
			//if(!c_flag)
			//{
			//	tmpCatalogIndexHead=CatalogIndexItem;
			//	c_flag=true;
			//}


		}
		else
		{
			iFileCount++;
			//��ӵ�ǰ�û��ļ���������ڵ�
			CFileIndex* fileIndexItem=new CFileIndex();
			fileIndexItem->m_strName=finder.GetFileName();
			//fileIndexItem->m_strParentCatalogName=finder.GetRoot();
			fileIndexItem->m_strPath=finder.GetFilePath();
			finder.GetLastAccessTime(tmpTime);
			tmpStr = tmpTime.Format(_T("%Y/%m/%d %H:%M:%S"));
			_tprintf_s(_T("%s\n"), (LPCTSTR) tmpStr);
			fileIndexItem->m_strEditTime=tmpStr;
			ULONGLONG fileSize=finder.GetLength();
			if(fileSize<1024)
				fileIndexItem->m_strFileSize.Format(_T("%.1lf B"),(double)fileSize);
			else if(fileSize>=1024&&fileSize<(1024*1024))
				fileIndexItem->m_strFileSize.Format(_T("%.1lf KB"),(double)fileSize/1024.0f);
			else
				fileIndexItem->m_strFileSize.Format(_T("%.1lf MB"),(double)fileSize/(1024*1024.0f));
			fileIndexItem->m_bIsShare=FALSE;
			fileIndexItem->m_strSharePath=_T("");

			//���뵽���ļ�����
			if(_T("") == tmpFileIndexHead->m_strName)
			{
				tmpFileIndexHead=fileIndexItem;
			}
			else
			{
				fileIndexItem->m_Next=tmpFileIndexHead->m_Next;
				tmpFileIndexHead->m_Next=fileIndexItem;
			}
			//fileIndexItem->m_Next=tmpFileIndex;
			//tmpFileIndex=fileIndexItem;
			//if(!f_flag)
			//{
			//	tmpFileIndexHead=fileIndexItem;
			//	f_flag=true;
			//}
		}
	}
	if(iDirecCount!=0)
		rootIndex->m_subCatalog=tmpCatalogIndexHead;
	else
		rootIndex->m_subCatalog=NULL;
	if(iFileCount!=0)
		rootIndex->m_subFile=tmpFileIndexHead;
	else
		rootIndex->m_subFile=NULL;
}

//�жϵ�ǰĿ¼�Ƿ�Ϊ��
BOOL CServIndex::IsDirecEmpty(CString path)
{
	CFileFind finder;
	int count=0;
	BOOL isWork;
	isWork=finder.FindFile(path+_T("\\*.*"));
	if(!isWork)
		return FALSE;
	while(isWork)
	{
		isWork=finder.FindNextFile();
		if(finder.IsDots())
			continue;
		count++;
	}
	return (count>0?FALSE:TRUE);
}
// CServIndex member functions

//��ȡ�û�������Ϣ
CString CServIndex::GetIndexInfo(CString indexPath,CString &rootName)
{
	CString path=indexPath;
	path += rootName+_T(".txt");
	setlocale( LC_CTYPE, ("chs"));

	CStdioFile pStdioFile;
	CString tmpStr;
	CString strIndexText;

	if(pStdioFile.Open(path,CFile::modeRead))
	{
		while(pStdioFile.ReadString(tmpStr))
		{
			strIndexText+=tmpStr;
		}
		pStdioFile.Close();
		return strIndexText;
	}
	else
		return NULL;
}

//��ȡĳһ��Ŀ¼�µ��ļ����ļ�����Ϣ
CString CServIndex::getCatalogInfo(CString rootPath,CString indexPath,CString floderPath)
{
	if(floderPath==_T(""))
		return _T("");

	CString	indexInfo=_T("");
	CString servFloderPath=rootPath+floderPath;
	int pos=floderPath.Find('\\');
	CString tmpStr,tmpName,tmpPath;
	CString oneIndexInfo;
	if(-1!=pos)
		tmpStr=GetIndexInfo(indexPath,floderPath.Left(pos));
	else
		tmpStr=GetIndexInfo(indexPath,floderPath);

	CArray<CString,CString&> *oneIndexInfoArr=new CArray<CString,CString&>();


	while(tmpStr.GetLength()>0)
	{
		oneIndexInfo=GetOneIndexInfo(tmpStr,pos);
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);

		oneIndexInfoArr->Add(oneIndexInfo);
	}

	for(int i=0;i<oneIndexInfoArr->GetSize();i++)
	{
		tmpName=GetCatalogName(oneIndexInfoArr->GetAt(i));
		tmpPath=GetPath(oneIndexInfoArr->GetAt(i));
		pos=tmpPath.Find(servFloderPath);
		if((pos==0)&&((tmpName.GetLength()+servFloderPath.GetLength()+1)==tmpPath.GetLength()))
		{
			indexInfo+=oneIndexInfoArr->GetAt(i);
		}
	}
	if(indexInfo==_T(""))
		indexInfo.Format(_T("%d+"),EMPTYCATALOG);
	else
	{
		tmpStr=indexInfo;
		indexInfo.Format(_T("%d+%s"),NONEMPTYCATALOG,tmpStr);
	}
	return indexInfo;
}

//��ȡ·������
CString CServIndex::GetPath(CString srcStr)
{
	if(srcStr==_T(""))
		return _T("");
	CString tmpName=GetCatalogName(srcStr);
	CString tmpPath,tmpStr=srcStr;
	int pos,iLength=0;
	if(srcStr[0]==_T('D'))
		iLength=4;
	if(srcStr[0]==_T('F'))
		iLength=2;
	//��ȡĿ¼·��
	for(int i=0;i<iLength;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}
	pos=tmpStr.Find('|');
	tmpPath=tmpStr.Left(pos);
	return tmpPath;
}
//��ȡ�ļ�����
CString CServIndex::GetCatalogName(CString srcStr)
{
	CString tmpStr;
	if(srcStr==_T(""))
		return _T("");

	int pos=0,iLength=0;
	tmpStr=srcStr;
	if(srcStr[0]==_T('D'))
		iLength=3;
	if(srcStr[0]==_T('F'))
		iLength=1;
	for(int i=0;i<iLength;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}

	pos=tmpStr.Find('|');
	return tmpStr.Left(pos);
}

//��ȡһ���������ļ����ļ���������¼
CString CServIndex::GetOneIndexInfo(CString srcStr,int &iPos)
{
	CString tmpStr;
	if(srcStr==_T(""))
		return _T("");

	tmpStr=srcStr;
	int pos=tmpStr.Find(';');
	iPos=pos;

	return tmpStr.Left(pos+1);
}

//����������¼
CString CServIndex::FindFileIndex(CString indexPath,CString rootName,CString findStr)
{
	CString indexInfo=GetIndexInfo(indexPath,rootName);
	CArray<CString,CString&>* pIndexInfoArr=new CArray<CString,CString&>();
	int pos;
	CString retIndexInfo;
	while(indexInfo.GetLength() > 0)
	{
		pos=indexInfo.Find(';');
		pIndexInfoArr->Add(indexInfo.Left(pos));
		indexInfo=indexInfo.Right(indexInfo.GetLength()-pos-1);
	}

	int count=pIndexInfoArr->GetCount();
	if(count == 0)
		return _T("");
	CString tmpName,tmpPath;
	for(int i=0;i<count;i++)
	{
		//������ҵ����û�����Ŀ¼�µ��ļ�
		if(m_strIndexPath == indexPath)
		{
			tmpName=GetCatalogName(pIndexInfoArr->GetAt(i));
			if(-1 != tmpName.Find(findStr))
				retIndexInfo=retIndexInfo+pIndexInfoArr->GetAt(i)+_T(";");

		}
		//������ҵ��ǻ���վĿ¼�µ��ļ�
		if(m_strRecycleIndexPath == indexPath)
		{
			tmpPath=GetPath(pIndexInfoArr->GetAt(i));
			tmpName=GetCatalogName(pIndexInfoArr->GetAt(i));
			CString cmpStr=m_strRecyclePath+rootName;
			if(tmpPath.GetLength() == cmpStr.GetLength()+1+tmpName.GetLength())
			{
				if(-1 != tmpName.Find(findStr))
					retIndexInfo=retIndexInfo+pIndexInfoArr->GetAt(i)+_T(";");
			}
		}
	}
	return retIndexInfo;
}

//��ʱ�����������¼
CString CServIndex::FindFileByTime(CString CurrentPath,CString strStartTime,CString StrEndTime)
{
	int pos;
	CString rootName;
	pos=CurrentPath.Find('\\');
	if(pos == -1)
		rootName=CurrentPath;
	else
		rootName=CurrentPath.Left(pos);
	CString indexInfo=GetIndexInfo(m_strIndexPath,rootName);

	CArray<CString,CString&>* pIndexInfoArr=new CArray<CString,CString&>();
	CString retIndexInfo;
	while(indexInfo.GetLength() > 0)
	{
		pos=indexInfo.Find(';');
		pIndexInfoArr->Add(indexInfo.Left(pos));
		indexInfo=indexInfo.Right(indexInfo.GetLength()-pos-1);
	}

	int count=pIndexInfoArr->GetCount();
	if(count == 0)
		return _T("");
	CString tmpTime,tmpPath,strPath;
	strPath=m_strRootPath+CurrentPath;
	for(int i=0;i<count;i++)
	{
		tmpPath=GetPath(pIndexInfoArr->GetAt(i));
		tmpTime=GetTime(pIndexInfoArr->GetAt(i));

		if((tmpTime<=StrEndTime && tmpTime>=strStartTime) && 0 == tmpPath.Find(strPath))
			retIndexInfo=retIndexInfo+pIndexInfoArr->GetAt(i)+_T(";");
	}

	return retIndexInfo;
}

//��ȡ�����е�ʱ���ַ���
CString CServIndex::GetTime(CString srcStr)
{
	int count;
	if(srcStr[0] == 'D')
		count = 5;
	if(srcStr[0] == 'F')
		count=3;

	int pos;
	CString tmpStr=srcStr;
	for(int i=0;i<count;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}
	pos=tmpStr.Find('|');
	tmpStr=tmpStr.Left(pos);
	pos=tmpStr.Find(' ');
	return tmpStr.Left(pos);
}