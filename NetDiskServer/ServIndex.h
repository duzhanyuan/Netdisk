#pragma once
#include "FileIndex.h"
#include "CatalogIndex.h"
// CServIndex command target
//////////////////////////////////////////////////////////////////////////
//�û��ļ��м������ļ��е������ļ��ṹ
//�ļ������ṹ
//typedef struct _FileIndex
//{
//	CString name;		//�ļ�����
//	CString path;		//�ļ�·��
//	CString editTime;	//�ļ�����ʱ��
//	BOOL isShare;		//�Ƿ���
//	CString sharePath;	//����·��
//	FileIndex* next;	//ͬ���ļ�����
//}FileIndex;
////Ŀ¼�����ṹ
// typedef struct _CatalogIndex
//{
//	CString name;			//Ŀ¼��������
//	CString path;			//Ŀ¼����·��
//	CString editTime;		//Ŀ¼����ʱ��
//	BOOL isShare;			//�Ƿ���
//	CString sharePath;		//����·��
//	FileIndex* nextFile;	//���ļ�����
//	CatalogIndex* nextCatalog;//��Ŀ¼����
//}CatalogIndex;
//


class CServIndex : public CObject
{
public:
	CServIndex();
	virtual ~CServIndex();
public:
	CFile*	m_cfile;
	CString m_strRootPath ;
	CString m_strIndexPath;
	CCatalogIndex* m_catalogIndexHead;
	CFileIndex*	   m_fileIndexHead;

public:
	BOOL	CreateIndex(CString rootName);					//�����û�Ŀ¼����
	BOOL	InsertIndexItem();				//����������¼
	BOOL	DeleteIndexItem();				//ɾ��������Ŀ
	BOOL	EditIndex();					//�༭������Ŀ
	BOOL	DeleteIndex(CString rootName);					//ɾ���û�Ŀ¼����
	void	UpdateIndex(CString rootPath,CCatalogIndex* catalogIndexHead);					//�����û�Ŀ¼����

public:
	int		getSubCatalogCount(CCatalogIndex* catalogHead);						//��ȡ��Ŀ¼��
	int		getSubFileCount(CFileIndex* fileHead);									//��ȡ���ļ���
	int		getNextCatalogCount(CCatalogIndex* catalogHead);					//��ȡͬ��Ŀ¼��
	void	writeSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//д����Ŀ¼��Ϣ
	void	writeNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//д��ͬ��Ŀ¼��Ϣ
	void	writeSubFileInfo(CFileIndex* fileIndex,CFile* pcFile);					//д�����ļ���Ϣ
	void	readSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//��ȡ��Ŀ¼��Ϣ
	void	readNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//��ȡͬ��Ŀ¼��Ϣ
	void	readSubFileInfo(CFileIndex* fileHead,CFile* pcFile);					//��ȡ���ļ���Ϣ

	void	testWriteSub(CCatalogIndex* catalogHead,CStdioFile* pStdFile);
	void	testWriteNext(CCatalogIndex* catalogHead,CStdioFile* pStdFile);
	void	testWriteFile(CFileIndex* fileHead,CStdioFile* pStdFile);

public:
	void	BuildSubCatalogAndFileList(CString rootPath,CCatalogIndex* rootIndex);
	void	RecurBuildCatalogList(CString path,CCatalogIndex* rootIndex);
	BOOL	IsDirecEmpty(CString path);
};


