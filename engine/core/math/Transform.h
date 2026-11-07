/// <file>Transform.cpp</file>
/// <summary>
/// 空間上における情報を表す構造体の定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "Math.h"

namespace yamaneko_engine {
	struct Transform {
	public:
		Vector3 position;
		Vector3 rotation; // ラジアン単位
		Vector3 scale;
		Transform() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

		Matrix4 GetRotationMatrix() const {
			Matrix4 rot = Matrix4(1.0f);
			rot = Math::Rotate(rot, rotation.y, Vector3(0.0f, 1.0f, 0.0f));
			rot = Math::Rotate(rot, rotation.x, Vector3(1.0f, 0.0f, 0.0f));
			rot = Math::Rotate(rot, rotation.z, Vector3(0.0f, 0.0f, 1.0f));
			return rot;
		}

		Vector3 GetForwardVector() const {
			return Vector3(GetRotationMatrix() * Vector4(0.0f, 0.0f, -1.0f, 0.0f));
		}

		Vector3 GetUpVector() const {
			return Vector3(GetRotationMatrix() * Vector4(0.0f, 1.0f, 0.0f, 0.0f));
		}

		Vector3 GetRightVector() const {
			return Vector3(GetRotationMatrix() * Vector4(1.0f, 0.0f, 0.0f, 0.0f));
		}

		Matrix4 GetModelMatrix() const {
			Matrix4 model = Matrix4(1.0f);
			model = Math::Translate(model, position);
			model = model * GetRotationMatrix();
			model = Math::Scale(model, scale);
			return model;
		}
	};
}
