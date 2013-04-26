// ServerContrl.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "ServerContrl.h"
#include "ServIndex.h"

char Buffer[MAX_BUFFER_LEN];

// CServerContrl

CServerContrl::CServerContrl()
{
	m_strDiskRootPath=_T("E:\\企业网盘目录");
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
							//获取该用户的目录索引信息
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
							CString path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
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

							CString path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
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
					case DELETEFILE:
						{
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;
							
							CString path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							//判断删除的是文件还是文件夹
							DWORD value = GetFileAttributes(path);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
								pServCtrl->	RecursiveDelete(path);
							else
								DeleteFile(path);

							//更新用户索引记录
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);
							break;
						}
					case REFRESH:
						{
							//更新用户索引记录
							CServIndex* ServIndex=new CServIndex();
							ServIndex->UpdateIndex(pServCtrl->m_strUserName);

							break;
						}
					case DOWNLOADFILE:
						{
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;

							CString path;
							path=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							//发送文件信息
							pServCtrl->SendFile(pServCtrl->m_fdAllSocket.fd_array[i],path);

							break;
						}
					case DOWNLOADCATALOG:
						{
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;
							CString tmpPath=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg;
							//发送目录信息
							pServCtrl->RecursiveSend(pServCtrl->m_fdAllSocket.fd_array[i],tmpPath,tmpPath);

							//发送目录结束信息
							ZeroMemory(&sendPack,sizeof(sendPack));
							CString endSending=_T("send_cat_end");
							int n=endSending.GetLength();
							int len=WideCharToMultiByte(CP_ACP,0,endSending,endSending.GetLength(),NULL,0,NULL,NULL);
							WideCharToMultiByte(CP_ACP,0,endSending,endSending.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
							sendPack.sContent[len]='\0';
							sendPack.nContentLen=len;
							sendPack.iType=1;
							sendPack.nPackLen=sizeof(sendPack);
							int result=send(pServCtrl->m_fdAllSocket.fd_array[i],(char*)&sendPack,sendPack.nPackLen,0);

							if(result==SOCKET_ERROR)
							{
								AfxMessageBox(_T("发送目录结束包失败！"));
							}
							break;
						}
					case MOVEFILE:
						{
							CString m_strUserMsg;//用户发过来的目录路径
							m_strUserMsg=recvPack.sContent;
							
							int pos=m_strUserMsg.Find('+');
							CString srcPath=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg.Left(pos);
							CString desPath=pServCtrl->m_strDiskRootPath+_T("\\")+m_strUserMsg.Right(m_strUserMsg.GetLength()-pos-1);

							//判断删除的是文件还是文件夹
							DWORD value = GetFileAttributes(srcPath);
							if( (value & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
							{
								pServCtrl->RecursiveMove(srcPath,desPath);
							}
							else
								MoveFile(srcPath,desPath);

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
//发送文件
void CServerContrl::SendFile(SOCKET cSocket,CString filePath)
{
	//发送文件数据
	FILE* infile;
	char c_path[1024];

	memset(c_path,0,filePath.GetLength()*2+1);
	USES_CONVERSION; 
	strcpy((LPSTR)c_path,OLE2A(filePath.LockBuffer()));
	infile=fopen(c_path,"rb");

	u_long ulFlagCount = 0;            //记录读了多少数据
	u_long ulFileRead = 0;					//一次实际读取的字节数
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
			AfxMessageBox(_T("发送文件失败！"));
		}
		else
		{
			ulFlagCount+=ulFileRead;
		}
	}

	fclose(infile);
	//发送文件结束包
	ZeroMemory(&sendPack,sizeof(sendPack));
	sendPack.nPackLen=sizeof(sendPack);
	CString strTmp=_T("send_file_end");
	int n=strTmp.GetLength();
	int len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
	sendPack.sContent[len]='\0';
	sendPack.nContentLen=len;
	sendPack.iType=2;
	result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
	if(result==SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送文件结束包失败！"));
	}
}
//递归移动目录
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
		//bIsDots为真表示是.或..
		bool bIsDots = (wcscmp(FindFileData.cFileName,_T(".")) == 0 || wcscmp(FindFileData.cFileName,_T("..")) == 0);
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && bIsDots == false)
		{
			//是目录,就再进入该目录
			wcscpy_s(wcPath,MAX_PATH,srcPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			RecursiveMove(wcPath,desPath+_T("\\")+FindFileData.cFileName);
			//寻找下一文件
			bContinue = FindNextFile(hFindFile,&FindFileData);
			continue;
		}

		if (bIsDots == false)
		{
			//是文件删除之
			wcscpy_s(wcPath,MAX_PATH,srcPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			MoveFile(wcPath,desPath+_T("\\")+FindFileData.cFileName);
		}
		//寻找下一文件
		bContinue = FindNextFile(hFindFile,&FindFileData);

	}

	FindClose(hFindFile);

	//删除空目录
	RemoveDirectory(srcPath);
	return true;
}
//递归目录发送文件夹
void CServerContrl::RecursiveSend(SOCKET cSocket,CString szPath,CString rootPath)
{
	CFileFind ff;  
	CString path = szPath;  
	int result;
	DataPackage sendPack;

	if(path.Right(1) != _T("\\"))  
		path += _T("\\");  


	path += _T("*.*");  
	BOOL res = ff.FindFile(path);  

	while(res)  
	{  
		res = ff.FindNextFile();  
		//是文件时直接发送文件
		if (!ff.IsDots() && !ff.IsDirectory())  
		{
			ZeroMemory(&sendPack,sizeof(sendPack));
			sendPack.iType=2; //发送文件
			sendPack.nPackLen=sizeof(sendPack);
			CString strTmp=ReturnCientPath(ff.GetFilePath(),rootPath);
			int n=strTmp.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			//如果发送的是文件，先发送一个确认包
			result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);
			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送文件结束包失败！"));
			}
			SendFile(cSocket,ff.GetFilePath());
		}
		else if (ff.IsDots())  
			continue;  
		else if (ff.IsDirectory())  
		{  
			//发送目录名称信息
			ZeroMemory(&sendPack,sizeof(sendPack));
			path = ff.GetFilePath(); 
			CString strTmp=ReturnCientPath(ff.GetFilePath(),rootPath);
			int n=strTmp.GetLength();
			int len=WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength(),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,strTmp,strTmp.GetLength()+1,sendPack.sContent,len+1,NULL,NULL);
			sendPack.sContent[len]='\0';
			sendPack.nContentLen=len;
			sendPack.iType=1;
			sendPack.nPackLen=sizeof(sendPack);
			result=send(cSocket,(char*)&sendPack,sendPack.nPackLen,0);

			if(result==SOCKET_ERROR)
			{
				AfxMessageBox(_T("发送文件结束包失败！"));
			}
			//是目录时继续递归，发送该目录下的文件  
			RecursiveSend(cSocket,path,rootPath);  
 
		}  
	}  

	ff.Close();

}

//解析服务器路径为客户端路径
CString CServerContrl::ReturnCientPath(CString servPath,CString rootPath)
{
	int pos=rootPath.GetLength();
	return servPath.Right(servPath.GetLength()-pos-1);
}
//递归删除文件夹
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
		//bIsDots为真表示是.或..
		bool bIsDots = (wcscmp(FindFileData.cFileName,_T(".")) == 0 || wcscmp(FindFileData.cFileName,_T("..")) == 0);
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && bIsDots == false)
		{
			//是目录,就再进入该目录
			wcscpy_s(wcPath,MAX_PATH,szPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			RecursiveDelete(wcPath);
			//寻找下一文件
			bContinue = FindNextFile(hFindFile,&FindFileData);
			continue;
		}

		if (bIsDots == false)
		{
			//是文件删除之
			wcscpy_s(wcPath,MAX_PATH,szPath);
			wcscat_s(wcPath,MAX_PATH,_T("\\"));
			wcscat_s(wcPath,MAX_PATH,FindFileData.cFileName);
			DeleteFile(wcPath);
		}
		//寻找下一文件
		bContinue = FindNextFile(hFindFile,&FindFileData);

	}

	FindClose(hFindFile);

	//删除空目录
	RemoveDirectory(szPath);
	return true;
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
