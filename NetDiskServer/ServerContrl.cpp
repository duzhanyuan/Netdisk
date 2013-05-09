// ServerContrl.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "ServerContrl.h"
#include "ServIndex.h"
#include "RecycleFileHistriyPath.h"
#include "HistroyVersionIndex.h"
char Buffer[MAX_BUFFER_LEN];

// CServerContrl

CServerContrl::CServerContrl()
{
	m_strDiskRootPath=_T("E:\\��ҵ����Ŀ¼\\�����û�\\");
	m_strDiskIndexPath=_T("E:\\��ҵ����Ŀ¼\\�����û�����Ŀ¼\\");
	m_strRecyclePath=_T("E:\\��ҵ����Ŀ¼\\����վ\\");
	m_strRecycleIndexPath=_T("E:\\��ҵ����Ŀ¼\\����վ����Ŀ¼\\");
	m_strHisVerRootPath=_T("E:\\��ҵ����Ŀ¼\\�ļ���ʷ�汾\\");
	m_strShareFloder=_T("�����ļ���");
	InitializeCriticalSection(&m_csFile);

}

CServerContrl::~CServerContrl()
{
	DeleteCriticalSection(&m_csFile);
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
					CString userInfo;

					int iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
					//��ȡ�û����͹�������Ϣ
					userInfo=recvPack.sContent;
					
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
							int ret=pServCtrl->UserLogin(userInfo);

							//�����û���¼��Ϣ
							CString msgStr;
							msgStr.Format(_T("%d"),ret);
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],msgStr);

							//��¼�û�����,�û����͹������ǡ��û��������롱 ���
							pServCtrl->m_strUserName=userInfo.Left(userInfo.Find(':'));
							break;
						}
					case UPDATECLIENT:
						{
							//��ȡ���û���Ŀ¼������Ϣ
							CServIndex* ServIndex=new CServIndex();
							pServCtrl->m_StrIndexInfo=ServIndex->GetIndexInfo(pServCtrl->m_strDiskIndexPath,userInfo);

							//����������Ϣ
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],pServCtrl->m_StrIndexInfo);

							break;
						}
					case GETCATALOGINFO:
						{
							CServIndex* ServIndex=new CServIndex();
							pServCtrl->m_StrIndexInfo=ServIndex->getCatalogInfo(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,userInfo);

							//����������Ϣ
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],pServCtrl->m_StrIndexInfo);

							break;
						}
					case UPLOADFILE:
						{
							//�����ļ�
							CString path=pServCtrl->m_strDiskRootPath+userInfo;
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
							CString rootpath=(0 == userInfo.Find(pServCtrl->m_strUserName)?pServCtrl->m_strUserName:pServCtrl->m_strShareFloder);
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,rootpath);

							//��Ӱ汾��Ϣ
							CHistroyVersionIndex* pIndex=new CHistroyVersionIndex();
							if(!pIndex->IfFileVerExist(pServCtrl->m_strUserName,path))
							{
								int pos=userInfo.ReverseFind('\\');
								CString fileName=userInfo.Right(userInfo.GetLength()-pos-1);
								pIndex->AddIndex(pServCtrl->m_strUserName,path);
							}
							break;
						}
					case NEWFLODER:
						{

							CString path=pServCtrl->m_strDiskRootPath+userInfo;
							if(!CreateDirectory(path,NULL))
							{
								//////////////////////////////////////////////////////////////////////////
								//����һ��������Ϣ
								//////////////////////////////////////////////////////////////////////////
								CString msgStr;
								msgStr.Format(_T("�����ļ���ʧ�ܣ��������Ϊ��%s"),GetLastError());
								return false;
							}

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							CString rootpath=(0 == userInfo.Find(pServCtrl->m_strUserName)?pServCtrl->m_strUserName:pServCtrl->m_strShareFloder);
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,rootpath);


							break;
						}
					case DELETEFILE:
						{
							CString path=pServCtrl->m_strDiskRootPath+userInfo;
							int pos=userInfo.ReverseFind('\\');

							//����ɾ�����ļ��������ڻ���վ�û�Ŀ¼�µĶ���Ŀ¼
							CString tmpStr=pServCtrl->m_strUserName+_T("\\")+userInfo.Right(userInfo.GetLength()-pos-1);
							CString recyPath=pServCtrl->m_strRecyclePath+tmpStr;

							//������ʷ�ļ�����
							CRecycleFileHistriyPath* pRecyleFileHisPath=new CRecycleFileHistriyPath(pServCtrl->m_strUserName);
							//���һ��������
							pRecyleFileHisPath->AddIndex(recyPath,path);

							//�ж�ɾ�������ļ������ļ���
							DWORD value = GetFileAttributes(path);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
								pServCtrl->	RecursiveMove(path,recyPath);
							else//�ƶ�������վ
							{
								//�ж��ļ��Ƿ���ڻ���վ��,������ǣ����ƶ��ļ�
								if(::GetFileAttributes(recyPath)==-1)
								{
									MoveFile(path,recyPath);									
								}
								else//������ڣ���ɾ��֮ǰ�ļ����ƶ�
								{
									DeleteFile(path);
									MoveFile(path,recyPath);									
									
								}

							}

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							CString rootpath=(0 == userInfo.Find(pServCtrl->m_strUserName)?pServCtrl->m_strUserName:pServCtrl->m_strShareFloder);
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,rootpath);

							//���»���վ������¼
							ServIndex->UpdateIndex(pServCtrl->m_strRecyclePath,pServCtrl->m_strRecycleIndexPath,pServCtrl->m_strUserName);

							break;
						}
					case REFRESH:
						{
							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,pServCtrl->m_strUserName);

							//���¹����ļ�������
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,pServCtrl->m_strShareFloder);
							break;
						}
					case DOWNLOADFILE:
						{
							CString path;
							path=pServCtrl->m_strDiskRootPath+userInfo;
							//�����ļ���Ϣ
							pServCtrl->SendFile(pServCtrl->m_fdAllSocket.fd_array[i],path);

							break;
						}
					case DOWNLOADCATALOG:
						{
							CString tmpPath=pServCtrl->m_strDiskRootPath+userInfo;
							//����Ŀ¼��Ϣ
							pServCtrl->RecursiveSend(pServCtrl->m_fdAllSocket.fd_array[i],tmpPath,tmpPath);

							//����Ŀ¼������Ϣ
							CString endSending=_T("send_cat_end");
							//����������Ϣ
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],endSending);

							break;
						}
					case MOVEFILE:
						{
							int pos=userInfo.Find('+');
							CString srcPath=pServCtrl->m_strDiskRootPath+userInfo.Left(pos);
							CString desPath=pServCtrl->m_strDiskRootPath+userInfo.Right(userInfo.GetLength()-pos-1);

							//�ж��ƶ������ļ������ļ���
							DWORD value = GetFileAttributes(srcPath);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
							{
								pServCtrl->RecursiveMove(srcPath,desPath);
							}
							else
							{
								if(::GetFileAttributes(desPath)==-1)
								{
									MoveFile(srcPath,desPath);									
								}
								else
								{
									DeleteFile(srcPath);
									MoveFile(srcPath,desPath);	
								}
							}


							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							CString rootpath=(0 == userInfo.Find(pServCtrl->m_strUserName)?pServCtrl->m_strUserName:pServCtrl->m_strShareFloder);
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,rootpath);

							break;
						}
					case RECYCLE_UPDATE:
						{
							//���»���վ������¼
							CServIndex* ServIndex=new CServIndex();
							//ServIndex->UpdateIndex(pServCtrl->m_strRecyclePath,pServCtrl->m_strRecycleIndexPath,pServCtrl->m_strUserName);

							pServCtrl->m_StrIndexInfo=ServIndex->getCatalogInfo(pServCtrl->m_strRecyclePath,pServCtrl->m_strRecycleIndexPath,userInfo);

							//����������Ϣ
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],pServCtrl->m_StrIndexInfo);
							break;
						}
				
					case RECYCLE_RESTORE:
						{
							//������ʷ�ļ�����
							CRecycleFileHistriyPath* pRecyleFileHisPath=new CRecycleFileHistriyPath(pServCtrl->m_strUserName);
							CString hisPath;
							CString curPath=pServCtrl->m_strRecyclePath+userInfo;
							pRecyleFileHisPath->DeleteIndex(curPath,hisPath);

							//�ж��ƶ������ļ������ļ���
							DWORD value = GetFileAttributes(curPath);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
							{
								pServCtrl->RecursiveMove(curPath,hisPath);
							}
							else
							{
								if(::GetFileAttributes(hisPath)==-1)
								{
									MoveFile(curPath,hisPath);									
								}
								else
								{
									DeleteFile(curPath);
									MoveFile(curPath,hisPath);	
								}
							}

							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							CString rootpath=(0 == userInfo.Find(pServCtrl->m_strUserName)?pServCtrl->m_strUserName:pServCtrl->m_strShareFloder);
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,rootpath);
							//���»���վ������¼
							ServIndex->UpdateIndex(pServCtrl->m_strRecyclePath,pServCtrl->m_strRecycleIndexPath,pServCtrl->m_strUserName);


							break;
						}
					case RECYCLE_DEL:
						{

							//������ʷ�ļ�����
							CRecycleFileHistriyPath* pRecyleFileHisPath=new CRecycleFileHistriyPath(pServCtrl->m_strUserName);
							CString hisPath;
							CString curPath=pServCtrl->m_strRecyclePath+userInfo;
							pRecyleFileHisPath->DeleteIndex(curPath,hisPath);

							//�ж��ƶ������ļ������ļ���
							DWORD value = GetFileAttributes(curPath);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
							{
								pServCtrl->RecursiveDelete(curPath);
							}
							else
							{
					
								DeleteFile(curPath);
		
							}

							CServIndex* ServIndex=new CServIndex();
							//���»���վ������¼
							ServIndex->UpdateIndex(pServCtrl->m_strRecyclePath,pServCtrl->m_strRecycleIndexPath,pServCtrl->m_strUserName);


							break;
						}
					case RECYCLE_CLEAN:
						{
							CString curPath=pServCtrl->m_strRecyclePath+userInfo;
							pServCtrl->RecursiveDelete(curPath);
							//�ݹ�ɾ��֮�����´����û��Ļ���վĿ¼
							CreateDirectory(curPath,NULL);

							CServIndex* ServIndex=new CServIndex();
							//���»���վ������¼
							ServIndex->UpdateIndex(pServCtrl->m_strRecyclePath,pServCtrl->m_strRecycleIndexPath,pServCtrl->m_strUserName);

							//�����ʷ�ļ�·����¼
							CRecycleFileHistriyPath* pRecyleFileHisPath=new CRecycleFileHistriyPath(pServCtrl->m_strUserName);
							pRecyleFileHisPath->CleanIndexFile();
							break;
						}
					case RECYCLE_FIND:
						{
							CServIndex* pIndex=new CServIndex();
							CString indexInfo=pIndex->FindFileIndex(pServCtrl->m_strRecycleIndexPath,pServCtrl->m_strUserName,userInfo);
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],indexInfo);

							break;
						}
					case GETFILESIZEINFO:
						{
							pServCtrl->m_fileTotalSize=0;
							CString path=pServCtrl->m_strDiskRootPath+userInfo;
							//�ж����ļ������ļ���
							DWORD value = GetFileAttributes(path);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
							{
								pServCtrl->GetDownLoadFileSize(path);
							}
							else
							{
								HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 		
								pServCtrl->m_fileTotalSize=GetFileSize(hFile,NULL);
								CloseHandle(hFile);
							}
							//�����ļ���С��Ϣ
							CString strFileSize;
							strFileSize.Format(_T("%lld"),pServCtrl->m_fileTotalSize);
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],strFileSize);
							
							break;
						}
					case HISTROYVERSION:
						{
							CHistroyVersionIndex* pIndex=new CHistroyVersionIndex();
							int pos=userInfo.ReverseFind('\\');
							CString fileName=userInfo.Right(userInfo.GetLength()-pos-1);
							CString indexInfo=pIndex->GetHisVerIndex(pServCtrl->m_strUserName,fileName);
							CString currentMd5Code=pIndex->GetCurrentMd5Code(pServCtrl->m_strDiskRootPath+userInfo);
							indexInfo = indexInfo+currentMd5Code;
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],indexInfo);

							break;
						}
					case HIS_RESTORE:
						{
							CHistroyVersionIndex* pIndex=new CHistroyVersionIndex();
							pIndex->RestoreIndex(pServCtrl->m_strUserName,userInfo);
							//�����û�������¼
							CServIndex* ServIndex=new CServIndex();
							CString rootpath=(0 == userInfo.Find(pServCtrl->m_strUserName)?pServCtrl->m_strUserName:pServCtrl->m_strShareFloder);
							ServIndex->UpdateIndex(pServCtrl->m_strDiskRootPath,pServCtrl->m_strDiskIndexPath,rootpath);

							break;
						}
					case HIS_DELETE:
						{
							CHistroyVersionIndex* pIndex=new CHistroyVersionIndex();
							pIndex->DelIndex(pServCtrl->m_strUserName,userInfo);
							break;
						}
					case HIS_CLEAR:
						{
							CHistroyVersionIndex* pIndex=new CHistroyVersionIndex();
							pIndex->ClearIndex(pServCtrl->m_strUserName,userInfo);
							break;
						}
					case FINDFILEBYSTR:
						{
							CServIndex* pIndex=new CServIndex();
							CString indexInfo=pIndex->FindFileIndex(pServCtrl->m_strDiskIndexPath,pServCtrl->m_strUserName,userInfo);
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],indexInfo);
							break;
						}
					case FINDFILEBYTIME:
						{
							CServIndex* pIndex=new CServIndex();
							int pos=userInfo.Find(':');
							CString strCurrentPath=userInfo.Left(pos);
							userInfo=userInfo.Right(userInfo.GetLength()-pos-1);
							CString strStartT,strEndT;
							pos=userInfo.Find(':');
							strStartT=userInfo.Left(pos);
							strEndT=userInfo.Right(userInfo.GetLength()-pos-1);
							CString indexInfo=pIndex->FindFileByTime(strCurrentPath,strStartT,strEndT);
							pServCtrl->SendDataInfo(pServCtrl->m_fdAllSocket.fd_array[i],indexInfo);
							break;
						}
					}
				}
			}
		}
	}
	Sleep(100);
}
//�ж�Ŀ¼�Ƿ����
bool CServerContrl::IfFloderExist(CString path)
{
	WIN32_FIND_DATA wfd;
	BOOL reVal=false;
	HANDLE hFind=FindFirstFile(path,&wfd);
	if((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		reVal=true;
	FindClose(hFind);
	return reVal;
}
//��ȡ�����ļ���С��Ϣ
void CServerContrl::GetDownLoadFileSize(CString path)
{
	CFileFind finder;
	BOOL ret;
	if(path.Find('.'))
		ret=finder.FindFile(path);
	else
		ret=finder.FindFile(path+_T("\\*.*"));
	
	while(ret)
	{
		ret=finder.FindNextFile();

		if(finder.IsDots())
			continue;
		if(finder.IsDirectory())
			GetDownLoadFileSize(path+_T("\\")+finder.GetFileName());
		else
			m_fileTotalSize+=finder.GetLength();

	}
	finder.Close();
}
//����������Ϣ
bool CServerContrl::SendDataInfo(SOCKET cSocket,CString strIndexInfo,int dataType/* =0 */)
{
	//�����û���Ŀ¼��Ϣ
	DataPackage sendPack;
	ZeroMemory(&sendPack,sizeof(sendPack));
	int n=strIndexInfo.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,strIndexInfo,strIndexInfo.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,strIndexInfo,strIndexInfo.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
	sendPack.sContent[len]='\0';
	sendPack.nContentLen=len;
	sendPack.nPackLen=sizeof(sendPack);
	sendPack.iType=dataType;

	//�˴�Ϊ��־�ļ����switch�ṹ
	int retvalue=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
	if(SOCKET_ERROR == retvalue)
	{
		AfxMessageBox(_T("�����û�Ŀ¼�ṹ��Ϣʧ�ܣ�"));
		return false;
	}
}
//�����ļ�
void CServerContrl::SendFile(SOCKET cSocket,CString filePath)
{
	//�����ļ�����
	FILE* infile;
	char c_path[1024];

	memset(c_path,0,filePath.GetLength()*2+1);
	USES_CONVERSION; 
	strcpy((LPSTR)c_path,OLE2A(filePath.LockBuffer()));
	infile=fopen(c_path,"rb");

	u_long ulFlagCount = 0;            //��¼���˶�������
	u_long ulFileRead = 0;					//һ��ʵ�ʶ�ȡ���ֽ���
	char c_msg[1024];
	CString recvMsg;
	int result;
	DataPackage sendPack;

	while((ulFileRead=fread(Buffer,sizeof( char),MAX_BUFFER_LEN,infile))!=0)
	{
		ZeroMemory(&sendPack,sizeof(sendPack));
		sendPack.nPackLen=sizeof(sendPack);
		sendPack.nContentLen=ulFileRead;
		CopyMemory(sendPack.sContent,Buffer,ulFileRead);
		sendPack.nPosition=ulFlagCount;
		sendPack.iType=2;

		result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
		Sleep(1);
		if(result==SOCKET_ERROR)
		{
			AfxMessageBox(_T("�����ļ�ʧ�ܣ�"));
		}
		else
		{
			ulFlagCount+=ulFileRead;
		}
	}

	fclose(infile);
	//�����ļ�������
	CString strTmp=_T("send_file_end");
	SendDataInfo(cSocket,strTmp,2);
}
//�ݹ��ƶ�Ŀ¼
bool CServerContrl::RecursiveMove(CString srcPath,CString desPath)
{
	if(srcPath == _T(""))
		return false;
	
	CreateDirectory(desPath,NULL);

	wchar_t wcPath[MAX_PATH] = {0};
	wcscpy_s(wcPath,MAX_PATH,srcPath);
	wcscat_s(wcPath,MAX_PATH,_T("\\*.*"));
	CString strPath=desPath;
	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData,sizeof(WIN32_FIND_DATA));

	HANDLE hFindFile = FindFirstFile(wcPath,&FindFileData);

	if(hFindFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bContinue = true;

	while (bContinue != false)
	{
		//bIsDotsΪ���ʾ��.��..
		bool bIsDots = (wcscmp(FindFileData.cFileName,_T(".")) == 0 || wcscmp(FindFileData.cFileName,_T("..")) == 0);
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && bIsDots == false)
		{
			//��Ŀ¼,���ٽ����Ŀ¼
			wcscpy_s(wcPath,MAX_PATH,srcPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			RecursiveMove(wcPath,desPath+_T("\\")+FindFileData.cFileName);
			//Ѱ����һ�ļ�
			bContinue = FindNextFile(hFindFile,&FindFileData);
			continue;
		}

		if (bIsDots == false)
		{
			//���ļ�ɾ��֮
			wcscpy_s(wcPath,MAX_PATH,srcPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			MoveFile(wcPath,desPath+_T("\\")+FindFileData.cFileName);
		}
		//Ѱ����һ�ļ�
		bContinue = FindNextFile(hFindFile,&FindFileData);

	}

	FindClose(hFindFile);

	//ɾ����Ŀ¼
	RemoveDirectory(srcPath);
	return true;
}
//�ݹ�Ŀ¼�����ļ���
void CServerContrl::RecursiveSend(SOCKET cSocket,CString szPath,CString rootPath)
{
	CFileFind ff;  
	CString path = szPath;  
	int result;

	if(path.Right(1) != _T("\\"))  
		path += _T("\\");  


	path += _T("*.*");  
	BOOL res = ff.FindFile(path);  

	while(res)  
	{  
		res = ff.FindNextFile();  
		//���ļ�ʱֱ�ӷ����ļ�
		if (!ff.IsDots() && !ff.IsDirectory())  
		{
			//������͵����ļ����ȷ���һ��ȷ�ϰ�
			CString strTmp=ReturnCientPath(ff.GetFilePath(),rootPath);
			SendDataInfo(cSocket,strTmp,2);
			SendFile(cSocket,ff.GetFilePath());
		}
		else if (ff.IsDots())  
			continue;  
		else if (ff.IsDirectory())  
		{  
			//����Ŀ¼������Ϣ
			path = ff.GetFilePath(); 
			CString strTmp=ReturnCientPath(ff.GetFilePath(),rootPath);
			SendDataInfo(cSocket,strTmp,1);
			//��Ŀ¼ʱ�����ݹ飬���͸�Ŀ¼�µ��ļ�  
			RecursiveSend(cSocket,path,rootPath);  
 
		}  
	}  

	ff.Close();

}

//����������·��Ϊ�ͻ���·��
CString CServerContrl::ReturnCientPath(CString servPath,CString rootPath)
{
	int pos=rootPath.GetLength();
	return servPath.Right(servPath.GetLength()-pos-1);
}
//�ݹ�ɾ���ļ���
bool CServerContrl::RecursiveDelete(CString szPath)  
{  
	if(szPath == _T(""))
		return false;

	wchar_t wcPath[MAX_PATH] = {0};
	wcscpy_s(wcPath,MAX_PATH,szPath);
	wcscat_s(wcPath,MAX_PATH,_T("\\*.*"));
	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData,sizeof(WIN32_FIND_DATA));

	HANDLE hFindFile = FindFirstFile(wcPath,&FindFileData);

	if(hFindFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bContinue = true;

	while (bContinue != false)
	{
		//bIsDotsΪ���ʾ��.��..
		bool bIsDots = (wcscmp(FindFileData.cFileName,_T(".")) == 0 || wcscmp(FindFileData.cFileName,_T("..")) == 0);
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && bIsDots == false)
		{
			//��Ŀ¼,���ٽ����Ŀ¼
			wcscpy_s(wcPath,MAX_PATH,szPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			RecursiveDelete(wcPath);
			//Ѱ����һ�ļ�
			bContinue = FindNextFile(hFindFile,&FindFileData);
			continue;
		}

		if (bIsDots == false)
		{
			//���ļ�ɾ��֮
			wcscpy_s(wcPath,MAX_PATH,szPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			DeleteFile(wcPath);
		}
		//Ѱ����һ�ļ�
		bContinue = FindNextFile(hFindFile,&FindFileData);

	}

	FindClose(hFindFile);

	//ɾ����Ŀ¼
	RemoveDirectory(szPath);
	return true;
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
