/*******************************************************************************
*   �ļ����� : diskinfomgr.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/9/12   15:00
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/9/12   1.00 ��ʼ�汾
*******************************************************************************/

#include "DiskinfoApp.h"
#include "EasyLog.h"
#include "IniFile.h"
#include "CommFunc.h"

#define DISKMGR_VERSION      "v1.1.000"

CEasyLog g_ClientLog;  //ȫ����־����
CIniFile*   g_pConfig = NULL; //ȫ�������ļ�����

/*******************************************************************************
* ��������  : main
* ��������  : N/A
* ����      : int argc
* ����      : char * argv[]
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/9/11   ��ȫ��   �½�
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

    //ȡ����·��
    string strFilePath;
    CCommFunc::GetProgrammeDir(strFilePath);

    string strConfigFile = strFilePath + "Config.ini";
    string strLogFile = strFilePath + "LiveStore.log";

    g_pConfig = new CIniFile(strConfigFile); //ֻ������ʽ�������ļ�����
    if (NULL == g_pConfig)
    {
        return -1;
    }
    //�������ļ��л�ȡ��־����
    int iLogLevel = g_pConfig->ReadInt(GLOABL_SECTION, KEY_LOGLEVEL, 1);

    //����־
    g_ClientLog.Open(strLogFile.c_str(), iLogLevel);

    //������־
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