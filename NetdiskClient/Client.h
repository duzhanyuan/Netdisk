#pragma once
// ����deprecation����
#pragma warning(disable: 4996)

// ����������(8*1024�ֽ�)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          12345                      // Ĭ�϶˿�
#define DEFAULT_IP            _T("127.0.0.1")            // Ĭ��IP��ַ
class CNetdiskClientDlg;
class Client;

// ���ڷ������ݵ��̲߳���
typedef struct _tagThreadParams_WORKER
{
	Client* pClient;                               // ��ָ�룬���ڵ������еĺ���
	SOCKET   sock;                                  // ÿ���߳�ʹ�õ�Socket
	int      nThreadNo;                             // �̱߳��
	char     szBuffer[MAX_BUFFER_LEN];

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 


typedef enum _USERLOGIN_TYPE
{
	USERNAME_NOTEXIST,					//�û���������
	PASSWD_ERROR,						//�������
	LOGIN_SUCCESS,						//��¼�ɹ�
	OTHER_ERROR							//��������			
}USERLOGIN_TYPE;

typedef enum _USER_OPR_TYPE
{
	UPLOADFILE=4,						//�ϴ��ļ�
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

typedef enum _SERV_SEND_TYPE
{
	EMPTYCATALOG=14,					//���ͷǿ�Ŀ¼��Ϣ
	NONEMPTYCATALOG						//���Ϳ�Ŀ¼��Ϣ
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
	void SendMsgToServ(CString strMsg); //������Ϣ
	int RecvloginMsg();						//��ȡ��¼��֤��Ϣ
	void Clean();
public:
	bool UpdateClient();				//���¿ͻ���Ŀ¼�ļ�����
	bool UpdateClientCatalog(CString baseFolder);			//���¿ͻ���Ŀ¼��Ϣ
	bool GetCatalogInfo(CString FloderName);						//��ȡĿ¼��Ϣ
	bool UploadClientFile();					//�ϴ��ļ�
	bool DeleteClientFile();					//ɾ���ļ�
	bool MoveClientFile();						//�ƶ��ļ�
	bool DownloadFile();						//�����ļ�
	bool FindFile();							//�����ļ�
public:
public:
	//CString m_strClientName;				//�û���¼��
	CNetdiskClientDlg*			m_pMainDlg;
};

