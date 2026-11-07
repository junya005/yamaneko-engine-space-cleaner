/// <file>FileSystem.cpp</file>
/// <summary>
/// ファイルの読み書き及びユーティリティに関するクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
/// 
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include "FileSystem.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <print>

namespace yamaneko_engine::FileSystem {
	std::string ReadAllText(const std::string& path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	bool WriteAllText(const std::string& path, const std::string& text) {
		std::ofstream file(path);
		if (!file.is_open()) {
			return false;
		}
		file << text;
		return true;
	}

	bool AppendText(const std::string& path, const std::string& text) {
		std::ofstream file(path, std::ios::app);
		if (!file.is_open()) {
			return false;
		}
		file << text;
		return true;
	}

	std::vector<char> ReadAllBytes(const std::string& path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			return std::vector<char>();
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);

		if (file.read(buffer.data(), size)) {
			std::cout << size << " バイトの読み込みに成功しました。\n";
			// ここで buffer を使ってデータを処理
		}
		return buffer;
	}

	bool WriteAllBytes(const std::string& path, const std::vector<char>& bytes) {
		std::ofstream file(path, std::ios::binary);
		if (!file.is_open()) {
			return false;
		}
		file.write(bytes.data(), bytes.size());
		return true;
	}

	bool IsExists(const std::string& path) {
		std::ifstream file(path);
		return file.is_open();
	}
}
