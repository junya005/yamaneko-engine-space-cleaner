/// <file>Time.cpp</file>
/// <summary>
/// 時間に関する機能を持ったクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "Time.h"
#include <SDL2/SDL.h>
#include <algorithm>

namespace yamaneko_engine::Time {
	namespace {
		uint64_t last_ticks = 0;
		float delta_time = 0.0f;
		float total_time = 0.0f;
		uint64_t frame_count = 0;
		float max_delta_time = 0.1f;
	}

	void Initialize() {
		last_ticks = SDL_GetPerformanceCounter();
		delta_time = 0.0f;
		total_time = 0.0f;
		frame_count = 0;
	}

	void Update() {
		uint64_t currentTicks = SDL_GetPerformanceCounter();
		uint64_t frequency = SDL_GetPerformanceFrequency();

		if (frequency == 0) {
			delta_time = 0.0f;
			return;
		}

		float rawDelta = static_cast<float>(currentTicks - last_ticks) / static_cast<float>(frequency);
		last_ticks = currentTicks;

		// デルタタイムのクランプ（デバッグ停止後や極端な負荷時のスパイク防止）
		delta_time = std::min(rawDelta, max_delta_time);
		total_time += delta_time;
		frame_count++;
	}

	float GetDeltaTime() {
		return delta_time;
	}

	float GetTotalTime() {
		return total_time;
	}

	uint64_t GetFrameCount() {
		return frame_count;
	}

	void SetMaxDeltaTime(float maxDeltaTime) {
		max_delta_time = maxDeltaTime;
	}

	float GetMaxDeltaTime() {
		return max_delta_time;
	}
}
