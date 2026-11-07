/// <file>Config.h</file>
/// <summary>
/// エンジンの設定に関する構造体、クラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
/// 
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <string>

namespace yamaneko_engine::Config {
	struct WindowConfig {
		std::string title = "Template Game";
		int width = 1280;
		int height = 720;
		bool vsync = true;
		bool full_screen = false;
	};
	
	struct EngineConfigData {
		WindowConfig window;
	};
	
	EngineConfigData& Get();
	void Set(const EngineConfigData& data);
	bool Load(const std::string& path);
	bool Save(const std::string& path);
}
