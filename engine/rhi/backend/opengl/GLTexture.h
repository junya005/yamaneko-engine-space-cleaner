/// <file>GLTexture.h</file>
/// <summary>
/// OpenGLテクスチャ操作に関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../RHITexture.h"
#include <glad/glad.h>

namespace yamaneko_engine {
	class GLTexture : public RHITexture {
    public:
        GLTexture(int width, int height, TextureType type, const void* data, unsigned int format = GL_RGBA)
            : width_(width), height_(height), type_(type), id_(0)
        {
            glGenTextures(1, &id_);
            glBindTexture(GL_TEXTURE_2D, id_);
          
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            
            if (data) {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        ~GLTexture() override {
            if (id_ != 0) {
                glDeleteTextures(1, &id_);
                id_ = 0;
            }
        }
        
        GLTexture(const GLTexture&) = delete;
        GLTexture& operator=(const GLTexture&) = delete;
        
        int GetWidth() override { return width_; }
        int GetHeight() override { return height_; }
		TextureType GetType() override { return type_; }
       
        void Bind(unsigned int slot = 0) const {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, id_);
        }
        
        void Unbind(unsigned int slot = 0) const {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        unsigned int GetID() const { return id_; }

    private:
        unsigned int id_ = 0;
        int width_ = 0;
        int height_ = 0;
		TextureType type_;
	};
}
