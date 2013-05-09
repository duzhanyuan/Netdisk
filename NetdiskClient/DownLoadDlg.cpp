// DownLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "DownLoadDlg.h"
#include "afxdialogex.h"
#include "NetdiskClientDlg.h"

// CDownLoadDlg dialog
#define TIMER 1 

IMPLEMENT_DYNAMIC(CDownLoadDlg, CDialogEx)

CDownLoadDlg::CDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDownLoadDlg::IDD, pParent)
{
	//  m_str = _T("");
	m_strSavePath = _T("");
	m_ulFileSize = 0;
	m_ulCurTotal = 0;
	//  m_strDownloadMsg = _T("");
}

CDownLoadDlg::~CDownLoadDlg()
{
}

void CDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLOADPRO, m_progDownload);
	//  DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_str);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	//  DDX_LBString(pDX, IDC_LIST_DOWNLOADMSG, m_strDownloadMsg);
	DDX_Control(pDX, IDC_LIST_DOWNLOADMSG, m_lbDownloadMsg);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_DOWNLOADPRO, m_progDownload);
}


BEGIN_MESSAGE_MAP(CDownLoadDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDownLoadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDownLoadDlg::OnBnClickedButton1)
	ON_WM_TIMER()

END_MESSAGE_MAP()


// CDownLoadDlg message handlers


void CDownLoadDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	//{
	//	if(m_pMainDlg->m_lcFileShow.GetCheck(i))
	//	{
	//	}
	//}
	if(m_strSavePath == _T(""))
	{
		AfxMessageBox(_T("��ѡ��Ҫ��ŵ�Ŀ¼"));
		return;
	}

	//��ȡ�������ļ���С
	for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(m_pMainDlg->m_lcFileShow.GetCheck(i))
		{

			//��ȡ���ش�С
			m_pMainDlg->m_Client.GetFileSizeFromServ(m_pMainDlg->m_strCurrentPath+_T("\\")+m_pMainDlg->m_lcFileShow.GetItemText(i,0));
			CString strFilesize=((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo;
			m_ulFileSize +=_ttoi64(strFilesize);
		}
	}

	//���������߳�
	HANDLE hDlHandle=CreateThread(0,0,DownLoadThread,this,0,0);
	CloseHandle(hDlHandle);

	
}

DWORD WINAPI CDownLoadDlg::DownLoadThread(LPVOID lpParam)
{
	CDownLoadDlg* pDownLoadDlg = (CDownLoadDlg*)lpParam;
	CString msg;
	CString fileName;

	for(int i=0;i<pDownLoadDlg->m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(pDownLoadDlg->m_pMainDlg->m_lcFileShow.GetCheck(i))
		{
			fileName=pDownLoadDlg->m_pMainDlg->m_lcFileShow.GetItemText(i,0);
			CString path=pDownLoadDlg->m_pMainDlg->m_strCurrentPath+_T("\\")+fileName;
			//����Ŀ¼
			if(pDownLoadDlg->m_pMainDlg->m_lcFileShow.GetItemText(i,1)==_T(""))
			{
				pDownLoadDlg->m_pMainDlg->m_Client.SendMsgToServ(path,DOWNLOADCATALOG);
				CString catPath=pDownLoadDlg->m_strSavePath+_T("\\")+fileName;
				if(CreateDirectory(catPath,NULL))
				{
					pDownLoadDlg->RecvDownLoadCat(catPath);
				}
			}
			//�����ļ�
			else
			{
				pDownLoadDlg->m_pMainDlg->m_Client.SendMsgToServ(path,DOWNLOADFILE);
				CString filepath=pDownLoadDlg->m_strSavePath+_T("\\")+fileName;
				pDownLoadDlg->RecvDownLoadFile(filepath);
			}

			msg.Format(_T("%s �Ѿ��������"),fileName);
			pDownLoadDlg->m_lbDownloadMsg.InsertString(pDownLoadDlg->m_lbDownloadMsg.GetCount(),msg);

			//pDownLoadDlg->UpdateData(FALSE);
		}
	}
	
	pDownLoadDlg->ResetCurFileDL();
	return 0;
}

//����Ŀ¼
void CDownLoadDlg::RecvDownLoadCat(CString catalogName)
{
	CString tmpStr;
	int retVal;
	DataPackage recvPack;
	ZeroMemory(&recvPack,sizeof(recvPack));
	recvPack.nPackLen=sizeof(recvPack);

	retVal=recv(m_pMainDlg->m_Client.m_Client.sock,(char*)&recvPack,sizeof(recvPack),0);
	tmpStr=recvPack.sContent;
	while(tmpStr != _T("send_cat_end"))
	{
		if(recvPack.iType == 1) //���͵���Ŀ¼������Ϣ
		{
			CString path=catalogName+_T("\\")+tmpStr;
			CreateDirectory(path,NULL);
		}
		if(recvPack.iType == 2)//���͵����ļ�
		{
			CString filePath=catalogName+_T("\\")+tmpStr;
			RecvDownLoadFile(filePath);
		}

		ZeroMemory(&recvPack,sizeof(recvPack));
		recvPack.nPackLen=sizeof(recvPack);

		retVal=recv(m_pMainDlg->m_Client.m_Client.sock,(char*)&recvPack,sizeof(recvPack),0);
		tmpStr=recvPack.sContent;

	}

}

//�����ļ�
void CDownLoadDlg::RecvDownLoadFile(CString path)
{
	DataPackage recvPack;
	int iErrCode;
	recvPack.nPackLen=sizeof(recvPack);

	//if(recvPack.iType == 2) //1Ϊ�ļ��У�2Ϊ�ļ�
	//{

	//�����ļ�
	FILE* inFile;
	char c_path[1024];
	int n=path.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,path,path.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,path,path.GetLength()+1,c_path,len+1,NULL,NULL);
	c_path[len]='\0';

	//�����ļ�����
	ZeroMemory(&recvPack,sizeof(recvPack));
	//�����ļ�
	inFile=fopen(c_path,"wb");
	fclose(inFile);

	iErrCode=recv(m_pMainDlg->m_Client.m_Client.sock,(char*)&recvPack,sizeof(recvPack),0);
	CString strMsg;
	strMsg=recvPack.sContent;
	while(iErrCode>0 && strMsg!=_T("send_file_end"))
	{
		inFile=fopen(c_path,"ab");

		u_long nFilePosition=recvPack.nPosition;
		int nRe=fseek(inFile,nFilePosition,SEEK_SET);
		if(nRe)
		{
			AfxMessageBox(_T("SendDataThrad�ж�λʧ��!"));
			break;
		}
		u_long nNumberOfBytesWritten=fwrite(&recvPack.sContent,sizeof(char),recvPack.nContentLen,inFile);
		m_ulCurTotal+=nNumberOfBytesWritten;// ���õ�ǰ�Ѿ���ȡ���ļ���С
		if(recvPack.nContentLen != nNumberOfBytesWritten)
		{
			AfxMessageBox(_T("д�ļ�ʧ��!"));
			break;
		}
		ZeroMemory(&recvPack,sizeof(recvPack));
		fflush(inFile);
		fclose(inFile);

		iErrCode=recv(m_pMainDlg->m_Client.m_Client.sock,(char*)&recvPack,sizeof(recvPack),0);
		strMsg=recvPack.sContent;

	}
	//	return NULL;
	//}
	//else if(recvPack.iType == 1)
	//{
	//	CString tmpPath;
	//	tmpPath=recvPack.sContent;
	//	tmpPath=path+_T("\\")+tmpPath;
	//	CreateDirectory(tmpPath,NULL);

	//	return strMsg;
	//}
}

BOOL CDownLoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_progDownload.ShowWindow(SW_HIDE);
	m_pMainDlg=(CNetdiskClientDlg*)AfxGetMainWnd();
	// TODO:  Add extra initialization here
	SetTimer(TIMER,10,NULL);
	m_btnOK.EnableWindow(TRUE);
	//��ʾ��Ϣ
	CString msg;
	for(int i=0;i<m_pMainDlg->m_lcFileShow.GetItemCount();i++)
	{
		if(m_pMainDlg->m_lcFileShow.GetCheck(i))
		{
			msg.Format(_T("%s ����ӣ��ȴ�����..."),m_pMainDlg->m_lcFileShow.GetItemText(i,0));
			m_lbDownloadMsg.InsertString(m_lbDownloadMsg.GetCount(),msg);
			//UpdateData(FALSE);

		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//ѡ���û�����ļ���Ŀ¼
void CDownLoadDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	TCHAR tcSaveFilePath[MAX_PATH];	//��ʱ�ַ�����
	BROWSEINFO browInfo = {0};		//����
	browInfo.hwndOwner = GetSafeHwnd();
	browInfo.ulFlags = BIF_RETURNFSANCESTORS   ;
	browInfo.lpszTitle = _T("���ñ���·��");//����
	LPITEMIDLIST browseFolder = SHBrowseForFolder(&browInfo);//������ļ��жԻ���
	if( browseFolder != NULL )
	{
		SHGetPathFromIDList(browseFolder, tcSaveFilePath);	//�õ��ļ���·��    
		SetCurrentDirectory(tcSaveFilePath);				//���ø�Ŀ¼Ϊ��ǰ�Ĺ���Ŀ¼
		m_strSavePath.Empty();
		m_strSavePath = tcSaveFilePath;						//�����ļ�·��

		UpdateData(FALSE);									//���¶Ի���
	}
	CString msg;
	msg.Format(_T("�����ļ����Ŀ¼Ϊ%s"),m_strSavePath);
	m_lbDownloadMsg.InsertString(m_lbDownloadMsg.GetCount(),msg);
}
/*
 * ���½�����
 */
void CDownLoadDlg::OnTimer(UINT nIDEvent) 
{

	if (m_ulCurTotal > 0)//��ǰ�����ļ����ֽ���
	{
		int nPercent = (float)m_ulCurTotal / m_ulFileSize * 100;//�ٷֱ�
		m_progDownload.SetPos(nPercent);							//���ý�����λ��
		
		m_progDownload.ShowWindow(SW_SHOW );						//��ʾ������
		
	}
	
	CDialog::OnTimer(nIDEvent);
}
/*
 * ���ؽ������ؼ�
 */
void CDownLoadDlg::ResetCurFileDL(void)
{
	m_ulCurTotal = 0;
	m_progDownload.SetPos(0);
	m_progDownload.ShowWindow(SW_HIDE);
	m_btnOK.EnableWindow(FALSE);
}