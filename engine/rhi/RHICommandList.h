/// <file>RHICommandList.h</file>
/// <summary>
/// コマンドリストに関するクラスの定義。
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
#include "RHITexture.h"

namespace yamaneko_engine {
	class RHICommandList {
	public:
		virtual void Clear(float r, float g, float b, float a) = 0;
		virtual void SetViewPort(int x, int y, int width, int height) = 0;
		virtual void SetDepthTest(bool enable) = 0;
		virtual void SetVertexBuffer(RHIBuffer* buffer) = 0;
		virtual void SetIndexBuffer(RHIBuffer* buffer) = 0;
		virtual void SetTexture(int slot, RHITexture* texture) = 0;
		virtual void SetShader(RHIShader* shader) = 0;
		virtual void Draw(uint32_t vertex_count, uint32_t start_vertex = 0) = 0;
		virtual void DrawIndexed(uint32_t index_count) = 0;
	};
}
