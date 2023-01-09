#include <iostream>
#include <fstream>
#include <cstdint>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "defs.hpp"
#include "util.hpp"
#include "FileInfo.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::program_options;

int main(int argc, char **argv) {
	options_description description{
		""
		"╔═══════════════════════════════════════════╗" "\n"
		"║           Packer::Pack - v0.1.0           ║" "\n"
		"╚═══════════════════════════════════════════╝" "\n"
		"   packs multiple files into a single file   " "\n"
		"     without any compression - very fast     "
		"\n\n"
		"usage: pack [options] <output> <inputs...>"
		"\n\n"
		"options"
	};
	
	description.add_options()
		("help,h", "produce this help message")
		("version,v", "show version information")
		("verbose,V", "print all logs to console")
		("output", value<string>()->required(), "output file")
		("inputs", value<vector<string>>()->required(), "input files");
	
	positional_options_description args;
	args.add("output", 1);
	args.add("inputs", -1);
	
	variables_map vmap;
	store(
		command_line_parser(argc, argv)
			.options(description).positional(args).run(),
		vmap
	);
	
	if (vmap.contains("help")) {
		cout << description << endl;
		return 0;
	}
	
	if (vmap.contains("version")) {
		cout << "v" VERSION << endl;
		return 0;
	}
	
	try {
		notify(vmap);
	} catch (const wrapexcept<required_option> &e) {
		ERROR("missing '<" << e.get_option_name().substr(2) << ">' argument");
		return 1;
	}
	
	auto verbose = vmap.contains("verbose");
	
	VERBOSE INFO("preparing output file");
	ofstream outputFile;
	path outputFilePath{vmap["output"].as<string>()};
	
	if (exists(outputFilePath)) {
		WARNING("output file '" << outputFilePath.string() << "' exists; overwrite it? [y/n]");
		loop {
			switch (readKey()) {
				case 'y':
					goto loop_end;
				case 'n':
					INFO("cancelled");
					return 0;
			}
		}
		loop_end:;
	}
	
	VERBOSE INFO("writing files count");
	outputFile.open(outputFilePath.c_str(), ios::binary | ios::trunc);
	const auto &inputs = vmap["inputs"].as<vector<string>>();
	uint64_t inputsCount = inputs.size();
	outputFile.write(reinterpret_cast<char *>(&inputsCount), sizeof inputsCount);
	
	VERBOSE INFO("preparing input files");
	vector<path> inputFilePaths;
	transform(
		inputs.begin(), inputs.end(), back_inserter(inputFilePaths),
		[](const auto &item) {
			path p{std::move(item)};
			if (!exists(p)) {
				ERROR("input file '" << p.string() << "' doesn't exist");
				exit(-2);
			}
			if (!is_regular_file(p)) {
				ERROR("'" << p.string() << "' is not a file");
				exit(-3);
			}
			return p;
		}
	);
	
	VERBOSE INFO("writing metadata");
	FileInfo fi{};
	for (const auto &item: inputFilePaths) {
		const auto &name = item.string();
		fi.nameSize = static_cast<uint16_t>(name.size());
		fi.fileSize = file_size(item);
		outputFile << fi;
		outputFile << name;
	}
	
	auto buffer = new char[BUFFER_SIZE];
	
	for (const auto &item: inputFilePaths) {
		VERBOSE INFO("packing file '" << item.string() << "'");
		ifstream inputFile{item.c_str(), ios::binary};
		loop {
			auto readAmount = inputFile.readsome(buffer, BUFFER_SIZE);
			outputFile.write(buffer, readAmount);
			if (readAmount < BUFFER_SIZE) break;
		}
		inputFile.close();
	}
	
	delete[] buffer;
	outputFile.close();
	INFO("done");
	
	return 0;
}
