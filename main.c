#include <stdio.h>
#include "DataTypes.h"
#include "MetroInterface.h"

int main(int argc,char *argv[]){ 
	Graph Athens;
	char start[MAX_NAME_LENGTH],target[MAX_NAME_LENGTH],ch;
	do{
		txt2graph(argv[1],&Athens);
		printf("Insert the departure and arrival stations\n\n");
		scanf("%s %s",start,target);	
		PrintList(PathFind(Athens,start,target),Athens);
		getchar();	/* ignore \n */
		printf("\n\nPress q to exit or a to try another journey...\t");
      scanf("%c",&ch);
      while(ch!='a' && ch!='q'){
         getchar();
         printf("\nPlease press q or a...\t");
         scanf("%c",&ch);
      }
	}while(ch=='a');
	printf("\033[2J\033[1;1H");					//clears screen
	return 0;
}
