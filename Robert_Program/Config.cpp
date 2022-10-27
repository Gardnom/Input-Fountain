#include "Config.h"

namespace Config {
	void LoadInputImages(InputImageHandler* inputImageHandler) {
		File f;

		if (auto pngFiles = f.GetAllFilesInDirectoryWithExtension("H:/dev/Robert_png/Resize", ".png")) {
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