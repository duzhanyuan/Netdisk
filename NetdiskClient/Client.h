#pragma once
// ����deprecation����
#pragma warning(disable: 4996)

// ����������(8*1024�ֽ�)
#define MAX_BUFFER_LEN 1024*8
#define	MAX_BUFFER_FILE_LEN	  1024*60
#define DEFAULT_PORT          12345                      // Ĭ�϶˿�
#define DEFAULT_IP            _T("127.0.0.1")            // Ĭ��IP��ַ
class CNetdiskClientApp;
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

typedef enum _SERV_SEND_TYPE
{
	EMPTYCATALOG=14,					//���ͷǿ�Ŀ¼��Ϣ
	NONEMPTYCATALOG						//���Ϳ�Ŀ¼��Ϣ
}SERV_SEND_TYPE;
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
	void SendMsgToServ(CString strMsg,int sendType); //������Ϣ
	int RecvloginMsg();						//��ȡ��¼��֤��Ϣ
	void Clean();
	bool RecvReturnMsg();				//��ȡ������������Ϣ
public:
	bool UpdateClient(CString path);				//���¿ͻ���Ŀ¼�ļ�����
	bool UpdateClientCatalog(CString Rootpath);			//���¿ͻ���Ŀ¼��Ϣ
	bool GetCatalogInfo(CString FloderName);						//��ȡĿ¼��Ϣ
	bool UploadClientFile();					//�ϴ��ļ�
	bool DeleteClientFile(CString path);		//ɾ���ļ�
	bool MoveClientFile(CString path);			//�ƶ��ļ�
	bool DownloadFile(CString path);			//�����ļ�
	bool FindFileByStr(CString findStr);		//�����ļ�
	bool CreateNewFloder(CString baseFloder);	//�½��ļ���
	bool RecycleDel(CString path);				//ɾ������վ
	bool RecycleRestore(CString path);			//��ԭ����վ
	bool RecycleClean(CString path);			//��ջ���վ
	bool UpdateRecycleList(CString path);		//���»���վ�б�
	bool RecycleFindFile(CString findStr);		//���һ���վ�ļ�
	bool GetFileSizeFromServ(CString path);		//��ȡ�����ļ���С
	bool GetHistroyVersionInfo(CString strInfo);//��ȡ��ʷ�汾��Ϣ
	bool ReStoreHisVerFile(CString path);		//��ԭ��ʷ��¼�汾
	bool DeleteHisVerFile(CString path);		//ɾ����ʷ��¼�汾
	bool CleanHisVerFile(CString path);			//�����ʷ��¼�汾
	bool GetFileByDataShow(CString strInfo);	//�����ڲ鿴�ļ�
public:
public:
	//CString m_strClientName;				//�û���¼��
	CNetdiskClientApp*			m_pMainApp;
};

