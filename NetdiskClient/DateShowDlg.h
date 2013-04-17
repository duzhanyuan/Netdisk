#pragma once
#include "resource.h"


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
	virtual BOOL OnInitDialog();
};
