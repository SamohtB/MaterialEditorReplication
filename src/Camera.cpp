#include "Camera.h"
#include "InputSystem.h"
#include "Debug.h"
#include <cmath>

Camera::Camera(String name, UINT width, UINT height) : AGameObject(name), m_viewportWidth(width), m_viewportHeight(height)
{
    this->SetActive(true);
    this->m_isPerspectiveView = true;
    this->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::Update(float deltaTime)
{
    this->m_deltaTime = deltaTime;
}

Matrix Camera::GetViewMatrix()
{
	return this->GetLocalMatrix().Invert();
}

Matrix Camera::GetProjectionMatrix()
{
	if (this->m_isPerspectiveView)
    {
        return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fov), m_aspectRatio, m_pNearZ, m_pFarZ);
    }

    return DirectX::XMMatrixOrthographicLH(static_cast<float>(this->m_viewportWidth), static_cast<float>(this->m_viewportHeight), m_oNearZ, m_oFarZ);
}

void Camera::SetViewportSize(UINT width, UINT height)
{
    this->m_viewportHeight = height;
    this->m_viewportWidth = width;
    this->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::SetLookAt(Vector3 lookat)
{
    Vector3 forward = lookat - this->m_localPosition;
    forward.Normalize();

	Matrix::CreateLookAt(this->m_localPosition, lookat, Vector3::Up);

    float yaw = atan2f(forward.x, forward.z);
    float pitch = asinf(-forward.y);

	this->SetRotation(pitch, yaw, 0.0f);
}