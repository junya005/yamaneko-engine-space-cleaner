/// <file>MediaLoader.h</file>
/// <summary>
/// メディアの読み込みに関するクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
/// 
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "SDL2/SDL.h"
#include <string>

namespace yamaneko_engine {
	class MediaLoader {
	public:
		static SDL_Surface* GetSurface(const std::string& path);
		static SDL_Surface* ConvertSurfaceFormat(SDL_Surface* surface, Uint32 pixel_format);
	};
}
