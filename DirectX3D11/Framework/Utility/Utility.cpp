#include "Framework.h"

string Utility::ToString(wstring str)
{
	string temp;
	temp.assign(str.begin(), str.end());

	return temp;
}

wstring Utility::ToWString(string str)
{
	wstring temp;
	temp.assign(str.begin(), str.end());

	return temp;
}

string Utility::GetExtension(string file)
{
	size_t index = file.find_last_of('.');

	return file.substr(index + 1, file.length());
}

bool Utility::ExistFile(string file)
{
	DWORD fileValue = GetFileAttributesA(file.c_str());
	
	return fileValue < 0xffffffff;
}
