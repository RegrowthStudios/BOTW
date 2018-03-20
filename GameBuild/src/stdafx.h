#pragma once

#include <Vorb/stdafx.h>

#include <stdlib.h>
#include <wchar.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <assert.h>

// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// D3D headers
#include <dxgi1_5.h>
#include <d3d11_4.h>

// GL Headers
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GL.h>

// Vorb headers
#include <Vorb\types.h>
#include <Vorb\Delegate.hpp>
#include <Vorb\Event.hpp>
template<typename T>
using SimpleVector = std::vector<T>;
template<typename... Args>
using StdEvent = Event<SimpleVector, Args...>;

#include "Global.h"
#include "Utils.h"
