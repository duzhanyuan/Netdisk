#pragma once
class CNetdiskClientDlg;

// CDownLoadDlg dialog

class CDownLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownLoadDlg)

public:
	CDownLoadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDownLoadDlg();

// Dialog Data
	enum { IDD = IDD_DOWNLOADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CTreeCtrl m_tcDownLoadDes;
	CProgressCtrl m_progDownload;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	void	RecvDownLoadFile(CString path);		//下载文件
	void	RecvDownLoadCat(CString catalogName);		//下载目录
//	CString m_str;
	CString m_strSavePath;
	CNetdiskClientDlg* m_pMainDlg;
	u_long	m_ulFileSize;
	u_long	m_ulCurTotal;
};
