#pragma once
#include "pch.h"

static const UINT FRAME_COUNT = 2; // Number of frames to buffer for double/triple buffering

static const size_t KILOBYTE = 1024;
static const size_t MEGABYTE = 1024 * KILOBYTE;

static const size_t POOL_SIZE_SMALL = 512 * KILOBYTE;  // 0.5 MB
static const size_t POOL_SIZE_MEDIUM = 1 * MEGABYTE;  // 1 MB
static const size_t POOL_SIZE_LARGE = 2 * MEGABYTE;  // 2 MB
static const size_t POOL_SIZE_HUGE = 4 * MEGABYTE;  // 4 MB