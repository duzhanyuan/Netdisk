
// NetdiskClientDlg.h : header file
//

#pragma once
#include "Client.h"
#include "CatalogShowDlg.h"
#include "DateShowDlg.h"
#include "resource.h"

//历史路径结构体
typedef struct _HistroyPath
{
	CString path;
	struct _HistroyPath* next;
}HistroyPath;

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
	CMFCMenuButton	m_mbUpload;
	Client			m_Client;
	CCatalogShowDlg m_catShowDlg;
	CDateShowDlg	m_dateShowDlg;
	CString			m_strUserLoginName;
	CString			m_strIndexInfo;
	HistroyPath*	m_HistroyPathArr;
	HistroyPath*	m_pHisPathHead;
	HistroyPath*	m_pHisPathTail;
	//int				m_iHistroyFlag;
	//CString			m_strCurrentPath;
public:
	void			InitCatalogAndDateShow();

	void			InitFileShow();
	void			ShowFileList(CString indexInfo);
	CString			getFileListItemName(CString srcStr);
	CString			getFileListItemSize(CString srcStr);
	CString			getFileListItemEditTime(CString srcStr);
	CString			getOneIndexInfo(CString srcStr,int &iPos);

	afx_msg void OnBnClickedBtnNewfloder();
	afx_msg void OnBnClickedBtnDelete();
	CTabCtrl m_tcCatalogShow;
	afx_msg void OnTcnSelchangeTabCatalogue(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_lcFileShow;
	afx_msg void OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfcbtnLastcatalog();
	afx_msg void OnBnClickedMfcbtnNextcatalog();
	CString m_strCurrentPath;
	CMFCButton m_btnUpload;
	afx_msg void OnBnClickedBtnUploadfile();
};
