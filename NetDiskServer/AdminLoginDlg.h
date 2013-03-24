#pragma once
#include "resource.h"

//#ifndef __ADMINLOGINDLG_H__
//#define __ADMINLOGINDLG_H__
class CAdminLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminLoginDlg)

public:

	CAdminLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdminLoginDlg();

// Dialog Data
	enum { IDD = IDD_ADDMINLOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	CString m_strAdminName;
	CString m_strAdminPasswd;
	CString m_strCompanyName;
	CString m_strCmpAdminName;
	CString m_strCmpAdminPasswd;

	BOOL m_bIsDefaultSet;
	CDatabase m_dbAdmin;
	afx_msg void OnClickedCheckDefaultset();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
//#endif