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

	//初始化socket环境
	ErrCode=WSAStartup(MAKEWORD(2,2),&wsaData);
	if(0 != ErrCode)
	{
		AfxMessageBox(_T("WSAStartup函数失败！"));
		return false;
	}

	//创建服务器套接字
	m_sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sServer)
	{
		AfxMessageBox(_T("socket函数失败！"));
		return false;
	}

	//获取系统默认的接收数据缓冲区大小
	unsigned int uiRcvBuf;
	int uiRcvBufLen = sizeof(uiRcvBuf);
	int nErrCode= getsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//设置系统接收数据缓冲区为默认值的10倍
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("修改系统接收数据缓冲区失败！"));
		return false;
	}


	//检查设置系统接收数据缓冲区是否成功
	unsigned int uiNewRcvBuf;
	getsockopt(m_sServer, SOL_SOCKET, SO_RCVBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("修改系统接收数据缓冲区失败！"));
		return false;

	}

	//获取系统默认的发送数据缓冲区大小
	nErrCode= getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		return false;
	}

	//设置系统发送数据缓冲区为默认值的10倍
	uiRcvBuf *= 10;
	nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiRcvBuf, uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("修改系统接收数据缓冲区失败！"));
		return false;

	}


	//检查设置系统发送数据缓冲区是否成功
	getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF,(char*)&uiNewRcvBuf, &uiRcvBufLen);
	if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
	{
		AfxMessageBox(_T("修改系统接收数据缓冲区失败！"));
		return false;

	}

	//服务器套接字地址
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_PORT);

	//绑定服务器
	nErrCode=bind(m_sServer,(sockaddr*)&servAddr,sizeof(SOCKADDR_IN));
	if(SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("bind 函数失败！"));
		closesocket(m_sServer);
		WSACleanup();
		return false;
	}

	//设置监听socket
	nErrCode=listen(m_sServer,SOMAXCONN);
	if(SOCKET_ERROR == nErrCode)
	{
		AfxMessageBox(_T("listen 函数失败！"));
		closesocket(m_sServer);
		WSACleanup();
		return false;
	}

	return true;
}

//启动服务器控制线程
bool CServerContrl::StartServ()
{
	//初始化socket
	if(!InitSocket())
	{
		return false;
	}

	//创建监听线程
	m_hServCtrl=CreateThread(0,0,ServListenThread,this,0,0);
	m_bThreadCtrl=FALSE;

	AfxMessageBox(_T("服务器启动成功！"));
	return true;
}

//监听线程
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
			AfxMessageBox(_T("select函数失败！"));
			return false;
		}

		for(int i=0;i<pServCtrl->m_fdAllSocket.fd_count;i++)
		{
			if(FD_ISSET(pServCtrl->m_fdAllSocket.fd_array[i],&pServCtrl->m_fdRead))
			{
				//接收新的连接
				if(pServCtrl->m_sServer == pServCtrl->m_fdAllSocket.fd_array[i])
				{
					sockaddr_in newConn;
					int connLen=sizeof(newConn);
					SOCKET sNew=accept(pServCtrl->m_sServer,(sockaddr*)&newConn,&connLen);
					FD_SET(sNew,&pServCtrl->m_fdAllSocket);
					//处理客户端在服务器上的显示
				}
				else
				{
					DataPackage recvPack;
					DataPackage sendPack;
					ZeroMemory(&recvPack,sizeof(recvPack));

					int iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
					if(0 == iErrCode)
					{
						AfxMessageBox(_T("连接关闭！"));
						return false;
					}
					else if(iErrCode < 0)
					{
						CString strMsg;
						strMsg.Format(_T("接收数据失败，错误代码为: %d\n"), WSAGetLastError());
						AfxMessageBox(strMsg);
						return false;
					}

					//判断用户的操作类型
					switch(recvPack.iType)
					{
					case LOGIN:
						{
							//连接数据库，判断接入的用户是否存在，存在则进行接收数据操作，否则继续监听
							CString userInfo;
							userInfo=recvPack.sContent;
							int ret=pServCtrl->UserLogin(userInfo);

							//返回用户登录信息
							ZeroMemory(&sendPack,sizeof(sendPack));
							sprintf(sendPack.sContent,"%d",ret);
							sendPack.nPackLen=sizeof(sendPack);
							int retvalue=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);
							if(SOCKET_ERROR == retvalue)
							{
								AfxMessageBox(_T("发送用户登录反馈失败！"));
								return false;
							}
							//记录用户名称
							pServCtrl->m_strUserName=userInfo.Left(userInfo.Find(':'));
							break;
						}
					case UPDATECLIENT:
						{
							CServIndex* ServIndex=new CServIndex();
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;
							//获取该目录下的子文件及文件夹信息
							pServCtrl->m_StrIndexInfo=ServIndex->GetIndexInfo(m_strUserMsg);

							//返回用户的目录信息
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
								AfxMessageBox(_T("发送用户目录信息失败！"));
								return false;
							}
							break;
						}
					case GETCATALOGINFO:
						{
							CServIndex* ServIndex=new CServIndex();
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;
							pServCtrl->m_StrIndexInfo=ServIndex->getCatalogInfo(m_strUserMsg);

							//返回用户的目录信息
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
								AfxMessageBox(_T("发送用户目录结构信息失败！"));
								return false;
							}
							break;
						}
					case UPLOADFILE:
						{
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;
							//创建文件
							CString path=_T("E:\\企业网盘目录\\")+m_strUserMsg;
							FILE* inFile;
							char c_path[1024];
							int n=path.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,path,path.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,path,path.GetLength()+1,c_path,len+1,NULL,NULL);
							c_path[len]='\0';


							//接收文件数据
							ZeroMemory(&recvPack,sizeof(recvPack));
							//创建文件
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
									AfxMessageBox(_T("SendDataThrad中定位失败!"));
									break;
								}
								u_long nNumberOfBytesWritten=fwrite(&recvPack.sContent,sizeof(char),recvPack.nContentLen,inFile);
								if(recvPack.nContentLen != nNumberOfBytesWritten)
								{
									AfxMessageBox(_T("写文件失败!"));
									break;
								}
								ZeroMemory(&recvPack,sizeof(recvPack));
								fflush(inFile);
								fclose(inFile);

								iErrCode=recv(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&recvPack,sizeof(recvPack),0);
								strMsg=recvPack.sContent;

							}
							//更新用户索引记录
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);
							break;
						}
					case NEWFLODER:
						{
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;

							CString path=_T("E:\企业网盘目录\\")+m_strUserMsg;
							if(!CreateDirectory(path,NULL))
							{
								CString msgStr;
								msgStr.Format(_T("创建文件夹失败，错误代码为：%s"),GetLastError());
								return false;
							}

							//更新用户索引记录
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
//判断接入连接的用户是否存在
int CServerContrl::UserLogin(CString Userinfo)
{
	CString userName,userPwd;
	int iPos=Userinfo.Find(_T(':'));
	userName=Userinfo.Left(iPos);
	userPwd=Userinfo.Right(Userinfo.GetLength()-iPos-1);

	//连接数据库
	CDatabase db;
	CString tmpSql,numstr;
	try
	{

		db.Open(NULL,FALSE,FALSE,_T("ODBC;DSN=netdiskdb;UID=root;PWD=597119919"));
		if(!db.IsOpen())
		{
			AfxMessageBox(_T("数据库打开失败！"));
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
		tmp.Format(_T("数据库操作出错：%s"),pe->m_strError);
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
