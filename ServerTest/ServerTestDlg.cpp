
// ServerTestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ServerTest.h"
#include "ServerTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerTestDlg 对话框



CServerTestDlg::CServerTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERTEST_DIALOG, pParent),
	m_sock(INVALID_SOCKET)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REGISTER, &CServerTestDlg::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_ADDKH, &CServerTestDlg::OnBnClickedAddkh)
	ON_BN_CLICKED(IDC_KHLIST, &CServerTestDlg::OnBnClickedKhlist)
	ON_BN_CLICKED(IDC_USERLIST, &CServerTestDlg::OnBnClickedUserlist)
	ON_BN_CLICKED(IDC_NEWCARD, &CServerTestDlg::OnBnClickedNewcard)
	ON_BN_CLICKED(IDC_CHUCARD, &CServerTestDlg::OnBnClickedChucard)
	ON_BN_CLICKED(IDC_XF, &CServerTestDlg::OnBnClickedXf)
	ON_BN_CLICKED(IDC_SIMLIST, &CServerTestDlg::OnBnClickedSimlist)
	ON_BN_CLICKED(IDC_ON1M, &CServerTestDlg::OnBnClickedOn1m)
	ON_BN_CLICKED(IDC_ON15D, &CServerTestDlg::OnBnClickedOn15d)
	ON_BN_CLICKED(IDC_KHSIMLIST, &CServerTestDlg::OnBnClickedKhsimlist)
	ON_BN_CLICKED(IDC_DU1M, &CServerTestDlg::OnBnClickedDu1m)
	ON_BN_CLICKED(IDC_LOGIN, &CServerTestDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_TINGJI, &CServerTestDlg::OnBnClickedTingji)
END_MESSAGE_MAP()


// CServerTestDlg 消息处理程序

BOOL CServerTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sAddr;
	ZeroMemory(&sAddr, sizeof(sAddr));
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(6086);
	sAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (SOCKET_ERROR == connect(m_sock, (sockaddr*)&sAddr, sizeof(sAddr)))
	{
		CString strErr;
		strErr.Format(_T("连接服务器失败:%d"), WSAGetLastError());
		MessageBox(strErr);
		return false;;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

byte csum(unsigned char *addr, int count)
{
	byte sum = 0;
	for (int i = 0; i< count; i++)
	{
		sum += (byte)addr[i];
	}
	return sum;
}

void CServerTestDlg::DealData(msgpack::sbuffer& sBuf)
{
	char* pCh = sBuf.data();
	int nLen = sBuf.size();
	byte pData[1024 * 32];
	memset(pData, 0, 1024 * 32);
	memcpy(pData, pCh + 6, nLen - 6);
	byte nSum = csum(pData, nLen - 6);
	sBuf.write("\x00", 1);
	memcpy(pCh + nLen, &nSum, 1);
	sBuf.write("\x0d", 1);
	nLen = sBuf.size();
	nLen -= 8;
	memcpy(pCh + 2, &nLen, 4);
	nLen += 8;

	send(m_sock, pCh, nLen, 0);

	recvlen = recv(m_sock, recvbuf, 1024 * 4 * 8, 0);
}

int CServerTestDlg::PaserserDate()
{
	if (recvlen < 6)
		return 0;

	if ((UCHAR)recvbuf[0] != 0xfb || (UCHAR)recvbuf[1] != 0xfc)//  没有数据开始标志
	{
		return -1;
	}

	DWORD dwFrameLen = *(INT*)(recvbuf + 2);
	if ((dwFrameLen + 8) > (DWORD)recvlen)
		return 0;

	byte nSum = recvbuf[6 + dwFrameLen];
	if (nSum != csum((unsigned char*)recvbuf + 6, dwFrameLen))
	{
		return -1;
	}

	if (0x0d != recvbuf[dwFrameLen + 7])
	{
		return -1;
	}
	recvlen = dwFrameLen;

	msgpack::unpacker unpack_;
	msgpack::object_handle result_;
	unpack_.reserve_buffer(recvlen);
	memcpy_s(unpack_.buffer(), recvlen, recvbuf + 6, recvlen/*nFrameLen*/);
	unpack_.buffer_consumed(recvlen/*nFrameLen*/);
	unpack_.next(result_);

	if (msgpack::type::ARRAY != result_.get().type)
	{
		return -1;
	}

	msgpack::object* pRootArray = result_.get().via.array.ptr;
	int nCmd = (pRootArray++)->as<int>();
	int nSubCmd = (pRootArray++)->as<int>();

	return dwFrameLen;
}

int CServerTestDlg::nRegister = 0;

void CServerTestDlg::OnBnClickedRegister()
{
	// TODO: 在此添加控件通知处理程序代码

	//[USER_DATA,USER_ADD,User,Password,Khid,Authority,Usertype,Fatherid]
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(5);
	_msgpack.pack((int)0x01);
	_msgpack.pack((int)0x01);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack_array(1);
	_msgpack.pack_array(6);

	char Temp[256] = { 0 };
	_stprintf_s(Temp, sizeof(Temp), _T("User%d"), nRegister);
	_msgpack.pack(Temp);

	_stprintf_s(Temp, sizeof(Temp), _T("Password%d"), nRegister);
	_msgpack.pack(Temp);

	_stprintf_s(Temp, sizeof(Temp), _T("Kh%d"), nRegister);
	_msgpack.pack(Temp);
	++nRegister;

	_msgpack.pack(1);
	_msgpack.pack(2);
	_msgpack.pack(1);

	DealData(sbuf);
}

static int nUserlist = 1;
void CServerTestDlg::OnBnClickedUserlist()
{
	// TODO: 在此添加控件通知处理程序代码
	// [KH_DATA,KH_ADD,Userid,Usertype,nTag,nPagesize,nAB,nIndex]
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(8);
	_msgpack.pack((int)0x01);
	_msgpack.pack((int)0x03);
	_msgpack.pack(2);
	_msgpack.pack(2);
	_msgpack.pack(nUserlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nUserlist - 1) * 10);
	nUserlist++;

	DealData(sbuf);
}

int CServerTestDlg::nKh = 0;
void CServerTestDlg::OnBnClickedAddkh()
{
	// TODO: 在此添加控件通知处理程序代码
	// [KH_DATA,KH_ADD,Userid,Usertype,Khmc,Fatherid,Lxfs,Ssdq,Jlxm,Dj,Bz]
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(5);
	_msgpack.pack((int)0x03);
	_msgpack.pack((int)0x01);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack_array(1);
	_msgpack.pack_array(7);
	char Temp[256] = { 0 };
	_stprintf_s(Temp, sizeof(Temp), _T("Kh%d"), nKh);
	_msgpack.pack(Temp);
	_msgpack.pack(1);
	_stprintf_s(Temp, sizeof(Temp), _T("Lxfs%d"), nKh);
	_msgpack.pack(Temp);
	_stprintf_s(Temp, sizeof(Temp), _T("Ssdq%d"), nKh);
	_msgpack.pack(Temp);
	_stprintf_s(Temp, sizeof(Temp), _T("Jlxm%d"), nKh);
	_msgpack.pack(Temp);
	_msgpack.pack(25.666);
	_stprintf_s(Temp, sizeof(Temp), _T("Bz%d"), nKh);
	_msgpack.pack(Temp);
	++nKh;

	DealData(sbuf);
}

static int nKhlist = 1;
void CServerTestDlg::OnBnClickedKhlist()
{
	// TODO: 在此添加控件通知处理程序代码
	// [KH_DATA,KH_ADD,Userid,Usertype,nTag,nPagesize,nAB,nIndex]
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(8);
	_msgpack.pack((int)0x03);
	_msgpack.pack((int)0x04);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(nKhlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nKhlist - 1) * 10);
	nKhlist++;

	DealData(sbuf);
}

static int nNewcard = 0;
void CServerTestDlg::OnBnClickedNewcard()
{
	// TODO: 在此添加控件通知处理程序代码
	for (size_t i = 0; i < 800000; i++)
	{
		msgpack::sbuffer sbuf;
		msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
		sbuf.write("\xfb\xfc", 6);
		_msgpack.pack_array(6);
		_msgpack.pack((int)0x08);
		_msgpack.pack((int)0x01);
		_msgpack.pack(1);
		_msgpack.pack(1);
		_msgpack.pack(1);
		_msgpack.pack_array(3);
		_msgpack.pack_array(5);
		char Temp[256] = { 0 };
		_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Iccid%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Dxzh%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Llchm%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Llclx%d"), nNewcard);
		_msgpack.pack(Temp);
		++nNewcard;
		_msgpack.pack_array(5);
		_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Iccid%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Dxzh%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Llchm%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Llclx%d"), nNewcard);
		_msgpack.pack(Temp);
		++nNewcard;
		_msgpack.pack_array(5);
		_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Iccid%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Dxzh%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Llchm%d"), nNewcard);
		_msgpack.pack(Temp);
		_stprintf_s(Temp, sizeof(Temp), _T("Llclx%d"), nNewcard);
		_msgpack.pack(Temp);
		++nNewcard;


		DealData(sbuf);
	}
}

static int nChucard = 0;
void CServerTestDlg::OnBnClickedChucard()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(6);
	_msgpack.pack((int)0x08);
	_msgpack.pack((int)0x02);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack_array(3);
	_msgpack.pack_array(4);
	char Temp[256] = { 0 };
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nChucard);
	_msgpack.pack(Temp);
	_msgpack.pack("Kh3");
	_msgpack.pack("2017-04-27");
	_msgpack.pack("Jlxm1");
	++nChucard;
	_msgpack.pack_array(4);
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nChucard);
	_msgpack.pack(Temp);
	_msgpack.pack("Kh3");
	_msgpack.pack("2017-04-27");
	_msgpack.pack("Jlxm1");
	++nChucard;
	_msgpack.pack_array(4);
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nChucard);
	_msgpack.pack(Temp);
	_msgpack.pack("Kh3");
	_msgpack.pack("2017-04-27");
	_msgpack.pack("Jlxm1");

	DealData(sbuf);
}

static int nXf = 0;
void CServerTestDlg::OnBnClickedXf()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(6);
	_msgpack.pack((int)0x08);
	_msgpack.pack((int)0x04);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack_array(4);
	_msgpack.pack_array(3);
	char Temp[256] = { 0 };
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nXf);
	_msgpack.pack(Temp);
	_msgpack.pack("2018-03-29");
	_msgpack.pack(1);
	++nXf;
	_msgpack.pack_array(3);
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nXf);
	_msgpack.pack(Temp);
	_msgpack.pack("2018-04-15");
	_msgpack.pack(1);
	++nXf;
	_msgpack.pack_array(3);
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nXf);
	_msgpack.pack(Temp);
	_msgpack.pack("2018-03-18");
	_msgpack.pack(1);
	++nXf;
	_msgpack.pack_array(3);
	_stprintf_s(Temp, sizeof(Temp), _T("Jrhm%d"), nXf);
	_msgpack.pack(Temp);
	_msgpack.pack("2018-02-20");
	_msgpack.pack(1);
	++nXf;

	DealData(sbuf);
}

int nSimlist = 1;
void CServerTestDlg::OnBnClickedSimlist()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(8);
	_msgpack.pack((int)0x02);
	_msgpack.pack((int)0x02);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(nSimlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nSimlist - 1) * 10);
	nSimlist++;

	DealData(sbuf);
}


void CServerTestDlg::OnBnClickedOn1m()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(9);
	_msgpack.pack((int)0x03);
	_msgpack.pack((int)0x09);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(nSimlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nSimlist - 1) * 10);
	_msgpack.pack_array(1);
	_msgpack.pack_array(1);
	_msgpack.pack(4);
	nSimlist++;

	DealData(sbuf);
}


void CServerTestDlg::OnBnClickedOn15d()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(9);
	_msgpack.pack((int)0x03);
	_msgpack.pack((int)0x08);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(nSimlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nSimlist - 1) * 10);
	_msgpack.pack_array(1);
	_msgpack.pack_array(1);
	_msgpack.pack(4);
	nSimlist++;

	DealData(sbuf);
}


void CServerTestDlg::OnBnClickedKhsimlist()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(9);
	_msgpack.pack((int)0x03);
	_msgpack.pack((int)0x05);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(nSimlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nSimlist - 1) * 10);
	_msgpack.pack_array(1);
	_msgpack.pack_array(1);
	_msgpack.pack(4);
	nSimlist++;

	DealData(sbuf);
}


void CServerTestDlg::OnBnClickedDu1m()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(9);
	_msgpack.pack((int)0x03);
	_msgpack.pack((int)10);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(nSimlist);
	_msgpack.pack(10);
	_msgpack.pack(2);
	_msgpack.pack((nSimlist - 1) * 10);
	_msgpack.pack_array(1);
	_msgpack.pack_array(1);
	_msgpack.pack(4);
	nSimlist++;

	DealData(sbuf);
}


void CServerTestDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(8);
	_msgpack.pack((int)0x01);
	_msgpack.pack((int)0x02);


	DealData(sbuf);
}


void CServerTestDlg::OnBnClickedTingji()
{
	// TODO: 在此添加控件通知处理程序代码
	// 1064946202200
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> _msgpack(&sbuf);
	sbuf.write("\xfb\xfc", 6);
	_msgpack.pack_array(6);
	_msgpack.pack((int)0x10);
	_msgpack.pack((int)0x01);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack(1);
	_msgpack.pack_array(1);
	_msgpack.pack_array(2);
	_msgpack.pack("1064946202200");
	_msgpack.pack("河南云呼通讯科技有限公司");

	DealData(sbuf);
}
