#pragma once

// CRecycleFileHistriyPath command target

class CRecycleFileHistriyPath : public CObject
{
public:
	CString		m_strCurrentRootPath;
	CString		m_strHistroyRootPath;
	CString		m_strIndexPath;
	CString		m_strFileName;
	CArray<CString,CString&>*		m_pFilePathArr;
public:
	CRecycleFileHistriyPath();
	CRecycleFileHistriyPath(CString userName);
	virtual ~CRecycleFileHistriyPath();
	
	BOOL DeleteIndex(CString curPath,CString &hisPath);
	BOOL AddIndex(CString curPath,CString hisIndex);
	void CleanIndexFile();
};


