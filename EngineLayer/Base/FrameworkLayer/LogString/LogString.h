#pragma once
#include <Windows.h>
#include <string>
#include <format>


/// -------------------------------------------------------------
///							ログ出力
/// -------------------------------------------------------------
class LogString
{
public: /// ---------- メンバ関数 ---------- ///

	// ログ出力
	static void Log(const std::string& message);
	
	// 文字コードユーティリティ
	// string->wstring
	static std::wstring ConvertString(const std::string& str);
	
	// wstring->string
	static std::string ConvertString(const std::wstring& str);
};

