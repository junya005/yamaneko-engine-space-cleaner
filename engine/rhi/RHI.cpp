/// <file>RHI.cpp</file>
/// <summary>
/// RHIモジュールの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "RHI.h"
#include <memory>

namespace yamaneko_engine::RHI {
	namespace {
		std::unique_ptr<RHIDevice> device = nullptr;
	}

	void Initialize(std::unique_ptr<RHIDevice> d) {
		device = std::move(d);
	}

	RHIDevice* GetDevice() {
		return device.get();
	}

	RHICommandList* GetCommandList() {
		return device->GetCommandList();
	}
}