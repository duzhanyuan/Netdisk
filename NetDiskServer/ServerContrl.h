#pragma once
// winsock 2 的头文件和库
#include <winsock2.h>
#include <MSWSock.h>

#pragma comment(lib,"ws2_32.lib")

#define SERV_PORT 12345
#define MAX_BUFFER_LEN 1024*8
// CServerContrl command target
//用户连接操作
typedef enum _USER_OPR_TYPE
{
	LOGIN=1,								//用户登录
	UPLOADFILE,							//上传文件
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
//用户登录操作类型
typedef enum _USERLOGIN_TYPE
{
	USERNAME_NOTEXIST,					//用户名不存在
	PASSWD_ERROR,						//密码错误
	LOGIN_SUCCESS,						//登录成功
	OTHER_ERROR							//其他错误			
}USERLOGIN_TYPE;
//发送的数据包
typedef struct _DataPackage  
{
	int iType;        //
	int nPackLen;
	char sContent[ MAX_BUFFER_LEN ];            //数据包缓冲区
	u_long nPosition;                //数据在文件中的位置
	int nContentLen;                //数据字节数
	//_FileInfor    FileInfor;        //文件信息
}DataPackage;

class CServerContrl : public CObject
{
public:
	SOCKET			m_sServer;
	fd_set			m_fdAllSocket;
	fd_set			m_fdRead;
	fd_set			m_fd_Write;
	CString			m_StrIndexInfo;
	HANDLE			m_hServCtrl;
	BOOL			m_bThreadCtrl;
	CString			m_strUserName;
public:
	bool InitSocket();

	bool StartServ();
	void StopServ();

	int UserLogin(CString Userinfo);

	static DWORD WINAPI ServListenThread(LPVOID lpParam);
	CServerContrl();
	virtual ~CServerContrl();
};


