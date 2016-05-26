#include "functions.h"

int rd_read(int fd, char *address, int num_bytes)
{
	unsigned long off;
	int size;
	int inode;
	int block_num;
	unsigned long add;
	char* PA;
	unsigned long i,j=0;
	if(file_des[fd].open)
	{
		
		if(num_bytes > inode_ptr[inode].size )
		{
			WriteCharacter("File Pointer Out of Scope");
			newline();
			return -1;
		}

		inode = file_des[fd].inode;
		off = file_des[fd].offset;
		size = inode_ptr[inode].size; 
		block_num = off/256;
		add = inode_ptr[inode].location[block_num] + (off%256);
		PA = add;
		
		for(i = 0 ; i < num_bytes ; i++)
		{
			if(((add+i)%256) != 0 || off==0)
			{
				address[i] = PA[j];		
			}
			else
			{
				add = inode_ptr[inode].location[block_num + 1];
				PA = add;
				j = 0;
				address[i] =  PA[j];
			}
			j++;
		}

	}
	else
	{
		WriteCharacter("File Not Open");
		newline();
		return -1;
	}

	file_des[fd].offset = num_bytes;

	return num_bytes;
}


int rd_write(int fd, char *address, int num_bytes)
{
	unsigned long off;
	int size;
	int inode;
	int block_num;
	unsigned long add;
	char* PA;
	unsigned long i,j=0;

	if(num_bytes > 2048)
	{
		WriteCharacter("Maximum File Size is 2048");
		newline();
		return -1;
	}
	
	if(file_des[fd].open)
	{
		inode = file_des[fd].inode;
		off = file_des[fd].offset; 
		block_num =  off/256;
		add = inode_ptr[inode].location[block_num] + (off%256);
		PA = add;


		
		for(i = 0 ; i < num_bytes ; i++)
		{
			if((((add+i)%256)!=0) || off==0)
			{
				PA[j] = address[i];

			}
			else
			{
				add = allocate_block(inode);
				if(add == -1)
				{
					WriteCharacter("No more memory available");
					newline();
					inode_ptr[inode].size+=i;
					inode_ptr[file_des[fd].parent_inode].size += i;
					return i;
				}
				
				PA = add;
				j = 0;
				PA[j] = address[i];
			}
			j++;

		}

	}
	else
	{
		WriteCharacter("File Not Open");
		newline();
		return -1;
	}

	file_des[fd].offset = num_bytes;
	inode_ptr[inode].size+=num_bytes;
	inode_ptr[file_des[fd].parent_inode].size += num_bytes;
	return num_bytes;
}


int rd_lseek(int fd, int offset)
{
	int inode = file_des[fd].inode;
	if(inode_ptr[inode].type == '0')
	{
		WriteCharacter("Cannot Seek a Directory");
		newline();
		return -1;
	}

	int size = inode_ptr[inode].size; 
	if(offset > size)
		 file_des[fd].offset = size;
	else
		file_des[fd].offset = offset;

	return 0;
}
