#pragma once

class Quad;

class Cursor : public Singleton<Cursor>
{
private:
	friend class Singleton;

	Quad* cursor;

	Cursor();
	~Cursor();

public:
	void Update();
	void PostRender();

	void HideCursor(bool bHide);
};