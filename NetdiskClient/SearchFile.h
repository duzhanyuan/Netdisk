#pragma once
class CNetdiskClientDlg;
// CSearchFile command target

class CSearchFile : public CObject
{
public:
	CNetdiskClientDlg* m_pMainDlg;
	CString			m_strCurrentPath;
	CString			m_strIndexInfo;
	CMap<CString,LPCTSTR,CString,LPCTSTR>* m_pMapFilePath;
public:
	CSearchFile();
	virtual ~CSearchFile();
	void SendSearchStr(CString searStr);
	void ShowFindFileList(CListCtrl* pListCtrl,CString indexInfo);
	CString getFindFilePath(CString indexInfo);
};


