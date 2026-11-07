/// <file>GLBuffer.h</file>
/// <summary>
/// OpenGLバッファ操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../RHIBuffer.h"
#include <glad/glad.h>

namespace yamaneko_engine {
	class GLBuffer : public RHIBuffer {
    public:
		GLBuffer(unsigned int target, size_t size, const void* data, unsigned int usage = GL_STATIC_DRAW)
            : target_(target), size_(size), id_(0)
        {
            // VAOはOpenGLのみなので、ここで生成
            if (target == GL_ARRAY_BUFFER) {
                glGenVertexArrays(1, &vao_);
                glBindVertexArray(vao_);
            }

            glGenBuffers(1, &id_);
            glBindBuffer(target_, id_);
            glBufferData(target_, size, data, usage);

            if (target_ == GL_ARRAY_BUFFER) {
                struct VertexLayout {
                    float pos[3];
                    float normal[3];
                    float uv[2];
                };

                GLsizei stride = sizeof(VertexLayout);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));

                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));

                glBindVertexArray(0);
            }

            glBindBuffer(target_, 0);
        }

        ~GLBuffer() {
            if (id_ != 0) {
                glDeleteBuffers(1, &id_);
                id_ = 0;
            }

            if (vao_ != 0) {
                glDeleteVertexArrays(1, &vao_);
                vao_ = 0;
            }
        }

        GLBuffer(const GLBuffer&) = delete;
        GLBuffer& operator=(const GLBuffer&) = delete;

        void UpdateData(const void* data, size_t size, size_t offset = 0) override {
            glBindBuffer(target_, id_);
            glBufferSubData(target_, offset, size, data);
            glBindBuffer(target_, 0);
        }

        size_t GetSize() override { return size_; }

        unsigned int GetID() const { return id_; }
        unsigned int GetVAO() const { return vao_; }
        unsigned int GetTarget() const { return target_; }

    private:
        unsigned int id_ = 0;
        unsigned int vao_ = 0;
        unsigned int target_ = GL_ARRAY_BUFFER;
        size_t size_ = 0;
    };
}
