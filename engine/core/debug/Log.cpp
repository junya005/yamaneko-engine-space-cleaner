/// <file>Log.h</file>
/// <summary>
/// ログに関する関数の実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "Log.h"

namespace yamaneko_engine::Log {
	namespace {
		std::mutex mutex_;
		std::deque<LogEntry> entries_;
		std::size_t capacity_;
		LogLevel minimum_level_;

		[[nondiscard]]
		bool ShouldLog(LogLevel level) {
			return level < minimum_level_;
		}

		[[nondiscard]]
		bool IsHistoryEnabled() {
			return capacity_ > 0;
		}

		void OutputLog(
			std::string_view log_level_name,
			std::string_view category,
			std::string_view message
		) {
			std::clog
				<< '[' << log_level_name << "] "
				<< '[' << category << "] "
				<< message << '\n';
		}

		void AppendHistoryUnlocked(LogEntry&& entry) {
			if (entries_.size() >= capacity_) {
				entries_.pop_front();
			}

			entries_.push_back(std::move(entry));
		}
	}
	
	/// <summary>
	/// ログの最低レベルを設定します
	/// </summary>
	/// <param name="level"></param>
	void SetMinimumLevel(LogLevel level) {
		std::scoped_lock lock(mutex_);
		minimum_level_ = level;
	}

	/// <summary>
	/// ログシステムを初期化します
	/// </summary>
	/// <param name="capacity">保存するログの許容量</param>
	void Initialize(std::size_t capacity) {
		capacity_ = capacity;
		SetMinimumLevel(LogLevel::Trace);
	}
	
	/// <summary>
	/// ログを書きます
	/// </summary>
	/// <param name="level">ログのレベル</param>
	/// <param name="category">ログのカテゴリ</param>
	/// <param name="message">ログのメッセージ</param>
	void Write(
		LogLevel level,
		std::string_view category,
		std::string_view message
	) {
		std::scoped_lock lock(mutex_);

		if (!ShouldLog(level)) {
			return;
		}

		OutputLog(GetLogLevelName(level), category, message);

		if (!IsHistoryEnabled()) {
			return;
		}

		LogEntry entry{
			level,
			std::string(category),
			std::string(message)
		};

		AppendHistoryUnlocked(std::move(entry));
	}

	/// <summary>
	/// 数値の中身をログとして出力します
	/// </summary>
	/// <typeparam name="...Args">出力したい数値の型配列</typeparam>
	/// <param name="level">ログのレベル</param>
	/// <param name="category">ログのカテゴリ</param>
	/// <param name="format">ログのメッセージのフォーマット</param>
	/// <param name="...args">フォーマットに埋め込む数値の配列</param>
	template<class... Args>
	void Writef(
		LogLevel level,
		std::string_view category,
		std::format_string<Args...> format,
		Args&&... args
	) {
		Write(
			level,
			category,
			std::format(format, std::forward<Args>(args)...)
		);
	}

	/// <summary>
	/// ログ履歴をコピーして返します。
	/// </summary>
	/// <returns>ログ履歴の配列</returns>
	[[nodiscard]]
	std::vector<LogEntry> Snapshot() {
		std::scoped_lock lock(mutex_);
		return std::vector<LogEntry>(
			entries_.begin(), entries_.end()
		);
	}

	/// <summary>
	/// ログ履歴を削除します
	/// </summary>
	void Clear() {
		std::scoped_lock lock(mutex_);
		entries_.clear();
	}
}