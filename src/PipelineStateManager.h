#pragma once
#include "pch.h"

#include "GraphicsCommons.h"
#include "ShaderTypes.h"
#include "ShaderLoader.h"

class PipelineStateManager
{
public:
	using PipelineMap = std::unordered_map<String, ComPtr<ID3D12PipelineState>>;

	PipelineStateManager(ID3D12Device* device);
	~PipelineStateManager() = default;

	ID3D12PipelineState* GetPipelineState(String shaderName) const;
	ID3D12RootSignature* GetRootSignature() const;

	void RegisterPipeline(ID3D12Device* device, String shaderName, const ShaderDesc& vertexDesc, const ShaderDesc& pixelDesc);

private:
	void CreateRootSignature(ID3D12Device* device);
	ComPtr<ID3D12PipelineState> CreatePipelineState(ID3D12Device* device, ComPtr<IDxcBlob> vs, ComPtr<IDxcBlob> ps);

	PipelineMap m_pipelineStates;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	std::unique_ptr<ShaderLoader> m_shaderLoader;
};

