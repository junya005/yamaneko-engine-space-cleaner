/// <file>UIRenderer.h</file>
/// <summary>
/// UI描画機能を提供するクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "../../platform/imgui/ImGuiContext.h"
#include "../../rhi/RHIDevice.h"
#include "../../rhi/backend/opengl/GLDevice.h"
#include "../../platform/sdl/Window.h"

namespace yamaneko_engine {
	class UIRenderer {
	public:
		static bool Initialize(RHIDevice& device) {
			GLDevice* gl_device = dynamic_cast<GLDevice*>(&device);
			if (!gl_device) {
				SDL_Log("UIRenderer::Initialize: Failed to cast RHIDevice to GLDevice.");
				return false;
			}
			return ImGuiContext::Initialize(Window::GetNativeHandle(), gl_device->GetContext());
		}

		static void BeginRender() {
			ImGuiContext::BeginFrame();
		}

		static void EndRender() {
			ImGuiContext::EndFrame();
		}

		static void Shutdown() {
			ImGuiContext::Shutdown();
		}
	};
}
