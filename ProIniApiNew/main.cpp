#include <iostream>
#include <map>

#include <atlstr.h>
#include "MyGetPrivateProfile.h"

using namespace std;

int main()
{
	/*CString str;
	GetPrivateProfileString("Section2", " key1 ", "ERROR", str.GetBuffer(), 128, "../MyIni2.ini");
	cout << str << endl;*/

	FunOpenIniFile("../MyIni2.ini");
	FunFindSectionAndKeyTag();
	CString str;
	int num1 = MyGetPrivateProfileString("Section7", "key2", "ERROR", &str, "../MyIni2.ini");
	cout << str << endl;

	FunOpenIniFile("../MyIni1.ini");
	FunFindSectionAndKeyTag();
	int num2 = MyGetPrivateProfileInt("Section7", "key2", 0, "../MyIni1.ini");
	cout << num2 << endl;
	return 0;
}