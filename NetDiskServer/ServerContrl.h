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
	LOGIN=1,							//用户登录
	UPLOADFILE,							//上传文件
	UPLOADCATLOG,						//上传文件夹
	NEWFLODER,							//新建文件夹
	DELETEFILE,							//删除
	DOWNLOADFILE,						//下载文件
	DOWNLOADCATALOG,					//下载目录
	MOVEFILE,							//移动
	REFRESH,							//刷新
	HISTROYVERSION,						//历史版本
	HIS_RESTORE,						//还原历史记录版本
	HIS_DELETE,							//删除历史记录版本
	HIS_CLEAR,							//清空历史记录版本
	UPDATECLIENT,						//更新客户端显示
	GETCATALOGINFO,						//获取目录下的文件和文件夹列表
	RECYCLE_RESTORE,					//回收站还原操作
	RECYCLE_DEL,						//回收站删除操作
	RECYCLE_CLEAN,						//回收站清空操作
	RECYCLE_FIND,						//回收站查找操作
	RECYCLE_UPDATE,						//更新回收站列表
	GETFILESIZEINFO,					//获取下载文件大小
	FINDFILEBYSTR,						//查找文件
	FINDFILEBYTIME						//按时间查找文件
}USER_OPR_TYPE;
//用户登录操作类型
typedef enum _USERLOGIN_TYPE
{
	USERNAME_NOTEXIST,					//用户名不存在
	PASSWD_ERROR,						//密码错误
	LOGIN_SUCCESS,						//登录成功
	OTHER_ERROR							//其他错误			
}USERLOGIN_TYPE;
//文件信息
//typedef struct _FileInfor
//{
//	char filepath[MAX_BUFFER_LEN];
//	char filename[MAX_BUFFER_LEN];
//};
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
	CString			m_strDiskRootPath;
	CString			m_strDiskIndexPath;
	CString			m_strRecyclePath;
	CString			m_strRecycleIndexPath;
	CString			m_strSharePath;
	CString			m_strShareIndexPath;
	CRITICAL_SECTION m_csFile;	//临界区对象
	u_long			m_fileTotalSize;
	CString			m_strShareFloder;
	CString			m_strHisVerRootPath;
public:
	bool InitSocket();

	bool StartServ();
	void StopServ();

	int	 UserLogin(CString Userinfo);
	bool RecursiveDelete(CString szPath);
	bool RecursiveMove(CString srcPath,CString desPath);
	void RecursiveSend(SOCKET cSocket,CString szPath,CString rootPath) ;
	void SendFile(SOCKET cSocket,CString filePath);
	CString ReturnCientPath(CString servPath,CString rootPath);
	bool SendDataInfo(SOCKET cSocket,CString strIndexInfo,int dataType=0);
	static DWORD WINAPI ServListenThread(LPVOID lpParam);
	void GetDownLoadFileSize(CString path);
	bool IfFloderExist(CString path);
	CServerContrl();
	virtual ~CServerContrl();
};


