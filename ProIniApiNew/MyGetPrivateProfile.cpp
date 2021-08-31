#include "MyGetPrivateProfile.h"
#include <iostream>
#include <atlstr.h>
#include <string>
#include <fstream>
#include <istream>
#include <map>

using namespace std;

typedef struct SecKeyInfo
{
	int secNum;
	int keyBegin;
	int keyEnd;
}SecKeyInfo;

map<int, CString> mapIniContent;
map<int, SecKeyInfo> secLocNum;
int iniLineCount = 0;
int secCount = 0;

//��ini�ļ��������ݱ�����map��
int FunOpenIniFile(const CString IniPath)
{
	mapIniContent.clear();
	int i = 0;
	string str;
	ifstream  inifile(IniPath);
	while (!inifile.eof())
	{
		getline(inifile, str);
		mapIniContent[i] = str.c_str();
		i++;
	}
	iniLineCount = i - 1;
	inifile.close();
	return 0;
}
//�������кϷ���Section�����е��к�,���ҳ����Ӧ��Key��Χ
int FunFindSectionAndKeyTag(void)
{
	secLocNum.clear();
	int i = 0;
	for (map<int, CString>::iterator it = mapIniContent.begin(); it != mapIniContent.end(); it++)
	{
		if ( (it->second.Find(";") != -1) && (it->second.Find("[") != -1) )
		{
			if (it->second.Find(";") < it->second.Find("["))
				continue;
		}
		if ((it->second.Find("[") != -1) && (it->second.Find("]") != -1) && (it->second.Find("[") < it->second.Find("]")))
		{
			secLocNum[i].secNum = it->first;
			i++;
		}
	}
	secCount = i - 1;
	i = 0;
	for (map<int, SecKeyInfo>::iterator it = secLocNum.begin(); it != secLocNum.end(); it++, i++)
	{
		
		if (it->second.secNum == iniLineCount)
		{
			it->second.keyBegin = -1;
		}
		else
		{
			it->second.keyBegin = it->second.secNum + 1;
		}
	}
	i = 0;
	for (map<int, SecKeyInfo>::iterator it = secLocNum.begin() ; it != secLocNum.end(); it++, i++)
	{
		if (it->first == secCount)
		{
			if (it->second.secNum == iniLineCount)
			{
				it->second.keyEnd = -1;
			}
			else if (it->second.keyBegin == iniLineCount)
			{
				it->second.keyEnd = iniLineCount;
			}
			else
			{
				it->second.keyEnd = iniLineCount;
			}
		}
		else
		{
			int tmp = (++it)->second.secNum - 1;
			--it;
			it->second.keyEnd = tmp;
		}
	}
	return 0;
}
//��map�в���Section�����������
int FunFindSectionName(const CString AllSectionName)
{
	CString tmp;
	map<int, CString>::iterator itIni = mapIniContent.begin();
	
	for (map<int, SecKeyInfo>::iterator itSec = secLocNum.begin(); itSec != secLocNum.end(); itSec++)
	{
		tmp = mapIniContent[itSec->second.secNum];
		if (tmp.Find(AllSectionName) != -1)
			return itSec->first;
	}
	return -1;
}
//��Section�ĺϷ�Key��Χ���ҵ�ָ��key�������������
int FunFindKeyName(const CString AllKeyName, const int SecTag)
{
	int i = -1;
	int tag;
	int viceLang;
	int keyLen = AllKeyName.GetLength();

	for (i = secLocNum[SecTag].keyBegin; i <= secLocNum[SecTag].keyEnd; i++)
	{
		tag = mapIniContent[i].Find(AllKeyName);
		viceLang = mapIniContent[i].Find(";");
		if (viceLang == -1)//û�зֺ�
		{
			if (tag != -1)//�ҵ�key��
			{
				return i;
			}
			else//û�ҵ�key
			{
				continue;
			}
		}
		else//�зֺ�
		{
			if (viceLang == 0)//��һ�����Ƿֺ�
			{
				continue;
			}
			else//�зֺŵ����ǵ�һ��
			{
				if (tag != -1)//�зֺ�Ҳ��key
				{
					if (viceLang < tag)//�зֺ�����key�����ֺ���keyǰ��
					{
						continue;
					}
					else//�зֺ�����key�����ֺ���key֮��
					{
						return i;
					}
				}
				else//�зֺŵ�û�ҵ�key
				{
					continue;
				}
			}
		}
	}
	return -1;
}
//��key��Χ��Ѱ���Ƿ���ںϷ�ֵ�����������ַ�����ֵ
int FunFindKeyValue(const int KeyTag, CString KeyName,CString *KeyValue)
{
	CString keySeg = mapIniContent[KeyTag];
	keySeg = keySeg.Right(keySeg.GetLength() - keySeg.Find(KeyName));

	if (keySeg.Find("=") == -1)//key��û�е��ںţ���ʽ����ȷ
	{
		return 0;
	}
	else//�����и����ں�
	{
		if (keySeg.Find(";") == -1)//�е��ںţ���û�зֺ�
		{
			if (keySeg.Find("=") >= keySeg.Find(KeyName) + KeyName.GetLength())//�е��ںţ���û�зֺţ��ҵ��ں���KeyName֮��
			{
				*KeyValue = keySeg.Right(keySeg.GetLength() - keySeg.Find("=") - 1);
			}
			else//�е��ںţ���û�зֺţ������ں���KeyName֮ǰ����ʽ����ȷ
			{
				return 0;
			}
		}
		else//�е��ںţ�ͬʱҲ�зֺ�
		{
			if (keySeg.Find(";") < keySeg.Find("="))//�е��ںţ�ͬʱҲ�зֺţ��ҷֺ��ڵȺ�ǰ��
			{
				return 0;
			}
			else//�е��ںţ�ͬʱҲ�зֺţ��ҷֺ��ڵȺź���
			{
				*KeyValue = keySeg.Mid(keySeg.Find("="), keySeg.Find(";") - keySeg.Find("="));
			}
		}
	}
	
	if (KeyValue->Find("=") != -1)
		KeyValue->Replace("=", "");
	KeyValue->Trim();

	return KeyValue->GetLength();
}
//��KeyValue�ַ���ת���ɺϷ����������ݷ���
bool FunKeyValueToInt(CString KeyValue, int *KeyValueInt)
{
	CString tmp;
	if (KeyValue.Find("-") == 0)//����
	{
		tmp += "-";
		for (int i = 1; i < KeyValue.GetLength(); i++)
		{
			if (KeyValue[i] >= '0' && KeyValue[i] <= '9')
				tmp += KeyValue[i];
			else
				break;
		}
	}
	else//����
	{
		for (int i = 0; i < KeyValue.GetLength(); i++)
		{
			if (KeyValue[i] >= '0' && KeyValue[i] <= '9')
				tmp += KeyValue[i];
			else
				break;
		}
	}

	if (tmp.GetLength() == 1)//ֻ�и�����
	{
		return false;
	}
	else
	{
		*KeyValueInt = atoi(tmp);
		return true;
	}
	
}

//��ȡini�ļ��е�string������
long MyGetPrivateProfileString(	const CString SectionName,
								const CString KeyName,
								const CString DefaultString,
								CString *ReStr,
								const CString IniPathName)
{
	CString AllSectionName = "[" + SectionName + "]";
	int SecTag = FunFindSectionName(AllSectionName);
	if (SecTag == -1)
	{
		*ReStr = DefaultString;
		return 0;
	}
	
	if (secLocNum[SecTag].keyBegin == -1 || secLocNum[SecTag].keyEnd == -1)
	{
		*ReStr = DefaultString;
		return 0;
	}
	
	int KeyTag = FunFindKeyName(KeyName, SecTag);
	CString KeyValue;
	int KeyValueLen = FunFindKeyValue(KeyTag, KeyName,&KeyValue);
	if (KeyValueLen == 0)
	{
		*ReStr = DefaultString;
		return 0;
	}
	*ReStr = KeyValue;
	return KeyValueLen;
}

//��ȡini�ļ��е�int������
long MyGetPrivateProfileInt(const CString SectionName,
							const CString KeyName,
							const long DefaultNum,
							const CString IniPathName)
{
	CString AllSectionName = "[" + SectionName + "]";
	int SecTag = FunFindSectionName(AllSectionName);
	if (SecTag == -1)
	{
		return DefaultNum;
	}

	if (secLocNum[SecTag].keyBegin == -1 || secLocNum[SecTag].keyEnd == -1)
	{
		return DefaultNum;
	}

	int KeyTag = FunFindKeyName(KeyName, SecTag);
	CString KeyValue;
	int KeyValueLen = FunFindKeyValue(KeyTag, KeyName, &KeyValue);
	if (KeyValueLen == 0)
	{
		return DefaultNum;
	}

	int KeyValueInt;
	bool AnaKey =  FunKeyValueToInt(KeyValue, &KeyValueInt);
	if (AnaKey == true)
	{
		return KeyValueInt;
	}
	else
	{
		return DefaultNum;
	}
}