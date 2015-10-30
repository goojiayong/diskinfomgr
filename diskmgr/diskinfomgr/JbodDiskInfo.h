#pragma once

#include "DirectDiskInfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
using std::map;
using std::vector;
class CJbodDiskInfo :
	public CDirectDiskInfo
{
public:
	CJbodDiskInfo();
	~CJbodDiskInfo();


/*******************************************************************************
* 函数名称  : Init
* 函数描述  : N/A
* 参数      : int argc
* 参数      : char * argv[]
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/28   郭家勇   新建
*******************************************************************************/
/*virtual void Init(int argc, char *argv[]);*/

/*******************************************************************************
* 函数名称  : GetDiskInfo
* 函数描述  : 获取磁盘信息，纯虚函数，在派生类中实现
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/28   郭家勇   新建
*******************************************************************************/
virtual int GetDiskInfo();

virtual int GetAdapterNumber();

virtual int GetSlotNumber(int adapternum, map<string, int>& slotmap, int numslots, int startslot);
/*******************************************************************************
* 函数名称  : DiskInfoToJson
* 函数描述  : 将磁盘信息转换为json字符串
* 参数      : string & strJson
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/28   郭家勇   新建
*******************************************************************************/
/*virtual int DiskInfoToJson(string& strJson);*/
};

