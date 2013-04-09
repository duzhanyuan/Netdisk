#include "StdAfx.h"
#include "Client.h"


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

	// 生成SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		TRACE("错误：初始化Socket失败，错误信息：%d\n", WSAGetLastError());
		return false;
	}

	char* pAddr;
	int len = Serveraddr.GetLength(); 
	pAddr = (char*)malloc((len*2+1)*sizeof(char));
	//CString的长度中汉字算一个长度 
	memset(pAddr, 0, 2*len+1); 
	USES_CONVERSION; 
	strcpy((LPSTR)pAddr,OLE2A(Serveraddr.LockBuffer()));

	// 生成地址信息
	Server = gethostbyname(pAddr);
	if (Server == NULL) 
	{
		closesocket(*pSocket);
		TRACE("错误：无效的服务器地址.\n");
		return false; 
	}


	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, 
		(char *)Server->h_addr,
		Server->h_length);

	ServerAddress.sin_port = htons(port);

	// 开始连接服务器
	if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		TRACE("错误：连接至服务器失败！\n");
		return false; 
	}

	return true;
}

// 初始化WinSock 2.2
bool Client::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		AfxMessageBox (_T("初始化WinSock 2.2失败！\n"));
		return false; // 错误
	}

	return true;
}

bool Client::Start()
{
	if(!LoadSocketLib())
	{
		AfxMessageBox(_T("加载socket失败！"));
		return false;
	}

	if(!ConnectToServ(&m_Client.sock,_T("127.0.0.1"),12345))
	{
		AfxMessageBox(_T("连接服务器失败！"));
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
	// 向服务器发送信息
	int len = strMsg.GetLength(); 
	//szTemp = (char*)malloc((len*2+1)*sizeof(char));
	//CString的长度中汉字算一个长度 
	memset(szTemp, 0, 2*len+1); 
	USES_CONVERSION; 
	strcpy((LPSTR)szTemp,OLE2A(strMsg.LockBuffer()));

	//sprintf( szTemp,("第1条信息：%s"),strMsg );
	nBytesSend = send(m_Client.sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSend) 
	{
		AfxMessageBox(_T("发送失败，错误代码为 %s"),GetLastError());
		return ; 
	}	

}

int Client::RecvMsg()
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

void Client::Clean()
{
	if(m_Client.sock != NULL)
	{
		closesocket(m_Client.sock);
		m_Client.sock=NULL;
	}
}