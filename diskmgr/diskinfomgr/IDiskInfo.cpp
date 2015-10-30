/*******************************************************************************
*   文件名称 : IDiskInfo.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/9/12   15:23
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/9/12   1.00 初始版本
*******************************************************************************/

#include "IDiskInfo.h"



/*******************************************************************************
* 函数名称  : IDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
IDiskInfo::IDiskInfo()
{
}


/*******************************************************************************
* 函数名称  : ~IDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
IDiskInfo::~IDiskInfo()
{
    for (map<string, tDiskInfo*>::iterator it = m_mapDiskInfo.begin(); it != m_mapDiskInfo.end(); it++)
    {
        delete it->second;
        //it = m_mapDiskInfo.erase(it);
    }
	m_mapDiskInfo.clear();
}

/*******************************************************************************
* * 函数名称  : syscmd_result
* * 函数描述  : 执行shell命令，并返回执行结果。
* * 输入参数  : char* cmd     : 要执行的shell命令
*               std::vector<std::string> &resvec ：用于保存执行shell命令的结果
* * 输出参数  : std::vector<std::string> &resvec 命令的结果
* * 返回值    : 0:success  other:failed
* * 备注      : N/A
* * 修改日期     修改人   修改内容
* * -----------------------------------------------
* * 2015/10/12    郭家勇   新建
* *******************************************************************************/
int IDiskInfo::syscmd_result(char* cmd, std::vector<std::string> &resvec)
{
	resvec.clear();
	FILE *pp = popen(cmd, "r"); //建立管道
	if (!pp) {
		return -1;
	}
	char tmp[1024]; //设置一个合适的长度，以存储每一行输出
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		if (tmp[strlen(tmp) - 1] == '\n') {
			tmp[strlen(tmp) - 1] = '\0'; //去除换行符
		}
		resvec.push_back(tmp);
	}
	pclose(pp); //关闭管道
	return 0;
}

int IDiskInfo::syscmd_result(char* cmd, string& resvec)
{
	FILE *pp = popen(cmd, "r"); //建立管道
	if (!pp) {
		return -1;
	}
	char tmp[1024]; //设置一个合适的长度，以存储每一行输出
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		if (tmp[strlen(tmp) - 1] == '\n') {
			tmp[strlen(tmp) - 1] = '\0'; //去除换行符
		}
		string s(tmp);
		resvec = s;
	}
	pclose(pp); //关闭管道
	return 0;
}
