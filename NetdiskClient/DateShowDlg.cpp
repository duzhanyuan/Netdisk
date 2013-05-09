// DateShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "DateShowDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

// CDateShowDlg dialog

IMPLEMENT_DYNAMIC(CDateShowDlg, CDialog)

CDateShowDlg::CDateShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateShowDlg::IDD, pParent)
{
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();
}

CDateShowDlg::~CDateShowDlg()
{
}

void CDateShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHSHOW, m_monthShow);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_ENDDATE, m_dtcEndDate);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_STARTDATE, m_dtcStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDDATE, m_ctEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTDATE, m_ctStartDate);
}


BEGIN_MESSAGE_MAP(CDateShowDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHSHOW, &CDateShowDlg::OnMcnSelchangeMonthshow)
	ON_BN_CLICKED(IDC_BTN_DATEFIND, &CDateShowDlg::OnBnClickedBtnDatefind)
END_MESSAGE_MAP()


// CDateShowDlg message handlers


void CDateShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(GetDlgItem(IDC_MONTHSHOW))
	{
		CRect rect;
		GetDlgItem(IDC_MONTHSHOW)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		rect.left=rect.left*cx/m_rect.Width();
		rect.right=rect.right*cx/m_rect.Width();
		rect.bottom=rect.bottom*cy/m_rect.Height();
		rect.top=rect.top*cy/m_rect.Height();
		GetDlgItem(IDC_MONTHSHOW)->MoveWindow(&rect);
	}
	GetClientRect(&m_rect);

	// TODO: Add your message handler code here
}


BOOL CDateShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetClientRect(&m_rect);
	m_ctStartDate=CTime::GetCurrentTime();
	m_ctEndDate=CTime::GetCurrentTime();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDateShowDlg::OnMcnSelchangeMonthshow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	SYSTEMTIME sysTime=pSelChange->stSelStart;
	CTime ctTime;
	ctTime=CTime((int)sysTime.wYear,(int)sysTime.wMonth,(int)sysTime.wDay,0,0,0,-1);
	CString selStr;
	selStr=ctTime.Format(_T("%Y/%m/%d"));
	_tprintf_s(_T("%s\n"), (LPCTSTR) selStr);
	CString CurrentPath=m_pMainDlg->m_strCurrentPath;
	//发送通过日期信息获取文件的信息
	if(m_pMainDlg->m_Client.GetFileByDataShow(CurrentPath+_T(":")+selStr+_T(":")+selStr))
	{
		m_pMainDlg->m_bFindFileState=TRUE;
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		CSearchFile* m_pSearchFile=new CSearchFile();
		m_pSearchFile->ShowFindFileList(&m_pMainDlg->m_lcFileShow,m_strIndexInfo);
	}
	*pResult = 0;
}


void CDateShowDlg::OnBnClickedBtnDatefind()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString startDate,endDate;
	startDate=m_ctStartDate.Format(_T("%Y/%m/%d"));
	endDate=m_ctEndDate.Format(_T("%Y/%m/%d"));
	CString CurrentPath=m_pMainDlg->m_strCurrentPath;
	//发送通过日期信息获取文件的信息
	if(m_pMainDlg->m_Client.GetFileByDataShow(CurrentPath+_T(":")+startDate+_T(":")+endDate))
	{
		m_pMainDlg->m_bFindFileState=TRUE;
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		CSearchFile* m_pSearchFile=new CSearchFile();
		m_pSearchFile->ShowFindFileList(&m_pMainDlg->m_lcFileShow,m_strIndexInfo);
	}
}
