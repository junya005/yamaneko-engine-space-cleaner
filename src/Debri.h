#pragma once

#include "../engine/Engine.h"

using namespace yamaneko_engine;

#define TRANSFORM GetTransform()

class Debri : public Module {
public:	
	void OnInitialize() override {
		float randomX = Random::Range(-4.0f, 4.0f);
		float randomY = Random::Range(-2.5f, 2.5f);

		TRANSFORM.position = Vector3(randomX, randomY, -20.0f);

		radius = 1.25f;

	}

	void OnUpdate(float delta_time) override {
		float move_distance = debri_speed * delta_time;

		Transform& transform = GetTransform();

		transform.position.z += move_distance;
	}

private:
	float radius = 1.25f;
	float debri_speed = 5.0f;
};
