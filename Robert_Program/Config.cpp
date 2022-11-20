#include "Config.h"

namespace Config {
	void LoadInputImages(InputImageHandler* inputImageHandler) {
		File f;
		//std::string imageDirectory = "H:/dev/Robert_png/Resize";
		std::wstring imageDirectory = File::ExePath() + std::wstring(L"/images");
		printf(std::string(imageDirectory.begin(), imageDirectory.end()).c_str());
		if (auto pngFiles = f.GetAllFilesInDirectoryWithExtension(imageDirectory, ".png")) {
			for (auto file : *pngFiles) {
				std::string str = file.string();
				std::cout << file << "\n";
				std::wstring wsTmp(str.begin(), str.end());
				inputImageHandler->AddInputImage(wsTmp);
			}
		}
	}

	void FirstTimeSetup(InputImageHandler* inputImageHandler) {
		LoadInputImages(inputImageHandler);
	}
}