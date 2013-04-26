// FileUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "FileUploadDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

char Buffer[MAX_BUFFER_LEN];
#define TIMER 1 
// CFileUploadDlg dialog

IMPLEMENT_DYNAMIC(CFileUploadDlg, CDialogEx)

CFileUploadDlg::CFileUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileUploadDlg::IDD, pParent)
{
	m_ulFileSize = 0;
	m_ulCurTotal = 0;
	m_iAddFileCount = 0;
}

CFileUploadDlg::~CFileUploadDlg()
{
}

void CFileUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UPLOADFILELIST, m_lcUploadFile);
	DDX_Control(pDX, IDC_BTN_UPLOAD, m_btnUpload);
	DDX_Control(pDX, IDC_PROGRESS_UPLOAD, m_UploadProg);
}


BEGIN_MESSAGE_MAP(CFileUploadDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_CLEANLIST, &CFileUploadDlg::OnStnClickedStaticCleanlist)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
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

	m_lcUploadFile.SetImageList(&m_pMainDlg->m_cSysIcon.m_ImageSmallList,LVSIL_SMALL);

}

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//获取主程序对话框句柄
	m_pMainDlg=((CNetdiskClientDlg*)AfxGetMainWnd());
	//获取socket
	m_Client=m_pMainDlg->m_Client;
	m_Client.m_Client.sock=((CNetdiskClientApp*)AfxGetApp())->m_TmpClient;


	InitUploadFileList();
	m_UploadProg.ShowWindow(SW_HIDE);

	//m_pFileUploadArr=new CArray<CString,CString&>();
	m_pFileUploadMap=new CMap<CString, LPCTSTR, CString, LPCTSTR>();

	
	//重绘static text，设置清空listview的功能
	m_pClearList.Attach(GetDlgItem(IDC_STATIC_CLEANLIST)->GetSafeHwnd());
	m_pClearList.m_pFileUploadDlg=this;

	SetTimer(TIMER,10,NULL);

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
		//m_pFileUploadArr->Add(uplaodFileDlg.GetPathName());
		m_pFileUploadMap->SetAt(strFileName,uplaodFileDlg.GetPathName());

		if(readFile.Open(uplaodFileDlg.GetPathName(),CFile::modeRead))
		{
			ULONGLONG f_size=readFile.GetLength();
			if(f_size<1024)
				strFileSize.Format(_T("%.2lf B"),(double)f_size);
			else if(f_size>=1024&&f_size<(1024*1024))
				strFileSize.Format(_T("%.2lf KB"),(double)f_size/1024.0f);
			else
				strFileSize.Format(_T("%.2lf MB"),(double)f_size/(1024*1024.0f));

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
			int nType=m_pMainDlg->m_cSysIcon.GetExtIconID(strFileName);
			//插入数据
			LVITEM lvi;
			CString strItem;

			lvi.mask = TVIF_TEXT|TVIF_IMAGE;
			lvi.iItem = m_iAddFileCount;
			lvi.iSubItem = 0;
			lvi.pszText =(LPTSTR)(LPCTSTR)strFileName;
			lvi.iImage = nType;		
			m_lcUploadFile.InsertItem(&lvi);

			lvi.iSubItem =1;
			lvi.pszText = (LPTSTR)(LPCTSTR)strFileSize;
			m_lcUploadFile.SetItem(&lvi);

			lvi.iSubItem =2;
			lvi.pszText = (LPTSTR)(LPCTSTR)_T("准备上传");
			m_lcUploadFile.SetItem(&lvi);
			//int nRow=m_lcUploadFile.InsertItem(0,strFileName);
			//m_lcUploadFile.SetItemText(nRow,1,strFileSize);
			//m_lcUploadFile.SetItemText(nRow,2,_T("准备上传"));
			m_iAddFileCount++;
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

	int count=0;
	for(int i=0;i<m_lcUploadFile.GetItemCount();i++)
	{
		if(m_lcUploadFile.GetCheck(i))
			count++;
	}
	if(count==0)
	{
		AfxMessageBox(_T("请先选择要上传的文件！"));
		return;
	}
	//根据文件大小，设置进度条的范围
	for(int i=0;i<m_lcUploadFile.GetItemCount();i++)
	{
		if(m_lcUploadFile.GetCheck(i))
		{
			u_long fileSize;
			CString strPath;
			m_pFileUploadMap->Lookup(m_lcUploadFile.GetItemText(i,0),strPath);
			HANDLE hFile = CreateFile(strPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 		
			fileSize=GetFileSize(hFile,NULL);
			m_ulFileSize+=fileSize;
			CloseHandle(hFile);
		}
	}
	m_btnUpload.EnableWindow(FALSE);	
	//m_UploadProg.ShowWindow(SW_SHOW);
	//m_UploadProg.SetRange(0,totalSize);
	//u_long tmpTotalSize=0;

	m_pThreadWorker.cSocket=m_Client.m_Client.sock;
	m_pThreadWorker.pFileULDlg=this;
	m_pThreadWorker.pListCtrl=&m_lcUploadFile;
	m_pThreadWorker.pClient=&m_Client;
	m_pThreadWorker.pMainDlg=m_pMainDlg;

	//创建发送文件线程
	HANDLE hSend=CreateThread(0,0,UploadThread,&m_pThreadWorker,0,0);
	CloseHandle(hSend);

	//循环等待线程的结束
	//while(1)
	//{
	//	if(m_bUploadThread)
	//		break;
	//}
	//重置界面显示
	//ResetCurFileDL();
	
}

DWORD WINAPI CFileUploadDlg::UploadThread(LPVOID lpParam)
{
	THREADWORKER* threadWorker=(THREADWORKER*)lpParam;
	CNetdiskClientDlg* pMainDlg=threadWorker->pMainDlg;
	Client* pClient=threadWorker->pClient;
	CListCtrl* pListCtrl=threadWorker->pListCtrl;
	SOCKET cSocket=threadWorker->cSocket;
	CFileUploadDlg* pFileULDlg=threadWorker->pFileULDlg;


	//传送文件
	for(int i=0;i<pListCtrl->GetItemCount();i++)
	{
		if(pListCtrl->GetCheck(i))
		{

			//发送文件路径信息
			int result;
			CString	pathName;
			pathName=pMainDlg->m_strCurrentPath+_T("\\")+pListCtrl->GetItemText(i,0);
			DataPackage sendPack;
			ZeroMemory(&sendPack,sizeof(sendPack));
			sendPack.nPackLen=sizeof(sendPack);
			sendPack.iType=UPLOADFILE;
			int n=pathName.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,pathName,pathName.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,pathName,pathName.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送文件信息失败！"));
			}

			//发送文件数据
			FILE* infile;
			char c_path[1024];
			//CString path=m_pFileUploadArr->GetAt(i);
			CString path;
			pFileULDlg->m_pFileUploadMap->Lookup(pListCtrl->GetItemText(i,0),path);
			memset(c_path,0,path.GetLength()*2+1);
			USES_CONVERSION; 
			strcpy((LPSTR)c_path,OLE2A(path.LockBuffer()));
			infile=fopen(c_path,"rb");

			u_long ulFlagCount = 0;            //记录读了多少数据
			u_long ulFileRead = 0;					//一次实际读取的字节数
			char c_msg[1024];
			CString recvMsg;

			while((ulFileRead=fread(Buffer,sizeof( char),MAX_BUFFER_LEN,infile))!=0)
			{
				DataPackage sendPack;
				ZeroMemory(&sendPack,sizeof(sendPack));
				sendPack.nPackLen=sizeof(sendPack);
				sendPack.nContentLen=ulFileRead;
				CopyMemory(sendPack.sContent,Buffer,ulFileRead);
				sendPack.nPosition=ulFlagCount;

				result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
				Sleep(1);
				if(result==SOCKET_ERROR)
				{
					AfxMessageBox(_T("发送文件失败！"));
				}
				else
				{
					ulFlagCount+=ulFileRead;
					pFileULDlg->m_ulCurTotal+=ulFileRead;
				}
				//进度条显示
				//m_UploadProg.SetPos(tmpTotalSize);
			}

			fclose(infile);
			//发送文件结束包
			ZeroMemory(&sendPack,sizeof(sendPack));
			sendPack.nPackLen=sizeof(sendPack);
			CString strTmp=_T("send_file_end");
			n=strTmp.GetLength();
			len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送文件结束包失败！"));
			}
			//更新文件状态
			pListCtrl->SetItemText(i,2,_T("上传成功"));
			//更新客户端信息
			if(pClient->GetCatalogInfo(pMainDlg->m_strCurrentPath))
			{
				pMainDlg->m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
				pMainDlg->ShowFileList(pMainDlg->m_strIndexInfo);
			}
		}

	}
	pFileULDlg->ResetCurFileDL();
	return 0;
}
/*
 * 更新进度条
 */
void CFileUploadDlg::OnTimer(UINT nIDEvent) 
{
	if (m_ulCurTotal > 0)//当前下载文件的字节数
	{
		int nPercent = (float)m_ulCurTotal / m_ulFileSize * 100;//百分比
		m_UploadProg.SetPos(nPercent);							//设置进度条位置
		
		m_UploadProg.ShowWindow(SW_SHOW );						//显示进度条
		
	}

	CDialog::OnTimer(nIDEvent);
}

/*
 * 隐藏进度条控件
 */
void CFileUploadDlg::ResetCurFileDL(void)
{
	m_ulCurTotal = 0;
	m_UploadProg.SetPos(0);
	m_UploadProg.ShowWindow(SW_HIDE);
	m_btnUpload.EnableWindow(TRUE);	
	//清空列表的选中状态
	for(int i=0;i<m_lcUploadFile.GetItemCount();i++)
	{
		m_lcUploadFile.SetCheck(i,0);
	}
}