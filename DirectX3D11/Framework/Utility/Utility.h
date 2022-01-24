#pragma once

namespace Utility
{
	string ToString(wstring str);
	wstring ToWString(string str);
	
	string GetExtension(string file);

	bool ExistFile(string file);
}