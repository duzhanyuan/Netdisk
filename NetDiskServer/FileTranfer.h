#pragma once

// CFileTranfer command target

class CFileTranfer : public CObject
{
	
public:
	CFileTranfer();
	virtual ~CFileTranfer();
	
	bool writeFile(CString filePath);
};


