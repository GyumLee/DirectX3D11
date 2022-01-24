#pragma once

struct Vector3
{
	XMVECTOR data;

	Vector3()
	{
		data = XMVectorSet(0, 0, 0, 0);
	}
	Vector3(Float3 value)
	{
		data = XMLoadFloat3(&value);
	}
	Vector3(float x, float y, float z)
	{
		data = XMVectorSet(x, y, z, 0);
	}
	Vector3(XMVECTOR value) : data(value)
	{
	}

	void SetX(float value) { data = XMVectorSetX(data, value); }
	void SetY(float value) { data = XMVectorSetY(data, value); }
	void SetZ(float value) { data = XMVectorSetZ(data, value); }

	float GetX() { return XMVectorGetX(data); }
	float GetY() { return XMVectorGetY(data); }
	float GetZ() { return XMVectorGetZ(data); }

	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	operator Float3()
	{
		Float3 result;
		XMStoreFloat3(&result, data);

		return result;
	}
	operator XMVECTOR() { return data; }

	Vector3 operator+(const Vector3& value) { return data + value.data; }
	Vector3 operator-(const Vector3& value) { return data - value.data; }
	Vector3 operator*(const Vector3& value) { return data * value.data; }
	Vector3 operator/(const Vector3& value) { return data / value.data; }

	void operator+=(const Vector3& value) { data += value.data; }
	void operator-=(const Vector3& value) { data -= value.data; }
	void operator*=(const Vector3& value) { data *= value.data; }
	void operator/=(const Vector3& value) { data /= value.data; }

	Vector3 operator+(const float& value) { return data + XMVectorReplicate(value); }
	Vector3 operator-(const float& value) { return data - XMVectorReplicate(value); }
	Vector3 operator*(const float& value) { return data * XMVectorReplicate(value); }
	Vector3 operator/(const float& value) { return data / XMVectorReplicate(value); }

	void operator+=(const float& value) { data += XMVectorReplicate(value); }
	void operator-=(const float& value) { data -= XMVectorReplicate(value); }
	void operator*=(const float& value) { data *= XMVectorReplicate(value); }
	void operator/=(const float& value) { data /= XMVectorReplicate(value); }

	friend Vector3 operator+(const float& value1, const Vector3& value2)
	{
		return XMVectorReplicate(value1) + value2.data;
	}
	friend Vector3 operator-(const float& value1, const Vector3& value2)
	{
		return XMVectorReplicate(value1) - value2.data;
	}
	friend Vector3 operator*(const float& value1, const Vector3& value2)
	{
		return XMVectorReplicate(value1) * value2.data;
	}
	friend Vector3 operator/(const float& value1, const Vector3& value2)
	{
		return XMVectorReplicate(value1) / value2.data;
	}

	friend Vector3 operator+(const Float3&value1, const Vector3& value2)
	{
		return XMLoadFloat3(&value1) + value2.data;
	}
	friend Vector3 operator-(const Float3& value1, const Vector3& value2)
	{
		return XMLoadFloat3(&value1) - value2.data;
	}
	friend Vector3 operator*(const Float3& value1, const Vector3& value2)
	{
		return XMLoadFloat3(&value1) * value2.data;
	}
	friend Vector3 operator/(const Float3& value1, const Vector3& value2)
	{
		return XMLoadFloat3(&value1) / value2.data;
	}

	float Length() const { return XMVectorGetX(XMVector3Length(data)); }
	Vector3 Normalize() const { return XMVector3Normalize(data); }

	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2) { return XMVector3Cross(vec1.data, vec2.data); }
	static float Dot(const Vector3& vec1, const Vector3& vec2) { return XMVectorGetX(XMVector3Dot(vec1.data, vec2.data)); }
};