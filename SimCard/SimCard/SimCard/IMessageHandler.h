#pragma once
#include "stdafx.h"
#include "SocketBussiness.h"

class IMessageHandler
{
public:
		/**
	 * 构造函数
	 */
	IMessageHandler()
	{

	}

	/**
	 * 析构函数
	 */
	virtual ~IMessageHandler()
	{

	}
	virtual void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm) = 0;
};