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

void compress(std::ifstream& inFile, std::ofstream& outFile, 
			  const std::string& path_to_file, const std::string& extension) 
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

	std::string encodedDoc;
	haz.encodeData(inDoc, freqMap, encodedMap, encodedDoc);

	/* Construct the path of compressed file
	*  encoded_file_path = dir_path + filename + file_extension + huffman_extension
	*/
	std::string encoded_file_path = hfile::getParentDicrectory(path_to_file) + 
									hfile::getFileName(path_to_file) + 
									hfile::getSourceFileExtension(path_to_file) +
									extension;
	
	/* Write to outFile */
	outFile.open(encoded_file_path, std::ios::binary);
	outFile << encodedDoc;
}

void decompress(std::ifstream& inFile, std::ofstream& outFile, 
				const std::string& path_to_file)
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

	std::string decodedDoc;
	haz.decodeData(inDoc, root, decodedDoc);

	// Free allocated memory
	delete root;
	
	/* Construct the path of decompressed file
	*  decoded_file_path = dir_path + d_<filename> + originalfile_extension
	*/
	std::string decoded_file_path = hfile::getParentDicrectory(path_to_file) + 
									"d_" + 
									hfile::getFileName(path_to_file) + 
									hfile::getOriginalFileExtension(path_to_file);

	/* Write to outFile */
	outFile.open(decoded_file_path, std::ios::binary);
	outFile << decodedDoc;

}