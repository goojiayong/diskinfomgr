/*******************************************************************************
*   文件名称 : DiskinfoApp.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/9/12   15:59
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/9/12   1.00 初始版本
*******************************************************************************/

#include "DiskinfoApp.h"
#include "DirectDiskInfo.h"
#include "HardRaidDiskInfo.h"
#include "JbodDiskInfo.h"
#include "EasyLog.h"
#include "commdef.h"

CDiskinfoApp* CDiskinfoApp::m_pInstance = NULL;

/*******************************************************************************
* 函数名称  : Init
* 函数描述  : N/A
* 参数      : int argc
* 参数      : char * argv[]
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
int CDiskinfoApp::Init(int argc, char *argv[])
{
	int ret = -1;
    //todo:初始化设置，构造IDiskInfo对象
	if (strcmp(argv[3], "0") == 0)
	{
		m_pDiskInfo = new CHardRaidDiskInfo();
	} 
	else if (strcmp(argv[3], "1") == 0)
	{
		m_pDiskInfo = new CDirectDiskInfo();
	} 
	else if (strcmp(argv[3], "2") == 0)
	{
		m_pDiskInfo = new CJbodDiskInfo();
	}
	else
	{
		cout << " arguments error: argv[3] " << endl;
		MYTRACE(LOG_LEVEL_ERROR, " arguments error: argv[3] ");
		ret = -1;
	}

	if (m_pDiskInfo != NULL)
	{
		m_pDiskInfo->Init(argc, argv);
		ret = 0;
	}

	return ret;
}

/*******************************************************************************
* 函数名称  : Run
* 函数描述  : N/A
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
void CDiskinfoApp::Run()
{
    //todo:执行处理函数
    if (NULL != m_pDiskInfo)
    {
        m_pDiskInfo->GetDiskInfo();
        string strDst;
        m_pDiskInfo->DiskInfoToJson(strDst);

        printf("%s", strDst.c_str());
    }

}

/*******************************************************************************
* 函数名称  : GetInstance
* 函数描述  : N/A
* 返回值    : CDiskinfoApp*
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
CDiskinfoApp* CDiskinfoApp::GetInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDiskinfoApp();
    }

    return m_pInstance;
}

/*******************************************************************************
* 函数名称  : ReleaseInstance
* 函数描述  : N/A
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/9/12   王全宝   新建
*******************************************************************************/
void CDiskinfoApp::ReleaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
    }
}

/*******************************************************************************
* 函数名称  : CDiskinfoApp
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CDiskinfoApp::CDiskinfoApp()
    :m_pDiskInfo(NULL)
{
}


/*******************************************************************************
* 函数名称  : ~CDiskinfoApp
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CDiskinfoApp::~CDiskinfoApp()
{
    DELETEP(m_pDiskInfo);
}
