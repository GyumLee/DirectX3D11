#pragma once

namespace Utility
{
	vector<string> SplitString(string origin, string tok);
	void Replace(string* str, string comp, string rep);

	string ToString(wstring str);
	wstring ToWString(string str);
	
	string GetExtension(string file);
	string GetFileName(string path);
	string GetFileNameWithoutExtension(string path);

	bool ExistDirectory(string path);
	bool ExistFile(string file);

	void CreateFolders(string path);
}