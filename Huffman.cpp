#include "Huffman.h"

#define MAX_TREE_HT 1000

std::string path2 = "C:\\Users\\sameh\\Documents\\Visual Studio 2017\\Projects\\Huffman\\Huffman\\compressed.bin";

Huffman::Huffman()

{
	//init();

}

void Huffman::init()
{
	// read all the meta data of all files into memory for easier lookup (rather than reading it at every compress procedure)
	std::ifstream file("archive.dat");

	if (file) {

		 
		file.read((char*)&size, sizeof(size));
		
			fileinfo * arr = new fileinfo[size];

			file.read((char*)&arr[0], sizeof(fileinfo)*(this->size));

			for (int i = 0; i < size; i++) {
				this->filesData.push_back(arr[i]);
				
				

			}

		file.close();
	
	}

	else {

		file.close();
		std::ofstream output("archive.dat",std::ios::binary);
		
		this->size = 0;

		output.write((char*)&size, sizeof(size));

		output.close();

	}




}

void Huffman::save()
{

	// save the contents of the files metadata in memory into the archive file

	fileinfo * arr = new fileinfo[size];
	int i = 0;
	for (fileinfo data : filesData) {
		arr[i] = data;
		i++;
	}



	std::ofstream output("archive.dat", std::ios::binary);



	output.write((char*)&(this->size), sizeof(this->size));
	output.write((char*)&arr[0], sizeof(fileinfo) * this->size);

	output.close();


}

void Huffman::buildEncodeMap(HeapNode* root, char buffer[], int count)
{
	if (root->left) {
		buffer[count]='0';
		buildEncodeMap(root->left, buffer, count + 1);
	}


	if (root->right) {
		buffer[count] = '1';

		buildEncodeMap(root->right, buffer, count + 1);
	}

	if (root->isLeaf()) {
	

		std::string s(&buffer[0], &buffer[count]);
		this->encode.insert({ root->c, s });
		
	}



}

void Huffman::buildTree()
{

		this->heap = new MinHeap(this->frequencies); 


		this->root= heap->buildTree();


		char charBuffer[MAX_TREE_HT];
		buildEncodeMap(this->root, charBuffer, 0);


}

fileinfo Huffman::getMetaData(int queryId)
{
	for (fileinfo fileinfo : filesData) {

		if (fileinfo.id == queryId) {
			return fileinfo;
		}
	}
}

void Huffman::compress(std::string path)
{
	//get file name
	std::string filename = path;


	const size_t last_slash_idx = filename.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filename.erase(0, last_slash_idx + 1);
	}


	/*const size_t period_idx = filename.rfind('.');
	if (std::string::npos != period_idx)
	{
		filename.erase(period_idx);
	}*/

	
	
	//count the frequencies from the file and build a map from it
	build_Frequencies(path);

	//build huffman tree and encoding map 
	buildTree();


	//create the meta data for the file and add it to memory to be saved later to the archive file

	fileinfo metaData;
	metaData.id = this->size;
	metaData.size = this->frequencies.size();
	int i = 0;


	for (auto itr = frequencies.begin(); itr != frequencies.end(); ++itr) {
		metaData.chars[i] = itr->first;
		metaData.freq[i] = itr->second;
		i++;
	}

	//get the size of code in last byte 

	std::string str = "";
	for (i = 0; i < buff.size(); i++) {
		str += encode[buff[i]];
	}
	
	metaData.bits_num_last_byte = str.size() % 8;
	for (i = 0; i < path.size(); i++){
		metaData.fileName[i] = path[i];
	}
	metaData.nameSize = path.size();

	
	this->filesData.push_back(metaData);
	this->size++;


	//debug begins
	/*for (auto itr = encode.begin(); itr != encode.end(); ++itr) {
		std::cout << itr->first
			<< '\t' << itr->second << '\n';
	}*/

	
	int  j = 0;
	i = 0;

	/*for (i = 0; i < buff.size(); i++) 
		std::cout << buff[i];*/

	/*std::cout << "\nencodes:\n";

	for (i = 0; i < buff.size(); i++) 
		std::cout << encode[this->buff[i]] <<"  ";*/

	//debug ends

	/*std::cout << "\n" << "output:" << "\n";*/



	

	

	std::ofstream file(filename+".bin", std::ios::binary);

	
	//stack the encoded bit stream into multiples of bytes

	std::string fullStr = "";


	for (i = 0; i < buff.size(); i++)           //put all codes in one string
		fullStr += encode[this->buff[i]];
	

	 std::vector<unsigned char> outputBuffer;      

	for (i = 0; i < fullStr.length(); i += 8) {
		unsigned char byte = 0;
		std::string str8 = "";
		if (i + 8 < fullStr.length()) {

			str8 = fullStr.substr(i, i + 8);
		}
		else {
			str8 = fullStr.substr(i, fullStr.size());
		}

		for (unsigned b = 0; b != 8; ++b) {
			if (b < str8.length()) {
				byte |= (str8[b] & 1) << b;
			}
			else
				byte |= 1 << b;
		}
		outputBuffer.push_back(byte);
		
	}

	
	//write the buffer to output file

	file.write((char*)&metaData.id, sizeof(int));
	file.write((const char*)&outputBuffer[0], outputBuffer.size());


	file.close();



	//clear the data for recycling (reusing) purposes since its object members and later on further compress prodecures might  be called

	this->encode.clear();
	this->buff.clear();
	this->frequencies.clear();


}

void Huffman::decompress(std::string path)
{

	//read the file data (file Id then the rest of the data)

	std::ifstream input;
	input.open(path, std::ios::binary);

	// read id

	int id;
	input.read((char*)&id, sizeof(id));

	// read the rest

	std::istreambuf_iterator<char> begin = input;
	std::vector<char> inputBuffer(begin, std::istreambuf_iterator<char>());


	input.close();


	//get the associated metainfo from the archive file
	fileinfo info = getMetaData(id);
	int num_lastbyte = info.bits_num_last_byte;


	build_Frequencies(info.chars, info.freq, info.size);

	buildTree();


	//reverse the bits read from the file
	int  k = 0, i = 0;

	std::bitset<8> setByte;
	std::string concat = "";
	std::string bitStr = "";
	std::string reverse = "";



	for (i = 0; i < inputBuffer.size(); i++) {

		setByte = inputBuffer[i];
		bitStr = setByte.to_string();
		reverse = "";

		if (i == (inputBuffer.size() - 1)) {
			for (k = 7; k >= 8 - num_lastbyte; k--) {
				reverse += bitStr[k];

			}

		}
		else {


			for (k = 7; k >= 0; k--) {
				reverse += bitStr[k];

			}

		}
		//std::cout << reverse << "  ";

		concat += reverse;

	}

	//decode the bit stream
	std::vector<char> outputBuffer;
	decode(/*this->root,*/ concat/*, 0*/, outputBuffer);




	//write to the file

	std::string name = "";
	for (i = 0; i < info.nameSize; i++) {
		name += info.fileName[i];
	}

	std::ofstream output("out\\" + name,std::ios::binary);
	

	char * arr = outputBuffer.data();
	output.write((char*)arr, outputBuffer.size());


	output.close();




	this->encode.clear();
	this->buff.clear();
	this->frequencies.clear();

}

void Huffman::decode(/*HeapNode* root,*/std::string code ,/*int count,*/  std::vector<char> &output)
{

		/*if (root->isLeaf()) {
			output.push_back(root->c);
			std::cout << root->c;
			decode(this->root, code, count, output);
		}

		char c = code[count];
		if (c == '0') {

			if (root->left)
				decode(root->left, code, count + 1, output);


		}
		else if(c =='1'){

			if (root->right)
				decode(root->right, code, count + 1, output);

		}
*/
	HeapNode* node = this->root;
	char c;
	int i = 0;
	while (i <= code.size()) {

		if (node->isLeaf()) {
			output.push_back(node->c);
			node = this->root;
		}

		else {

			c = code[i];

			if (c == '0') {

				if (node->left)

					node = node->left;
			}

			else if (c == '1') {
				if (node->right)

					node = node->right;
			}
			i++;
		}
		if (i == code.size() - 2) {
			std::cout << "";
		}

	}
	
}

void Huffman::build_Frequencies(std::string path)
{

	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);

	std::istreambuf_iterator<char> begin = file;

	std::vector<char> buffer(begin, std::istreambuf_iterator<char>());

	this->buff = buffer;



	for (char c : buffer) {
		if (this->frequencies.find(c) != this->frequencies.end()) {
			this->frequencies[c]++;
		}
		else {
			this->frequencies.insert({ c,1 });
		}
	}

	file.close();

 


}

void Huffman::build_Frequencies(char chars[], int freq[],int size)
{

	for (int i = 0; i < size; i++) {

		frequencies.insert({ chars[i],freq[i] });

	}
	

}



