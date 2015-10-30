#include "JbodDiskInfo.h"


CJbodDiskInfo::CJbodDiskInfo()
{
}


CJbodDiskInfo::~CJbodDiskInfo()
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
* 2015/10/28   ������   �½�
*******************************************************************************/
// void CJbodDiskInfo::Init(int argc, char *argv[])
// {
// 	m_strRow = argv[1];
// 	m_strColumn = argv[2];
// 	m_strRaidtype = "soft";
// 	m_intRetCode = 0;
// }

/*******************************************************************************
* ��������  : GetDiskInfo
* ��������  : ��ȡ������Ϣ�����麯��������������ʵ��
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/28   ������   �½�
*******************************************************************************/
int CJbodDiskInfo::GetDiskInfo()
{
	int ret = 0;

	/* ��ȡÿһ����̵����͡�������wwn��sn(sn �Ǳ�Ҫ�ģ������޷�ȷ�����̵�slot��) */
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
		diskinfo->m_strSlotNumber = strslot; //������һ������Ĳۺ�
		m_mapDiskInfo[diskinfo->m_strSN] = diskinfo; //�Ѵ�����Ϣ��sn�Ŷ�Ӧ���ŵ� m_mapDiskInfo ��
	}
	
	int adapternum = GetAdapterNumber(); //��ȡRAID��������
	if (adapternum < 0)
	{
		MYTRACE(LOG_LEVEL_ERROR, "get adapter number error:adapternum=%d", adapternum);
		m_intRetCode = -1;
	}

	/* ����ÿһ��RAID����ȡÿһ�����������ۺ� */
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

		/* ͨ��sn���ҵ�ÿһ�������RAID���е�slot�ţ��ټ��������ۺ� */
		map<string, tDiskInfo*>::iterator diskit;
		for (diskit = m_mapDiskInfo.begin(); diskit != m_mapDiskInfo.end();diskit++)
		{
			map<string, int>::iterator slotit;
			slotit = slotmap.find(diskit->first);
			int diskslotnum = ((adapternum*numslots) + (slotit->second - startslot)); //���յ�����ۺ�
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
* ��������  : GetAdapterNumber
* ��������  : ��ȡRAID��������
* ����      : N/A
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/29   ������   �½�
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
* ��������  : GetSlotNumber
* ��������  : ��ȡ���̵Ĳۺ���Ϣ
* �������  : int adapternum
* �������  : map<string, int>& slotmap
*		      int numslots , int startslot
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/29   ������   �½�
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

