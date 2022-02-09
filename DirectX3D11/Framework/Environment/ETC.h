#pragma once

class ETC : public Singleton<ETC>
{
private:
	friend class Singleton;

	ETC();
	~ETC();

	bool isShowCursor = true;
	bool isBindCursor = false;
	bool isSkipMouseDelta = false;

public:
	void Update();

	bool IsSkipMouseDelta() { return isSkipMouseDelta; }

private:
	void CursorSwitch();
	RECT BindCursor();
	void ReplaceCursor();
};