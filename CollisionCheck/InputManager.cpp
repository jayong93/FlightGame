#include "InputManager.h"

InputManager* InputManager::GetInstance()
{
	if (inst) return inst;
	else { inst = new InputManager(); return inst; }
}

void InputManager::PushKey(int key)
{
	keyList[key] = true;
}

void InputManager::ReleaseKey(int key)
{
	keyList[key] = false;
}

bool InputManager::GetKeyState(unsigned char key)
{
	return keyList[key];
}

bool InputManager::GetSpecialKeyState(int key)
{
	return keyList[key + 300];
}

InputManager* InputManager::inst = nullptr;