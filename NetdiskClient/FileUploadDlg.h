#pragma once


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
	void InitUploadFileList();
	bool IfFileAdded(CString fileName);
	afx_msg void OnStnClickedStaticCleanlist();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CListCtrl m_lcUploadFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAddfile();
	afx_msg void OnBnClickedBtnUpload();
};
