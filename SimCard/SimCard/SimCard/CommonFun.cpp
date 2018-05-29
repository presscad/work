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
	char *cha = (char*)str.data();             // 将string转换成char*。
	tm tm_;                                    // 定义tm结构体。
	int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
	sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
	tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
	tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
	tm_.tm_mday = day;                         // 日。
	tm_.tm_hour = hour;                        // 时。
	tm_.tm_min = minute;                       // 分。
	tm_.tm_sec = second;                       // 秒。
	tm_.tm_isdst = 0;                          // 非夏令时。
	time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
	return t_;                                 // 返回值。 
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

//字符串分割函数
std::vector<std::string> split(std::string str,std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str+=pattern;//扩展字符串以方便操作
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