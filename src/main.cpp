#include "hzip.hpp"
#include "hfile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

void help();

int main(int argc, char* argv[])
{
	if (argc > 1 && argc <= 5) {
		std::string option = argv[1];

		if (option == "-c") {
			std::string source_file_path = argv[2];
			std::string dest_file_path = argv[3];

			ifstream inFile;
			inFile.open(source_file_path);

			ofstream headerFile;
			ofstream dataFile;
			
			std::cout << "Compressing [" << source_file_path << "]...\n";
			compress(inFile, headerFile, dataFile, 
					 source_file_path, dest_file_path, h_extension::har);
			std::cout << "Finished.\n";
			std::cout << "Saved at: " << dest_file_path << std::endl;
			inFile.close();
			headerFile.close();
			dataFile.close();
		}

		else if (option == "-j") {
			std::string header_file_path = argv[2];
			std::string data_file_path = argv[3];

			std::string dest_file_path = argv[4];

			ifstream headerFile;
			headerFile.open(header_file_path);

			ifstream dataFile;
			dataFile.open(data_file_path);

			ofstream joinedFile;
			std::cout << "Joining 2 files "
					  << "[" << header_file_path << "] + [" << data_file_path << "]...\n";
			joinFile(headerFile, dataFile, joinedFile, header_file_path, dest_file_path);
			std::cout << "Finished.\n";

			std::cout << "Saved at: " << dest_file_path << std::endl;
			headerFile.close();
			dataFile.close();
			joinedFile.close();
		}

		else if (option == "-d") {
			std::string source_file_path = argv[2];
			std::string dest_file_path = argv[3];

			ifstream inFile;
			inFile.open(source_file_path);

			ofstream decompressedFile;

			std::cout << "Decompressing [" << source_file_path << "]...\n";
			decompress(inFile, decompressedFile, source_file_path, dest_file_path);
			std::cout << "Finished.\n";
			std::cout << "Saved at: " << dest_file_path << std::endl;
			inFile.close();
			decompressedFile.close();
		}

		else if (option == "--help") {
			help();
		}

		else {
			std::cout << "Invalid option.\n";
			std::cout << "Try \"--help\" for more info.\n";
		} 
	}
	
	else {
		std::cout << "Not enough or too many arguments.\n";
		std::cout << "Try \"--help\" for more info.\n";
	}
	
	return EXIT_SUCCESS;
}

void help()
{
	std::cout << "Author: Harry Nguyen.\n";
	std::cout << "Version: SRAR v1.0\n";
	std::cout << "Hello from 2018.\n\n";
	std::cout << "USAGE: \n";
	std::cout << "For compression: srar -c <path_to_raw_file> <path_to_dir>\n";
	std::cout << "For decompression: srar -d <path_to_raw_file> <path_to_dir>\n";
	std::cout << "For joining 2 files: srar -j <path_to_first_file> <path_to_second_file> <path_to_dir>\n";
	std::cout << "For showing this messsage: srar --help\n";
}
