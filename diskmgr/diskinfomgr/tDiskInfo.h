/*******************************************************************************
*   文件名称 : tDiskInfo.h
*   作    者 : 王全宝
*   创建时间 : 2015/10/9   10:45
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/9   1.00 初始版本
*******************************************************************************/

#pragma once

#include <string>
using std::string;

//磁盘信息结构定义
struct tDiskInfo
{
    string      m_strSN; //磁盘sn
    string      m_strWWN; //wwn
    string      m_strSymbol;  //磁盘id
    string      m_strDiskType; //磁盘类型，SSD、HDD
    string      m_strCapacity; //容量
    string      m_strTemperature; //温度
	string      m_strInRaid; //是否被用来创建了RAID
	string      m_strRaidName; //所在的RAID组名称
	string      m_strSlotNumber; //磁盘所在的物理位置
	string      m_strPciAddress;
};