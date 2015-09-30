#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;


typedef struct{
	string filename;
	string hash;
	int block;
	time_t last_updated_on;
}METADATA;


METADATA* readMeta(string filename){
	METADATA* tmp = (METADATA*)malloc(sizeof(METADATA));
	ifstream in(filename.c_str(), ios::in|ios::binary);
	in.read((char*)tmp, sizeof(METADATA));
	in.close();
	return tmp;
}

void writeMeta(METADATA* tmp){
	string metaFile = tmp->filename+".metadata";
	ofstream out(metaFile.c_str(), ios::out|ios::binary);
	out.write((char*)tmp, sizeof(METADATA));
	out.close();
}

int main(){
	METADATA m;
	m.filename = "Struct";
	m.hash = "hashed value here";
	m.block = 0;
	m.last_updated_on = 0;

	writeMeta(&m);
	METADATA *p = readMeta("Struct.metadata");
	cout<<p->filename;
	cout<<p->hash;
}

























/*
class MetaData{
	public:
		string filename;
		string hash;
		int block_size;
		time_t last_update;

	
		MetaData();
		MetaData(string, string, time_t = 0, int = 0);
		void writeMetaData();
		void readMetaData(string file);
};

MetaData::MetaData():block_size(0), last_update(0){}
MetaData::MetaData(string file, string hash, time_t last_update, int block_size):filename(file), hash(hash), last_update(last_update), block_size(block_size){}

void MetaData::writeMetaData(){
	string metadata_file = this->filename+".metadata";
	ofstream out(metadata_file.c_str(), ios::out | ios::binary);
	out.write((char*)this, sizeof(MetaData));
	out.close();
}

void MetaData::readMetaData(string file){
	try{
		ifstream in(file.c_str(), ios::in|ios::binary);
		in.read((char*)this, sizeof(MetaData));
		in.close();
	}
	catch(...){
		cout<<"Error";
	}
	
}
int main(){
	MetaData m("Test","Hashed value of Test");
	m.writeMetaData();
	try{
		MetaData m2;
	m2.readMetaData("Test.metadata");
	cout<<m2.filename;
	}
	catch(...){
		cout<<"Error";
	}
}*/