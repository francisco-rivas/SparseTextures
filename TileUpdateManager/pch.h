//*********************************************************
//
// Copyright 2020 Intel Corporation 
//
// Permission is hereby granted, free of charge, to any 
// person obtaining a copy of this software and associated 
// documentation files(the "Software"), to deal in the Software 
// without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to 
// whom the Software is furnished to do so, subject to the 
// following conditions :
// The above copyright notice and this permission notice shall 
// be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
//
//*********************************************************

#pragma once

#include <windows.h>
#undef max
#undef min

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <dxcapi.h>         // Be sure to link with dxcompiler.lib.
#include <d3d12shader.h>    // Shader reflection.

#include <string>
#include <vector>
#include <memory>      // for unique_ptr
#include <list>
#include <wrl.h>
#include <shellapi.h>
#include <stdexcept>
#include <sstream>
#include <crtdbg.h>
#include <ppl.h>
#include <random>
#include <fstream>
#include <functional>
#include <algorithm>
#include <memory>
#include "d3dx12.h"

#ifdef _DEBUG
#include <assert.h>
#define ASSERT(X) assert(X)
#define DebugPrint(...) OutputDebugString(Streaming::AutoString::Concat(__VA_ARGS__).c_str());
#else
#define ASSERT(X)
#define DebugPrint(...)
#endif
