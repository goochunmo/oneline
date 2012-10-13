#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <process.h>

#define star "********************************"

void LIST(void);
void INSERT(void);
void FIle_check(int);
void HEADER_write(void);

typedef struct booktype
{
	char book[30];
	char author[15];
	char publisher[15];
	char price[15];
	char year[15];

} book; 

typedef struct bk
{
	int byte;
	char data[32];

}bk;


int infd;
int outfd;

int main()
{
	char ca;

	int count = 0;

	infd = open("book.DBF", S_IREAD | S_IWRITE);
	FIle_check(infd);
	close(infd);
	
	while(1)
	{
		printf("1.insert\n");
		printf("2.list\n");
		printf("3.quit\n");
		scanf("%c", &ca);
		
		switch(ca)
		{
		
			case '1':
				INSERT();
				fflush(stdin);
				break;

			case '2':
				LIST();
				fflush(stdin);
				break;
			
			case '3':
				printf("���� ����� !!\n");
				return 0;
				
			default:
				printf("error\n");
				break;	
		}
	}

	return 0;
}



void INSERT(void)
{
	book bo={{0,},}; // ����ü ���ϳ��� �����Ⱚ û�����ش�
	bk buff={{0,},};
	HEADER_write();


	infd = open("book.DBF",O_WRONLY|O_APPEND|O_BINARY);
	FIle_check(infd);

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

	write(infd,&bo, sizeof(book));
	write(infd, "|", 1);
	close(infd);


	infd = open("book.DBF",O_RDWR|O_BINARY);
	FIle_check(infd);
	lseek(infd,0,SEEK_SET); // ���� �����ٿ��� ����
	read(infd,&buff,sizeof(bk));
	buff.byte = buff.byte + 1;
	lseek(infd,0,SEEK_SET);
	write(infd,&buff,sizeof(bk));
	printf("���ڵ尹�� = [%d]\n", buff.byte);


	close(infd);
	return ;
}

void LIST(void)
{
	char ca=0;
	int ia;
	book bo = {{0,},};

	outfd = open("book.DBF",O_RDONLY|O_BINARY);
	FIle_check(outfd);
	lseek(outfd, sizeof(bk)+1,SEEK_SET);
	while((read(outfd,&bo,sizeof(book)) > 0)&&(ca != 'q'))
	{
		ca = getch();
		if(ca == ' ')
		{
			printf("\nå : %s", bo.book);
			printf("\n���� : %s", bo.author);
			printf("\n���ǻ� : %s", bo.publisher);
			printf("\n���� : %s", bo.price);
			printf("\n�⵵ : %s\n", bo.year);
			lseek(outfd, 1,SEEK_CUR); // ���ϳ��� ������ġ���� ����
		}
		if(ca == 'q')
		{
			break;
		}
	}
	close(outfd);

}

void HEADER_write(void)
{
	bk H = {0,"********************************"};
	bk buff;
	int Check;
	infd = open("book.DBF",O_RDWR|O_BINARY);
	FIle_check(infd);
	read(infd, &buff,sizeof(bk));

	Check = strncmp(buff.data,star,sizeof(buff.data));
	if(0 != Check) //�ѹ��� ���� �ϰ� �Ѵ� ���Ͼȿ� *�� ������� �۵��Ѵ�
	{
		lseek(infd, 0,SEEK_SET);
		write(infd,&H, sizeof(bk));
	}


	write(infd, "|", 1);
	close(infd);

	return ;
}

void FIle_check(int FILE_num)
{
	if(FILE_num == 0)
	{
		printf("file create error!\n");
		exit(-1);
	}
	return ;

}
