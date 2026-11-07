/// <file>SubModule.cpp</file>
/// <summary>
/// サブモジュールクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "SubModule.h"
#include "../../GameObject.h"
#include "../Module.h"

namespace yamaneko_engine {
	void SubModule::Initialize() {
		OnInitialize();
	}

	void SubModule::Update(float delta_time) {
		if (!is_active_) return;
		OnUpdate(delta_time);
	}
	
	GameObject* SubModule::GetGameObject() const { return parent_ ? parent_->GetGameObject() : nullptr; }

	Transform& SubModule::GetTransform() const { return parent_->GetTransform(); }

	RHIShader* SubModule::GetShader() const { return parent_ ? parent_->GetShader() : nullptr; }
}