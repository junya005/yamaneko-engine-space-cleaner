/// <file>GameObject.h</file>
/// <summary>
/// ゲームオブジェクトクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>


#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../core/math/Transform.h"
#include "../renderer/3d/Model.h"
#include "../rhi/RHIShader.h"
#include "module/Module.h"

namespace yamaneko_engine {
	class GameObject {
	public:
		Transform transform;
		std::unique_ptr<Model> model = nullptr;
		RHIShader* shader = nullptr;

		bool is_active = true;

		float radius = 1.0f;

		std::string tag;

		GameObject(
			std::string tag, 
			std::string model_path,
			RHIShader* shader = nullptr
		) : 
			tag(tag), 
			model(nullptr),
			shader(shader)
		{
			if (!model_path.empty()) {
				model = std::make_unique<Model>(model_path.c_str());
			}
		}

		virtual ~GameObject() = default;

		template<typename T, typename... Args>
		T* AddModule(Args&&... args) {
			auto module = std::make_unique<T>(std::forward<Args>(args)...);
			T* module_ptr = module.get();
			module_ptr->SetOwner(this);
			modules_.push_back(std::move(module));
			module_ptr->Initialize();
			return module_ptr;
		}

		void Update(float delta_time) {
			for(auto& module : modules_) {
				if (module) {
					module->Update(delta_time);
				}
			}
		}

		void Draw() {
			if (!is_active || !model || !shader) return;

			shader->SetMat4("model", transform.GetModelMatrix());

			if (model != NULL) {
				model->Draw(*shader);
			}
		}

	private:
		std::vector<std::unique_ptr<GameObject>> children_;
		std::vector<std::unique_ptr<Module>> modules_;
	};
}
