#pragma once
#include "pch.h"

static const UINT FRAME_COUNT = 2; // Number of frames to buffer for double/triple buffering

static const UINT KILOBYTE = 1024;
static const UINT MEGABYTE = 1024 * KILOBYTE;

static const UINT POOL_SIZE_SMALL = 512 * KILOBYTE;  // 0.5 MB
static const UINT POOL_SIZE_MEDIUM = 1 * MEGABYTE;   // 1 MB
static const UINT POOL_SIZE_LARGE = 2 * MEGABYTE;    // 2 MB
static const UINT POOL_SIZE_HUGE = 4 * MEGABYTE;