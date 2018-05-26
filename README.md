**SRAR v1.0** 
------------------------------------------------------------------------
*This is an encrypt/descrypt application using Huffman Coding Algorithm*

**Description:**
*By using Huffman Coding Algorithm, the encrypted file will contains the header (which can be used for decryption) and the actual encrypted data. This application will export 2 files: 1 contains the header, and the other contains the encrypted data. Hackers will have to have both 2 files for decrypt your data. So of course, as long as you carefully seperate 2 files, your data is encrypted safely (at least I think).*


* **Installation:**
	* cd to the directory contains the project
	* type "make"

* **Usage:**
	* `srar [OPTION] SOURCE ... DEST`
	* OPTION:
		* -e: encryption
		* -j: joins header and body
		* -d: decryption

		* --help: display usage 