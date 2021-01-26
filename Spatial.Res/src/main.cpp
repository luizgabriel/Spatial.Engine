#include <argh.h>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

std::string toVariableName(std::string fileName, char separator, const std::locale& locale)
{
	for (size_t i = 0; i < fileName.size(); i++) {
		if (std::isalpha(fileName[i], locale)) {
			fileName[i] = std::toupper(fileName[i], locale);
		} else {
			fileName[i] = separator;
		}
	}

	return fileName;
}

std::string formatToHex(char value)
{
	return fmt::format("{:#04x}", value);
}

int main(int argc, char** argv)
{
	const auto args = argh::parser(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);
	const auto locale = std::locale{"C"};

	fs::path outputHeaderPath;
	if (!(args({"-h", "--header"}) >> outputHeaderPath))
	{
		std::cerr << "Option \"-h | --header [path]\" required.";
		return 1;
	}

	fs::path outputSourcePath;
	if (!(args({"-s", "--source"}) >> outputSourcePath))
	{
		std::cerr << "Option \"-s | --source [path]\" required.";
		return 1;
	}

	size_t columnSize;
	if (!(args({"-c", "--c"}) >> columnSize))
	{
		columnSize = 64;
	}

	std::string prefix;
	args({"-p", "--prefix"}) >> prefix;

	auto outputHeader = std::ofstream{outputHeaderPath};
	auto outputSource = std::ofstream{outputSourcePath};

	outputHeader << "#pragma once\n";
	outputHeader << "\n#include <stddef.h>\n";

	size_t i = 1;
	fs::path inputFile;
	while (args(i) >> inputFile)
	{
		i++;

		if (inputFile.empty())
		{
			return 0;
		}
		else if (!fs::exists(inputFile))
		{
			std::cerr << "File not found: " << inputFile << std::endl;
			return 1;
		}

		auto variableName = prefix + toVariableName(inputFile.stem().string(), '_', locale);
		std::cout << fmt::format("Generating {1} and {1}_SIZE\n", inputFile.string(), variableName);
		size_t count = 0;
		outputSource << fmt::format("\nextern const char {0}[] = {{\n", variableName);

		{
			auto ifs = std::ifstream{inputFile};
			auto ift = std::istreambuf_iterator<char>{ifs};
			std::for_each(ift, std::istreambuf_iterator<char>{}, [&](char c) {
				outputSource << formatToHex(c) << ',';
				count++;

				if (count % columnSize == 0)
					outputSource << "\n";
			});
		}

		outputSource << "\n};" << std::endl;

		// Build header file
		outputHeader << fmt::format("\n\n// {0}", inputFile.string());
		outputHeader << fmt::format("\nextern const char {0}[{1}];", variableName, count);
		outputHeader << fmt::format("\nconstexpr unsigned int {0}_SIZE = {1};", variableName, count);

	}

	return 0;
}