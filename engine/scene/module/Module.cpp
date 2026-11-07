/// <file>Module.cpp</file>
/// <summary>
/// モジュールクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>


#include "Module.h"
#include "../GameObject.h"

namespace yamaneko_engine {
	void Module::Initialize() {
		OnInitialize();
	}

	void Module::Update(float delta_time) {
		if (!is_active_) return;
		OnUpdate(delta_time);
		for (auto& sub_module : sub_modules_) {
			if (sub_module) {
				sub_module->Update(delta_time);
			}
		}
	}

	GameObject* Module::GetGameObject() const { return owner_; }
	Transform& Module::GetTransform() const { return owner_->transform; }
	RHIShader* Module::GetShader() const { return owner_->shader; }

}