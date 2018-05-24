/*
* Author: Harry Nguyen
* Date: 23/05/2018
*/ 

#ifndef _HZIP_HPP
#define _HZIP_HPP

#include <string>
#include <vector>
#include <map>
#include <queue>

// Data type for a Huffman char
typedef unsigned short hchar_t; 

// Data type for frequency 
typedef unsigned int hfreq_t;

// Data type for frequency table
typedef std::map<hchar_t, hfreq_t> frmap_t; 

// Data type for huffman code map
typedef std::map<hchar_t, std::string> hmap_t; 

// Data type for a bit
typedef char bit_t;

const hchar_t PSEUDO_EOF = 256; // A virtual end of file 
const hchar_t NOT_A_CHAR = 257; // Write to internal nodes
const bit_t MAX_NUMBER_OF_BITS = 8; 

/* Convert string of bits ( "0" or "1") 
* to real byte 
* Return converted bytes in the form of strings
*/
std::string stobyte(std::string& sBits);

/*
* A Huffman node contains a character, its frequency
* and 2 children node
*/
struct NODE
{
	hchar_t _character;
	hfreq_t _freq;

	NODE* _lChild;
	NODE* _rChild;

	NODE(hchar_t character, hfreq_t freq):
		_character(character), _freq(freq),
		_lChild(nullptr), _rChild(nullptr) {}

	~NODE() {
		delete _lChild;
		delete _rChild;
	}
};

// ADT to implement min heap
struct Comparator
{
	bool operator()(NODE* lChild, NODE* rChild) 
	{
		return lChild->_freq > rChild->_freq;
	}
};

class HZip
{
public:
	HZip();

	/*
	* Build a frequency table 
	* and return a frequency map
	*/
	frmap_t buildFrequencyTable(const std::string& input);

	/* 
	* Build a Huffman tree 
	* and return the root
	*/
	NODE* buildEncodingTree(frmap_t freqTable);

	/*
	* Get encoded map 
	* value is string of binary code corresponding to each char
	*/
	hmap_t getEncodedMap(NODE* encodingTree);
	
	/*
	* Read character from input string 
	* and write bytes to output for compression
	*/
	void encodeData(std::string& input, const frmap_t freqMap, 
					hmap_t encodedMap, std::string& output);

	/*
	* Construct frequency table from the header 
	* in the input string
	*/
	frmap_t headerProcessing(std::string& input);

	/*
	* Read bits by bits from the input string
	* and write character to the output.
	* For decompression
	*/
	void decodeData(std::string& input, NODE* encodingTree, std::string& output);

protected:
	/* Protected member */
	/*
	* Build huffman encoding mao recursively
	*/
	void buildEncodingMap(NODE* encodingTree, std::string sCode);

protected:
	hmap_t p_encodedMap;

private:
	std::priority_queue<NODE*, std::vector<NODE*>, Comparator> m_minHeap;
};

#endif