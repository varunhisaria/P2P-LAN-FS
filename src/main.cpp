#include <iostream>
#include "utils.h"
#include "metadata.h"

using namespace std;

//test methods

void getFilesInDirectoryTest()
{
	Utils obj;
	vector<string> res = obj.getFilesInDirectory();
	for(int  i =0;i<res.size();i++)
		cout<<res[i]<<endl;
}

void getMD5Test()
{
	Utils obj;
	string res = obj.getMD5("hello");
	cout<<res<<endl;
}

void writeMetaDataTest()
{
    Utils obj;
    obj.writeMetaData("main.cpp");
}

void readMetaDataTest()
{
    Utils obj;
    MetaData metaData = obj.readMetaData("main.cpp");
    cout << metaData.fileName << '\n';
    cout << metaData.fileSize << '\n';
    cout << metaData.fileHash << '\n';
    cout << metaData.lastUpdated << '\n';
    cout << metaData.noOfBlocks << '\n';
    for(int i = 0; i<metaData.noOfBlocks; i++)
    {
        cout << metaData.blockHash[i];
        if(i!=metaData.noOfBlocks-1)
            cout<<'\n';
    }
}


//actual main methods
void addFile(string fileName)
{
    Utils utils;
    utils.writeMetaData(fileName);
    //update list file
}


int main()
{
	//getFilesInDirectoryTest();
	//getMD5Test();
	writeMetaDataTest();
	readMetaDataTest();
	return 0;
}

