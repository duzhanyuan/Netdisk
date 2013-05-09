
// NetdiskClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "NetdiskClientDlg.h"
#include "afxdialogex.h"
#include "FileUploadDlg.h"
#include "NewFloderDlg.h"
#include "DownLoadDlg.h"
#include "MoveFileDlg.h"
#include "RecycleDlg.h"
#include "HistroyVersionDlg.h"
#include "SearchFile.h"
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
	m_strShareFloder=_T("共享文件夹");
	//连接服务器获取客户端更新信息
	m_strUserLoginName=((CNetdiskClientApp*)AfxGetApp())->m_loginName;
	m_strCurrentPath=m_strUserLoginName;
	m_strSearchStr = _T("");
}

void CNetdiskClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CATALOGUE, m_tcCatalogShow);
	DDX_Control(pDX, IDC_LIST_FILE, m_lcFileShow);
	DDX_Text(pDX, IDC_EDIT_CURRENTPATH, m_strCurrentPath);
	DDX_Control(pDX, IDC_BTN_UPLOADFILE, m_btnUpload);
	DDX_Control(pDX, IDC_BTN_SHOWICON, m_btnShowIcon);
	DDX_Control(pDX, IDC_BTN_SHOWLIST, m_btnShowList);
	DDX_Control(pDX, IDC_BTN_RETURNBACK, m_btnReturnBack);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearchStr);
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
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CNetdiskClientDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, &CNetdiskClientDlg::OnBnClickedBtnDownload)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CNetdiskClientDlg::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_SHOWLIST, &CNetdiskClientDlg::OnBnClickedBtnShowlist)
	ON_BN_CLICKED(IDC_BTN_SHOWICON, &CNetdiskClientDlg::OnBnClickedBtnShowicon)
	ON_BN_CLICKED(IDC_BTN_RECYCLE, &CNetdiskClientDlg::OnBnClickedBtnRecycle)
	ON_BN_CLICKED(IDC_BTN_RETURNBACK, &CNetdiskClientDlg::OnBnClickedBtnReturnback)
	ON_BN_CLICKED(IDC_BTN_HISTORY, &CNetdiskClientDlg::OnBnClickedBtnHistory)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CNetdiskClientDlg::OnBnClickedBtnSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILE, &CNetdiskClientDlg::OnNMClickListFile)
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

	//添加文件夹图标识别
	HICON hIcon;
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,1);
	m_cSysIcon.m_ImageSmallList.Add(hIcon);
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,0);
	m_cSysIcon.m_ImageLargeList.Add(hIcon);
	m_cSysIcon.m_arExtName.Add(_T(""));

	//m_Client.Start();


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
	m_lcFileShow.InsertColumn(0,_T("文件名"),LVCFMT_CENTER,300);
	m_lcFileShow.InsertColumn(1,_T("大小"),LVCFMT_CENTER,150);
	m_lcFileShow.InsertColumn(2,_T("更新时间"),LVCFMT_CENTER,175);
	InitFileShow();

	//更新客户端文件信息显示
	OnBnClickedBtnRefresh();

	//初始化文件显示形式
	m_btnShowList.EnableWindow(FALSE);
	m_btnShowIcon.EnableWindow(TRUE);

	m_btnReturnBack.EnableWindow(FALSE);

	m_bFindFileState=FALSE;
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
	CNewFloderDlg* pNewFloder=new CNewFloderDlg();
	//将主函数指针赋给新建文件夹对话框
	pNewFloder->m_pMainDlg=this;
	if(pNewFloder->DoModal()== IDOK)
	{
		
	}
}

//删除操作
void CNetdiskClientDlg::OnBnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	m_bFindFileState=FALSE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

	int count=0;
	//遍历listview，查看是否有选中要删除的项
	for(int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
			count++;
	}
	if(count == 0)
	{
		AfxMessageBox(_T("请选择要删除的文件或者文件夹"));
		return;
	}
	//警告用户是否继续删除操作
	int nRes = AfxMessageBox(_T("确定将所选文件移到回收站？"),MB_YESNO|MB_ICONQUESTION);
	if(nRes == IDYES)
	{
		//遍历删除文件或者文件夹 
		for(int i=0;i<m_lcFileShow.GetItemCount();i++)
		{
			if(m_lcFileShow.GetCheck(i))
			{
				CString path=m_strCurrentPath+_T("\\")+m_lcFileShow.GetItemText(i,0);
				m_Client.DeleteClientFile(path);
			}

		}
		//更新客户端显示
		OnBnClickedBtnRefresh();
		////获取文件列表显示
		//if(m_Client.GetCatalogInfo(m_strCurrentPath))
		//{
		//	m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		//	ShowFileList(&m_lcFileShow,m_strIndexInfo);
		//}
		////更新我的网盘目录结构视图
		//if(m_Client.UpdateClientCatalog(m_strUserLoginName))
		//{
		//	m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		//	m_pCatShowDlg->UpdateCatalogShow(m_pCatShowDlg->m_MyDisk,m_strIndexInfo);

		//}

	}

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

}

//初始化文件显示
void CNetdiskClientDlg::InitFileShow()
{

	m_lcFileShow.ModifyStyle(0,LVS_SHOWSELALWAYS|LVS_REPORT|LVS_SINGLESEL|LVSIL_NORMAL);
	m_lcFileShow.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);

	m_lcFileShow.SetImageList(&m_cSysIcon.m_ImageSmallList,LVSIL_SMALL);


}

void CNetdiskClientDlg::ShowFileList(CListCtrl* plistCtrl,CString indexInfo)
{
	//UpdateData(FALSE);
	plistCtrl->DeleteAllItems();

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

	int nType; // 用于标示文件还是目录
	for(int i=0;i<oneIndexInfoArr->GetSize();i++)
	{
		strName=getFileListItemName(oneIndexInfoArr->GetAt(i));
		strSize=getFileListItemSize(oneIndexInfoArr->GetAt(i));
		strTime=getFileListItemEditTime(oneIndexInfoArr->GetAt(i));

		if(strSize == _T(""))
			nType=0;
		else
			nType=m_cSysIcon.GetExtIconID(strName);
		//插入数据
		LVITEM lvi;
		CString strItem;

		lvi.mask = TVIF_TEXT|TVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText =(LPTSTR)(LPCTSTR)strName;
		lvi.iImage = nType;		
		plistCtrl->InsertItem(&lvi);

		lvi.iSubItem =1;
		lvi.pszText = (LPTSTR)(LPCTSTR)strSize;
		plistCtrl->SetItem(&lvi);

		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)strTime;
		plistCtrl->SetItem(&lvi);

	}
	//SetCatalogSelected(m_strCurrentPath);
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
		//处于查找状态下
		if(m_bFindFileState)
		{
			CString tmpPath;
			m_pSearchFile->m_pMapFilePath->Lookup(m_lcFileShow.GetItemText(selItem,0)+_T("|")+m_lcFileShow.GetItemText(selItem,2),tmpPath);
			int pos;
			if(-1==(pos=tmpPath.Find(m_strUserLoginName)))
				pos=tmpPath.Find(m_strShareFloder);
			tmpPath=tmpPath.Right(tmpPath.GetLength()-pos);
			pos=tmpPath.ReverseFind('\\');
			m_strCurrentPath=tmpPath.Left(pos);
			m_bFindFileState=FALSE;
			((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
			((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

		}

		if(m_lcFileShow.GetItemText(selItem,1)==_T(""))
		{
			m_strCurrentPath=m_strCurrentPath+_T("\\")+m_lcFileShow.GetItemText(selItem,0);
			UpdateData(FALSE);

			m_btnReturnBack.EnableWindow(TRUE);
			//将本次操作路径添加到历史路径数组中
			HistroyPath* tmpHistroyPath=new HistroyPath();
			tmpHistroyPath->path=m_strCurrentPath;
			tmpHistroyPath->next=m_HistroyPathArr->next;
			m_HistroyPathArr->next=tmpHistroyPath;
			m_pHisPathTail=tmpHistroyPath;

			//得到该路径所在目录下的子文件和子目录信息
			if(m_Client.GetCatalogInfo(m_strCurrentPath))
			{
				m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
				ShowFileList(&m_lcFileShow,m_strIndexInfo);
				//设置目录视图选中
				SetCatalogSelected(m_strCurrentPath);
			}
		}
	}
	

	*pResult = 0;
}
//设置目录选中效果
void CNetdiskClientDlg::SetCatalogSelected(CString curPath)
{
	if(0 == (curPath.Find(m_strUserLoginName)))//判断用户单击的是我的网盘中的文件夹还是共享文件夹中的文件夹
		m_pCatShowDlg->SetSelectByFileListClick(m_pCatShowDlg->m_MyDisk,curPath);
	else 
		m_pCatShowDlg->SetSelectByFileListClick(m_pCatShowDlg->m_ShareDisk,curPath);

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
		ShowFileList(&m_lcFileShow,m_strIndexInfo);
		m_pCatShowDlg->SetSelectByFileListClick(m_pCatShowDlg->m_MyDisk,m_strCurrentPath);
	}
}

//返回上一个操作所在目录
void CNetdiskClientDlg::OnBnClickedMfcbtnNextcatalog()
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	if(m_Client.GetCatalogInfo(m_strCurrentPath))
	{
		m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		ShowFileList(&m_lcFileShow,m_strIndexInfo);
		m_pCatShowDlg->SetSelectByFileListClick(m_pCatShowDlg->m_MyDisk,m_strCurrentPath);
	}
}

//上传文件
void CNetdiskClientDlg::OnBnClickedBtnUploadfile()
{
	// TODO: Add your control notification handler code here
	CFileUploadDlg* m_pFileUpload=new CFileUploadDlg();
	if(m_pFileUpload->DoModal()==IDOK)
	{

	}
}

//刷新客户端视图
void CNetdiskClientDlg::OnBnClickedBtnRefresh()
{
	// TODO: Add your control notification handler code here
	m_bFindFileState=FALSE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

	if(m_Client.UpdateClient(m_strCurrentPath))
	{
		//获取文件列表显示
		if(m_Client.GetCatalogInfo(m_strCurrentPath))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			ShowFileList(&m_lcFileShow,m_strIndexInfo);
		}
		//更新我的网盘目录结构显示
		if(m_Client.UpdateClientCatalog(m_strUserLoginName))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pCatShowDlg->UpdateCatalogShow(m_pCatShowDlg->m_MyDisk,m_strIndexInfo);

		}
		//更新共享文件夹目录结构显示
		if(m_Client.UpdateClientCatalog(m_strShareFloder))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pCatShowDlg->UpdateCatalogShow(m_pCatShowDlg->m_ShareDisk,m_strIndexInfo);
		}
	}
}
//实现文件下载
void CNetdiskClientDlg::OnBnClickedBtnDownload()
{
	// TODO: Add your control notification handler code here
	int count=0;
	//判断是否有选中文件或者目录
	for (int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
			count++;
	}
	if(count==0)
	{
		AfxMessageBox(_T("请选择要下载的文件或者目录"));
		return ;
	}

	//开始下载文件或者目录
	CDownLoadDlg* pDownloadDlg=new CDownLoadDlg();
	if(pDownloadDlg->DoModal()== IDOK)
	{

	}
}

//移动文件或者文件夹
void CNetdiskClientDlg::OnBnClickedBtnMove()
{
	// TODO: Add your control notification handler code here
	m_bFindFileState=FALSE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

	int count=0;
	for(int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
		{
			count++;
		}
	}

	if(count == 0)
	{
		AfxMessageBox(_T("请选择要移动的文件或者目录"));
		return ;
	}

	CMoveFileDlg* pMovefileDlg=new CMoveFileDlg();
	if(pMovefileDlg->DoModal()==IDOK)
	{

	}
}

//按列表模式显示文件
void CNetdiskClientDlg::OnBnClickedBtnShowlist()
{
	// TODO: Add your control notification handler code here
	m_btnShowIcon.EnableWindow(TRUE);
	m_btnShowList.EnableWindow(FALSE);
	InitFileShow();
}

//按图标模式显示文件
void CNetdiskClientDlg::OnBnClickedBtnShowicon()
{
	// TODO: Add your control notification handler code here
	m_btnShowList.EnableWindow(TRUE);
	m_btnShowIcon.EnableWindow(FALSE);

	LONG lStyle;
	lStyle = GetWindowLong(m_lcFileShow.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle &= ~LVS_EX_CHECKBOXES; 
	SetWindowLong(m_lcFileShow.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_lcFileShow.GetExtendedStyle();

	m_lcFileShow.SetExtendedStyle(dwStyle); //设置扩展风格
	m_lcFileShow.SetImageList(&m_cSysIcon.m_ImageLargeList,LVSIL_NORMAL);

}

//回收站功能
void CNetdiskClientDlg::OnBnClickedBtnRecycle()
{
	// TODO: Add your control notification handler code here
	CRecycleDlg* pRecycleDlg=new CRecycleDlg();
	if(pRecycleDlg->DoModal() == IDOK)
	{

	}
}

//返回上一级目录
void CNetdiskClientDlg::OnBnClickedBtnReturnback()
{
	// TODO: Add your control notification handler code here
	m_bFindFileState=FALSE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

	if(m_strCurrentPath == m_strUserLoginName || m_strCurrentPath == m_strShareFloder)
	{
		m_btnReturnBack.EnableWindow(FALSE);
		return ;
	}
	int pos=m_strCurrentPath.ReverseFind('\\');
	m_strCurrentPath=m_strCurrentPath.Left(pos);

	SetCatalogSelected(m_strCurrentPath);

}

//历史版本功能
void CNetdiskClientDlg::OnBnClickedBtnHistory()
{
	// TODO: Add your control notification handler code here
	int count=0;
	for(int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
		{
			count++;
		}
	}
	if(count == 0)
	{
		AfxMessageBox(_T("请选择一个文件！"));
		return ;
	}
	if(count > 1)
	{
		AfxMessageBox(_T("一次只能选择一个文件"));
		return ;
	}
	for(int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
		{
			if(m_lcFileShow.GetItemText(i,1) == _T(""))
			{
				AfxMessageBox(_T("请选择一个文件！"));
				return ;
			}
		}
	}

	CHistroyVersionDlg* pHisVersionDlg=new CHistroyVersionDlg();
	if(pHisVersionDlg->DoModal() == IDOK)
	{

	}
}

//查找文件操作
void CNetdiskClientDlg::OnBnClickedBtnSearch()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if(m_strSearchStr == _T(""))
		return ;
	m_bFindFileState=TRUE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(FALSE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(FALSE);
	CSearchFile* pSearch=new CSearchFile();
	pSearch->SendSearchStr(m_strSearchStr);
}

//查找状态下一次只能处理一个文件或者文件夹操作
void CNetdiskClientDlg::OnNMClickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	//查找状态下
	if(m_bFindFileState)
	{
		int checkItem=pNMItemActivate->iItem;
		if(!m_lcFileShow.GetCheck(checkItem))
		{
			for(int i=0; i<m_lcFileShow.GetItemCount(); i++)
				m_lcFileShow.SetCheck(i,0);
			m_lcFileShow.SetCheck(checkItem,1);
		}
		//设置当前路径
		CString tmpPath;
		m_pSearchFile->m_pMapFilePath->Lookup(m_lcFileShow.GetItemText(checkItem,0)+_T("|")+m_lcFileShow.GetItemText(checkItem,2),tmpPath);
		int pos=tmpPath.Find(m_strUserLoginName);
		tmpPath=tmpPath.Right(tmpPath.GetLength()-pos);
		pos=tmpPath.ReverseFind('\\');
		m_strCurrentPath=tmpPath.Left(pos);
	}
	*pResult = 0;
}
