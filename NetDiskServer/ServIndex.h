#pragma once
#include "FileIndex.h"
#include "CatalogIndex.h"
// CServIndex command target
typedef enum _SERV_SEND_TYPE
{
	EMPTYCATALOG=14,					//发送非空目录信息
	NONEMPTYCATALOG						//发送空目录信息
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
	BOOL	UpdateIndex(CString rootPath,CString indexPath,CString rootName);					//创建、更新用户目录索引
	BOOL	InsertIndexItem();				//插入索引条录
	BOOL	DeleteIndexItem();				//删除索引条目
	BOOL	EditIndex();					//编辑索引条目
	BOOL	DeleteIndex(CString rootName);					//删除用户目录索引
	void	UpdateIndexList(CString rootPath,CCatalogIndex* catalogIndexHead);					//更新用户目录索引链表
	CString	GetIndexInfo(CString indexPath,CString &rootName);					//获取索引信息
public:
	int		getSubCatalogCount(CCatalogIndex* catalogHead);						//获取子目录数
	int		getSubFileCount(CFileIndex* fileHead);									//获取子文件数
	int		getNextCatalogCount(CCatalogIndex* catalogHead);					//获取同级目录数
	//void	writeSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//写入子目录信息
	void	writeNextCatalogInfo(CCatalogIndex* catalogHead,CStdioFile* pStdFile);			//写入同级目录信息
	void	writeSubFileInfo(CFileIndex* fileHead,CStdioFile* pStdFile);					//写入子文件信息
	void	readSubCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//读取子目录信息
	void	readNextCatalogInfo(CCatalogIndex* catalogHead,CFile* pcFile);			//读取同级目录信息
	void	readSubFileInfo(CFileIndex* fileHead,CFile* pcFile);					//读取子文件信息
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


