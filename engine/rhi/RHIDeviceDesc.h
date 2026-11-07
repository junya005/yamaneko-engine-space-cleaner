/// <file>RHIDeviceDesc.h</file>
/// <summary>
/// デバイス記述に関する構造体の定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

namespace yamaneko_engine
{
    struct RHIDeviceDesc {
        int api_version_major = 3;
        int api_version_minor = 2;
        bool enable_debug_layer = false;
        bool enable_vsync = true;
    };
}