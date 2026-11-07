/// <file>Scene.h</file>
/// <summary>
/// シーンクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <vector>
#include <memory>
#include "GameObject.h"

namespace yamaneko_engine {
	class Scene {
	public:
		~Scene() {
			OnDestory();
		};

		void AddGameObject(std::unique_ptr<GameObject> game_object) {
			game_objects_.push_back(std::move(game_object));
		}

		void RemoveGameObject(GameObject* game_object) {
			game_objects_.erase(
				std::remove_if(
					game_objects_.begin(),
					game_objects_.end(),
					[game_object](const std::unique_ptr<GameObject>& obj) {
						return obj.get() == game_object;
					}
				),
				game_objects_.end()
			);
		}

		void RemoveGameObjects(std::function<bool(const std::unique_ptr<GameObject>&)> predicate) {
			game_objects_.erase(
				std::remove_if(
					game_objects_.begin(),
					game_objects_.end(),
					predicate
				),
				game_objects_.end()
			);
		}


		void RemoveGameObjectByTag(const std::string& tag) {
			game_objects_.erase(
				std::remove_if(
					game_objects_.begin(),
					game_objects_.end(),
					[&tag](const std::unique_ptr<GameObject>& obj) {
						return obj->tag == tag;
					}
				),
				game_objects_.end()
			);
		}

		std::vector<GameObject*> GetAllGameObjectsByTag(const std::string& tag) {
			std::vector<GameObject*> result;
			for (auto& game_object : game_objects_) {
				if (game_object && game_object->tag == tag) {
					result.push_back(game_object.get());
				}
			}
			return result;
		}

		void Update(float deltaTime) {
			for (auto& game_object : game_objects_) {
				if (game_object) {
					game_object->Update(deltaTime);
				}
			}

			OnUpdate(deltaTime);
		}

		void Render() {
			for (auto& game_object : game_objects_) {
				if (game_object) {
					game_object->Draw();
				}
			}
			OnRender();
		}

		virtual void OnEnter() {}
		virtual void OnExit() {}
		virtual void OnUpdate(float deltaTime) { (void)deltaTime; }
		virtual void OnRender() {}
		virtual void OnUIRender() {}
		virtual void OnDestory() {}

	private:
		std::vector<std::unique_ptr<GameObject>> game_objects_;
	};
}
