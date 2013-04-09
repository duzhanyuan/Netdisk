#include "StdAfx.h"
#include "NetDiskIOCP.h"


NetDiskIOCP::NetDiskIOCP(void)
{
}


NetDiskIOCP::~NetDiskIOCP(void)
{
}

//初始化winsocket环境
bool NetDiskIOCP::InitWinSocket()
{
	WSADATA wsadata;
	int relt;
	relt=WSAStartup(MAKEWORD(2,2),&wsadata);

	if(NO_ERROR != relt)
	{
		AfxMessageBox(_T("初始化winsocket 失败！"));
		return false;
	}

	return true;
}

//初始化IOCP
bool NetDiskIOCP::InitIOCP()
{
	//创建完成端口
	m_IoCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

	if(NULL == m_IoCompletionPort)
	{
		AfxMessageBox(_T("创建完成端口失败！"));
		return false;
	}

	//设置工作线程
	SYSTEM_INFO sy;
	GetSystemInfo(&sy);
	int nThreadNum=sy.dwNumberOfProcessors * 2 +2;
	m_pWorkerThread = new HANDLE[nThreadNum];
	DWORD ThreadID;

	for(int i=0;i<nThreadNum;i++)
	{
		PER_WORK_THREAD* pwThread=new PER_WORK_THREAD;
		pwThread->ndIocp = this;
		pwThread->threadNo = i+1;

		m_pWorkerThread[i]=::CreateThread(0,0,ServerWorkThread,(void*)pwThread,0,&ThreadID);
	}
	return true;
}
DWORD WINAPI ServerWorkThread(LPVOID lpPrama)
{

}
//初始化服务器socket
bool NetDiskIOCP::InitServSocket()
{
	m_Listen->socket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_Listen->socket)
	{
		AfxMessageBox(_T("WSASocket() 失败！"));
		WSACleanup();
		return false;
	}

	//绑定监听socket至完成端口
	if(NULL == CreateIoCompletionPort((HANDLE)m_Listen->socket,m_IoCompletionPort,(ULONG_PTR)m_Listen,0))
	{
		AfxMessageBox(_T("绑定完成端口失败！"));
		CleanSocket(m_Listen->socket);
		return false;
	}

	//设置服务器地址信息并绑定改地址和端口
	m_ServAddr.sin_family = AF_INET;
	m_ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServAddr.sin_port = htons(DEFAULT_PORT);

	if(SOCKET_ERROR == bind(m_Listen->socket,(const sockaddr*)&m_ServAddr,sizeof(m_ServAddr)))
	{
		AfxMessageBox(_T("绑定地址端口错误！"));
		return false;
	}

	//设置socket监听
	if(SOCKET_ERROR == listen(m_Listen->socket,5))
	{
		AfxMessageBox(_T("设置socket监听失败！"));
		return false;
	}

	return true;
}

//清空socket
void NetDiskIOCP::CleanSocket(SOCKET &socket)
{
	if(NULL != socket)
	{
		closesocket(socket);
		socket=NULL;
	}
}