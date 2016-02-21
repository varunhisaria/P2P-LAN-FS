#include <iostream>
#include <dirent.h>
#include <openssl/md5.h>
#include <fstream>
#include <vector>

#include "utils.h"
#include "metadata.h"
#include "constants.h"

using namespace std;

vector<string> Utils::getFilesInDirectory()
{
	DIR *dpdf;
	class dirent *epdf;
	vector<string> fileNames;
	dpdf = opendir("./");
	if (dpdf != NULL)
	{
		while (epdf = readdir(dpdf))
		{
   			if(epdf->d_name[0]!='.')
   			fileNames.push_back(epdf->d_name);
		}
	}
	closedir(dpdf);
	return fileNames;
}

pair<long long int,long long int> Utils::getSizeAndNoOfBlocks(string fileName)
{
    pair<long long int, long long int> pll;
	long long int parts,fileSize;

	ifstream infile(fileName.c_str(), ios::binary | ios::ate);

	if (infile.is_open())
	{
		fileSize=infile.tellg();
		parts=fileSize/BUFFER_SIZE;
		if(BUFFER_SIZE*parts<fileSize)
			parts++;
		infile.close();
    }

    pll = make_pair(fileSize,parts);

    return pll;
}

string Utils::getMD5(string text){
    string md5;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)text.c_str(), text.size(), result);

    char buf[MD5_DIGEST_LENGTH];
    for (int i=0;i<MD5_DIGEST_LENGTH;i++){
        sprintf(buf, "%02x", result[i]);
        md5.append( buf );
    }
    return md5;
}

vector<string> Utils::getMD5OfAllBlocks(string fileName)
{
    vector<string> hashes;
    char * buffer;
	int part,total_parts,size,size_to_write,size_left;

	//open file to be broken
	ifstream infile(fileName.c_str(), ios::binary);
	if (infile.is_open())
	{
        infile.seekg(0,ios::beg);

        pair<long long int, long long int> pll;
        pll = getSizeAndNoOfBlocks(fileName);
        size=pll.first;
        total_parts=pll.second;

		// allocate memory for file content
		buffer = new char [BUFFER_SIZE];

		size_left=size;
		part=1;
		while(part<=total_parts){
			if(part==total_parts)
				size_to_write=min(size_left,BUFFER_SIZE);
			else
				size_to_write=1024;

      		infile.read(buffer,size_to_write);



            hashes.push_back(getMD5(buffer));
            size_left-=size_to_write;
			part++;
		}
		// release dynamically-allocated memory
		delete[] buffer;

		//close infile
		infile.close();
	}
	return hashes;
}

/*Usage : md5(<filename>)*/
string Utils::getMD5OfFile(string filename){
    int BLOCK_SIZE=1024;
	unsigned char hash[MD5_DIGEST_LENGTH];
	MD5_CTX md5Context;
	int bytes;
	// Buffer to read data from file
	char data[BLOCK_SIZE];
	// Buffer to hold data while conversion of md5 to string
	char buff[2];
	//Final string
	string md5;
	//Assuming file exists. Error checking to be done
	FILE *in = fopen(filename.c_str(), "rb");
	MD5_Init(&md5Context);
	while((bytes = fread(data, 1, BLOCK_SIZE, in))!=0){
		//cout<<"Bytes read :: "<<bytes<<endl;
		MD5_Update(&md5Context, data, bytes);
	}
	MD5_Final(hash,&md5Context);
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
		sprintf(buff,"%02x",hash[i]);
		md5.append(buff);
	}
	return md5;
}

void Utils::writeMetaData(string fileName){
	//ifstream inFile(fileName, ios::in | ios::binary);

	MetaData metaData;
	metaData.fileName = fileName;

	pair<long long int, long long int> pll;
	pll = getSizeAndNoOfBlocks(fileName);
    metaData.fileSize=pll.first;
    metaData.noOfBlocks=pll.second;

    metaData.fileHash= getMD5OfFile(fileName);

    metaData.lastUpdated=time(0);

    metaData.blockHash=getMD5OfAllBlocks(fileName);

	string metaFileName = fileName+ META_EXTENSION;

	ofstream out(metaFileName.c_str());
	out<<metaData;
	out.close();
}

MetaData Utils::readMetaData(string fileName)
{
    MetaData metaData;
    string metaFileName = fileName+ META_EXTENSION;
    ifstream ifs(metaFileName.c_str());
	if(ifs >> metaData)
	{
		//no error
	}
	return metaData;
}

void Utils::updateFilesList()
{
    vector<string> fileNames;
    fileNames = getFilesInDirectory();
    for(int i=0; i<fileNames.size(); i++)
    {

    }
}
