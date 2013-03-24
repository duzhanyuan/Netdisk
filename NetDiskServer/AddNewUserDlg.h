#pragma once
//class CNetDiskServerDlg;

// CAddNewUserDlg dialog

class CAddNewUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddNewUserDlg)

public:
	CAddNewUserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddNewUserDlg();

// Dialog Data
	enum { IDD = IDD_ADDUSERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	CString m_strUserServAddr;
	int m_iUserServPort;
	CString m_strUserFileSavePath;
	CString m_strUserPasswd;
	afx_msg void OnBtnShowpath();
	afx_msg void OnBnClickedOk();
	CString m_strComfPwd;

	//CNetDiskServerDlg* m_pNetdiskDlg;
	virtual BOOL OnInitDialog();
};
