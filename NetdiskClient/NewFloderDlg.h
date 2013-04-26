#pragma once
class CNetdiskClientDlg;

// CNewFloderDlg dialog

class CNewFloderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewFloderDlg)

public:
	CNewFloderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewFloderDlg();

// Dialog Data
	enum { IDD = IDD_NEWFLODERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNewFloderName;
	CNetdiskClientDlg* m_pMainDlg;

	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
