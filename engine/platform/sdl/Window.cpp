/// <file>Window.cpp</file>
/// <summary>
/// ウィンドウ操作に関する関数の実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "Window.h"

namespace yamaneko_engine::Window
{
	namespace {
		SDL_Window* window = nullptr;
		int width = 0;
		int height = 0;
		bool is_fullscreen = false;
	}

	bool Initialize(const std::string& title, int w, int h, Uint32 flags) {
		width = w;
		height = h;

		// ウィンドウ作成
		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			flags
		);

		if (window == nullptr) {
			SDL_Log("Failed to window initialization: %s", SDL_GetError());
			return false;
		}

		return true;
	}

	void Cleanup() {
		if (window) {
			SDL_DestroyWindow(window);
			window = nullptr;
		}	
	}

	void SetSize(int w, int h) {
		if (!window) return;
		width = w;
		height = h;
		SDL_SetWindowSize(window, width, height);
	}

	void SetFullScreen(bool fullscreen) {
		if (!window) return;
		Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
		SDL_SetWindowFullscreen(window, fullscreen);
		SDL_SetWindowSize(window, width, height);
	}

	void ToggleFullScreen() {
		SetFullScreen(IsFullScreen());
	}

	bool IsFullScreen() {
		if (!window) return false;
		Uint32 flags = SDL_GetWindowFlags(window);
		return (flags & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP)) != 0;
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	int GetWidthCenter() { return width / 2; }
	int GetHeightCenter() { return height / 2; }
	void* GetNativeHandle() { return window; }
}