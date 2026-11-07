#pragma once

#include <vector>
#include <memory>
#include <imgui.h>
#include <SDL2/SDL.h>
#include "../engine/scene/Scene.h"
#include "../engine/scene/Collision.h"
#include "../engine/platform/sdl/Window.h"
#include "../engine/platform/sdl/Input.h"
#include "../engine/core/math/Random.h"
#include "../engine/platform/sdl/Time.h"
#include "../engine/rhi/RHI.h"
#include "../engine/scene/module/Camera.h"
#include "Player.h"
#include "Debri.h"

using namespace yamaneko_engine;

class MainScene : public Scene {
public:
	MainScene() {
		RHIDevice* device = RHI::GetDevice();
		shader_ = device->CreateShader(
			"assets/shader/simple_vertex_shader.glsl", 
			"assets/shader/simple_fragment_shader.glsl"
		);

		// プレイヤーの自機モデル読み込み
		auto player = std::make_unique<GameObject>("Player", "assets/model/airplane/airplane.obj", shader_);
		player->AddModule<Player>();
		airplane_ = player.get();
		AddGameObject(std::move(player));

		// カメラの作成
		auto camera = std::make_unique<GameObject>("Camera", "", shader_);
		camera->AddModule<Camera>();
		camera->transform.position = Vector3(0.0f,0.0f,10.0f);
		AddGameObject(std::move(camera));
		
		keycodes_ = {SDLK_1, SDLK_2, SDLK_3, SDLK_4};
		current_keycode_ = keycodes_[current_keycode_index_];
	}
	
	void OnEnter() override {
	
	}

	void OnExit() override {
	
	}

	void OnUpdate(float deltaTime) override {
		int screen_width_ = Window::GetWidth();
		int screen_height_ = Window::GetHeight();

		if (!is_game_over_)
		{
			if (Input::GetKeyDown(current_keycode_) && !is_invinsible_) {
				succes_count_++;
				current_keycode_index_ = Random::Range(0, 3);
				current_keycode_ = keycodes_[current_keycode_index_];
				ok_display_time_current_ = 0.0f;

				if (succes_count_ == succes_count_max_) {
					is_invinsible_ = true;
					invinsible_time_current_ = invinsible_time_;
					succes_count_ = 0;
				}
			}

			if (ok_display_time_current_ <= ok_display_time_) {
				ok_display_time_current_+=deltaTime;
			}

			if (is_invinsible_) {
				invinsible_time_current_ -= deltaTime;
				if (invinsible_time_current_ < 0) {
					is_invinsible_ = false;
				}
			}

			if (time_stamp_ <= Time::GetTotalTime()) {
				std::unique_ptr<GameObject> debri = std::make_unique<GameObject>("Debri", "assets/model/debri/debri.obj", shader_);
				debri->AddModule<Debri>();
				AddGameObject(std::move(debri));
				time_stamp_ += debri_spawn_time_span_;
			}

			RemoveGameObjects([](const std::unique_ptr<GameObject>& go) {
				return go->tag == "Debri" && (go->transform.position.z > 5.0f || go->is_active == false);
			});

			std::vector<GameObject*> debris = GetAllGameObjectsByTag("Debri");
			for (auto& go : debris) {
				if (Physics::CheckSphereCollision(*airplane_, *go)) {
					if (!is_invinsible_) {
						is_game_over_ = true;
						go->is_active = false;
					}
					else {
						score_++;
						go->is_active = false;
					}
				}
			}

			// 画面外への飛び出し防止（クランプ処理）
			if (screen_width_ > 0 && screen_height_ > 0) {
				airplane_->transform.position.x = std::clamp(airplane_->transform.position.x, -4.0f, 4.0f);
				airplane_->transform.position.y = std::clamp(airplane_->transform.position.y, -2.5f, 2.5f);
			}
		}
	}

	void OnRender() override {
		
	}

	void OnUIRender() override {
		// 画面のサイズ（解像度）を取得する
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 screenSize = io.DisplaySize;

		// ウィンドウの位置を左上に、サイズを画面全体に強制固定する
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(screenSize, ImGuiCond_Always);

		if (ImGui::Begin("Template Controller & Debug", nullptr, canvas_flags_)) {
			ImGui::SetWindowFontScale(5.0f);
			
			if (is_game_over_){
				ImVec2 font_size = ImGui::CalcTextSize("Game Over");
				ImGui::SetCursorPos(
					ImVec2(
						(float)Window::GetWidth()*0.5f - font_size.x*0.5f,
						(float)Window::GetHeight()*0.5f - font_size.y*0.5f
					)
				);
				ImGui::Text("Game Over\nScore: %d", score_);
			}
			else {
				ImGui::SetCursorPos(ImVec2(20.0f,20.0f));
				ImGui::Text("Score: %d", score_);
				ImGui::SetCursorPos(ImVec2(20.0f, 80.0f));
				ImGui::Text("Charge Power: %d", static_cast<int>(charge_power_));
				
				if (is_invinsible_) {
					ImGui::SetCursorPos(ImVec2(20.0f, 140.0f));
					ImGui::Text("Invinsible Time: %d", static_cast<int>(invinsible_time_current_));
				}
				else {
					ImGui::SetCursorPos(ImVec2((float)Window::GetWidth() * 0.5f, 600.0f));
					ImGui::Text("Next Key: %d", current_keycode_index_ + 1);
				}

				if (ok_display_time_current_ < ok_display_time_) {
					ImGui::SetCursorPos(ImVec2(
						(float)Window::GetWidth() * 0.5f, 
						(float)Window::GetHeight()*0.5f
					));
					ImGui::Text("OK", current_keycode_index_ + 1);
				}
			}
		}
		ImGui::End();
	}

private:
	// 背景や枠線をすべて透明・無効化するフラグを設定
	ImGuiWindowFlags canvas_flags_ = ImGuiWindowFlags_NoTitleBar     // タイトルバーなし
		| ImGuiWindowFlags_NoResize       // サイズ変更不可
		| ImGuiWindowFlags_NoMove         // 移動不可
		| ImGuiWindowFlags_NoScrollbar    // スクロールバーなし
		| ImGuiWindowFlags_NoBackground   // 背景を完全に透明化
		| ImGuiWindowFlags_NoCollapse     // 折りたたみ不可
		| ImGuiWindowFlags_NoNav;         // キーボード・ゲームパッドナビ無効化（必要に応じて）
	
	RHIShader* shader_ = nullptr;
	GameObject* airplane_ = nullptr;
	float time_stamp_ = 0;
	float debri_spawn_time_span_ = 1.5f;
	float charge_power_ = 0.0f;
	float charge_power_max_ = 100.0f;
	float charge_speed_ = 30.0f;
	float invinsible_time_ = 5.0f;
	float invinsible_time_current_ = 0.0f;
	int score_ = 0;
	bool is_invinsible_ = false;
	bool is_game_over_ = false;
	std::vector<SDL_KeyCode> keycodes_;
	int current_keycode_index_ = Random::Range(0, 3);
	SDL_KeyCode current_keycode_;
	int succes_count_ = 0;
	int succes_count_max_ = 4;
	float ok_display_time_ = 0.5f;
	float ok_display_time_current_ = ok_display_time_;
};