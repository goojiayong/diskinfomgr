#include "JbodDiskInfo.h"


CJbodDiskInfo::CJbodDiskInfo()
{
}


CJbodDiskInfo::~CJbodDiskInfo()
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
* 2015/10/28   郭家勇   新建
*******************************************************************************/
// void CJbodDiskInfo::Init(int argc, char *argv[])
// {
// 	m_strRow = argv[1];
// 	m_strColumn = argv[2];
// 	m_strRaidtype = "soft";
// 	m_intRetCode = 0;
// }

/*******************************************************************************
* 函数名称  : GetDiskInfo
* 函数描述  : 获取磁盘信息，纯虚函数，在派生类中实现
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/28   郭家勇   新建
*******************************************************************************/
int CJbodDiskInfo::GetDiskInfo()
{
	int ret = 0;

	/* 获取每一块磁盘的类型、容量、wwn、sn(sn 是必要的，否则无法确定磁盘的slot号) */
	for (int i = 0; i < m_vecAllDisk.size(); i++)
	{
		string strdisk = m_vecAllDisk[i];
		tDiskInfo* diskinfo = new tDiskInfo;
		diskinfo->m_strSymbol = strdisk;
		ret = GetDiskSmart(diskinfo, strdisk);
		if (ret < 0)
		{
			MYTRACE(LOG_LEVEL_ERROR, "get disk infomation error:%s",strdisk.c_str());
			m_intRetCode = -1;
			continue;
		}

		ret = GetRaidName(diskinfo, strdisk);
		if (ret != 0)
		{
			MYTRACE(LOG_LEVEL_ERROR, "get raid name error:%s", strdisk.c_str());
			m_intRetCode = -1;
		}
		
		std::stringstream strtmp;
		string strslot;
		strtmp << i;
		strtmp >> strslot;
		diskinfo->m_strSlotNumber = strslot; //先设置一个虚拟的槽号
		m_mapDiskInfo[diskinfo->m_strSN] = diskinfo; //把磁盘信息和sn号对应并放到 m_mapDiskInfo 中
	}
	
	int adapternum = GetAdapterNumber(); //获取RAID卡的数量
	if (adapternum < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get adapter number error:adapternum=%d", adapternum);
		m_intRetCode = -1;
	}

	/* 根据每一块RAID卡获取每一块磁盘在物理槽号 */
	for (int i = 0; i < adapternum; i++)
	{
		int numslots = 0;
		int startslot = 0;
		map<string, int> slotmap;
		ret = GetSlotNumber(i, slotmap, numslots, startslot);
		if (ret < 0)
		{
			MYTRACE(LOG_LEVEL_ERROR, "get slot number error:adapternum=%d", adapternum);
			m_intRetCode = -1;
		}

		/* 通过sn号找到每一块磁盘在RAID卡中的slot号，再计算出物理槽号 */
		map<string, tDiskInfo*>::iterator diskit;
		for (diskit = m_mapDiskInfo.begin(); diskit != m_mapDiskInfo.end();diskit++)
		{
			map<string, int>::iterator slotit;
			slotit = slotmap.find(diskit->first);
			int diskslotnum = ((adapternum*numslots) + (slotit->second - startslot)); //最终的物理槽号
			std::stringstream strtmp;
			string strslot;
			strtmp << diskslotnum;
			strtmp >> strslot;
			diskit->second->m_strSlotNumber = strslot;
		}
		
	}
	return ret;
}


/*******************************************************************************
* 函数名称  : GetAdapterNumber
* 函数描述  : 获取RAID卡的数量
* 参数      : N/A
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/29   郭家勇   新建
*******************************************************************************/
int CJbodDiskInfo::GetAdapterNumber()
{
	int ret = 0;
	char get_adapternum_cmd[128] = {};
	sprintf(get_adapternum_cmd, "sas2ircu list 2>/dev/null| grep -A 10 '\\--- ' | sed '1d;$d' | wc -l");
	string strnum;
	ret = syscmd_result(get_adapternum_cmd, strnum);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get adapter number error:%s", get_adapternum_cmd);
		m_intRetCode = -1;
		return ret;
	}

	return atoi(strnum.c_str());
}

/*******************************************************************************
* 函数名称  : GetSlotNumber
* 函数描述  : 获取磁盘的槽号信息
* 输入参数  : int adapternum
* 输出参数  : map<string, int>& slotmap
*		      int numslots , int startslot
* 返回值    : int
* 备注      : N/A
* 修改日期     修改人   修改内容
* -----------------------------------------------
* 2015/10/29   郭家勇   新建
*******************************************************************************/
int CJbodDiskInfo::GetSlotNumber(int adapternum, map<string, int>& slotmap, int numslots, int startslot)
{
	int ret = 0;
	char get_slotnum_cmd[128] = {};
	vector<string> slotnum_vec;
	sprintf(get_slotnum_cmd, "sas2ircu %d display 2>/dev/null|grep -e 'Serial No' -e 'Slot #' -e 'Numslots' -e StartSlot", adapternum);
	ret = syscmd_result(get_slotnum_cmd, slotnum_vec);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get slot number error:%s", get_slotnum_cmd);
		m_intRetCode = -1;
		return ret;
	}

	vector<string>::iterator it = slotnum_vec.end();
	string ss = (*(it - 1)).substr((*(it - 1)).find(":") + 2);
	string ns = (*(it - 2)).substr((*(it - 2)).find(":") + 2);
	numslots = atoi(ns.c_str());
	startslot = atoi(ss.c_str());

// 	cout << "numslots = " << numslots << endl;
// 	cout << "startslot = " << startslot << endl;
	for (int i = 0; i < slotnum_vec.size(); i++)
	{
		if (slotnum_vec[i].find("Slot #") != string::npos)
		{
			int slotnum = atoi(slotnum_vec[i].substr(slotnum_vec[i].find(":") + 2).c_str());
			string sn = slotnum_vec[i + 1].substr(slotnum_vec[i + 1].find(":") + 2);
			slotmap[sn] = slotnum;
			i++;
		}
	}
	return ret;
}

