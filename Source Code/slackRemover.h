#ifndef SLACKREMOVER_H
#define SLACKREMOVER_H


#pragma comment(lib, "Shlwapi.lib")

#include <tchar.h>
#include <strsafe.h>
#include <windows.h> 
#include <string> 
#include <iostream> 
#include <fstream> 
#include <winioctl.h>
#include <stdio.h>
#include "Shlwapi.h"
#include <iomanip>
#include <Ntddvol.h>
#include <Pathcch.h>
#include "boost/program_options.hpp"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp> 



using namespace std;

typedef struct fInfo{
	HANDLE hanFile;
	HANDLE tempHan;
	LPCWSTR fileName;
	LARGE_INTEGER fileSize;
	wstring filePath;
	NTFS_VOLUME_DATA_BUFFER volBufData;
	DWORD slackSpace;
	DWORD bytePerSec;
	LPCWSTR Volpath;
} fileStat;


class slackInfo{

	private:
		void allDirFiles(const string& tDir);
		fileStat fileCreate(const string& targetFile);
		BOOL ClearSlack( fileStat info);
		DWORD slackSpace (fileStat info );
		wstring strChange(const string& s);

	public:
		void allDirFilesdum(const string tDir);
		fileStat fileCreatedum(const string targetFile);
		void volumeInfo (fileStat inf );
		void ClearSlackdum (fileStat in);




};



#endif