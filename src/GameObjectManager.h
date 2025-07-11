#pragma once
#include "pch.h"
#include "FrameConstants.h"
#include "DynamicConstantBufferPool.h"

class DeviceContext;
class AGameObject;
class AMeshObject;

class GameObjectManager
{
public:
    using GameObjectPtr = std::shared_ptr<AGameObject>;
    using MeshPtr = std::shared_ptr<AMeshObject>;
    using ObjectList =  std::vector<GameObjectPtr>;
    using RenderedList = std::vector<MeshPtr>;
    using Table = std::unordered_map<String, GameObjectPtr>;

    static GameObjectManager* GetInstance();
    static void Initialize(ID3D12Device* device);
    static void Destroy();

    AGameObject* FindObjectByName(String name);
    std::vector<AGameObject*> GetAllObjects();
    int ActiveObjects();
    void UpdateAll(float deltaTime);
    void RenderAll(DeviceContext* context, String shader);

    void AddGameObject(GameObjectPtr gameObject, bool isRendered = true);
    void DeleteObject(AGameObject* game_object);
    void DeleteObjectByName(String name);
    void ClearAllObjects();

    AGameObject* GetSelectedObject() const;
    void SetSelectedObject(AGameObject* object);

    void UploadObjectConstants(UINT frameIndex);

    GameObjectManager(ID3D12Device* device);
    ~GameObjectManager() = default;
    GameObjectManager(GameObjectManager const&) = delete;
    GameObjectManager& operator=(GameObjectManager const&) = delete;

private:
    static std::unique_ptr<GameObjectManager> sharedInstance;

    std::unique_ptr<DynamicConstantBufferPool> m_objectConstantsBuffer;

    ObjectList m_objectList;
    RenderedList m_renderedList;
    Table m_objectTable;

	AGameObject* m_selectedObject = nullptr;

    friend class AGameObject;
};

