/// <file>ViewInfo.cpp</file>
/// <summary>
/// 空間を描画するための情報を表すクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "Math.h"
#include "Transform.h"

namespace yamaneko_engine {
	enum MoveDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class ViewInfo {
	public:
		inline static ViewInfo* current = nullptr;
		inline static std::vector<ViewInfo*> view_info_instances = std::vector<ViewInfo*>();

		Transform transform;
		float fov = 45.0f; // 視野角
		bool for_debug = false; // デバッグ用カメラかどうか
		float aspect_ratio = 16.0f / 9.0f; // アスペクト比
		float near_plane = 0.1f; // 近平面距離
		float far_plane = 100.0f; // 遠平面距離

		ViewInfo(bool for_debug = false) : transform(), for_debug(for_debug) {
			if (current == nullptr) {
				current = this;
			}
			view_info_instances.push_back(this);
		}

		~ViewInfo() {
			auto it = std::find(view_info_instances.begin(), view_info_instances.end(), this);
			if (it != view_info_instances.end()) {
				view_info_instances.erase(it);
			}
			if (current == this) {
				current = view_info_instances.empty() ? nullptr : view_info_instances.back();
			}

			if (view_info_instances.empty())
			{
				current = nullptr;
				view_info_instances.clear();
			}
		}

		void ProcessMovement(MoveDirection direction, float velocity, float deltaTime) {
			if (!for_debug) return;

			velocity *= deltaTime; // カメラの移動速度

			Vector3 forward = transform.GetForwardVector();
			Vector3 right = transform.GetRightVector();
			Vector3 up = transform.GetUpVector();

			if (direction == FORWARD)
				transform.position += forward * velocity;
			if (direction == BACKWARD)
				transform.position -= forward * velocity;
			if (direction == RIGHT)
				transform.position += right * velocity;
			if (direction == LEFT)
				transform.position -= right * velocity;
			if (direction == UP)
				transform.position += up * velocity;
			if (direction == DOWN)
				transform.position -= up * velocity;
		}

		void ProcessViewMovement(float xoffset, float yoffset) {
			if (!for_debug) return;

			float sensitivity = 0.1f; // マウス感度
			xoffset *= sensitivity;
			yoffset *= sensitivity;
			transform.rotation.y += Math::Radians(xoffset);
			transform.rotation.x += Math::Radians(yoffset);
			// 上下の回転を制限
			if (transform.rotation.x > Math::Radians(89.0f))
				transform.rotation.x = Math::Radians(89.0f);
			if (transform.rotation.x < Math::Radians(-89.0f))
				transform.rotation.x = Math::Radians(-89.0f);
		}

		void ProcessZoom(float yoffset) {
			if (!for_debug) return;

			fov -= yoffset;
			if (fov < 1.0f)
				fov = 1.0f;
			if (fov > 45.0f)
				fov = 45.0f;
		}

		glm::highp_mat4 GetViewMatrix() const {
			Vector3 forward = transform.GetForwardVector();
			Vector3 up = transform.GetUpVector();
			return Math::LookAt(transform.position, transform.position + glm::normalize(forward), up);
		}

		glm::highp_mat4 GetProjectionMatrix() const {
			return Math::Perspective(Math::Radians(fov), aspect_ratio, near_plane, far_plane);
		}
	};
}
