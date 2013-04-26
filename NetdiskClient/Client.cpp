#include "StdAfx.h"
#include "Client.h"
#include "locale.h"
#include "NetdiskClient.h"
#include "NetdiskClientDlg.h"

Client::Client(void)
{
	m_pMainApp=(CNetdiskClientApp*)AfxGetApp() ;
}


Client::~Client(void)
{
}

bool Client::ConnectToServ(SOCKET *pSocket,CString Serveraddr,int port)
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	// ����SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		TRACE("���󣺳�ʼ��Socketʧ�ܣ�������Ϣ��%d\n", WSAGetLastError());
		return false;
	}

	//��ȡϵͳĬ�ϵķ������ݻ�������С
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	int nErrCode= getsockopt(m_Client.sock, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//����ϵͳ�������ݻ�����ΪĬ��ֵ��BUF_TIMES��
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_Client.sock, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
	}


	//�������ϵͳ�������ݻ������Ƿ�ɹ�
	unsigned int uiNewRcvBuf;
	getsockopt(m_Client.sock, SOL_SOCKET, SO_SNDBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));;
	}

	char* pAddr;
	int len = Serveraddr.GetLength(); 
	pAddr = (char*)malloc((len*2+1)*sizeof(char));
	//CString�ĳ����к�����һ������ 
	memset(pAddr, 0, 2*len+1); 
	USES_CONVERSION; 
	strcpy((LPSTR)pAddr,OLE2A(Serveraddr.LockBuffer()));

	// ���ɵ�ַ��Ϣ
	Server = gethostbyname(pAddr);
	if (Server == NULL) 
	{
		closesocket(*pSocket);
		TRACE("������Ч�ķ�������ַ.\n");
		return false; 
	}


	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, 
		(char *)Server->h_addr,
		Server->h_length);

	ServerAddress.sin_port = htons(port);

	// ��ʼ���ӷ�����
	if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		TRACE("����������������ʧ�ܣ�\n");
		return false; 
	}

	return true;
}

// ��ʼ��WinSock 2.2
bool Client::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		AfxMessageBox (_T("��ʼ��WinSock 2.2ʧ�ܣ�\n"));
		return false; // ����
	}

	return true;
}

bool Client::Start()
{
	if(!LoadSocketLib())
	{
		AfxMessageBox(_T("����socketʧ�ܣ�"));
		return false;
	}

	if(!ConnectToServ(&m_Client.sock,_T("127.0.0.1"),12345))
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
		return false;
	}

	return true;
}

void Client::Stop()
{
	if(m_Client.sock!=NULL)
	{
		closesocket(m_Client.sock);
		m_Client.sock=NULL;
	}

}

void Client::SendMsgToServ(CString strMsg,int sendType)
{
	/*char szTemp[MAX_BUFFER_LEN];*/
	//memset( szTemp,0,sizeof(szTemp) );
	DataPackage sendPack;
	ZeroMemory(&sendPack,sizeof(sendPack));
	
	int nBytesSend=0;
	// �������������Ϣ
	int n=strMsg.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,strMsg,strMsg.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,strMsg,strMsg.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
	sendPack.sContent[len]='\0';
	sendPack.nContentLen=len;
	sendPack.nPackLen=sizeof(sendPack);
	sendPack.iType=sendType;
	
	nBytesSend = send(m_Client.sock, (char*)&sendPack, sendPack.nPackLen, 0);
//	Sleep(10);
	if (SOCKET_ERROR == nBytesSend) 
	{
		CString tmpStr;
		tmpStr.Format(_T("����ʧ�ܣ��������Ϊ:%d"),WSAGetLastError());
		AfxMessageBox(tmpStr);
		return ; 
	}	

}

//���շ�����������Ϣ
int Client::RecvloginMsg()
{
	int iResult;
	//char recvbuf[MAX_BUFFER_LEN];
	//int recvbuflen=MAX_BUFFER_LEN;
	DataPackage recvPack;
	recvPack.nPackLen=sizeof(recvPack);

	iResult=recv(m_Client.sock,(char*)&recvPack,recvPack.nPackLen,0);
	if(0 == iResult)
	{
		AfxMessageBox(_T("Connection closed\n"));
		return -1;
	}
	if(iResult > 0)
	{
		return atoi(recvPack.sContent);
	}
	else
	{
		AfxMessageBox(_T("recv failed: %d\n"), WSAGetLastError());
		return -1;
	}

}

//����׽���
void Client::Clean()
{
	if(m_Client.sock != NULL)
	{
		closesocket(m_Client.sock);
		m_Client.sock=NULL;
	}
}

//���¿ͻ���Ŀ¼��Ϣ
bool Client::UpdateClientCatalog()
{
	//���û�Ŀ¼�ĸ�Ŀ¼�����͸�����������Ŀ¼����Ϊ�û���¼��
	SendMsgToServ(m_pMainApp->m_loginName,UPDATECLIENT);
	//��ȡ������Ϣ
	if(!RecvReturnMsg())
	{
		AfxMessageBox(_T("��ȡ������Ϣʧ�ܣ�"));
		return false;
	}
	return true;
}

//����Ŀ¼·���������������������ظ�Ŀ¼���б���Ϣ
bool Client::GetCatalogInfo(CString FloderName)
{
	//����Ŀ¼·���������� 
//	CString sendMsgStr;
	//sendMsgStr.Format(_T("%d+%s"),GETCATALOGINFO,FloderName);
	SendMsgToServ(FloderName,GETCATALOGINFO);

	//��ȡ������Ϣ
	if(!RecvReturnMsg())
	{
		AfxMessageBox(_T("��ȡ������Ϣʧ�ܣ�"));
		return false;
	}
	return true;
}

//���ʹ����ļ��е���Ϣ
bool Client::CreateNewFloder(CString baseFloder)
{
	//���Ͳ�����Ϣ��������
	SendMsgToServ(baseFloder,NEWFLODER);
	//��ȡ������Ϣ
	//if(!RecvReturnMsg())
	//{
	//	AfxMessageBox(_T("��ȡ������Ϣʧ�ܣ�"));
	//	return false;
	//}
	return true;
}

//����ɾ���ļ���Ϣ
bool Client::DeleteClientFile(CString path)
{
	//���Ͳ�����Ϣ��������
	SendMsgToServ(path,DELETEFILE);
	return true;
}
//���������ļ���Ϣ
bool Client::DownloadFile(CString path)
{
	SendMsgToServ(path,DELETEFILE);
	return true;
}

bool Client::RecvReturnMsg()
{
	//��ȡ�û����ص�Ŀ¼��Ϣ
	//CString catalogIndexInfo;
	int iResult;
	//char recvbuf[MAX_BUFFER_LEN];
	//int recvbuflen=MAX_BUFFER_LEN;
	DataPackage recvPack;
	ZeroMemory(&recvPack,sizeof(recvPack));
	
	//CString m_StrIndexInfo;
	//do 
	//{
		iResult=recv(m_Client.sock,(char*)&recvPack,sizeof(recvPack),0);
		Sleep(10);
		if(0 == iResult)
		{
			AfxMessageBox(_T("Connection closed\n"));
			return false;
		}
		//�ɹ���ȡ����
		if(iResult > 0)
		{
			((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo=recvPack.sContent;
			return true;
		}
		else
		{
			CString msgStr;
			msgStr.Format(_T("recv failed,������룺 %d\n"), WSAGetLastError());
			AfxMessageBox(msgStr);
			return false;
		}
}

//ˢ�¿ͻ�����Ϣ
bool Client::UpdateClient(CString path)
{
	SendMsgToServ(path,REFRESH);
	return true;
}

//�ƶ��ļ��л����ļ�
bool Client::MoveClientFile(CString path)
{
	SendMsgToServ(path,MOVEFILE);
	return true;
}