#ifndef _HFILE_HPP_
#define _HFILE_HPP_

#include "hzip.hpp"

#include <fstream>
#include <sstream>
#include <string>

const unsigned char HEADER = 0;
const unsigned char ACTUAL_COMPRESSED_DATA = 1;

/* Namespace for file extensions */
namespace h_extension {
	const std::string hzip = ".hzip";
	const std::string har = ".har";
	const std::string tom = ".tom";
	const std::string peter = ".peter";
	const std::string harry = ".harry";
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

	std::string getSrarExtension(const std::string& path_to_file);
}

/* Compress inFile and split into 2 files: 
*          the header and actual compressed file.
*  ---------------------------------------------------
*  dest_file_path is the path to the directory you want to write 
*  your compressed file to, no need to include file name.
*/
void compress(const std::ifstream& inFile, 
			  std::ofstream& outHeaderFile, std::ofstream& outDataFile, 
			  const std::string& source_file_path, 
			  const std::string& dest_file_path,  
			  const std::string& extension);


/* Join 2 files inFile_1 and inFile_2 and write to outFile.
*  -----------------------------------------------------------------------
*  dest_file_path must include file name you want to create after joining
*/
void joinFile(const std::ifstream& inFile_1, const std::ifstream& inFile_2, 
			  std::ofstream& outFile, 
			  const std::string& source_file_path,
			  const std::string& dest_file_path);


/* Decompress inFile and write to outFile 
*  -------------------------------------------------------------------
*  dest_file_path is the path to the directory you want to write 
*  your compressed file to, no need to include file name.
*/
void decompress(const std::ifstream& inFile, std::ofstream& outFile, 
				const std::string& source_file_path, 
				const std::string& dest_file_path);

#endif