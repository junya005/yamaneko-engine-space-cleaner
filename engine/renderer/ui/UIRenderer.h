/// <file>UIRenderer.h</file>
/// <summary>
/// UI描画機能を提供するクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../rhi/RHIDevice.h"

namespace yamaneko_engine {
	class UIRenderer {
	public:
		static bool Initialize(RHIDevice& device);
		static void BeginRender();
		static void EndRender();
		static void Shutdown();
	};
}
