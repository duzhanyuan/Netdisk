// SysIcon.cpp: implementation of the CSysIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysIcon::CSysIcon()
{
	m_ImageSmallList.Create(16,16,ILC_COLOR32|ILC_MASK,1,1000);
	m_ImageLargeList.Create(32,32,ILC_COLOR32|ILC_MASK,1,1000);

}

CSysIcon::~CSysIcon()
{
	m_ImageSmallList.Detach();
	m_ImageLargeList.Detach();

}

int CSysIcon::FindIconID(LPCTSTR szExt)
{
	TCHAR szFile[_MAX_FNAME];
	_tcscpy(szFile,szExt);
	TCHAR *p=NULL;
	if(p=_tcsrchr(szFile,'.'))
	{
		p++;
		if(*p=='\0')
		{
			_tcscpy(szFile,_T("*"));
			p=szFile;
		}
	}
	else
	{
		_tcscpy(szFile,_T("*"));
		p=szFile;
	}

	int nSize=m_arExtName.GetSize();
	for(int i=0;i<nSize;i++)
		if(m_arExtName.GetAt(i).CompareNoCase(p)==0)
			return i;

	return -1;
}

int CSysIcon::GetExtIconID(LPCTSTR szExt)
{
	int nIndex=FindIconID(szExt);//根据扩展名找寻
	if(nIndex==-1)//找不到
		nIndex=AddIcon(szExt);//添加
	return nIndex;
}

int CSysIcon::AddIcon(LPCTSTR szExt)
{
	SHFILEINFO shFi;

	memset(&shFi,0,sizeof(shFi));

	SHGetFileInfo(szExt,FILE_ATTRIBUTE_NORMAL,&shFi,sizeof(shFi),SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_TYPENAME|SHGFI_ICON|SHGFI_SMALLICON);

	if(shFi.hIcon==NULL)
		return -1;

	HICON hIcon=CopyIcon(shFi.hIcon);//??????????????????????????????????hIcon没有关闭
	DestroyIcon(shFi.hIcon);
	m_ImageSmallList.Add(hIcon);

	SHGetFileInfo(szExt,FILE_ATTRIBUTE_NORMAL,&shFi,sizeof(shFi),SHGFI_USEFILEATTRIBUTES|SHGFI_DISPLAYNAME|SHGFI_TYPENAME|SHGFI_ICON|SHGFI_LARGEICON);
	if(shFi.hIcon==NULL)
		return -1;

	hIcon=CopyIcon(shFi.hIcon);//??????????????????????????????????hIcon没有关闭
	DestroyIcon(shFi.hIcon);
	m_ImageLargeList.Add(hIcon);

	TCHAR szFile[_MAX_FNAME];
	_tcscpy(szFile,szExt);
	TCHAR *p=NULL,*pExt=szFile;
	while(p=_tcsrchr(pExt,'.'))
		pExt=p+1;

	if(pExt==szFile)
		_tcscpy(szFile,_T("*"));

	return m_arExtName.Add(pExt);
}

