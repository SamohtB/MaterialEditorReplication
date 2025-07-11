#include "RenderSystem.h"

#include "Debug.h"

#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "LightManager.h"

#include "PipelineStateManager.h"
#include "DescriptorHeapManager.h"

#include "ShaderTypes.h"

RenderSystem::RenderSystem(UINT width, UINT height, HWND hwnd) :
	m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{
	this->m_renderDevice = std::make_unique<RenderDevice>();
	auto d3dDevice = this->m_renderDevice->GetD3DDevice();

	this->m_deviceContext = std::make_unique<DeviceContext>(d3dDevice);
	this->m_swapChainManager = std::make_unique<SwapChainManager>(this->m_renderDevice->GetFactory(),
		this->m_deviceContext->GetCommandQueue(), width, height, hwnd);

	auto heapManager = this->m_renderDevice->GetDescriptorHeapManager();

	this->m_renderTargetManager = std::make_unique<RenderTargetManager>(d3dDevice, this->m_swapChainManager->GetSwapChain(),
		heapManager->GetRTVCPUHandleAt(0), heapManager->GetRTVDescriptorSize());

	this->m_frameConstantsBuffer = std::make_unique<FrameConstantsBuffer>(d3dDevice, FRAME_COUNT);
	this->m_depthBuffer = std::make_unique<DepthBuffer>(d3dDevice, heapManager->GetDSVCPUHandle(), width, height);

	/* Initial Signal */
	this->m_deviceContext->SignalCurrentFrameGPU(0);
}

void RenderSystem::BeginFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	
	this->m_deviceContext->WaitForFrameGPU(currentFrameIndex);
	this->m_deviceContext->ResetCommands(currentFrameIndex);

	this->m_deviceContext->SetRootSignature(this->m_renderDevice->GetPSOManager()->GetRootSignature());

	auto heaps = this->m_renderDevice->GetDescriptorHeapManager()->GetActiveHeaps();
	this->m_deviceContext->SetDescriptorHeaps(heaps);

	this->m_deviceContext->SetViewport(&m_viewport);
	this->m_deviceContext->SetScissorRect(&m_scissorRect);
	
	GraphicsEngine::GetInstance()->GetMaterialManager()->BeginFrame(currentFrameIndex);
	GraphicsEngine::GetInstance()->GetLightManager()->BeginFrame(currentFrameIndex);

	auto renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex);
	this->m_deviceContext->TransitionToRenderTarget(renderTarget);

	auto rtvHandle = this->m_renderDevice->GetDescriptorHeapManager()->GetRTVCPUHandleAt(currentFrameIndex);
	auto dsvHandle = this->m_renderDevice->GetDescriptorHeapManager()->GetDSVCPUHandle();
	this->m_deviceContext->ClearRenderTargetColor(rtvHandle, dsvHandle, this->m_clearColor);

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void RenderSystem::EndFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	auto renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), this->m_deviceContext->GetCommandList());

	this->m_deviceContext->TransitionToPresent(renderTarget);
	this->m_deviceContext->ExecuteCommandList();

	this->m_swapChainManager->PresentFrame();

	this->m_deviceContext->SignalCurrentFrameGPU(currentFrameIndex);
	this->m_swapChainManager->UpdateFrameIndex();
}

void RenderSystem::UpdateFrameConstants(const FrameConstantsData& data)
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_frameConstantsBuffer->Update(data, currentFrameIndex);
}

D3D12_GPU_VIRTUAL_ADDRESS RenderSystem::GetFrameConstantsAddress()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	return this->m_frameConstantsBuffer->GetGPUVirtualAddress(currentFrameIndex);
}

ID3D12PipelineState* RenderSystem::GetPipelineState(const String& shaderName) const
{
	return m_renderDevice->GetPSOManager()->GetPipelineState(shaderName);
}

UINT RenderSystem::GetCurrentFrameIndex()
{
	return this->m_swapChainManager->GetCurrentFrameIndex();;
}

ComPtr<ID3D12Device> RenderSystem::GetD3DDevicePtr()
{
	return this->m_renderDevice->GetD3DDevicePtr();
}

RenderDevice* RenderSystem::GetRenderDevice()
{
	return this->m_renderDevice.get();
}

DeviceContext* RenderSystem::GetDeviceContext()
{
	return this->m_deviceContext.get();
}

String RenderSystem::GetActiveShader() const
{
	return this->m_activeShader;
}

void RenderSystem::SetActiveShader(const String& shaderName)
{
	if (this->m_activeShader == shaderName)
	{
		Debug::Log(shaderName + "shader already active!");
		return;
	}

	if (!ShaderTypes::IsValid(shaderName)) 
	{
		Debug::Log("Invalid shader: " + shaderName);
		return;
	}

	this->m_activeShader = shaderName;
}

void RenderSystem::SetClearColor(const std::vector<float>& color)
{
	this->m_clearColor = color;
}
