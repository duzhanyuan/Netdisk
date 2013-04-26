// SysIcon.h: interface for the CSysIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSICON_H__954BE09E_CA69_4EE9_A5DA_B5C940CA4305__INCLUDED_)
#define AFX_SYSICON_H__954BE09E_CA69_4EE9_A5DA_B5C940CA4305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSysIcon							//系统的文件图标
{
public:
	int GetExtIconID(LPCTSTR szExt);		//得到扩展名的图标

	CImageList m_ImageSmallList;
	CImageList m_ImageLargeList;
	CStringArray m_arExtName;

	CSysIcon();
	virtual ~CSysIcon();

private:
	int AddIcon(LPCTSTR szExt);			//添加
	int FindIconID(LPCTSTR szExt);		//根据扩展名找图标
};

#endif // !defined(AFX_SYSICON_H__954BE09E_CA69_4EE9_A5DA_B5C940CA4305__INCLUDED_)
