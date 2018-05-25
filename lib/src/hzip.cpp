#include "hzip.hpp"

/* Convert string of bits to bytes
*/
std::string stobyte(std::string& sBits)
{
	std::string sBytes;

	std::size_t numberOfBits = 0;
	bit_t bit = 0;

	/* Iterate through string of bits 
	* If there are not enough 8 bits (to construct 1 byte) 
	* when reaching the end of string bits then
	* 0 will be filled. That's the purpose of "+ numberOfBits"
	*/
	for(std::size_t i = 0; i < sBits.size() + numberOfBits; ++i) {

		// Convert character in form of binary to real bits.
		(sBits[i] == '0') ? (bit = 0) : (bit = 1);

		// Initialise byteChar once
		static char byteChar = 0;

		// then get 1 bit
		byteChar = (byteChar << 1) | bit;

		++numberOfBits;

		// If reaching 8 bits (1 byte)
		if (numberOfBits == MAX_NUMBER_OF_BITS) {
			// then add that byte to the string
			sBytes += byteChar;

			// and reset byteChar and numberOfBits for the next iteration
			byteChar = 0;
			numberOfBits = 0;
		}
	}

	return sBytes;
}


HZip::HZip()
{}

frmap_t
HZip::buildFrequencyTable(const std::string& input)
{
	frmap_t freqTable;

   	for (std::size_t i = 0; i < input.size(); ++i)
   		++freqTable[hchar_t(input[i])]; // cast input[i] from char to hchar_t 

   	// push a PSEDO_EOF to frequency table with freq = 1
   	freqTable[PSEUDO_EOF] = 1;

   	return freqTable;
}

NODE* 
HZip::buildEncodingTree(frmap_t freqTable)
{
	// push each char and its frequency to min heap
	for (auto it = freqTable.begin(); it != freqTable.end(); ++it)
		m_minHeap.push(new NODE(it->first, it->second));

	// push until there is just 1 node left (root)
	while (m_minHeap.size() > 1) {
		// get least frequency node
		NODE* lChild = m_minHeap.top();
		m_minHeap.pop();

		// get next least frequency node
		NODE* rChild = m_minHeap.top();
		m_minHeap.pop();

		/*
		* Make a parent node with
		* key is a char or a NOT_A_CHAR which indicates an internal node
		* value is the sum of 2 least frequency
		*/
		NODE* parent = new NODE(NOT_A_CHAR, lChild->_freq + rChild->_freq);

		// Link to its children
		parent->_lChild = lChild;
		parent->_rChild = rChild;

		m_minHeap.push(parent);
	}

	// Top of min heap is the root of built tree
	return m_minHeap.top();
}

/*
* Traverse binary tree recursively
* and push char and its huffman code to the map
*/
void 
HZip::buildEncodingMap(NODE* encodingTree, std::string sCode)
{
	if (!encodingTree) 
		return ;

	// If leaf node
	if (encodingTree->_character != NOT_A_CHAR) 
		p_encodedMap.insert(std::pair<hchar_t, std::string> (encodingTree->_character, sCode));

	HZip::buildEncodingMap(encodingTree->_lChild, sCode + "0");
	HZip::buildEncodingMap(encodingTree->_rChild, sCode + "1");
}

/*
* Get Huffman coding Map
*/
hmap_t
HZip::getEncodedMap(NODE* encodingTree)
{
	HZip::buildEncodingMap(encodingTree, "");
	delete encodingTree;
	return p_encodedMap;
}

void
HZip::encodeData(std::string& input, const frmap_t freqMap, 
				 hmap_t encodedMap, std::string& output)
{
	/*
	* Construct the header 
	* Format: {,<hchart>$<hfreq>...}
	*/
	std::string header;
	header = "{";
	for (auto it = freqMap.begin(); it != freqMap.end(); ++it) 
		header += "," + std::to_string(it->first) + "$" + std::to_string(it->second);
	header += "}";

	/*
	* Reconstruct the document with string of '0' and '1'
	*/
	std::string sBits;
	for (std::size_t i = 0; i < input.size(); ++i) {
		sBits += encodedMap[hchar_t(input[i])];
	}
	// Put a PSEUDO_EOF at the end of the input string
	sBits += encodedMap[PSEUDO_EOF];

	// Output = header + byte of string bits from huffman code
	output = header + stobyte(sBits);
}

frmap_t
HZip::headerProcessing(std::string& input)
{
	frmap_t freqTable;

	/* Get header */
	std::size_t endOfHeader = input.find_first_of("}");
	std::string header = input.substr(0, endOfHeader + 1);
	
	/* Positions for constructing key/value from header */
	std::size_t start = header.find_first_of("{,") + 2;
	std::size_t end = start;

	while (start < header.size()) {
		end = header.find_first_of("$", start);
		// get character from header
		std::string character = header.substr(start, end - start); 
		hchar_t hCharacter = std::stoi(character); // convert string to integer
		start = end + 1;


		end = header.find_first_of(",}", start);
		// get frequency from header
		std::string freq = header.substr(start, end - start);
		hfreq_t hFreq = std::stoi(freq); // convert char(freq) to integer
		start = end + 1;

		// Reconstruct freqTable
		freqTable.insert(std::pair<hchar_t, hfreq_t> (hCharacter, hFreq));
	}

	return freqTable;
}

void 
HZip::decodeData(std::string& input, NODE* rootOfEncodedTree, std::string& output)
{
	// initialise current node 
	NODE* curNode = rootOfEncodedTree;

	// flag for break 2 loops
	bool flagBreak = false;

	/* Get position right after the header */
	std::size_t endOfHeader = input.find_first_of("}");
	std::size_t pos = endOfHeader + 1;

	while (pos < input.size()) {
		/* Read 1 bit at a time from input */
		for (bit_t j = MAX_NUMBER_OF_BITS - 1; j >= 0; --j) {
			// Get bit
			bit_t bit = (input[pos] >> j) & 0x01;

			/* If bit = 0 then go to the left child
			* else go to the right child
			*/
			(bit == 0) ? 
				(curNode = curNode->_lChild) : (curNode = curNode->_rChild);
			
			// Reach the end of input
			if (curNode->_character == PSEUDO_EOF) {
				// turn on flagBreak for breaking the while loop
				flagBreak = true;

				// break the for loop
				break;
			}

			// If reaching leaf node
			else if (curNode->_character != NOT_A_CHAR) {
				// print node's character and add to the output
				output += curNode->_character;

				// Scan from root again
				curNode = rootOfEncodedTree;
			}
		}

		// Used for break the outer loop (while loop)
		if (flagBreak)
			break;

		// next position of input string
		++pos;
	}
}

