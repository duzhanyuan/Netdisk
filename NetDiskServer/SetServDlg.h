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
	CString m_strServAddr;//��������ַ
	int m_iServPort;//�������˿ں�
};
