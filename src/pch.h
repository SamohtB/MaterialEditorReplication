#pragma once

// === System headers ===
#include <windows.h>
#include <wrl.h>                
#include <shellapi.h>
#include <comdef.h> 

// === STL ===
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <memory>
#include <stack>
#include <cassert>

// === DirectX 12 core === 
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <dxcapi.h>
#include "d3dx12.h"

// === DEAR IMGUI ===
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

// WRL
using Microsoft::WRL::ComPtr;
using String = std::string;

