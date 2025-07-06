#include "ShaderLoader.h"
#include "Debug.h"

ShaderLoader::ShaderLoader()
{
	Debug::ThrowIfFailed(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_utils)), "DXC Utils creation failed!");
	Debug::ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler)), "DXC Compiler creation failed!");
	Debug::ThrowIfFailed(m_utils->CreateDefaultIncludeHandler(&m_includeHandler), "DXC Include Handler failed!");
}

ComPtr<IDxcBlob> ShaderLoader::CompileShader(ShaderDesc desc)
{
	DxcBuffer sourceBuffer{};
	ComPtr<IDxcBlobEncoding> source;

	m_utils->LoadFile(desc.shaderFilePath.c_str(), nullptr, &source);
	sourceBuffer.Ptr = source->GetBufferPointer();
	sourceBuffer.Size = source->GetBufferSize();
	sourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR args[] = {
	L"-E", desc.shaderEntry.c_str(),        // Entry point
	L"-T", desc.shaderTarget.c_str(),       // Target profile
	L"-Zpr",						// Row-major
	L"-HV", L"2021",          // HLSL 2021
	L"-enable-16bit-types",
	L"-O3"                    // Optimization
	};

	ComPtr<IDxcBlob> shaderBlob;
	ComPtr<IDxcResult> result;
	ComPtr<IDxcBlobUtf8> errors;

	Debug::ThrowIfFailed(m_compiler->Compile(&sourceBuffer, args, _countof(args), m_includeHandler.Get(), IID_PPV_ARGS(&result)), "Shader Compilation failed!");
	Debug::ThrowIfFailed(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr));

	if (errors && errors->GetStringLength() > 0) 
		Debug::LogError(errors->GetStringPointer());

	Debug::ThrowIfFailed(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr));

	return shaderBlob;
}