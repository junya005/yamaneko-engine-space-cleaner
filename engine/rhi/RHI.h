/// <file>RHI.h</file>
/// <summary>
/// RHI操作に関する関数の定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <memory>
#include "RHIDevice.h"
#include "RHICommandList.h"

namespace yamaneko_engine::RHI {
	void Initialize(std::unique_ptr<RHIDevice> d);
	RHIDevice* GetDevice();
	RHICommandList* GetCommandList();
}