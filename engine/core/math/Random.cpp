/// <file>Random.cpp</file>
/// <summary>
/// 乱数生成に関する関数の実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "Random.h"

namespace yamaneko_engine::Random {
	namespace {
		std::random_device rd;
		std::mt19937 engine;
	}

	void Initialize(int seed){
		if (seed == -1) {
			engine = std::mt19937(rd());
			return;
		}

		engine = std::mt19937(seed);
	}

	int Range(int a, int b) {
		std::uniform_int_distribution<int> dist(a, b);
		return dist(engine);
	}

	float Range(float a, float b) {
		std::uniform_real_distribution<float> dist(a, b);
		return dist(engine);
	}

	double Range(double a, double b) {
		std::uniform_real_distribution<double> dist(a, b);
		return dist(engine);
	}
}