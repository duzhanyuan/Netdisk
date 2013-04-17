// FileUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "FileUploadDlg.h"
#include "afxdialogex.h"


// CFileUploadDlg dialog

IMPLEMENT_DYNAMIC(CFileUploadDlg, CDialogEx)

CFileUploadDlg::CFileUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileUploadDlg::IDD, pParent)
{

}

CFileUploadDlg::~CFileUploadDlg()
{
}

void CFileUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UPLOADFILELIST, m_lcUploadFile);
}


BEGIN_MESSAGE_MAP(CFileUploadDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_CLEANLIST, &CFileUploadDlg::OnStnClickedStaticCleanlist)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_ADDFILE, &CFileUploadDlg::OnBnClickedBtnAddfile)
	ON_BN_CLICKED(IDC_BTN_UPLOAD, &CFileUploadDlg::OnBnClickedBtnUpload)
END_MESSAGE_MAP()


// CFileUploadDlg message handlers


void CFileUploadDlg::OnStnClickedStaticCleanlist()
{
	// TODO: Add your control notification handler code here
	
}


void CFileUploadDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnMouseMove(nFlags, point);
}

void CFileUploadDlg::InitUploadFileList()
{
	LONG lStyle;
	lStyle = GetWindowLong(m_lcUploadFile.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_lcUploadFile.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_lcUploadFile.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_lcUploadFile.SetExtendedStyle(dwStyle); //设置扩展风格
	m_lcUploadFile.InsertColumn(0,_T("文件名"),LVCFMT_CENTER,300);
	m_lcUploadFile.InsertColumn(1,_T("大小"),LVCFMT_CENTER,150);
	m_lcUploadFile.InsertColumn(2,_T("状态"),LVCFMT_CENTER,150);

}

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitUploadFileList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//添加文件到列表中准备上传
void CFileUploadDlg::OnBnClickedBtnAddfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog uplaodFileDlg(TRUE);
	CFile readFile;
	CString strFileName,strFileSize;

	if(uplaodFileDlg.DoModal()==IDOK)
	{
		strFileName=uplaodFileDlg.GetFileName();	
		if(readFile.Open(uplaodFileDlg.GetPathName(),CFile::modeRead))
		{
			ULONGLONG f_size=readFile.GetLength();
			if(f_size<1024)
				strFileSize.Format(_T("%.1lf B"),(double)f_size);
			else if(f_size>=1024&&f_size<(1024*1024))
				strFileSize.Format(_T("%.1lf KB"),(double)f_size/1024.0f);
			else
				strFileSize.Format(_T("%.1lf MB"),(double)f_size/(1024*1024.0f));

			readFile.Close();
		}
	}
	if(m_lcUploadFile.GetItemCount()>100)
	{
		AfxMessageBox(_T("文件添加过多，无法继续添加文件！"));
		return;
	}

	if(strFileName!=_T(""))
	{
		if(!IfFileAdded(strFileName))
		{
			int nRow=m_lcUploadFile.InsertItem(0,strFileName);
			m_lcUploadFile.SetItemText(nRow,1,strFileSize);
			m_lcUploadFile.SetItemText(nRow,2,_T("准备上传"));
		}
		else
			AfxMessageBox(_T("文件已经添加！"));
	}

}
bool CFileUploadDlg::IfFileAdded(CString fileName)
{
	int count=m_lcUploadFile.GetItemCount();
	CString name;
	for(int i=0;i<count;i++)
	{
		name=m_lcUploadFile.GetItemText(i,0);
		if(name==fileName)
			return true;
	}
	return false;
}

//将所选择的文件上传
void CFileUploadDlg::OnBnClickedBtnUpload()
{
	// TODO: Add your control notification handler code here

}
