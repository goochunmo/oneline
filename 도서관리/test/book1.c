#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <process.h>

typedef struct booktype
{
	char book[30];
	char author[15];
	char publisher[10];
	char price[10];
	char year[5];
} book; 

int main()
{

	book bo;
	char ca;

//	FILE *fp;
//	char ch;
	int infd;
	int outfd;

	

	printf("1.insert\n");
	printf("2.list\n");
	printf("3.quit\n");



	
	while(1)
	{	

		scanf("%c", &ca);

		switch(ca)
		{
		
			case '1':
				printf("책:");
				scanf("%s",&bo.book);
				printf("저자:");
				scanf("%s",&bo.author);
				printf("출판사:");
				scanf("%s",&bo.publisher);
				printf("가격:");
				scanf("%s",&bo.price);
				printf("년도:");
				scanf("%s",&bo.year);


				
				break;

			case '2':

				if((outfd = creat("dll-10.out", S_IREAD | S_IWRITE)) < 0)
				{
					printf("file create error!\n");
					exit(-1);
				}
				

				write(outfd,&bo, sizeof(book));
				close(outfd);

				if((infd = open("dll-10.out", O_RDONLY | O_BINARY)) <0)
				{
					printf("file open error!\n");
					exit(-1);
				}
				read(infd,&bo, sizeof(book)) <= sizeof(book);
				printf("\n책 : %s", bo.book);
				printf("\n저자 : %s", bo.author);
				printf("\n출판사 : %s", bo.publisher);
				printf("\n가격 : %s", bo.price);
				printf("\n년도 : %s", bo.year);
				close(infd);
				break;
			
			case '3':
				return 0;
				break;

				
	
		}
	}

	return 0;
}
