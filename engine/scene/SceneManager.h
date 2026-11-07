/// <file>SceneManager.h</file>
/// <summary>
/// シーンマネージャークラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <memory>
#include "Scene.h"

namespace yamaneko_engine::SceneManager {
	void Initialize(std::unique_ptr<Scene> initial_scene);
	void Shutdown();
	void Update(float deltaTime);
	void Render();
	void OnUIRender();

	template<typename T, typename... Args>
	void ChangeScene(Args&&... args) {
		RequestSceneChange(std::make_unique<T>(std::forward<Args>(args)...));
	}

	void RequestSceneChange(std::unique_ptr<Scene> new_scene);

	Scene* GetCurrentScene();
}
