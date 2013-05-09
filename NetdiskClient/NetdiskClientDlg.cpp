
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
	m_strShareFloder=_T("�����ļ���");
	//���ӷ�������ȡ�ͻ��˸�����Ϣ
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
	//�ϴ��ļ����ϴ��ļ���
	//CMenu * popupMenu = new CMenu();
	//popupMenu->CreatePopupMenu();
	//popupMenu->AppendMenuW(MF_STRING, ID_UPLOADFILE, _T("�ϴ��ļ�"));
	//popupMenu->AppendMenuW(MF_STRING, ID_UPLOADCAT, _T("�ϴ��ļ���"));
	//m_mbUpload.m_hMenu = popupMenu->m_hMenu;

	//����ļ���ͼ��ʶ��
	HICON hIcon;
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,1);
	m_cSysIcon.m_ImageSmallList.Add(hIcon);
	ExtractIconEx(_T("shell32.dll"),3,NULL,&hIcon,0);
	m_cSysIcon.m_ImageLargeList.Add(hIcon);
	m_cSysIcon.m_arExtName.Add(_T(""));

	//m_Client.Start();


	//��ʷ·�������ʼ��
	m_HistroyPathArr=new HistroyPath();
	m_HistroyPathArr->path=m_strCurrentPath;
	m_HistroyPathArr->next=NULL;
	m_pHisPathHead=m_HistroyPathArr;
	m_pHisPathTail=m_HistroyPathArr;

	//����¼��֤�ɹ�֮�������socket����������
	m_Client.m_Client.sock=((CNetdiskClientApp*)AfxGetApp())->m_TmpClient;
	
	m_pCatShowDlg=new CCatalogShowDlg();
	m_pDateShowDlg=new CDateShowDlg();
	//��Ŀ¼��ʾ�Լ���������ʾ��ʼ��
	InitCatalogAndDateShow();

	//�ļ���ʾ��ʼ��
	m_lcFileShow.InsertColumn(0,_T("�ļ���"),LVCFMT_CENTER,300);
	m_lcFileShow.InsertColumn(1,_T("��С"),LVCFMT_CENTER,150);
	m_lcFileShow.InsertColumn(2,_T("����ʱ��"),LVCFMT_CENTER,175);
	InitFileShow();

	//���¿ͻ����ļ���Ϣ��ʾ
	OnBnClickedBtnRefresh();

	//��ʼ���ļ���ʾ��ʽ
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


//�½��ļ��в���
void CNetdiskClientDlg::OnBnClickedBtnNewfloder()
{
	CNewFloderDlg* pNewFloder=new CNewFloderDlg();
	//��������ָ�븳���½��ļ��жԻ���
	pNewFloder->m_pMainDlg=this;
	if(pNewFloder->DoModal()== IDOK)
	{
		
	}
}

//ɾ������
void CNetdiskClientDlg::OnBnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	m_bFindFileState=FALSE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

	int count=0;
	//����listview���鿴�Ƿ���ѡ��Ҫɾ������
	for(int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
			count++;
	}
	if(count == 0)
	{
		AfxMessageBox(_T("��ѡ��Ҫɾ�����ļ������ļ���"));
		return;
	}
	//�����û��Ƿ����ɾ������
	int nRes = AfxMessageBox(_T("ȷ������ѡ�ļ��Ƶ�����վ��"),MB_YESNO|MB_ICONQUESTION);
	if(nRes == IDYES)
	{
		//����ɾ���ļ������ļ��� 
		for(int i=0;i<m_lcFileShow.GetItemCount();i++)
		{
			if(m_lcFileShow.GetCheck(i))
			{
				CString path=m_strCurrentPath+_T("\\")+m_lcFileShow.GetItemText(i,0);
				m_Client.DeleteClientFile(path);
			}

		}
		//���¿ͻ�����ʾ
		OnBnClickedBtnRefresh();
		////��ȡ�ļ��б���ʾ
		//if(m_Client.GetCatalogInfo(m_strCurrentPath))
		//{
		//	m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		//	ShowFileList(&m_lcFileShow,m_strIndexInfo);
		//}
		////�����ҵ�����Ŀ¼�ṹ��ͼ
		//if(m_Client.UpdateClientCatalog(m_strUserLoginName))
		//{
		//	m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
		//	m_pCatShowDlg->UpdateCatalogShow(m_pCatShowDlg->m_MyDisk,m_strIndexInfo);

		//}

	}

}


//��Ӧ��Ŀ¼�鿴�����ǰ����ڲ鿴
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
//��ʼ����Ŀ¼�Ͱ�������ʾ
void CNetdiskClientDlg::InitCatalogAndDateShow()
{
	m_tcCatalogShow.InsertItem(0,_T("��Ŀ¼�鿴"));
	m_tcCatalogShow.InsertItem(1,_T("�����ڲ鿴"));

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

//��ʼ���ļ���ʾ
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

	int nType; // ���ڱ�ʾ�ļ�����Ŀ¼
	for(int i=0;i<oneIndexInfoArr->GetSize();i++)
	{
		strName=getFileListItemName(oneIndexInfoArr->GetAt(i));
		strSize=getFileListItemSize(oneIndexInfoArr->GetAt(i));
		strTime=getFileListItemEditTime(oneIndexInfoArr->GetAt(i));

		if(strSize == _T(""))
			nType=0;
		else
			nType=m_cSysIcon.GetExtIconID(strName);
		//��������
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

//��ȡһ��������¼
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
//��ȡ������Ϣ�е��ļ������ļ�������
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

//��ȡ������Ϣ�е��ļ��л����ļ��ĸ���ʱ��
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

//��ȡ������Ϣ�е��ļ��л����ļ��Ĵ�С���ļ��еĴ�СΪ0
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

//�ļ��б�˫���¼�����
void CNetdiskClientDlg::OnNMDblclkListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	int selItem=pNMItemActivate->iItem;
	if(selItem!=-1)
	{
		m_strCurrentPath=m_pCatShowDlg->m_strCurrentPath;
		//���ڲ���״̬��
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
			//�����β���·����ӵ���ʷ·��������
			HistroyPath* tmpHistroyPath=new HistroyPath();
			tmpHistroyPath->path=m_strCurrentPath;
			tmpHistroyPath->next=m_HistroyPathArr->next;
			m_HistroyPathArr->next=tmpHistroyPath;
			m_pHisPathTail=tmpHistroyPath;

			//�õ���·������Ŀ¼�µ����ļ�����Ŀ¼��Ϣ
			if(m_Client.GetCatalogInfo(m_strCurrentPath))
			{
				m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
				ShowFileList(&m_lcFileShow,m_strIndexInfo);
				//����Ŀ¼��ͼѡ��
				SetCatalogSelected(m_strCurrentPath);
			}
		}
	}
	

	*pResult = 0;
}
//����Ŀ¼ѡ��Ч��
void CNetdiskClientDlg::SetCatalogSelected(CString curPath)
{
	if(0 == (curPath.Find(m_strUserLoginName)))//�ж��û����������ҵ������е��ļ��л��ǹ����ļ����е��ļ���
		m_pCatShowDlg->SetSelectByFileListClick(m_pCatShowDlg->m_MyDisk,curPath);
	else 
		m_pCatShowDlg->SetSelectByFileListClick(m_pCatShowDlg->m_ShareDisk,curPath);

}
//������һ��Ŀ¼
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

//������һ����������Ŀ¼
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

//�ϴ��ļ�
void CNetdiskClientDlg::OnBnClickedBtnUploadfile()
{
	// TODO: Add your control notification handler code here
	CFileUploadDlg* m_pFileUpload=new CFileUploadDlg();
	if(m_pFileUpload->DoModal()==IDOK)
	{

	}
}

//ˢ�¿ͻ�����ͼ
void CNetdiskClientDlg::OnBnClickedBtnRefresh()
{
	// TODO: Add your control notification handler code here
	m_bFindFileState=FALSE;
	((CMFCButton*)GetDlgItem(IDC_BTN_UPLOADFILE))->EnableWindow(TRUE);
	((CMFCButton*)GetDlgItem(IDC_BTN_NEWFLODER))->EnableWindow(TRUE);

	if(m_Client.UpdateClient(m_strCurrentPath))
	{
		//��ȡ�ļ��б���ʾ
		if(m_Client.GetCatalogInfo(m_strCurrentPath))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			ShowFileList(&m_lcFileShow,m_strIndexInfo);
		}
		//�����ҵ�����Ŀ¼�ṹ��ʾ
		if(m_Client.UpdateClientCatalog(m_strUserLoginName))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pCatShowDlg->UpdateCatalogShow(m_pCatShowDlg->m_MyDisk,m_strIndexInfo);

		}
		//���¹����ļ���Ŀ¼�ṹ��ʾ
		if(m_Client.UpdateClientCatalog(m_strShareFloder))
		{
			m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_pCatShowDlg->UpdateCatalogShow(m_pCatShowDlg->m_ShareDisk,m_strIndexInfo);
		}
	}
}
//ʵ���ļ�����
void CNetdiskClientDlg::OnBnClickedBtnDownload()
{
	// TODO: Add your control notification handler code here
	int count=0;
	//�ж��Ƿ���ѡ���ļ�����Ŀ¼
	for (int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
			count++;
	}
	if(count==0)
	{
		AfxMessageBox(_T("��ѡ��Ҫ���ص��ļ�����Ŀ¼"));
		return ;
	}

	//��ʼ�����ļ�����Ŀ¼
	CDownLoadDlg* pDownloadDlg=new CDownLoadDlg();
	if(pDownloadDlg->DoModal()== IDOK)
	{

	}
}

//�ƶ��ļ������ļ���
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
		AfxMessageBox(_T("��ѡ��Ҫ�ƶ����ļ�����Ŀ¼"));
		return ;
	}

	CMoveFileDlg* pMovefileDlg=new CMoveFileDlg();
	if(pMovefileDlg->DoModal()==IDOK)
	{

	}
}

//���б�ģʽ��ʾ�ļ�
void CNetdiskClientDlg::OnBnClickedBtnShowlist()
{
	// TODO: Add your control notification handler code here
	m_btnShowIcon.EnableWindow(TRUE);
	m_btnShowList.EnableWindow(FALSE);
	InitFileShow();
}

//��ͼ��ģʽ��ʾ�ļ�
void CNetdiskClientDlg::OnBnClickedBtnShowicon()
{
	// TODO: Add your control notification handler code here
	m_btnShowList.EnableWindow(TRUE);
	m_btnShowIcon.EnableWindow(FALSE);

	LONG lStyle;
	lStyle = GetWindowLong(m_lcFileShow.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle &= ~LVS_EX_CHECKBOXES; 
	SetWindowLong(m_lcFileShow.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_lcFileShow.GetExtendedStyle();

	m_lcFileShow.SetExtendedStyle(dwStyle); //������չ���
	m_lcFileShow.SetImageList(&m_cSysIcon.m_ImageLargeList,LVSIL_NORMAL);

}

//����վ����
void CNetdiskClientDlg::OnBnClickedBtnRecycle()
{
	// TODO: Add your control notification handler code here
	CRecycleDlg* pRecycleDlg=new CRecycleDlg();
	if(pRecycleDlg->DoModal() == IDOK)
	{

	}
}

//������һ��Ŀ¼
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

//��ʷ�汾����
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
		AfxMessageBox(_T("��ѡ��һ���ļ���"));
		return ;
	}
	if(count > 1)
	{
		AfxMessageBox(_T("һ��ֻ��ѡ��һ���ļ�"));
		return ;
	}
	for(int i=0;i<m_lcFileShow.GetItemCount();i++)
	{
		if(m_lcFileShow.GetCheck(i))
		{
			if(m_lcFileShow.GetItemText(i,1) == _T(""))
			{
				AfxMessageBox(_T("��ѡ��һ���ļ���"));
				return ;
			}
		}
	}

	CHistroyVersionDlg* pHisVersionDlg=new CHistroyVersionDlg();
	if(pHisVersionDlg->DoModal() == IDOK)
	{

	}
}

//�����ļ�����
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

//����״̬��һ��ֻ�ܴ���һ���ļ������ļ��в���
void CNetdiskClientDlg::OnNMClickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	//����״̬��
	if(m_bFindFileState)
	{
		int checkItem=pNMItemActivate->iItem;
		if(!m_lcFileShow.GetCheck(checkItem))
		{
			for(int i=0; i<m_lcFileShow.GetItemCount(); i++)
				m_lcFileShow.SetCheck(i,0);
			m_lcFileShow.SetCheck(checkItem,1);
		}
		//���õ�ǰ·��
		CString tmpPath;
		m_pSearchFile->m_pMapFilePath->Lookup(m_lcFileShow.GetItemText(checkItem,0)+_T("|")+m_lcFileShow.GetItemText(checkItem,2),tmpPath);
		int pos=tmpPath.Find(m_strUserLoginName);
		tmpPath=tmpPath.Right(tmpPath.GetLength()-pos);
		pos=tmpPath.ReverseFind('\\');
		m_strCurrentPath=tmpPath.Left(pos);
	}
	*pResult = 0;
}
