#pragma once
// 屏蔽deprecation警告
#pragma warning(disable: 4996)

// 缓冲区长度(8*1024字节)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          12345                      // 默认端口
#define DEFAULT_IP            _T("127.0.0.1")            // 默认IP地址

class CClient;

// 用于发送数据的线程参数
typedef struct _tagThreadParams_WORKER
{
	CClient* pClient;                               // 类指针，用于调用类中的函数
	SOCKET   sock;                                  // 每个线程使用的Socket
	int      nThreadNo;                             // 线程编号
	char     szBuffer[MAX_BUFFER_LEN];

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 

typedef enum _USERLOGIN_TYPE
{
	USERNAME_NOTEXIST,					//用户名不存在
	PASSWD_ERROR,						//密码错误
	LOGIN_SUCCESS,						//登录成功
	OTHER_ERROR							//其他错误			
}USERLOGIN_TYPE;

class Client
{
public:
	THREADPARAMS_WORKER m_Client;
public:
	Client(void);
	~Client(void);
	
	bool ConnectToServ(SOCKET *pSocket,CString addr,int port);
	bool LoadSocketLib();
	bool Start();
	void Stop();
	void SendMsgToServ(CString strMsg);
	int RecvMsg();
	void Clean();
};

