#include <iostream>
#include <cstdio>
#include <cstring>
#include <openssl/md5.h>

using namespace std;

/*Usage : md5(<filename>)*/
string md5(string filename, int BLOCK_SIZE=1024){
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
		cout<<"Bytes read :: "<<bytes<<endl;
		MD5_Update(&md5Context, data, bytes);
	}
	MD5_Final(hash,&md5Context);
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
		sprintf(buff,"%02x",hash[i]);
		md5.append(buff);
	}
	return md5;
}