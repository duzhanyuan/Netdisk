#pragma once
#define DEFAULT_ADDR "127.0.0.1"
#define DEFAULT_PORT 12345

typedef struct _PER_SOCKET_CONTEXT
{
	SOCKET socket;
	SOCKADDR_IN clientAddr;
}PER_SOCKET_CONTEXT,*PER_SOCKET_CONTEXT;

typedef struct _WORKER_THREAD
{
	NetDiskIOCP* ndIocp;
	int threadNo;
}PER_WORK_THREAD,*PPER_WORK_THREAD;

class NetDiskIOCP
{
public:
	HANDLE m_IoCompletionPort;
	PER_SOCKET_CONTEXT m_Listen;
	SOCKET m_sAccept;
	SOCKADDR_IN m_ServAddr;
	HANDLE* m_pWorkerThread;
public:
	NetDiskIOCP(void);
	~NetDiskIOCP(void);
public:
	void CleanSocket(SOCKET &socket);
public:
	//初始化socket环境
	bool InitWinSocket();

	//初始化IOCP
	bool InitIOCP();

	//初始化套接字
	bool InitServSocket();
};

