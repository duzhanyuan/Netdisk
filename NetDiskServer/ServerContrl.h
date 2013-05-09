#pragma once
// winsock 2 ��ͷ�ļ��Ϳ�
#include <winsock2.h>
#include <MSWSock.h>

#pragma comment(lib,"ws2_32.lib")

#define SERV_PORT 12345
#define MAX_BUFFER_LEN 1024*8
// CServerContrl command target
//�û����Ӳ���
typedef enum _USER_OPR_TYPE
{
	LOGIN=1,							//�û���¼
	UPLOADFILE,							//�ϴ��ļ�
	UPLOADCATLOG,						//�ϴ��ļ���
	NEWFLODER,							//�½��ļ���
	DELETEFILE,							//ɾ��
	DOWNLOADFILE,						//�����ļ�
	DOWNLOADCATALOG,					//����Ŀ¼
	MOVEFILE,							//�ƶ�
	REFRESH,							//ˢ��
	HISTROYVERSION,						//��ʷ�汾
	HIS_RESTORE,						//��ԭ��ʷ��¼�汾
	HIS_DELETE,							//ɾ����ʷ��¼�汾
	HIS_CLEAR,							//�����ʷ��¼�汾
	UPDATECLIENT,						//���¿ͻ�����ʾ
	GETCATALOGINFO,						//��ȡĿ¼�µ��ļ����ļ����б�
	RECYCLE_RESTORE,					//����վ��ԭ����
	RECYCLE_DEL,						//����վɾ������
	RECYCLE_CLEAN,						//����վ��ղ���
	RECYCLE_FIND,						//����վ���Ҳ���
	RECYCLE_UPDATE,						//���»���վ�б�
	GETFILESIZEINFO,					//��ȡ�����ļ���С
	FINDFILEBYSTR,						//�����ļ�
	FINDFILEBYTIME						//��ʱ������ļ�
}USER_OPR_TYPE;
//�û���¼��������
typedef enum _USERLOGIN_TYPE
{
	USERNAME_NOTEXIST,					//�û���������
	PASSWD_ERROR,						//�������
	LOGIN_SUCCESS,						//��¼�ɹ�
	OTHER_ERROR							//��������			
}USERLOGIN_TYPE;
//�ļ���Ϣ
//typedef struct _FileInfor
//{
//	char filepath[MAX_BUFFER_LEN];
//	char filename[MAX_BUFFER_LEN];
//};
//���͵����ݰ�
typedef struct _DataPackage  
{
	int iType;        //
	int nPackLen;
	char sContent[ MAX_BUFFER_LEN ];            //���ݰ�������
	u_long nPosition;                //�������ļ��е�λ��
	int nContentLen;                //�����ֽ���
	//_FileInfor    FileInfor;        //�ļ���Ϣ
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
	CRITICAL_SECTION m_csFile;	//�ٽ�������
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


