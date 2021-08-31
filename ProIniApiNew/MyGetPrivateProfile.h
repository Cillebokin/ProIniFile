#ifndef _MY_GET_PRIVATE_PROFILE_H_
#define _MY_GET_PRIVATE_PROFILE_H_

#include <map>
#include "atlstr.h"

//========================打开ini文件并将内容保存在map中==============================//
//const CString IniPath:	输入->指定的ini文件路径
//return int:				返回值->0表示成功
int FunOpenIniFile(const CString IniPath);
//====================================================================================//


//============查找所有合法的Section所在行的行号,并找出其对应的Key范围=================//
//void:						无参数
//return int:				返回值->0表示成功
int FunFindSectionAndKeyTag(void);
//====================================================================================//


//==============在Section的合法Key范围内找到指定key，并返回其序号=====================//
//const CString AllKeyName:	需要查找的指定Key名
//const int SecTag:			需要查找的Key所在的Section序号
//return int:				返回值->key所在的序号表示成功 / 返回值->-1表示查找失败
int FunFindKeyName(const CString AllKeyName, const int SecTag);
//====================================================================================//


//==============在key范围中寻找是否存在合法值，并返回其字符串型值 ====================//
//const int KeyTag:			需要判定的Key的所在序号
//const CString KeyName:	用于比较的原Key名
//CString *KeyValue:		需要返回的合法Key值
//return int:				返回值->Key值的合法长度表示查找成功 / 返回值->0表示查找失败
int FunFindKeyValue(const int KeyTag, const CString KeyName, CString *KeyValue);
//====================================================================================//


//==================将KeyValue字符串转换成合法的整形数据返回 =========================//
//CString KeyValue:			需要传入的原Key值字符串
//int KeyValueInt:			用于返回Key值转换后的整形数字
//return bool:				返回值->true表示成功转换为数字 / 返回值->false表示原字符串无法转换
bool FunKeyValueToInt(CString KeyValue, int KeyValueInt);
//====================================================================================//


//==========================获取ini文件中的int型数据 =================================//
//const CString SectionName:需要查找的Section名
//const CString KeyName:	需要查找的Key名
//const long DefaultNum:	查找失败时的默认返回值
//const CString IniPathName:需要查找的ini文件路径
//return long:				返回值->key值表示查找成功 / 返回值->DefaultNum表示查找失败
long MyGetPrivateProfileInt(const CString SectionName,
							const CString KeyName,
							const long DefaultNum,
							const CString IniPathName);
//====================================================================================//


//=========================获取ini文件中的string型数据 ===============================//
//const CString SectionName:	需要查找的Section名
//const CString KeyName:		需要查找的Key名
//const CString DefaultString:	查找失败时的默认返回值
//CString *ReStr:				用于返回查找到的Key值字符串
//const CString IniPathName:	需要查找的ini文件路径
//return long:					返回值->key值的长度表示查找成功 / 返回值->DefaultString表示查找失败
long MyGetPrivateProfileString(	const CString SectionName,
								const CString KeyName,
								const CString DefaultString,
								CString *ReStr,
								const CString IniPathName);
//====================================================================================//

#endif