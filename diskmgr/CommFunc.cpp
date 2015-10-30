/*******************************************************************************
*   文件名称 : CommFunc.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/10/8   16:07
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/8   1.00 初始版本
*******************************************************************************/

#include "CommFunc.h"

#ifdef _WINDOWS
#include <Windows.h>
#include <Shlwapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

/*******************************************************************************
* 函数名称  : IsDirectoryExist
* 函数描述  : N/A
* 参数      : string & strPath
* 返回值    : bool
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2013/12/30   王全宝   新建
*******************************************************************************/
bool CCommFunc::IsDirectoryExist( const string& strPath )
{
    if (strPath.empty())
    {
        return false;
    }
    
#ifdef _WINDOWS
    DWORD dw;
    dw = GetFileAttributesA(strPath.c_str());
    if (((dw!=INVALID_FILE_ATTRIBUTES)&&(dw&FILE_ATTRIBUTE_DIRECTORY)) == FALSE)
    {
        return false;
    }    
#else
    if(access(strPath.c_str(), F_OK) == -1) //不存在
    {
        return false;
    }
#endif

    return true;
}

bool CCommFunc::MyCreateDirectory( string& strPath )
{
#ifdef _WINDOWS
    if (CreateDirectory(strPath.c_str(), NULL) == FALSE)
    {
        return false;
    }    
#else
    if(mkdir(strPath.c_str(), 0777) == -1) //失败
    {
        return false;
    }
#endif

    return true;
}

/*******************************************************************************
* 函数名称  : GetProgrammeDir
* 函数描述  : 获取程序当前路径
* 输入参数  : itype :  0: 包含"\"; 1:不包含
* 输出参数  : N/A
* 返回值    : N/A
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2013/01/05    王全宝   新建
*******************************************************************************/
void CCommFunc::GetProgrammeDir(string& strPath, int itype /*= 0*/)
{
#ifdef _WINDOWS
    //取程序路径
    char szFilePath[MAX_PATH] = {0};
    (void)GetModuleFileName(NULL,szFilePath,MAX_PATH);
    strPath = szFilePath;
    if (itype == 0)
    {
        strPath = strPath.substr(0, strPath.rfind("\\")+1);
    }
    else
    {
        strPath = strPath.substr(0, strPath.rfind("\\"));
    }
#else   
    char buf[256];
    int count = readlink( "/proc/self/exe", buf, 256 ); 
    if ( count < 0 ) 
    { 
         buf[0] = '\0';
         return ; 
    } 
    strPath = buf;
    if (itype == 0)
    {
        strPath = strPath.substr(0, strPath.rfind("/")+1);
    }
    else
    {
        strPath = strPath.substr(0, strPath.rfind("/"));
    }

#endif
}

void CCommFunc::trim(string& strSrc)
{
    //left
    strSrc.erase(0, strSrc.find_first_not_of(' '));
    //right
    strSrc.erase(strSrc.find_last_not_of(' ')+1);
}
