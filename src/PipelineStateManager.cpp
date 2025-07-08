#include "PipelineStateManager.h"
#include "Debug.h"

PipelineStateManager::PipelineStateManager(ID3D12Device* device)
{
    this->m_shaderLoader = std::make_unique<ShaderLoader>();
	CreateRootSignature(device);

    // === Default ===
    ShaderDesc vertexshaderDesc(L"Assets/Shaders/UnlitVertexShader.hlsl", L"VSMain", L"vs_6_5");
    ShaderDesc pixelShaderDesc(L"Assets/Shaders/UnlitPixelShader.hlsl", L"PSMain", L"ps_6_5");
    RegisterPipeline(device, ShaderTypes::UNLIT, vertexshaderDesc, pixelShaderDesc);


    // === PBS pipeline (Shader Model 6.6) ===
    ShaderDesc vertexShaderDesc_PBS(L"Assets/Shaders/LitVertexShader.hlsl", L"VSMain", L"vs_6_5");
    ShaderDesc pixelShaderDesc_PBS(L"Assets/Shaders/LitPixelShader.hlsl", L"PSMain", L"ps_6_5");
    RegisterPipeline(device, ShaderTypes::LIT, vertexShaderDesc_PBS, pixelShaderDesc_PBS);
}

ID3D12PipelineState* PipelineStateManager::GetPipelineState(String shaderName) const
{
	auto it = m_pipelineStates.find(shaderName);
    if (it != m_pipelineStates.end())
    {
        return it->second.Get();
    }
    else
    {
        Debug::LogError("Invalid Shader Name: " + shaderName);
        return nullptr;
    }
}

ID3D12RootSignature* PipelineStateManager::GetRootSignature() const
{
	return m_rootSignature.Get();
}

void PipelineStateManager::RegisterPipeline(ID3D12Device* device, String shaderName, const ShaderDesc& vertexDesc, const ShaderDesc& pixelDesc)
{ 
    auto vs = m_shaderLoader->CompileShader(vertexDesc);
    auto ps = m_shaderLoader->CompileShader(pixelDesc);
	auto pso = CreatePipelineState(device, vs, ps);

    if (pso == nullptr)
    {
        Debug::LogWarning(shaderName + " shader creation failed!");
        return;
    }

	m_pipelineStates[shaderName] = pso;
    Debug::Log(shaderName +  " shader creation success!");
}

void PipelineStateManager::CreateRootSignature(ID3D12Device* device)
{
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

    CD3DX12_DESCRIPTOR_RANGE1 rootRanges[1];
    rootRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_SRV_COUNT, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

    CD3DX12_ROOT_PARAMETER1 rootParams[4];
    rootParams[RootDescriptorIndex::OBJECT_CONSTANTS].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_ALL);
    rootParams[RootDescriptorIndex::FRAME_CONSTANTS].InitAsConstantBufferView(1, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_ALL);
    rootParams[RootDescriptorIndex::TEXTURES].InitAsDescriptorTable(1, &rootRanges[0], D3D12_SHADER_VISIBILITY_ALL);
    rootParams[RootDescriptorIndex::MATERIAL_CONSTANTS].InitAsConstantBufferView(2, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_ALL);

    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = 0;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParams), rootParams, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    Debug::ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error),
        "Root Description creation failed!");

    Debug::ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),IID_PPV_ARGS(&m_rootSignature)),
        "Root Signature creation failed!");
}

ComPtr<ID3D12PipelineState> PipelineStateManager::CreatePipelineState(ID3D12Device* device, ComPtr<IDxcBlob> vs, ComPtr<IDxcBlob> ps)
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthStencilDesc.StencilEnable = FALSE;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout.data(), static_cast<UINT>(inputLayout.size()) };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE( { vs->GetBufferPointer(), vs->GetBufferSize() });
    psoDesc.PS = CD3DX12_SHADER_BYTECODE( { ps->GetBufferPointer(), ps->GetBufferSize() });
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    ComPtr<ID3D12PipelineState> pipelineState;
    auto hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));

    if (SUCCEEDED(hr)) return pipelineState;
    else return nullptr;
}
