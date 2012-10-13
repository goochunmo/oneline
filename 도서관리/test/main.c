#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int mian()
{
	FILE *fp;
	char ch;
	
	printf("please enter <$> to finish data input.\n");
	fp = fopen("dll-1.dat","w");
	while((ch = getche()) !=  '$')
		{
			putc(ch, fp);
		}
	fclose (fp);

	printf("\nthe contents of dll-1.dat.\n");
	if((fp = fopen("dll-1.dat","r")) == NULL)
	{
		printf("can not open %s file.\n","dll-1.dat");
		exit(-1);
	}
	while((ch =getc(fp)) != EOF)
	{
	printf("%c",ch);
	}
	fclose(fp);
}