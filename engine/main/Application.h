/// <file>Application.h</file>
/// <summary>
/// アプリケーションのエントリーポイントおよびライフサイクル管理を提供するクラスの宣言。
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
#include "../core/config/Config.h"
#include "../rhi/RHIDevice.h"

namespace yamaneko_engine {
	class Application {
	public:
		Application() : config_(nullptr) { instance_ = this; }

		~Application() = default;

		virtual void OnInitialize(Config::EngineConfigData& config) {};
		virtual void OnUpdate(float deltaTime) = 0;
		virtual void OnRender() = 0;
		virtual void OnUIRender() = 0;
		virtual void OnShutdown() = 0;

		bool Initialize();
		void Run();
		void Shutdown();

		static Application* Get() { return instance_; }
	
	private:
		inline static Application* instance_;
		Config::EngineConfigData* config_;
		bool is_initialized_ = false;
	};

	Application* CreateApplication();
}
