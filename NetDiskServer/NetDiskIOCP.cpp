#include "StdAfx.h"
#include "NetDiskIOCP.h"


NetDiskIOCP::NetDiskIOCP(void)
{
}


NetDiskIOCP::~NetDiskIOCP(void)
{
}

//��ʼ��winsocket����
bool NetDiskIOCP::InitWinSocket()
{
	WSADATA wsadata;
	int relt;
	relt=WSAStartup(MAKEWORD(2,2),&wsadata);

	if(NO_ERROR != relt)
	{
		AfxMessageBox(_T("��ʼ��winsocket ʧ�ܣ�"));
		return false;
	}

	return true;
}

//��ʼ��IOCP
bool NetDiskIOCP::InitIOCP()
{
	//������ɶ˿�
	m_IoCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);

	if(NULL == m_IoCompletionPort)
	{
		AfxMessageBox(_T("������ɶ˿�ʧ�ܣ�"));
		return false;
	}

	//���ù����߳�
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
//��ʼ��������socket
bool NetDiskIOCP::InitServSocket()
{
	m_Listen->socket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_Listen->socket)
	{
		AfxMessageBox(_T("WSASocket() ʧ�ܣ�"));
		WSACleanup();
		return false;
	}

	//�󶨼���socket����ɶ˿�
	if(NULL == CreateIoCompletionPort((HANDLE)m_Listen->socket,m_IoCompletionPort,(ULONG_PTR)m_Listen,0))
	{
		AfxMessageBox(_T("����ɶ˿�ʧ�ܣ�"));
		CleanSocket(m_Listen->socket);
		return false;
	}

	//���÷�������ַ��Ϣ���󶨸ĵ�ַ�Ͷ˿�
	m_ServAddr.sin_family = AF_INET;
	m_ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServAddr.sin_port = htons(DEFAULT_PORT);

	if(SOCKET_ERROR == bind(m_Listen->socket,(const sockaddr*)&m_ServAddr,sizeof(m_ServAddr)))
	{
		AfxMessageBox(_T("�󶨵�ַ�˿ڴ���"));
		return false;
	}

	//����socket����
	if(SOCKET_ERROR == listen(m_Listen->socket,5))
	{
		AfxMessageBox(_T("����socket����ʧ�ܣ�"));
		return false;
	}

	return true;
}

//���socket
void NetDiskIOCP::CleanSocket(SOCKET &socket)
{
	if(NULL != socket)
	{
		closesocket(socket);
		socket=NULL;
	}
}