
// NetDiskServerDlg.h : header file
//

#pragma once
#include"MyIOCP.h"

class CSetDeprtNameDlg;

// CNetDiskServerDlg dialog
class CNetDiskServerDlg : public CDialogEx
{
// Construction
public:
	CNetDiskServerDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	enum { IDD = IDD_NETDISKSERVER_DIALOG };
	//CNetDiskServerDlg(CString strCompyName);

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
	CString m_strServAddr;//服务器的地址
	int m_iServPort;//服务器的端口号
	CTreeCtrl m_tcShowDeprt;//显示部门视图
	CDatabase m_dbDepart;
	HTREEITEM m_tcRoot;
	CString m_strCompyName;
	CSetDeprtNameDlg* m_pAddNewDeprt;
	HTREEITEM m_hCurrent;//当前选中的节点
	int m_iServControl;
	MyIOCP m_MyIocp;
	int m_ID;
public:
	BOOL ShowDeprtDbInfo();
	HTREEITEM ReturnDepartment(CString DeprtName);
	void StartServ();//启动服务器
	void UpdateServData();
public:
	afx_msg void OnBnClickedMfcbtnSetserv();
	afx_msg void OnBnClickedMfcbtnQuit();

	afx_msg void AddDepartment();
	afx_msg void OnBtnAdduser();
	afx_msg void OnSelchangedTreeShowdeparment(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfcbtnServcontrol();
	afx_msg void OnBnClickedMfcbtnDeldeprt();
	afx_msg void OnBnClickedMfcbtnDeluser();

};
