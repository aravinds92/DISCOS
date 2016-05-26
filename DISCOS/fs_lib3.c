#include "functions.h"


int rd_unlink(char *pathname)
{

	int inode, parent;
	int size,loc,fd;
	int i = 0,k,m,a,n,j;

	char temp[14];
	char parent_name[14];


	if(strcmp(&parent_name[0],pathname) == 0)
	{
		WriteCharacter("Cannot unlink root");
		newline();
		return -1;
	}


	parent_name[0] = '/';
	parent_name[1] = '\0';

	while(1)
	{
		if(pathname[i] == '/')
		{
			k = i+1;
			m=0;
			while(pathname[k] != '\0' && pathname[k] != '/')
			{
				temp[m]=pathname[k];
				m++;
				k++;
			}
			temp[m] = '\0';
			
			if(pathname[k] == '\0')
			{			
				inode = get_inode_num(&temp[0],&parent_name[0]);
				
				if(inode == -1)
				{
					WriteCharacter("Directory not found");
					newline();
					clear(&temp[0]);
					clear(&parent_name[0]);
					return -1;	
				}
				break;
			}
			
			if(pathname[k] == '/')
			{
				parent = get_inode_num(&temp[0] , &parent_name[0]);
				if(parent == -1)
				{
					WriteCharacter("Directory not found");
					newline();
					clear(&temp[0]);
					clear(&parent_name[0]);
					return -1;
				}
				strcpy(&parent_name[0], &temp[0]);
			}
			i = k;
		}	
				
	}


	fd = find_fd_from_inode(inode);

	if((fd != -1) && (file_des[fd].open == 1))
	{
		WriteCharacter("Cannot unlink open file");
		newline();
		return -1;
	}

	size = inode_ptr[inode].size;
	if(size>0)
	{
		WriteCharacter("Cannot unlink Non-empty file");
		newline();
		return -1;	
	}

	loc = size/256;



	for(i=0;i<loc;i++)
	{
		free_block(inode_ptr[inode].location[loc]);
		
		if(a==-1)
		{
			WriteCharacter("Invalid inode number");
			newline();
			return -1;
		}

		inode_ptr[inode].location[loc] = 0;
	}

	inode_ptr[inode].type = 0;
	inode_ptr[inode].size = 0;

	free_inode(inode);


	for(i=0;i<14;i++)
	{
		temp[i] = '\0';
		parent_name[i] = '\0';
	}

	for(i=0;i<inode_ptr[parent].size/256;i++)
	{
		n = inode_ptr[parent].location[i];
		for(j=0;j<16;j++)
		{	
			if(DE_ptr[n+j].inode_num == inode)
			{
				DE_ptr[n+j].inode_num = -1;
				clear(DE_ptr[n+j].filename);
			}
		}
	}


	for(i=0;i<1024;i++)
	{
		if(log[i].inode == inode)
		{
			clear(&(log[i].dir[0]));
			clear(&(log[i].parent[0]));
			log[i].inode = -1;
		}
	}

	inode_ptr[parent].size-=16;

	return 1;

}




int rd_readdir(int fd, char *address)
{
	unsigned long off = file_des[fd].offset;	
	unsigned long add;
	int block_num;
	char* PA;
	unsigned long i;
	int  child_inode;
	int  parent_inode;

	//checking if the directory has been opened
	if(file_des[fd].open)
	{
		parent_inode = file_des[fd].inode;

		//checking if the directory has been read
		if(inode_ptr[parent_inode].size <= 0)
		{
			WriteCharacter("The Directory is Empty");
			newline();
			return -1;
		}

		//checking for End of Directory --> Read all the directories
		if(file_des[fd].dir_read_count >= inode_ptr[parent_inode].size)
		{
			WriteCharacter("End of Directory Entries");
			newline();
			return -1;
		}

		//else read the directory entries
		off = file_des[fd].offset;
		block_num =  off/256;
		add = inode_ptr[parent_inode].location[block_num] + (off%256);
		PA = add;
		DE_ptr = add;
		child_inode = DE_ptr->inode_num;
		
		//Finding the next active directoy entry in the block
		while(1)
		{
			//checking for end of block and then change the block
			if( (add%256) == 0  && off != 0 )
			{
				add = inode_ptr[parent_inode].location[block_num++];
				PA = add;
				DE_ptr = add;
				child_inode = DE_ptr->inode_num;
			}

			//checking for the next available open directory entry
			if(child_inode == -1)
			{
				off += 16;
				add += 16;
				DE_ptr = add;
				child_inode = DE_ptr->inode_num;
			}

			//directory found
			else
			{
				off += 16;
				file_des[fd].dir_read_count += 16;
				
				//copying the data to the address specified
				for(i = 0 ; i < 16 ; i++)
				{
					address[i] = PA[i];
				}
				break;
			}
		}	
	}

	//Directory not open
	else
	{
		WriteCharacter("Directory Not Open");
		newline();
		return -1;
	}

	file_des[fd].offset = off; 
	return 1;
}