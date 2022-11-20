#pragma once
#include <filesystem>
#include <optional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <windows.h>

namespace fs = std::filesystem;

class File {
public:
	static std::optional<std::vector<fs::path>> GetAllFilesInDirectoryWithExtension(const std::wstring& directory, const std::string& extension) {
		std::vector<fs::path> result;
		auto entries = fs::directory_iterator(directory);
		if (!fs::exists(directory) || !fs::is_directory(directory)) {
			return {};
		}

		for (const auto& entry : entries) {
			bool hasCorrectExtension = entry.path().extension().generic_string().compare(extension) == 0;
			if (hasCorrectExtension)
				result.push_back(entry);
		}
		return result;
	}

	static std::optional<std::string> ReadFile(const std::wstring& filePath) {
		if (!fs::exists(filePath)) {
			return {};
		}
		std::ifstream fileStream;
		fileStream.open(filePath);

		std::ostringstream strStream;
		strStream << fileStream.rdbuf();
		return strStream.str();
	}

	static bool WriteFile(const std::wstring filePath, std::string contents) {
		std::fstream fileStream;
		fileStream.open(filePath, std::ios::out);
		fileStream.write(contents.c_str(), contents.size());

		return true;
	}

	static std::wstring ExePath() {
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		return std::wstring(buffer).substr(0, pos);
	}
};
