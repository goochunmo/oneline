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
				printf("å:");
				scanf("%s",&bo.book);
				printf("����:");
				scanf("%s",&bo.author);
				printf("���ǻ�:");
				scanf("%s",&bo.publisher);
				printf("����:");
				scanf("%s",&bo.price);
				printf("�⵵:");
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
				printf("\nå : %s", bo.book);
				printf("\n���� : %s", bo.author);
				printf("\n���ǻ� : %s", bo.publisher);
				printf("\n���� : %s", bo.price);
				printf("\n�⵵ : %s", bo.year);
				close(infd);
				break;
			
			case '3':
				return 0;
				break;

				
	
		}
	}

	return 0;
}
