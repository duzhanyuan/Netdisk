#include "StdAfx.h"
#include "Client.h"
#include "locale.h"
#include "NetdiskClient.h"

Client::Client(void)
{
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

void Client::SendMsgToServ(CString strMsg)
{
	char szTemp[MAX_BUFFER_LEN];
	//memset( szTemp,0,sizeof(szTemp) );
	int nBytesSend=0;
	// �������������Ϣ
	int len = strMsg.GetLength(); 
	//szTemp = (char*)malloc((len*2+1)*sizeof(char));
	//CString�ĳ����к�����һ������ 
	memset(szTemp, 0, 2*len+1); 
	USES_CONVERSION; 
	strcpy((LPSTR)szTemp,OLE2A(strMsg.LockBuffer()));

	//sprintf( szTemp,("��1����Ϣ��%s"),strMsg );
	nBytesSend = send(m_Client.sock, szTemp, strlen(szTemp), 0);
	Sleep(100);
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
	char recvbuf[MAX_BUFFER_LEN];
	int recvbuflen=MAX_BUFFER_LEN;
	iResult=recv(m_Client.sock,recvbuf,recvbuflen,0);
	if(0 == iResult)
	{
		AfxMessageBox(_T("Connection closed\n"));
		return -1;
	}
	if(iResult > 0)
	{
		
		return atoi(recvbuf);
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
bool Client::UpdateClientCatalog(CString baseFolder)
{
	//���û�Ŀ¼�ĸ�Ŀ¼�����͸�����������Ŀ¼����Ϊ�û���¼��
	CString sendMsgStr;
	sendMsgStr.Format(_T("%d+%s"),UPDATECLIENT,baseFolder);
	SendMsgToServ(sendMsgStr);

	//��ȡ�û����ص�Ŀ¼��Ϣ
	CString catalogIndexInfo;
	int iResult;
	char recvbuf[MAX_BUFFER_LEN];
	int recvbuflen=MAX_BUFFER_LEN;
	//CString m_StrIndexInfo;
	//do 
	//{
		iResult=recv(m_Client.sock,recvbuf,recvbuflen,0);
		Sleep(100);
		if(0 == iResult)
		{
			AfxMessageBox(_T("Connection closed\n"));
			return false;
		}
		//�ɹ���ȡ����
		if(iResult > 0)
		{
			recvbuf[iResult]='\0';
			//m_StrIndexInfo=recvbuf;
			//m_pMainDlg=(CNetdiskClientDlg*)(AfxGetMainWnd()->m_hWnd);
			//m_pMainDlg->m_strIndexInfo=recvbuf;
			((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo=recvbuf;
			return true;
		}
		else
		{
			AfxMessageBox(_T("recv failed: %d\n"), WSAGetLastError());
			return false;
		}
	//} while (iResult>0);

		//setlocale( LC_CTYPE, ("chs"));
		//CStdioFile sfile;
		//sfile.Open(_T("C:\\test.txt"),CFile::modeWrite|CFile::modeCreate);
		//sfile.WriteString(m_StrIndexInfo);
		//sfile.Close();
}

//����Ŀ¼·���������������������ظ�Ŀ¼���б���Ϣ
bool Client::GetCatalogInfo(CString FloderName)
{
	//����Ŀ¼·���������� 
	CString sendMsgStr;
	sendMsgStr.Format(_T("%d+%s"),GETCATALOGINFO,FloderName);
	SendMsgToServ(sendMsgStr);

	//��ȡ�û����ص�Ŀ¼��Ϣ
	CString catalogIndexInfo;
	int iResult;
	char recvbuf[MAX_BUFFER_LEN];
	int recvbuflen=MAX_BUFFER_LEN;

	iResult=recv(m_Client.sock,recvbuf,recvbuflen,0);

	if(0 == iResult)
	{
		AfxMessageBox(_T("Connection closed\n"));
		return false;
	}
	//�ɹ���ȡ����
	if(iResult > 0)
	{
		recvbuf[iResult]='\0';
		//m_StrIndexInfo=recvbuf;
		//m_pMainDlg=(CNetdiskClientDlg*)(AfxGetMainWnd()->m_hWnd);
		//m_pMainDlg->m_strIndexInfo=recvbuf;
		((CNetdiskClientApp*)AfxGetApp())->m_strIndexInfo=recvbuf;
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