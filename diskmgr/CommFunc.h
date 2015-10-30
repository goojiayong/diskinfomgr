/*******************************************************************************
*   文件名称 : CommFunc.h
*   作    者 : 王全宝
*   创建时间 : 2015/10/8   16:06
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/8   1.00 初始版本
*******************************************************************************/

#pragma once

#include <string>
using std::string;

class CCommFunc
{
public:
    static bool IsDirectoryExist(const string& strPath);
    static bool MyCreateDirectory(string& strPath);

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
    static void GetProgrammeDir(string& strPath, int itype =0);

    static void trim(string& strSrc);

};

