#pragma once
#include "FileIndex.h"
// CCatalogIndex command target

class CCatalogIndex : public CObject
{
public:
	DECLARE_SERIAL(CCatalogIndex)
	CCatalogIndex();
	virtual ~CCatalogIndex();
	void Serialize( CArchive& archive );

public:
	CString		m_strName;			//目录索引名称
	CString		m_strPath;			//目录索引路径
	CString		m_strEditTime;		//目录更改时间
	BOOL		m_bIsShare;			//是否共享
	CString		m_strSharePath;		//共享路径
	CFileIndex* m_subFile;			//子文件索引
	CCatalogIndex* m_NextCatalog;	//同级目录索引
	CCatalogIndex* m_subCatalog;	//子目录索引
};


