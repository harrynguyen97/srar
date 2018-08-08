**SRAR v1.0** 
------------------------------------------------------------------------
*This is an compress/decompress application using Huffman Coding Algorithm*

**Description:**
*The compressed file will be devided into 2 files: 1 header and 1 actual compressed content. You should join them when decompressing*


* **Installation:**
	* cd to the directory contains the project
	* type `make`

* **Usage:**
	* `srar [OPTION] SOURCE ... DEST`
	* OPTION:
		* -e: encryption
		* -j: joins header and body
		* -d: decryption

		* --help: display usage 