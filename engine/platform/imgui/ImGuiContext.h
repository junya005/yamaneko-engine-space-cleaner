/// <file>ImGuiContext.h</file>
/// <summary>
/// ImGuiの機能をラップしたクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <imgui.h>
#include <SDL2/SDL.h>

namespace yamaneko_engine::ImGuiContext {
	bool Initialize(void* window_handle, void* gl_context);
    void Shutdown();
    void BeginFrame();
    void EndFrame();
	void ProcessEvent(const SDL_Event& e);
}
