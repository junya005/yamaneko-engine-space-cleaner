/// <file>Application.cpp</file>
/// <summary>
/// アプリケーションのエントリーポイントおよびライフサイクル管理を提供するクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "Application.h"
#include "../platform/sdl/Window.h"
#include "../platform/sdl/Time.h"
#include "../platform/sdl/Input.h"
#include "../platform/imgui/ImGuiContext.h"
#include "../rhi/RHI.h"
#include "../rhi/RHIDeviceDesc.h"
#include "../rhi/backend/opengl/GLDevice.h"

namespace yamaneko_engine {
	bool Application::Initialize() {
		if(!Config::Load("settings/engine_config.ini")) {
			Config::Set(Config::EngineConfigData());
		}

		config_ = &Config::Get();

		Window::Initialize(
			config_->window.title,
			config_->window.width, 
			config_->window.height, 
			SDL_WINDOW_OPENGL | (config_->window.full_screen ? SDL_WINDOW_FULLSCREEN : 0)
		);

		RHIDeviceDesc desc{
			4, 
			5,
			false,
			config_->window.vsync
		};

		RHI::Initialize(std::make_unique<GLDevice>(desc));

		if(!RHI::GetDevice()->Initialize(Window::GetNativeHandle())) {
			SDL_Log("Failed to initialize RHI device.");
			return false;
		}

		GLDevice* gl_device = static_cast<GLDevice*>(RHI::GetDevice());
		gl_device->Initialize(Window::GetNativeHandle());
		if(!ImGuiContext::Initialize(Window::GetNativeHandle(), gl_device->GetContext())) {
			SDL_Log("Failed to initialize ImGui context.");
			return false;
		}

		Time::Initialize();
		Input::Initialize();

		OnInitialize(*config_);
		is_initialized_ = true;

		return true;
	}

	void Application::Run() {
		if(is_initialized_ == false) {
			SDL_Log("Application is not initialized. Call Initialize() before Run().");
			return;
		}

		// メインループ
		bool is_running = true;
		while (is_running) {
			Input::BeginFrame();
			
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					is_running = false;
				}
				ImGuiContext::ProcessEvent(e);
				Input::ProcessEvent(e);
			}

			Time::Update();
			OnUpdate(Time::GetDeltaTime());

			RHI::GetCommandList()->Clear(0.1f, 0.12f, 0.15f, 1.0f);
			RHI::GetCommandList()->SetDepthTest(true);

			OnRender();

			ImGuiContext::BeginFrame();
			OnUIRender();
			ImGuiContext::EndFrame();

			RHI::GetDevice()->Present();
		}
	}

	void Application::Shutdown() {
		ImGuiContext::Shutdown();
		Window::Cleanup();
	}
}