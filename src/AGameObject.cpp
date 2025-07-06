#include "AGameObject.h"

AGameObject::AGameObject(String name) : m_id(0), m_name(name), m_active(true), m_dirty(false)
{
	this->m_localPosition = Vector3(0, 0, 0);
	this->m_localRotation = Vector3(0, 0, 0);
	this->m_localScale = Vector3(1, 1, 1);
	this->m_localMatrix = GetLocalMatrix();
}

bool AGameObject::IsActive() const
{
	return m_active;
}

void AGameObject::SetActive(bool value)
{
	this->m_active = value;
}

int AGameObject::GetId() const
{
	return this->m_id;
}

void AGameObject::SetId(const UINT id)
{
	this->m_id = id;
}

AGameObject::String AGameObject::GetName() const
{
	return this->m_name;
}

void AGameObject::SetName(String name)
{
	this->m_name = name;
}

void AGameObject::SetPosition(float x, float y, float z)
{
	this->m_localPosition = Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::SetPosition(Vector3 vector)
{
	this->m_localPosition = vector;
	this->m_dirty = true;
}

void AGameObject::Move(float x, float y, float z)
{
	this->m_localPosition += Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::Move(Vector3 vector)
{
	this->m_localPosition += vector;
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalPosition()
{
	return this->m_localPosition;
}

void AGameObject::SetRotation(float pitch, float yaw, float roll)
{
	this->m_localRotation = Vector3(pitch, yaw, roll);
	this->m_dirty = true;
}

void AGameObject::SetRotation(Vector3 vector)
{
	this->m_localRotation = vector;
	this->m_dirty = true;
}

void AGameObject::Rotate(float pitch, float yaw, float roll)
{
	this->m_localRotation += Vector3(pitch, yaw, roll);
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalRotation()
{
	return this->m_localRotation;
}

void AGameObject::SetScale(float x, float y, float z)
{
	this->m_localScale = Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::SetScale(Vector3 vector)
{
	this->m_localScale = vector;
	this->m_dirty = true;
}

void AGameObject::Scale(float scale)
{
	this->m_localScale += Vector3(scale, scale, scale);
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalScale()
{
	return this->m_localScale;
}

Matrix AGameObject::GetLocalMatrix()
{
	if (this->m_dirty)
	{
		this->m_dirty = false;

		Matrix scaleMatrix = Matrix::CreateScale(m_localScale);
		Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_localRotation.y),
			DirectX::XMConvertToRadians(m_localRotation.x),
			DirectX::XMConvertToRadians(m_localRotation.z)
		);
		Matrix translationMatrix = Matrix::CreateTranslation(m_localPosition);

		m_localMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	}
	
	return m_localMatrix;
}

Vector3 AGameObject::GetForwardVector() const
{
	float pitch = DirectX::XMConvertToRadians(m_localRotation.x);
	float yaw = DirectX::XMConvertToRadians(m_localRotation.y);

	Vector3 vector = Vector3(cosf(pitch) * sinf(yaw), -sinf(pitch), cosf(pitch) * cosf(yaw));
	vector.Normalize();
	return vector;
}

Vector3 AGameObject::GetRightVector() const
{
	float yaw = DirectX::XMConvertToRadians(m_localRotation.y);

	Vector3 vector = Vector3(cosf(yaw), 0.0f, -sinf(yaw));
	vector.Normalize();
	return vector;
}

Vector3 AGameObject::GetUpVector() const
{
	Vector3 vector = this->GetForwardVector().Cross(GetRightVector());
	vector.Normalize();
	return vector;
}
