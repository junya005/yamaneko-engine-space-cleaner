#pragma once

#include "../engine/Engine.h"

using namespace yamaneko_engine;

class Player : public Module{
public:
	float player_speed = 5.0f;

	void OnUpdate(float delta_time) override {
		if(!GetGameObject()) return;

		float move_distance = player_speed * delta_time;
		Transform& transform = GetTransform();

		if (Input::GetKey(SDL_SCANCODE_W)) {
			transform.position.y += move_distance;
		}
		if (Input::GetKey(SDL_SCANCODE_S)) {
			transform.position.y -= move_distance;
		}
		if (Input::GetKey(SDL_SCANCODE_A)) {
			transform.position.x -= move_distance;
		}
		if (Input::GetKey(SDL_SCANCODE_D)) {
			transform.position.x += move_distance;
		}
	}
};
