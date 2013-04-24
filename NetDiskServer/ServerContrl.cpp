// ServerContrl.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "ServerContrl.h"
#include "ServIndex.h"

// CServerContrl

CServerContrl::CServerContrl()
{
}

CServerContrl::~CServerContrl()
{
}

bool CServerContrl::InitSocket()
{
	WSADATA wsaData;
	int ErrCode;

	//��ʼ��socket����
	ErrCode=WSAStartup(MAKEWORD(2,2),&wsaData);
	if(0 != ErrCode)
	{
		AfxMessageBox(_T("WSAStartup����ʧ�ܣ�"));
		return false;
	}

	//�����������׽���
	m_sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sServer)
	{
		AfxMessageBox(_T("socket����ʧ�ܣ�"));
		return false;
	}

	//��ȡϵͳĬ�ϵĽ������ݻ�������С
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	int nErrCode= getsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//����ϵͳ�������ݻ�����ΪĬ��ֵ��10��
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;
	}


	//�������ϵͳ�������ݻ������Ƿ�ɹ�
	unsigned int uiNewRcvBuf;
	getsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;

	}

	//��ȡϵͳĬ�ϵķ������ݻ�������С
	nErrCode= getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//����ϵͳ�������ݻ�����ΪĬ��ֵ��10��
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;

	}


	//�������ϵͳ�������ݻ������Ƿ�ɹ�
	getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
		return false;

	}

	//�������׽��ֵ�ַ
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_PORT);

	//�󶨷�����
	nErrCode=bind(m_sServer,(sockaddr*)&servAddr,sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("bind ����ʧ�ܣ�"));
		closesocket(m_sServer);
		WSACleanup();
		return false;
	}

	//���ü���socket
	nErrCode=listen(m_sServer,SOMAXCONN);
	if(SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("listen ����ʧ�ܣ�"));
		closesocket(m_sServer);
		WSACleanup();
		return false;
	}

	return true;
}

//���������������߳�
bool CServerContrl::StartServ()
{
	//��ʼ��socket
	if(!InitSocket())
	{
		return false;
	}

	//���������߳�
	m_hServCtrl=CreateThread(0,0,ServListenThread,this,0,0);
	m_bThreadCtrl=FALSE;

	AfxMessageBox(_T("�����������ɹ���"));
	return true;
}

//�����߳�
DWORD WINAPI CServerContrl::ServListenThread(LPVOID lpParam)
{

	CServerContrl* pServCtrl=(CServerContrl*)lpParam;

	if(pServCtrl->m_bThreadCtrl)
		ExitThread(0);

	FD_ZERO(&(pServCtrl->m_fdAllSocket));
	FD_SET(pServCtrl->m_sServer,&(pServCtrl->m_fdAllSocket));
	timeval tv;
	tv.tv_sec=1000;
	tv.tv_usec=0;

	while(true)
	{
		pServCtrl->m_fdRead=pServCtrl->m_fdAllSocket;
		int nRet=select(NULL,&(pServCtrl->m_fdRead),NULL,NULL,&tv);
		if(0 == nRet)
		{
			AfxMessageBox(_T("select����ʧ�ܣ�"));
			return false;
		}

		for(int i=0;i<pServCtrl->m_fdAllSocket.fd_count;i++)
		{
			if(FD_ISSET(pServCtrl->m_fdAllSocket.fd_array[i],&pServCtrl->m_fdRead))
			{
				//�����µ�����
				if(pServCtrl->m_sServer == pServCtrl->m_fdAllSocket.fd_array[i])
				{
					sockaddr_in newConn;
					int connLen=sizeof(newConn);
					SOCKET sNew=accept(pServCtrl->m_sServer,(sockaddr*)&newConn,&connLen);
					FD_SET(sNew,&pServCtrl->m_fdAllSocket);
					//����ͻ����ڷ������ϵ���ʾ
				}
				else
				{
					DataPackage recvPack;
					DataPackage sendPack;
					ZeroMemory(&recvPack,sizeof(recvPack));

					int iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
					if(0 == iErrCode)
					{
						AfxMessageBox(_T("���ӹرգ�"));
						return false;
					}
					else if(iErrCode < 0)
					{
						CString strMsg;
						strMsg.Format(_T("��������ʧ�ܣ��������Ϊ: %d\n"), WSAGetLastError());
						AfxMessageBox(strMsg);
						return false;
					}

					//�ж��û��Ĳ�������
					switch(recvPack.iType)
					{
					case LOGIN:
						{
							//�������ݿ⣬�жϽ�����û��Ƿ���ڣ���������н������ݲ����������������
							CString userInfo;
							userInfo=recvPack.sContent;
							int ret=pServCtrl->UserLogin(userInfo);

							//�����û���¼��Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							sprintf(sendPack.sContent,"%d",ret);
							sendPack.nPackLen=sizeof(sendPack);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("�����û���¼����ʧ�ܣ�"));
								return false;
							}
							//��¼�û�����
							pServCtrl->m_strUserName=userInfo.Left(userInfo.Find(':'));
							break;
						}
					case UPDATECLIENT:
						{
							CServIndex* ServIndex=new CServIndex();
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							//��ȡ��Ŀ¼�µ����ļ����ļ�����Ϣ
							pServCtrl->m_StrIndexInfo=ServIndex->GetIndexInfo(m_strUserMsg);

							//�����û���Ŀ¼��Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							int n=pServCtrl->m_StrIndexInfo.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
							sendPack.sContent[len]='\0';
							sendPack.nContentLen=len;
							sendPack.nPackLen=sizeof(sendPack);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("�����û�Ŀ¼��Ϣʧ�ܣ�"));
								return false;
							}
							break;
						}
					case GETCATALOGINFO:
						{
							CServIndex* ServIndex=new CServIndex();
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							pServCtrl->m_StrIndexInfo=ServIndex->getCatalogInfo(m_strUserMsg);

							//�����û���Ŀ¼��Ϣ
							ZeroMemory(&sendPack,sizeof(sendPack));
							int n=pServCtrl->m_StrIndexInfo.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,pServCtrl->m_StrIndexInfo,pServCtrl->m_StrIndexInfo.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
							sendPack.sContent[len]='\0';
							sendPack.nContentLen=len;
							sendPack.nPackLen=sizeof(sendPack);

							//u_long mode = 1;
							//ioctlsocket(pServCtrl->m_fdAllSocket.fd_array[i],FIONBIO,&mode);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("�����û�Ŀ¼�ṹ��Ϣʧ�ܣ�"));
								return false;
							}
							break;
						}
					case UPLOADFILE:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;
							//�����ļ�
							CString path=_T("E:\\��ҵ����Ŀ¼\\")+m_strUserMsg;
							FILE* inFile;
							char c_path[1024];
							int n=path.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,path,path.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,path,path.GetLength()+1,c_path,len+1,NULL,NULL);
							c_path[len]='\0';


							//�����ļ�����
							ZeroMemory(&recvPack,sizeof(recvPack));
							//�����ļ�
							inFile=fopen(c_path,"wb");
							fclose(inFile);

							iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
							CString strMsg;
							strMsg=recvPack.sContent;
							while(iErrCode>0 && strMsg!=_T("send_file_end"))
							{
								inFile=fopen(c_path,"ab");

								u_long nFilePosition=recvPack.nPosition;
								int nRe=fseek(inFile,nFilePosition,SEEK_SET);
								if(nRe)
								{
									AfxMessageBox(_T("SendDataThrad�ж�λʧ��!"));
									break;
								}
								u_long nNumberOfBytesWritten=fwrite(&recvPack.sContent,sizeof(char),recvPack.nContentLen,inFile);
								if(recvPack.nContentLen != nNumberOfBytesWritten)
								{
									AfxMessageBox(_T("д�ļ�ʧ��!"));
									break;
								}
								ZeroMemory(&recvPack,sizeof(recvPack));
								fflush(inFile);
								fclose(inFile);

								iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
								strMsg=recvPack.sContent;

							}
							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);
							break;
						}
					case NEWFLODER:
						{
							CString m_strUserMsg;//�û���������Ŀ¼·��
							m_strUserMsg=recvPack.sContent;

							CString path=_T("E:\��ҵ����Ŀ¼\\")+m_strUserMsg;
							if(!CreateDirectory(path,NULL))
							{
								CString msgStr;
								msgStr.Format(_T("�����ļ���ʧ�ܣ��������Ϊ��%s"),GetLastError());
								return false;
							}

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);
							break;
						}
					}
				}
			}
		}
	}
	Sleep(100);
}
//�жϽ������ӵ��û��Ƿ����
int CServerContrl::UserLogin(CString Userinfo)
{
	CString userName,userPwd;
	int iPos=Userinfo.Find(_T(':'));
	userName=Userinfo.Left(iPos);
	userPwd=Userinfo.Right(Userinfo.GetLength()-iPos-1);

	//�������ݿ�
	CDatabase db;
	CString tmpSql,numstr;
	try
	{

		db.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
		if(!db.IsOpen())
		{
			AfxMessageBox(_T("���ݿ��ʧ�ܣ�"));
			return OTHER_ERROR;
		}
		tmpSql.Format(_T("select * from netdisk_user_tb"));
		CRecordset rs(&db);
		rs.Open(CRecordset::snapshot,tmpSql);
		if(rs.IsBOF())
			return OTHER_ERROR;

		while(!rs.IsEOF())
		{
			rs.GetFieldValue((short)2,numstr);
			if(numstr == userName)
			{
				rs.GetFieldValue((short)3,numstr);
				if(numstr != userPwd)
					return PASSWD_ERROR;
				else
					return LOGIN_SUCCESS;
			}
			rs.MoveNext();
		}

		if(rs.IsEOF())
			return USERNAME_NOTEXIST;
		/*tmpSql.Format(_T("select count(*) as num from netdisk_user_tb where name='%s'"),userName);
		CRecordset rs(&db);
		rs.Open(CRecordset::snapshot,tmpSql);
		if(rs.IsBOF())
		return OTHER_ERROR;
		rs.GetFieldValue((short)0,numstr);
		if(numstr!=_T("0"))
		{

		CString pwdStr;
		rs.GetFieldValue((short)4,numstr);
		pwdStr=numstr;
		if(pwdStr !=userPwd)
		return PASSWD_ERROR;
		else
		return LOGIN_SUCCESS;
		}
		else
		{
		return USERNAME_NOTEXIST;
		}*/
		rs.Close();

	}
	catch(CDBException* pe)
	{
		CString tmp;
		tmp.Format(_T("���ݿ��������%s"),pe->m_strError);
		AfxMessageBox(tmp);
		pe->Delete();
	}
	db.Close();
	return OTHER_ERROR;
}

void CServerContrl::StopServ()
{
	m_bThreadCtrl=TRUE;
	closesocket(m_sServer);
	WSACleanup();
}
// CServerContrl member functions
