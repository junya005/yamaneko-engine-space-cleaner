/// <file>FileSystem.h</file>
/// <summary>
/// ファイルの読み書き及びユーティリティに関するクラスの宣言。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
/// 
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace yamaneko_engine::FileSystem {
	/* text read/write */
	
	/// <summary>
	/// 指定されたパスのファイルを文字列として取得する
	/// </summary>
	/// <param name="path">取得するファイルのパス</param>
	/// <returns>ファイル中身の文字列</returns>
	std::string ReadAllText(const std::string& path);
	
	/// <summary>
	/// 指定されたパスのファイルに文字列を書き込む
	/// </summary>
	/// <param name="path">書き込むファイルのパス</param>
	/// <param name="text">書き込む内容</param>
	/// <returns>書き込みが成功したかどうか</returns>
	bool WriteAllText(const std::string& path, const std::string& text);

	/// <summary>
	/// 指定されたパスのファイルの末尾に文字列を書き込む
	/// </summary>
	/// <param name="path">書き込むファイルのパス</param>
	/// <param name="text">書き込む内容</param>
	/// <returns>書き込みが成功したかどうか</returns>
	bool AppendText(const std::string& path, const std::string& text);

	/* binary read/write */

	/// <summary>
	/// 指定されたパスのファイルをバイトとして取得する
	/// </summary>
	/// <param name="path">取得するファイルのパス</param>
	/// <returns>ファイル内容のバイト配列</returns>
	std::vector<char> ReadAllBytes(const std::string& path);

	/// <summary>
	/// 指定されたパスのファイルにバイトを書き込む 
	/// </summary>
	/// <param name="path">書き込むファイルのパス</param>
	/// <param name="bytes">書き込む内容</param>
	/// <returns>書き込みに成功したかどうか</returns>
	bool WriteAllBytes(const std::string& path, const std::vector<char>& bytes);

	/* utility */

	/// <summary>
	/// 指定されたパスのファイルが存在するか確認する
	/// </summary>
	/// <param name="path">調べるファイル</param>
	/// <returns>存在するか</returns>
	bool IsExists(const std::string& path);
}
