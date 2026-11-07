/// <file>EntryPoint.h</file>
/// <summary>
/// エントリーポイントに関する関数の実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "Application.h"

extern yamaneko_engine::Application* yamaneko_engine::CreateApplication();

int main(int argc, char** argv) {
	auto app = yamaneko_engine::CreateApplication();

	if (app->Initialize()) {
		app->Run();
		app->Shutdown();
	}

	delete app;
	return 0;
}
