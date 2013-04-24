
// NetdiskClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "NetdiskClientDlg.h"
#include "afxdialogex.h"
#include "FileUploadDlg.h"

//#include "CatalogShowOpr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetdiskClientDlg dialog




CNetdiskClientDlg::CNetdiskClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetdiskClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetdiskClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CATALOGUE, m_tcCatalogShow);
	DDX_Control(pDX, IDC_LIST_FILE, m_lcFileShow);
	DDX_Text(pDX, IDC_EDIT_CURRENTPATH, m_strCurrentPath);
	DDX_Control(pDX, IDC_BTN_UPLOADFILE, m_btnUpload);
}

BEGIN_MESSAGE_MAP(CNetdiskClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_NEWFLODER, &CNetdiskClientDlg::OnBnClickedBtnNewfloder)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CNetdiskClientDlg::OnBnClickedBtnDelete)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CATALOGUE, &CNetdiskClientDlg::OnTcnSelchangeTabCatalogue)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, &CNetdiskClientDlg::OnNMDblclkListFile)
	ON_BN_CLICKED(IDC_MFCBTN_LASTCATALOG, &CNetdiskClientDlg::OnBnClickedMfcbtnLastcatalog)
	ON_BN_CLICKED(IDC_MFCBTN_NEXTCATALOG, &CNetdiskClientDlg::OnBnClickedMfcbtnNextcatalog)
	ON_BN_CLICKED(IDC_BTN_UPLOADFILE, &CNetdiskClientDlg::OnBnClickedBtnUploadfile)
END_MESSAGE_MAP()


// CNetdiskClientDlg message handlers

BOOL CNetdiskClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here
	//上传文件和上传文件夹
	//CMenu * popupMenu = new CMenu();
	//popupMenu->CreatePopupMenu();
	//popupMenu->AppendMenuW(MF_STRING, ID_UPLOADFILE, _T("上传文件"));
	//popupMenu->AppendMenuW(MF_STRING, ID_UPLOADCAT, _T("上传文件夹"));
	//m_mbUpload.m_hMenu = popupMenu->m_hMenu;



	//m_Client.Start();
	//连接服务器获取客户端更新信息
	m_strUserLoginName=((CNetdiskClientApp*)AfxGetApp())->m_loginName;
	m_strCurrentPath=m_strUserLoginName;

	//历史路径数组初始化
	m_HistroyPathArr=new HistroyPath();
	m_HistroyPathArr->path=m_strCurrentPath;
	m_HistroyPathArr->next=NULL;
	m_pHisPathHead=m_HistroyPathArr;
	m_pHisPathTail=m_HistroyPathArr;

	//将登录验证成功之后的连接socket赋给主程序
	m_Client.m_Client.sock=((CNetdiskClientApp*)AfxGetApp())->m_TmpClient;
	
	m_pCatShowDlg=new CCatalogShowDlg();
	m_pDateShowDlg=new CDateShowDlg();
	//按目录显示以及按日期显示初始化
	InitCatalogAndDateShow();

	//文件显示初始化
	InitFileShow();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetdiskClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetdiskClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetdiskClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//新建文件夹操作
void CNetdiskClientDlg::OnBnClickedBtnNewfloder()
{
	DWORD dwStyle = m_lcFileShow.GetExtendedStyle();
	dwStyle |= LVS_EDITLABELS;//可编辑
	m_lcFileShow.SetExtendedStyle(dwStyle); //设置扩展风格
}

//删除操作
void CNetdiskClientDlg::OnBnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	//m_Client.SendMsgToServ(_T("删除"));

}


//响应按目录查看或者是按日期查看
void CNetdiskClientDlg::OnTcnSelchangeTabCatalogue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	switch(m_tcCatalogShow.GetCurSel())
	{
	case 0:
		m_pCatShowDlg->ShowWindow(SW_SHOW);
		m_pDateShowDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pCatShowDlg->ShowWindow(SW_HIDE);
		m_pDateShowDlg->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
//初始化按目录和按日期显示
void CNetdiskClientDlg::InitCatalogAndDateShow()
{
	m_tcCatalogShow.InsertItem(0,_T("按目录查看"));
	m_tcCatalogShow.InsertItem(1,_T("按日期查看"));

	m_pCatShowDlg->Create(IDD_CATALOGSHOW,GetDlgItem(IDC_TAB_CATALOGUE));
	m_pDateShowDlg->Create(IDD_DATESHOW,GetDlgItem(IDC_TAB_CATALOGUE));

	CRect rc;
	m_tcCatalogShow.GetClientRect(&rc);
	rc.top += 21;
	rc.bottom -= 1;
	rc.left -= 1;
	rc.right += 1;
	m_pCatShowDlg->MoveWindow(&rc);
	m_pDateShowDlg->MoveWindow(&rc);
	m_pCatShowDlg->ShowWindow(SW_SHOW);
	m_pDateShowDlg->ShowWindow(SW_HIDE);


	if(m_Client.UpdateClientCatalog(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		m_pCatShowDlg->UpdateCatalogShow(m_strIndexInfo);

	}
	else
	{
		AfxMessageBox(_T("刷新客户端信息失败！"));
	}
}

//初始化文件显示
void CNetdiskClientDlg::InitFileShow()
{
	LONG lStyle;
	lStyle = GetWindowLong(m_lcFileShow.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_lcFileShow.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_lcFileShow.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮
	dwStyle |= LVS_EX_GRIDLINES;//网格线
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_lcFileShow.SetExtendedStyle(dwStyle); //设置扩展风格

	m_lcFileShow.InsertColumn(0,_T("文件名"),LVCFMT_CENTER,300);
	m_lcFileShow.InsertColumn(1,_T("大小"),LVCFMT_CENTER,150);
	m_lcFileShow.InsertColumn(2,_T("更新时间"),LVCFMT_CENTER,200);
	//int nRow = m_lcFileShow.InsertItem(0, _T("11"));//插入行
	//m_lcFileShow.SetItemText(nRow, 2, _T("jacky"));//设置数据

	//初始化文件列表显示
	if(m_Client.GetCatalogInfo(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowFileList(m_strIndexInfo);
	}
}

void CNetdiskClientDlg::ShowFileList(CString indexInfo)
{
	//UpdateData(FALSE);
	m_lcFileShow.DeleteAllItems();

	CArray<CString,CString&> *oneIndexInfoArr=new CArray<CString,CString&>();
	CString strName,strTime,strSize,oneIndexInfo,tmpStr;
	int pos;
	
	pos=indexInfo.Find('+');
	tmpStr=indexInfo.Left(pos);
	if(_ttoi(tmpStr)==EMPTYCATALOG)
		return;
	else
		tmpStr=indexInfo.Right(indexInfo.GetLength()-pos-1);

	while(tmpStr.GetLength()>0)
	{
		oneIndexInfo=getOneIndexInfo(tmpStr,pos);
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);

		oneIndexInfoArr->Add(oneIndexInfo);
	}

	for(int i=0;i<oneIndexInfoArr->GetSize();i++)
	{
		strName=getFileListItemName(oneIndexInfoArr->GetAt(i));
		strSize=getFileListItemSize(oneIndexInfoArr->GetAt(i));
		strTime=getFileListItemEditTime(oneIndexInfoArr->GetAt(i));

		int nRow=m_lcFileShow.InsertItem(0,strName);
		m_lcFileShow.SetItemText(nRow,1,strSize);
		m_lcFileShow.SetItemText(nRow,2,strTime);
	}
}

//获取一条索引记录
CString CNetdiskClientDlg::getOneIndexInfo(CString srcStr,int &iPos)
{
	CString tmpStr;
	if(srcStr==_T(""))
		return _T("");

	tmpStr=srcStr;
	int pos=tmpStr.Find(';');
	iPos=pos;

	return tmpStr.Left(pos);
}
//获取索引信息中的文件或者文件夹名称
CString CNetdiskClientDlg::getFileListItemName(CString srcStr)
{
	if(srcStr==_T(""))
		return _T("");
	CString tmpStr=srcStr;
	int pos,iLength=0;
	if(srcStr[0]==_T('D'))
		iLength=3;
	if(srcStr[0]==_T('F'))
		iLength=1;
	for(int i=0;i<iLength;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}
	pos=tmpStr.Find('|');
	return tmpStr.Left(pos);
}

//获取索引信息中的文件夹或者文件的更新时间
CString CNetdiskClientDlg::getFileListItemEditTime(CString srcStr)
{
	if(srcStr==_T(""))
		return _T("");

	CString tmpStr=srcStr;
	int pos,iLength=0;

	if(srcStr[0]==_T('D'))
		iLength=5;
	if(srcStr[0]==_T('F'))
		iLength=3;
	for(int i=0;i<iLength;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}

	pos=tmpStr.Find('|');
	return tmpStr.Left(pos);
}

//获取索引信息中的文件夹或者文件的大小，文件夹的大小为0
CString CNetdiskClientDlg::getFileListItemSize(CString srcStr)
{
	if(srcStr==_T("")||srcStr[0]==_T('D'))
		return _T("");

	CString tmpStr=srcStr;
	int pos;
	for(int i=0;i<4;i++)
	{
		pos=tmpStr.Find('|');
		tmpStr=tmpStr.Right(tmpStr.GetLength()-pos-1);
	}

	pos=tmpStr.Find('|');
	return tmpStr.Left(pos);
}

//文件列表双击事件处理
void CNetdiskClientDlg::OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int selItem=pNMItemActivate->iItem;
	if(selItem!=-1)
	{
		m_strCurrentPath=m_pCatShowDlg->m_strCurrentPath;
		if(m_lcFileShow.GetItemText(selItem,1)==_T(""))
		{
			m_strCurrentPath=m_strCurrentPath+_T("\\")+m_lcFileShow.GetItemText(selItem,0);
			UpdateData(FALSE);

			//将本次操作路径添加到历史路径数组中
			HistroyPath* tmpHistroyPath=new HistroyPath();
			tmpHistroyPath->path=m_strCurrentPath;
			tmpHistroyPath->next=m_HistroyPathArr->next;
			m_HistroyPathArr->next=tmpHistroyPath;
			m_pHisPathTail=tmpHistroyPath;

			if(m_Client.GetCatalogInfo(m_strCurrentPath))
			{
				m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
				ShowFileList(m_strIndexInfo);
				m_pCatShowDlg->SetSelectByFileListClick(m_strCurrentPath);
			}
		}
	}
	

	*pResult = 0;
}

//返回上一级目录
void CNetdiskClientDlg::OnBnClickedMfcbtnLastcatalog()
{
	// TODO: Add your control notification handler code here
	HistroyPath* tmpHisPathHead=m_HistroyPathArr;
	while(tmpHisPathHead->next!=m_pHisPathTail)
	{
		tmpHisPathHead=tmpHisPathHead->next;
	}

	m_strCurrentPath=tmpHisPathHead->path;
	UpdateData(FALSE);
	if(m_Client.GetCatalogInfo(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowFileList(m_strIndexInfo);
		m_pCatShowDlg->SetSelectByFileListClick(m_strCurrentPath);
	}
}

//
void CNetdiskClientDlg::OnBnClickedMfcbtnNextcatalog()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	if(m_Client.GetCatalogInfo(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowFileList(m_strIndexInfo);
		m_pCatShowDlg->SetSelectByFileListClick(m_strCurrentPath);
	}
}



void CNetdiskClientDlg::OnBnClickedBtnUploadfile()
{
	// TODO: Add your control notification handler code here
	CFileUploadDlg* m_pFileUpload=new CFileUploadDlg();
	if(m_pFileUpload->DoModal()==IDOK)
	{

	}
}
