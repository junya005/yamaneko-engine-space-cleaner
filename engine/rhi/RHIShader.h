/// <file>RHIShader.h</file>
/// <summary>
/// シェーダー操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <glm/glm.hpp>
#include <string>

namespace yamaneko_engine {
	class RHIShader {
	public:
		virtual ~RHIShader() = default;

		virtual void SetBool(const std::string& name, bool value) const = 0;
		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetVec2(const std::string& name, float x, float y) const = 0;
		virtual void SetVec3(const std::string& name, float x, float y, float z) const = 0;
		virtual void SetVec4(const std::string& name, float x, float y, float z, float w) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) const = 0;
		virtual void SetSampler2D(const std::string& name, int textureUnit) const = 0;
	};
}

