#pragma once
#include "MinHeap.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <vector>
#include <bitset>

class fileinfo {

public :

	int id;
	int size;
	char chars[256];
	int freq[256];
	int bits_num_last_byte;
	char fileName[500];
	int nameSize;
};



class Huffman
{
public:
	Huffman();
	void init();
	void save();


	void compress(std::string path);
	void decompress(std::string path);


private:

	int size;
	std::vector<fileinfo> filesData;


	std::map<char, int> frequencies;
	std::vector<char> buff;
	std::map<char, std::string> encode;
	HeapNode* root;
	MinHeap* heap;	


	void buildEncodeMap(HeapNode* root, char buffer[], int count);
	void build_Frequencies(std::string path);
	void build_Frequencies(char chars[],int freq[],int size);
	void buildTree();
	void decode(/*HeapNode* root,*/std::string code/*,int count*/, std::vector<char> &output);

	fileinfo getMetaData(int queryId);

};

