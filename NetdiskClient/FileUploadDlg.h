#pragma once
#include "Client.h"
#include "NetdiskClientDlg.h"
#include "HyperLink.h"

// CFileUploadDlg dialog

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

public:	
	void InitUploadFileList();
	bool IfFileAdded(CString fileName);
	afx_msg void OnStnClickedStaticCleanlist();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAddfile();
	afx_msg void OnBnClickedBtnUpload();

};
