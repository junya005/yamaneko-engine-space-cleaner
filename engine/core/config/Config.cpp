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

#include "Config.h"
#include "../io/FileSystem.h"
#include <fstream>
#include <sstream>

namespace yamaneko_engine::Config {
    namespace {
        EngineConfigData engine_config_data_;

    }

     EngineConfigData& Get() {
		return engine_config_data_;
	}

    void Set(const EngineConfigData& config) {
        engine_config_data_ = config;
    }

	bool Load(const std::string& path) {
        if (!FileSystem::IsExists(path)) {
            return false;
        }

        std::string content = FileSystem::ReadAllText(path);
        if (content.empty()) {
            return false;
        }

        std::stringstream ss(content);
        std::string line;
        std::string current_section = "";

        while (std::getline(ss, line)) {
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }
            
            if (line.front() == '[' && line.back() == ']') {
                current_section = line.substr(1, line.size() - 2);
                continue;
            }
            
            size_t delimiter_pos = line.find('=');
            if (delimiter_pos == std::string::npos) {
                continue;
            }

            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);
            
            if (current_section == "Window") {
                if (key == "title") {
                    engine_config_data_.window.title = value;
                }
                else if (key == "width") {
                    engine_config_data_.window.width = std::stoi(value);
                }
                else if (key == "height") {
                    engine_config_data_.window.height = std::stoi(value);
                }
                else if (key == "vsync") {
                    engine_config_data_.window.vsync = (value == "1" || value == "true");
                }
                else if (key == "full_screen") {
                    engine_config_data_.window.full_screen = (value == "1" || value == "true");
                }
            }
        }
        return true;
	}
	
	bool Save(const std::string& path) {
		std::stringstream ss;
        
        ss << "[Window]\n";
        ss << "title=" << engine_config_data_.window.title << "\n";
        ss << "width=" << engine_config_data_.window.width << "\n";
        ss << "height=" << engine_config_data_.window.height << "\n";
        ss << "vsync=" << (engine_config_data_.window.vsync ? 1 : 0) << "\n";
        ss << "full_screen=" << (engine_config_data_.window.full_screen ? 1 : 0) << "\n";
        
        return FileSystem::WriteAllText(path, ss.str());
	}
}
