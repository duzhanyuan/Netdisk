#pragma once
#include "Client.h"
// CUserLoginDlg dialog

class CUserLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserLoginDlg)

public:
	CUserLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserLoginDlg();

// Dialog Data
	enum { IDD = IDD_USERLONGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bAutoLogin;
	CString m_strName;
	CString m_strPasswd;
	Client m_Client;
	CNetdiskClientApp* m_ndApp;
private:
 
	virtual BOOL OnInitDialog();
	void OnTimer(UINT nIdEvent);
	CStatic m_StaticState;
	afx_msg void OnBnClickedBtnLogin();
 
};
