/// <file>GLShader.h</file>
/// <summary>
/// OpenGLシェーダー操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../../core/io/FileSystem.h"
#include "../../RHIShader.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace yamaneko_engine{
	class GLShader : public RHIShader {
	public:
		GLShader(const char* vertex_path, const char* fragment_path) {
			// 頂点シェーダーのロード
			std::string vertexShaderSource = FileSystem::ReadAllText(vertex_path);
			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
			glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
			glCompileShader(vertexShader);

			int  success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			// フラグメントシェーダーのロード
			std::string fragmentShaderSource = FileSystem::ReadAllText(fragment_path);
			unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
			glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			// シェーダープログラムの作成
			id_ = glCreateProgram();
			glAttachShader(id_, vertexShader);
			glAttachShader(id_, fragmentShader);
			glLinkProgram(id_);

			glGetProgramiv(id_, GL_LINK_STATUS, &success);

			if (!success) {
				glGetProgramInfoLog(id_, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
		}

		~GLShader() override {
			glDeleteProgram(id_);
		}

		void SetBool(const std::string& name, bool value) const override {
			glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
		}

		void SetInt(const std::string& name, int value) const override {
			glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
		}

		void SetFloat(const std::string& name, float value) const override {
			glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
		}

		void SetVec2(const std::string& name, float x, float y) const override {
			glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
		}

		void SetVec3(const std::string& name, float x, float y, float z) const override {
			glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
		}

		void SetVec4(const std::string& name, float x, float y, float z, float w) const override {
			glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
		}

		void SetMat4(const std::string& name, const glm::mat4& mat) const override {
			glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
		}

		void SetSampler2D(const std::string& name, int textureUnit) const override {
			glUniform1i(glGetUniformLocation(id_, name.c_str()), textureUnit);
		}

		GLuint GetID() { return id_; }

	private:
		GLuint id_;
	};
}
