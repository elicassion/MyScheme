#pragma once
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;
FILE *input = stdin;
string s="";
int len=0;
char *next_token() {
    char *res = NULL;
	string ans;
    int cur=0;
    int ch;
    while (!cur)
    {
        if(len > 0 && (s[0] == '(' || s[0] == ')'))// for cases:'(' ')'
        {
            ans=s;
            cur=len;
            s="";
            len=0;
            break;
        }
        if ((ch = fgetc(input)) == EOF)// 读字符
            break;
        switch (ch)
        {
            case '(':
            case ')':
                if(len>0)
                    ans=s;cur=len;//一个block的右括号
                s="";//多个嵌套的连续右括号
                s=s+(char)ch;
                len=1;
                break;
            default:
                if (isspace(ch))
                {
                    if(len>0)
                         ans=s;cur=len; //一段内容（符号或者数字）
                    s="";
                    len=0;
                }
                else
				{
					s=s+(char)ch;
                    len++; //连续读入字符组成内容
                }
        }
    }
    if(cur>0)
    {
        res=new char[cur+2];
        for(int i=0;i<=cur-1;i++)
            res[i]=ans[i];
		res[cur]='\0';
    }
    return res; //传回所读内容
}
