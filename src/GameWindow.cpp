#include "GameWindow.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "EngineGUIManager.h"
#include "NameRegistry.h"

#include "RenderSystem.h"
#include "BatchUploader.h"
#include "MaterialManager.h"
#include "TextureManager.h"

#include "Debug.h"
#include "Random.h"
#include "Colors.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
	EngineTime::Initialize(60);
	InputSystem::Initialize();
	CameraManager::Initialize(this->m_width, this->m_height);
	EngineGUIManager::Initialize(hwnd, this->m_width, this->m_height);
	NameRegistry::Initialize();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	GraphicsEngine::GetInstance()->GetTextureManager()->LoadInitialTextures();
	GraphicsEngine::GetInstance()->GetMaterialManager()->LoadInitialMaterials();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
} 


void GameWindow::OnUpdate()
{
	auto deltaTime = EngineTime::GetDeltaTime();
	m_ticks += deltaTime;

	InputSystem::GetInstance()->ProcessInput();

	CameraManager::GetInstance()->Update(deltaTime);
	GameObjectManager::GetInstance()->UpdateAll(deltaTime);

	FrameConstantsData frameData = {};
	frameData.viewMatrix = CameraManager::GetInstance()->GetActiveCameraViewMatrix();
	frameData.projMatrix = CameraManager::GetInstance()->GetActiveCameraProjMatrix();
	frameData.cameraPosition = CameraManager::GetInstance()->GetActiveCameraPosition();

	GraphicsEngine::GetInstance()->GetRenderSystem()->UpdateFrameConstants(frameData);
}

void GameWindow::OnRender()
{
	auto context = GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext();
	auto shader = GraphicsEngine::GetInstance()->GetRenderSystem()->GetActiveShader();

	GraphicsEngine::GetInstance()->GetRenderSystem()->BeginFrame();

	GameObjectManager::GetInstance()->RenderAll(context, shader);

	/*	Temporary Per Frame Batch Uploader Calls */
	/* Shared Resources Coming Soon */
	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	EngineGUIManager::GetInstance()->DrawAllUI();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();

	GraphicsEngine::GetInstance()->GetRenderSystem()->EndFrame();
}

void GameWindow::OnDestroy()
{
	NameRegistry::Destroy();
	EngineGUIManager::Destroy();
	CameraManager::Destroy();
	InputSystem::Destroy();
	GameObjectManager::Destroy();
	GraphicsEngine::Destroy();
}
