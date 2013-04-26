#pragma once
#include "Client.h"
#include "NetdiskClientDlg.h"
#include "HyperLink.h"

// CFileUploadDlg dialog
typedef struct _THREADWORKER
{
	SOCKET  cSocket;
	CFileUploadDlg* pFileULDlg;
	CListCtrl*	pListCtrl;
	Client* pClient;
	CNetdiskClientDlg* pMainDlg;
}THREADWORKER;

class CFileUploadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileUploadDlg)

public:
	CFileUploadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileUploadDlg();

// Dialog Data
	enum { IDD = IDD_UPLOADFILEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CNetdiskClientDlg* m_pMainDlg;
	Client			m_Client;
	CMap<CString, LPCTSTR, CString, LPCTSTR>*	  m_pFileUploadMap;
	CHyperLink		m_pClearList;
	CButton m_btnUpload;
	CProgressCtrl m_UploadProg;
	CListCtrl m_lcUploadFile;
	u_long	m_ulFileSize;
	u_long	m_ulCurTotal;
	THREADWORKER  m_pThreadWorker;
	int		m_iAddFileCount;
public:	
	void InitUploadFileList();
	bool IfFileAdded(CString fileName);
	afx_msg void OnStnClickedStaticCleanlist();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	void	OnTimer(UINT nIDEvent);
	void	ResetCurFileDL(void);
	static DWORD WINAPI UploadThread(LPVOID lpParam);
	afx_msg void OnBnClickedBtnAddfile();
	afx_msg void OnBnClickedBtnUpload();

};
