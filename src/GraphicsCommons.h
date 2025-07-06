#pragma once

enum RootDescriptorIndex : int {
	OBJECT_CONSTANTS = 0,
	TEXTURES = 1,
	MATERIAL_CONSTANTS = 2,
	FRAME_CONSTANTS = 3
};

const UINT MAX_SRV_COUNT = 128;