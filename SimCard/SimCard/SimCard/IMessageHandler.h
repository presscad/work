#pragma once
#include "stdafx.h"
#include "SocketBussiness.h"

class IMessageHandler
{
public:
		/**
	 * ���캯��
	 */
	IMessageHandler()
	{

	}

	/**
	 * ��������
	 */
	virtual ~IMessageHandler()
	{

	}
	virtual void HandleClick(TNotifyUI& msg, CPaintManagerUI& pm) = 0;
};