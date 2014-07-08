FileSlackSpace v1.0

	The objective of this tool is to zero out file slackspace. 

	What is file slackspace? 
	Slackspace is the unoccupied space on a disk. Files are organized into sectors 
        that make up a cluster where each sector has a set size, in bytes, where data 
        from a file is distributed contiguously until the sector is full. Once the 
	sector is full, the data moves onto the next one. Most of the time the file 
	size is not an exact multiple of a Sector, thus leaving empty space.

	Why is it important to deal with slackspace?
	Deleting a file is an often misunderstood concept. The data is not usually 
	deleted at all, but rather marked as free space for another file to write 
	over. This results in lingering data that the user thinks he or she deleted. 
	The data can then be retrieved by unauthorized users. This tool can eliminate 
	the risks of lingering data by writing meaningless data to a file's slackspace. 
	

	This tool was written in c++ using Microsoft Visual Studios 2012 on Windows 8.

How to Use:
	Upon installation, run the application through the command prompt as an administrator. 
	Make sure you navigate to the tool's directory. To run the tool simply type in 
	removeSlack.exe followed by one of the menu options bellow.

	Options:
 		-f [ --fileclean ] arg  Choose a file to clean its slack space
  		-d [ --driveclean ] arg Choose a drive to clean all of its files
  		-h [ --help ]           Displays help message with options
 
	For example you can type in "removeSlack.exe -h " or "removeSlack.exe
	--help" (without the quotes) in order to get the option menu to pop up.
	When using the fileclean or driveclean you must enter a valid directory
	after using a flag. For example "removeSlack.exe -d "C:\Users\Public\Documents" "
	(without outside the quotes). 

	Warnings: 
		-Sometimes files under 1KB might fail because they are usually written in
		the MFT(Master File Table).
		-Might not work on FAT32 
		-file names or folders with spaces will crash the program(using quotes
		for the argument will fixes this)
	
	Example use:
		Below is an example where removeSlack is on the desktop and you want to use
		it to clean a folder's file(s) on the desktop that is call 'Test.'

		C:\Users\Public\Desktop> removeSlack.exe -d "C:\Users\Public\Desktop\Test"


current functions:

	strChange()
		This function takes in an input string and converts it into a wstring.
	
	fileCreate()
		This is the core function that does all the workload such as creating file
		handles, getting disk and file information.

	allDirFiles()
		This function uses boost function in order to iterate through all files in
		a folder and call fileCreate for each individual file.

	ClearSlack()
		This function saves the current state of the file and then begins to resize
		the target file. After resizing the file to take up the full amount of space
		on the disk, it will use the old state of the file to trim all the data written.
		It will repeat this process a few times to make sure the old slackspace data is
		written over.
		
	main()
		Main function that uses boost parsing from the command line in order to 
		parse arguments according to given flags.
	

To Do:
	1. Implement compatibility with FAT32 file system.
	2. Add options to display file data or disk data.
	

Resources:
	The following to link and pdf have a great deal of information on how files are organized 
	on disk.
		http://www.wd-3.com/archive/luserland.htm
		http://grayscale-research.org/new/pdfs/NTFS%20forensics.pdf


Contact information:
	(name)Diego Urquiza
	(email)Diego_Urquiza@Mcafee.com
