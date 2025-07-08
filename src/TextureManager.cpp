#include "TextureManager.h"
#include "Debug.h"

TextureManager::TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader)
    : m_heapManager(heapManager), m_uploader(uploader)
{
    this->m_startHandle = heapManager->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
}    

void TextureManager::LoadInitialTextures()
{
    LoadTexture(TextureType::DEFAULT, L"Assets/Textures/default.png");
    //CreateDefaultWhiteTexture();

    LoadTexture(TextureType::ROCK_COLOR, L"Assets/Textures/rock_d.png");
    LoadTexture(TextureType::ROCK_NORMAL, L"Assets/Textures/rock_n.png");
    LoadTexture(TextureType::ROCK_ROUGH, L"Assets/Textures/rock_r.png");
    LoadTexture(TextureType::ROCK_AO, L"Assets/Textures/rock_ao.png");
    LoadTexture(TextureType::ROCK_HEIGHT, L"Assets/Textures/rock_h.png");

    LoadTexture(TextureType::METAL_COLOR, L"Assets/Textures/metal_plate_d.jpg");
    LoadTexture(TextureType::METAL_NORMAL, L"Assets/Textures/metal_plate_n.jpg");
    LoadTexture(TextureType::METAL_ROUGH, L"Assets/Textures/metal_plate_r.jpg");
    LoadTexture(TextureType::METAL_METAL, L"Assets/Textures/metal_plate_m.jpg");

    LoadTexture(TextureType::BRICKS_COLOR, L"Assets/Textures/bricks_d.jpg");
    LoadTexture(TextureType::BRICKS_NORMAL, L"Assets/Textures/bricks_n.jpg");
    LoadTexture(TextureType::BRICKS_ROUGH, L"Assets/Textures/bricks_r.jpg");
    LoadTexture(TextureType::BRICKS_AO, L"Assets/Textures/bricks_ao.jpg");

    LoadTexture(TextureType::LOGO, L"Assets/Textures/LOGO.png");
    
}

void TextureManager::LoadTexture(const String& textureName, const std::wstring& filePath)
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetSRVCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedTexture(filePath, srvHandle);

    TexturePtr texture = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[textureName] = srvIndex;
    this->m_textureMap[textureName] = std::move(texture);
    Debug::Log(textureName + " Loaded at " + std::to_string(srvIndex));
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVStart()
{
    return this->m_startHandle;
}

UINT TextureManager::GetTextureSRVIndex(const String& textureName)
{
    auto it = this->m_srvMap.find(textureName);

    if (it != this->m_srvMap.end())
        return it->second;

    Debug::LogError(textureName + " Not Found!");
    return 0;     
}

ImTextureID TextureManager::GetThumbnail(const String& textureName)
{
	auto it = this->m_srvMap.find(textureName);
    if (it != this->m_srvMap.end())
    {
        /* Get UINT Handle Index */
		auto& handleIndex = it->second;

		ImTextureID textureID = (ImTextureID)m_heapManager->GetSRVGPUHandleAt(handleIndex).ptr;
        return textureID;
	}

    Debug::LogError("Thumbnail for " + textureName + " not found or invalid handle.");
    return (ImTextureID)nullptr;
}

void TextureManager::CreateDefaultWhiteTexture()
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetSRVCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedWhitePixelTexture(srvHandle);

    TexturePtr whiteTex = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[TextureType::DEFAULT] = srvIndex;
    this->m_textureMap[TextureType::DEFAULT] = std::move(whiteTex);

    Debug::Log("Default white texture generated and uploaded.");
}
