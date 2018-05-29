
// FeiyongDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Feiyong.h"
#include "FeiyongDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern bool start(std::string strjfFile, std::string strxhFile);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFeiyongDlg 对话框




CFeiyongDlg::CFeiyongDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFeiyongDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFeiyongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFeiyongDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFeiyongDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFeiyongDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFeiyongDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CFeiyongDlg 消息处理程序

BOOL CFeiyongDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFeiyongDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFeiyongDlg::OnPaint()
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
HCURSOR CFeiyongDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFeiyongDlg::OnBnClickedButton1()
{
	CString xhmx,JFMX;
	GetDlgItem(IDC_EDIT1)->GetWindowText(xhmx);
    GetDlgItem(IDC_EDIT2)->GetWindowText(JFMX);
	start(xhmx.GetBuffer(),JFMX.GetBuffer());
}


void CFeiyongDlg::OnBnClickedButton2()//jfmx
{
	OPENFILENAME opfn;     
	CHAR strFilename[MAX_PATH];//存放文件名     
	//初始化     
	ZeroMemory(&opfn, sizeof(OPENFILENAME));     
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
	//设置过滤     
	opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
	//默认过滤器索引设为1     
	opfn.nFilterIndex = 1;     
	//文件名的字段必须先把第一个字符设为\0
	opfn.lpstrFile = strFilename;     
	opfn.lpstrFile[0] = '\0';     
	opfn.nMaxFile = sizeof(strFilename);     
	//设置标志位，检查目录或文件是否存在     
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
	//opfn.lpstrInitialDir = NULL;     
	// 显示对话框让用户选择文件     
	if(GetOpenFileName(&opfn))     
	{
		std::string strfile = strFilename;
		GetDlgItem(IDC_EDIT1)->SetWindowText(strfile.c_str());
		
	}
}


void CFeiyongDlg::OnBnClickedButton3()//各月交费
{
	OPENFILENAME opfn;     
	CHAR strFilename[MAX_PATH];//存放文件名     
	//初始化     
	ZeroMemory(&opfn, sizeof(OPENFILENAME));     
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小     
	//设置过滤     
	opfn.lpstrFilter = "xlsx文件\0*.xlsx\0xls文件\0*.xls\0";     
	//默认过滤器索引设为1     
	opfn.nFilterIndex = 1;     
	//文件名的字段必须先把第一个字符设为\0
	opfn.lpstrFile = strFilename;     
	opfn.lpstrFile[0] = '\0';     
	opfn.nMaxFile = sizeof(strFilename);     
	//设置标志位，检查目录或文件是否存在     
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
	//opfn.lpstrInitialDir = NULL;     
	// 显示对话框让用户选择文件     
	if(GetOpenFileName(&opfn))     
	{
		std::string strfile = strFilename;
		GetDlgItem(IDC_EDIT2)->SetWindowText(strfile.c_str());

	}
}
