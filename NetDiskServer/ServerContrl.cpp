// ServerContrl.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "ServerContrl.h"
#include "ServIndex.h"

char Buffer[MAX_BUFFER_LEN];

// CServerContrl

CServerContrl::CServerContrl()
{
	m_strDiskRootPath=_T("E:\\��ҵ����Ŀ¼");
	InitializeCriticalSection(&m_csFile);

}

CServerContrl::~CServerContrl()
{
	DeleteCriticalSection(&m_csFile);
}

bool CServerContrl::InitSocket()
{
	WSADATA wsaData;
	int ErrCode;

	//��ʼ��socket����
	ErrCode=WSAStartup(MAKEWORD(2,2),&wsaData);
	if(0 != ErrCode)
	{
		AfxMessageBox(_T("WSAStartup����ʧ�ܣ�"));
		return false;
	}

	//�����������׽���
	m_sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sServer)
	{
		AfxMessageBox(_T("socket����ʧ�ܣ�"));
		return false;
	}

	//��ȡϵͳĬ�ϵĽ������ݻ�������С
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	int nErrCode= getsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//����ϵͳ�������ݻ�����ΪĬ��ֵ��10��
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;
	}


	//�������ϵͳ�������ݻ������Ƿ�ɹ�
	unsigned int uiNewRcvBuf;
	getsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;

	}

	//��ȡϵͳĬ�ϵķ������ݻ�������С
	nErrCode= getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//����ϵͳ�������ݻ�����ΪĬ��ֵ��10��
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;

	}


	//�������ϵͳ�������ݻ������Ƿ�ɹ�
	getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;

	}

	//�������׽��ֵ�ַ
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_PORT);

	//�󶨷�����
	nErrCode=bind(m_sServer,(sockaddr*)&servAddr,sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("bind ����ʧ�ܣ�"));
		closesocket(m_sServer);
		WSACleanup();
		return false;
	}

	//���ü���socket
	nErrCode=listen(m_sServer,SOMAXCONN);
	if(SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("listen ����ʧ�ܣ�"));
		closesocket(m_sServer);
		WSACleanup();
		return false;
	}

	return true;
}

//���������������߳�
bool CServerContrl::StartServ()
{
	//��ʼ��socket
	if(!InitSocket())
	{
		return false;
	}

	//���������߳�
	m_hServCtrl=CreateThread(0,0,ServListenThread,this,0,0);
	m_bThreadCtrl=FALSE;
	
	AfxMessageBox(_T("�����������ɹ���"));
	return true;
}

//�����߳�
DWORD WINAPI CServerContrl::ServListenThread(LPVOID lpParam)
{

	CServerContrl* pServCtrl=(CServerContrl*)lpParam;

	if(pServCtrl->m_bThreadCtrl)
		ExitThread(0);

	FD_ZERO(&(pServCtrl->m_fdAllSocket));
	FD_SET(pServCtrl->m_sServer,&(pServCtrl->m_fdAllSocket));
	timeval tv;
	tv.tv_sec=1000;
	tv.tv_usec=0;

	while(true)
	{
		pServCtrl->m_fdRead=pServCtrl->m_fdAllSocket;
		int nRet=select(NULL,&(pServCtrl->m_fdRead),NULL,NULL,&tv);
		if(0 == nRet)
		{
			AfxMessageBox(_T("select����ʧ�ܣ�"));
			return false;
		}

		for(int i=0;i<pServCtrl->m_fdAllSocket.fd_count;i++)
		{
			if(FD_ISSET(pServCtrl->m_fdAllSocket.fd_array[i],&pServCtrl->m_fdRead))
			{
				//�����µ�����
				if(pServCtrl->m_sServer == pServCtrl->m_fdAllSocket.fd_array[i])
				{
					sockaddr_in newConn;
					int connLen=sizeof(newConn);
					SOCKET sNew=accept(pServCtrl->m_sServer,(sockaddr*)&newConn,&connLen);
					FD_SET(sNew,&pServCtrl->m_fdAllSocket);
					//����ͻ����ڷ������ϵ���ʾ
				}
				else
				{
					DataPackage recvPack;
					DataPackage sendPack;
					ZeroMemory(&recvPack,sizeof(recvPack));

					int iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
					if(0 == iErrCode)
					{
						AfxMessageBox(_T("���ӹرգ�"));
						return false;
					}
					else if(iErrCode < 0)
					{
						CString strMsg;
						strMsg.Format(_T("��������ʧ�ܣ��������Ϊ: %d\n"), WSAGetLastError());
						AfxMessageBox(strMsg);
						return false;
					}

					//�ж��û��Ĳ�������
					switch(recvPack.iType)
					{
					case LOGIN:
						{
							//�������ݿ⣬�жϽ�����û��Ƿ���ڣ���������н������ݲ����������������
							CString userInfo;
							userInfo=recvPack.sContent;
							int ret=pServCtrl->UserLogin(userInfo);

							//�����û���¼��Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							sprintf(sendPack.sContent,"%d",ret);
							sendPack.nPackLen=sizeof(sendPack);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("�����û���¼����ʧ�ܣ�"));
								return false;
							}
							//��¼�û�����
							pServCtrl->m_strUserName=userInfo.Left(userInfo.Find(':'));
							break;
						}
					case UPDATECLIENT:
						{
							CServIndex* ServIndex=new CServIndex();
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							//��ȡ���û���Ŀ¼������Ϣ
							pServCtrl->m_StrIndexInfo=ServIndex->GetIndexInfo(m_strUserMsg);

							//�����û���Ŀ¼��Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							int n=pServCtrl->m_StrIndexInfo.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
							sendPack.sContent[len]='\0';
							sendPack.nContentLen=len;
							sendPack.nPackLen=sizeof(sendPack);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("�����û�Ŀ¼��Ϣʧ�ܣ�"));
								return false;
							}
							break;
						}
					case GETCATALOGINFO:
						{
							CServIndex* ServIndex=new CServIndex();
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							pServCtrl->m_StrIndexInfo=ServIndex->getCatalogInfo(m_strUserMsg);

							//�����û���Ŀ¼��Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							int n=pServCtrl->m_StrIndexInfo.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
							sendPack.sContent[len]='\0';
							sendPack.nContentLen=len;
							sendPack.nPackLen=sizeof(sendPack);

							//u_long mode = 1;
							//ioctlsocket(pServCtrl->m_fdAllSocket.fd_array[i],FIONBIO,&mode);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("�����û�Ŀ¼�ṹ��Ϣʧ�ܣ�"));
								return false;
							}
							break;
						}
					case UPLOADFILE:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							//�����ļ�
							CString path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
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

							iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
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
								if(recvPack.nContentLen != nNumberOfBytesWritten)
								{
									AfxMessageBox(_T("д�ļ�ʧ��!"));
									break;
								}
								ZeroMemory(&recvPack,sizeof(recvPack));
								fflush(inFile);
								fclose(inFile);

								iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
								strMsg=recvPack.sContent;

							}
							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);
							break;
						}
					case NEWFLODER:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;

							CString path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							if(!CreateDirectory(path,NULL))
							{
								CString msgStr;
								msgStr.Format(_T("�����ļ���ʧ�ܣ��������Ϊ��%s"),GetLastError());
								return false;
							}

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);

							break;
						}
					case DELETEFILE:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							
							CString path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							//�ж�ɾ�������ļ������ļ���
							DWORD value = GetFileAttributes(path);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
								pServCtrl->	RecursiveDelete(path);
							else
								DeleteFile(path);

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);
							break;
						}
					case REFRESH:
						{
							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);

							break;
						}
					case DOWNLOADFILE:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;

							CString path;
							path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							//�����ļ���Ϣ
							pServCtrl->SendFile(pServCtrl->m_fdAllSocket.fd_array[i],path);

							break;
						}
					case DOWNLOADCATALOG:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							CString tmpPath=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							//����Ŀ¼��Ϣ
							pServCtrl->RecursiveSend(pServCtrl->m_fdAllSocket.fd_array[i],tmpPath,tmpPath);

							//����Ŀ¼������Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							CString endSending=_T("send_cat_end");
							int n=endSending.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,endSending,endSending.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,endSending,endSending.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
							sendPack.sContent[len]='\0';
							sendPack.nContentLen=len;
							sendPack.iType=1;
							sendPack.nPackLen=sizeof(sendPack);
							int result=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);

							if(result==SOCKET_ERROR)
							{
								AfxMessageBox(_T("����Ŀ¼������ʧ�ܣ�"));
							}
							break;
						}
					case MOVEFILE:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							
							int pos=m_strUserMsg.Find('+');
							CString srcPath=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg.Left(pos);
							CString desPath=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg.Right(m_strUserMsg.GetLength()-pos-1);

							//�ж�ɾ�������ļ������ļ���
							DWORD value = GetFileAttributes(srcPath);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
							{
								pServCtrl->RecursiveMove(srcPath,desPath);
							}
							else
								MoveFile(srcPath,desPath);

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);

							break;
						}
					}
				}
			}
		}
	}
	Sleep(100);
}
//�����ļ�
void CServerContrl::SendFile(SOCKET cSocket,CString filePath)
{
	//�����ļ�����
	FILE* infile;
	char c_path[1024];

	memset(c_path,0,filePath.GetLength()*2+1);
	USES_CONVERSION; 
	strcpy((LPSTR)c_path,OLE2A(filePath.LockBuffer()));
	infile=fopen(c_path,"rb");

	u_long ulFlagCount = 0;            //��¼���˶�������
	u_long ulFileRead = 0;					//һ��ʵ�ʶ�ȡ���ֽ���
	char c_msg[1024];
	CString recvMsg;
	int result;
	DataPackage sendPack;

	while((ulFileRead=fread(Buffer,sizeof( char),MAX_BUFFER_LEN,infile))!=0)
	{
		ZeroMemory(&sendPack,sizeof(sendPack));
		sendPack.nPackLen=sizeof(sendPack);
		sendPack.nContentLen=ulFileRead;
		CopyMemory(sendPack.sContent,Buffer,ulFileRead);
		sendPack.nPosition=ulFlagCount;
		sendPack.iType=2;

		result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
		Sleep(1);
		if(result==SOCKET_ERROR)
		{
			AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
		}
		else
		{
			ulFlagCount+=ulFileRead;
		}
	}

	fclose(infile);
	//�����ļ�������
	ZeroMemory(&sendPack,sizeof(sendPack));
	sendPack.nPackLen=sizeof(sendPack);
	CString strTmp=_T("send_file_end");
	int n=strTmp.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
	sendPack.sContent[len]='\0';
	sendPack.nContentLen=len;
	sendPack.iType=2;
	result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
	if(result==SOCKET_ERROR)
	{
		AfxMessageBox(_T("�����ļ�������ʧ�ܣ�"));
	}
}
//�ݹ��ƶ�Ŀ¼
bool CServerContrl::RecursiveMove(CString srcPath,CString desPath)
{
	if(srcPath == _T(""))
		return false;
	
	CreateDirectory(desPath,NULL);

	wchar_t wcPath[MAX_PATH] = {0};
	wcscpy_s(wcPath,MAX_PATH,srcPath);
	wcscat_s(wcPath,MAX_PATH,_T("\\*.*"));
	CString strPath=desPath;
	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData,sizeof(WIN32_FIND_DATA));

	HANDLE hFindFile = FindFirstFile(wcPath,&FindFileData);

	if(hFindFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bContinue = true;

	while (bContinue != false)
	{
		//bIsDotsΪ���ʾ��.��..
		bool bIsDots = (wcscmp(FindFileData.cFileName,_T(".")) == 0 || wcscmp(FindFileData.cFileName,_T("..")) == 0);
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && bIsDots == false)
		{
			//��Ŀ¼,���ٽ����Ŀ¼
			wcscpy_s(wcPath,MAX_PATH,srcPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			RecursiveMove(wcPath,desPath+_T("\\")+FindFileData.cFileName);
			//Ѱ����һ�ļ�
			bContinue = FindNextFile(hFindFile,&FindFileData);
			continue;
		}

		if (bIsDots == false)
		{
			//���ļ�ɾ��֮
			wcscpy_s(wcPath,MAX_PATH,srcPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			MoveFile(wcPath,desPath+_T("\\")+FindFileData.cFileName);
		}
		//Ѱ����һ�ļ�
		bContinue = FindNextFile(hFindFile,&FindFileData);

	}

	FindClose(hFindFile);

	//ɾ����Ŀ¼
	RemoveDirectory(srcPath);
	return true;
}
//�ݹ�Ŀ¼�����ļ���
void CServerContrl::RecursiveSend(SOCKET cSocket,CString szPath,CString rootPath)
{
	CFileFind ff;  
	CString path = szPath;  
	int result;
	DataPackage sendPack;

	if(path.Right(1) != _T("\\"))  
		path += _T("\\");  


	path += _T("*.*");  
	BOOL res = ff.FindFile(path);  

	while(res)  
	{  
		res = ff.FindNextFile();  
		//���ļ�ʱֱ�ӷ����ļ�
		if (!ff.IsDots() && !ff.IsDirectory())  
		{
			ZeroMemory(&sendPack,sizeof(sendPack));
			sendPack.iType=2; //�����ļ�
			sendPack.nPackLen=sizeof(sendPack);
			CString strTmp=ReturnCientPath(ff.GetFilePath(),rootPath);
			int n=strTmp.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			//������͵����ļ����ȷ���һ��ȷ�ϰ�
			result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("�����ļ�������ʧ�ܣ�"));
			}
			SendFile(cSocket,ff.GetFilePath());
		}
		else if (ff.IsDots())  
			continue;  
		else if (ff.IsDirectory())  
		{  
			//����Ŀ¼������Ϣ
			ZeroMemory(&sendPack,sizeof(sendPack));
			path = ff.GetFilePath(); 
			CString strTmp=ReturnCientPath(ff.GetFilePath(),rootPath);
			int n=strTmp.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			sendPack.iType=1;
			sendPack.nPackLen=sizeof(sendPack);
			result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);

			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("�����ļ�������ʧ�ܣ�"));
			}
			//��Ŀ¼ʱ�����ݹ飬���͸�Ŀ¼�µ��ļ�  
			RecursiveSend(cSocket,path,rootPath);  
 
		}  
	}  

	ff.Close();

}

//����������·��Ϊ�ͻ���·��
CString CServerContrl::ReturnCientPath(CString servPath,CString rootPath)
{
	int pos=rootPath.GetLength();
	return servPath.Right(servPath.GetLength()-pos-1);
}
//�ݹ�ɾ���ļ���
bool CServerContrl::RecursiveDelete(CString szPath)  
{  
	if(szPath == _T(""))
		return false;

	wchar_t wcPath[MAX_PATH] = {0};
	wcscpy_s(wcPath,MAX_PATH,szPath);
	wcscat_s(wcPath,MAX_PATH,_T("\\*.*"));
	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData,sizeof(WIN32_FIND_DATA));

	HANDLE hFindFile = FindFirstFile(wcPath,&FindFileData);

	if(hFindFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bContinue = true;

	while (bContinue != false)
	{
		//bIsDotsΪ���ʾ��.��..
		bool bIsDots = (wcscmp(FindFileData.cFileName,_T(".")) == 0 || wcscmp(FindFileData.cFileName,_T("..")) == 0);
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && bIsDots == false)
		{
			//��Ŀ¼,���ٽ����Ŀ¼
			wcscpy_s(wcPath,MAX_PATH,szPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			RecursiveDelete(wcPath);
			//Ѱ����һ�ļ�
			bContinue = FindNextFile(hFindFile,&FindFileData);
			continue;
		}

		if (bIsDots == false)
		{
			//���ļ�ɾ��֮
			wcscpy_s(wcPath,MAX_PATH,szPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			DeleteFile(wcPath);
		}
		//Ѱ����һ�ļ�
		bContinue = FindNextFile(hFindFile,&FindFileData);

	}

	FindClose(hFindFile);

	//ɾ����Ŀ¼
	RemoveDirectory(szPath);
	return true;
}  
//�жϽ������ӵ��û��Ƿ����
int CServerContrl::UserLogin(CString Userinfo)
{
	CString userName,userPwd;
	int iPos=Userinfo.Find(_T(':'));
	userName=Userinfo.Left(iPos);
	userPwd=Userinfo.Right(Userinfo.GetLength()-iPos-1);

	//�������ݿ�
	CDatabase db;
	CString tmpSql,numstr;
	try
	{

		db.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
		if(!db.IsOpen())
		{
			AfxMessageBox(_T("���ݿ��ʧ�ܣ�"));
			return OTHER_ERROR;
		}
		tmpSql.Format(_T("select * from netdisk_user_tb"));
		CRecordset rs(&db);
		rs.Open(CRecordset::snapshot,tmpSql);
		if(rs.IsBOF())
			return OTHER_ERROR;

		while(!rs.IsEOF())
		{
			rs.GetFieldValue((short)2,numstr);
			if(numstr == userName)
			{
				rs.GetFieldValue((short)3,numstr);
				if(numstr != userPwd)
					return PASSWD_ERROR;
				else
					return LOGIN_SUCCESS;
			}
			rs.MoveNext();
		}

		if(rs.IsEOF())
			return USERNAME_NOTEXIST;
		/*tmpSql.Format(_T("select count(*) as num from netdisk_user_tb where name='%s'"),userName);
		CRecordset rs(&db);
		rs.Open(CRecordset::snapshot,tmpSql);
		if(rs.IsBOF())
		return OTHER_ERROR;
		rs.GetFieldValue((short)0,numstr);
		if(numstr!=_T("0"))
		{

		CString pwdStr;
		rs.GetFieldValue((short)4,numstr);
		pwdStr=numstr;
		if(pwdStr !=userPwd)
		return PASSWD_ERROR;
		else
		return LOGIN_SUCCESS;
		}
		else
		{
		return USERNAME_NOTEXIST;
		}*/
		rs.Close();

	}
	catch(CDBException* pe)
	{
		CString tmp;
		tmp.Format(_T("���ݿ��������%s"),pe->m_strError);
		AfxMessageBox(tmp);
		pe->Delete();
	}
	db.Close();
	return OTHER_ERROR;
}

void CServerContrl::StopServ()
{
	m_bThreadCtrl=TRUE;
	closesocket(m_sServer);
	WSACleanup();
}
// CServerContrl member functions
