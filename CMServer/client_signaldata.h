#pragma once
#include <iostream>
#include <string>
#include <list>

typedef void(*PTIoRequestSuccess)(DWORD dwTranstion, void* key, void* buf);
typedef void(*PTIoRequestFailed)(void* key, void* buf);

typedef struct _client_buf_t
{
	WSAOVERLAPPED ol;
	PTIoRequestFailed pfnFailed;
	PTIoRequestSuccess pfnSuccess;
	struct _client_sock* pRelateClientSock;
	WSABUF wsaBuf;
	DWORD dwRecvedCount;
	DWORD dwSendedCount;
	int datalen;
}CLIENT_BUF_T;
#define SIZE_OF_CLIENT_BUF_T sizeof(CLIENT_BUF_T)

typedef struct _client_buf
{
public:
	WSAOVERLAPPED ol;
	PTIoRequestFailed pfnFailed;
	PTIoRequestSuccess pfnSuccess;
	struct _client_sock* pRelateClientSock;
	WSABUF wsaBuf;
	DWORD dwRecvedCount;
	DWORD dwSendedCount;
	int datalen;
	TCHAR data[1];

	void Init(DWORD dwSize)
	{
		memset(&ol, 0x00, sizeof(ol));
		dwRecvedCount = 0;
		dwSendedCount = 0;
		datalen = dwSize;
		memset(data, 0x00, dwSize);
	}

	void SetIoRequestFunction(PTIoRequestFailed _pfnFailed, PTIoRequestSuccess _pfnSuccess)
	{
		pfnFailed = _pfnFailed;
		pfnSuccess = _pfnSuccess;
	}
}CLIENT_BUF, BUFFER_OBJ;

typedef struct _client_sock_t
{
	int nKey;
	SOCKET sock;
	std::list<CLIENT_BUF*>* lstSend;
	CRITICAL_SECTION cs;
	WSABUF wsabuf[2];
	DWORD dwBufsize;
	DWORD dwWritepos;
	DWORD dwReadpos;
	DWORD dwRightsize;
	DWORD dwLeftsize;
	bool bFull;
	bool bEmpty;
	byte sum;
	volatile long nRef;
}CLIENT_SOCK_T;
#define SIZE_OF_CLIENT_SOCK_T sizeof(CLIENT_SOCK_T)

typedef struct _client_sock
{
	int nKey;
	SOCKET sock;
	std::list<CLIENT_BUF*>* lstSend;
	CRITICAL_SECTION cs;
	WSABUF wsabuf[2];
	DWORD dwBufsize;
	DWORD dwWritepos;
	DWORD dwReadpos;
	DWORD dwRightsize;
	DWORD dwLeftsize;
	bool bFull;
	bool bEmpty;
	byte sum;
	volatile long nRef;
	TCHAR buf[1];

	void Init(DWORD _Bufsize)
	{
		if (INVALID_SOCKET != sock)
		{
			closesocket(sock);
			sock = INVALID_SOCKET;
		}
		dwBufsize = _Bufsize;
		dwWritepos = 0;
		dwReadpos = 0;
		dwRightsize = 0;
		dwLeftsize = 0;
		nKey = 0;
		bFull = false;
		bEmpty = true;
		sum = 0;
		nRef = 1;
		InitializeCriticalSection(&cs);
		lstSend->clear();
	}

	void Clear()
	{
		if (NULL != lstSend)
			delete lstSend;
	}

	void InitRLsize()
	{
		if (bFull)
		{
			dwRightsize = 0;
			dwLeftsize = 0;
		}
		else if (dwReadpos <= dwWritepos)
		{
			dwRightsize = dwBufsize - dwWritepos;
			dwLeftsize = dwReadpos;
		}
		else
		{
			dwRightsize = dwReadpos - dwWritepos;
			dwLeftsize = 0;
		}
	}

	void InitWSABUFS()
	{
		InitRLsize();
		wsabuf[0].buf = buf + dwWritepos;
		wsabuf[0].len = dwRightsize;
		wsabuf[1].buf = buf;
		wsabuf[1].len = dwLeftsize;
	}

	void InitWRpos(DWORD dwTranstion)
	{
		if (dwTranstion <= 0)
			return;

		if (bFull)
			return;

		dwWritepos = (dwTranstion > dwRightsize) ? (dwTranstion - dwRightsize) : (dwWritepos + dwTranstion);
		bFull = (dwWritepos == dwReadpos);
		bEmpty = false;
	}

	byte csum(unsigned char *addr, int count)
	{
		for (int i = 0; i< count; i++)
		{
			sum += (byte)addr[i];
		}
		return sum;
	}

	DWORD GetCmdDataLength()
	{
		if (bEmpty)
			return 0;

		DWORD dwNum = 0;
		sum = 0;
		if (dwWritepos > dwReadpos)
		{
			DWORD nDataNum = dwWritepos - dwReadpos;
			if (nDataNum < 6)
				return 0;

			if ((UCHAR)buf[dwReadpos] != 0xfb || (UCHAR)buf[dwReadpos + 1] != 0xfc)//  没有数据开始标志
			{
				closesocket(sock);
				sock = INVALID_SOCKET;
				return 0;
			}

			DWORD dwFrameLen = *(INT*)(buf + dwReadpos + 2);
			if ((dwFrameLen + 8) > nDataNum)
				return 0;

			byte nSum = buf[dwReadpos + 6 + dwFrameLen];
			if (nSum != csum((unsigned char*)buf + dwReadpos + 6, dwFrameLen))
			{
				closesocket(sock);
				sock = INVALID_SOCKET;
				return 0;
			}

			if (0x0d != buf[dwReadpos + dwFrameLen + 7])
			{
				closesocket(sock);
				sock = INVALID_SOCKET;
				return 0;
			}

			dwReadpos += 6;

			return dwFrameLen + 2;
		}
		else
		{
			DWORD dwLeft = dwBufsize - dwReadpos;
			if (dwLeft < 6)
			{
				char subchar[6];
				memcpy(subchar, buf + dwReadpos, dwLeft);
				memcpy(subchar + dwLeft, buf, 6 - dwLeft);

				if ((UCHAR)subchar[0] != 0xfb || (UCHAR)subchar[1] != 0xfc)//  没有数据开始标志
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return 0;
				}

				DWORD dwFrameLen = *(INT*)(subchar + 2);
				if ((dwFrameLen + 8) > (dwLeft + dwWritepos)) // 消息太长了
				{
					if (bFull)
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
					}
					return 0;
				}

				DWORD dwIndex = (dwReadpos + 6 - dwBufsize);
				byte nSum = buf[dwIndex + dwFrameLen];
				if (nSum != csum((unsigned char*)buf + dwIndex, dwFrameLen))
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return 0;
				}

				if (0x0d != buf[dwIndex + dwFrameLen + 1])
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return 0;
				}

				dwReadpos = (dwReadpos + 6) > dwBufsize ? (dwReadpos + 6 - dwBufsize) : (dwReadpos + 6);

				return dwFrameLen + 2;
			}
			else
			{
				if ((UCHAR)buf[dwReadpos] != 0xfb || (UCHAR)buf[dwReadpos + 1] != 0xfc)//  没有数据开始标志
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return 0;
				}

				DWORD dwFrameLen = *(INT*)(buf + dwReadpos + 2);
				if ((dwFrameLen + 8) > (dwLeft + dwWritepos)) // 消息太长了
				{
					if (bFull)
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
					}
					return 0;
				}

				byte nSum = buf[(dwReadpos + 6 + dwFrameLen) >= dwBufsize ? (dwReadpos + 6 + dwFrameLen - dwBufsize) : (dwReadpos + 6 + dwFrameLen)];
				if ((dwFrameLen + 6) > dwLeft)
				{
					csum((unsigned char*)buf + dwReadpos + 6, dwLeft - 6);
					csum((unsigned char*)buf, dwFrameLen - dwLeft + 6);
					if (nSum != sum)
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
						return 0;
					}
				}
				else
				{
					if (nSum != csum((unsigned char*)buf + dwReadpos + 6, dwFrameLen))
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
						return 0;
					}
				}

				if (0x0d != buf[(dwReadpos + dwFrameLen + 7) >= dwBufsize ? (dwReadpos + dwFrameLen + 7 - dwBufsize) : (dwReadpos + dwFrameLen + 7)])
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return 0;
				}

				dwReadpos = (dwReadpos + 6) > dwBufsize ? (dwReadpos + 6 - dwBufsize) : (dwReadpos + 6);

				return dwFrameLen + 2;
			}
		}
		return 0;
	}

	int Read(TCHAR* _buf, DWORD dwNum)
	{
		if (dwNum <= 0)
			return 0;

		if (bEmpty)
			return 0;

		bFull = false;
		if (dwReadpos < dwWritepos)
		{
			memcpy(_buf, buf + dwReadpos, dwNum);
			dwReadpos += dwNum;
			bEmpty = (dwReadpos == dwWritepos);
			return dwNum;
		}
		else if (dwReadpos >= dwWritepos)
		{
			DWORD leftcount = dwBufsize - dwReadpos;
			if (leftcount > dwNum)
			{
				memcpy(_buf, buf + dwReadpos, dwNum);
				dwReadpos += dwNum;
				bEmpty = (dwReadpos == dwWritepos);
				return dwNum;
			}
			else
			{
				memcpy(_buf, buf + dwReadpos, leftcount);
				dwReadpos = dwNum - leftcount;
				memcpy(_buf + leftcount, buf, dwReadpos);
				bEmpty = (dwReadpos == dwWritepos);
				return leftcount + dwReadpos;
			}
		}

		return 0;
	}

	void AddRef()
	{
		InterlockedIncrement(&nRef);
	}

	void DecRef()
	{
		InterlockedDecrement(&nRef);
	}

	bool CheckSend(CLIENT_BUF* data)
	{
		EnterCriticalSection(&cs);
		if (lstSend->empty())
		{
			lstSend->push_front(data);
			LeaveCriticalSection(&cs);
			return true;
		}
		else
		{
			lstSend->push_front(data);
			LeaveCriticalSection(&cs);
			return false;
		}
	}

	CLIENT_BUF* GetNextData()
	{
		CLIENT_BUF* data = NULL;;
		EnterCriticalSection(&cs);
		lstSend->pop_back();
		if (!lstSend->empty())
			data = lstSend->back();
		LeaveCriticalSection(&cs);
		return data;
	}
}CLIENT_SOCK, SOCKET_OBJ;
#define SIZE_OF_CLIENT_SOCK sizeof(CLIENT_SOCK)