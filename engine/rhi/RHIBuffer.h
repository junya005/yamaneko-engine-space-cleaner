/// <file>RHIBuffer.h</file>
/// <summary>
/// バッファ操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

namespace yamaneko_engine {
	class RHIBuffer {
	public:
		virtual ~RHIBuffer() = default;

		virtual void UpdateData(const void* data, size_t size, size_t offset = 0) = 0;
		virtual size_t GetSize() = 0;
	};
}
