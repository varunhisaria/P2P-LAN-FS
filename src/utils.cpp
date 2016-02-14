#include <iostream>
#include <dirent.h>

#include "utils.h"

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

