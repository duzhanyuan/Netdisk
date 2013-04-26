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
	lStyle = GetWindowLong(m_lcUploadFile.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_lcUploadFile.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle = m_lcUploadFile.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//������
	dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_lcUploadFile.SetExtendedStyle(dwStyle); //������չ���
	m_lcUploadFile.InsertColumn(0,_T("�ļ���"),LVCFMT_CENTER,300);
	m_lcUploadFile.InsertColumn(1,_T("��С"),LVCFMT_CENTER,150);
	m_lcUploadFile.InsertColumn(2,_T("״̬"),LVCFMT_CENTER,150);

	m_lcUploadFile.SetImageList(&m_pMainDlg->m_cSysIcon.m_ImageSmallList,LVSIL_SMALL);

}

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//��ȡ������Ի�����
	m_pMainDlg=((CNetdiskClientDlg*)AfxGetMainWnd());
	//��ȡsocket
	m_Client=m_pMainDlg->m_Client;
	m_Client.m_Client.sock=((CNetdiskClientApp*)AfxGetApp())->m_TmpClient;


	InitUploadFileList();
	m_UploadProg.ShowWindow(SW_HIDE);

	//m_pFileUploadArr=new CArray<CString,CString&>();
	m_pFileUploadMap=new CMap<CString, LPCTSTR, CString, LPCTSTR>();

	
	//�ػ�static text���������listview�Ĺ���
	m_pClearList.Attach(GetDlgItem(IDC_STATIC_CLEANLIST)->GetSafeHwnd());
	m_pClearList.m_pFileUploadDlg=this;

	SetTimer(TIMER,10,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//����ļ����б���׼���ϴ�
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
		AfxMessageBox(_T("�ļ���ӹ��࣬�޷���������ļ���"));
		return;
	}

	if(strFileName!=_T(""))
	{
		if(!IfFileAdded(strFileName))
		{
			int nType=m_pMainDlg->m_cSysIcon.GetExtIconID(strFileName);
			//��������
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
			lvi.pszText = (LPTSTR)(LPCTSTR)_T("׼���ϴ�");
			m_lcUploadFile.SetItem(&lvi);
			//int nRow=m_lcUploadFile.InsertItem(0,strFileName);
			//m_lcUploadFile.SetItemText(nRow,1,strFileSize);
			//m_lcUploadFile.SetItemText(nRow,2,_T("׼���ϴ�"));
			m_iAddFileCount++;
		}
		else
			AfxMessageBox(_T("�ļ��Ѿ���ӣ�"));
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

//����ѡ����ļ��ϴ�
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
		AfxMessageBox(_T("����ѡ��Ҫ�ϴ����ļ���"));
		return;
	}
	//�����ļ���С�����ý������ķ�Χ
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

	//���������ļ��߳�
	HANDLE hSend=CreateThread(0,0,UploadThread,&m_pThreadWorker,0,0);
	CloseHandle(hSend);

	//ѭ���ȴ��̵߳Ľ���
	//while(1)
	//{
	//	if(m_bUploadThread)
	//		break;
	//}
	//���ý�����ʾ
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


	//�����ļ�
	for(int i=0;i<pListCtrl->GetItemCount();i++)
	{
		if(pListCtrl->GetCheck(i))
		{

			//�����ļ�·����Ϣ
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
				AfxMessageBox(_T("�����ļ���Ϣʧ�ܣ�"));
			}

			//�����ļ�����
			FILE* infile;
			char c_path[1024];
			//CString path=m_pFileUploadArr->GetAt(i);
			CString path;
			pFileULDlg->m_pFileUploadMap->Lookup(pListCtrl->GetItemText(i,0),path);
			memset(c_path,0,path.GetLength()*2+1);
			USES_CONVERSION; 
			strcpy((LPSTR)c_path,OLE2A(path.LockBuffer()));
			infile=fopen(c_path,"rb");

			u_long ulFlagCount = 0;            //��¼���˶�������
			u_long ulFileRead = 0;					//һ��ʵ�ʶ�ȡ���ֽ���
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
					AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
				}
				else
				{
					ulFlagCount+=ulFileRead;
					pFileULDlg->m_ulCurTotal+=ulFileRead;
				}
				//��������ʾ
				//m_UploadProg.SetPos(tmpTotalSize);
			}

			fclose(infile);
			//�����ļ�������
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
				AfxMessageBox(_T("�����ļ�������ʧ�ܣ�"));
			}
			//�����ļ�״̬
			pListCtrl->SetItemText(i,2,_T("�ϴ��ɹ�"));
			//���¿ͻ�����Ϣ
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
 * ���½�����
 */
void CFileUploadDlg::OnTimer(UINT nIDEvent) 
{
	if (m_ulCurTotal > 0)//��ǰ�����ļ����ֽ���
	{
		int nPercent = (float)m_ulCurTotal / m_ulFileSize * 100;//�ٷֱ�
		m_UploadProg.SetPos(nPercent);							//���ý�����λ��
		
		m_UploadProg.ShowWindow(SW_SHOW );						//��ʾ������
		
	}

	CDialog::OnTimer(nIDEvent);
}

/*
 * ���ؽ������ؼ�
 */
void CFileUploadDlg::ResetCurFileDL(void)
{
	m_ulCurTotal = 0;
	m_UploadProg.SetPos(0);
	m_UploadProg.ShowWindow(SW_HIDE);
	m_btnUpload.EnableWindow(TRUE);	
	//����б��ѡ��״̬
	for(int i=0;i<m_lcUploadFile.GetItemCount();i++)
	{
		m_lcUploadFile.SetCheck(i,0);
	}
}