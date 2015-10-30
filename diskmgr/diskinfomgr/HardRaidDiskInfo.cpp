/*******************************************************************************
*   文件名称 : HardRaidDiskInfo.cpp
*   作    者 : 王全宝
*   创建时间 : 2015/10/9   10:31
*   文件描述 : 
*   版权声明 : Copyright (C) 2014-2015 湖南安存科技有限公司
*   修改历史 : 2015/10/9   1.00 初始版本
*******************************************************************************/

#include "HardRaidDiskInfo.h"



/*******************************************************************************
* 函数名称  : CHardRaidDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CHardRaidDiskInfo::CHardRaidDiskInfo()
{
}


/*******************************************************************************
* 函数名称  : ~CHardRaidDiskInfo
* 函数描述  : N/A
* 返回值    : 
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
CHardRaidDiskInfo::~CHardRaidDiskInfo()
{
}

/*******************************************************************************
* 函数名称  : Init
* 函数描述  : N/A
* 参数      : int argc
* 参数      : char * argv[]
* 返回值    : void
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
void CHardRaidDiskInfo::Init(int argc, char *argv[])
{
	m_strRow = argv[1];
	m_strColumn = argv[2];
	m_strRaidtype = "hard";
	m_intRetCode = 0;
}

/*******************************************************************************
* 函数名称  : GetDiskInfo
* 函数描述  : N/A
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
int CHardRaidDiskInfo::GetDiskInfo()
{
	int ret = 0;
	char get_adapnum_cmd[128] = {};
	sprintf(get_adapnum_cmd, " MegaCli -pdlist -aAll | grep Adapter | wc -l"); //获取系统中RAID卡的数量
	string num;
	ret = syscmd_result(get_adapnum_cmd, num);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get adapter number error");
		return ret;
	}
	
	int adapnum = atoi(num.c_str());
	MYTRACE(LOG_LEVEL_DEBUG, "get adapter number:%d ", adapnum);

	for (int i = 0; i < adapnum; i++)
	{
			SingleAdapter(i);
	}
		

// 	else
// 	{
// 		MYTRACE(LOG_LEVEL_ERROR, "get adapter number error, adapter number is: %s", num.c_str());
// 		return -1;
// 	}
}

/*******************************************************************************
* 函数名称  : DiskInfoToJson
* 函数描述  : N/A
* 参数      : string & strJson
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/09   王全宝   新建
*******************************************************************************/
int CHardRaidDiskInfo::DiskInfoToJson(string& strJson)
{
	int row = atoi(m_strRow.c_str());
	int column = atoi(m_strColumn.c_str());

	string x = "x";
	string ary = m_strRow + x + m_strColumn;
	Json::Value root;
	root["array"] = Json::Value(ary);
	root["raid_type"] = Json::Value(m_strRaidtype);
	root["retcode"] = Json::Value(m_intRetCode);

	map<string, map<string, tDiskInfo*>*>::iterator adapit;
	map<string, tDiskInfo*>::iterator diskit;

	for (adapit = m_mapAdatInfo.begin(); adapit != m_mapAdatInfo.end(); adapit++ )
	{
		Json::Value adapter;
		adapter["adapter_name"] = Json::Value(adapit->first);

		if (adapit->second->empty())
		{
			adapter["disks"] = Json::Value("N/A");
		}

		for (diskit = adapit->second->begin(); diskit != adapit->second->end(); diskit++)
		{
			// 	for (it = m_mapDiskInfo.begin(); it != m_mapDiskInfo.end(); it++)
			// 	{
					// 		stringstream s1;
					// 		string s2;
					// 		s1 << i;
					// 		s1 >> s2;
					// 		map<string, tDiskInfo*>::iterator it;
					// 		it = m_mapDiskInfo.find(s2);
					// 		if (it != m_mapDiskInfo.end())
					// 		{

			Json::Value slot;
			slot["slot_number"] = Json::Value(diskit->second->m_strSlotNumber);
			Json::Value info;
			info["in_raid"] = Json::Value(diskit->second->m_strInRaid);
			info["raid_name"] = Json::Value(diskit->second->m_strRaidName);
			info["WWN"] = Json::Value(diskit->second->m_strWWN);
			info["symbol"] = Json::Value(diskit->second->m_strSymbol);
			info["type"] = Json::Value(diskit->second->m_strDiskType);
			info["capacity"] = Json::Value(diskit->second->m_strCapacity);
			slot["disk"] = Json::Value(info);
			adapter["disks"].append(slot);
			// 		}
			// 		else
			// 		{
			// 			Json::Value slot;
			// 			slot["slot_number"] = Json::Value(s2);
			// 			slot["disk"] = Json::Value("N/A");
			// 			root["disks"].append(slot);
			// 		}
			// 	}
				// 		Json::Value jValue;
				// 		jValue["array"] = "test";
				// 		jValue["code"] = 100;
		}
		root["adapters"].append(adapter);
	}
	Json::FastWriter writer;
	strJson = writer.write(root).c_str();
	// 	Json::FastWriter fw;
	// 	/*cout << fw.write(root) << endl;*/



	return 0;
}

/*******************************************************************************
* 函数名称  : SingleAdapter()
* 函数描述  : 单RAID卡的情况下获取磁盘信息
* 参数      : N/A
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/23   郭家勇   新建
*******************************************************************************/
int CHardRaidDiskInfo::SingleAdapter(int num)
{
	stringstream s1;
	string s2;
	s1 << num;
	s1 >> s2;
	/*string adapdev = "a" + s2;*/
	string adapdev = s2;
	int ret = 0;
	/* 获取指定RAID卡上所有磁盘的容量、槽号、磁盘类型和WWNk号 */
	vector<string> alldiskinfo;
	char get_diskinfo_cmd[128] = {};
	sprintf(get_diskinfo_cmd, " MegaCli -pdlist -a%s | grep \"Raw Size\\|Slot Number\\|Media Type\\|WWN\"", adapdev.c_str());
	ret = syscmd_result(get_diskinfo_cmd, alldiskinfo);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "Hard raid get disk infomation error:%s", get_diskinfo_cmd);
		return ret;
	}

	/* 获取指定RAID卡上所有RAID组、RAID-NAME 和每个RAID组由哪几块磁盘组成 */
	vector<string> raidinfo;
	char get_raidinfo_cmd[128] = {};
	sprintf(get_raidinfo_cmd, "MegaCli -cfgdsply -a%s |grep  -e 'Slot Number' -e 'Name' |grep -v 'Product Name'", adapdev.c_str());
	ret = syscmd_result(get_raidinfo_cmd, raidinfo);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "Hard raid get disk infomation error:%s", get_raidinfo_cmd);
		return ret;
	}

	/*vector<map<string, tDiskInfo*>*> allinfo;*/
	map<string, tDiskInfo*>* mapdiskinfo = new  map<string, tDiskInfo*>;
	/* 把每一块磁盘的信息保存在一个diskinfo中，并把每块磁盘的diskinfo 和每块磁盘的slot 号对应保存在成员变量m_mapDiskInfo中 */
	for (int i = 0; i < alldiskinfo.size(); i++)
	{
		if (alldiskinfo[i].find("Slot Number") != string::npos)
		{
			tDiskInfo* diskinfo = new tDiskInfo; //用于保存每块磁盘的对象

			string slot = alldiskinfo[i].substr(alldiskinfo[i].find(":")+2);//获取磁盘在RAID卡中的槽号
			/*int slotnum = (num * 8) + atoi(slot.c_str());*/
			diskinfo->m_strSlotNumber = slot;
			diskinfo->m_strSymbol = "N/A"; //硬RAID卡管理的磁盘在系统中没有"sda" 类似的设备名称

			string wwnstr = "wwn-"; 
			string wwnnum = alldiskinfo[i + 1].substr(5); //获取WWN号
			diskinfo->m_strWWN = wwnstr + wwnnum; //在wwn号前加上 “ww-”
			
			/*string capa = alldiskinfo[i + 2].substr(10,alldiskinfo[i+2].find("B "));*/
			int seek = (alldiskinfo[i + 2].find(" [") - 10);
			
			string capa = alldiskinfo[i + 2].substr(10, seek); //获取容量
			diskinfo->m_strCapacity = capa; 
			
			/* 判断是否为HDD */
			if (alldiskinfo[i+3].find("Hard Disk Device") != string::npos ) 
			{
				diskinfo->m_strDiskType = "HDD";
			}
			else
			{
				diskinfo->m_strDiskType = "SSD";
			}
			diskinfo->m_strInRaid = "no";     //默认磁盘没有在RAID组中
			diskinfo->m_strRaidName = "N/A";  //既然没有在RAID组中，那当然也就没有RAID-NAME啦
				
			m_mapDiskInfo[slot] = diskinfo; 
			mapdiskinfo->insert(make_pair(slot, diskinfo));
		}
	}
	
	string raidname;
	for (int i = 0; i < raidinfo.size(); i++)
	{		
		if (raidinfo[i].find("Name") != string::npos)
		{
			 raidname = raidinfo[i].substr(raidinfo[i].find(":")+1);
			 continue;
		}
		string slot = raidinfo[i].substr(raidinfo[i].find(":") + 2);
		map<string, tDiskInfo*>::iterator  it;
		/*it = m_mapDiskInfo.find(slot);*/
		it = mapdiskinfo->find(slot);
		it->second->m_strInRaid = "yes";
		if (raidname != "")
		{
			it->second->m_strRaidName = raidname;
		}
	}
	m_mapAdatInfo[adapdev] = mapdiskinfo; //把每一块RAID卡所管理的所有磁盘信息和RAID卡的名称对应并保存在m_mapAdatInfo成员变量中
}

/*******************************************************************************
* 函数名称  : MoreAdapter()
* 函数描述  : 多RAID卡的情况下获取磁盘信息
* 参数      : int adpnum RAID卡的数量
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/23   郭家勇   新建
*******************************************************************************/
int CHardRaidDiskInfo::MoreAdapter(int adapnum)
{
	cout << "Function of unrealized" << endl;
}
