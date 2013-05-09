#pragma once
#include "FileIndex.h"
#include "CatalogIndex.h"
// CServIndex command target
typedef enum _SERV_SEND_TYPE
{
	EMPTYCATALOG=14,					//���ͷǿ�Ŀ¼��Ϣ
	NONEMPTYCATALOG						//���Ϳ�Ŀ¼��Ϣ
}SERV_SEND_TYPE;

class CServIndex : public CObject
{
public:
	CServIndex();
	virtual ~CServIndex();
public:
	CFile*	m_cfile;
	CString m_strRootPath ;
	CString m_strIndexPath;
	CString	m_strRecyclePath;
	CString m_strRecycleIndexPath;
	CCatalogIndex* m_catalogIndexHead;
	CFileIndex*	   m_fileIndexHead;

public:
	BOOL	UpdateIndex(CString rootPath,CString indexPath,CString rootName);					//�����������û�Ŀ¼����
	BOOL	InsertIndexItem();				//����������¼
	BOOL	DeleteIndexItem();				//ɾ��������Ŀ
	BOOL	EditIndex();					//�༭������Ŀ
	BOOL	DeleteIndex(CString rootName);					//ɾ���û�Ŀ¼����
	void	UpdateIndexList(CString rootPath,CCatalogIndex* catalogIndexHead);					//�����û�Ŀ¼��������
	CString	GetIndexInfo(CString indexPath,CString &rootName);					//��ȡ������Ϣ
public:
	int		getSubCatalogCount(CCatalogIndex* catalogHead);						//��ȡ��Ŀ¼��
	int		getSubFileCount(CFileIndex* fileHead);									//��ȡ���ļ���
	int		getNextCatalogCount(CCatalogIndex* catalogHead);					//��ȡͬ��Ŀ¼��
	//void	writeSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//д����Ŀ¼��Ϣ
	void	writeNextCatalogInfo(CCatalogIndex* catalogHead,CStdioFile* pStdFile);			//д��ͬ��Ŀ¼��Ϣ
	void	writeSubFileInfo(CFileIndex* fileHead,CStdioFile* pStdFile);					//д�����ļ���Ϣ
	void	readSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//��ȡ��Ŀ¼��Ϣ
	void	readNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//��ȡͬ��Ŀ¼��Ϣ
	void	readSubFileInfo(CFileIndex* fileHead,CFile* pcFile);					//��ȡ���ļ���Ϣ
	CString	getCatalogInfo(CString rootPath,CString indexPath,CString floderPath);
	CString	GetOneIndexInfo(CString srcStr,int &iPos);
	CString	GetCatalogName(CString srcStr);
	CString	GetPath(CString srcStr);
	CString GetTime(CString srcStr);
	CString	FindFileIndex(CString indexPath,CString rootName,CString findStr);
	CString FindFileByTime(CString CurrentPath,CString strStartTime,CString StrEndTime);
	//void	testWriteSub(CCatalogIndex* catalogHead,CStdioFile* pStdFile);
	//void	testWriteNext(CCatalogIndex* catalogHead,CStdioFile* pStdFile);
	//void	testWriteFile(CFileIndex* fileHead,CStdioFile* pStdFile);

public:
	void	BuildSubCatalogAndFileList(CString rootPath,CCatalogIndex* rootIndex);
	void	RecurBuildCatalogList(CString path,CCatalogIndex* rootIndex);
	BOOL	IsDirecEmpty(CString path);
};


