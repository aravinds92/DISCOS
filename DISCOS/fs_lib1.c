#include "functions.h"

int rd_mkdir(char* pathname)				//file.char_ptr[i] = file.char_ptr[i] || 0xf0;
{
	int inode, parent;
	unsigned long block_temp, block;
	int i = 0,k,m;
	if(files_num==1024)
	{	
		WriteCharacter("Maximum nuber of files created");
		return -1;
	}


	inode = find_inode();
	block = find_block();
	char temp[14];
	char parent_name[14];
	parent_name[0] = '/';

	//newline();

	while(1)
	{
		if(pathname[i] == '/')
		{
			k = i+1;
			m=0;
			while(pathname[k] != '\0' && pathname[k] != '/')			//pathname[k] != '/'
			{
				temp[m]=pathname[k];
				m++;
				k++;
			}
			temp[m] = '\0';
			
			if(pathname[k] == '\0')
			{			
				/*for(i=0;i<inode_ptr[parent].size;i+=16)
				{
					DE_ptr = (inode_ptr[parent].location[i/256] + i%256);
					if(strcmp(&DE_ptr->filename[0],&temp[0]) == 0)
					{
						WriteCharacter("Directory already exists");
						newline();
						return -1;
					}
				}*/
				strcpy(&(log[files_num].dir[0]), &temp[0]);
				strcpy(&(log[files_num].parent[0]), &parent_name[0]);
				log[files_num].inode = inode;
				files_num++;
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
				//inode_ptr[parent].size += 16;
			}
		}	
		
		i = k;
	}
	

	inode_ptr[parent].size += 16;
	block_temp = allocate_block(parent);
	DE_ptr = block_temp;
	strcpy(&(DE_ptr->filename[0]) , &temp[0]);
	DE_ptr->inode_num = inode;

	//Updating the INODE of the directory being created
	inode_ptr[inode].type ='0';
	inode_ptr[inode].size = 0;
	inode_ptr[inode].location[0] = block;
	for(i=1; i < 10;i++)
		inode_ptr[inode].location[i] = 0;

	for(i =0;i<14;i++)
	{
		temp[i] = '\0';
		parent_name[i] = '\0';
	}

	return	0;
}


int rd_creat(char* pathname)	
{
	int inode, parent;
	unsigned long block_temp, block;
	int i = 0,k,m;

	//newline();

	if(files_num==1024)
	{	
		WriteCharacter("Maximum nuber of files created");
		newline();
		return -1;
	}

	inode = find_inode();
	block = find_block();
	char temp[14];
	char parent_name[14];

	parent_name[0] = '/';

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
				strcpy(&(log[files_num].dir[0]), &temp[0]);
				strcpy(&(log[files_num].parent[0]), &parent_name[0]);
				log[files_num].inode = inode;
				files_num++;
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
	

	inode_ptr[parent].size += 16;		

	//updating the parent directory inode
	block_temp = allocate_block(parent);
	

	DE_ptr = block_temp;
	strcpy(&(DE_ptr->filename[0]) , &temp[0]);
	DE_ptr->inode_num = inode;
	

	//Updating the INODE of the directory being created
	inode_ptr[inode].type = '1';
	inode_ptr[inode].size = 0;
	inode_ptr[inode].location[0] = block;
	
	for(i=1; i < 10;i++)
		inode_ptr[inode].location[i] = 0;

	for(i=0;i<14;i++)
	{
		temp[i] = '\0';
		parent_name[i] = '\0';
	}
	
	return 0;
}


int rd_open(char* pathname)
{
	char temp[14];
	char parent_name[14];
	int inode, parent;
	int i=0, k, m;
	int free_fd;
	

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
				inode = get_inode_num(&temp[0] , &parent_name[0]);
				if(inode == -1)
				{
					WriteCharacter("File Does Not Exist");
					newline();
					return -1;
				}
				break;
			}
			
			if(pathname[k] == '/')
			{
				parent = get_inode_num(&temp[0] , &parent_name[0]);
				if(parent == -1)
				{
					WriteCharacter("No Directory Found");
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

	free_fd = find_fd();
	if(free_fd == -1)
	{
		WriteCharacter("No file descriptor available");
		newline();
		return -1;
	}
	file_des[free_fd].inode = inode;
	file_des[free_fd].open = 1;
	file_des[free_fd].offset = 0;
	file_des[free_fd].parent_inode = parent;
	open_files_count++;

	for(i=0;i<14;i++)
	{
		temp[i] = '\0';
		parent_name[i] = '\0';
	}

	return free_fd;
	//return inode;
}


int rd_close(int n)
{
	if(file_des[n].open == 0)
	{
		WriteCharacter("File descriptor not open");
		newline();
		return -1;
	}

	file_des[n].inode = -1;
	file_des[n].offset = 0;
	open_files_count--;
	return 1;
}

