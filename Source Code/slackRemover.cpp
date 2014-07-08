#include "slackRemover.h"


void slackInfo::allDirFilesdum(const string tDir){
	const string newtDir = tDir;
	allDirFiles(newtDir);
}

// good function to pthread
void slackInfo::allDirFiles(const string& tDir){
	
	
	//fileStat hfile;
	namespace fs = boost::filesystem;
	fs::path targetDir(tDir);

	
	
	fs::directory_iterator it(targetDir), eod; 


	/*** pass files into fileCreate or recurse back into folders***/
	
	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod)){

		if (is_regular_file(p)){
			
			cout<<"File directory: " << p << endl;
			string result = p.string();
			/*hfile = */BOOL results = ClearSlack(fileCreate(result));
			if(results == 0 ){
				cout<<"Could not clear slackspace."<<endl;
			}
			
		}

		else if (is_directory(p)){

			//put it back 
			string result = p.string();
			cout << "\n\nFolder: "<<result <<endl<< endl;
			allDirFiles(result);
		}
		//ADD ERROR HANDLER for none file or dir
		else
			cout<<p<<" is not a file or directory."<<endl;
	}
	
	/***************************************************************/
}


fileStat slackInfo::fileCreatedum(const string targetFile){
	const string targetFilez = targetFile;
	return fileCreate(targetFilez);
}

fileStat slackInfo::fileCreate(const string& targetFile){

	fileStat filez;

	/*********call fileStat struct and check if file exist************/

	fstream file(targetFile.c_str());
	if (!file){
		cout << "File does not exist" << endl;
	}

	/****************************************************************/

	/************convert string from comand line to LPCWSTR********/
	wstring stemp = strChange(targetFile);
	LPCWSTR result = stemp.c_str();
	
	/****************************************************************/

	/**************** Path of the file ******************************/
	wchar_t pathBuffer[MAX_PATH];
	HRESULT hr =::StringCchCopy(pathBuffer,
								_countof(pathBuffer),
								stemp.c_str());

	if (!::PathStripToRoot(pathBuffer)){
		cout << "error with PathStripToRoot" << endl;
	}
	wstring root = wstring(pathBuffer);

	

	//wcout << "This is the path " << root << endl;

	//input path string for CreateFile
	
	int wstringln =sizeof(&root);
	root[2] = '\0';
	wstring newroot = L"\\\\.\\"+root;
	LPCWSTR pathz = newroot.c_str();

	/****************************************************************/

	/**********CreateFile of user input file ************************/
	HANDLE hFile = INVALID_HANDLE_VALUE; 

	hFile = CreateFile(result,			
					   GENERIC_READ | GENERIC_WRITE |FILE_READ_ATTRIBUTES,	
					   FILE_SHARE_READ | FILE_SHARE_WRITE,
					   0,							
					   OPEN_EXISTING,					 
					   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,			
					   0);							

	if (hFile == INVALID_HANDLE_VALUE)  {
		cout << "File does not exist" << endl;
		CloseHandle(hFile);
		//system("pause");
		}

	//cout << "You opened/created the file succesfully: " << hFile << endl;

	/****************************************************************/

	/********************** Find the File size *********************/
	WIN32_FILE_ATTRIBUTE_DATA fdata;
	if(!GetFileAttributesEx(result,GetFileExInfoStandard,&fdata)){
		cout<< "Error with GetFileAtributesEx " <<endl;
	}
	LARGE_INTEGER fSize;
	fSize.HighPart = fdata.nFileSizeHigh;
	fSize.LowPart = fdata.nFileSizeLow;

	//cout<< "This is the GetFileAttributesEx file size: " <<fSize.QuadPart<<endl;

	/****************************************************************/

	/******* Find information on disk (SectorPerCluster etc) ********/
	DWORD dwSectPerClust,
		dwBytesPerSect,
		dwFreeClusters,
		dwTotalClusters;

	int diskClust = 0;
	diskClust = GetDiskFreeSpace(root.c_str(),
								 &dwSectPerClust,
								 &dwBytesPerSect,
								 &dwFreeClusters,
								 &dwTotalClusters);

	if (diskClust == 0){
		cout << "error in function diskInfo with GetDiskFreeSpace" << endl;
	}

	/*
	cout << "Sectors Per Cluster: " << dwSectPerClust << endl;
	cout << "Bytes Per Sector: " << dwBytesPerSect << endl;
	cout << "Free Clusters: " << dwFreeClusters << endl;
	cout << "Total Clusters: " << dwTotalClusters << endl<<endl<<endl;
	*/

	filez.volBufData.BytesPerCluster = dwBytesPerSect * dwSectPerClust;
	filez.volBufData.NumberSectors.QuadPart = dwTotalClusters;
	filez.volBufData.NumberSectors.QuadPart *= dwSectPerClust;
	

	/******************************************************************/



	/*********************** Find the slack Space **********************/

	// File size / cluster size(in butes) = # of clusters needed
// if (File size % cluster size !=0) then you need 1 more cluster
// # of clusters + 1 = total clusters used
	DWORD extraCluster;
	if (fSize.QuadPart % filez.volBufData.BytesPerCluster != 0){
		extraCluster = 1;
	}
	else{
		extraCluster = 0;
	}

	DWORD clustersNeed = (fSize.QuadPart / filez.volBufData.BytesPerCluster) + (extraCluster);
	/*cout << "This file size " << fSize.QuadPart<< " / Cluster size	" << filez.volBufData.BytesPerCluster 
		<< "= # of clusters needed: " << clustersNeed << endl;*/

	// slack space = (totals clusters * cluster size) - File size
	DWORD slackSpaces = clustersNeed * filez.volBufData.BytesPerCluster - fSize.QuadPart;
	/*cout << "The total slack space is clusters needed: " << clustersNeed << " * Cluster size: " << filez.volBufData.BytesPerCluster 
		<< " - file size: " << fSize.QuadPart << " = total slack space: " << slackSpaces << endl << endl << endl;*/
	/********************************************************************/

	/****** set temp file endpoint *******/
	HANDLE hFileOrg = INVALID_HANDLE_VALUE; 
	
	hFileOrg = CreateFile(result,			
					   GENERIC_READ | GENERIC_WRITE |FILE_READ_ATTRIBUTES,	
					   FILE_SHARE_READ | FILE_SHARE_WRITE,
					   0,							
					   OPEN_EXISTING,					 
					   FILE_ATTRIBUTE_NORMAL,			
					   0);							

	if (hFileOrg == INVALID_HANDLE_VALUE)  {
		cout << "File does not exist" << endl;
		//CloseHandle(hFileOrg);
		//system("pause");
		}

	/********************************************************************/

	filez.fileName = result;
	filez.tempHan = hFileOrg;
	filez.slackSpace = slackSpaces;
	filez.hanFile = hFile;
	filez.filePath = root;
	filez.fileSize = fSize;
	filez.Volpath = pathz;
	filez.bytePerSec = dwBytesPerSect;
	return filez;
}

/**** Convert string into wstring *****/
wstring slackInfo::strChange(const string& s)
{
	int len,slength;

	slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;

	return r;
}

void slackInfo::ClearSlackdum (fileStat in){
	ClearSlack(in);

}

BOOL slackInfo::ClearSlack( fileStat info) {

	
	cout<< "File size: " <<info.fileSize.QuadPart<<endl;
	cout<< "Amount of SlackSpace: "<<info.slackSpace<<endl;


	
	BOOL returnz = SetFilePointer(info.tempHan, info.fileSize.QuadPart,NULL,FILE_BEGIN);
	if(returnz ==0){
		cout<<"Error with SetFilePointer"<<endl;
	}

	DWORD dwBytesWritten;
	

	cout<<"Clearing File's slack space..."<<endl;

	/******************* Loop to write random 0s and 1s to the end of the file **********/
	for( int a = 2; a<6;a++){

		//Alternate 0s and 1s
		int b,c;
		b = 2;
		c = a%b;

		char * wfileBuff = new char[info.slackSpace];
		memset (wfileBuff,c,info.slackSpace);
		

		returnz = SetFilePointer(info.hanFile, info.fileSize.QuadPart,NULL,FILE_BEGIN);
		if(returnz ==0){
			cout<<"Error with SetFilePointer"<<endl;
			return 0;
		}
		
		/**** Lock file, Write data, Unlock file *******/
		if(LockFile(info.hanFile, returnz, 0, info.slackSpace, 0) != 0)

		returnz =WriteFile(info.hanFile, wfileBuff, info.slackSpace, &dwBytesWritten, NULL);
		if(returnz ==0){
			cout<<"There is an error with WriteFile"<<endl;
			cout<<"Error: "<<GetLastError()<<endl;
			return 0;
		}
		if(UnlockFile(info.hanFile, returnz, 0, info.slackSpace, 0) != 0);

		/***********************************************/

		//cout<<dwBytesWritten<<endl<<endl;
		//system("pause"); 


		//Cut out the extra data written from the file
		if(!SetEndOfFile(info.tempHan)){
			cout<<"Error seting the end of the file"<<endl;
			return 0;
		}
	
	}
	/****************************************************************************************/
	cout<<"Slackspace overwritten successfully.\n\n\n";

	return 1;
}


//function needs some more test(function is an extra feature and has nothing to do with removing slackspace)
void slackInfo::volumeInfo (fileStat inf ){

/******************* CreateFile for volume ***********************/
	/*
							***Note***
		The following two functions(CreateFile and DeviceIoControl) will
		not work unless the application is ran as admin. However, these
		two functions are not needed to write over file slackspace.
	*/


	HANDLE hDevice = INVALID_HANDLE_VALUE;  

	
	hDevice = CreateFile(inf.Volpath,         
						 GENERIC_READ,              
						 FILE_SHARE_READ | FILE_SHARE_WRITE,
						 NULL,             
						 OPEN_EXISTING,    
						 FILE_ATTRIBUTE_NORMAL,                
			   			 NULL);            

	if (hDevice == INVALID_HANDLE_VALUE){    // cannot open the drive
		//wcout << "error at hDevice with path: "<< endl;
		DWORD error = GetLastError();
		if (error == ERROR_ACCESS_DENIED){
			//cout << "error_access_denied" << endl;
		}
		//cout << error << endl;
		//system("pause");
	}
	/******************************************************************/

	/********** Get extent information on the volume **********************/
	VOLUME_DISK_EXTENTS volExtent;

	DWORD bytesReturned = 0;
	BOOL check = 0;

	check = DeviceIoControl(hDevice,
		IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
		NULL,
		0,
		&volExtent,
		sizeof(volExtent),
		&bytesReturned,
		NULL);
	if(check == 0){
		//cout<<"Error with DeviceIoControl with control code IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS"<<endl;
	}

	DWORD beginCluster = 0;
	DWORD beginRoot = 0;

	
	LONGLONG volumeSize = inf.volBufData.NumberSectors.QuadPart * inf.bytePerSec;
	LONGLONG volumeStartSector = volExtent.Extents[0].StartingOffset.QuadPart / inf.bytePerSec;

	STARTING_VCN_INPUT_BUFFER inputVcnBuff;
	RETRIEVAL_POINTERS_BUFFER rtPtrBuff;

	memset(&inputVcnBuff, 0 , sizeof(STARTING_VCN_INPUT_BUFFER));
	memset(&rtPtrBuff, 0 , sizeof(RETRIEVAL_POINTERS_BUFFER));

	cout<<"Volume Size: "<<volumeSize<<endl;
	cout<<"Volume start Sector: "<<volumeStartSector<<endl;


	/***********************************************************************************************************/


}