/// <file>Collision.h</file>
/// <summary>
/// 衝突判定ロジックの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../core/math/Math.h"
#include "GameObject.h"

namespace yamaneko_engine::Physics {
	inline bool CheckSphereCollision(const GameObject& a, const GameObject& b) {
		Vector3 diff = b.transform.position - a.transform.position;

		float dist_sq = Math::Dot(diff, diff);
		float radius_sum = a.radius + b.radius;

		return dist_sq <= (radius_sum * radius_sum);
	}
}