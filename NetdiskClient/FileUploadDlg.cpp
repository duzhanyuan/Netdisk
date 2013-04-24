// FileUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "FileUploadDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

char Buffer[MAX_BUFFER_LEN];

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
	DDX_Control(pDX, IDC_BTN_UPLOAD, m_btnUpload);
	DDX_Control(pDX, IDC_PROGRESS_UPLOAD, m_UploadProg);
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

}

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitUploadFileList();
	m_UploadProg.ShowWindow(SW_HIDE);

	//m_pFileUploadArr=new CArray<CString,CString&>();
	m_pFileUploadMap=new CMap<CString, LPCTSTR, CString, LPCTSTR>();

	//��ȡ������Ի�����
	m_pMainDlg=((CNetdiskClientDlg*)AfxGetMainWnd());
	//��ȡsocket
	m_Client=m_pMainDlg->m_Client;
	m_Client.m_Client.sock=((CNetdiskClientApp*)AfxGetApp())->m_TmpClient;

	//�ػ�static text���������listview�Ĺ���
	m_pClearList.Attach(GetDlgItem(IDC_STATIC_CLEANLIST)->GetSafeHwnd());
	m_pClearList.m_pFileUploadDlg=this;
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
			int nRow=m_lcUploadFile.InsertItem(0,strFileName);
			m_lcUploadFile.SetItemText(nRow,1,strFileSize);
			m_lcUploadFile.SetItemText(nRow,2,_T("׼���ϴ�"));
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
	u_long totalSize=0;
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
			totalSize+=fileSize;
			CloseHandle(hFile);
		}
	}
	m_btnUpload.EnableWindow(FALSE);	
	m_UploadProg.ShowWindow(SW_SHOW);
	m_UploadProg.SetRange(0,totalSize);
	u_long tmpTotalSize=0;

	//�����ļ�
	for(int i=0;i<m_lcUploadFile.GetItemCount();i++)
	{
		if(m_lcUploadFile.GetCheck(i))
		{
			
			//�����ļ�·����Ϣ
			int result;
			CString	pathName;
			pathName=m_pMainDlg->m_strCurrentPath+_T("\\")+m_lcUploadFile.GetItemText(i,0);
			DataPackage sendPack;
			ZeroMemory(&sendPack,sizeof(sendPack));
			sendPack.nPackLen=sizeof(sendPack);
			sendPack.iType=UPLOADFILE;
			int n=pathName.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,pathName,pathName.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,pathName,pathName.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			result=send(m_Client.m_Client.sock,(char*)&sendPack,sendPack.nPackLen,0);
			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("�����ļ���Ϣʧ�ܣ�"));
			}
			
			//�����ļ�����
			FILE* infile;
			char c_path[1024];
			//CString path=m_pFileUploadArr->GetAt(i);
			CString path;
			m_pFileUploadMap->Lookup(m_lcUploadFile.GetItemText(i,0),path);
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

				result=send(m_Client.m_Client.sock,(char*)&sendPack,sendPack.nPackLen,0);
				Sleep(1);
				if(result==SOCKET_ERROR)
				{
					AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
				}
				else
				{
					ulFlagCount+=ulFileRead;
					tmpTotalSize+=ulFileRead;
				}
				//��������ʾ
				m_UploadProg.SetPos(tmpTotalSize);
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
			result=send(m_Client.m_Client.sock,(char*)&sendPack,sendPack.nPackLen,0);
			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("�����ļ�������ʧ�ܣ�"));
			}
			//�����ļ�״̬
			m_lcUploadFile.SetItemText(i,2,_T("�ϴ��ɹ�"));


			//���¿ͻ�����Ϣ
			if(m_Client.GetCatalogInfo(m_pMainDlg->m_strCurrentPath))
			{
				m_pMainDlg->m_strIndexInfo=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
				m_pMainDlg->ShowFileList(m_pMainDlg->m_strIndexInfo);
			}
		}

	}

	m_btnUpload.EnableWindow(TRUE);	
	m_UploadProg.ShowWindow(SW_HIDE);
	//����б��ѡ��״̬
	for(int i=0;i<m_lcUploadFile.GetItemCount();i++)
	{
		m_lcUploadFile.SetCheck(i,0);
	}
}
