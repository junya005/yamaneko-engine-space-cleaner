/// <file>Random.h</file>
/// <summary>
/// 乱数生成に関する関数の宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <random>

namespace yamaneko_engine::Random {
	void Initialize(int seed = -1);
	int Range(int a, int b);
	float Range(float a, float b);
	double Range(double a, double b);
}