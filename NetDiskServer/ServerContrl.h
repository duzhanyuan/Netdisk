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
	LOGIN=1,								//�û���¼
	UPLOADFILE,							//�ϴ��ļ�
	UPLOADCATLOG,						//�ϴ��ļ���
	NEWFLODER,							//�½��ļ���
	DELETEFILE,							//ɾ��
	DOWNLOAD,							//����
	MOVEFILE,							//�ƶ�
	REFRESH,							//ˢ��
	HISTROYVERSION,						//��ʷ�汾
	UPDATECLIENT,						//���¿ͻ�����ʾ
	GETCATALOGINFO						//��ȡĿ¼�µ��ļ����ļ����б�
}USER_OPR_TYPE;
//�û���¼��������
typedef enum _USERLOGIN_TYPE
{
	USERNAME_NOTEXIST,					//�û���������
	PASSWD_ERROR,						//�������
	LOGIN_SUCCESS,						//��¼�ɹ�
	OTHER_ERROR							//��������			
}USERLOGIN_TYPE;
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
public:
	bool InitSocket();

	bool StartServ();
	void StopServ();

	int UserLogin(CString Userinfo);

	static DWORD WINAPI ServListenThread(LPVOID lpParam);
	CServerContrl();
	virtual ~CServerContrl();
};


