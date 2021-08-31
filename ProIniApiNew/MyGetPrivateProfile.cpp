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

//打开ini文件并将内容保存在map中
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
//查找所有合法的Section所在行的行号,并找出其对应的Key范围
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
//在map中查找Section并返回其序号
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
//在Section的合法Key范围内找到指定key，并返回其序号
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
		if (viceLang == -1)//没有分号
		{
			if (tag != -1)//找到key了
			{
				return i;
			}
			else//没找到key
			{
				continue;
			}
		}
		else//有分号
		{
			if (viceLang == 0)//第一个就是分号
			{
				continue;
			}
			else//有分号但不是第一个
			{
				if (tag != -1)//有分号也有key
				{
					if (viceLang < tag)//有分号且有key，但分号在key前面
					{
						continue;
					}
					else//有分号且有key，但分号在key之后
					{
						return i;
					}
				}
				else//有分号但没找到key
				{
					continue;
				}
			}
		}
	}
	return -1;
}
//在key范围中寻找是否存在合法值，并返回其字符串型值
int FunFindKeyValue(const int KeyTag, CString KeyName,CString *KeyValue)
{
	CString keySeg = mapIniContent[KeyTag];
	keySeg = keySeg.Right(keySeg.GetLength() - keySeg.Find(KeyName));

	if (keySeg.Find("=") == -1)//key中没有等于号，格式不正确
	{
		return 0;
	}
	else//起码有个等于号
	{
		if (keySeg.Find(";") == -1)//有等于号，且没有分号
		{
			if (keySeg.Find("=") >= keySeg.Find(KeyName) + KeyName.GetLength())//有等于号，且没有分号，且等于号在KeyName之后
			{
				*KeyValue = keySeg.Right(keySeg.GetLength() - keySeg.Find("=") - 1);
			}
			else//有等于号，且没有分号，但等于号在KeyName之前，格式不正确
			{
				return 0;
			}
		}
		else//有等于号，同时也有分号
		{
			if (keySeg.Find(";") < keySeg.Find("="))//有等于号，同时也有分号，且分号在等号前面
			{
				return 0;
			}
			else//有等于号，同时也有分号，且分号在等号后面
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
//将KeyValue字符串转换成合法的整形数据返回
bool FunKeyValueToInt(CString KeyValue, int *KeyValueInt)
{
	CString tmp;
	if (KeyValue.Find("-") == 0)//负数
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
	else//正数
	{
		for (int i = 0; i < KeyValue.GetLength(); i++)
		{
			if (KeyValue[i] >= '0' && KeyValue[i] <= '9')
				tmp += KeyValue[i];
			else
				break;
		}
	}

	if (tmp.GetLength() == 1)//只有个符号
	{
		return false;
	}
	else
	{
		*KeyValueInt = atoi(tmp);
		return true;
	}
	
}

//获取ini文件中的string型数据
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

//获取ini文件中的int型数据
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