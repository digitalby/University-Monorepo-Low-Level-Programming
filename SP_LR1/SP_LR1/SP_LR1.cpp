#include <string>
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <fstream>
namespace fs = std::filesystem;

int main()
{
	std::string nameOfOutput = "";
	std::string searchCriteria = "";
	std::ofstream fileStream;

	while (true) {
		std::cout << "Enter name of output file (ex.: output.txt): ";
		std::cin >> nameOfOutput;
		std::cout << std::endl;

		try {
			fileStream.open(nameOfOutput, std::ios::app);
			if (!fileStream.is_open()) {
				std::cout << "Couldn't open file stream." << std::endl;
				continue;
			}
		}
		catch(...) {
			std::cout << "File stream initialization exception." << std::endl;
			continue;
		}
		break;
	}

	std::cout << "Enter file extension to search for (ex.: jpg): ";
	std::cin >> searchCriteria;
	std::cout << std::endl;

	std::for_each(searchCriteria.begin(),
		searchCriteria.end(),
		[](char& ch) {
			ch = ::toupper(static_cast<unsigned char>(ch));
		});

	std::cout << "Processing..." << std::endl;

	int filesFound = 0;
	for (const auto& entry : fs::directory_iterator("")) {
		std::string entryPathString = entry.path().string();
		int fileExtensionPosition = entryPathString.rfind(".");
		if (fileExtensionPosition == -1) continue;
		std::string filetype = entryPathString.substr(fileExtensionPosition, entryPathString.length()-1);
		filetype = filetype.erase(0, 1);
		std::for_each(filetype.begin(),
			filetype.end(),
			[](char& ch) {
				ch = ::toupper(static_cast<unsigned char>(ch));
			});
		if (searchCriteria == filetype) {
			fileStream << entry.path() << std::endl;
			filesFound++;
		}
	}

	std::cout << "Operation completed OK with " << filesFound << " file(s) found." << std::endl;

	fileStream.close();
	_getch();
}