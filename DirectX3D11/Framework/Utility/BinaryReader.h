#pragma once

class BinaryReader
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryReader(wstring filePath);
	BinaryReader(string filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	string String();
	wstring WString();
	Float4 XMFLOAT4();

	class Vector3 Vector();

	void Byte(void** data, UINT dataSize);

	bool IsFailed();
};