#pragma once
#include "pch.h"
#include "ShaderTypes.h"
#include <dxcapi.h>

class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader() = default;

	ComPtr<IDxcBlob> CompileShader(ShaderDesc desc);

private:
	ComPtr<IDxcCompiler3> m_compiler;
	ComPtr<IDxcUtils> m_utils;
	ComPtr<IDxcIncludeHandler> m_includeHandler;
};

