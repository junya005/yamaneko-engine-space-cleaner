/// <file>RHITexture.h</file>
/// <summary>
/// テクスチャ操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <string>

namespace yamaneko_engine {
	enum TextureType {
		Diffuse,
		Specular,
	};

	inline std::string TextureTypeToString(TextureType type) {
		switch (type) {
		case TextureType::Diffuse:
			return "texture_diffuse";
		case TextureType::Specular:
			return "texture_specular";
		default:
			return "Unknown";
		}
	}

	class RHITexture {
	public:
		virtual ~RHITexture() = default;
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual TextureType GetType() = 0;
	};
}

