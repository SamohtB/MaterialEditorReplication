#pragma once
#include "pch.h"
#include "Math.h"

class InputListener
{
public:
	InputListener()
	{

	}

	~InputListener()
	{

	}

	virtual void OnKeyPressed(int key) {}
	virtual void OnKeyReleased(int key) {}
	virtual void OnKeyHeld(int key) {}

	virtual void OnMouseMove(const Vector2& deltaMousePos) {}
	virtual void OnMouseWheel(const float& delta) {}

	virtual void OnLeftMousePressed(const Vector2& mousePos) {}
	virtual void OnLeftMouseReleased(const Vector2& mousePos) {}

	virtual void OnRightMousePressed(const Vector2& mousePos) {}
	virtual void OnRightMouseReleased(const Vector2& mousePos) {}

protected:

};