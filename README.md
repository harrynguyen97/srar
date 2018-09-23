# srar v1.1

This is a lossless compress/decompress application using Huffman Coding algorithm.

## Installation
- Clone this repository and make
```
git clone https://github.com/ngharry/srar.git
cd srar
make
```

## Usage
srar [OPTION] SOURCE ... DEST

where OPTION:
  * -c: compression
  * -j: join header and body
  * -d: decompression

  * --help: display usage

**NOTE**: The compressed file will be devided into 2 files: 1 header and 1 actual compressed content. You should join them when decompressing.

## TODO
- [ ] Update README.
- [ ] Update code to be "high cohesion - loose coupling".

## Bug Reporting 


