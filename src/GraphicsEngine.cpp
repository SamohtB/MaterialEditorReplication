#include "GraphicsEngine.h"

#include "RenderSystem.h"
#include "RenderDevice.h"
#include "TextureManager.h"
#include "MaterialManager.h"

#include "Debug.h"

std::unique_ptr<GraphicsEngine> GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine::GraphicsEngine(UINT width, UINT height, HWND hwnd)
{
	try
	{
		this->m_renderSystem = std::make_unique<RenderSystem>(width, height, hwnd);
	}
	catch (...)
	{
		Debug::LogError("Render System initialization failed!");
		return;
	}

	auto device = this->m_renderSystem->GetD3DDevicePtr();
	auto heapManager = this->m_renderSystem->GetRenderDevice()->GetDescriptorHeapManagerPtr();

	try
	{
		this->m_batchUploader = std::make_shared<BatchUploader>(device);
	}
	catch (...)
	{
		Debug::LogError("Batch Uploader initialization failed!");
		return;
	}

	try
	{
		this->m_textureManager = std::make_unique<TextureManager>(heapManager, this->m_batchUploader);
	}
	catch (...)
	{
		Debug::LogError("Texture Manager initialization failed!");
		return;
	}

	try
	{
		this->m_materialManager = std::make_unique<MaterialManager>(device.Get());
	}
	catch (...)
	{
		Debug::LogError("Material Manager initialization failed!");
		return;
	}
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	return sharedInstance.get();
}

void GraphicsEngine::Initialize(UINT width, UINT height, HWND hwnd)
{
	try
	{
		sharedInstance.reset();
		sharedInstance = std::make_unique<GraphicsEngine>(width, height, hwnd);
	}
	catch (...)
	{
		Debug::LogError("Graphics Engine initialization failed!");
	}
}

void GraphicsEngine::Destroy()
{
	sharedInstance.reset();
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return this->m_renderSystem.get();
}

BatchUploader* GraphicsEngine::GetBatchUploader()
{
	return this->m_batchUploader.get();
}

TextureManager* GraphicsEngine::GetTextureManager()
{
	return this->m_textureManager.get();
}

MaterialManager* GraphicsEngine::GetMaterialManager()
{
	return this->m_materialManager.get();
}

