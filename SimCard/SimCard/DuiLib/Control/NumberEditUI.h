#pragma once

namespace DuiLib
{
class CNumberEditUI :
	public CEditUI
{
public:
	CNumberEditUI(void)
	{

	}
	~CNumberEditUI(void)
	{

	}
	LPCTSTR GetClass() const
	{
		// �����������ȥ����ͷ����C�ַ�
		return _T("NumberEdit");
	}

	

	LPVOID GetInterface(LPCTSTR pstrName)
	{
		// �����xml�ؼ�������
		if (_tcscmp(pstrName, _T("NumberEdit")) == 0)
			return static_cast<CNumberEditUI*>(this);


		return CControlUI::GetInterface(pstrName);
	}
};
};

