#pragma once

#include <imgui.h>
#include "../engine/main/EntryPoint.h"
#include "../engine/scene/SceneManager.h"
#include "MainScene.h"

using namespace yamaneko_engine;

class GameApp : public Application {
public:
    GameApp() : Application() {}

    void OnInitialize(Config::EngineConfigData& config) override {
		SceneManager::Initialize(std::make_unique<MainScene>());
	}

	void OnUpdate(float deltaTime) override {
		SceneManager::Update(deltaTime);
	}

	void OnRender() override {
		SceneManager::Render();
	}

	void OnUIRender() override {
		SceneManager::OnUIRender();
	}

	void OnShutdown() override {
		SceneManager::Shutdown();
	}

private:

};
