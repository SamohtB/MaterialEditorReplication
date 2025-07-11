#pragma once
#include <memory>
#include <windows.h>

class RenderSystem;
class BatchUploader;
class TextureManager;
class MaterialManager;
class LightManager;

/* central graphics subsystem entry point */
class GraphicsEngine
{
public:
	static GraphicsEngine* GetInstance();
	static void Initialize(UINT width, UINT height, HWND hwnd);
	static void Destroy();

	RenderSystem* GetRenderSystem();
	BatchUploader* GetBatchUploader();
	TextureManager* GetTextureManager();
	MaterialManager* GetMaterialManager();
	LightManager* GetLightManager();

	GraphicsEngine(UINT width, UINT height, HWND hwnd);
	~GraphicsEngine() = default;
	GraphicsEngine(GraphicsEngine const&) = delete;
	GraphicsEngine& operator=(GraphicsEngine const&) = delete;

private:
	static std::unique_ptr<GraphicsEngine> sharedInstance;

	std::unique_ptr<RenderSystem> m_renderSystem = nullptr;
	std::shared_ptr<BatchUploader> m_batchUploader = nullptr;
	std::unique_ptr<TextureManager> m_textureManager = nullptr;
	std::unique_ptr<MaterialManager> m_materialManager = nullptr;
	std::unique_ptr<LightManager> m_lightManager = nullptr;
};

