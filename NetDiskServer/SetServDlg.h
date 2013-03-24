#pragma once


// CSetServDlg dialog

class CSetServDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetServDlg)

public:
	CSetServDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetServDlg();

// Dialog Data
	enum { IDD = IDD_SETSERVDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CString m_strServAddr;//服务器地址
	int m_iServPort;//服务器端口号
};
