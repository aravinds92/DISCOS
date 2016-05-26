#include "functions.h"

void file_system_begin()
{
	int len = 50;
	char y[len];
	char x[len];
	int a;
	int ptr[10];
	int n,i;

	for( n = 0 ; n < len; n++)
	{
		x[n] = 'A';
		y[n] = '\0';
	}


/**********************************************************************************/
							//Test 1 - Create Max number of files/Directory
/**********************************************************************************/

	/*for(i = 0 ; i < 1023; i ++)
	{
		a = rd_mkdir("/sys");
	}
	print_num(a+1);*/
	

/**********************************************************************************/
							//Test 1 - Remove Unlink
/**********************************************************************************/	

	/*a = rd_mkdir("/sys1");
	a = rd_mkdir("/sys2");
	a = rd_mkdir("/sys3");
	a = rd_mkdir("/sys4");
	a = rd_mkdir("/sys5");
	a = rd_mkdir("/sys6");
	a = rd_mkdir("/sys7");
	a = rd_mkdir("/sys8");
	a = rd_mkdir("/sys9");
	a = rd_mkdir("/sys10");
	a = rd_mkdir("/sys11");
	a = rd_mkdir("/sys12");
	a = rd_mkdir("/sys13");
	a = rd_mkdir("/sys14");
	a = rd_mkdir("/sys15");
	a = rd_mkdir("/sys16");
	a = rd_mkdir("/sys17");

	a = rd_unlink("/sys2");
	print_num(a);
	a = rd_unlink("/sys17");
	print_num(a);

	/a = rd_mkdir("/sys");
	print_num(a);
	a = rd_mkdir("/sys/var");
	print_num(a);
	a = rd_unlink("/sys/var");
	print_num(a);
	a = rd_unlink("/sys");
	print_num(a);*/

/**********************************************************************************/
					//Test 2 - 	Generate the Largest Possible File
/**********************************************************************************/	


	/*a = rd_creat("/sys.txt");
	ptr[0] = rd_open("/sys.txt");
	a = rd_write(ptr[0], &x[0], len);
	print_num(a);*/


/**********************************************************************************/
					//Test 3 - 	Lseek to Read
/**********************************************************************************/	

	/*a = rd_creat("/sys.txt");
	ptr[0] = rd_open("/sys.txt");
	a = rd_write(ptr[0], &x[0], len);
	a = rd_lseek(ptr[0], 0);
	a = rd_read(ptr[0], &y[0], len);
	print_num(a);
	newline();
	WriteCharacter(&y[0]);
	newline();
	a =rd_close(ptr[0]);*/

/**********************************************************************************/
					//Test 4 - 	Make and Read Directory Enteries
/**********************************************************************************/	

	/*a = rd_mkdir("/sys");
	a = rd_mkdir("/sys/sys1");
	a = rd_mkdir("/sys/sys2");
	a = rd_mkdir("/sys/sys3");
	a = rd_mkdir("/sys/sys4");
	a = rd_mkdir("/sys/sys5");
	a = rd_mkdir("/sys/sys6");
	a = rd_mkdir("/sys/sys7");
	a = rd_mkdir("/sys/sys8");
	a = rd_mkdir("/sys/sys9");
	a = rd_mkdir("/sys/sys10");
	a = rd_mkdir("/sys/sys11");
	a = rd_mkdir("/sys/sys12");
	a = rd_mkdir("/sys/sys13");
	a = rd_mkdir("/sys/sys14");
	a = rd_mkdir("/sys/sys15");
	a = rd_mkdir("/sys/sys16");
	a = rd_mkdir("/sys/sys17");

	ptr[0] = rd_open("/sys");

	for(i = 1 ; i <=18; i ++ )
	{
		a = rd_readdir(ptr[0], &y[0]);
		WriteCharacter(&y[0]);
		newline();
		clear(&y[0]);
	}	*/	



	return;
}
