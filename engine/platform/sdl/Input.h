/// <file>Input.h</file>
/// <summary>
/// SDL2のAPIを用いてハードウェア入力を取得するクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <SDL2/SDL.h>

namespace yamaneko_engine::Input {
	void Initialize();
	void BeginFrame();
	void ProcessEvent(const SDL_Event& e);
	bool GetKey(SDL_Scancode scancode);
	bool GetKeyDown(SDL_Keycode keycode);
	bool GetKeyUp(SDL_Keycode keycode);
	float GetMouseWheel();
}
