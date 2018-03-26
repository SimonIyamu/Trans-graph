#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataTypes.h"

#define	FALSE 0
#define TRUE 1 

int visited[MAXVERTEX];
Vertex start[MAXVERTEX];
Vertex end[MAXVERTEX];
ListNode *L;
int clock,push;



void Connect(Graph *G,Vertex v,Vertex w)
{
	Edge *lastedge, *new;

	new=malloc(sizeof(Edge));
	new->endpoint=w;
	new->nextedge=NULL;
	
	if(G->firstedge[v]==NULL)
		G->firstedge[v]=new;
	else{
		lastedge=G->firstedge[v];
		while(lastedge->nextedge!=NULL)
			lastedge=lastedge->nextedge;
		lastedge->nextedge=new;
	}


}


int txt2graph(char filename[],Graph *G){
	FILE *fp;
	int ch,i=0,found=0,j,newline=0,line=0,firstofline=1,pos,v,letters=1;
	char station[MAX_NAME_LENGTH],previouspos;
	G->n=0;
	

	fp = fopen(filename,"r");
	if(fp == NULL) {
		perror("Error in opening file");
		exit(-1);
	}
	printf("\033[2J\033[1;1H");					//clears screen
	printf("\n\n\t\t\t\t\t\t\t\t..::Train Stations::..");
	while(1){
		ch = fgetc(fp);
		letters++;
		if(ch=='\n'){
			/* In the end of the file, there is a \n always... So we have to make sure its not that case*/
			fgetc(fp);
			if(!feof(fp)){		/*This way I check If the next character is EOF*/
				newline=1;
			}
			/*Now fp must go where it was*/
			if(newline){
				fp=fopen(filename,"r");
				for(j=0 ; j<letters-1 ; j++)
					fgetc(fp);
			}
		}
		if(ch=='-' || ch=='\n' || feof(fp)){
			station[i]='\0';
		
			if(firstofline){
				line++;
				printf("\n\n\t\t\t\t\t\t\t\t\tLine %d:\n",line);
				printf("%s",station);
				strcpy(G->stations[G->n],station);
            ListNode *newL=malloc(sizeof(ListNode));
            newL->v=line;
            newL->next=NULL;
				G->line[G->n]=newL;
				G->firstedge[G->n]=NULL;
				previouspos=G->n;
				G->n++;
				firstofline=0;
				newline=0;
			}	
			else{
				printf(" - %s",station);
				for(v=0; v<G->n ; v++)
					if(!strcmp(G->stations[v],station)){	//Returns 0 when equal
						found=1;
						pos=v;
					}
				if(found){
					Connect(G,previouspos,pos);
					Connect(G,pos,previouspos);
               ListNode *temp,*newL=malloc(sizeof(ListNode));
               newL->v=line;
               newL->next=NULL;
               temp=G->line[pos];
               while(temp->next!=NULL)
                       temp=temp->next;
               temp->next=newL;
					previouspos=pos;
				}
				else{	
					strcpy(G->stations[G->n],station);
               ListNode *newL=malloc(sizeof(ListNode));
               newL->v=line;
               newL->next=NULL;
					G->line[G->n]=newL;
					G->firstedge[G->n]=NULL;
					Connect(G,previouspos,G->n);
					Connect(G,G->n,previouspos);

					previouspos=G->n;
					G->n++;
				}
				if(newline)
					firstofline=1;
				found=0;
			}
			i=0;
			strcpy(station,"");
			if(feof(fp))
				break;
			else continue;	
		}
		station[i++]=ch;
	}
	fclose(fp);
	printf("\n\n\t\t\t\t\t\t\t\t  ::::::::::::::::::\n\n\n");
}

void InsertAtStart(ListNode **h,Vertex v)
{
	ListNode *new,*temp;
	new=malloc(sizeof(ListNode));
	new->v=v;
	new->next=NULL;
	if(*h==NULL)
		*h=new;
	else{
		new->next=*h;
		*h=new;
	}
}

void InsertAtEnd(ListNode **h,Vertex v)
{
	ListNode *new,*temp;
	new=malloc(sizeof(ListNode));
	new->v=v;
	new->next=NULL;
	if(*h==NULL)
		*h=new;
	else{
		temp=*h;
		while(temp->next!=NULL)
			temp=temp->next;
		temp->next=new;
	}

}
/*Returns the last item of the stack,and deletes it*/
Vertex Pop(void){
	ListNode *tmp=L;
	Vertex rtrn=L->v;
	L=L->next;
	free(tmp);
	return rtrn;
}

/* Traverse: recursive traversal of a graph
   Pre: v is a vertex of graph G
   Post: The depth first traversal, using function Visit, has been
   completed for v and for all vertices adjacent to v.
   Uses: Traverse recursively, Visit */
void Traverse(Graph G, Vertex v)
{
  start[v]=clock++;

  Vertex w;
  Edge *curedge;

  visited[v]=TRUE;

  curedge=G.firstedge[v];      /* curedge is a pointer to the first edge (v,_) of V */
  while (curedge){
    w=curedge->endpoint;        /* w is a successor of v and (v,w) is the current edge */
    if (!visited[w]) Traverse(G, w);
    curedge=curedge->nextedge;  /*curedge is a pointer to the next edge (v,_) of V */
  }
  InsertAtStart(&L,v);
  end[v]=clock++;
}

/* DepthFirst: depth-first traversal of a graph
   Pre: The graph G has been created.
   Post: The function Visit has been performed at each vertex of G in depth-first order
   Uses: Function Traverse produces the recursive depth-first order */
ListNode *PathFind(Graph G,char strt[],char tar[])
{
	clock=0;
	Vertex v,startt,target;
	ListNode *Path=NULL, *temp;
	int found=0;

	free(L);
	L=NULL;

	for(v=0; v < G.n; v++)
		if(!strcmp(G.stations[v],strt)){
			found=1;
			startt=v;
		}

	if(!found){
		printf("Invalid departure station\n");
		return NULL;
	}
	found=0;
	for(v=0; v < G.n; v++)
		if(!strcmp(G.stations[v],tar)){
			found=1;
			target=v;
		}
	if(!found){
		printf("Invalid arrival station\n");
      return NULL;
	}
	found=0;

	/*	DFS	*/
	for (v=0; v < G.n; v++)
		visited[v]=FALSE;
	Traverse(G, startt);

	temp=L;
	while(temp!=NULL){
		if(temp->v==target) found=1;
		temp=temp->next;
	}
	temp=L;
	if(found){
		while(temp!=NULL){
			v=temp->v;
			if(end[v]>=end[target] && start[v]<=start[target])	/* The vertexes that satisfy this statement are the vertexes*/
										/* of the path, so we copy them to the Path List*/
				InsertAtEnd(&Path,v);
			temp=temp->next;
		}
		return Path;
	}
	else
		return NULL;
}


void PrintList(ListNode *h,Graph G)
{
   if(h==NULL)
      return;
   printf("\n\n\t\t    _____                . . . . . o o o o o\n");
   printf("\t\t  __|[_]|__ ___________ _______    ____      o\n");
   printf("\t\t |[] [] []| [] [] [] [] [_____(__  ][]]_n_n__][.\n");
   printf("\t\t_|________|_[_________]_[________]_|__|________)<\n");
   printf("\t\t  oo    oo 'oo      oo ' oo    oo 'oo 0000---oo");  putchar(92); printf("\n");
   printf("\t\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	ListNode *temp=h->next;
	printf("%s ",G.stations[h->v]);
	while(temp!=NULL){
		printf("-> %s ",G.stations[temp->v]);
		temp=temp->next;
	}
	putchar(10);
}

void TryPrintList(ListNode *h,Graph G)
{
	ListNode *temp=h->next;
	char prevstation[MAX_NAME_LENGTH],prevline,changeline;
	if(h==NULL){
		printf("These stations are not connected\n");
		return;
	}
	printf("\nEmbark on %s station of line %d, ",G.stations[h->v],G.line[h->v]->v);
	strcpy(prevstation,G.stations[h->v]);
	prevline=G.line[h->v]->v;
	while(temp!=NULL){
      changeline=1;
		temp=temp->next;
		if(temp!=NULL){
         
         ListNode *temp2=G.line[temp->v];
         while(temp2!=NULL){
            if(temp2->v==prevline){
               changeline=0;
            }
            temp2=temp2->next;
         }

			if(changeline){//if(G.line[temp->v]!=prevline){
				printf("and disembark on %s station.\n",prevstation);       /*mporei to of line na to valw sto disembark*/
				//if(temp->next!=NULL)
				   printf("Embark on %s station of line %d,",prevstation,G.line[temp->v]->v);
			}
			prevline=G.line[temp->v]->v;  //mallon
			strcpy(prevstation,G.stations[temp->v]);
		}
	}
	if(!strcmp(prevstation,G.stations[h->v]))
		printf("and disembark on %s station.\n\n",G.stations[h->next->v]);
	else
		printf("and disembark on %s station.\n\n",prevstation);
}

/* PrintElement: prints the value of its parameter x */
void PrintElement(Vertex x)
{
  printf("%d ", x);
}
