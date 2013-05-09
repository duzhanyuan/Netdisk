#pragma once

// CHistroyVersionIndex command target
typedef struct _HISVERFILE
{
	CString fileName;
	CString fileSize;
	CString filePath;
	CString fileMd5Code;
}HISVERFILE;

class CHistroyVersionIndex : public CObject
{
public:
	CString		m_strHisVerRootIndexPath;
	CString		m_strHisVerRootPath;
	CString		m_strDiskRootPath;
public:
	CHistroyVersionIndex();
	virtual ~CHistroyVersionIndex();
	void UpdateIndex(CString userName,CString folderName);
	void AddIndex(CString userName,CString srcPath);
	void DelIndex(CString userName,CString strInfo);
	void ClearIndex(CString userName,CString strInfo);
	bool IfFloderExist(CString path);
	bool IfFileVerExist(CString userName,CString filePath);
	CString GetMd5Code();
	CString GetHisVerIndex(CString userName,CString fileName);
	int	 GetFileVersion(CString userName,CString folderName);
	CString GetCurrentMd5Code(CString filePath);
	void RestoreIndex(CString userName,CString strInfo);
};


