
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
	CString m_strServAddr;//�������ĵ�ַ
	int m_iServPort;//�������Ķ˿ں�
	CTreeCtrl m_tcShowDeprt;//��ʾ������ͼ
	CDatabase m_dbDepart;
	HTREEITEM m_tcRoot;
	CString m_strCompyName;
	CSetDeprtNameDlg* m_pAddNewDeprt;
	HTREEITEM m_hCurrent;//��ǰѡ�еĽڵ�
	int m_iServControl;
	MyIOCP m_MyIocp;
	int m_ID;
public:
	BOOL ShowDeprtDbInfo();
	HTREEITEM ReturnDepartment(CString DeprtName);
	void StartServ();//����������
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
