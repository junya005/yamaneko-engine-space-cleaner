/// <file>SubModule.h</file>
/// <summary>
/// サブモジュールクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

namespace yamaneko_engine {
	class Module;
	class GameObject;
	struct Transform;
	class RHIShader;

	class SubModule {
	public:
		virtual ~SubModule() = default;
		void Initialize();
		void Update(float delta_time);

		void SetParentModule(Module* parent) { parent_ = parent; }

		Module* GetParentModule() const { return parent_; }

		GameObject* GetGameObject() const;

		Transform& GetTransform() const;

		RHIShader* GetShader() const;

	protected:
		virtual void OnInitialize() {};
		virtual void OnUpdate(float delta_time) {};

	private:
		Module* parent_ = nullptr;
		bool is_active_ = true;
	};
}