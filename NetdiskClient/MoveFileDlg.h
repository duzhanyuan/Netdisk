#pragma once
class CNetdiskClientDlg;

// CMoveFileDlg dialog

class CMoveFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMoveFileDlg)

public:
	CMoveFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMoveFileDlg();

// Dialog Data
	enum { IDD = IDD_MOVEFILEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tcMoveFile;
	CNetdiskClientDlg*	m_pMainDlg;
	HTREEITEM	m_MyDisk;
	HTREEITEM	m_ShareDisk;
	CString		m_strMovePath;
	CString		m_strShareFloder;
	virtual BOOL OnInitDialog();
	void ShowCatalog(HTREEITEM rootNode,CString indexInfo);
	BOOL DeleteSubRoot(HTREEITEM hItem);
	CString GetOneIndexInfo(CString srcStr,int &iPos);
	CString GetCatalogName(CString srcStr);
	CString getRootCatalogName(CString srcStr);
	afx_msg void OnBnClickedBtnMoveok();
	afx_msg void OnBnClickedBtnMovecancel();
	afx_msg void OnTvnSelchangedTreeMovefile(NMHDR *pNMHDR, LRESULT *pResult);
};
