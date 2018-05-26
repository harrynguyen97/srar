#include "hfile.hpp"

std::string 
hfile::getFileName(const std::string& path_to_file)
{
	std::size_t start = 0;

	/* In case there is no parent directory at all */
	if (path_to_file.find_last_of("/") != std::string::npos) 
		start = path_to_file.find_last_of("/") + 1;
	else 
		start = 0;
	
	std::size_t end = path_to_file.find_first_of(".", start);

	std::string file_name = path_to_file.substr(start, end - start);
	
	return file_name;
}

std::string 
hfile::getParentDicrectory(const std::string& path_to_file) 
{
	std::size_t end = 0;

	/* In case there is no parent directory at all */
	if(path_to_file.find_last_of("/") != std::string::npos)
		end = path_to_file.find_last_of("/") + 1;
	else 
		end = 0;

	std::string parent_directory = path_to_file.substr(0, end);
	
	return parent_directory;
}

std::string
hfile::getSourceFileExtension(const std::string& path_to_file) 
{
	std::size_t start = path_to_file.find_last_of(".");
	std::size_t end = path_to_file.size();

	std::string source_file_extension = path_to_file.substr(start, end - start);

	return source_file_extension;
}

std::string 
hfile::getOriginalFileExtension(const std::string& path_to_file) 
{
	std::size_t endOfPath = 0;

	/* In case there is no parent directory at all */
	if (path_to_file.find_last_of("/") != std::string::npos) 
		endOfPath = path_to_file.find_last_of("/");
	else
		endOfPath = 0;
	
	// find the distance between 2 . in path
	// example: test.txt.har -> 4 (.txt)
	std::size_t start = path_to_file.find_first_of(".", endOfPath);

	// get next position of "." 
	std::size_t end = path_to_file.find_first_of(".", start + 1);
	
	std::string original_file_extension = path_to_file.substr(start, end - start);
	return original_file_extension;
}

std::string 
hfile::getSrarExtension(const std::string& path_to_file)
{
	std::size_t start = path_to_file.find_last_of(".");
	std::size_t end = path_to_file.size();

	std::string srarExtension = path_to_file.substr(start, end - start);

	return srarExtension;
}

void compress(const std::ifstream& inFile, 
			  std::ofstream& outHeaderFile, std::ofstream& outDataFile, 
			  const std::string& source_file_path, 
			  const std::string& dest_file_path, 
			  const std::string& extension) 
{
	/* Read whole string in inFile 
	*  and store to inDoc
	*/
	std::stringstream buf;
	buf << inFile.rdbuf();
 	std::string inDoc = buf.str();

 	/* Read more on Huffman coding for more info */
 	HZip haz;
	frmap_t freqMap = haz.buildFrequencyTable(inDoc);
	NODE* root = haz.buildEncodingTree(freqMap);
	hmap_t encodedMap = haz.getEncodedMap(root);

	// data vector contains header and actual compressed data
	std::vector<std::string> data;

	haz.encodeData(inDoc, freqMap, encodedMap, data);

	/* Construct the path of header file
	*  header_file_path = dest_dir_path + "h@"+ source_filename + 
	*                     source_file_extension + huffman_extension
	*/
	std::string header_file_path = hfile::getParentDicrectory(dest_file_path) + "h@" +
								   hfile::getFileName(source_file_path) + 
								   hfile::getSourceFileExtension(source_file_path) +
								   extension;
	
	/* Write to header file */
	outHeaderFile.open(header_file_path, std::ios::binary);
	outHeaderFile << data[HEADER];

	/* Construct the path of actual compressed data file
	*  similar to header_file_path, except for "d@" + filename instead of "h@"
	*/
	std::string data_file_path = hfile::getParentDicrectory(dest_file_path) + "d@" +
								 hfile::getFileName(source_file_path) + 
								 hfile::getSourceFileExtension(source_file_path) +
								 extension;

	/* Write to actual compressed file */
	outDataFile.open(data_file_path, std::ios::binary);
	outDataFile << data[ACTUAL_COMPRESSED_DATA];
}

void 
decompress(const std::ifstream& inFile, std::ofstream& outFile, 
		   const std::string& source_file_path,
		   const std::string& dest_file_path)
{
	/* Read whole string in inFile 
	*  and store to inDoc
	*/
	std::stringstream buf;
	buf << inFile.rdbuf();
 	std::string inDoc = buf.str();

 	/* Read more on Huffman coding decompression for more info */
 	HZip haz;
	frmap_t freqTable = haz.headerProcessing(inDoc);
	NODE* root = haz.buildEncodingTree(freqTable);

	std::string decompressedDoc;
	haz.decodeData(inDoc, root, decompressedDoc);

	// Free allocated root
	delete root;
	
	/* Construct the path of decompressed file
	*  decompressed_file_path = dest_dir_path + <filename> + source_originalfile_extension
	*/
	std::string decompressed_file_path = hfile::getParentDicrectory(dest_file_path) + 
										 hfile::getFileName(source_file_path) + 
										 hfile::getOriginalFileExtension(source_file_path);

	/* Write to outFile */
	outFile.open(decompressed_file_path, std::ios::binary);
	outFile << decompressedDoc;
}

void 
joinFile(const std::ifstream& inFile_1, const std::ifstream& inFile_2, 
		 std::ofstream& outFile, 
		 const std::string& source_file_path,
		 const std::string& dest_file_path)
{
	/* Read whole inFile_1 and store to string inDoc_1 */
	std::stringstream buf1;
	buf1 << inFile_1.rdbuf();
 	std::string inDoc_1 = buf1.str();

 	/* Read whole inFile_2 and store to string inDoc_2 */
 	std::stringstream buf2;
	buf2 << inFile_2.rdbuf();
 	std::string inDoc_2 = buf2.str();

 	// Join 2 strings
 	std::string outDoc = inDoc_1 + inDoc_2;

 	// Get raw file name (still contains "h@" and "d@")
 	std::string source_file_name = hfile::getFileName(source_file_path);

 	/* Remove "h@" or "d@" for getting actual file name */
 	std::size_t start = source_file_name.find_first_of("@") + 1;
 	std::size_t end = source_file_name.size();
 	std::string joined_file_name = source_file_name.substr(start, end - start);
 
 	/* joined_file_path = dest_dir + source_file_name + 
 	*					  original_source_file_extension + srar_extension 
 	*/
 	std::string joined_file_path = hfile::getParentDicrectory(dest_file_path) + 
 								   joined_file_name + 
 								   hfile::getOriginalFileExtension(source_file_path) +
 								   hfile::getSrarExtension(source_file_path);

 	/* Write joined string to dest_file_path */
 	outFile.open(joined_file_path, std::ios::binary);
	outFile << outDoc;
}