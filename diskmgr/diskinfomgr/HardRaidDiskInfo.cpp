/*******************************************************************************
*   �ļ����� : HardRaidDiskInfo.cpp
*   ��    �� : ��ȫ��
*   ����ʱ�� : 2015/10/9   10:31
*   �ļ����� : 
*   ��Ȩ���� : Copyright (C) 2014-2015 ���ϰ���Ƽ����޹�˾
*   �޸���ʷ : 2015/10/9   1.00 ��ʼ�汾
*******************************************************************************/

#include "HardRaidDiskInfo.h"



/*******************************************************************************
* ��������  : CHardRaidDiskInfo
* ��������  : N/A
* ����ֵ    : 
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
CHardRaidDiskInfo::CHardRaidDiskInfo()
{
}


/*******************************************************************************
* ��������  : ~CHardRaidDiskInfo
* ��������  : N/A
* ����ֵ    : 
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
CHardRaidDiskInfo::~CHardRaidDiskInfo()
{
}

/*******************************************************************************
* ��������  : Init
* ��������  : N/A
* ����      : int argc
* ����      : char * argv[]
* ����ֵ    : void
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
void CHardRaidDiskInfo::Init(int argc, char *argv[])
{
	m_strRow = argv[1];
	m_strColumn = argv[2];
	m_strRaidtype = "hard";
	m_intRetCode = 0;
}

/*******************************************************************************
* ��������  : GetDiskInfo
* ��������  : N/A
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
*******************************************************************************/
int CHardRaidDiskInfo::GetDiskInfo()
{
	int ret = 0;
	char get_adapnum_cmd[128] = {};
	sprintf(get_adapnum_cmd, " MegaCli -pdlist -aAll | grep Adapter | wc -l"); //��ȡϵͳ��RAID��������
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
* ��������  : DiskInfoToJson
* ��������  : N/A
* ����      : string & strJson
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/09   ��ȫ��   �½�
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
* ��������  : SingleAdapter()
* ��������  : ��RAID��������»�ȡ������Ϣ
* ����      : N/A
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/23   ������   �½�
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
	/* ��ȡָ��RAID�������д��̵��������ۺš��������ͺ�WWNk�� */
	vector<string> alldiskinfo;
	char get_diskinfo_cmd[128] = {};
	sprintf(get_diskinfo_cmd, " MegaCli -pdlist -a%s | grep \"Raw Size\\|Slot Number\\|Media Type\\|WWN\"", adapdev.c_str());
	ret = syscmd_result(get_diskinfo_cmd, alldiskinfo);
	if (ret < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "Hard raid get disk infomation error:%s", get_diskinfo_cmd);
		return ret;
	}

	/* ��ȡָ��RAID��������RAID�顢RAID-NAME ��ÿ��RAID�����ļ��������� */
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
	/* ��ÿһ����̵���Ϣ������һ��diskinfo�У�����ÿ����̵�diskinfo ��ÿ����̵�slot �Ŷ�Ӧ�����ڳ�Ա����m_mapDiskInfo�� */
	for (int i = 0; i < alldiskinfo.size(); i++)
	{
		if (alldiskinfo[i].find("Slot Number") != string::npos)
		{
			tDiskInfo* diskinfo = new tDiskInfo; //���ڱ���ÿ����̵Ķ���

			string slot = alldiskinfo[i].substr(alldiskinfo[i].find(":")+2);//��ȡ������RAID���еĲۺ�
			/*int slotnum = (num * 8) + atoi(slot.c_str());*/
			diskinfo->m_strSlotNumber = slot;
			diskinfo->m_strSymbol = "N/A"; //ӲRAID������Ĵ�����ϵͳ��û��"sda" ���Ƶ��豸����

			string wwnstr = "wwn-"; 
			string wwnnum = alldiskinfo[i + 1].substr(5); //��ȡWWN��
			diskinfo->m_strWWN = wwnstr + wwnnum; //��wwn��ǰ���� ��ww-��
			
			/*string capa = alldiskinfo[i + 2].substr(10,alldiskinfo[i+2].find("B "));*/
			int seek = (alldiskinfo[i + 2].find(" [") - 10);
			
			string capa = alldiskinfo[i + 2].substr(10, seek); //��ȡ����
			diskinfo->m_strCapacity = capa; 
			
			/* �ж��Ƿ�ΪHDD */
			if (alldiskinfo[i+3].find("Hard Disk Device") != string::npos ) 
			{
				diskinfo->m_strDiskType = "HDD";
			}
			else
			{
				diskinfo->m_strDiskType = "SSD";
			}
			diskinfo->m_strInRaid = "no";     //Ĭ�ϴ���û����RAID����
			diskinfo->m_strRaidName = "N/A";  //��Ȼû����RAID���У��ǵ�ȻҲ��û��RAID-NAME��
				
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
	m_mapAdatInfo[adapdev] = mapdiskinfo; //��ÿһ��RAID������������д�����Ϣ��RAID�������ƶ�Ӧ��������m_mapAdatInfo��Ա������
}

/*******************************************************************************
* ��������  : MoreAdapter()
* ��������  : ��RAID��������»�ȡ������Ϣ
* ����      : int adpnum RAID��������
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/23   ������   �½�
*******************************************************************************/
int CHardRaidDiskInfo::MoreAdapter(int adapnum)
{
	cout << "Function of unrealized" << endl;
}
