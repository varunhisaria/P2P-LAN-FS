#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include <openssl/md5.h>

using namespace std;

typedef struct{
	long long size;
	string file_name;
	string file_hash;
}META;

int main(int argc, char* argv[]){
	// Variables
	int SIZE = 256;
	char buff[SIZE];
	struct stat results;
	string in,out, metadata;
	streampos size, pos=0;
	long parts=0, counter=0;
	
	
	// Check for cli args
	// If first argument : set size to variable+
	/*
	if(argc == 2){
		SIZE = argv[1];
	}*/
	
	

	cout<<"Enter file name :: ";
	cin>>in;
	char* in_file = new char[in.length()+1];
	strcpy(in_file, in.c_str());	

	// Get size of file
	if(stat(in_file, &results)==0){
		size = results.st_size;
		cout<<"File size :: "<<size<<endl;
	}


	//Create metadata file
	metadata = in+".meta"
	char* metadata_file = new char(metadata.length());
	strcpy(metadata_file, metadata.c_str());
	ofstream out_file_pointer(metadata_file);

	//Create structure to hold metadata
	META m;
	m.size = size;
	m.file_name = in;
	m.file_hash = 



	// Calculate part size
	parts = size/SIZE;
	if(size % SIZE)
		parts++;

	// File Pointers
	ifstream in_file_pointer(in_file, ios::in | ios::binary);
	
	// Break file into parts
	while(parts>0){
		// Initialise output file pointer
		out = in + "_" + to_string(counter);
		char* out_file = new char[out.length()+1];
		strcpy(out_file, out.c_str());
		cout<<"Output filename :: "<<out<<endl;
		ofstream out_file_pointer(out_file, ios::out | ios::binary);

		// Read data into buffer
		in_file_pointer.seekg(pos);
		in_file_pointer.read(buff, SIZE);
		out_file_pointer.write(buff, SIZE);

		// Update location from where to read next
		pos+=SIZE;
		// Update part number
		parts--;
		counter++;
	}
}