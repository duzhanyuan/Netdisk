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

	m_strName = _T("");
	m_strPasswd = _T("");
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
	m_StaticState.SetWindowText(_T("�������ӣ����Ժ�......"));
 
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
//��¼��֤
void CUserLoginDlg::OnBnClickedBtnLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 
	//�����ͻ���
	m_Client.Start();

	//�ж��û����Ƿ�Ϊ��
	if(m_strName==_T(""))
	{
		AfxMessageBox(_T("�û���Ϊ�գ����������룡"));
		return;
	}

	//�ж������Ƿ�Ϊ��
	if(m_strPasswd==_T(""))
	{
		AfxMessageBox(_T("����Ϊ�գ����������룡"));
		return;
	}

	//�ж��û������Ƿ���ڷǷ��ַ�
	unsigned char *c_ascll;
	BYTE *pChar ;
	pChar= (BYTE*)m_strName.GetBuffer(0);
	for(int i=0;i<m_strName.GetLength();i++)
	{
		//c_ascll[0]=m_strName[i];
		//c_ascll[1]=m_strName[i+1];
		////c_ascll=pChar[i];
		//if((c_ascll>=33&&c_ascll<=47)||(c_ascll>=58&&c_ascll<=64)||(c_ascll>=91&&c_ascll<=96)||(c_ascll>=123&&c_ascll<=126))
		//{
		//		AfxMessageBox(_T("�û����а����Ƿ��ַ������������룡"));
		//		return;
		//}
	}
	m_Client.SendMsgToServ(m_strName+_T(":")+m_strPasswd);

	int retvalue=m_Client.RecvMsg();
	switch(retvalue){
	case LOGIN_SUCCESS:
		CDialog::OnOK();
		break;
	case -1:
		AfxMessageBox(_T("��������������룡"));
		break;
	case OTHER_ERROR:
		AfxMessageBox(_T("����������ʧ�ܣ�"));
		break;

	case USERNAME_NOTEXIST:
		AfxMessageBox(_T("�û��������ڣ�"));
		break;

	case PASSWD_ERROR:
		AfxMessageBox(_T("��������������룡"));
		break;
	}
	m_Client.Clean();
	return ;
}
 