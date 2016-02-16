#include <iostream>
#include "utils.h"

using namespace std;

void getFilesInDirectoryTest()
{
	Utils obj;
	vector<string> res = obj.getFilesInDirectory();
	for(int  i =0;i<res.size();i++)
		cout<<res[i]<<endl;
}

int main()
{
	getFilesInDirectoryTest();
	return 0;	
}

