#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "FrameConstants.h"
#include "RenderSystem.h"
#include "AGameObject.h"
#include "AMeshObject.h"
#include "Debug.h"

std::unique_ptr<GameObjectManager> GameObjectManager::sharedInstance = nullptr;

GameObjectManager* GameObjectManager::GetInstance()
{
    return sharedInstance.get();
}

void GameObjectManager::Initialize(ID3D12Device* device)
{
    try 
    {
        sharedInstance = std::make_unique<GameObjectManager>(device);
    }
    catch (...)
    {
        Debug::LogError("Game Object Manager creation failed!");
    }
}

void GameObjectManager::Destroy()
{
    sharedInstance.reset();
}

GameObjectManager::GameObjectManager(ID3D12Device* device)
{
    this->m_objectConstantsBuffer = std::make_unique<DynamicConstantBufferPool>(device, POOL_SIZE_MEDIUM);
}

AGameObject* GameObjectManager::FindObjectByName(String name)
{
    auto it = m_objectTable.find(name);

    if (it != m_objectTable.end())
    {
        return it->second.get();
    }

    return nullptr;
}

std::vector<AGameObject*> GameObjectManager::GetAllObjects()
{
    std::vector<AGameObject*> allObjects;

    for (const auto& obj : m_objectList)
    {
        allObjects.push_back(obj.get());
    }

    return allObjects;
}

int GameObjectManager::ActiveObjects()
{
    int activeCount = 0;

    for (const auto& obj : m_objectList)
    {
        if (obj->IsActive())
        {
            activeCount++;
        }
    }

    return activeCount;
}

void GameObjectManager::UpdateAll(float deltaTime)
{
    for (const auto& object : this->m_objectList)
    {
        if (object->IsActive())
        {
            object->Update(deltaTime);
        }
    }
}

void GameObjectManager::RenderAll(DeviceContext* context)
{
    for (const auto& object : m_renderedList)
    {
        if (object->IsActive())
        {
            object->Draw(context);
        }
    }
}

void GameObjectManager::AddGameObject(GameObjectPtr gameObject, bool isRendered)
{
    const std::string& name = gameObject->GetName();
    if (m_objectTable.contains(name))
    {
        Debug::LogWarning("GameObjectManager::AddGameObject: Object with name '" + name + "' already exists. Overwriting.");
    }

    this->m_objectTable[name] = gameObject;
    this->m_objectList.push_back(gameObject);

    if (isRendered)
    {
        auto mesh = std::dynamic_pointer_cast<AMeshObject>(gameObject);

        if (mesh)
        {
            this->m_renderedList.push_back(mesh);
        }
        else
        {
            Debug::LogWarning("AddGameObject: Object marked as rendered is not a mesh. Skipping render list insertion.");
        }
    }
}

void GameObjectManager::DeleteObject(AGameObject* gameObject)
{
    if (!gameObject) return;

    auto nameIt = m_objectTable.find(gameObject->GetName());
    if (nameIt != m_objectTable.end())
    {
        m_objectTable.erase(nameIt);
    }

    m_objectList.erase(
        std::remove_if(
            m_objectList.begin(), 
            m_objectList.end(), 
            [gameObject](const GameObjectPtr& ptr)
            {
				return ptr.get() == gameObject;
            }),
        m_objectList.end()
    );

    auto mesh = dynamic_cast<AMeshObject*>(gameObject);
    if (mesh)
    {
        m_renderedList.erase(
            std::remove_if(
                m_renderedList.begin(),
                m_renderedList.end(),
                [mesh](const std::shared_ptr<AMeshObject>& ptr)
                {
                    return ptr.get() == mesh;
                }),
            m_renderedList.end()
        );
    }
}

void GameObjectManager::DeleteObjectByName(String name)
{
    auto it = m_objectTable.find(name);
    if (it == m_objectTable.end()) return;

    GameObjectPtr gameObject = it->second;

    m_objectList.erase(
        std::remove(m_objectList.begin(), m_objectList.end(), gameObject),
        m_objectList.end()
    );

    auto mesh = std::dynamic_pointer_cast<AMeshObject>(gameObject);
    if (mesh)
    {
        m_renderedList.erase(
            std::remove(m_renderedList.begin(), m_renderedList.end(), mesh),
            m_renderedList.end()
        );
    }

    m_objectTable.erase(it);
}

void GameObjectManager::ClearAllObjects()
{
    m_objectList.clear();
	m_renderedList.clear();
    m_objectTable.clear();
}

AGameObject* GameObjectManager::GetSelectedObject() const
{
    return this->m_selectedObject;
}

void GameObjectManager::SetSelectedObject(AGameObject* object)
{
	if (object == this->m_selectedObject) return; // No change
	this->m_selectedObject = object;
}

void GameObjectManager::UploadObjectConstants(UINT frameIndex)
{
	m_objectConstantsBuffer->BeginFrame(frameIndex);

	for (const auto& object : m_renderedList)
	{
		if (!object->IsActive()) continue;
		ObjectConstantsData objData = {};
		objData.modelMatrix = object->GetLocalMatrix();
		objData.objectId = 0;

		auto cb = m_objectConstantsBuffer->Allocate(sizeof(ObjectConstantsData));
		memcpy(cb.cpuPtr, &objData, sizeof(ObjectConstantsData));
		object->SetGPUAddress(frameIndex, cb.gpuAddr);
	}
}
