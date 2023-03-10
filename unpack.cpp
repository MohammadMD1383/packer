#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "defs.hpp"
#include "FileInfo.hpp"
#include "util.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::program_options;

int main(int argc, char **argv) {
	options_description description{
		""
		"╔═══════════════════════════════════════════╗" "\n"
		"║          Packer::UnPack - v0.1.0          ║" "\n"
		"╚═══════════════════════════════════════════╝" "\n"
		"      unpacks a previously packed file       "
		"\n\n"
		"usage: unpack [options] <input>"
		"\n\n"
		"options"
	};
	
	description.add_options()
		("help,h", "produce this help message")
		("version,v", "show version information")
		("verbose,V", "print all logs to console")
		("print,p", "if specified, will print contents of the package")
		("input", value<string>()->required(), "input file");
	
	positional_options_description args;
	args.add("input", 1);
	
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
		ERROR_MSG("missing '<" << e.get_option_name().substr(2) << ">' argument");
		return 1;
	}
	
	auto verbose = vmap.contains("verbose");
	
	VERBOSE INFO_MSG("checking input file");
	path inputFilePath{vmap["input"].as<string>()};
	if (!exists(inputFilePath)) {
		ERROR_MSG("input file '" << inputFilePath.string() << "' doesn't exist");
		return -1;
	}
	
	VERBOSE INFO_MSG("reading metadata");
	ifstream inputFile{inputFilePath.c_str(), ios::binary};
	uint64_t filesCount;
	inputFile.read(reinterpret_cast<char *>(&filesCount), sizeof filesCount);
	
	auto maxLength = 0;
	FileInfo fi{};
	map<path, uint64_t> outputs;
	for (uint64_t i = 0; i < filesCount; ++i) {
		inputFile >> fi;
		maxLength = max(maxLength, static_cast<decltype(maxLength)>(fi.nameSize));
		auto buffer = new char[fi.nameSize + 1];
		inputFile.read(buffer, fi.nameSize);
		buffer[fi.nameSize] = 0;
		outputs[path(buffer)] = fi.fileSize;
		delete[] buffer;
	}
	
	if (vmap.contains("print")) {
		for (const auto &output: outputs)
			cout << left
			     << setw(maxLength) << output.first.string()
			     << " | "
			     << setw(20) << output.second << " bytes"
			     << endl;
		
		return 0;
	}
	
	VERBOSE INFO_MSG("preparing for output");
	auto buffer = new char[BUFFER_SIZE];
	for (const auto &item: outputs) {
		goto next_file_end;
		next_file:
		continue;
		next_file_end:
		
		const auto &filePath = item.first;
		const auto &fileSize = item.second;
		
		if (exists(filePath)) {
			WARNING_MSG("output file '" << filePath.string() << "' exists; overwrite it? [y/n]");
			loop {
				switch (readKey()) {
					case 'y':
						goto loop_end;
					case 'n':
						inputFile.seekg(fileSize, ios::cur);
						goto next_file;
				}
			}
			loop_end:;
		}
		
		ofstream outputFile{filePath.c_str(), ios::binary | ios::trunc};
		auto tmpSize = fileSize;
		
		VERBOSE INFO_MSG("unpacking file '" << filePath.string() << "'");
		loop {
			if (tmpSize <= BUFFER_SIZE) {
				inputFile.read(buffer, tmpSize);
				outputFile.write(buffer, tmpSize);
				break;
			}
			
			inputFile.read(buffer, BUFFER_SIZE);
			outputFile.write(buffer, BUFFER_SIZE);
			tmpSize -= BUFFER_SIZE;
		}
		
		outputFile.close();
	}
	
	delete[] buffer;
	INFO_MSG("done");
	
	return 0;
}
