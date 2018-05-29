#pragma once
#include "stdafx.h"
int MyMessageBox(HWND hWnd,std::string strMsg,std::string strTip="ÌבÊ¾");
std::vector<std::string> split(std::string str,std::string pattern);
string&  replace_all(string&   str,const   string&   old_value,const   string&   new_value);
bool AllisNum(string str) ;
std::string& trim(std::string &s)  ;