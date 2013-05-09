#pragma once
class CNetdiskClientDlg;
class CSearchFile;
// CRecycleDlg dialog

class CRecycleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecycleDlg)

public:
	CRecycleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecycleDlg();

// Dialog Data
	enum { IDD = IDD_RECYCLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSearchFile;
	CListCtrl m_lcRecycleList;
	CNetdiskClientDlg* m_pMainDlg;
	CString	 m_strIndexInfo;
	CString  m_strCurrentPath;
	CSearchFile* m_pSearchFile;
	virtual BOOL OnInitDialog();
	void InitRecycleList();
	afx_msg void OnNMDblclkListRecyclelist(NMHDR *pNMHDR, LRESULT *pResult);
	CMFCButton m_btnRestore;
	afx_msg void OnBnClickedMfcbtnRestore();
	afx_msg void OnBnClickedMfcbtnDelete();
	afx_msg void OnBnClickedMfcbtnCleanup();
	afx_msg void OnBnClickedMfcbtnRefresh();
	afx_msg void OnBnClickedMfcbtnSearch();
};
