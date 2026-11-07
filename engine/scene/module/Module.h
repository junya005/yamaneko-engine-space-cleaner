/// <file>Module.h</file>
/// <summary>
/// モジュールクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>


#pragma once

#include <vector>
#include <memory>
#include "sub/SubModule.h"

namespace yamaneko_engine {
	class GameObject;
    struct Transform;
    class RHIShader;

	class Module {
	public:
		virtual ~Module() = default;
		void Initialize();
		void Update(float delta_time);

		void SetOwner(GameObject* owner) { owner_ = owner; }

		template<typename T, typename... Args>
		T* AddSubModule(Args&&... args) {
			auto sub_module = std::make_unique<T>(std::forward<Args>(args)...);
			T* sub_module_ptr = sub_module.get();
			sub_module_ptr->SetParentModule(this);
			sub_modules_.push_back(std::move(sub_module));
			sub_module_ptr->Initialize();
			return sub_module_ptr;
		}

		GameObject* GetGameObject() const;
		Transform& GetTransform() const;
		RHIShader* GetShader() const;

	protected:
		virtual void OnInitialize() {};
		virtual void OnUpdate(float delta_time) {};

	private:
		bool is_active_ = true;
		GameObject* owner_ = nullptr;
		std::vector<std::unique_ptr<SubModule>> sub_modules_;
	};
}