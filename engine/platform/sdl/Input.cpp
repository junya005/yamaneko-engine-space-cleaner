/// <file>Input.cpp</file>
/// <summary>
/// SDLのAPIを用いてハードウェア入力を取得するクラスの実装
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "Input.h"
#include <unordered_map>

namespace yamaneko_engine::Input{
	namespace {
		std::unordered_map<SDL_Keycode, bool> key_downs;
		std::unordered_map<SDL_Keycode, bool> key_ups;
		float mouse_wheel_y;
		const Uint8* keyboard_state;
	}

	void Initialize() {
		keyboard_state = SDL_GetKeyboardState(nullptr);
	}

	void BeginFrame() {
		key_downs.clear();
		key_ups.clear();
	}

	void ProcessEvent(const SDL_Event& e) {
		if (e.type == SDL_KEYDOWN) {
			if (e.key.repeat == 0) {
				key_downs[e.key.keysym.sym] = true;
			}
		}

		if (e.type == SDL_KEYUP) {
			if (e.key.repeat == 0) {
				key_ups[e.key.keysym.sym] = true;
			}
		}

		if (e.type == SDL_MOUSEWHEEL) {
			mouse_wheel_y = static_cast<float>(e.wheel.y);
		}
		else {
			mouse_wheel_y = 0.0f;
		}
	}

	bool GetKey(SDL_Scancode scancode) {
		if (keyboard_state) {
			return keyboard_state[scancode] != 0;
		}
		return false;
	}

	bool GetKeyDown(SDL_Keycode keycode) {
		auto it = key_downs.find(keycode);
		if (it != key_downs.end()) {
			return it->second;
		}
		return false;
	}

	bool GetKeyUp(SDL_Keycode keycode) {
		auto it = key_ups.find(keycode);
		if (it != key_ups.end()) {
			return it->second;
		}
		return false;
	}

	float GetMouseWheel() {
		return mouse_wheel_y;
	}
}
