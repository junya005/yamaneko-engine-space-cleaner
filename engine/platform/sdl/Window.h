/// <file>Window.h</file>
/// <summary>
/// ウィンドウ操作に関する関数の宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <string>

namespace yamaneko_engine::Window
{
	bool Initialize(const std::string& title, int width, int height, Uint32 flags);
	void Cleanup();
	void SetSize(int width, int height);
	void SetFullScreen(bool fullscreen);
	void ToggleFullScreen();
	bool IsFullScreen();

	int GetWidth();
	int GetHeight();
	int GetWidthCenter();
	int GetHeightCenter();
		
	void* GetNativeHandle();
}
