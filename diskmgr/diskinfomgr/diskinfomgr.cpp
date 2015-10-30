/*******************************************************************************
*   文件名称 : diskinfomgr.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/9/12   15:00
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/9/12   1.00 初始版本
*******************************************************************************/

#include "DiskinfoApp.h"
#include "EasyLog.h"
#include "IniFile.h"
#include "CommFunc.h"

#define DISKMGR_VERSION      "v1.1.000"

CEasyLog g_ClientLog;  //全局日志对象
CIniFile*   g_pConfig = NULL; //全局配置文件对象

/*******************************************************************************
* 函数名称  : main
* 函数描述  : N/A
* 参数      : int argc
* 参数      : char * argv[]
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/11   王全宝   新建
*******************************************************************************/
int main(int argc, char *argv [])
{   
	int ret = 0;
	if (argc != 5 )
	{
		cout << argc ;
		cout << " arguments error" << endl;
		MYTRACE(LOG_LEVEL_ERROR, "arguments error");
		return -1;
	} 

    //取程序路径
    string strFilePath;
    CCommFunc::GetProgrammeDir(strFilePath);

    string strConfigFile = strFilePath + "Config.ini";
    string strLogFile = strFilePath + "LiveStore.log";

    g_pConfig = new CIniFile(strConfigFile); //只读的形式建配置文件对象
    if (NULL == g_pConfig)
    {
        return -1;
    }
    //从配置文件中获取日志级别
    int iLogLevel = g_pConfig->ReadInt(GLOABL_SECTION, KEY_LOGLEVEL, 1);

    //打开日志
    g_ClientLog.Open(strLogFile.c_str(), iLogLevel);

    //测试日志
    MYTRACE(LOG_LEVEL_INFO, "livestore version<%s>", DISKMGR_VERSION);
    
	ret = CDiskinfoApp::GetInstance()->Init(argc, argv);
	if (ret != 0)
	{
		cout << "program init failed" << endl;
		MYTRACE(LOG_LEVEL_INFO, "program init failed");
		return ret;
	}

    CDiskinfoApp::GetInstance()->Run();
    
    CDiskinfoApp::ReleaseInstance();
    return 0;
}