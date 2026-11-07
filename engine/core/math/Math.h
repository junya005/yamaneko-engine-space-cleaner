/// <file>Math.cpp</file>
/// <summary>
/// glmのエイリアスと数学的関数の定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

namespace yamaneko_engine {
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Vector2Int = glm::ivec2;
	using Matrix4 = glm::mat4;

	namespace Math {
		inline float Radians(float degrees) { return glm::radians(degrees); }

		inline Matrix4 Translate(const Matrix4& matrix, const Vector3& vector) { return glm::translate(matrix, vector); }
		inline Matrix4 Rotate(const Matrix4& matrix, float angle, const Vector3& axis) { return glm::rotate(matrix, angle, axis); }
		inline Matrix4 Scale(const Matrix4& matrix, const Vector3& scale) { return glm::scale(matrix, scale); }

		inline Matrix4 LookAt(Vector3 eye, Vector3 center, Vector3 up) { return glm::lookAt(eye, center, up); }
		inline Matrix4 Perspective(float fov, float aspect, float near, float far) { return glm::perspective(fov, aspect, near, far); }

		inline float Dot(Vector3 a, Vector3 b) { return glm::dot(a, b); }
	}
}
