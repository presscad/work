#pragma once
#include "stdafx.h"
#include "Messagebox.h"
#include <time.h>
#include "MainPage.h"
#include "YonghuCardPage.h"
#include "XsrqCardPage.h"
#include "KhRqTjPage.h"
CMainPage * g_MainPage;
CYonghuCardPage * g_YonghuCardPage;
CXsrqCardPage* g_XsrqCardPage;
CKhRqTjPage* g_pKhrqTjPage;
int MyMessageBox(HWND hWnd,std::string strMsg,std::string strTip)
{
	CMessagebox* pDlg = new CMessagebox(strMsg,strTip);
	pDlg->Create(hWnd,"", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE|WS_EX_TOPMOST);
	pDlg->CenterWindow();
	return  pDlg->ShowModal();
}

time_t StringToDatetime(string str)
{
	char *cha = (char*)str.data();             // ��stringת����char*��
	tm tm_;                                    // ����tm�ṹ�塣
	int year, month, day, hour, minute, second;// ����ʱ��ĸ���int��ʱ������
	sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// ��string�洢������ʱ�䣬ת��Ϊint��ʱ������
	tm_.tm_year = year - 1900;                 // �꣬����tm�ṹ��洢���Ǵ�1900�꿪ʼ��ʱ�䣬����tm_yearΪint��ʱ������ȥ1900��
	tm_.tm_mon = month - 1;                    // �£�����tm�ṹ����·ݴ洢��ΧΪ0-11������tm_monΪint��ʱ������ȥ1��
	tm_.tm_mday = day;                         // �ա�
	tm_.tm_hour = hour;                        // ʱ��
	tm_.tm_min = minute;                       // �֡�
	tm_.tm_sec = second;                       // �롣
	tm_.tm_isdst = 0;                          // ������ʱ��
	time_t t_ = mktime(&tm_);                  // ��tm�ṹ��ת����time_t��ʽ��
	return t_;                                 // ����ֵ�� 
}

time_t convert(int year,int month,int day)
{
	tm info={0};
	info.tm_year=year-1900;
	info.tm_mon=month-1;
	info.tm_mday=day;
	return mktime(&info);
}

int  get_days(const char* from, const char* to)
{
	int year,month,day;
	sscanf(from,"%d-%d-%d",&year,&month,&day);
	int fromSecond=(int)convert(year,month,day);
	sscanf(to,"%d-%d-%d",&year,&month,&day);
	int toSecond=(int)convert(year,month,day);
	return (toSecond-fromSecond)/24/3600;
}

std::string& trim(std::string &s)   
{  
	if (s.empty())   
	{  
		return s;  
	}  

	s.erase(0,s.find_first_not_of(" "));  
	s.erase(s.find_last_not_of(" ") + 1);  
	return s;  
}  

//�ַ����ָ��
std::vector<std::string> split(std::string str,std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str+=pattern;//��չ�ַ����Է������
	int size=str.size();

	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
	return result;
}

string&  replace_all(string&   str,const   string&   old_value,const   string&   new_value)     
{     
	while(true)   {     
		string::size_type   pos(0);     
		if(   (pos=str.find(old_value))!=string::npos   )     
			str.replace(pos,old_value.length(),new_value);     
		else   break;     
	}     
	return   str;     
}    

bool AllisNum(string str)  
{  
	for (int i = 0; i < str.size(); i++)
	{
		int tmp = (int)str[i];
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	} 
	return true;
}