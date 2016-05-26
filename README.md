# DISCOS<br />
File system implementation on bare metal x86 Hardwate<br />
To run our DISCOS implementation:<br /> 
  ● Untar the directory<br /> 
  ● If running on 32 bit system:<br />
		   		○ Open the Makefile and comment the line 'AS = 32'<br />
		    	○ If running on a 64 bit system, leave the line intact.<br />
  ● Open the c file play_fs.c and add the following file system functions in any order necessary:<br />
		    	○ rd_open<br />
		    	○ rd_close<br />
		    	○ rd_read<br />
		    	○ rd_write<br />
		    	○ rd_lseek<br />
		    	○ rd_unlink<br />
		    	○ rd_readdir<br />
  ● Run make to see the result on the QEMU window.<br />
 
 
The program working is as follows:<br />
1. A 2MB space is allocated from the system memory map and is assigned as the initial 
location of the file system space.<br />
2. The superblock, inodes and block bitmaps are initialized.<br />
3. Control is transferred over to the filesystem_begin() function where all the file operations 
are done.<br />
4. The rd_mkdir and rd_creat functions assign blocks for the files and then assign inodes to 
them. The information about whether inodes are free or not are stored in bytes 5­133 of 
the file system.<br /> 
5. The Open function assigns a file descriptor to a file and initializes the offset and inode 
values in the code space.<br /> 
6. The Read and Write functions read and write data from and to the locations specified by 
offset from the initial address specified in the location field of the corresponding inodes.<br />
7. The unlink function resets the value in the inode of the corresponding file and the 
corresponding file descriptor entries. It is also removed from the directory entry in the 
corresponding block of the directory. This way it does not show up when we do a 
read_dir.<br />
8. The read_dir is similar to the read function except that it reads the contents of the 
directory one by one by using the offset field in the file descriptor structure.<br />
9. The lseek function operates on files and changes the value of offset in a open file’s file 
descriptor structure.<br />
10. The close function closes a file and clears it’s file descriptor values. Only after a file is 
closed that it can be unlinked.<br />
