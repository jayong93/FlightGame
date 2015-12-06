#pragma once
#include <map>

class InputManager
{
public:
	static InputManager* GetInstance();

	void PushKey(int key);
	void ReleaseKey(int key);
	bool GetKeyState(unsigned char key);
	bool GetSpecialKeyState(int key);
private:
	static InputManager* inst;

	std::map<int, bool> keyList;
};

typedef InputManager IM;