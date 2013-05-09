#pragma once
class CNetdiskClientDlg;
// CHistroyVersionDlg dialog

class CHistroyVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistroyVersionDlg)

public:
	CHistroyVersionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistroyVersionDlg();

// Dialog Data
	enum { IDD = IDD_HISTROYVERSIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcHistroyVersionFile;
	CNetdiskClientDlg* m_pMainDlg;
	CString m_strIndexInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRestore();
	void InitHisVerFileList();
	void ShowHisVerFileList(CString fileName,CString strInfo);
	CString getFileListItemSize(CString indexInfo);
	CString getFileListItemEditTime(CString indexInfo);
	CString	getFileListItemVersionNo(CString indexInfo);
	CString getFileListItemMd5Code(CString indexInfo);
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnNMClickListHistroyfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnClean();
};
