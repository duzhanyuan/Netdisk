
// NetdiskClientDlg.h : header file
//

#pragma once
#include "Client.h"

// CNetdiskClientDlg dialog
class CNetdiskClientDlg : public CDialogEx
{
// Construction
public:
	CNetdiskClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NETDISKCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCMenuButton m_mbUpload;
	Client m_Client;
	afx_msg void OnBnClickedBtnNewfloder();
	afx_msg void OnBnClickedBtnDelete();
};
