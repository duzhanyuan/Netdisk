#pragma once
#include "resource.h"
class CNetdiskClientDlg;

// CDateShowDlg dialog

class CDateShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CDateShowDlg)

public:
	CDateShowDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDateShowDlg();

// Dialog Data
	enum { IDD = IDD_DATESHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMonthCalCtrl m_monthShow;
	CRect m_rect;
	CNetdiskClientDlg* m_pMainDlg;
	CString m_strIndexInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnMcnSelchangeMonthshow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDatefind();
//	CDateTimeCtrl m_dtcEndDate;
//	CDateTimeCtrl m_dtcStartDate;
	CTime m_ctEndDate;
	CTime m_ctStartDate;
};
