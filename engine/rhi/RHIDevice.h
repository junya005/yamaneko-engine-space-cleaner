/// <file>RHIDevice.h</file>
/// <summary>
/// デバイス操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "RHIBuffer.h"
#include "RHIShader.h"
#include "RHICommandList.h"
#include <string>

namespace yamaneko_engine {
    class RHIDevice {
    public:
        virtual ~RHIDevice() = default;
        virtual bool Initialize(void* windowHandle) = 0;
        virtual void Present() = 0;
        virtual RHIBuffer* CreateVertexBuffer(size_t size, const void* data) = 0;
        virtual RHIBuffer* CreateIndexBuffer(size_t size, const void* data) = 0;
		virtual RHITexture* CreateTexture2D(int width, int height, TextureType type, const void* data) = 0;
        virtual RHIShader* CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
        virtual RHICommandList* GetCommandList() = 0;
    };
}
