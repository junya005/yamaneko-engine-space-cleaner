/// <file>GLCommandList.h</file>
/// <summary>
/// OpenGLコマンドリスト操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../RHICommandList.h"
#include "GLBuffer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include <glad/glad.h>

namespace yamaneko_engine {
	class GLCommandList : public RHICommandList {
	public:
		void Clear(float r, float g, float b, float a) override {
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void SetViewPort(int x, int y, int width, int height) override {
			glViewport(x, y, width, height);
		}

		void SetDepthTest(bool enable) override {
			if (enable)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}

		void SetVertexBuffer(RHIBuffer* buffer) override {
			GLBuffer* gl_buffer = static_cast<GLBuffer*>(buffer);
			if (!gl_buffer) return;

			if (gl_buffer->GetVAO() != 0)
			{
				glBindVertexArray(gl_buffer->GetVAO());
			}
		}

		void SetIndexBuffer(RHIBuffer* buffer) override {
			GLBuffer* gl_buffer = static_cast<GLBuffer*>(buffer);
			if (!gl_buffer) return;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer->GetID());
		}

		void SetTexture(int slot, RHITexture* texture) override {
			GLTexture* gl_texture = static_cast<GLTexture*>(texture);
			if (!gl_texture) return;

			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, gl_texture->GetID());
		}

		void SetShader(RHIShader* shader) override {
			GLShader* gl_shader = static_cast<GLShader*>(shader);
			if (!gl_shader) return;

			glUseProgram(gl_shader->GetID());
		}

		void Draw(uint32_t vertex_count, uint32_t start_vertex = 0) override {
			glDrawArrays(GL_TRIANGLES, start_vertex, vertex_count);
		}

		void DrawIndexed(uint32_t index_count) override {
			glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
		}
	};
}
