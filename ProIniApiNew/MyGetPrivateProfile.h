#ifndef _MY_GET_PRIVATE_PROFILE_H_
#define _MY_GET_PRIVATE_PROFILE_H_

#include <map>
#include "atlstr.h"

//========================��ini�ļ��������ݱ�����map��==============================//
//const CString IniPath:	����->ָ����ini�ļ�·��
//return int:				����ֵ->0��ʾ�ɹ�
int FunOpenIniFile(const CString IniPath);
//====================================================================================//


//============�������кϷ���Section�����е��к�,���ҳ����Ӧ��Key��Χ=================//
//void:						�޲���
//return int:				����ֵ->0��ʾ�ɹ�
int FunFindSectionAndKeyTag(void);
//====================================================================================//


//==============��Section�ĺϷ�Key��Χ���ҵ�ָ��key�������������=====================//
//const CString AllKeyName:	��Ҫ���ҵ�ָ��Key��
//const int SecTag:			��Ҫ���ҵ�Key���ڵ�Section���
//return int:				����ֵ->key���ڵ���ű�ʾ�ɹ� / ����ֵ->-1��ʾ����ʧ��
int FunFindKeyName(const CString AllKeyName, const int SecTag);
//====================================================================================//


//==============��key��Χ��Ѱ���Ƿ���ںϷ�ֵ�����������ַ�����ֵ ====================//
//const int KeyTag:			��Ҫ�ж���Key���������
//const CString KeyName:	���ڱȽϵ�ԭKey��
//CString *KeyValue:		��Ҫ���صĺϷ�Keyֵ
//return int:				����ֵ->Keyֵ�ĺϷ����ȱ�ʾ���ҳɹ� / ����ֵ->0��ʾ����ʧ��
int FunFindKeyValue(const int KeyTag, const CString KeyName, CString *KeyValue);
//====================================================================================//


//==================��KeyValue�ַ���ת���ɺϷ����������ݷ��� =========================//
//CString KeyValue:			��Ҫ�����ԭKeyֵ�ַ���
//int KeyValueInt:			���ڷ���Keyֵת�������������
//return bool:				����ֵ->true��ʾ�ɹ�ת��Ϊ���� / ����ֵ->false��ʾԭ�ַ����޷�ת��
bool FunKeyValueToInt(CString KeyValue, int KeyValueInt);
//====================================================================================//


//==========================��ȡini�ļ��е�int������ =================================//
//const CString SectionName:��Ҫ���ҵ�Section��
//const CString KeyName:	��Ҫ���ҵ�Key��
//const long DefaultNum:	����ʧ��ʱ��Ĭ�Ϸ���ֵ
//const CString IniPathName:��Ҫ���ҵ�ini�ļ�·��
//return long:				����ֵ->keyֵ��ʾ���ҳɹ� / ����ֵ->DefaultNum��ʾ����ʧ��
long MyGetPrivateProfileInt(const CString SectionName,
							const CString KeyName,
							const long DefaultNum,
							const CString IniPathName);
//====================================================================================//


//=========================��ȡini�ļ��е�string������ ===============================//
//const CString SectionName:	��Ҫ���ҵ�Section��
//const CString KeyName:		��Ҫ���ҵ�Key��
//const CString DefaultString:	����ʧ��ʱ��Ĭ�Ϸ���ֵ
//CString *ReStr:				���ڷ��ز��ҵ���Keyֵ�ַ���
//const CString IniPathName:	��Ҫ���ҵ�ini�ļ�·��
//return long:					����ֵ->keyֵ�ĳ��ȱ�ʾ���ҳɹ� / ����ֵ->DefaultString��ʾ����ʧ��
long MyGetPrivateProfileString(	const CString SectionName,
								const CString KeyName,
								const CString DefaultString,
								CString *ReStr,
								const CString IniPathName);
//====================================================================================//

#endif