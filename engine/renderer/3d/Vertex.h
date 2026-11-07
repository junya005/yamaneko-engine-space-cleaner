/// <file>Vertex.h</file>
/// <summary>
/// 頂点に関する構造体の定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../core/math/Math.h"

namespace yamaneko_engine {
	struct Vertex {
		Vector3 position;
		Vector3 normal;
		Vector2 tex_coords;
	};
}
