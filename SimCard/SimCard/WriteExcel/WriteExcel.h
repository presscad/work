// WriteExcel.h : WriteExcel DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWriteExcelApp
// �йش���ʵ�ֵ���Ϣ������� WriteExcel.cpp
//

class CWriteExcelApp : public CWinApp
{
public:
	CWriteExcelApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
