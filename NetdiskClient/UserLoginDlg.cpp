// UserLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetdiskClient.h"
#include "UserLoginDlg.h"
#include "afxdialogex.h"
#define TIMER_GETLIST 500


// CUserLoginDlg dialog

IMPLEMENT_DYNAMIC(CUserLoginDlg, CDialog)

CUserLoginDlg::CUserLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLoginDlg::IDD, pParent)
{

	m_strName = _T("user");
	m_strPasswd = _T("123456");
	m_ndApp=(CNetdiskClientApp*)AfxGetApp() ;
}

CUserLoginDlg::~CUserLoginDlg()
{
}

void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AUTOLOGIN, m_bAutoLogin);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_USERPWD, m_strPasswd);
	DDX_Control(pDX, IDC_STATIC_LOGINMSG, m_StaticState);
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CUserLoginDlg::OnBnClickedBtnLogin)

END_MESSAGE_MAP()


// CUserLoginDlg message handlers


BOOL CUserLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//SetTimer(TIMER_GETLIST,300,NULL);

	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CUserLoginDlg::OnTimer(UINT nIdEvent)
{
	if(nIdEvent!=TIMER_GETLIST)
	{
		CDialog::OnTimer(nIdEvent);
		return;
	}
	//////////////////////////

	ASSERT(KillTimer(TIMER_GETLIST));
	m_StaticState.SetWindowText(_T("正在连接，请稍后......"));
 
	try
	{
		
	}
	catch(CDBException* pe)
	{
		AfxMessageBox(pe->m_strError);
		pe->Delete();
	}
	m_StaticState.SetWindowText(_T(""));
  
	////////////////////////
	CDialog::OnTimer(nIdEvent);
	return;
}
//登录验证
void CUserLoginDlg::OnBnClickedBtnLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 
	//启动客户端
	if(!m_Client.Start())
		return;

	//判断用户名是否为空
	if(m_strName==_T(""))
	{
		AfxMessageBox(_T("用户名为空，请重新输入！"));
		return;
	}

	//判断密码是否为空
	if(m_strPasswd==_T(""))
	{
		AfxMessageBox(_T("密码为空，请重新输入！"));
		return;
	}

	//判断用户名中是否存在非法字符
	char illegalChar[]={'!','@','#','$','%','^','&','*','(',')','{','}',
						'[',']','\'','\\','|','?','\"','/','>','<','.',
						':',';','~'};
	char c_ascll;
	for(int i=0;i<m_strName.GetLength();i++)
	{
		c_ascll=m_strName[i];
		for(int j=0;j<27;j++)
			if(c_ascll == illegalChar[j])
			{
				AfxMessageBox(_T("用户名中包含非法字符，请重新输入！"));
				return;
			}
		//c_ascll[0]=m_strName[i];
		//c_ascll[1]=m_strName[i+1];
		////c_ascll=pChar[i];
		//if((c_ascll>=33&&c_ascll<=47)||(c_ascll>=58&&c_ascll<=64)||(c_ascll>=91&&c_ascll<=96)||(c_ascll>=123&&c_ascll<=126))
		//{
		//		AfxMessageBox(_T("用户名中包含非法字符，请重新输入！"));
		//		return;
		//}
	}
	m_Client.SendMsgToServ(m_strName+_T(":")+m_strPasswd);

	int retvalue=m_Client.RecvloginMsg();
	switch(retvalue){
	case LOGIN_SUCCESS:
		////保存连接成功的socket
		m_ndApp->m_TmpClient=m_Client.m_Client.sock;
		CDialog::OnOK();
		break;
	case -1:
		AfxMessageBox(_T("程序出错，重新输入！"));
		m_Client.Clean();
		break;
	case OTHER_ERROR:
		AfxMessageBox(_T("服务器操作失败！"));
		m_Client.Clean();
		break;

	case USERNAME_NOTEXIST:
		AfxMessageBox(_T("用户名不存在！"));
		m_Client.Clean();		
		break;

	case PASSWD_ERROR:
		AfxMessageBox(_T("密码错误，重新输入！"));
		m_Client.Clean();		
		break;
	}
	return ;
}
 