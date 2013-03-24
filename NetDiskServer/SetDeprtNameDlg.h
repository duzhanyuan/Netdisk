#pragma once
class CNetDiskServerDlg;

// CSetDeprtNameDlg dialog

class CSetDeprtNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetDeprtNameDlg)

public:
	CSetDeprtNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetDeprtNameDlg();

// Dialog Data
	enum { IDD = IDD_SETDEPRTNAMEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDeprtName;
	CNetDiskServerDlg* m_pNdServDlg;
	afx_msg void OnBnClickedOk();
};
