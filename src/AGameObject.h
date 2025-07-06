#pragma once
#include "pch.h"
#include "Math.h"

class DeviceContext;

class AGameObject
{
public:
    using String = std::string;

    AGameObject(String name);
    virtual ~AGameObject() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw(DeviceContext* context, String shader) = 0;

    bool IsActive() const;
    void SetActive(bool value);

    int GetId() const;
    void SetId(const UINT id);

    String GetName() const;
    void SetName(String name);

    void SetPosition(float x, float y, float z);
    void SetPosition(Vector3 vector);
    void Move(float x, float y, float z);
    void Move(Vector3 vector);
    Vector3 GetLocalPosition();

    void SetRotation(float pitch, float yaw, float roll);
    void SetRotation(Vector3 vector);
    void Rotate(float pitch, float yaw, float roll);
    Vector3 GetLocalRotation();

    void SetScale(float x, float y, float z);
    void SetScale(Vector3 vector);
    void Scale(float scale);
    Vector3 GetLocalScale();

    Matrix GetLocalMatrix();

	Vector3 GetForwardVector() const;
	Vector3 GetRightVector() const;
	Vector3 GetUpVector() const;

protected:
    unsigned int m_id = 0;    
    String m_name{};
    bool m_active = true;
    bool m_dirty = false;

    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_localScale;
    Matrix m_localMatrix;
};

