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
/*virtual void Init(int argc, char *argv[]);*/

/*******************************************************************************
* ��������  : GetDiskInfo
* ��������  : ��ȡ������Ϣ�����麯��������������ʵ��
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/28   ������   �½�
*******************************************************************************/
virtual int GetDiskInfo();

virtual int GetAdapterNumber();

virtual int GetSlotNumber(int adapternum, map<string, int>& slotmap, int numslots, int startslot);
/*******************************************************************************
* ��������  : DiskInfoToJson
* ��������  : ��������Ϣת��Ϊjson�ַ���
* ����      : string & strJson
* ����ֵ    : int
* ��ע      : N/A
* �޸�����     �޸���   �޸�����
* -----------------------------------------------
* 2015/10/28   ������   �½�
*******************************************************************************/
/*virtual int DiskInfoToJson(string& strJson);*/
};

