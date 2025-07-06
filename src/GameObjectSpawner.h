#pragma once

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "Camera.h"

#include "GameObjectManager.h"
#include "NameRegistry.h"
#include "Debug.h"

enum ObjectType
{
	PRIMITVE_CUBE = 0,
	PRIMITVE_PLANE,
	PRIMITVE_SPHERE,
};

class GameObjectSpawner
{
public:

	static void CreatePrimitive(ObjectType type)
	{
        using GameObjectPtr = std::shared_ptr<AGameObject>;
        GameObjectPtr obj = nullptr;
        std::string name{};

        switch (type)
        {
        case PRIMITVE_CUBE:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Cube");
            obj = std::make_shared<Cube>(name);
            break;

        case PRIMITVE_PLANE:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Plane");
            obj = std::make_shared<Plane>(name);
            break;

        case PRIMITVE_SPHERE:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Sphere");
            obj = std::make_shared<Sphere>(name);
            break;

        default:
            return;
        }

        if (obj)
        {
            GameObjectManager::GetInstance()->AddGameObject(obj);
        }
        else
        {
            Debug::LogError("Primitive not Created! Error Occured GameObjectSpawner::CreatePrimitive ");
        }
    }

};

