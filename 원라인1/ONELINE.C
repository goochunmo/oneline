#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#define		F10		68		// 2byte, 0
#define		ESC		27		// 1byte
#define		RIGHT	77		// 2byte, e0
#define		LEFT	75		// 2byte, e0
#define		ENTER	13		// 1byte

typedef struct _char_tag 
{
	char character; // 입력받은 하나의 문자
	int char_number; // 각 행에서 문자의 일련번호
	struct char_tag *prev; // 앞의 char_tag 구조체(앞의 문자)를 가리키는 포인터
	struct char_tag *next; // 다음 char_tag구조체(다음 문자)를 가리키는 포인터
}char_tag;

//===================================================전역변수
int x = 0;
int y = 0;
char ch;
char attr;
char far *location;
void *test;
int cursor_x = 4;
int cursor_y = 11;
char_tag *head = NULL;

//===================================================
void wm();
void VAG_inverse_attrib(unsigned char far *attrib);
void VGA_inverse_bar(int x, int y, int length);
void Option(unsigned char);
void move_cursor (int page, int x, int y);
void cursor_off();
void cursor_on();
char_tag *double_linked_list(unsigned char);
void text_out_test(char_tag *);
//==================================================

int main()
{
	unsigned char Key_temp;
	

	clrscr(); /*초기화*/
	wm();
	
	while(1)
	{

		move_cursor (0, cursor_x, cursor_y);
		Key_temp = getch();
		if(Key_temp == 0)
		{
			Key_temp = getch();
		}
		switch(Key_temp)
		{
			case F10 :
				cursor_off();
				gettext(1,1,80,25, location);
				Option(Key_temp);
				puttext(1,1,80,25, location);
				cursor_on();
				break;

			case RIGHT :
				if(cursor_x < 74)
				{
					cursor_x++;
				}
				
				break;

			case LEFT :
				if(cursor_x > 4)
				{
					cursor_x--;
				}
				break;

			default :
				head = double_linked_list(Key_temp);
				text_out_test(head);
				break;


		}
	}
	

	getch();
	return 0;
}

char_tag *double_linked_list(unsigned char Key_temp)
{

	char_tag *newnode;
	char_tag *current = head;
	char_tag *temp;

	newnode = (char_tag *)malloc (sizeof(char_tag));

	newnode-> character = Key_temp;
	newnode-> char_number = cursor_x;
	newnode-> prev = NULL;
	newnode-> next = NULL;

	if(head == NULL)
	{
		head = newnode;
	}
	else
	{
		while(current->next != NULL) 
		{
			current = current->next;
		}
		current->next = newnode;
	}
	return head;

}
void text_out_test(char_tag *head)
{
	char_tag *head_temp = head;
	int cnt;

	for(cnt = 0;head_temp -> character!= 0 ;cnt++)
	{
		attr = 7;
		location = 0xb8000000 + cursor_y * 160 + cursor_x * 2 ;
		*location++ = head_temp->character;
		*location = attr;
		head_temp = head_temp -> next;
	}
	cursor_x++;
}
void move_cursor (int page, int x, int y)
{ 
	union REGS regs;
	regs.h.ah =2;
	regs.h.dh = y;
	regs.h.dl= x;
	regs.h.bh = page;
	int86(0x10, &regs, &regs);
}

void cursor_off()
{ 
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0x20;
	regs.h.cl = 0;
	int86(0x10, &regs, &regs);
}

void cursor_on()
{ 
	union REGS regs;
	regs.h.ah = 1;
	regs.h.ch = 0x0B;
	regs.h.cl = 0x0C;
	int86(0x10, &regs, &regs);
}

void Option(unsigned char Key_temp)
{
	int Key_cnt = 0;

	while(1)
	{
		if(ESC == Key_temp)
		{
			break;
		}
		else if(F10 == Key_temp)							// NEW
		{
			Key_cnt++;
			VGA_inverse_bar(8,3,3); /*x좌표,y좌표,글자길이*/
			Key_temp = getch();
			if((Key_temp == 0)||(Key_temp == 0xe0))
			{
				Key_temp = getch();
			}
		}
		else if((RIGHT == Key_temp)&&(Key_cnt == 1))		// 오른쪽에 load
		{
			wm();
			VGA_inverse_bar(15,3,4);
			Key_temp = getch();
			Key_cnt++;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		else if((LEFT == Key_temp)&&(Key_cnt == 2))		// 왼쪽
		{
			wm();
			VGA_inverse_bar(8,3,3); 
			Key_temp = getch();
			Key_cnt--;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		
		else if((RIGHT == Key_temp)&&(Key_cnt == 2))		// 오른쪽에 save
		{
			wm();
			VGA_inverse_bar(24,3,4);
			Key_temp = getch();
			Key_cnt++;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}

		else if((LEFT == Key_temp)&&(Key_cnt == 3))		// 오른쪽에 save
		{
			wm();
			VGA_inverse_bar(15,3,4);
			Key_temp = getch();
			Key_cnt--;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}


		else if((RIGHT == Key_temp)&&(Key_cnt == 3))		// 오른쪽에 save as
		{
			wm();
			VGA_inverse_bar(33,3,7);
			Key_temp = getch();
			Key_cnt++;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		else if((LEFT == Key_temp)&&(Key_cnt == 4))		// 오른쪽에 save as
		{
			wm();
			VGA_inverse_bar(24,3,4);
			Key_temp = getch();
			Key_cnt--;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}

		else if((RIGHT == Key_temp)&&(Key_cnt == 4))	// EXIT
		{
			wm();
			VGA_inverse_bar(45,3,4);
			Key_temp = getch();
			Key_cnt++;
			if(Key_temp == ENTER)
			{
				exit(1);
			}
			else if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		else if((LEFT == Key_temp)&&(Key_cnt == 5))	// EXIT
		{
			wm();
			VGA_inverse_bar(33,3,7);
			Key_temp = getch();
			Key_cnt--;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		else if((RIGHT == Key_temp)&&(Key_cnt == 5))	
		{
			wm();
			VGA_inverse_bar(8,3,3);	
			Key_temp = getch();
			Key_cnt=1;
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		else if((LEFT == Key_temp)&&(Key_cnt == 1))	// EXIT
		{
			wm();
			VGA_inverse_bar(45,3,4);
			Key_cnt=5;
			
			Key_temp = getch();
			if(Key_temp == ENTER)
			{
				exit(1);
			}
			else if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}
		else
		{	
			Key_temp = getch();
			if(Key_temp == 0)
			{
				Key_temp = getch();
			}
		}

	}

	return ;
}

void VAG_inverse_attrib(unsigned char far *attrib)
{
	unsigned char origin_attrib;
	origin_attrib = *attrib;
	*attrib >>= 4;
	*attrib = *attrib & 0x0f;
	origin_attrib <<= 4;
	*attrib = *attrib | origin_attrib;
}

void VGA_inverse_bar(int x, int y, int length)
{
	int i = 0;
	unsigned char far *attr_memory = (unsigned char far *) 0xb8000001L;
	attr_memory = attr_memory + y * 160 + x * 2;
	for(i = 0; i < length; i++)
	{
		VAG_inverse_attrib(attr_memory);
		attr_memory += 2;
	}
}
void wm()
{
	for(x = 75; x < 76; ++x)
	{
		for(y = 2; y < 5; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 187;
	attr = 7;
	location = 0xb8000000 + 1 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;
	
	ch = 188;
	attr = 7;
	location = 0xb8000000 + 5 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;


	/*좌측 상단*/

	
	for(x = 3; x < 4; ++x)
	{
		for(y = 2; y < 5; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 201;
	attr = 7;
	location = 0xb8000000 + 1 * 160 + 3 * 2 ;
	*location++ = ch;
	*location = attr;
	

	ch = 200;
	attr = 7;
	location = 0xb8000000 + 5 * 160 + 3 * 2 ;
	*location++ = ch;
	*location = attr;

	/*좌측 하단*/
	ch = 200;
	attr = 7;
	location = 0xb8000000 + 24 * 160 + 0 * 2 ;
	*location++ = ch;
	*location = attr;
	/*우측 하단*/
	ch = 188;
	attr = 7;
	location = 0xb8000000 + 24 * 160 + 79 * 2 ;
	*location++ = ch;
	*location = attr;
	
	/*하단*/

	for(x = 1; x < 79; ++x)
	{
		for(y = 24; y < 79; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	/*상단*/
	for(x = 1; x < 79; ++x)
	{
		for(y = 0; y < 1; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	
	
	/*상단 밑*/
	for(x = 4; x < 75; ++x)
	{
		for(y =1; y < 2; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
		
	for(x = 4; x < 75; ++x)
	{
		for(y = 5; y < 6; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	/*오른쪽*/
	for(x = 79; x < 80; ++x)
	{
		for(y = 1; y < 24; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 187;
	attr = 7;
	location = 0xb8000000 + 0 * 160 + 79 * 2 ;
	*location++ = ch;
	*location = attr;

	/*왼쪽*/
	for(x = 0; x < 1; ++x)
	{
		for(y = 1; y < 24; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}

	ch = 201;
	attr = 7;
	location = 0xb8000000 + 0 * 160 + 0 * 2 ;
	*location++ = ch;
	*location = attr;
			
	/*중앙*/
	for(x = 4; x < 75; ++x)
	{
		for(y =7; y < 8; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}

	for(x = 4; x < 75; ++x)
	{
		for(y = 15; y < 16; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	
	/*중앙 왼쪽*/
	for(x = 3; x < 4; ++x)
	{
		for(y = 8; y < 15; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 201;
	attr = 7;
	location = 0xb8000000 + 7 * 160 + 3 * 2 ;
	*location++ = ch;
	*location = attr;

	ch = 200;
	attr = 7;
	location = 0xb8000000 + 15 * 160 + 3 * 2 ;
	*location++ = ch;
	*location = attr;

	/*중앙 오른쪽*/
	for(x = 75; x < 76; ++x)
	{
		for(y = 8; y < 15; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 187;
	attr = 7;
	location = 0xb8000000 + 7 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;

	ch = 188;
	attr = 7;
	location = 0xb8000000 + 15 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;
	/*중앙 밑상단*/
	for(x = 4; x < 75; ++x)
	{
		for(y =18; y < 19; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}

	for(x = 4; x < 75; ++x)
	{
		for(y = 15; y < 16; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	/*중앙밑 오른쪽*/
	for(x = 75; x < 76; ++x)
	{
		for(y = 19; y < 22; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 187;
	attr = 7;
	location = 0xb8000000 + 18 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;

	ch = 188;
	attr = 7;
	location = 0xb8000000 + 22 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;
	/*중앙밑 오른쪽*/
	for(x = 75; x < 76; ++x)
	{
		for(y = 19; y < 22; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 187;
	attr = 7;
	location = 0xb8000000 + 18 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;

	ch = 188;
	attr = 7;
	location = 0xb8000000 + 22 * 160 + 75 * 2 ;
	*location++ = ch;
	*location = attr;

	/*중앙 밑하단*/
	for(x = 4; x < 75; ++x)
	{
		for(y =22; y < 23; ++y)
		{
			ch = 205;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	/*중앙밑 왼쪽*/
	for(x = 3; x < 4; ++x)
	{
		for(y = 19; y < 22; ++y)
		{
			ch = 186;
			attr = 7;
			location = 0xb8000000 + y * 160 + x * 2 ;
			*location++ = ch;
			*location = attr;
		}
	}
	ch = 201;
	attr = 7;
	location = 0xb8000000 + 18 * 160 + 3 * 2 ;
	*location++ = ch;
	*location = attr;
	

	ch = 200;
	attr = 7;
	location = 0xb8000000 + 22 * 160 + 3 * 2 ;
	*location++ = ch;
	*location = attr;

	/*New*/
	ch = 78;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 8 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 101;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 9 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 119;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 10 * 2 ;
	*location++ = ch;
	*location = attr;
	
	/*Load*/
	ch = 76;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 15 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 111;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 16 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 97;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 17 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 100;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 18 * 2 ;
	*location++ = ch;
	*location = attr;

	/*Save*/
	ch = 83;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 24 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 97;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 25 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 118;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 26 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 101;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 27 * 2 ;
	*location++ = ch;
	*location = attr;

	/*Save as*/
	ch = 83;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 33 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 97;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 34 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 118;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 35 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 101;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 36 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 37 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 97;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 38 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 115;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 39 * 2 ;
	*location++ = ch;
	*location = attr;

	/*Exit*/
	ch = 69;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 45 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 120;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 46 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 105;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 47 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 116;
	attr = 7;
	location = 0xb8000000 + 3 * 160 + 48 * 2 ;
	*location++ = ch;
	*location = attr;

	/*f10*/
	ch = 70;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 12 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 49;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 13 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 48;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 14 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 15 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 45;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 16 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 17 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 77;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 18 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 101;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 19 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 110;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 20 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 117;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 21 * 2 ;
	*location++ = ch;
	*location = attr;

	/*Alt + x - Exit*/
	ch = 65;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 26 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 108;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 27 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 116;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 28 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 29 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 43;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 30 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 31 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 120;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 32 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 33 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 45;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 34 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 35 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 69;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 36 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 120;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 37 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 105;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 38 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 116;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 39 * 2 ;
	*location++ = ch;
	*location = attr;

	/*F1 - Help*/
	ch = 70;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 45 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 49;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 46 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 47 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 45;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 48 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 32;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 49 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 72;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 50 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 101;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 51 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 108;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 52 * 2 ;
	*location++ = ch;
	*location = attr;
	ch = 112;
	attr = 7;
	location = 0xb8000000 + 20 * 160 + 53 * 2 ;
	*location++ = ch;
	*location = attr;
	
}





