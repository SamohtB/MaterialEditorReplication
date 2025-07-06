#pragma once
#include "pch.h"
#include "FrameConstants.h"
#include "ConstantBuffer.h"

class DeviceContext;
class AGameObject;

class GameObjectManager
{
public:
    using GameObjectPtr = std::shared_ptr<AGameObject>;
    using String = std::string;
    using List =  std::vector<GameObjectPtr>;
    using Table = std::unordered_map<String, GameObjectPtr>;
    using CBMap = std::unordered_map<UINT, std::array<UINT, FRAME_COUNT>>;

    static GameObjectManager* GetInstance();
    static void Initialize(ID3D12Device* device);
    static void Destroy();

    AGameObject* FindObjectByName(String name);
    std::vector<AGameObject*> GetAllObjects();
    int ActiveObjects();
    void UpdateAll(float deltaTime);
    void RenderAll(DeviceContext* context, String shader);

    void AddGameObject(GameObjectPtr gameObject, bool hasConstantBuffer = true);
    void DeleteObject(AGameObject* game_object);
    void DeleteObjectByName(String name);
    void ClearAllObjects();

    AGameObject* GetSelectedObject() const;
    void SetSelectedObject(AGameObject* object);

    void UpdateConstantBuffer(UINT objId, const ObjectConstantsData& data);
    D3D12_GPU_VIRTUAL_ADDRESS GetObjectConstantsAddress(UINT objectId, UINT frameIndex);

private:
    UINT ReserveSlot();

    GameObjectManager(ID3D12Device* device);
    ~GameObjectManager() = default;
    GameObjectManager(GameObjectManager const&) {}
    GameObjectManager& operator=(GameObjectManager const&) {}

    static GameObjectManager* sharedInstance;

    CBMap m_cbMap;
    List m_renderedObjectList;
    List m_logicObjectList;
    Table m_objectTable;
    std::unique_ptr<ObjectConstantsBuffer> m_objectConstantsBuffer;

    UINT m_nextRenderedSlot = 0;
    UINT m_nextLogicSlot = 0;
    const int MAX_OBJECT_COUNT = 128;

	AGameObject* m_selectedObject = nullptr;

    friend class AGameObject;
};

