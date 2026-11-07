/// <file>ImGuiContext.cpp</file>
/// <summary>
/// ImGuiの機能をラップしたクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "ImGuiContext.h"
#include <SDL2/SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace yamaneko_engine::ImGuiContext {
	bool Initialize(void* window_handle, void* gl_context) {
		// ImGuiの初期化
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// ImGuiバックエンドの初期化
		SDL_Window* window = static_cast<SDL_Window*>(window_handle);
		if (!window) {
			return false;
		}

		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init("#version 150");
		return true;
	}

	void Shutdown() {
		// ImGuiの終了処理
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void BeginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void EndFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ProcessEvent(const SDL_Event& e) {
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
}
