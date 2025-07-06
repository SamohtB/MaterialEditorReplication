#include "InputSystem.h"
#include "InputListener.h"
#include "Windows.h"

InputSystem* InputSystem::sharedInstance = nullptr;

InputSystem* InputSystem::GetInstance()
{
	return sharedInstance;
}

void InputSystem::Initialize()
{
	try
	{
		sharedInstance = new InputSystem();
	}
	catch (...)
	{
		throw std::exception("InputSystem not created successfully");
	}
}

void InputSystem::Destroy()
{
	delete sharedInstance;
}

InputSystem::InputSystem() {}

void InputSystem::ProcessInput()
{
	// === Mouse Movement ===
	Vector2 current_mouse_pos = GetMousePosition();

	if (m_firstTime)
	{
		m_oldMousePosition = current_mouse_pos;
		m_firstTime = false;
	}

	if (m_oldMousePosition != current_mouse_pos)
	{
		for (auto listener : m_listenersMap)
			listener->OnMouseMove(current_mouse_pos);

		m_oldMousePosition = current_mouse_pos;
	}

	// === Mouse Scroll ===
	if (m_mouseWheelDelta != 0.0f)
	{
		for (auto listener : m_listenersMap)
			listener->OnMouseWheel(m_mouseWheelDelta);

		m_mouseWheelDelta = 0.0f;
	}


	// === Keyboard State Handling ===
	BYTE currentKeys[256];

	if (!::GetKeyboardState(currentKeys)) return;

	for (int i = 0; i < 256; ++i)
	{
		bool isDown = (currentKeys[i] & 0x80) != 0;
		bool wasDown = (m_keysState[i] & 0x80) != 0;

		auto& state = m_keyStates[i];

		// Update key state enum
		if (isDown && !wasDown)			state = KeyState::Pressed;
		else if (isDown && wasDown)		state = KeyState::Held;
		else if (!isDown && wasDown)	state = KeyState::Released;
		else							state = KeyState::Idle;

		switch (state)
		{
		case KeyState::Pressed:
			for (auto listener : m_listenersMap)
			{
				if (i == VK_LBUTTON)	listener->OnLeftMousePressed(current_mouse_pos);
				if (i == VK_RBUTTON)	listener->OnRightMousePressed(current_mouse_pos);
				listener->OnKeyPressed(i);
			}
			break;

		case KeyState::Held:
			for (auto listener : m_listenersMap)
			{
				//if (i == VK_LBUTTON)	listener->OnLeftMousePressed(current_mouse_pos);
				//if (i == VK_RBUTTON)	listener->OnRightMousePressed(current_mouse_pos);
				listener->OnKeyHeld(i);
			}
			break;

		case KeyState::Released:
			for (auto listener : m_listenersMap)
			{
				if (i == VK_LBUTTON)	listener->OnLeftMouseReleased(current_mouse_pos);
				if (i == VK_RBUTTON)	listener->OnRightMouseReleased(current_mouse_pos);
				listener->OnKeyReleased(i);
			}
			break;
		}
	}

	::memcpy(m_keysState, currentKeys, sizeof(m_keysState));
}

void InputSystem::AddListener(InputListener* listener)
{
	m_listenersMap.insert(listener);
}

void InputSystem::RemoveListener(InputListener* listener)
{
	m_listenersMap.erase(listener);
}

bool InputSystem::IsKeyDown(int key)
{
	return (m_keysState[key] & 0x80) != 0;
}

bool InputSystem::IsKeyUp(int key)
{
	return (m_keysState[key] & 0x80) == 0;
}

void InputSystem::SetCursorPosition(const Vector2& point)
{
	::SetCursorPos(static_cast<int>(point.x), static_cast<int>(point.y));
}

void InputSystem::ShowCursor(bool show)
{
	::ShowCursor(show);
}

void InputSystem::GetMouseScrollDelta(float delta)
{
	this->m_mouseWheelDelta += delta;
}

Vector2 InputSystem::GetMousePosition()
{
	POINT point;
	if (::GetCursorPos(&point))
	{
		return DirectX::XMVectorSet(static_cast<float>(point.x), static_cast<float>(point.y), 0.0f, 0.0f);
	}
	else
	{
		return DirectX::XMVectorZero();
	}
}

