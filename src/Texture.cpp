#include "Texture.h"

Texture::Texture(ComPtr<ID3D12Resource> buffer, UINT index)
	: m_textureResource(buffer), m_srvHandleIndex(index) 
{

}
