#include "hzip.hpp"
#include "hfile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

std::string getCompressedFilePath(const std::string& path_to_file, 
								  const std::string& extension);
std::string getDecompressedFilePath(const std::string& path_to_file);
void help();

int main(int argc, char* argv[])
{
	std::string option = argv[1];
	if (argc == 3) {

		std::string path_to_file = argv[2];
		ifstream iFile;
		iFile.open(path_to_file);

		if (iFile.is_open()) {
		   	if (option == "-c") {
		   		std::cout << "Compressing...\n";
		   		ofstream encodedFile;
		   		compress(iFile, encodedFile, path_to_file, h_extension::tom);
		   		encodedFile.close();
		   		std::cout << "Finished.\n";

		   		std::cout << "Compressed file path: "
		   		          << getCompressedFilePath(path_to_file, h_extension::tom) 
		   		          << std::endl;
		   	}
		   	else if (option == "-d") {
		   		std::cout << "Decompressing...\n";
		   		ofstream decodedFile;
		   		decompress(iFile, decodedFile, path_to_file);
		   		decodedFile.close();
		   		std::cout << "Finished.\n";

		   		std::cout << "Decompressed file path: "
		   				  << getDecompressedFilePath(path_to_file) << std::endl; 
		   	}
		   	else {
		   		std::cout << "Invalid option.\n";
				std::cout << "Try 'srar --help' for more information.\n";
		   	}
		   	iFile.close();
		}
	   	else 
	   		std::cout << "Error in opening specified file.\n";
	}
	else if (argc == 2 && option == "--help") 
		help();

	else {
		std::cout << "Invalid option.\n";
		std::cout << "Try 'srar --help' for more information.\n";
	}
	return EXIT_SUCCESS;
}

void help()
{
	std::cout << "Author: Harry Nguyen.\n";
	std::cout << "Version: SRAR v1.0\n";
	std::cout << "Hello from 2018.\n\n";
	std::cout << "USAGE: \n";
	std::cout << "For compression: srar -c <path_to_file>\n";
	std::cout << "For decompression: srar -d <path_to_file>\n";
	std::cout << "For showing this messsage: srar --help\n";
}

std::string getCompressedFilePath(const std::string& path_to_file, 
								  const std::string& extension)
{
	return hfile::getParentDicrectory(path_to_file) +
		   hfile::getFileName(path_to_file) +
		   hfile::getSourceFileExtension(path_to_file) +
		   extension;
}

std::string getDecompressedFilePath(const std::string& path_to_file)
{
	return hfile::getParentDicrectory(path_to_file) + "d_" +
	 	   hfile::getFileName(path_to_file) +
	 	   hfile::getOriginalFileExtension(path_to_file);
} 