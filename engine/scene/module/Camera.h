/// <file>Camera.h</file>
/// <summary>
/// カメラモジュールクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <memory>
#include "Module.h"
#include "core/math/ViewInfo.h"

namespace yamaneko_engine {
	class Camera : public Module {
	public:
		void OnInitialize() {
			camera_ = std::make_unique<ViewInfo>(false);
		}

		void OnUpdate(float delta_time) override {
			if (camera_->current != camera_.get()) return;

			camera_->transform = GetTransform();

			RHIShader* shader = GetShader();
			if (!shader) return;

			// ビュー行列の作成
			Matrix4 view = Matrix4(1.0f);
			view = camera_->GetViewMatrix();

			// プロジェクション行列の作成
			Matrix4 projection = Matrix4(1.0f);
			projection = camera_->GetProjectionMatrix();

			shader->SetMat4("view", view);
			shader->SetMat4("projection", projection);
		}

	private:
		std::unique_ptr<ViewInfo> camera_;
	};
}