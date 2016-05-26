#define twoMB 2*1024*1024			//Size in bytes
#define indoe_offset 1 
#define initial_inode_cnt 1024
#define number_of_blocks 8192
#define bitmap_limit 66816
#define bitmap_offset 258*256
#define fs_offset 262*256
#define NULL 0


int pos_x,pos_y;

typedef struct __attribute__((packed))
{
	unsigned char type;
	unsigned long size;
	unsigned long location[10];
}inode;

inode *inode_ptr;


typedef struct
{
	char filename[14];
	short inode_num;
}dir_entry;

dir_entry *DE_ptr;

typedef struct
{
	char dir[14];
	int inode;	
	char parent[14];
}dir_log;

dir_log log[1024];

int files_num;

typedef struct
{
	int inode;
	int open;
	unsigned long offset;
	int parent_inode;
	int dir_read_count;
}file_descriptor;

file_descriptor file_des[1024];

long write_pos[1024];

typedef union 
{
	unsigned char* char_ptr;
	unsigned short* short_ptr;
	unsigned long* long_ptr;
}file_sys;

file_sys file;


unsigned char* filesys_ptr;				//Offset to the beginning of the file system
unsigned long open_files_count;


//Utility function
void WriteCharacter(char *);
void newline();
void print_num(unsigned long);
int filesys_init(void);
unsigned long find_block();
int find_inode();
int rd_mkdir(char *);
void file_system_begin();
int rd_mkdir(char*);
int rd_creat(char*);
int strcmp(char * , char *);
int strlen(char *);
void strcpy(char *, char *);
int get_inode_num(char *, char *);
void clear(char*);
int find_fd_from_inode(int);
void free_block(unsigned long block);
void free_inode(int inode);
unsigned long allocate_block(int);
int find_fd();


//File system function
int rd_open(char*);
int rd_close(int);
int rd_read(int fd, char *address, int num_bytes);
int rd_write(int fd, char *address, int num_bytes);
int rd_lseek(int fd, int offset);
int rd_unlink(char *pathname);
int rd_readdir(int fd, char *address);
