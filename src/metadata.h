#ifndef __METADATA_H_INCLUDED__
#define __METADATA_H_INCLUDED__

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class MetaData{
	public:
	string fileName;
	int fileSize;
	string fileHash;
	time_t lastUpdated;
	int noOfBlocks;
	vector<string> blockHash;

    // Insertion operator
	friend ostream& operator<<(ostream& os, const MetaData& s)
	{
		os << s.fileName << '\n';
		os << s.fileSize << '\n';
		os << s.fileHash << '\n';
		os << s.lastUpdated << '\n';
		os << s.noOfBlocks << '\n';
		for(int i = 0; i<s.noOfBlocks; i++)
		{
            os << s.blockHash[i];
            if(i!=s.noOfBlocks-1)
                cout<<'\n';
		}
		return os;
	}

	// Extraction operator
	friend istream& operator>>(istream& is, MetaData& s)
	{
		is >> s.fileName;
		is >> s.fileSize;
		is >> s.fileHash;
		is >> s.lastUpdated;
		is >> s.noOfBlocks;
		string temp;
		for(int i = 0; i<s.noOfBlocks; i++)
		{
            is >> temp;
            s.blockHash.push_back(temp);
		}
		return is;
	}
};

#endif


