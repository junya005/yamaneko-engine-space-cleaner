/// <file>GLDevice.h</file>
/// <summary>
/// OpenGLデバイスクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../RHIDevice.h"
#include "../../RHIDeviceDesc.h"
#include "../../RHIBuffer.h"
#include "GLBuffer.h"
#include "../../RHIShader.h"
#include "GLShader.h"
#include "GLCommandList.h"
#include <glad/glad.h>
#include <SDL2/SDL.h>

namespace yamaneko_engine {
    class GLDevice : public RHIDevice {
    public:
        GLDevice(RHIDeviceDesc &desc) : desc_(desc) {
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, desc.api_version_major);
		    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, desc.api_version_minor);
		    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			command_list_ = GLCommandList();
        }

        ~GLDevice() override {
            if (gl_context_) {
                SDL_GL_DeleteContext(gl_context_);
                gl_context_ = nullptr;
            }
        }

        bool Initialize(void* window_handle) override {
            window_ = static_cast<SDL_Window*>(window_handle);
            if (!window_) return false;
            
			// OpenGLコンテキストの作成
			gl_context_ = SDL_GL_CreateContext(window_);
			if (gl_context_ == nullptr) {
				SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
				return false;
			}

			// GLADの初期化
			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
				SDL_Log("Failed to initialize GLAD");
				return false;
			}

			// 垂直同期（VSync）の設定
			if (desc_.enable_vsync) {
				if (SDL_GL_SetSwapInterval(1) < 0) {
					SDL_Log("Warning: Unable to set VSync: %s", SDL_GetError());
				}
			} else {
				SDL_GL_SetSwapInterval(0);
			}

            return true;
        }

        void Present() override {
			if (window_) {
				SDL_GL_SwapWindow(window_);
			}
        }

		RHIBuffer* CreateVertexBuffer(size_t size, const void* data) override {
			return new GLBuffer(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		RHIBuffer* CreateIndexBuffer(size_t size, const void* data) override {
			return new GLBuffer(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}

		RHITexture* CreateTexture2D(int width, int height, TextureType type, const void* data) override {
			return new GLTexture(width, height, type, data);
		}

		RHIShader* CreateShader(const std::string& vertex_src, const std::string& fragment_src) override {
			return new GLShader(vertex_src.c_str(), fragment_src.c_str());
		}

		RHICommandList* GetCommandList() override {
			return &command_list_;
		}

		void* GetContext() const { return gl_context_; }

    private:
        RHIDeviceDesc desc_;
        SDL_Window* window_;
        SDL_GLContext gl_context_;
		GLCommandList command_list_;
    };
}

