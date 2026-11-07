/// <file>MediaLoader.cpp</file>
/// <summary>
/// メディアの読み込みに関するクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
/// 
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "MediaLoader.h"
#include <SDL_image.h>

namespace yamaneko_engine {
	SDL_Surface* MediaLoader::GetSurface(const std::string& path) {
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface) {
			return nullptr;
		}
		return surface;
	}

	SDL_Surface* MediaLoader::ConvertSurfaceFormat(SDL_Surface* surface, Uint32 pixel_format) {
		if (!surface) {
			return nullptr;
		}
		SDL_Surface* converted_surface = SDL_ConvertSurfaceFormat(surface, pixel_format, 0);
		if (!converted_surface) {
			return nullptr;
		}
		return converted_surface;
	}
}
