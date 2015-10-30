/*******************************************************************************
*   文件名称 : EasyLog.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/10/8   16:08
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/8   1.00 初始版本
*******************************************************************************/


#include "EasyLog.h"
#include <string>
using std::string;

#ifdef _WINDOWS
#pragma warning (disable:4996)
#endif

// 日志文件大小，最大1000K,超过就存成xxx.bak文件
#define MAX_LOGFILE_LEN 1000 * 1024

/*******************************************************************************
* 函数名称  : CEasyLog
* 函数描述  : 构造函数
* 输入参数  : N/A
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2012/11/05    王全宝   新建
*******************************************************************************/
CEasyLog::CEasyLog()
#ifdef _WINDOWS
    :m_hMutex(NULL)    
#else //windows
    :m_pMutex(PTHREAD_MUTEX_INITIALIZER)
#endif    
    ,m_iLogLevel(LOG_LEVEL_ERROR)
{
    memset(m_szLogFilePath, 0, sizeof(m_szLogFilePath));
}

/*******************************************************************************
* 函数名称  : ~CEasyLog
* 函数描述  : 析构函数
* 输入参数  : N/A
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2012/11/05    王全宝   新建
*******************************************************************************/
CEasyLog::~CEasyLog(void)
{
    Close();
}

/*******************************************************************************
* 函数名称  : Open
* 函数描述  : 开启日志系统
* 输入参数  : pszLogFilePath：日志文件名
                iLogLevel: 日志级别,默认error级别
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2012/11/05    王全宝   新建
*******************************************************************************/
void CEasyLog::Open(const char* pszLogFilePath, int iLogLevel /*= LOG_LEVEL_ERROR*/)
{
    strncpy(m_szLogFilePath, pszLogFilePath, FILENAME_MAX - 1);
    m_szLogFilePath[FILENAME_MAX - 1] = '\0';
 
#ifdef _WINDOWS
    //直接用文件的绝对路径做MUTEX的名字，会创建失败，故fd
    string strMutexName = m_szLogFilePath;
    strMutexName = strMutexName.substr(strMutexName.rfind("\\") + 1) +" log_mutex";   
    m_hMutex = CreateMutex(NULL, FALSE, strMutexName.c_str()); //根据文件名创建互斥对象
#else //windows
    pthread_mutex_init(&m_pMutex,NULL);
#endif
    m_iLogLevel = iLogLevel;
}
 
/*******************************************************************************
* 函数名称  : Close
* 函数描述  : 关闭日志系统
* 输入参数  : N/A
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2012/11/05    王全宝   新建
*******************************************************************************/
void CEasyLog::Close()
{
#ifdef _WINDOWS
    if (m_hMutex != NULL)
    {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
#else 
    pthread_mutex_destroy(&m_pMutex);

#endif
}

/*******************************************************************************
* 函数名称  : WriteLog
* 函数描述  : 写日志
* 输入参数  : iLevel:  日志级别
             pszFmt: 输入日志信息
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2012/11/05    王全宝   新建
*******************************************************************************/
void CEasyLog::WriteLog(int iLevel, const char* pszFile, int iLine, const char* pszFmt, ...)
{
    if (m_iLogLevel >= iLevel)
    {
#ifdef _WINDOWS
       WaitForSingleObject(m_hMutex, INFINITE); 
#else 
        pthread_mutex_lock(&m_pMutex);        
#endif        

        CheckFileSize(); //超过大小备份
        FILE* fp = fopen(m_szLogFilePath, "a");
        if (fp != NULL)
        {
            char szBuff[1024] = {0};
            va_list argptr;
            va_start(argptr, pszFmt);
            vsnprintf(szBuff, 1023, pszFmt, argptr);
            va_end(argptr);
#ifdef _WINDOWS
            SYSTEMTIME st;
            GetLocalTime(&st);

            fprintf(fp, "%s(%d)[%u][%04d-%d-%d %02d:%02d:%02d:%03d]%s\n", 
                pszFile, iLine,
                GetCurrentThreadId(),
                st.wYear, st.wMonth, st.wDay,st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
                szBuff);

#else 
            time_t t =time(NULL);
            struct tm local = { 0 };
            localtime_r(&t, &local);
            //struct tm *local;
            //time_t t;
            //t=time(NULL);
            //local=localtime(&t);
            fprintf(fp, "%s(%d)[%x][%04d-%d-%d %02d:%02d:%02d]%s\n", 
                pszFile, iLine,
                (unsigned)pthread_self(),
                1900+local.tm_year, 1+local.tm_mon, local.tm_mday,local.tm_hour, local.tm_min, local.tm_sec,
                szBuff);

#endif
            fclose(fp);
        }
#ifdef _WINDOWS
       ReleaseMutex(m_hMutex); 
#else 
       pthread_mutex_unlock(&m_pMutex);
        
#endif
        
    }
}

/*******************************************************************************
* 函数名称  : CheckFileSize
* 函数描述  : 检查日志文件大小，超出大小则转储
* 输入参数  : N/A
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : 如果大小超出门限则存入xx.bak中,后续日志继续在清空后的原日志文件写
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2012/11/05    王全宝   新建
*******************************************************************************/
void CEasyLog::CheckFileSize()
{
    FILE* fp = fopen(m_szLogFilePath, "rb");
    if (fp != NULL)
    {
        // 先获取文件大小
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fclose(fp);

        // 文件超过门限则复制到备份文件
        if (size > MAX_LOGFILE_LEN)
        {
            // 备份文件
            char szBakFile[FILENAME_MAX] = {0};
            sprintf(szBakFile, "%s.bak", m_szLogFilePath);
            FILE* fpbak =fopen(szBakFile, "rb");
            if (fpbak != NULL) //检查文件已存在
            {
                fclose(fpbak);
                remove(szBakFile);  //删除文件
            }

            rename(m_szLogFilePath, szBakFile); //重命名文件
        }
    }
}

