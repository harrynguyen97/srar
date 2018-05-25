#ifndef _HFILE_HPP_
#define _HFILE_HPP_

#include "hzip.hpp"

#include <fstream>
#include <sstream>
#include <string>

/* Namespace for file extensions */
namespace h_extension {
	const std::string hzip = ".hzip";
	const std::string har = ".har";
	const std::string tom = ".tom";
	const std::string peter = ".peter";
	const std::string harry = ".harry";
	const std::string ngochoa = ".ngochoa";
}

/*
*  Namespace for path manipulation
*  ----------------------------------------
*  Can be used for constructing the path of 
*  compressed and decompressed file
*/
namespace hfile {
	std::string getFileName(const std::string& path_to_file);
	std::string getParentDicrectory(const std::string& path_to_file);

	/* Get the extension of file need to be compressed
	*  Example: raw file: test.txt -> .txt
	*  --------------------------------------------------
	*  Used for constructing the path to compressed file
	*/
	std::string getSourceFileExtension(const std::string& path_to_file);

	/* Get the extension of original file 
	*  Example: compressed file: test.txt.har -> txt
	*  ---------------------------------------------------
	* Used for constructing the path to decompressed file
	*/ 
	std::string getOriginalFileExtension(const std::string& path_to_file);
}

/* Compress inFile and write to outFile
*  ------------------------------------------------------------------
*  path_to_file is used for constructing the path to compressed file
*/
void compress(std::ifstream& inFile, std::ofstream& outFile, 
			  const std::string& path_to_file, const std::string extension);

/* Decompress inFile and write to outFile 
*  -------------------------------------------------------------------
*  path_to_file is used for constructing the path to decompressed file
*/
void decompress(std::ifstream& inFile, std::ofstream& outFile, 
				const std::string& path_to_file);

#endif