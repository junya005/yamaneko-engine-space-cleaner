/// <file>SceneManager.cpp</file>
/// <summary>
/// シーンマネージャークラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "SceneManager.h"
#include "Scene.h"

namespace yamaneko_engine::SceneManager {
	namespace {
		std::unique_ptr<Scene> current_scene = nullptr;
		std::unique_ptr<Scene> next_scene = nullptr;
		bool has_pending_transition = false;
	}

	void Initialize(std::unique_ptr<Scene> initial_scene) {
		current_scene = std::move(initial_scene);
		next_scene.reset();
		has_pending_transition = false;
	}

	void Shutdown() {
		if (current_scene) {
			current_scene->OnExit();
			current_scene.reset();
		}
		next_scene.reset();
		has_pending_transition = false;
	}

	void RequestSceneChange(std::unique_ptr<Scene> new_scene) {
		next_scene = std::move(new_scene);
		has_pending_transition = true;
	}

	void ProcessTransition() {
		if (!has_pending_transition) {
			return;
		}

		if (current_scene) {
			current_scene->OnExit();
		}

		current_scene = std::move(next_scene);
		has_pending_transition = false;
		if (current_scene) {
			current_scene->OnEnter();
		}
	}

	void Update(float deltaTime) {
		// 遅延シーン遷移を適用
		ProcessTransition();

		if (current_scene) {
			current_scene->Update(deltaTime);
		}
	}

	void Render() {
		if (current_scene) {
			current_scene->Render();
		}
	}

	void OnUIRender() {
		if (current_scene) {
			current_scene->OnUIRender();
		}
	}

	Scene* GetCurrentScene() { 
		return current_scene.get(); 
	}
}
