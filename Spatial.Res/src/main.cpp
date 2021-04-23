#include <algorithm>
#include <argh.h>
#include <ghc/filesystem.hpp>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

namespace fs = ghc::filesystem;

std::string toVariableName(const fs::path& filePath, char separator, const std::locale& locale)
{
	auto fileName = filePath.filename().string();

	std::transform(fileName.begin(), fileName.end(), fileName.begin(), [&](auto c) {
		if (std::isalpha(c, locale))
			return std::toupper(c, locale);
		else
			return separator;
	});

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
	const auto workingDir = fs::path{args[0]}.parent_path();

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

		const auto relativeInputFile = fs::relative(inputFile, workingDir);
		const auto variableName = prefix + toVariableName(relativeInputFile, '_', locale);
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
		outputHeader << fmt::format("\n\n// {0}", relativeInputFile.string());
		outputHeader << fmt::format("\nextern const char {0}[{1}];", variableName, count);
		outputHeader << fmt::format("\nconstexpr unsigned int {0}_SIZE = {1};", variableName, count);
	}

	return 0;
}