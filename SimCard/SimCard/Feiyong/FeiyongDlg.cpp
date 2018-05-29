
// FeiyongDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFeiyongDlg �Ի���




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


// CFeiyongDlg ��Ϣ�������

BOOL CFeiyongDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFeiyongDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	CHAR strFilename[MAX_PATH];//����ļ���     
	//��ʼ��     
	ZeroMemory(&opfn, sizeof(OPENFILENAME));     
	opfn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С     
	//���ù���     
	opfn.lpstrFilter = "xlsx�ļ�\0*.xlsx\0xls�ļ�\0*.xls\0";     
	//Ĭ�Ϲ�����������Ϊ1     
	opfn.nFilterIndex = 1;     
	//�ļ������ֶα����Ȱѵ�һ���ַ���Ϊ\0
	opfn.lpstrFile = strFilename;     
	opfn.lpstrFile[0] = '\0';     
	opfn.nMaxFile = sizeof(strFilename);     
	//���ñ�־λ�����Ŀ¼���ļ��Ƿ����     
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
	//opfn.lpstrInitialDir = NULL;     
	// ��ʾ�Ի������û�ѡ���ļ�     
	if(GetOpenFileName(&opfn))     
	{
		std::string strfile = strFilename;
		GetDlgItem(IDC_EDIT1)->SetWindowText(strfile.c_str());
		
	}
}


void CFeiyongDlg::OnBnClickedButton3()//���½���
{
	OPENFILENAME opfn;     
	CHAR strFilename[MAX_PATH];//����ļ���     
	//��ʼ��     
	ZeroMemory(&opfn, sizeof(OPENFILENAME));     
	opfn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С     
	//���ù���     
	opfn.lpstrFilter = "xlsx�ļ�\0*.xlsx\0xls�ļ�\0*.xls\0";     
	//Ĭ�Ϲ�����������Ϊ1     
	opfn.nFilterIndex = 1;     
	//�ļ������ֶα����Ȱѵ�һ���ַ���Ϊ\0
	opfn.lpstrFile = strFilename;     
	opfn.lpstrFile[0] = '\0';     
	opfn.nMaxFile = sizeof(strFilename);     
	//���ñ�־λ�����Ŀ¼���ļ��Ƿ����     
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;     
	//opfn.lpstrInitialDir = NULL;     
	// ��ʾ�Ի������û�ѡ���ļ�     
	if(GetOpenFileName(&opfn))     
	{
		std::string strfile = strFilename;
		GetDlgItem(IDC_EDIT2)->SetWindowText(strfile.c_str());

	}
}
