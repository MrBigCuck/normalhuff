#include <stdio.h>
#include <stdlib.h>

#include "MinHeap.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <vector>
#include "Huffman.h"
#include <fstream>
#include <bitset>


std::string path = "hey.txt";
std::string pathbin = "foobar.bin";

//
//class fileinfo {
//
//public:
//	char c;
//	int size;
//
//	friend std::ostream & operator << (std::ostream &out, const fileinfo & obj)
//	{
//		out <<obj.c << obj.size;
//		return out;
//	}
//
//
//	friend std::istream & operator >> (std::istream &in, fileinfo &obj)
//	{
//		in >> obj.c;
//		in >> obj.size;
//		return in;
//	}
//
//
//};


int main() {

	/*fileinfo file1;
	fileinfo file2;
	fileinfo file3;
	
	file1.c = '1';
	file1.size = 1;

	file2.c = '2';
	file2.size = 2;

	file3.c = '3';
	file3.size = 3;


	int size = 2;

	std::ofstream outfile;
	outfile.open("archive.bin",std::ios::binary);

	if (!outfile.is_open()) {
		std::cout << "cant open outfile";
	}

	outfile.write((char*)&size, sizeof(size));
	outfile.write((char*)&file1,sizeof(file1));

	outfile.close();




	fileinfo file4;
	fileinfo file5;
	fileinfo file6;

	int size2;

	std::ifstream infile;
	infile.open("archive.bin",std::ios::binary);

	if (!infile.is_open()) {
		std::cout << "cant open infile";
	}

	infile.read((char*)&size2, sizeof(size2));

	infile.read((char*)&file4, sizeof(file4));


	infile.close();

	std::cout << "size" << size2;
	std::cout << "\n";

	std::cout << file4.c;
	std::cout << file4.size;
*/
	
	Huffman * algo = new Huffman();
	algo->init();
	
	/*algo->compress("hey.txt");
	algo->compress("man.txt");
	algo->compress("lol.txt");*/


	//algo->compress("no2.jpg");
	/*algo->decompress("no2.jpg.bin");*/

	algo->decompress("hey.txt.bin");
	algo->decompress("man.txt.bin");
	algo->decompress("lol.txt.bin");

	//algo->compress("Untitled.bmp");

	algo->save();
	
	
	//std::string path = "Untitled.jpg";
	//std::ifstream file;
	//file.open(path, std::ios::in | std::ios::binary);

	//std::istreambuf_iterator<char> begin = file;

	//std::vector<char> buffer(begin, std::istreambuf_iterator<char>());

	//for (char value : buffer) {
	//	std::cout << value;
	//}


	//file.close();




	//
	//std::ofstream output("out\\" + path /*std::ios::binary*/);


	//char * arr = buffer.data();
	//output.write((char*)arr, buffer.size());


	//output.close();



	getchar();
	return 0;

}