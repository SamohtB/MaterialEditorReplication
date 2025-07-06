#pragma once
#include "pch.h"
#include "Math.h"

enum class KeyState
{
	Idle,
	Pressed,
	Held,
	Released
};

class InputListener;

class InputSystem
{
public:
	static InputSystem* GetInstance();
	static void Initialize();
	static void Destroy();

	void ProcessInput();
	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);

	bool IsKeyDown(int key);
	bool IsKeyUp(int key);

	void SetCursorPosition(const Vector2& point);
	void ShowCursor(bool show);

	void GetMouseScrollDelta(float delta);

private:
	InputSystem();
	~InputSystem() = default;
	InputSystem(InputSystem const&) {}
	InputSystem& operator=(InputSystem const&) {}

	static InputSystem* sharedInstance;

	std::unordered_set<InputListener*> m_listenersMap;
	std::unordered_map<int, KeyState> m_keyStates;

	unsigned char m_keysState[256] = {};
	unsigned char m_oldKeysState[256] = {};

	Vector2 m_oldMousePosition;
	float m_mouseWheelDelta = 0.0f;

	bool m_firstTime = true;

	Vector2 GetMousePosition();
};