#pragma once
#include <string>
#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class Texture
{
public:
    Texture(ComPtr<ID3D12Resource> buffer, UINT index);
    ~Texture() = default;

private:
    ComPtr<ID3D12Resource> m_textureResource;
    UINT m_srvHandleIndex;
};

