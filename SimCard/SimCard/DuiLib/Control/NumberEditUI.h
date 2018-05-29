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
		// 这个是类名，去掉开头的是C字符
		return _T("NumberEdit");
	}

	

	LPVOID GetInterface(LPCTSTR pstrName)
	{
		// 这个是xml控件的名称
		if (_tcscmp(pstrName, _T("NumberEdit")) == 0)
			return static_cast<CNumberEditUI*>(this);


		return CControlUI::GetInterface(pstrName);
	}
};
};

