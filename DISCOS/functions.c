#include "functions.h"

void WriteCharacter(char *c)
{
	char forecolour=0x0F;
	char backcolour=0x00;
	int index = 0;
	char *video_buffer;
	char attrib;
	
	while(c[index])
	{
		if(pos_x>=80)
		{
			pos_x=0;
			pos_y=pos_y+1;
		}
		if(pos_y>25)
			break;
		//now print it
		attrib = (backcolour << 4) | (forecolour & 0x0F);
		video_buffer = (char *)0xB8000 + (pos_y * 80 + pos_x) ;
		*video_buffer = attrib;
		*video_buffer++ = c[index];
		index++;
		pos_x = pos_x + 2;
	}
	
     return;
}

void print_num(unsigned long mem)
{
	int j=0,n[10],m;
	char *ch="";
	
	
	while(mem>0)
	{
		n[j] = mem%10;
		mem = mem/10;
		j++;
	}
	for(m=j-1;m>=0;m--)
	{
		*ch = n[m] + '0';
		WriteCharacter(ch);
		//pos_x= pos_x -2;
	}

	pos_x = 0;
	pos_y +=1;	

return;
}


int filesys_init(void)
{
	int i;
	unsigned int root_inode;
	unsigned long block_num;
	for(i=2;i<twoMB;i++)
	{
		file.short_ptr[i] = 0;
	}

	file.short_ptr[0] = 7931;									//Number of blocks available to store data initially
	file.short_ptr[1] = initial_inode_cnt;						//Number of Inode structures available initially

	inode_ptr = file.char_ptr + 256;
	filesys_ptr = file.char_ptr + bitmap_limit;

	root_inode = find_inode();
	block_num = find_block();

	inode_ptr[root_inode].type = '0';
	inode_ptr[root_inode].size = 0;
	inode_ptr[root_inode].location[0] = block_num;
	for(i=1;i<10;i++)
		inode_ptr[root_inode].location[i] = 0;

	for(i=0;i<1024;i++)
	{
		file_des[i].open = 0;
		file_des[i].inode = -1;
		file_des[i].offset = 0;
		write_pos[i] = -16;
	}

	open_files_count = 0;

	log[root_inode].parent[0] = "\0";
	log[root_inode].dir[0] = "/";
	log[root_inode].inode = 0;
	files_num = 1;

	file_system_begin();

	return 1;
}

unsigned long find_block()												//Finds a free block from the memory. Returns -1 if unsuccesful 
{																//and the block nuber if successful
	unsigned int i=bitmap_offset;
	unsigned int cnt = 0;
	unsigned long ret = 0;
	unsigned char and=0x80;
	unsigned int iter_no = 0;
	unsigned int n = file.char_ptr[i] & and;
	//print_num(file.char_ptr[i]+1);
	while(1)														//Breaks out of this loop if block found or all the blocks are full
	{
		iter_no++;
		if(file.char_ptr[i]!=0xff)
		{
			cnt = 0;
			and = 0x80;
			while(cnt<8)											//Loop at each byte 8 times
			{
				n = file.char_ptr[i] & and;
				if(n == 0)				
				{				
					ret = (iter_no-1)*8 + cnt;
					file.char_ptr[i] = (file.char_ptr[i] | and);
					//print_num(file.char_ptr[i]);
					ret = file.char_ptr + fs_offset + ret*256;
					file.short_ptr[0]--;
					return ret;
				}
				else
				{
					and = and>>1;
					cnt++;
				}
			}
		}
		else
		{
			i++;
			if(i>bitmap_limit)
				return -1;
		}

	}

}

void newline()
{
	pos_x=0;
	pos_y+=1;
	return;
}


int find_inode()
{
	unsigned char i=file.char_ptr + 5;
	unsigned int ret=0,cnt = 0;
	unsigned char and=0x80;
	unsigned int iter_no = 0;
	unsigned int n = file.char_ptr[i] & and;
	
	while(1)														//Breaks out of this loop if block found or all the blocks are full
	{
		iter_no++;
		if(file.char_ptr[i]!=0xff)
		{
			cnt = 0;
			and = 0x80;
			while(cnt<8)											//Loop at each byte 8 times
			{
				n = file.char_ptr[i] & and;
				if(n == 0)				
				{				
					ret = (iter_no-1)*8 + cnt;
					file.char_ptr[i] = (file.char_ptr[i] | and);
					file.short_ptr[1]--;
					return ret;
				}
				else
				{
					and = and>>1;
					cnt++;
				}
			}
		}
		else
		{
			i++;
			if(i>bitmap_limit)
				return -1;
		}

	}
}

int strcmp(char * a, char *b)
{
	int ret = 0;
	int i=0;

	int a_len=strlen(a);
	int b_len=strlen(b);

	if(a_len != b_len)
		return 1;

	while(i < a_len)
	{	
		if(a[i] != b[i])
		{	
			ret = 1;
			break;
		}
		i++;
	}

	return ret;
}

 int strlen(char *a)
 {
 	int i=0;

 	while(a[i] != '\0')
 	{
 		i++;
 	}

 	return i;
 }

void strcpy(char *a, char *b)
{
	int i =0;

	while(b[i] != '\0')
	{
		a[i] = b[i];
		i++;
	}
	return;
 }

int get_inode_num(char *temp, char* parent)
{
	int i = 0;

	
	for(i =0; i < files_num ; i++)
	{
		if(strcmp(temp, &(log[i].dir[0])) == 0)
		{
			if(strcmp(parent, &(log[i].parent[0])) == 0)
			{
				return log[i].inode;
			}
		}
	}
	
	return -1;
}

unsigned long allocate_block(int inode_num)
{	
	long size = inode_ptr[inode_num].size;
	//print_num(size);
	unsigned long new_block;
	int new_inode;
	int loc;

	loc = inode_ptr[inode_num].size/256;

	if((size!=0) && (size%256 == 0))  
	{
		new_block = find_block();
		write_pos[inode_num] = 0;
		if(new_block == -1)
		{
			WriteCharacter("No more memory available");
			newline();
			return -1;
		}
		
		inode_ptr[inode_num].location[loc] = new_block;
		inode_ptr[inode_num].size+=16;
		return inode_ptr[inode_num].location[inode_ptr[inode_num].size/256];
	}

	else if(write_pos[inode_num]<256)
	{
		write_pos[inode_num] += 16;
		return (inode_ptr[inode_num].location[loc] + write_pos[inode_num]);
	} 

}


int find_fd(void)
{
	int i;
	for(i=0;i<1024;i++)
	{
		if(file_des[i].open==0)
			return i;
	}
	return -1;
}


void clear(char* ptr)
{
	int i,len;
	len = strlen(ptr);
	for(i=0;i<len;i++)
	{
		ptr[i] = '\0';
	}
	return;
}

int find_fd_from_inode(int inode)
{
	int i;
	for(i=0;i<1024;i++)
	{
		if(file_des[i].inode == inode)
		{
			return i;
		}
	}

	return -1;
}

void free_block(unsigned long block)
{
	unsigned int i=bitmap_offset;
	block = block/256;
	block = block-261;
	int n = block/8;
	int off = block%8;
	off = ~off;
	i = i + n;
	file.char_ptr[i] = 	(file.char_ptr[i] & off);
	return;
}

void free_inode(int inode)
{
	unsigned int i= file.char_ptr + 5;;
	int n = inode/8;
	int off = inode%8;
	off = ~off;
	i = i + n;
	file.char_ptr[i] = 	(file.char_ptr[i] & off);
	return;
}