#ifndef __UTILS_H_INCLUDED__
#define __UTILS_H_INCLUDED__

#include <vector>
#include "metadata.h"

using namespace std;

class Utils
{
	public:
	vector<string> getFilesInDirectory();
	pair<long long int,long long int> getSizeAndNoOfBlocks(string fileName);
	string getMD5(string text);
	vector<string> getMD5OfAllBlocks(string filename);
	string getMD5OfFile(string filename);
	void writeMetaData(string fileName);
	MetaData readMetaData(string fileName);
	void updateFilesList();
};

#endif
