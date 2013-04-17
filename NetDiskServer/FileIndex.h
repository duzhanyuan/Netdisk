#pragma once

// CFileIndex command target

class CFileIndex : public CObject
{
public:
	DECLARE_SERIAL(CFileIndex)
	CFileIndex();
	virtual ~CFileIndex();
	void Serialize( CArchive& archive );
public:
	CString		m_strName;		//文件名称
	CString		m_strPath;		//文件路径
	CString		m_strEditTime;	//文件更改时间
	CString		m_strParentCatalogName;//父目录名称	
	CString		m_strFileSize;	//文件大小
	BOOL		m_bIsShare;		//是否共享
	CString		m_strSharePath;	//共享路径
	CFileIndex* m_Next;	//同级文件索引
};


