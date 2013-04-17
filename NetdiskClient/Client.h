#pragma once
// 屏蔽deprecation警告
#pragma warning(disable: 4996)

// 缓冲区长度(8*1024字节)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          12345                      // 默认端口
#define DEFAULT_IP            _T("127.0.0.1")            // 默认IP地址
class CNetdiskClientDlg;
class Client;

// 用于发送数据的线程参数
typedef struct _tagThreadParams_WORKER
{
	Client* pClient;                               // 类指针，用于调用类中的函数
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

typedef enum _USER_OPR_TYPE
{
	UPLOADFILE=4,						//上传文件
	UPLOADCATLOG,						//上传文件夹
	NEWFLODER,							//新建文件夹
	DELETEFILE,							//删除
	DOWNLOAD,							//下载
	MOVEFILE,							//移动
	REFRESH,							//刷新
	HISTROYVERSION,						//历史版本
	UPDATECLIENT,						//更新客户端显示
	GETCATALOGINFO						//获取目录下的文件和文件夹列表
}USER_OPR_TYPE;

typedef enum _SERV_SEND_TYPE
{
	EMPTYCATALOG=14,					//发送非空目录信息
	NONEMPTYCATALOG						//发送空目录信息
}SERV_SEND_TYPE;
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
	void SendMsgToServ(CString strMsg); //发送信息
	int RecvloginMsg();						//获取登录验证信息
	void Clean();
public:
	bool UpdateClient();				//更新客户端目录文件数据
	bool UpdateClientCatalog(CString baseFolder);			//更新客户端目录信息
	bool GetCatalogInfo(CString FloderName);						//获取目录信息
	bool UploadClientFile();					//上传文件
	bool DeleteClientFile();					//删除文件
	bool MoveClientFile();						//移动文件
	bool DownloadFile();						//下载文件
	bool FindFile();							//查找文件
public:
public:
	//CString m_strClientName;				//用户登录名
	CNetdiskClientDlg*			m_pMainDlg;
};

